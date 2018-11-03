// Dll1.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "../dlog/dlog.h"

int main()
{
    int res =  dlog_init("\\log", "MRSystem", false);

    LogW("dll输出的日志是这个！");
    return res;
}

//fun1是启动
extern "C"  __declspec(dllexport) int __stdcall Fun1()
{
    return main();
}
