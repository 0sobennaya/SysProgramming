using System.Diagnostics;
using System.Linq.Expressions;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;


namespace SysProgSharpDmitrieva
{
    public partial class Form1 : Form
    {
        enum MessageType
        {
            INIT,
            EXIT,
            START,
            SEND,
            STOP,
            CONFIRM,
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        struct Header
        {
            [MarshalAs(UnmanagedType.I4)]
            public MessageType type;
            [MarshalAs(UnmanagedType.I4)]
            public int num;
            [MarshalAs(UnmanagedType.I4)]
            public int addr;
            [MarshalAs(UnmanagedType.I4)]
            public int size;
        };

        [DllImport("DllDmitrieva.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        static extern Header sendClient(MessageType type, int num = 0, int addr = 0, string str = "");

        public Form1()
        {
            InitializeComponent();
            Header h = sendClient(MessageType.INIT);
            if (h.type == MessageType.CONFIRM)
            {
                listBox.Items.Add("Все потоки");
                listBox.Items.Add("Главный поток");
                for (int i = 0; i < h.num; ++i)
                {
                    listBox.Items.Add($"Thread {i}");
                }
            }
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

        private void Update_List(int m)
        {
            int n = listBox.Items.Count - 2;
            if (m >= n)
            {
                for (int i = n; i < m; ++i)
                {
                    listBox.Items.Add($"Thread {i}");
                }
            }
            else
            {
                for (int i = m; i < n; ++i)
                {
                    listBox.Items.RemoveAt(listBox.Items.Count - 1);
                }
            }
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            int n = (int)Counter.Value;
            Header h = sendClient(MessageType.START, n);
            if (h.type == MessageType.CONFIRM)
            {
                Update_List(h.num);
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            Header h = sendClient(MessageType.STOP);
            if (h.type == MessageType.CONFIRM)
            {
                Update_List(h.num);
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                sendClient(MessageType.EXIT);
            }
            catch (Exception ex)
            {
                return;
            }


        }

        private void SendButton_Click(object sender, EventArgs e)
        {

            Header h = sendClient(MessageType.SEND, 0, listBox.SelectedIndex, textBox.Text);
            if (h.type == MessageType.CONFIRM)
            {
                textBox.Text = "";
            }

        }

    }
}
