using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Threading;
using xuexue;

//Copy "$(SolutionDir)x64\Release\dlog.dll" "$(TargetDir)dlog.dll"

namespace UnitTest
{
    [TestClass]
    public class UnitTest1
    {

        [TestMethod]
        public void TestMethodDLog()
        {
            int ThreadStartCount = 0;
            int DoneCount = 0;

            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");

            DLog.dlog_init(logDir);
            StringBuilder sb = new StringBuilder(256);
            DLog.dlog_get_log_dir(sb);
            DLog.dlog_memory_log_enable(true);

            for (int threadCount = 0; threadCount < 20; threadCount++)//20个线程一起写
            {
                Task.Run(() =>
                {
                    ThreadStartCount++;
                    for (int i = 0; i < 5000; i++)
                    {
                        xuexue.DLog.LogI($"测试日志{i}");

                    }
                    DoneCount++;
                });
            }

            while (true)
            {
                Thread.Sleep(1);
                if (DoneCount == ThreadStartCount)
                {
                    break;
                }
            }

            StringBuilder msg = new StringBuilder(256);
            int msgCount = 0;
            while (true)
            {
                if (DLog.dlog_get_memlog(msg, 0, 256) > 0)
                {
                    msgCount++;
                    Assert.IsTrue(msg.ToString().Contains("测试日志"));//检查文本内容是否有大问题
                }
                else
                {
                    break;
                }
            }

            Assert.IsTrue(msgCount == 20 * 5000);//检查消息条数是否漏了

            DLog.dlog_close();//必须要关闭，否则有线程在后台还

        }
 

        [TestMethod]
        public void TestMethodDLogSpeed()
        {
            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");
            xuexue.DLog.dlog_init(logDir, "速度测试");

            for (int i = 0; i < 100000; i++)
            {
                //xuexue.DLog.LogI("测试日志123");//也只要960ms
                //xuexue.DLog.LogI($"测试日志123");//也只要960ms
                xuexue.DLog.LogI($"测试日志123{i}");//也只要1s,速度基本不变
            }

            DLog.dlog_close();
        }
    }


}
