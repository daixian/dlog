#pragma once
//仓库位置 https://github.com/daixian/dlog

#ifndef _DLOG_H_
#    define _DLOG_H_

// --------------------- windows ---------------------
// 由三个定义来控制,构建时会根据是否是共享库定义DLOG_DLL_EXPORTS或DLOG_STATIC,
// 使用该库的其他项目会由这个库的conan文件加入DLOG_STATIC或DLOG_DLL的定义.
#    if defined(_WIN32) || defined(_WIN64)
// 如果是库自身构建时
#        if defined(DLOG_DLL_EXPORTS) //导出库使用dll模式
#            define DLOG_EXPORT __declspec(dllexport)
#            define DLOG__LOCAL
#        elif defined(DLOG_STATIC)
#            define DLOG_EXPORT
#            define DLOG__LOCAL
#        endif

// 如果是用户,在使用的时候定义DLOG_STATIC表示静态链接,DLOG_DLL或没有定义表示动态链接
#        ifndef DLOG_EXPORT
#            pragma comment(lib, "dlog.lib")
#            define DLOG_EXPORT __declspec(dllimport)
#        endif
// ---------- 非VC的编译器那么先不区分dllimport ---------------
#    else
#        define DLOG_EXPORT __attribute__((visibility("default")))
#        define DLOG__LOCAL __attribute__((visibility("hidden")))
#
#        define __cdecl //默认是，加上了反而有warning __attribute__((__cdecl__))
#    endif

//它是定义vs编译器使用utf-8,现在整个dlog统一应该使用utf-8作为输出.注意文件需要保存成UTF8带BOM
#    ifdef DLOG_UTF8
#        pragma execution_character_set("utf-8")
#    endif

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

// 错误状态,目前还没时间去实现返回使用...
enum class DLogError
{
    Unknown = -1,
    Ok = 0,
    NotImplemented = 1,
    NotInitialized = 2,
    AlreadyInitialized = 3,
    InvalidParameter = 4,
    InvalidContext = 5,
    InvalidHandle = 6,
    RuntimeIncompatible = 7,
    RuntimeNotFound = 8,
    SymbolNotFound = 9,
    DisplayNotFound = 10,
    DeviceNotFound = 11,
    TargetNotFound = 12,
    CapabilityNotFound = 13,
    BufferTooSmall = 14,
    SyncFailed = 15,
    OperationFailed = 16,
    InvalidAttribute = 17,
};

// 日志回调函数指针类型
typedef void (*DlogLoggerCallback)(int level, const char* message);

