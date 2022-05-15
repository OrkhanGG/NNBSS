///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
// Purpose:		Get/Set Hardware/Machine/PC related info
///////////////////////////////////////////////////////////////////////////////
#ifndef HARDWARE_H
#define HARDWARE_H

// Standard C Lib
#include <iostream>
#include <Windows.h>
#include <vector>

// Forward Declarations
class SNNBSSMVMData;

class CNNBSSHardware
{
public:
	CNNBSSHardware();
	~CNNBSSHardware();

	// Current Machine/PC info
		// Get CPU Info&Name
	std::string GetCpuInfo();
	ULONGLONG CPUFileSubtractTimes(const FILETIME one, const FILETIME two);
	int GetCPUUsage(double& val);
		// Get GPU Name
	std::string GetGPUName();
		// Get Ram Size(In Bytes)
	float GetRamSize();
	int GetPhysicalMemoryUsage();// Ram usage with %
		// Memory & Virtual Memory Info
	void GetVirtualMemoryInfo(float& total, float& available, float& reserved, float& percentage);
	int GetMemoryUsage();
		// Disk Space Info with %
	int GetDiskFreeSpacePercentage();

	// Manager for _GetUSBCameraDevicesList 
	void GetConnectedUSBCameraList(std::vector<std::string>& deviceList,
		std::vector<std::string>& devicePath);
private:
	// OpenCV method: Check for connected USB Cameras
	int _GetUSBCameraDevicesList(std::vector<std::string>& list, std::vector<std::string>& devicePaths, bool silent = true);

public:
	// Notifys system when port stance is changed like on usb connect
	void PortStanceListener(int stance);
};

#endif // ! HARDWARE_H