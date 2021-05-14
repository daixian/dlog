﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。

#if (DLOG_DLL_EXPORTS)

#    if defined(_WIN32) || defined(_WIN64)
#        include "stdafx.h"
#        include "dlog.h"
#        include "./Common/Debug.h"

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:

    case DLL_PROCESS_DETACH: //卸载dll的时候自动释放
        //目前发现貌似最后一次进入是0
        if (ul_reason_for_call == 0) {
            if (dlog::Debug::GetInst()->isInit) {
                if (dlog::Debug::GetInst()->isPrintInternalMessage)
                    LogI("dll释放,日志关闭!");
                dlog_close();
            }
        }
        break;
    }
    return TRUE;
}
#    endif

#endif