/**
 * 模块初始化,日志文件夹路径可以使用绝对目录也可以使用相对目录(第三个参数进行相对位置的设置), 
 * 如果使用相对目录,那么程序会将它理解为相对模块目录,路径例如 char* logDir = "log",char* program = "dlog".
 * isForceInit如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志. 如果之前未被初始化返回0,
 * 如果成功复用那么就返回1,如果强制重设成功那么返回2, 如果强制重设但是失败还是复用了那么返回3.
 * 在这个函数调用之后可以调用dlog_is_initialized()函数查询是否初始化成功.
 *
 * 如果是在同一秒重复创建关闭日志文件,那么由于文件名一样,会被写入同一个日志文件.
 * 注意不支持UTF8的中文程序名~~ 如有乱码文件名使用dlog_init_wchar_filename()函数进行初始化.
 * 
 * @author daixian
 * @date 2018/4/22
 *
 * @param [in] logDir       (Optional) 日志文件夹路径名.
 * @param [in] program      (Optional) 程序名,他会成为文件名的一部分,所以不要使用一些非法符号,注意这里不支持UTF8的中文.
 * @param      dir_relatvie (Optional) 相对路径的相对文件夹位置.
 *                                          APPDATA: 相对于AppData文件夹.
 *                                          MODULE:  相对于dll文件自身文件夹
 * @param      isForceInit  (Optional) 如果为false，那么就可以不强制初始化模块，理论上整个程序C#和C++都共用一个日志.
 *
 * @returns 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2, 如果强制重设但是失败还是复用了那么返回3.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_init(const char* logDir = "log",
                                             const char* program = "dlog",
                                             dlog_init_relative dir_relatvie = dlog_init_relative::APPDATA,
                                             bool isForceInit = false);

/**
 * 模块初始化,输入的路径使用utf16编码,主要是方便c#的调用和解决各种日志文件名乱码.使用方法同dlog_init().
 *
 * @author daixian
 * @date 2021/4/11
 *
 * @param [in] logDir       (Optional) 日志文件夹路径名.
 * @param [in] program      (Optional) 程序名,他会成为文件名的一部分,所以不要使用一些非法符号.
 * @param      dir_relatvie (Optional) 相对路径的相对文件夹位置.
 *                                          APPDATA: 相对于AppData文件夹.
 *                                          MODULE:  相对于dll文件自身文件夹
 * @param      isForceInit  (Optional) 如果为false，那么就可以不强制初始化模块，理论上整个程序C#和C++都共用一个日志.
 *
 * @returns 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2, 如果强制重设但是失败还是复用了那么返回3.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_init_wchar_filename(const wchar_t* logDir = L"log",
                                                            const wchar_t* program = L"dlog",
                                                            dlog_init_relative dir_relatvie = dlog_init_relative::APPDATA,
                                                            bool isForceInit = false);

/**
 * 关闭模块.
 *
 * @author daixian
 * @date 2018/4/22
 *
 * @returns 成功返回0.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_close();

/**
 * 是否日志已经初始化了.
 *
 * @author daixian
 * @date 2018/4/22
 *
 * @returns 已经初始化了返回true.
 */
extern "C" DLOG_EXPORT bool __cdecl dlog_is_initialized();

/**
 * 在调用过init之后可以查询是否是初始化失败的状态,如果是初始化失败的状态那么通常是严重的文件系统错误.
 *
 * @author daixian
 * @date 2021/3/22
 *
 * @returns 初始化失败的状态返回true.
 */
extern "C" DLOG_EXPORT bool __cdecl dlog_is_init_fail();

/**
 * 设置整个log使能.
 *
 * @author daixian
 * @date 2018/4/23
 *
 * @param  enable 设置为false之后Log函数会直接返回不作任何操作.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_enable(bool enable);

/**
 * 设置控制台log是否使能.(需要在init之前调用这个函数,close之后下次打开重置为enable,需要重新再设置一次)
 *
 * @author daixian
 * @date 2018/5/11
 *
 * @param  enable 设置为false之后不会开启相关控制台日志功能.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_console_log_enable(bool enable);

/**
 * 设置文件log是否使能.(需要在init之前调用这个函数,close之后下次打开会重置为enable,需要重新再设置一次)
 *
 * @author daixian
 * @date 2018/5/11
 *
 * @param  enable 设置为false之后Log函数会直接返回不作任何操作.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_file_log_enable(bool enable);

/**
 * 设置Dlog的常规文件日志（非内存日志）门限,大于等于该优先级的日志都会写入.
 *
 * @author daixian
 * @date 2018/11/15
 *
 * @param  fileThr 要设置的门限.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_set_file_thr(dlog_level fileThr);

/**
 * 得到Dlog的常规文件日志（非内存日志）门限,大于等于该优先级的日志都会写入.
 *
 * @author daixian
 * @date 2018/11/15
 *
 * @returns 门限值(类型为dlog_level或者spdlog::level::level_enum).
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_file_thr();

/**
 * 设置Dlog的内存日志门限,大于等于该优先级的日志都会写入.
 *
 * @author daixian
 * @date 2018/11/15
 *
 * @param  memoryThr 要设置的门限.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_set_memory_thr(dlog_level memoryThr);

/**
 * 得到Dlog的内存日志门限,大于等于该优先级的日志都会写入.
 *
 * @author daixian
 * @date 2018/11/15
 *
 * @returns 门限值(类型为dlog_level或者spdlog::level::level_enum).
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_memory_thr();

/**
 * 设置最高某一级日志输出到控制台，大于等于这一级别的日志都会输出到控制台，小于这一级别的日志不会输出到控制台.
 *
 * @author daixian
 * @date 2018/4/23
 *
 * @param  LogSeverity 大于等于这一级的日志都会输出到控制台.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_set_console_thr(dlog_level LogSeverity);

/**
 * 得到Dlog的控制台日志门限,大于等于该优先级的日志都会写入.
 *
 * @author daixian
 * @date 2018/4/23
 *
 * @returns 门限值(类型为dlog_level或者spdlog::level::level_enum).
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_console_thr();

/**
 * 设置立即刷新的日志等级.
 *
 * @author daixian
 * @date 2018/4/23
 *
 * @param  LogSeverity 大于等于这一级的日志都会立即刷新.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_set_flush_on(dlog_level LogSeverity);

/**
 * 设置每隔多长时间flush的间隔时间.
 *
 * @author daixian
 * @date 2019/4/17.
 *
 * @param  second 秒数.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_set_flush_every(int second);

/**
 * 立即flush.
 *
 * @author daixian
 * @date 2019/3/25
 */
