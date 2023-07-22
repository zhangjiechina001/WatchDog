#ifndef CLEANMEM_H
#define CLEANMEM_H

#include <qt_windows.h>
#include <Psapi.h>
//#include "winSysInfo.h"

void cleanMemory() {
//    NormalMemInfo info = WinSysInfo::getProcessMemoryInfo();
//    QString memoryWokersize = QString::number((info.workSize >> 20), 'f', 0);

//    if (memoryWokersize.toInt() > 50) {
//        HANDLE handle;
//        DWORD processID = GetCurrentProcessId();

//        handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
//        if (handle == NULL) {
//            return ;
//        }
//        EmptyWorkingSet(handle);
//    }
}

#endif // CLEANMEM_H
