// dlog.cpp : 定义 DLL 应用程序的导出函数。
//

#include "dlog.h"

#include "Common/GLogHelper.h"
#include "Common/Debug.h"
#include "Common/MemoryLog.h"
#include <mutex>

using namespace dxlib;

//互斥锁，用于初始化和关闭的时候
std::mutex mt;

//全局对象，用于在dll释放的时候自动析构
static GLogHelper* volatile inst = NULL;

///-------------------------------------------------------------------------------------------------
/// <summary> 模块初始化. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="program"> [in]程序名. </param>
/// <param name="logDir">  [in]日志文件夹路径名（相对模块目录）. </param>
///
/// <returns> 如果之前未被初始化返回0，否则返回1. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(char* logDir, char* program)
{
    mt.lock();
    if (inst == NULL)
    {
        inst = new GLogHelper(program, logDir);
        mt.unlock();
        return 0;
    }
    else
    {
        if (inst->programName.compare(program) != 0 &&//如果两次设置的程序名不一致，那么才删除
            strcmp(program, "dlog") != 0)//同时第二次设置的这个程序名不能等于默认名字
        {
            delete inst;
            inst = new GLogHelper(program, logDir);
        }
        mt.unlock();
        return 1;
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
    if (inst != NULL)
    {
        delete inst;
        inst = NULL;
        mt.unlock();
        return 0;
    }
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
    if (inst != NULL)
    {
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
    Debug::GetInst()->isLog = enable;
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 设置大于等于某一级日志输出到控制台. 参数有DLOG_INFO,DLOG_WARNING,DLOG_ERROR,
/// DLOG_FATAL.
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
extern "C" DLOG_EXPORT void __stdcall LogI(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
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
extern "C" DLOG_EXPORT void __stdcall LogW(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
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
extern "C" DLOG_EXPORT void __stdcall LogE(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogE_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a fatal. </summary>
///
/// <remarks> Dx                                                                                                                                                                                                                     , 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogFATAL(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogFATAL_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

#pragma region memory log

///-------------------------------------------------------------------------------------------------
/// <summary> 设置内存log是否使能. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
DLOG_EXPORT void dlog_memory_log_enable(bool enable)
{
    Debug::GetInst()->isLogMemory = enable;
}


///-------------------------------------------------------------------------------------------------
/// <summary> 提取一条内存日志. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="buff">   [in,out] 缓存buffer. </param>
/// <param name="offset"> The offset. </param>
/// <param name="count">  Number of. </param>
///
/// <returns> 提取出的日志长度，如果为0表示没有提出日志来. </returns>
///-------------------------------------------------------------------------------------------------
DLOG_EXPORT int dlog_get_memlog(char * buff, int offset, int count)
{
    std::string msg;
    int copyLen = 0;
    if (MemoryLog::GetInst()->getLog(msg))
    {
        copyLen = msg.size() < count ? msg.size() : count;
        msg.copy(buff, copyLen);
    }
    buff[copyLen] = 0;
    return copyLen;
}

#pragma endregion