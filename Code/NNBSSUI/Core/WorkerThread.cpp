///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "WorkerThread.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// Standard C Lib
#include <random>

CNNBSSHardwareUIThread::CNNBSSHardwareUIThread(long sleepTime)
	: wxThread(wxTHREAD_JOINABLE), _sleepTime(sleepTime)
{
}

wxThread::ExitCode CNNBSSHardwareUIThread::Entry()
{
	while (!TestDestroy())
	{
		try
		{
			double m_CPUUsageFractional;
			CNNBSSHardwareAddressHandle()->GetCPUUsage(m_CPUUsageFractional);

			float m_VirtualMemoryFractional,total,available,reserved;
			CNNBSSHardwareAddressHandle()->GetVirtualMemoryInfo(total,available,reserved, m_VirtualMemoryFractional);

			long CPUGeneralUsage = (int)m_CPUUsageFractional,
				VirtualMemoryGeneralUsage = (int)m_VirtualMemoryFractional,
				RAMGeneralUsage = CNNBSSHardwareAddressHandle()->GetPhysicalMemoryUsage(),
				MemoryUsage = CNNBSSHardwareAddressHandle()->GetMemoryUsage(),
				DiskSpacePercentage = CNNBSSHardwareAddressHandle()->GetDiskFreeSpacePercentage();

			{
				// if not in range
				if (CPUGeneralUsage < 0)
					CPUGeneralUsage = 0;
				else if (CPUGeneralUsage > 100)
					CPUGeneralUsage = 100;

				if (VirtualMemoryGeneralUsage < 0)
					VirtualMemoryGeneralUsage = 0;
				else if (VirtualMemoryGeneralUsage > 100)
					VirtualMemoryGeneralUsage = 100;

				if (RAMGeneralUsage < 0)
					RAMGeneralUsage = 0;
				else if (RAMGeneralUsage > 100)
					RAMGeneralUsage = 100;

				if (MemoryUsage < 0)
					MemoryUsage = 0;
				else if (MemoryUsage > 100)
					MemoryUsage = 100;

				if (DiskSpacePercentage < 0)
					DiskSpacePercentage = 0;
				else if (DiskSpacePercentage > 100)
					DiskSpacePercentage = 100;

			}

			if(CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_CPU_BARNAME))
				CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_CPU_BARNAME)
				->_SetCurrent(CPUGeneralUsage);
			if(CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME))
				CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)
				->_SetCurrent(VirtualMemoryGeneralUsage);
			if(CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_RAM_BARNAME))
				CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, 
					NNBSS_STATUSBAR_RAM_BARNAME)->_SetCurrent(RAMGeneralUsage);
			if(CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_MEMORY_BARNAME))
				CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_MEMORY_BARNAME)
				->_SetCurrent(MemoryUsage);
			if (CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_DISKSPACE_BARNAME))
				CNNBSSStatusProgressBarDeviceStatusAddressHandle(nullptr, NNBSS_STATUSBAR_DISKSPACE_BARNAME)
				->_SetCurrent(DiskSpacePercentage);

			//{
			//	static bool _ShowOverLoadWarning = true;
			//	if (_ShowOverLoadWarning)
			//	{
			//		if (CPUGeneralUsage > 90 || VirtualMemoryGeneralUsage > 80 || RAMGeneralUsage > 80
			//			|| MemoryUsage > 80 || DiskSpacePercentage > 90)
			//		{
			//			NNBSSMessageBoxSimple(_("Warning"),
			//				_("An abnormal behavior has been detected on your computer.\nThis may be due to insufficient space or because you want him to do many tasks.\nPlease make sure your computer requirement is suitable for these tasks.\nOtherwise, you will get unpleasant results."), wxICON_ERROR);
			//			_ShowOverLoadWarning = false;
			//		}
			//	}
			//}

			{
				// (USB)Port listener

				static std::vector<std::string> oldDeviceList, oldDevicePathList;
				std::vector<std::string> currentDeviceList, currentDevicePathList;
				CNNBSSHardwareAddressHandle()->GetConnectedUSBCameraList(currentDeviceList, currentDevicePathList);
				static bool _OnceGate = true;
				if (_OnceGate)
				{
					// Since their size will be different on init, make them equal for once

					_OnceGate = false;

					oldDeviceList = currentDeviceList;
					oldDevicePathList = currentDevicePathList;
				}
				if (oldDeviceList != currentDeviceList || oldDevicePathList != currentDevicePathList)
				{
					if (oldDeviceList.size() > currentDeviceList.size())
					{
						// USB Camera Unmounted
						CNNBSSHardwareAddressHandle()->PortStanceListener(NNBSS_PORT_EVT_ON_UNMOUNT_USB_CAMERA);
					}
					else if (oldDeviceList.size() < currentDeviceList.size())
					{
						// USB Camera Mounted
						CNNBSSHardwareAddressHandle()->PortStanceListener(NNBSS_PORT_EVT_ON_MOUNT_USB_CAMERA);
					}

					oldDeviceList = currentDeviceList;
					oldDevicePathList = currentDevicePathList;
				}
			}

			Sleep(_sleepTime);
		}
		catch (const std::exception& e)
		{
			wxLogError("Something went wrong with input/output of insertion of the devices.\n Reason:%s",e.what());
			break;
		}
		catch (...)
		{
			wxLogError("Unknown exception has been thrown by Hardware i/o listener!");
			break;
		}
	}


	return static_cast<wxThread::ExitCode>(nullptr);
}