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
        /// 如果使用相对目录,那么程序会将它理解为相对模块目录,路径例如 char* logDir = "\\log",char* program = "dlog".
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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern int dlog_init(string logDir = "log", string program = "dlog",
            INIT_RELATIVE dir_relatvie = INIT_RELATIVE.APPDATA,
            bool isForceInit = false);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 关闭模块. </summary>
        ///
        /// <remarks> Dx, 2018/4/22. </remarks>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern int dlog_close();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置整个log使能. </summary>
        ///
        /// <remarks> Dx, 2018/4/23. </remarks>
        ///
        /// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_enable(bool enable);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置控制台log是否使能. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_console_log_enable(bool enable);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="fileThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_set_file_thr(LOG_THR fileThr);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 得到Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入.
        /// </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern int dlog_get_file_thr();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="usualThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_set_memory_thr(LOG_THR memoryThr);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="usualThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern int dlog_get_memory_thr();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到Dlog的控制台日志门限,大于等于该优先级的日志都会写入. </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///
        /// <param name="usualThr"> The usual thr. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern int dlog_get_console_thr();

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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_set_console_thr(LOG_THR LogSeverity);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置立即刷新的flush on. </summary>
        ///
        /// <remarks> Dx, 2019/3/18. </remarks>
        ///
        /// <param name="LogSeverity"> The log severity. </param>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_set_flush_on(LOG_THR LogSeverity);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置每隔多长时间flush的间隔时间. </summary>
        ///
        /// <remarks> Dx, 2019/4/17. </remarks>
        ///
        /// <param name="second"> 秒数. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_set_flush_every(int second);

        /// <summary>
        /// 立即刷新
        /// </summary>
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_flush();

        /// <summary>
        /// Debug级别日志,它是一个varargs的函数
        /// </summary>
        /// <param name="strFormat"></param>
        public static void LogD(string str)
        {
            int count = Encoding.UTF8.GetByteCount(str);
            byte[] bytes = new byte[count + 1];//自动置0了
            Encoding.UTF8.GetBytes(str, 0, str.Length, bytes, 0);
            LogD(bytes);
        }

        /// <summary>
        /// Info级别日志,它是一个varargs的函数
        /// </summary>
        /// <param name="strFormat"></param>
        public static void LogI(string str)
        {
            int count = Encoding.UTF8.GetByteCount(str);
            byte[] bytes = new byte[count + 1];
            Encoding.UTF8.GetBytes(str, 0, str.Length, bytes, 0);
            LogI(bytes);
        }

        /// <summary>
        /// Warnning级别日志,它是一个varargs的函数
        /// </summary>
        /// <param name="strFormat"></param>
        public static void LogW(string str)
        {
            int count = Encoding.UTF8.GetByteCount(str);
            byte[] bytes = new byte[count + 1];
            Encoding.UTF8.GetBytes(str, 0, str.Length, bytes, 0);
            LogW(bytes);
        }

        /// <summary>
        /// Error级别日志,它是一个varargs的函数
        /// </summary>
        /// <param name="strFormat"></param>
        public static void LogE(string str)
        {
            int count = Encoding.UTF8.GetByteCount(str);
            byte[] bytes = new byte[count + 1];
            Encoding.UTF8.GetBytes(str, 0, str.Length, bytes, 0);
            LogE(bytes);
        }

        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogD(byte[] str);


        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogI(byte[] str);


        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogW(byte[] str);


        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void LogE(byte[] str);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 设置内存log是否使能. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
        ///-------------------------------------------------------------------------------------------------
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void dlog_memory_log_enable(bool enable);

        /// <summary>
        /// 提取内存中的日志，如果给的buffer过短那么就只能得到不完整的信息.
        /// </summary>
        /// <param name="buff"></param>
        /// <param name="offset"></param>
        /// <param name="count"></param>
        /// <returns>写入buff的的消息长度</returns>
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        public static extern int dlog_get_memlog(StringBuilder buff, int offset, int count);


        /// <summary>
        /// 提取内存中的日志，如果给的buffer过短那么就只能得到不完整的信息.
        /// </summary>
        public static string dlog_get_memlog()
        {
            StringBuilder buff = new StringBuilder(512);
            dlog_get_memlog(buff, 0, buff.Capacity);
            return buff.ToString();
        }

        /// <summary>
        /// 提取内存中的日志，如果给的buffer过短那么就只能得到不完整的信息.
        /// </summary>
        public static string dlog_get_memlog(StringBuilder buff)
        {
            buff.Clear();
            dlog_get_memlog(buff, 0, buff.Capacity);
            return buff.ToString();
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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        private static extern int dlog_get_appdata_dir(StringBuilder buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 得到appdata的路径,目录末尾不带斜杠"C:\\Users\\dx\\AppData\\Roaming".
        /// </summary>
        ///
        /// <remarks> Dx, 2019/3/19. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string dlog_get_appdata_dir()
        {
            StringBuilder buff = new StringBuilder(512);
            dlog_get_appdata_dir(buff, buff.Capacity);
            return buff.ToString();
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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        private static extern int dlog_get_module_dir(StringBuilder buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到模块的路径,目录末尾不带斜杠. </summary>
        ///
        /// <remarks> Dx, 2019/3/19. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string dlog_get_module_dir()
        {
            StringBuilder buff = new StringBuilder(512);
            dlog_get_module_dir(buff, buff.Capacity);
            return buff.ToString();
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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        private static extern int dlog_get_log_dir(StringBuilder buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到日志文件夹的路径. </summary>
        ///
        /// <remarks> Dx, 2019/3/22. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string dlog_get_log_dir()
        {
            StringBuilder buff = new StringBuilder(512);
            dlog_get_log_dir(buff, buff.Capacity);
            return buff.ToString();
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
        [DllImport("dlog", CallingConvention = CallingConvention.Cdecl)]
        private static extern int dlog_get_log_file_path(StringBuilder buff, int size);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 得到日志文件的路径. </summary>
        ///
        /// <remarks> Dx, 2019/3/19. </remarks>
        ///
        /// <returns> 路径字符串. </returns>
        ///-------------------------------------------------------------------------------------------------
        public static string dlog_get_log_file_path()
        {
            StringBuilder buff = new StringBuilder(512);
            dlog_get_log_file_path(buff, buff.Capacity);
            return buff.ToString();
        }
    }
}