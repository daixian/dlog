[![Build Status](https://dev.azure.com/daixian/dlogPipeline/_apis/build/status/dlogPipeline-x64?branchName=master)](https://dev.azure.com/daixian/dlogPipeline/_build/latest?definitionId=2&branchName=master)
### 构建相关
* 1、 目前已经换成了conan的构建和包管理。
* 2、 上传到github的CI待完成...
* 3、 内部从glog换成的spdlog。
* 4、 去掉了boost，换成了Poco库。

## NDK
* 会提示找不到-lpthread.[参考 ar cr libpthread.a](https://stackoverflow.com/questions/57289494/ndk-r20-ld-ld-error-cannot-find-lpthread)

[Release下载(x64,x86)](https://github.com/daixian/dlog/releases)

### 使用示例
1、下面是c#工程里的初始化。初始化使用一个文件夹路径，然后会在这个文件夹里新建一个日志文件，目前会自动删除3天前的日志文件。第二个参数为你的程序名，会用来给日志文件命名。在最初的时候设置了debug级别以上的日志都立即flush，防止有的程序一启动就异常崩溃退出导致没有日志写进文件=。=
``` c#
//初始化日志文件在 %APPDATA%/FViewTool/log 文件夹，日志文件名形如"FViewTool.20190317-214418.log"
DLog.dlog_init("FViewTool\\log", "FViewTool", DLog.INIT_RELATIVE.APPDATA, false);

//设置立即flush的等级为debug
DLog.dlog_set_flush_on(DLog.LOG_THR.debug);

//输出一条日志,立即flush了
DLog.LogI("App Start!");

//或者直接调用flush也可以...
DLog.dlog_flush();
```
2、日志分为文件、控制台、内存分别独立的三路日志。通常后台工作时可以关闭控制台日志和内存日志。有的时候如果发现日志性能极低，那么有可能是stdout被重定向到了某个看不见控制台，可以尝试关闭控制台输出。通常频繁的日志不能输出到控制台。
``` c#
//设置只有err级别的日志才输出到控制台
DLog.dlog_set_console_thr(DLog.LOG_THR.err);

//设置只有err级别的日志才写到文件
DLog.dlog_set_file_thr(DLog.LOG_THR.err);

//禁用控制台输出日志
 DLog.dlog_console_log_enable(false);
```
3、内存日志是一个暂存在内存里的日志文本队列，用于c#把c++端写的日志从内存队列里获取出来，然后显示到c#的UI界面等等。c#部分提取内存日志只能从内存队列里一条一条提取。如果提取不到日志则dlog_get_memlog()函数返回0，可以定时提取一批然后显示到UI上.
``` c#
//设置使能内存日志（默认是不使能的）
DLog.dlog_memory_log_enable(true);

//创建一个字符串buff，减少GC
StringBuilder buff = new StringBuilder(512);
//提取一条内存日志，写入到buff的起始
DLog.dlog_get_memlog(buff, 0, buff.Capacity);
```
4、c++接口和上面的c#一致。




### 其他记录

boost的记录：
    default_options = ["shared=False",
                       "header_only=False",
                       "error_code_header_only=False",
                       "system_no_deprecated=False",
                       "asio_no_deprecated=False",
                       "filesystem_no_deprecated=False",
                       "fPIC=True",
                       "layout=system",
                       "magic_autolink=False",
                       "python_executable=None",
                       "python_version=None",
                       "namespace=boost",
                       "namespace_alias=False",
                       "zlib=True",
                       "bzip2=True",
                       "lzma=False",
                       "zstd=False",
                       "use_icu=False",
                       "segmented_stacks=False",
                       "extra_b2_flags=None"
