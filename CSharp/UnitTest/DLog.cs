using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace xuexue
{
    /// <summary>
    /// 调用dlog的C#类
    /// </summary>
    public class DLog
    {
        /// <summary>
        /// 日志的优先级设定
        /// </summary>
        public enum LOG_THR : int
        {
            trace = 0,
            debug = 1,
            info = 2,
            warn = 3,
            err = 4,
            critical = 5,
            off = 6,
        };

        /// <summary>
        /// 初始化目录的相对位置
        /// </summary>
        public enum INIT_RELATIVE : int
        {
            APPDATA = 0,
            MODULE = 1,
        };

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 模块初始化,日志文件夹路径可以使用绝对目录也可以使用相对目录(第三个参数进行相对位置的设置),
        /// 如果使用相对目录,那么程序会将它理解为相对模块目录,路径例如 char* logDir = "log",char* program = "dlog".
        /// isForceInit如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志.
        /// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
        /// 如果强制重设但是失败还是复用了那么返回3.
        /// </summary>
        ///
        /// <remarks>
        /// 如果函数输入的第一个参数是相对路径,则第三个参数生效.
        /// INIT_RELATIVE.APPDATA: 相对于AppData文件夹.
        /// INIT_RELATIVE.MODULE: 相对于dll文件自身文件夹.
        /// Dx, 2018/4/22.
        /// </remarks>
        ///
        /// <param name="logDir">      [in]日志文件夹路径名,如果是绝对路径,则第三个参数无效. </param>
        /// <param name="program">     [in]程序名. </param>
        /// <param name="dir_relatvie">(Optional)相对路径的相对位置. </param>
        /// <param name="isForceInit"> (Optional) 如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志. </param>
        ///
        /// <returns>
        /// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
        /// 如果强制重设但是失败还是复用了那么返回3.
        /// </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_init_wchar_filename", CharSet = CharSet.Unicode)]
        public static extern int Init(string logDir = "log", string program = "dlog",
            INIT_RELATIVE dir_relatvie = INIT_RELATIVE.APPDATA,
            bool isForceInit = false);


        /// <summary>
        /// 是否日志已经初始化了.
        /// </summary>
        /// <returns></returns>
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_is_initialized")]
        public static extern bool IsInitialized();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 关闭模块. </summary>
        ///
        /// <remarks> Dx, 2018/4/22. </remarks>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_close")]
        public static extern int Close();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置整个log使能. </summary>
        ///
        /// <remarks> Dx, 2018/4/23. </remarks>
        ///
        /// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_enable")]
        public static extern void Enable(bool enable);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置控制台log是否使能. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_console_log_enable")]
        public static extern void ConsoleLogEnable(bool enable);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="fileThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_set_file_thr")]
        public static extern void SetFileThr(LOG_THR fileThr);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 得到Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入.
        /// </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_file_thr")]
        public static extern int GetFileThr();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="usualThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_set_memory_thr")]
        public static extern void SetMemoryThr(LOG_THR memoryThr);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="usualThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_memory_thr")]
        public static extern int GetMemoryThr();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到Dlog的控制台日志门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="usualThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_console_thr")]
        public static extern int GetConsoleThr();

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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_set_console_thr")]
        public static extern void SetConsoleThr(LOG_THR LogSeverity);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置立即刷新的flush on. </summary>
        ///
        /// <remarks> Dx, 2019/3/18. </remarks>
        ///
        /// <param name="LogSeverity"> The log severity. </param>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_set_flush_on")]
        public static extern void SetFlushOn(LOG_THR LogSeverity);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置每隔多长时间flush的间隔时间. </summary>
        ///
        /// <remarks> Dx, 2019/4/17. </remarks>
        ///
        /// <param name="second"> 秒数. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_set_flush_every")]
        public static extern void SetFlushEvery(int second);

        /// <summary>
        /// 立即刷新
        /// </summary>
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_flush")]
        public static extern void Flush();

        /// <summary>
        /// Debug级别日志
        /// </summary>
        /// <param name="str"></param>
        public static void LogD(string str)
        {
            wLogMsg(LOG_THR.debug, str);
        }

        /// <summary>
        /// Info级别日志
        /// </summary>
        /// <param name="str"></param>
        public static void LogI(string str)
        {
            wLogMsg(LOG_THR.info, str);
        }

        /// <summary>
        /// Warnning级别日志
        /// </summary>
        /// <param name="str"></param>
        public static void LogW(string str)
        {
            wLogMsg(LOG_THR.warn, str);
        }

        /// <summary>
        /// Error级别日志
        /// </summary>
        /// <param name="str"></param>
        public static void LogE(string str)
        {
            wLogMsg(LOG_THR.err, str);
        }

        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogD(byte[] str);


        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogI(byte[] str);


        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogW(byte[] str);


        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogE(byte[] str);

        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogMsg(LOG_THR level, byte[] str);

        /// <summary>
        /// 这是使用UTF16的函数.
        /// </summary>
        /// <param name="level"></param>
        /// <param name="str"></param>
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern void wLogMsg(LOG_THR level, string str);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置内存log是否使能. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_memory_log_enable")]
        public static extern void MemoryLogEnable(bool enable);

        /// <summary>
        /// 提取内存中的日志，如果给的buffer过短那么就只能得到不完整的信息.
        /// </summary>
        /// <param name="buff"></param>
        /// <param name="offset"></param>
        /// <param name="count"></param>
        /// <returns>写入buff的的消息长度</returns>
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_memlog")]
        private static extern int dlog_get_memlog(byte[] buff, int offset, int count);


        /// <summary>
        /// 提取内存中的日志，如果给的buffer过短那么就只能得到不完整的信息.
        /// </summary>
        public static string GetMemorylog()
        {
            byte[] buff = new byte[2048];
            int len = dlog_get_memlog(buff, 0, buff.Length);
            return Encoding.UTF8.GetString(buff, 0, len);
        }

        /// <summary>
        /// 连续提取多条日志
        /// </summary>
        /// <returns></returns>
        public static List<string> GetMemorylogs()
        {
            List<string> msgs = new List<string>();
            byte[] buff = new byte[2048];

            int len;
            do
            {
                len = dlog_get_memlog(buff, 0, buff.Length);
                if (len > 0)
                {
                    msgs.Add(Encoding.UTF8.GetString(buff, 0, len));
                }

            } while (len > 0);

            return msgs;
        }

        /// <summary>
        /// 提取内存中的日志，如果给的buffer过短那么就只能得到不完整的信息.
        /// </summary>
        public static string GetMemorylog(byte[] buff)
        {
            int len = dlog_get_memlog(buff, 0, buff.Length);
            return Encoding.UTF8.GetString(buff, 0, len);
        }

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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_appdata_dir")]
        private static extern int dlog_get_appdata_dir(byte[] buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 得到appdata的路径,目录末尾不带斜杠"C:\\Users\\dx\\AppData\\Roaming".
        /// </summary>
        ///
        /// <remarks> Dx, 2019/3/19. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string GetAppdataDir()
        {
            byte[] buff = new byte[1024];
            int len = dlog_get_appdata_dir(buff, buff.Length);
            return Encoding.UTF8.GetString(buff, 0, len);
        }

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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_module_dir")]
        private static extern int dlog_get_module_dir(byte[] buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到模块的路径,目录末尾不带斜杠. </summary>
        ///
        /// <remarks> Dx, 2019/3/19. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string GetModuleDir()
        {
            byte[] buff = new byte[1024];
            int len = dlog_get_module_dir(buff, buff.Length);
            return Encoding.UTF8.GetString(buff, 0, len);
        }

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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_log_dir")]
        private static extern int dlog_get_log_dir(byte[] buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到日志文件夹的路径. </summary>
        ///
        /// <remarks> Dx, 2019/3/22. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string GetLogDir()
        {
            byte[] buff = new byte[1024];
            int len = dlog_get_log_dir(buff, buff.Length);
            return Encoding.UTF8.GetString(buff, 0, len);
        }

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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, EntryPoint = "dlog_get_log_file_path")]
        private static extern int dlog_get_log_file_path(byte[] buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到日志文件的路径. </summary>
        ///
        /// <remarks> Dx, 2019/3/19. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string GetLogFilePath()
        {
            byte[] buff = new byte[1024];
            int len = dlog_get_log_file_path(buff, buff.Length);
            return Encoding.UTF8.GetString(buff, 0, len);
        }
    }
}