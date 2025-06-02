namespace SysProgSharpDmitrieva
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            SendButton = new Button();
            usersBox = new ListBox();
            textBox = new TextBox();
            messagesBox = new ListBox();
            SuspendLayout();
            // 
            // SendButton
            // 
            SendButton.Location = new Point(538, 10);
            SendButton.Name = "SendButton";
            SendButton.Size = new Size(94, 29);
            SendButton.TabIndex = 2;
            SendButton.Text = "Send";
            SendButton.UseVisualStyleBackColor = true;
            SendButton.Click += SendButton_Click;
            // 
            // usersBox
            // 
            usersBox.FormattingEnabled = true;
            usersBox.Location = new Point(12, 60);
            usersBox.Name = "usersBox";
            usersBox.Size = new Size(226, 324);
            usersBox.TabIndex = 4;
            // 
            // textBox
            // 
            textBox.Location = new Point(12, 12);
            textBox.Name = "textBox";
            textBox.Size = new Size(520, 27);
            textBox.TabIndex = 5;
            // 
            // messagesBox
            // 
            messagesBox.FormattingEnabled = true;
            messagesBox.Location = new Point(244, 61);
            messagesBox.Name = "messagesBox";
            messagesBox.Size = new Size(388, 324);
            messagesBox.TabIndex = 6;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(644, 404);
            Controls.Add(messagesBox);
            Controls.Add(textBox);
            Controls.Add(usersBox);
            Controls.Add(SendButton);
            Name = "Form1";
            Text = "Dmitrieva";
            FormClosed += Form1_FormClosed;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private Button SendButton;
        private ListBox usersBox;
        private TextBox textBox;
        private ListBox messagesBox;
    }
}
