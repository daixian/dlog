// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "dlog.h"

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:

    case DLL_PROCESS_DETACH:
        //ж��dll��ʱ���Զ��ͷ�
        if (ul_reason_for_call == 0)//Ŀǰ����ò�����һ�ν�����0
        {
            LogI("dll�ͷ�,��־�ر�!");

            dlog_close();
        }
        break;
    }
    return TRUE;
}