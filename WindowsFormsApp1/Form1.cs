using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using xuexue;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            Fun1();
            DLog.dlog_init("\\log", "MRSystem", false);
        }


        [DllImport(@"Dll1.dll")]
        public static extern void Fun1();



        private void button1_Click(object sender, EventArgs e)
        {
            DLog.LogW("C#输出的日志！");
        }
    }
}
