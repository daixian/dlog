[![Build Status](https://dev.azure.com/daixian/dlogPipeline/_apis/build/status/dlogPipeline-x64?branchName=master)](https://dev.azure.com/daixian/dlogPipeline/_build/latest?definitionId=2&branchName=master)
### 构建相关
* 1、 目前都是MD的编译方式，VS2017的运行时。
* 2、 使用了cvpkg，但是使用了自定义的windows-static-md。
* 3、 内部从glog换成的spdlog。

[Release下载](https://github.com/daixian/dlog/releases)

### 使用示例
1、下面是c#工程里的初始化，在最初的时候设置了debug级别以上的日志都立即flush，防止有的程序上来一启动就异常退出导致日志没有写进文件=。=
``` c#
//初始化日志文件在 %APPDATA%/FViewTool/log 文件夹
DLog.dlog_init("FViewTool\\log", "FViewTool", DLog.INIT_RELATIVE.APPDATA, false);
//设置立即flush的等级为debug
DLog.dlog_set_flush_on(DLog.LOG_THR.Debug);
//输出一条日志,立即flush了
DLog.LogI("App Start!");
```
2、日志分为文件、控制台、内存分别独立的三路日志。通常后台工作时可以关闭控制台日志和内存日志。在有的时候发现日志性能极低，那么可能是stdout被重定向到了某个控制台。
``` c#
//设置只有err级别的日志才输出到控制台
DLog.dlog_set_console_thr(DLog.LOG_THR.err);

//设置只有err级别的日志才输出到控制台
DLog.dlog_set_file_thr(DLog.LOG_THR.err);
```
3、内存日志是一个暂存在内存里的日志文本队列，用于c#把c++端的日志从内存队列里获取出来，然后显示到UI界面等等。c#部分提取内存日志只能一条一条提取。
``` c#
//设置使能内存日志（默认是不使能的）
DLog.dlog_memory_log_enable(true);

//创建一个字符串buff，减少GC
StringBuilder buff = new StringBuilder(512);
//提取一条内存日志，写入到buff的起始
DLog.dlog_get_memlog(buff, 0, buff.Capacity);
```