extern "C" DLOG_EXPORT void __cdecl dlog_flush();

/**
 * Logs an info.
 *
 * @author daixian
 * @date 2018/4/22
 *
 * @param  strFormat Describes the format to use.
 * @param  ...       Variable arguments providing additional information.
 */
extern "C" DLOG_EXPORT void __cdecl LogI(const char* strFormat, ...);

/**
 * Logs a warring.
 *
 * @author daixian
 * @date 2018/4/23
 *
 * @param  strFormat Describes the format to use.
 * @param  ...       Variable arguments providing additional information.
 */
extern "C" DLOG_EXPORT void __cdecl LogW(const char* strFormat, ...);

/**
 * Logs an error.
 *
 * @author daixian
 * @date 2018/4/23
 *
 * @param  strFormat Describes the format to use.
 * @param  ...       Variable arguments providing additional information.
 */
extern "C" DLOG_EXPORT void __cdecl LogE(const char* strFormat, ...);

/**
 * Logs a debug.
 *
 * @author daixian
 * @date 2018/4/23
 *
 * @param  strFormat Describes the format to use.
 * @param  ...       Variable arguments providing additional information.
 */
extern "C" DLOG_EXPORT void __cdecl LogD(const char* strFormat, ...);

//wchar_t转成UTF8的日志
extern "C" DLOG_EXPORT void __cdecl wLogI(const wchar_t* strFormat, ...);
extern "C" DLOG_EXPORT void __cdecl wLogW(const wchar_t* strFormat, ...);
extern "C" DLOG_EXPORT void __cdecl wLogE(const wchar_t* strFormat, ...);
extern "C" DLOG_EXPORT void __cdecl wLogD(const wchar_t* strFormat, ...);

/**
 * 不带格式化字符串的输出日志.
 *
 * @author daixian
 * @date 2021/3/23
 *
 * @param  level   日志等级.
 * @param  message 日志内容.
 */
extern "C" DLOG_EXPORT void __cdecl LogMsg(dlog_level level, const char* message);

/**
 * 不带格式化字符串的输出日志.
 *
 * @author daixian
 * @date 2021/3/23
 *
 * @param  level   日志等级.
 * @param  message 日志内容.
 */
extern "C" DLOG_EXPORT void __cdecl wLogMsg(dlog_level level, const wchar_t* message);

/**
 * 设置一个传入外部的回调,注意每次close日志系统会清空这个设置,之后需要重新设置一次.
 *
 * @author daixian
 * @date 2021/3/23
 *
 * @param  fp 函数指针.
 */
extern "C" DLOG_EXPORT void __stdcall dlog_set_logger_function(DlogLoggerCallback fp);

/**
 * 是否在windows平台下初始化控制台的时候执行chcp 65001,现在默认不是执行.(需要在init之前调用这个函数)
 *
 * @author daixian
 * @date 2021/3/23
 *
 * @param  ischcp65001 设为true则在windows平台下初始化控制台的时候执行chcp 65001.
 */
