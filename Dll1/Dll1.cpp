// Dll1.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include "dlog/dlog.h"
int main()
{

    dlog_init("\\log", "MRSystem", false);

    LogW("dll输出的日志是这个！");
}

//fun1是启动
extern "C"  __declspec(dllexport) void __stdcall Fun1()
{
    main();
}
