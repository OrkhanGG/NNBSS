#include "Hardware.h"
///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

// Note, this file contains so much special for Windows OS.
// Crossplatform version of this application must consider this part!

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

// Standard C Lib
#include "dshow.h"
#include <atlbase.h>
#include <strmif.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <intrin.h> // NOTE this header is MSVC specific!
#include <array>
// wxWidgets
#include <wx/log.h>

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "quartz.lib")
#pragma comment(lib,"user32.lib")

// Use to convert bytes to MB
#define DIV_BYTE_TO_MB 1048576
#define DIV_NORMALIZER 1024

// Specify the width of the field in which to print the numbers. 
// The asterisk in the format specifier "%*I64d" takes an integer 
// argument and uses it to pad and right justify the number.
#define INFO_FIELD_WIDTH 7

//#define _WIN32_WINNT  0x0501 // I misunderstand that(compiler warns about redefinition)
#include <cstdlib>

CNNBSSHardware::CNNBSSHardware()
{
	if (CNNBSSHardwareUIThreadAddressHandle(1000L, false)->Run() != wxTHREAD_NO_ERROR)
		wxLogError("Could not create the worker thread needed to catch i/o of devices!");
}

CNNBSSHardware::~CNNBSSHardware()
{
	CNNBSSHardwareUIThreadAddressHandle(0L, true);
}

// Get CPU Name&Info
std::string CNNBSSHardware::GetCpuInfo()
{
	// 4 is essentially hardcoded due to the __cpuid function requirements.
	// NOTE: Results are limited to whatever the sizeof(int) * 4 is...
	std::array<int, 4> integerBuffer = {};
	constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();

	std::array<char, 64> charBuffer = {};

	// The information you wanna query __cpuid for.
	// https://docs.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=vs-2019
	constexpr std::array<int, 3> functionIds = {
		// Manufacturer
		//  EX: "Intel(R) Core(TM"
		0x8000'0002,
		// Model
		//  EX: ") i7-8700K CPU @"
		0x8000'0003,
		// Clockspeed
		//  EX: " 3.70GHz"
		0x8000'0004
	};

	std::string cpu;

	for (int id : functionIds)
	{
		// Get the data for the current ID.
		__cpuid(integerBuffer.data(), id);

		// Copy the raw data from the integer buffer into the character buffer
		std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);

		// Copy that data into a std::string
		cpu += std::string(charBuffer.data());
	}

	cpu.erase(std::remove(cpu.begin(), cpu.end(),' '));

	return cpu;
}
//included libraries/functionality for input/output
#include <fstream>
FILETIME prevSysIdle, prevSysKernel, prevSysUser;
// TIME DIFF FUNC
ULONGLONG CNNBSSHardware::CPUFileSubtractTimes(const FILETIME one, const FILETIME two)
{
	LARGE_INTEGER a, b;
	a.LowPart = one.dwLowDateTime;
	a.HighPart = one.dwHighDateTime;

	b.LowPart = two.dwLowDateTime;
	b.HighPart = two.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}
int CNNBSSHardware::GetCPUUsage(double& val)
{
	FILETIME sysIdle, sysKernel, sysUser;
	// sysKernel include IdleTime
	if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0) // GetSystemTimes func FAILED return value is zero;
		return 0;

	if (prevSysIdle.dwLowDateTime != 0 && prevSysIdle.dwHighDateTime != 0)
	{
		ULONGLONG sysIdleDiff, sysKernelDiff, sysUserDiff;
		sysIdleDiff = CPUFileSubtractTimes(sysIdle, prevSysIdle);
		sysKernelDiff = CPUFileSubtractTimes(sysKernel, prevSysKernel);
		sysUserDiff = CPUFileSubtractTimes(sysUser, prevSysUser);

		ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
		ULONGLONG kernelTotal = sysKernelDiff - sysIdleDiff; // kernelTime - IdleTime = kernelTime, because sysKernel include IdleTime

		if (sysTotal > 0) // sometimes kernelTime > idleTime
			val = (double)(((kernelTotal + sysUserDiff) * 100.0) / sysTotal);
	}

	prevSysIdle = sysIdle;
	prevSysKernel = sysKernel;
	prevSysUser = sysUser;

	return 1;
}
// Get GPU Name
std::string CNNBSSHardware::GetGPUName()
{
	char ch[128];
	for (int i = 0; ; i++)
	{
		DISPLAY_DEVICE dd = { sizeof(dd), 0 };
		BOOL f = EnumDisplayDevices(NULL, i, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
		if (!f)
			break;

		//convert from wide char to narrow char array
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, dd.DeviceString, -1, ch, 260, &DefChar, NULL);

	}
	return std::string(ch);
}
// Ram size
float CNNBSSHardware::GetRamSize()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex); // I misunderstand that

	GlobalMemoryStatusEx(&statex);

	// Physical RAM
	return (float)statex.ullTotalPhys / (DIV_NORMALIZER * DIV_NORMALIZER * DIV_NORMALIZER);
}
int CNNBSSHardware::GetPhysicalMemoryUsage()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex); // I misunderstand that

	GlobalMemoryStatusEx(&statex);

	float total = (float)statex.ullTotalPhys / (DIV_NORMALIZER * DIV_NORMALIZER * DIV_NORMALIZER);
	float available = (float)statex.ullAvailPhys / (DIV_NORMALIZER * DIV_NORMALIZER * DIV_NORMALIZER);
	float reserved = total - available;

	int result = (int)reserved * 100 / total;
