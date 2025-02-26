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
            StartButton = new Button();
            StopButton = new Button();
            SendButton = new Button();
            Counter = new NumericUpDown();
            listBox = new ListBox();
            textBox = new TextBox();
            ((System.ComponentModel.ISupportInitialize)Counter).BeginInit();
            SuspendLayout();
            // 
            // StartButton
            // 
            StartButton.Location = new Point(27, 46);
            StartButton.Name = "StartButton";
            StartButton.Size = new Size(94, 29);
            StartButton.TabIndex = 0;
            StartButton.Text = "Start";
            StartButton.UseVisualStyleBackColor = true;
            StartButton.Click += StartButton_Click;
            // 
            // StopButton
            // 
            StopButton.Location = new Point(27, 100);
            StopButton.Name = "StopButton";
            StopButton.Size = new Size(94, 29);
            StopButton.TabIndex = 1;
            StopButton.Text = "Stop";
            StopButton.UseVisualStyleBackColor = true;
            StopButton.Click += StopButton_Click;
            // 
            // SendButton
            // 
            SendButton.Location = new Point(27, 153);
            SendButton.Name = "SendButton";
            SendButton.Size = new Size(94, 29);
            SendButton.TabIndex = 2;
            SendButton.Text = "Send";
            SendButton.UseVisualStyleBackColor = true;
            // 
            // Counter
            // 
            Counter.Location = new Point(27, 211);
            Counter.Name = "Counter";
            Counter.Size = new Size(94, 27);
            Counter.TabIndex = 3;
            Counter.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // listBox
            // 
            listBox.FormattingEnabled = true;
            listBox.Location = new Point(294, 46);
            listBox.Name = "listBox";
            listBox.Size = new Size(265, 164);
            listBox.TabIndex = 4;
            // 
            // textBox
            // 
            textBox.Location = new Point(27, 273);
            textBox.Name = "textBox";
            textBox.Size = new Size(94, 27);
            textBox.TabIndex = 5;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(644, 404);
            Controls.Add(textBox);
            Controls.Add(listBox);
            Controls.Add(Counter);
            Controls.Add(SendButton);
            Controls.Add(StopButton);
            Controls.Add(StartButton);
            Name = "Form1";
            Text = "Dmitrieva";
            ((System.ComponentModel.ISupportInitialize)Counter).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button StartButton;
        private Button StopButton;
        private Button SendButton;
        private NumericUpDown Counter;
        private ListBox listBox;
        private TextBox textBox;
    }
}