extern "C" DLOG_EXPORT void __stdcall dlog_set_is_chcp65001(bool ischcp65001);

/**
 * 是否打印日志模块的启动时的内部消息,默认不打印.(需要在init之前调用这个函数)
 *
 * @author daixian
 * @date 2021/3/25
 *
 * @param  isPrint 设为true则会打印一些日志模块启动的内容.
 */
extern "C" DLOG_EXPORT void __stdcall dlog_set_is_print_internal_message(bool isPrint);

/**
 * 是否在日志文件头写入utf8的bom头,默认是写入.(需要在init之前调用这个函数)
 *
 * @author daixian
 * @date 2021/3/25
 *
 * @param  isUTF8BOM 设为true则会写入bom头.
 */
extern "C" DLOG_EXPORT void __stdcall dlog_set_is_use_utf8_bom(bool isUTF8BOM);

/**
 * 转换UTF8到gbk.
 *
 * @author daixian
 * @date 2021/3/23
 *
 * @param       s_utf8 utf8编码的字符串.
 * @param [out] dst    目标buff.
 * @param       size   buff大小.
 *
 * @returns 结果字符串长度.
 */

extern "C" DLOG_EXPORT int __cdecl dlog_convert_utf8_to_gbk(const char* s_utf8, char* dst, int size);

/**
 * 转换gbk到utf8.
 *
 * @author daixian
 * @date 2021/3/23
 *
 * @param       s_gbk gbk编码的字符串.
 * @param [out] dst   目标buff.
 * @param       size  buff大小.
 *
 * @returns 结果字符串长度.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_convert_gbk_to_utf8(const char* s_gbk, char* dst, int size);

#    pragma region 内存缓存日志相关

/**
 * 设置内存日志功能是否使能.
 *
 * @author daixian
 * @date 2018/5/11
 *
 * @param  enable 设置为false之后Log函数会直接返回不作任何操作.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_memory_log_enable(bool enable);

/**
 * 提取一条内存日志.
 *
 * @author daixian
 * @date 2018/5/11
 *
 * @param [in,out] buff   缓存buffer.
 * @param          offset The offset.
 * @param          count  Number of.
 *
 * @returns 提取出的日志长度，如果为0表示没有提出日志来.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_memlog(char* buff, int offset, int count);

/**
 * 设置内存日志的临时保存的最多条数.
 *
 * @author daixian
 * @date 2019/8/21
 *
 * @param  maxCount 临时保存的条数.
 */
extern "C" DLOG_EXPORT void __cdecl dlog_set_memlog_save_limit(unsigned int maxCount);

#    pragma endregion

/**
 * 得到appdata的路径,目录末尾不带斜杠"C:\\Users\\dx\\AppData\\Roaming".
 *
 * @author daixian
 * @date 2019/3/19
 *
 * @param [in] buff 拷贝字符的buff.
 * @param      size buff大小.
 *
 * @returns 实际的字符串长度.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_appdata_dir(char* buff, int size);

/**
 * 得到模块的路径,目录末尾不带斜杠.
 *
 * @author daixian
 * @date 2019/3/19
 *
 * @param [in] buff 拷贝字符的buff.
 * @param      size buff大小.
 *
 * @returns 实际的字符串长度.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_module_dir(char* buff, int size);

/**
 * 得到日志文件夹的路径.
 *
 * @author daixian
 * @date 2019/3/19
 *
 * @param [in] buff 拷贝字符的buff.
 * @param      size buff大小.
 *
 * @returns 实际的字符串长度.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_log_dir(char* buff, int size);

/**
 * 得到日志文件的路径.
 *
 * @author daixian
 * @date 2019/3/19
 *
 * @param [in] buff 拷贝字符的buff.
 * @param      size buff大小.
 *
 * @returns 实际的字符串长度.
 */
extern "C" DLOG_EXPORT int __cdecl dlog_get_log_file_path(char* buff, int size);

#endif // !_DLOG_H_
