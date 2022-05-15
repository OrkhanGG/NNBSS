///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef THREAD_H
#define THREAD_H

// Standard C Lib
#include <atomic>
#include <memory>
#include <vector>

#define MAX_CT_LETTER 26

class CNNBSSHardwareUIThread : public wxThread
{
public:
    CNNBSSHardwareUIThread(long sleepTime = 500);

protected:
    ExitCode Entry() override;

private:
    char PREV_DRIVE_LIST[MAX_CT_LETTER];
    char NEW_DRIVE_LIST[MAX_CT_LETTER];
    
    long _sleepTime;
};


#endif // THREAD_H