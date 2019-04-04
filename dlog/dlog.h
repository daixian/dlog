#pragma once
#ifndef _DLOG_H_
#define _DLOG_H_

#if defined(_WIN32) || defined(_WIN64)

#if defined DLOG_EXPORTS
#define DLOG_EXPORT __declspec(dllexport)
#define DLOG__LOCAL
#else
#pragma comment(lib, "dlog.lib")
#define DLOG_EXPORT __declspec(dllimport)
#endif

#elif defined(__linux__)
#if __GNUC__ >= 4
#if defined DLOG_EXPORTS
#define DLOG_EXPORT __attribute__((visibility("default")))
#define DLOG__LOCAL __attribute__((visibility("hidden")))
#else
#define DLOG_EXPORT
#define DLOG__LOCAL
#endif
#define __stdcall
#define __cdecl
#else
#endif

#endif

//给用户使用的LogSeverity定义,目前和spdlog里的定义一致,但是只使用debug,info,warn,err
enum class dlog_level
{
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    err = 4,
    critical = 5,
    off = 6,
};

//日志库初始化的时候如果使用了相对目录,那么相对目录的选择
enum class dlog_init_relative
{
    //相对appdata文件夹
    APPDATA = 0,
    //相对dll自身文件夹
    MODULE = 1,
};

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 模块初始化,日志文件夹路径可以使用绝对目录也可以使用相对目录(第三个参数进行相对位置的设置),
/// 如果使用相对目录,那么程序会将它理解为相对模块目录,路径例如 char* logDir = "\\log",char* program = "dlog".
/// isForceInit如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志.
/// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
/// 如果强制重设但是失败还是复用了那么返回3.
/// </summary>
///
/// <remarks>
/// - DLOG_INIT_RELATIVE_APPDATA: 相对于AppData文件夹.
/// - DLOG_INIT_RELATIVE_MODULE: 相对于dll文件自身文件夹.
/// - 如果是在同一秒重复创建关闭日志文件,那么由于文件名一样,会被写入同一个日志文件.
///     Dx, 2018/4/22.
/// </remarks>
///
/// <param name="logDir">      [in]日志文件夹路径名（相对模块目录）. </param>
/// <param name="program">     [in]程序名. </param>
/// <param name="dir_relatvie">(Optional)相对路径的相对文件夹位置. </param>
/// <param name="isForceInit"> (Optional) 如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志. </param>
///
/// <returns>
/// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
/// 如果强制重设但是失败还是复用了那么返回3.
/// </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(const char* logDir = "log",
                                               const char* program = "dlog",
                                               dlog_init_relative dir_relatvie = dlog_init_relative::APPDATA,
                                               bool isForceInit = false);

///-------------------------------------------------------------------------------------------------
/// <summary> 关闭模块. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_close();

///-------------------------------------------------------------------------------------------------
/// <summary> 设置整个log使能. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_enable(bool enable);

///-------------------------------------------------------------------------------------------------
/// <summary> 设置控制台log是否使能. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_console_log_enable(bool enable);

///-------------------------------------------------------------------------------------------------
/// <summary> 设置Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="fileThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_file_thr(dlog_level fileThr);

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 得到Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入.
/// </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_file_thr();

///-------------------------------------------------------------------------------------------------
/// <summary> 设置Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_memory_thr(dlog_level memoryThr);

///-------------------------------------------------------------------------------------------------
/// <summary> 得到Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_memory_thr();

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 设置最高某一级日志输出到控制台，大于等于这一级别的日志都会输出到控制台，小于这一级别的日志不会输出到控制台.
/// </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="LogSeverity"> 大于等于这一级的日志都会输出到控制台. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_console_thr(dlog_level LogSeverity);

///-------------------------------------------------------------------------------------------------
/// <summary> 得到Dlog的控制台日志门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_console_thr();

///-------------------------------------------------------------------------------------------------
/// <summary> 设置立即刷新的flush on. </summary>
///
/// <remarks> Dx, 2019/3/18. </remarks>
///
/// <param name="LogSeverity"> The log severity. </param>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_flush_on(dlog_level LogSeverity);

///-------------------------------------------------------------------------------------------------
/// <summary> 立即flush. </summary>
///
/// <remarks> Dx, 2019/3/25. </remarks>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_flush();

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
extern "C" DLOG_EXPORT void __cdecl LogI(const char* strFormat, ...);

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a warring. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __cdecl LogW(const char* strFormat, ...);

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an error. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __cdecl LogE(const char* strFormat, ...);

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a debug. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __cdecl LogD(const char* strFormat, ...);

#pragma region 内存缓存日志相关

///-------------------------------------------------------------------------------------------------
/// <summary> 设置内存log是否使能. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_memory_log_enable(bool enable);

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
extern "C" DLOG_EXPORT int __stdcall dlog_get_memlog(char* buff, int offset, int count);

#pragma endregion

///-------------------------------------------------------------------------------------------------
/// <summary> 得到appdata的路径,目录末尾不带斜杠"C:\\Users\\dx\\AppData\\Roaming". </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_appdata_dir(char* buff, int size);

///-------------------------------------------------------------------------------------------------
/// <summary> 得到模块的路径,目录末尾不带斜杠. </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_module_dir(char* buff, int size);

///-------------------------------------------------------------------------------------------------
/// <summary> 得到日志文件夹的路径. </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_log_dir(char* buff, int size);

///-------------------------------------------------------------------------------------------------
/// <summary> 得到日志文件的路径. </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_log_file_path(char* buff, int size);

#endif // !_DLOG_H_