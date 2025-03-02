using System.Diagnostics;

namespace SysProgSharpDmitrieva
{
    public partial class Form1 : Form
    {
        Process? childProcess = null;
        System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.ManualReset, "StopEvent");
        System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        System.Threading.EventWaitHandle closeEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseEvent");

        public Form1()
        {
            InitializeComponent();
        }

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
            if (childProcess == null || childProcess.HasExited)
            {
                childProcess = Process.Start("SysProgDmitrieva.exe");
                childProcess.EnableRaisingEvents = true;
                childProcess.Exited += OnProcessExited;
                confirmEvent.WaitOne();
                listBox.Items.Add($"��� ������");
                listBox.Items.Add($"������� �����");
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
                    listBox.Items.Add($"thread {j + i - 2}");
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
                listBox.Items.RemoveAt(listBox.Items.Count - 1);
                listBox.SelectedIndex = listBox.Items.Count - 1;
            }

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (!(childProcess == null || childProcess.HasExited))
            {
                closeEvent.Set();
                confirmEvent.WaitOne();
                childProcess = null;
            }
        }
    }
}
