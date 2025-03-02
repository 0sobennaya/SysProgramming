using System.Diagnostics;

namespace SysProgSharpDmitrieva
{
    public partial class Form1 : Form
    {
        Process? childProcess = null;
        System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.ManualReset, "StopEvent");
        System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");

        public Form1()
        {
            InitializeComponent();
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                childProcess = Process.Start("SysProgDmitrieva.exe");
                childProcess.EnableRaisingEvents = true;
                confirmEvent.WaitOne();
                listBox.Items.Add($"test");
                listBox.SelectedIndex = 0;
            }
            else
            {
                int value = (int)Counter.Value;
                int j = listBox.Items.Count;
                for (int i = 0; i < value; i++)
                {
                    startEvent.Set();
                    confirmEvent.WaitOne();
                    listBox.Items.Add($"thread {j + i - 1}");
                }
                listBox.SelectedIndex = listBox.Items.Count - 1;
            }

        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            if (!(childProcess == null || childProcess.HasExited))
            {
                stopEvent.Set();
                confirmEvent.WaitOne();
            }

        }
    }
}
