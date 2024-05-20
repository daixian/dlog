[![Build Status](https://dev.azure.com/daixian/dlogPipeline/_apis/build/status/dlogPipeline-x64?branchName=master)](https://dev.azure.com/daixian/dlogPipeline/_build/latest?definitionId=2&branchName=master)
## 记录
1. 目前已经换成了conan的构建和包管理。
2. 内部从glog换成的spdlog。
3. 完全去掉了boost，换成了Poco库。
4. 使用conan的CI已经完成 -> [Release下载(x64,x86)](https://github.com/daixian/dlog/releases)
5. 随便的加了个日志的加密接口。

## 编译库的方法
1. 使用pip安装conan构建工具：
    ``` bash
    pip install conan -U
    conan remote add conan-center https://conan.bintray.com
    conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
    ```
2. clone该项目的主分支。
3. 在终端中执行项目根目录下的build.bat脚本即可，如果网速过慢的话，我目前的解决方法是给上面两个服务器搭反向代理，编译完成后可以在**conan的文件夹**中或者**项目根目录下的/temp/archive**中看到编译结果如下:
    ``` bash
    dx@DESKTOP-IFNRFQ6:/mnt/d/Work/dlog$ tree ./temp/
    ./temp/
    └── archive
        └── dlog
            ├── Windows_Visual Studio16_x86_64_MD
            │   ├── bin
            │   │   └── dlog.dll
            │   ├── include
            │   │   └── dlog
            │   │       └── dlog.h
            │   └── lib
            │       └── dlog.lib
            ├── Windows_Visual Studio16_x86_64_MT
            │   ├── bin
            │   │   └── dlog.dll
            │   ├── include
            │   │   └── dlog
            │   │       └── dlog.h
            │   └── lib
            │       └── dlog.lib
            ├── Windows_Visual Studio16_x86_MD
            │   ├── bin
            │   │   └── dlog.dll
            │   ├── include
            │   │   └── dlog
            │   │       └── dlog.h
            │   └── lib
            │       └── dlog.lib
            └── Windows_Visual Studio16_x86_MT
                ├── bin
                │   └── dlog.dll
                ├── include
                │   └── dlog
                │       └── dlog.h
                └── lib
                    └── dlog.lib

    22 directories, 12 files
    ```

## NDK
* 会提示找不到-lpthread.[参考 ar cr libpthread.a](https://stackoverflow.com/questions/57289494/ndk-r20-ld-ld-error-cannot-find-lpthread)

## C++使用示例
1. 首先强烈建议整个日志系统使用UTF8的编码。
2. 整个库只引用一个dlog.h头文件即可，在头文件上方写一个宏定义DLOG_UTF8，这是为了设置vs编译器编译代码字符串成UTF8。
3. 使用LogI这样的函数进行输出即可，格式化控制符和C言语的printf完全一致。
    ``` cpp
    #define DLOG_UTF8
    #include "dlog/dlog.h"

    //在初始化之前进行一些准备设置
    dlog_set_is_use_utf8_bom(true);
    dlog_memory_log_enable(false);
    dlog_console_log_enable(false);

    //执行初始化
    int res = dlog_init("log", "speed", dlog_init_relative::MODULE);
    //判断初始化是否成功
    ASSERT_TRUE(dlog_is_initialized());

    for (size_t i = 0; i < 100; i++) {
        LogI("试试❀❀❀❀❀❀❀❀❀,%d",i);
    }
    dlog_flush();

    //...

    dlog_close();
    ```

## C#使用示例
1、下面是C#工程里的初始化，C#的封装使用[Dlog.cs文件](https://github.com/daixian/dlog/blob/master/CSharp/UnitTest/DLog.cs)。初始化使用一个文件夹路径，然后会在这个文件夹里新建一个日志文件，目前会自动删除3天前的数量在50个以上的日志文件。第二个参数为你的程序名，会用来给日志文件命名，所以不要使用一些特殊符号。默认是warning等级的日志会立即flush，可以在最初的时候设置了debug级别以上的日志都立即flush，防止有的程序一启动就异常崩溃退出导致没有日志写进文件=。=
``` c#
//初始化日志文件在 %APPDATA%/FViewTool/log 文件夹，日志文件名形如"FViewTool.20190317-214418.log"
DLog.Init("FViewTool\\log", "FViewTool", DLog.INIT_RELATIVE.APPDATA, false);

//设置立即flush的等级为debug
DLog.SetFlushOn(DLog.LOG_THR.debug);

//输出一条日志,立即flush了
DLog.LogI("App Start!");

//或者直接调用flush也可以...
DLog.Flush();
```
2、日志分为文件、控制台、内存分别独立的三路日志。通常后台工作时可以关闭控制台日志和内存日志。有的时候如果发现日志性能极低，那么有可能是stdout被重定向到了某个看不见控制台，可以尝试关闭控制台输出。通常频繁的日志不能输出到控制台。
``` c#
//设置只有err级别的日志才输出到控制台
DLog.SetConsoleThr(DLog.LOG_THR.err);

//设置只有err级别的日志才写到文件
DLog.SetFileThr(DLog.LOG_THR.err);

//禁用控制台输出日志,这个函数最好放在init之前执行
 DLog.ConsoleLogEnable(false);
```
3、内存日志是一个暂存在内存里的日志文本队列，用于c#把c++端写的日志从内存队列里获取出来，然后显示到c#的UI界面等等。c#部分提取内存日志只能从内存队列里一条一条提取。如果提取不到日志则dlog_get_memlog()函数返回0，可以定时提取一批然后显示到UI上.
``` c#
//设置使能内存日志（默认是不使能的）
DLog.MemoryLogEnable(true);

//提取一批内存日志,可以见GetMemorylog()函数的实现
List<string> logs = DLog.GetMemorylogs();
```

## 老版本conan的python环境
``` shell
python -m venv .venv

.\.venv\Scripts\Activate.ps1
# 确认环境激活后执行下面的
pip install -r requirements.txt
```
