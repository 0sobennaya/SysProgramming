using System.Diagnostics;
using System.Linq.Expressions;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;


namespace SysProgSharpDmitrieva
{
    public partial class Form1 : Form
    {
        HashSet<int> OtherIDs = [];
        private volatile bool _running = true;
        public enum MessageTypes : int
        {
            MT_INIT,
            MT_EXIT,
            MT_GETDATA,
            MT_DATA,
            MT_NODATA,
            MT_CONFIRM,
            MT_NEWSESSION,
        };

        public enum MessageRecipients : int
        {
            MR_BROKER = 10,
            MR_ALL = 50,
            MR_USER = 100
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        struct MessageHeader
        {
            [MarshalAs(UnmanagedType.I4)]
            public MessageRecipients to;
            [MarshalAs(UnmanagedType.I4)]
            public MessageRecipients from;
            [MarshalAs(UnmanagedType.I4)]
            public MessageTypes type;
            [MarshalAs(UnmanagedType.I4)]
            public int size;
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        struct MessageTransfer
        {
            public MessageHeader header;
            public IntPtr data;
            public int clientID;
        };

        [DllImport("DLLDmitrieva.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        static extern MessageTransfer SendMsg(int to, MessageTypes type, string data = "");

        [DllImport("DLLDmitrieva.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        static extern void FreeMessageTransfer(MessageTransfer msg);

        void ProcessMessages()
        {
            while (_running)
            {
                var m = SendMsg((int)MessageRecipients.MR_BROKER, MessageTypes.MT_GETDATA);
                switch (m.header.type)
                {
                    case MessageTypes.MT_DATA:
                        messagesBox.Invoke(new Action(() => {
                            messagesBox.Items.Add($"[{m.header.from}>] {Marshal.PtrToStringUni(m.data)}");
                        }));
                        FreeMessageTransfer(m);
                        break;
                    case MessageTypes.MT_INIT:
                        OtherIDs.Add((int)m.header.from);
                        RefreshusersBox();
                        break;
                    case MessageTypes.MT_EXIT:
                        if (m.header.from == MessageRecipients.MR_BROKER)
                            return;
                        else
                        {
                            OtherIDs.Remove((int)m.header.from);
                            RefreshusersBox();
                        }
                        break;
                    default:
                        Thread.Sleep(100);
                        break;
                }
            }
        }

        private void RefreshusersBox()
        {
            usersBox.DataSource = null;
            usersBox.DataSource = OtherIDs.Select(id => new DisplayUser { Id = id }).ToList();
        }
        public Form1()
        {
            InitializeComponent();

            var m = SendMsg((int)MessageRecipients.MR_BROKER, (int)MessageTypes.MT_INIT);

            if (m.header.type == MessageTypes.MT_INIT)
            {
                OtherIDs.Add(10);
                OtherIDs.Add(50);
                RefreshusersBox();
                messagesBox.Items.Add($"Ваш clientID: {m.clientID}");
            }

            Thread t = new Thread(ProcessMessages);
            t.Start();
        }

        private void OnProcessExited(object sender, EventArgs e)
        {
            if (usersBox.InvokeRequired)
            {
                usersBox.Invoke(new Action(() => usersBox.Items.Clear()));
            }
            else
            {
                usersBox.Items.Clear();
            }

        }





        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            _running = false;
        }

        private void SendButton_Click(object sender, EventArgs e)
        {
            string message = textBox.Text;
            int to = (usersBox.SelectedItem as DisplayUser).Id;

            string to_name = to == 50 ? "Всем" : (to == 10 ? "Серверу" : to.ToString());

            messagesBox.Items.Add($"[{to_name}<] {message}");

            var m = SendMsg(to, MessageTypes.MT_DATA, message);

            if (m.header.type == MessageTypes.MT_CONFIRM)
            {
                textBox.Text = string.Empty;
            }
        }
        public class DisplayUser
        {
            public int Id { get; set; }
            public string DisplayName
            {
                get
                {
                    return Id switch
                    {
                        (int)MessageRecipients.MR_BROKER => "Главный поток",
                        (int)MessageRecipients.MR_ALL => "Все потоки",
                        _ => $"Пользователь {Id}"
                    };
                }
            }

            public override string ToString()
            {
                return DisplayName;
            }
        }

    }
}
