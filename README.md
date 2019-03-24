[![Build Status](https://dev.azure.com/daixian/dlogPipeline/_apis/build/status/dlogPipeline-x64?branchName=master)](https://dev.azure.com/daixian/dlogPipeline/_build/latest?definitionId=2&branchName=master)
### 1、 目前都是MD的编译方式，VS2017的运行时.
### 2、 内部从glog换成的spdlog.

[Release下载](https://github.com/daixian/dlog/releases)

#### 使用示例
c#工程里的初始化
``` c#
//初始化日志文件在 %APPDATA%/FViewTool/log 文件夹
DLog.dlog_init("FViewTool\\log", "FViewTool", DLog.INIT_RELATIVE.APPDATA, false);
//设置立即flush的等级为最低
DLog.dlog_set_flush_on(DLog.LOG_THR.Debug);
//输出一条日志,立即flush了
DLog.LogI("App Start!");
```