using System.Runtime.InteropServices;
using System.Text;

namespace xuexue
{
    public class DLog
    {
        ///-------------------------------------------------------------------------------------------------
        /// <summary> 模块初始化. </summary>
        ///
        /// <remarks> Dx, 2018/4/22. </remarks>
        ///
        /// <param name="logDir">  [in] (Optional)
        ///                        日志文件夹路径名（相对模块目录）. </param>
        /// <param name="program"> [in] (Optional) 程序名. </param>
        ///
        /// <returns> 如果之前未被初始化返回0，否则返回1. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog")]
        public static extern int dlog_init(string logDir = "\\log", string program = "dlog");

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 关闭模块. </summary>
        ///
        /// <remarks> Dx, 2018/4/22. </remarks>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog")]
        public static extern int dlog_close();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到当前设置的日志目录. </summary>
        ///
        /// <remarks> Dx, 2018/4/22. </remarks>
        ///
        /// <param name="sb"> [out] If non-null, the result. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog")]
        public static extern void dlog_get_log_dir(StringBuilder sb);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置整个log使能. </summary>
        ///
        /// <remarks> Dx, 2018/4/23. </remarks>
        ///
        /// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog")]
        public static extern void dlog_enable(bool enable);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 设置最高某一级日志输出到控制台，大于等于这一级别的日志都会输出到控制台，小于这一级别的日志不会输出到控制台.
        /// 参数有DLOG_INFO,DLOG_WARNING,DLOG_ERROR, DLOG_FATAL.
        /// </summary>
        ///
        /// <remarks> Dx, 2018/4/23. </remarks>
        ///
        /// <param name="LogSeverity"> 大于等于这一级的日志都会输出到控制台. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog")]
        public static extern void dlog_FLAGS_stderrthreshold(int LogSeverity);

        [DllImport("dlog")]
        public static extern void LogI(string strFormat);

        [DllImport("dlog")]
        public static extern void LogW(string strFormat);

        [DllImport("dlog")]
        public static extern void LogE(string strFormat);

        [DllImport("dlog")]
        public static extern void dlog_memory_log_enable(bool enable);

        /// <summary>
        /// 提取内存中的日志
        /// </summary>
        /// <param name="buff"></param>
        /// <param name="offset"></param>
        /// <param name="count"></param>
        /// <returns></returns>
        [DllImport("dlog")]
        public static extern int dlog_get_memlog(StringBuilder buff, int offset, int count);
    }
}