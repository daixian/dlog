// Dll1.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "../dlog/dlog.h"

//这个dll为了测试多个模块共用一个dlog的dll
int main()
{
    int res = dlog_init("\\log", "MRSystem", false);

    LogI("Cpp dll2 输出的日志是这个！");
    return res;
}

//fun1是启动
extern "C"  __declspec(dllexport) int __stdcall Fun2()
{
    return main();
}