#if 0
	// number * 100 / base
#define MYTESTTT "MYTEST"
	wxLog::AddTraceMask(MYTESTTT);
	wxLogTrace(MYTESTTT, "result of usage: %i t:%i a:%i r:%i", result, (int)total, (int)available, (int)reserved);
#endif
	// Physical RAM
	return result;
}
// Virtual Memory Info
void CNNBSSHardware::GetVirtualMemoryInfo(float& total, float& available, float& reserved, float& percentage)
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex); // I misunderstand that

	GlobalMemoryStatusEx(&statex);

	float _total = (float)statex.ullTotalVirtual/ (DIV_NORMALIZER * DIV_NORMALIZER * DIV_NORMALIZER);
	float _available = (float)statex.ullAvailVirtual / (DIV_NORMALIZER * DIV_NORMALIZER * DIV_NORMALIZER);
	float _reserved = _total - _available;

	int result = (int)_reserved * 100 / _total;

	percentage = result;
	reserved = _reserved;
	available = _available;
	total = _total;
#if 0
	// number * 100 / base
#define MYTESTTT "MYTEST"
	wxLog::AddTraceMask(MYTESTTT);
	wxLogTrace(MYTESTTT, "result of usage: %i t:%i a:%i r:%i", result, (int)total, (int)available, (int)reserved);
#endif
}
// Memory Usage
int CNNBSSHardware::GetMemoryUsage()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	return (int)statex.dwMemoryLoad;

	// to be informed:
#if 0
	_tprintf(TEXT("There is  %*ld percent of memory in use.\n"), INFO_FIELD_WIDTH, statex.dwMemoryLoad);
	_tprintf(TEXT("There are %*I64d total Mbytes of physical memory.\n"), INFO_FIELD_WIDTH, statex.ullTotalPhys / DIV_BYTE_TO_MB);
	_tprintf(TEXT("There are %*I64d free Mbytes of physical memory.\n"), INFO_FIELD_WIDTH, statex.ullAvailPhys / DIV_BYTE_TO_MB);
	_tprintf(TEXT("There are %*I64d total Mbytes of paging file.\n"), INFO_FIELD_WIDTH, statex.ullTotalPageFile / DIV_BYTE_TO_MB);
	_tprintf(TEXT("There are %*I64d free Mbytes of paging file.\n"), INFO_FIELD_WIDTH, statex.ullAvailPageFile / DIV_BYTE_TO_MB);
	_tprintf(TEXT("There are %*I64d total Mbytes of virtual memory.\n"), INFO_FIELD_WIDTH, statex.ullTotalVirtual / DIV_BYTE_TO_MB);
	_tprintf(TEXT("There are %*I64d free Mbytes of virtual memory.\n"), INFO_FIELD_WIDTH, statex.ullAvailVirtual / DIV_BYTE_TO_MB);
	_tprintf(TEXT("There are %*I64d free Mbytes of extended memory.\n"), INFO_FIELD_WIDTH, statex.ullAvailExtendedVirtual / DIV_BYTE_TO_MB);
#endif
}
// Free Disk Space
int CNNBSSHardware::GetDiskFreeSpacePercentage()
{
	DWORD lpSectorsPerCluster,
		lpBytesPerSector,
		lpNumberOfFreeClusters,
		lpTotalNumberOfClusters;

	if (GetDiskFreeSpace(NULL,
		&lpSectorsPerCluster,
		&lpBytesPerSector,
		&lpNumberOfFreeClusters,
		&lpTotalNumberOfClusters))
	{
		return 100-int(double(lpNumberOfFreeClusters) / double(lpTotalNumberOfClusters) * 100.0);
	}
	else
	{
		return 0;
	}
}

