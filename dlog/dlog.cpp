// dlog.cpp : 定义 DLL 应用程序的导出函数。
//
#include "dlog.h"

#include "./Common/GLogHelper.h"
#include "./Common/Debug.h"
#include "./Common/MemoryLog.h"
#include "./Common/FileHelper.h"
#include <mutex>

//注意引用了glog头不能引用windows头,有宏定义冲突ERROR

using namespace dxlib;

//互斥锁，用于初始化和关闭的时候
std::mutex mt;

//全局对象，用于在dll释放的时候自动析构
static GLogHelper* volatile inst = NULL;

///-------------------------------------------------------------------------------------------------
/// <summary> 去得到一个全局指针的值，如果c++的值和c#的值相等那么应该是共用log模块了. </summary>
///
/// <remarks> Dx, 2018/11/2. </remarks>
///
/// <returns>
/// 直接返回全局的GLogHelper指针
/// </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void* __stdcall dlog_gh_ptr()
{
    return inst;
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 模块初始化,日志文件夹路径可以使用绝对目录也可以使用相对目录, 如果使用相对目录,
/// 那么程序会将它理解为相对模块目录,路径例如 char* logDir = "\\log",char*
/// program = "dlog".
/// isForceInit如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志.
/// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
/// 如果强制重设但是失败还是复用了那么返回3.
/// </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="logDir">      [in]日志文件夹路径名（相对模块目录）. </param>
/// <param name="program">     [in]程序名. </param>
/// <param name="isForceInit"> 如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志. </param>
///
/// <returns>
/// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
/// 如果强制重设但是失败还是复用了那么返回3.
/// </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(const char* logDir, const char* program, bool isForceInit)
{
    mt.lock();
    if (inst == NULL) {
        inst = new GLogHelper(program, logDir);
        mt.unlock();
        return 0; //第一次初始化
    }
    else {
        //只要没有ForceInit那么都是会复用的
        if (!isForceInit) {
            mt.unlock();
            return 1; //成功复用
        }
        //如果设置了强制初始化
        if (inst->programName.compare(program) != 0 && //如果两次设置的程序名不一致，那么才删除
            strcmp(program, "dlog") != 0) {            //同时第二次设置的这个程序名不能等于默认名字
            delete inst;
            inst = new GLogHelper(program, logDir);
            mt.unlock();
            return 2; //强制重设了一次glog
        }
        mt.unlock();
        return 3; //强制重设了一次glog
    }
}

///-------------------------------------------------------------------------------------------------
/// <summary> 关闭模块. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_close()
{
    mt.lock();
    Debug::GetInst()->Reset();
    if (inst != NULL) {
        delete inst;
        inst = NULL;
        mt.unlock();
        return 0;
    }
    mt.unlock();
    return 1;
}

extern "C" DLOG_EXPORT int __stdcall dlog_remove_old_log_file()
{
    mt.lock();

    mt.unlock();
    return 1;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到当前设置的日志目录. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="result"> [out] If non-null, the result. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_get_log_dir(char* result)
{
    if (inst != NULL) {
        inst->logDirPath.copy(result, inst->logDirPath.size());
    }
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置整个log使能. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_enable(bool enable)
{
    Debug::GetInst()->isEnable = enable;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会工作. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_usual_thr(int usualThr)
{
    Debug::GetInst()->logUsualThr = usualThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会工作. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_usual_thr()
{
    return Debug::GetInst()->logUsualThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置Dlog的内存日志门限,大于等于该优先级的日志都会工作. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_memory_thr(int memoryThr)
{
    Debug::GetInst()->logMemoryThr = memoryThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到Dlog的内存日志门限,大于等于该优先级的日志都会工作. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_memory_thr()
{
    return Debug::GetInst()->logMemoryThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 设置大于等于某一级日志输出到控制台. 参数有DLOG_INFO,DLOG_WARNING,DLOG_ERROR
/// </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="LogSeverity"> 大于等于这一级的日志都会输出到控制台. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_FLAGS_stderrthreshold(int LogSeverity)
{
    FLAGS_stderrthreshold = LogSeverity; // 当日志级别大于等于此级别时，自动将此日志输出到标准错误(终端窗口)中
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an info. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogI(const char* strFormat, ...)
{
    if (inst == NULL) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogI_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a w. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogW(const char* strFormat, ...)
{
    if (inst == NULL) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogW_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an e. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogE(const char* strFormat, ...)
{
    if (inst == NULL) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogE_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a debug. </summary>
///
/// <remarks> Dx                                                                                                                                                                                                                     , 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogD(const char* strFormat, ...)
{
    if (inst == NULL) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogD_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

#pragma region memory log

///-------------------------------------------------------------------------------------------------
/// <summary> 设置内存log是否使能. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="enable"> 设置为false之后内存日志函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_memory_log_enable(bool enable)
{
    Debug::GetInst()->isMemLogEnable = enable;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 提取一条内存日志,注意是一条. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="buff">   [in,out] 缓存buffer. </param>
/// <param name="offset"> The offset. </param>
/// <param name="count">  缓存buffer的长度，如果给的buffer过小那么就没办法完整提取. </param>
///
/// <returns> 提取出的日志char*的有效长度，如果为0表示没有提出日志来. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_memlog(char* buff, int offset, int length)
{
    std::string msg;
    int copyLen = 0;
    if (MemoryLog::GetInst()->getLog(msg)) {
        copyLen = msg.size() < (length - 1) ? msg.size() : (length - 1); //这里string的size是否就等于字节的size???
        msg.copy(buff, copyLen);
    }
    else {
        return 0;
    }
    buff[copyLen] = 0;
    return copyLen;
}

#pragma endregion