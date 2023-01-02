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
            this.listViewXOFSMST = new System.Windows.Forms.ListView();
            this.gbAREADEL = new System.Windows.Forms.GroupBox();
            this.listViewAREADEL = new System.Windows.Forms.ListView();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnLoadtAreaDel = new System.Windows.Forms.Button();
            this.btnSaveAreaDel = new System.Windows.Forms.Button();
            this.gbXOFSMST.SuspendLayout();
            this.gbAREADEL.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbXOFSMST
            // 
            this.gbXOFSMST.Controls.Add(this.listViewXOFSMST);
            this.gbXOFSMST.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbXOFSMST.Location = new System.Drawing.Point(5, 7);
            this.gbXOFSMST.Name = "gbXOFSMST";
            this.gbXOFSMST.Size = new System.Drawing.Size(741, 284);
            this.gbXOFSMST.TabIndex = 0;
            this.gbXOFSMST.TabStop = false;
            this.gbXOFSMST.Text = "XOFSMST";
            // 
            // listViewXOFSMST
            // 
            this.listViewXOFSMST.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewXOFSMST.HideSelection = false;
            this.listViewXOFSMST.Location = new System.Drawing.Point(7, 20);
            this.listViewXOFSMST.Name = "listViewXOFSMST";
            this.listViewXOFSMST.Size = new System.Drawing.Size(726, 257);
            this.listViewXOFSMST.TabIndex = 0;
            this.listViewXOFSMST.UseCompatibleStateImageBehavior = false;
            // 
            // gbAREADEL
            // 
            this.gbAREADEL.Controls.Add(this.listViewAREADEL);
            this.gbAREADEL.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbAREADEL.Location = new System.Drawing.Point(5, 297);
            this.gbAREADEL.Name = "gbAREADEL";
            this.gbAREADEL.Size = new System.Drawing.Size(741, 284);
            this.gbAREADEL.TabIndex = 0;
            this.gbAREADEL.TabStop = false;
            this.gbAREADEL.Text = "AREADEL";
            // 
            // listViewAREADEL
            // 
            this.listViewAREADEL.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewAREADEL.HideSelection = false;
            this.listViewAREADEL.Location = new System.Drawing.Point(7, 20);
            this.listViewAREADEL.Name = "listViewAREADEL";
            this.listViewAREADEL.Size = new System.Drawing.Size(726, 257);
            this.listViewAREADEL.TabIndex = 0;
            this.listViewAREADEL.UseCompatibleStateImageBehavior = false;
            // 
            // btnClose
            // 
            this.btnClose.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClose.Location = new System.Drawing.Point(671, 586);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(72, 33);
            this.btnClose.TabIndex = 1;
            this.btnClose.Text = "CLOSE";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnLoadtAreaDel
            // 
            this.btnLoadtAreaDel.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLoadtAreaDel.Location = new System.Drawing.Point(12, 586);
            this.btnLoadtAreaDel.Name = "btnLoadtAreaDel";
            this.btnLoadtAreaDel.Size = new System.Drawing.Size(77, 33);
            this.btnLoadtAreaDel.TabIndex = 2;
            this.btnLoadtAreaDel.Text = "LOAD";
            this.btnLoadtAreaDel.UseVisualStyleBackColor = true;
            this.btnLoadtAreaDel.Click += new System.EventHandler(this.btnLoadtAreaDel_Click);
            // 
            // btnSaveAreaDel
            // 
            this.btnSaveAreaDel.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSaveAreaDel.Location = new System.Drawing.Point(95, 586);
            this.btnSaveAreaDel.Name = "btnSaveAreaDel";
            this.btnSaveAreaDel.Size = new System.Drawing.Size(77, 33);
            this.btnSaveAreaDel.TabIndex = 2;
            this.btnSaveAreaDel.Text = "SAVE";
            this.btnSaveAreaDel.UseVisualStyleBackColor = true;
            this.btnSaveAreaDel.Click += new System.EventHandler(this.btnSaveAreaDel_Click);
            // 
            // FormDbAddition
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(750, 623);
            this.ControlBox = false;
            this.Controls.Add(this.btnSaveAreaDel);
            this.Controls.Add(this.btnLoadtAreaDel);
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
        private System.Windows.Forms.Button btnLoadtAreaDel;
        private System.Windows.Forms.Button btnSaveAreaDel;
    }
}