// -----------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//------------------------------------------------------------------------
//-------------------------------------------------
//--------------------------           Universal Serial Bus Device SPECIAL
//-------------------------------------------------
//------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
int CNNBSSHardware::_GetUSBCameraDevicesList(std::vector<std::string>& list, std::vector<std::string>& devicePaths, bool silent)
{
	ICreateDevEnum* pDevEnum = NULL;
	IEnumMoniker* pEnum = NULL;
	int deviceCounter = 0;
	CoInitialize(NULL);

	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
		reinterpret_cast<void**>(&pDevEnum));


	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,
			&pEnum, 0);

		if (hr == S_OK) {

			//printf("SETUP: Looking For Capture Devices\n");
			IMoniker* pMoniker = NULL;

			while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {

				IPropertyBag* pPropBag;
				hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
					(void**)(&pPropBag));

				if (FAILED(hr)) {
					pMoniker->Release();
					continue;  // Skip this one, maybe the next one will work.
				}


				// Find the description or friendly name.
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"Description", &varName, 0);

				if (FAILED(hr)) hr = pPropBag->Read(L"FriendlyName", &varName, 0);

				if (SUCCEEDED(hr))
				{

					hr = pPropBag->Read(L"FriendlyName", &varName, 0);

					int count = 0;
					char tmp[255] = { 0 };
					while (varName.bstrVal[count] != 0x00 && count < 255)
					{
						tmp[count] = (char)varName.bstrVal[count];
						count++;
					}
					list.emplace_back(tmp);

					// then read Device Path
					{
						VARIANT DP_Path;
						VariantInit(&DP_Path);

						hr = pPropBag->Read(L"DevicePath", &DP_Path, 0);

						if (SUCCEEDED(hr))
						{
							int __count = 0;
							char __tmp[255] = { 0 };
							while (DP_Path.bstrVal[__count] != 0x00 && __count < 255)
							{
								__tmp[__count] = (char)DP_Path.bstrVal[__count];
								__count++;
							}

							devicePaths.emplace_back(__tmp);
						}
					}
				}

				pPropBag->Release();
				pPropBag = NULL;

				pMoniker->Release();
				pMoniker = NULL;

				deviceCounter++;
			}

			pDevEnum->Release();
			pDevEnum = NULL;

			pEnum->Release();
			pEnum = NULL;
		}
	}
	
	return deviceCounter;
}

void CNNBSSHardware::GetConnectedUSBCameraList(std::vector<std::string>& deviceList, std::vector<std::string>& devicePath)
{
	_GetUSBCameraDevicesList(deviceList, devicePath);

	std::reverse(deviceList.begin(), deviceList.end());
	std::reverse(devicePath.begin(), devicePath.end());

	// We should sort array again. Since it should not overlap devices already in use
	//----- get existing USB devices that are in use by NNBSS
	for (int i = 0; i < deviceList.size(); i++)
	{
		// check if this device is in activeDevices vector;
		for (int j = 0; j < SCNNBSSMVMParametersList.size(); j++)
		{
			std::string m_ListDName = deviceList[i], m_ParamDName = SCNNBSSMVMParametersList[j]->m_MVMName
				, m_ListDPath = devicePath[i], m_ParamDPath = SCNNBSSMVMParametersList[j]->m_DevicePath;

			if (m_ListDName == m_ParamDName && m_ListDPath == m_ParamDPath)
			{
				long addressU;
				if (SCNNBSSMVMParametersList[j]->m_MVMSourceAddress.ToLong(&addressU)) {
					if (addressU >= 0) {
						nnbsstd::move_vector_index_to(deviceList, i, (int)addressU);
						nnbsstd::move_vector_index_to(devicePath, i, (int)addressU);
					}
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//------------------------------------------------------------------------
//-------------------------------------------------
//--------------------------           LISTENERs
//-------------------------------------------------
//------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------

void CNNBSSHardware::PortStanceListener(int stance)
{
	switch (stance)
	{
	case NNBSS_PORT_EVT_ON_STANCE_CHANGE:
		break;
	case NNBSS_PORT_EVT_ON_MOUNT_USB_CAMERA:
		break;
	case NNBSS_PORT_EVT_ON_UNMOUNT_USB_CAMERA:
		break;
	case NNBSS_PORT_EVT_ON_MOUNT_MOUSE:
		break;
	case NNBSS_PORT_EVT_ON_MOUNT_KEYBOARD:
		break;
	case NNBSS_PORT_EVT_ON_MOUNT_MIC:
		break;
	case NNBSS_PORT_EVT_ON_MOUNT_UNKNOWN:
		break;
	default:
		break;
	}
}