using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;


namespace SysProgSharpDmitrieva
{
    public partial class Form1 : Form
    {

        Process? ChildProcess = null;
        EventWaitHandle StartEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        EventWaitHandle StopEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StopEvent");
        EventWaitHandle ConfirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        EventWaitHandle CloseEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseEvent");
        EventWaitHandle SendEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "SendEvent");

        public Form1()
        {
            InitializeComponent();
        }
        [DllImport("DllDmitrieva.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern void mapsend(int addr, string str);
        private void OnProcessExited(object sender, EventArgs e)
        {
            if (listBox.InvokeRequired)
            {
                listBox.Invoke(new Action(() => listBox.Items.Clear()));
            }
            else
            {
                listBox.Items.Clear();
            }

        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            if (ChildProcess == null || ChildProcess.HasExited)
            {
                ChildProcess = Process.Start("SysProgDmitrieva.exe");
                ChildProcess.EnableRaisingEvents = true;
                ChildProcess.Exited += OnProcessExited;
                listBox.Items.Add("Все потоки");
                listBox.Items.Add("Главный поток");
            }
            else
            {
                int n = (int)Counter.Value;
                int CountItems = listBox.Items.Count;
                for (int i = 0; i < n; ++i)
                {
                    StartEvent.Set();
                    ConfirmEvent.WaitOne();
                    listBox.Items.Add($"Thread {i + CountItems - 2}");
                }

            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            if (!(ChildProcess == null || ChildProcess.HasExited))
            {
                StopEvent.Set();
                ConfirmEvent.WaitOne();
                listBox.Items.RemoveAt(listBox.Items.Count - 1);
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (!(ChildProcess == null || ChildProcess.HasExited))
            {
                CloseEvent.Set();
                ConfirmEvent.WaitOne();
            }
        }

        private void SendButton_Click(object sender, EventArgs e)
        {
            if (!(ChildProcess == null || ChildProcess.HasExited))
            {
                mapsend(listBox.SelectedIndex, textBox.Text);
                SendEvent.Set();
                ConfirmEvent.WaitOne();
            }
        }
    }
}
