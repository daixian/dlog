#pragma once
#include <string>

namespace dxlib {

class GLogHelper
{
  public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary> 构造，同时进行glog配置. </summary>
    ///
    /// <remarks> Dx, 2018/4/22. </remarks>
    ///
    /// <param name="program"> [in]程序名. </param>
    /// <param name="logDir">  [in]日志文件夹路径名（相对模块目录）. </param>
    ///-------------------------------------------------------------------------------------------------
    GLogHelper(const char* program, const char* logDir = "\\log");

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 析构:GLOG内存清理. </summary>
    ///
    /// <remarks> Surface, 2019/3/17. </remarks>
    ///-------------------------------------------------------------------------------------------------
    ~GLogHelper();

    //错误回调：（未能用）将信息输出到单独的文件和 LOG(ERROR)
    void static SignalHandle(const char* data, int size);

    /// <summary> 日志文件夹的记录(绝对路径). </summary>
    std::string logDirPath;

    /// <summary> 日志程序名的记录(glog中没有api来记录，所以要自己记录). </summary>
    std::string programName;

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 移除老的日志文件(3天前). </summary>
    ///
    /// <remarks> Surface, 2019/3/17. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void removeOldFile(long sec = 3600 * 24 * 3);
};

} // namespace dxlib