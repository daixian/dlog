﻿using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Threading;
using xuexue;

//Copy "$(SolutionDir)x64\Release\dlog.dll" "$(ProjectDir)dlog.dll"


namespace UnitTest
{
    [TestClass]
    public class UnitTest1
    {

        [TestMethod]
        public void TestMethodDLogMemlog()
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
                    //要注意条数不能太多了，否则超出内存日志的缓存上限，会造成后面Assert条数失败
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
        public void TestMethodDLogInitClose()
        {
            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");
            if (Directory.Exists(logDir))
            {
                try
                {
                    string[] files = Directory.GetFiles(logDir);
                    for (int i = 0; i < files.Length; i++)
                    {
                        File.Delete(files[i]);
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }

            int count = 0;

            for (int i = 0; i < 50; i++)
            {
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_close();
                count++;
            }

            for (int i = 0; i < 50; i++)
            {
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_close();
                count++;
            }


            for (int i = 0; i < 50; i++)
            {
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.dlog_close();
                count++;
            }

            for (int i = 0; i < 50; i++)
            {
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_init(logDir, $"开关测试二{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_init(logDir, $"开关测试二{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_close();
                DLog.dlog_close();
                count++;
            }

            for (int i = 0; i < 50; i++)
            {
                DLog.dlog_init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.dlog_close();
                DLog.dlog_close();
                count++;
            }

            string[] logfiles = Directory.GetFiles(logDir);
            Assert.IsTrue(logfiles.Length == 300);
        }

        [TestMethod]
        public void TestMethodDLogMT()
        {
            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");
            if (Directory.Exists(logDir))
            {
                try
                {
                    string[] files = Directory.GetFiles(logDir);
                    for (int i = 0; i < files.Length; i++)
                    {
                        File.Delete(files[i]);
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }
            xuexue.DLog.dlog_init(logDir, "线程测试");

            //设置一个一般的控制变量，多个线程对它读写
            int controlFlag = 0;

            int doneCount = 0;

            Interlocked.Increment(ref doneCount);
            Task.Run(()=> {
                int count = 0;
                while (count<1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });

            Interlocked.Increment(ref doneCount);
            Task.Run(() => {
                int count = 0;
                while (count < 1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });


            Interlocked.Increment(ref doneCount);
            Task.Run(() => {
                int count = 0;
                while (count < 1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);//这里++还是不行
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);//这里--还是不行
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });


            Interlocked.Increment(ref doneCount);
            Task.Run(() => {
                int count = 0;
                while (count < 1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });

            while (true)
            {
                Thread.Sleep(1);
                if (doneCount == 0)
                {
                    break;
                }
            }

            DLog.dlog_close();
        }

    }
}
