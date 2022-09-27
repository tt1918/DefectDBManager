namespace DefectDBManager
{
    partial class FormDbAddition
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
            this.gbXOFSMST = new System.Windows.Forms.GroupBox();
            this.gbAREADEL = new System.Windows.Forms.GroupBox();
            this.btnClose = new System.Windows.Forms.Button();
            this.listViewXOFSMST = new System.Windows.Forms.ListView();
            this.listViewAREADEL = new System.Windows.Forms.ListView();
            this.gbXOFSMST.SuspendLayout();
            this.gbAREADEL.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbXOFSMST
            // 
            this.gbXOFSMST.Controls.Add(this.listViewXOFSMST);
            this.gbXOFSMST.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbXOFSMST.Location = new System.Drawing.Point(5, 7);
            this.gbXOFSMST.Name = "gbXOFSMST";
            this.gbXOFSMST.Size = new System.Drawing.Size(741, 284);
            this.gbXOFSMST.TabIndex = 0;
            this.gbXOFSMST.TabStop = false;
            this.gbXOFSMST.Text = "XOFSMST";
            // 
            // gbAREADEL
            // 
            this.gbAREADEL.Controls.Add(this.listViewAREADEL);
            this.gbAREADEL.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbAREADEL.Location = new System.Drawing.Point(5, 297);
            this.gbAREADEL.Name = "gbAREADEL";
            this.gbAREADEL.Size = new System.Drawing.Size(741, 284);
            this.gbAREADEL.TabIndex = 0;
            this.gbAREADEL.TabStop = false;
            this.gbAREADEL.Text = "AREADEL";
            // 
            // btnClose
            // 
            this.btnClose.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClose.Location = new System.Drawing.Point(671, 586);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(72, 33);
            this.btnClose.TabIndex = 1;
            this.btnClose.Text = "CLOSE";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // listViewXOFSMST
            // 
            this.listViewXOFSMST.HideSelection = false;
            this.listViewXOFSMST.Location = new System.Drawing.Point(7, 20);
            this.listViewXOFSMST.Name = "listViewXOFSMST";
            this.listViewXOFSMST.Size = new System.Drawing.Size(726, 257);
            this.listViewXOFSMST.TabIndex = 0;
            this.listViewXOFSMST.UseCompatibleStateImageBehavior = false;
            // 
            // listViewAREADEL
            // 
            this.listViewAREADEL.HideSelection = false;
            this.listViewAREADEL.Location = new System.Drawing.Point(7, 20);
            this.listViewAREADEL.Name = "listViewAREADEL";
            this.listViewAREADEL.Size = new System.Drawing.Size(726, 257);
            this.listViewAREADEL.TabIndex = 0;
            this.listViewAREADEL.UseCompatibleStateImageBehavior = false;
            // 
            // FormDbAddition
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(750, 623);
            this.ControlBox = false;
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.gbAREADEL);
            this.Controls.Add(this.gbXOFSMST);
            this.Name = "FormDbAddition";
            this.Text = "XOFSMST/AREADEL VIEWER";
            this.Load += new System.EventHandler(this.FormDbAddition_Load);
            this.VisibleChanged += new System.EventHandler(this.FormDbAddition_VisibleChanged);
            this.gbXOFSMST.ResumeLayout(false);
            this.gbAREADEL.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbXOFSMST;
        private System.Windows.Forms.GroupBox gbAREADEL;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.ListView listViewXOFSMST;
        private System.Windows.Forms.ListView listViewAREADEL;
    }
}