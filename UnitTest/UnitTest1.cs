using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Threading;

//Copy "$(SolutionDir)x64\Release\dlog.dll" "$(TargetDir)dlog.dll"

namespace UnitTest
{
    [TestClass]
    public class UnitTest1
    {

        [TestMethod]
        public void TestMethodDLog()
        {
            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");
            //if (Directory.Exists(logDir))
            //{
            //    Directory.Delete(logDir);
            //}
            xuexue.DLog.dlog_init(logDir);
            StringBuilder sb = new StringBuilder(256);
            xuexue.DLog.dlog_get_log_dir(sb);

            for (int thraadCount = 0; thraadCount < 100; thraadCount++)
            {
                Task.Run(() =>
                {
                    ThreadStartCount++;
                    for (int i = 0; i < 1000; i++)
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
            xuexue.DLog.dlog_close();//必须要关闭，否则有线程在后台还
            Assert.IsTrue(true );
        }

        volatile int ThreadStartCount = 0;
        volatile int DoneCount = 0;

 
    }
}
