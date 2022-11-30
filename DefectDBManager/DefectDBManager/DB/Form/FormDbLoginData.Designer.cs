namespace DefectDBManager
{
    partial class FormDbLoginData
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lblTitle = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lblDomain = new System.Windows.Forms.Label();
            this.tbDbName = new System.Windows.Forms.TextBox();
            this.lblUserID = new System.Windows.Forms.Label();
            this.tbUserID = new System.Windows.Forms.TextBox();
            this.lblPassword = new System.Windows.Forms.Label();
            this.tbPassword = new System.Windows.Forms.TextBox();
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.lblPort = new System.Windows.Forms.Label();
            this.tbPort = new System.Windows.Forms.TextBox();
            this.lblHostIP = new System.Windows.Forms.Label();
            this.tbHostIP = new System.Windows.Forms.TextBox();
            this.lblType = new System.Windows.Forms.Label();
            this.rbConStringType1 = new System.Windows.Forms.RadioButton();
            this.rbConStringType2 = new System.Windows.Forms.RadioButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblTitle
            // 
            this.lblTitle.AutoSize = true;
            this.lblTitle.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTitle.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.lblTitle.Location = new System.Drawing.Point(74, 6);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(122, 18);
            this.lblTitle.TabIndex = 0;
            this.lblTitle.Text = "DB LOGIN INFO";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Black;
            this.panel1.Controls.Add(this.lblTitle);
            this.panel1.Location = new System.Drawing.Point(4, 4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(254, 31);
            this.panel1.TabIndex = 1;
            // 
            // lblDomain
            // 
            this.lblDomain.AutoSize = true;
            this.lblDomain.BackColor = System.Drawing.SystemColors.Control;
            this.lblDomain.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDomain.Location = new System.Drawing.Point(9, 128);
            this.lblDomain.Name = "lblDomain";
            this.lblDomain.Size = new System.Drawing.Size(66, 16);
            this.lblDomain.TabIndex = 2;
            this.lblDomain.Text = "DB NAME";
            this.lblDomain.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbDbName
            // 
            this.tbDbName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbDbName.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbDbName.Location = new System.Drawing.Point(90, 128);
            this.tbDbName.Name = "tbDbName";
            this.tbDbName.Size = new System.Drawing.Size(164, 18);
            this.tbDbName.TabIndex = 3;
            this.tbDbName.Text = "1";
            this.tbDbName.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblUserID
            // 
            this.lblUserID.AutoSize = true;
            this.lblUserID.BackColor = System.Drawing.SystemColors.Control;
            this.lblUserID.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblUserID.Location = new System.Drawing.Point(9, 151);
            this.lblUserID.Name = "lblUserID";
            this.lblUserID.Size = new System.Drawing.Size(20, 16);
            this.lblUserID.TabIndex = 2;
            this.lblUserID.Text = "ID";
            this.lblUserID.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbUserID
            // 
            this.tbUserID.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbUserID.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbUserID.Location = new System.Drawing.Point(90, 151);
            this.tbUserID.Name = "tbUserID";
            this.tbUserID.Size = new System.Drawing.Size(164, 18);
            this.tbUserID.TabIndex = 3;
            this.tbUserID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblPassword
            // 
            this.lblPassword.AutoSize = true;
            this.lblPassword.BackColor = System.Drawing.SystemColors.Control;
            this.lblPassword.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPassword.Location = new System.Drawing.Point(9, 175);
            this.lblPassword.Name = "lblPassword";
            this.lblPassword.Size = new System.Drawing.Size(29, 16);
            this.lblPassword.TabIndex = 2;
            this.lblPassword.Text = "PW";
            this.lblPassword.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbPassword
            // 
            this.tbPassword.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbPassword.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbPassword.Location = new System.Drawing.Point(90, 175);
            this.tbPassword.Name = "tbPassword";
            this.tbPassword.Size = new System.Drawing.Size(164, 18);
            this.tbPassword.TabIndex = 3;
            this.tbPassword.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDisconnect.Location = new System.Drawing.Point(137, 205);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(101, 36);
            this.btnDisconnect.TabIndex = 4;
            this.btnDisconnect.Text = "DISCONNECT";
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // btnClose
            // 
            this.btnClose.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClose.Location = new System.Drawing.Point(192, 247);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(66, 36);
            this.btnClose.TabIndex = 4;
            this.btnClose.Text = "CLOSE";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnConnect.Location = new System.Drawing.Point(33, 205);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(101, 36);
            this.btnConnect.TabIndex = 4;
            this.btnConnect.Text = "CONNECT";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // lblPort
            // 
            this.lblPort.AutoSize = true;
            this.lblPort.BackColor = System.Drawing.SystemColors.Control;
            this.lblPort.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPort.Location = new System.Drawing.Point(9, 105);
            this.lblPort.Name = "lblPort";
            this.lblPort.Size = new System.Drawing.Size(43, 16);
            this.lblPort.TabIndex = 5;
            this.lblPort.Text = "PORT";
            this.lblPort.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbPort
            // 
            this.tbPort.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbPort.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbPort.Location = new System.Drawing.Point(90, 105);
            this.tbPort.Name = "tbPort";
            this.tbPort.Size = new System.Drawing.Size(164, 18);
            this.tbPort.TabIndex = 6;
            this.tbPort.Text = "1521";
            this.tbPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblHostIP
            // 
            this.lblHostIP.AutoSize = true;
            this.lblHostIP.BackColor = System.Drawing.SystemColors.Control;
            this.lblHostIP.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblHostIP.Location = new System.Drawing.Point(9, 81);
            this.lblHostIP.Name = "lblHostIP";
            this.lblHostIP.Size = new System.Drawing.Size(60, 16);
            this.lblHostIP.TabIndex = 7;
            this.lblHostIP.Text = "HOST IP";
            this.lblHostIP.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbHostIP
            // 
            this.tbHostIP.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbHostIP.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbHostIP.Location = new System.Drawing.Point(90, 81);
            this.tbHostIP.Name = "tbHostIP";
            this.tbHostIP.Size = new System.Drawing.Size(164, 18);
            this.tbHostIP.TabIndex = 8;
            this.tbHostIP.Text = "100.0.0.1";
            this.tbHostIP.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblType
            // 
            this.lblType.AutoSize = true;
            this.lblType.BackColor = System.Drawing.SystemColors.Control;
            this.lblType.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblType.Location = new System.Drawing.Point(10, 55);
            this.lblType.Name = "lblType";
            this.lblType.Size = new System.Drawing.Size(40, 16);
            this.lblType.TabIndex = 9;
            this.lblType.Text = "TYPE";
            this.lblType.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // rbConStringType1
            // 
            this.rbConStringType1.AutoSize = true;
            this.rbConStringType1.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.rbConStringType1.Location = new System.Drawing.Point(87, 50);
            this.rbConStringType1.Name = "rbConStringType1";
            this.rbConStringType1.Size = new System.Drawing.Size(64, 21);
            this.rbConStringType1.TabIndex = 10;
            this.rbConStringType1.TabStop = true;
            this.rbConStringType1.Text = "Type1";
            this.rbConStringType1.UseVisualStyleBackColor = true;
            this.rbConStringType1.CheckedChanged += new System.EventHandler(this.rbConStringType1_CheckedChanged);
            // 
            // rbConStringType2
            // 
            this.rbConStringType2.AutoSize = true;
            this.rbConStringType2.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.rbConStringType2.Location = new System.Drawing.Point(154, 50);
            this.rbConStringType2.Name = "rbConStringType2";
            this.rbConStringType2.Size = new System.Drawing.Size(64, 21);
            this.rbConStringType2.TabIndex = 10;
            this.rbConStringType2.TabStop = true;
            this.rbConStringType2.Text = "Type2";
            this.rbConStringType2.UseVisualStyleBackColor = true;
            this.rbConStringType2.CheckedChanged += new System.EventHandler(this.rbConStringType2_CheckedChanged);
            // 
            // FormDbLoginData
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(264, 290);
            this.Controls.Add(this.rbConStringType2);
            this.Controls.Add(this.rbConStringType1);
            this.Controls.Add(this.lblType);
            this.Controls.Add(this.lblHostIP);
            this.Controls.Add(this.tbHostIP);
            this.Controls.Add(this.lblPort);
            this.Controls.Add(this.tbPort);
            this.Controls.Add(this.lblDomain);
            this.Controls.Add(this.lblUserID);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.lblPassword);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.tbPassword);
            this.Controls.Add(this.btnDisconnect);
            this.Controls.Add(this.tbUserID);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.tbDbName);
            this.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.Name = "FormDbLoginData";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "DB LOING DATA";
            this.Load += new System.EventHandler(this.FormDbLoginData_Load);
            this.VisibleChanged += new System.EventHandler(this.FormDbLoginData_VisibleChanged);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblTitle;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lblDomain;
        private System.Windows.Forms.TextBox tbDbName;
        private System.Windows.Forms.Label lblUserID;
        private System.Windows.Forms.TextBox tbUserID;
        private System.Windows.Forms.Label lblPassword;
        private System.Windows.Forms.TextBox tbPassword;
        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Label lblPort;
        private System.Windows.Forms.TextBox tbPort;
        private System.Windows.Forms.Label lblHostIP;
        private System.Windows.Forms.TextBox tbHostIP;
        private System.Windows.Forms.Label lblType;
        private System.Windows.Forms.RadioButton rbConStringType1;
        private System.Windows.Forms.RadioButton rbConStringType2;
    }
}