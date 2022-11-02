namespace DefectDBManager
{
    partial class FormEditDefect
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.panelTitle = new System.Windows.Forms.Panel();
            this.lblTitle = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.btnClearAll = new System.Windows.Forms.Button();
            this.btnUpdate = new System.Windows.Forms.Button();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.dgvDefect = new System.Windows.Forms.DataGridView();
            this.ColMRKF1 = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.ColLNCD = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColFALTID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColSIZE = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColPPCD = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColROLLNAME = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.panelTitle.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvDefect)).BeginInit();
            this.SuspendLayout();
            // 
            // panelTitle
            // 
            this.panelTitle.BackColor = System.Drawing.Color.DarkGray;
            this.panelTitle.Controls.Add(this.lblTitle);
            this.panelTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelTitle.Location = new System.Drawing.Point(3, 3);
            this.panelTitle.Name = "panelTitle";
            this.panelTitle.Size = new System.Drawing.Size(520, 26);
            this.panelTitle.TabIndex = 2;
            // 
            // lblTitle
            // 
            this.lblTitle.AutoSize = true;
            this.lblTitle.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblTitle.ForeColor = System.Drawing.Color.White;
            this.lblTitle.Location = new System.Drawing.Point(9, 6);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(130, 13);
            this.lblTitle.TabIndex = 0;
            this.lblTitle.Text = "DEFECTS EDITOR";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.panelTitle, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel3, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.dgvDefect, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 447F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 42F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 43F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(526, 564);
            this.tableLayoutPanel1.TabIndex = 3;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 320F));
            this.tableLayoutPanel2.Controls.Add(this.btnClearAll, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnUpdate, 1, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 482);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(520, 36);
            this.tableLayoutPanel2.TabIndex = 4;
            // 
            // btnClearAll
            // 
            this.btnClearAll.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClearAll.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnClearAll.Location = new System.Drawing.Point(3, 3);
            this.btnClearAll.Name = "btnClearAll";
            this.btnClearAll.Size = new System.Drawing.Size(94, 30);
            this.btnClearAll.TabIndex = 0;
            this.btnClearAll.Text = "CLEAR ALL";
            this.btnClearAll.UseVisualStyleBackColor = true;
            this.btnClearAll.Click += new System.EventHandler(this.btnClearAll_Click);
            // 
            // btnUpdate
            // 
            this.btnUpdate.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnUpdate.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnUpdate.Location = new System.Drawing.Point(103, 3);
            this.btnUpdate.Name = "btnUpdate";
            this.btnUpdate.Size = new System.Drawing.Size(94, 30);
            this.btnUpdate.TabIndex = 1;
            this.btnUpdate.Text = "UPDATE";
            this.btnUpdate.UseVisualStyleBackColor = true;
            this.btnUpdate.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 4;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 160F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 160F));
            this.tableLayoutPanel3.Controls.Add(this.btnOK, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnClose, 2, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 524);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(520, 37);
            this.tableLayoutPanel3.TabIndex = 5;
            // 
            // btnOK
            // 
            this.btnOK.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnOK.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnOK.Location = new System.Drawing.Point(163, 3);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(94, 31);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnClose
            // 
            this.btnClose.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClose.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnClose.Location = new System.Drawing.Point(263, 3);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(94, 31);
            this.btnClose.TabIndex = 1;
            this.btnClose.Text = "CLOSE";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // dgvDefect
            // 
            this.dgvDefect.AllowUserToAddRows = false;
            this.dgvDefect.AllowUserToDeleteRows = false;
            this.dgvDefect.AllowUserToResizeColumns = false;
            this.dgvDefect.AllowUserToResizeRows = false;
            this.dgvDefect.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvDefect.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ColMRKF1,
            this.ColLNCD,
            this.ColFALTID,
            this.ColSIZE,
            this.ColPPCD,
            this.ColROLLNAME});
            this.dgvDefect.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvDefect.Location = new System.Drawing.Point(3, 35);
            this.dgvDefect.MultiSelect = false;
            this.dgvDefect.Name = "dgvDefect";
            this.dgvDefect.RowHeadersVisible = false;
            this.dgvDefect.RowTemplate.Height = 23;
            this.dgvDefect.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dgvDefect.Size = new System.Drawing.Size(520, 441);
            this.dgvDefect.TabIndex = 6;
            this.dgvDefect.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvDefect_CellDoubleClick);
            // 
            // ColMRKF1
            // 
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.NullValue = false;
            this.ColMRKF1.DefaultCellStyle = dataGridViewCellStyle1;
            this.ColMRKF1.HeaderText = "";
            this.ColMRKF1.Name = "ColMRKF1";
            this.ColMRKF1.Width = 20;
            // 
            // ColLNCD
            // 
            this.ColLNCD.HeaderText = "마킹";
            this.ColLNCD.Name = "ColLNCD";
            this.ColLNCD.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.ColLNCD.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.ColLNCD.Width = 75;
            // 
            // ColFALTID
            // 
            this.ColFALTID.HeaderText = "FALTID";
            this.ColFALTID.Name = "ColFALTID";
            this.ColFALTID.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.ColFALTID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.ColFALTID.Width = 75;
            // 
            // ColSIZE
            // 
            this.ColSIZE.HeaderText = "SIZE";
            this.ColSIZE.Name = "ColSIZE";
            this.ColSIZE.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.ColSIZE.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.ColSIZE.Width = 75;
            // 
            // ColPPCD
            // 
            this.ColPPCD.HeaderText = "PPCD";
            this.ColPPCD.Name = "ColPPCD";
            this.ColPPCD.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.ColPPCD.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.ColPPCD.Width = 75;
            // 
            // ColROLLNAME
            // 
            this.ColROLLNAME.HeaderText = "ROLLNAME";
            this.ColROLLNAME.Name = "ColROLLNAME";
            this.ColROLLNAME.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.ColROLLNAME.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.ColROLLNAME.Width = 180;
            // 
            // FormEditDefect
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(526, 564);
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "FormEditDefect";
            this.Text = "FormEditDefect";
            this.VisibleChanged += new System.EventHandler(this.FormEditDefect_VisibleChanged);
            this.panelTitle.ResumeLayout(false);
            this.panelTitle.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvDefect)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelTitle;
        private System.Windows.Forms.Label lblTitle;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Button btnClearAll;
        private System.Windows.Forms.Button btnUpdate;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.DataGridView dgvDefect;
        private System.Windows.Forms.DataGridViewCheckBoxColumn ColMRKF1;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColLNCD;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColFALTID;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColSIZE;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColPPCD;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColROLLNAME;
    }
}