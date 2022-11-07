namespace DefectDBManager
{
    partial class FormSkipParam
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.gbInspDataMask = new System.Windows.Forms.GroupBox();
            this.btnDelOffset = new System.Windows.Forms.Button();
            this.btnUpdateSkipOffset = new System.Windows.Forms.Button();
            this.btnAddOffset = new System.Windows.Forms.Button();
            this.tbEndY = new System.Windows.Forms.TextBox();
            this.tbStartY = new System.Windows.Forms.TextBox();
            this.tbEndX = new System.Windows.Forms.TextBox();
            this.tbStartX = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lblSizeY = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.lblSizeX = new System.Windows.Forms.Label();
            this.lvOffsetSkip = new System.Windows.Forms.ListView();
            this.gbSizeSkip = new System.Windows.Forms.GroupBox();
            this.ucSkipParam6 = new DefectDBManager.ucSkipParam();
            this.btnUpdateSizeSkip = new System.Windows.Forms.Button();
            this.ucSkipParam5 = new DefectDBManager.ucSkipParam();
            this.ucSkipParam4 = new DefectDBManager.ucSkipParam();
            this.ucSkipParam3 = new DefectDBManager.ucSkipParam();
            this.ucSkipParam2 = new DefectDBManager.ucSkipParam();
            this.ucSkipParam1 = new DefectDBManager.ucSkipParam();
            this.cbDest = new System.Windows.Forms.ComboBox();
            this.btnClose = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.gbInspDataMask.SuspendLayout();
            this.gbSizeSkip.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tableLayoutPanel1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.btnClose);
            this.splitContainer1.Size = new System.Drawing.Size(749, 497);
            this.splitContainer1.SplitterDistance = 447;
            this.splitContainer1.SplitterWidth = 5;
            this.splitContainer1.TabIndex = 0;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 58.53976F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 41.46024F));
            this.tableLayoutPanel1.Controls.Add(this.gbInspDataMask, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.gbSizeSkip, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(749, 447);
            this.tableLayoutPanel1.TabIndex = 2;
            // 
            // gbInspDataMask
            // 
            this.gbInspDataMask.Controls.Add(this.btnDelOffset);
            this.gbInspDataMask.Controls.Add(this.btnUpdateSkipOffset);
            this.gbInspDataMask.Controls.Add(this.btnAddOffset);
            this.gbInspDataMask.Controls.Add(this.tbEndY);
            this.gbInspDataMask.Controls.Add(this.tbStartY);
            this.gbInspDataMask.Controls.Add(this.tbEndX);
            this.gbInspDataMask.Controls.Add(this.tbStartX);
            this.gbInspDataMask.Controls.Add(this.label4);
            this.gbInspDataMask.Controls.Add(this.label2);
            this.gbInspDataMask.Controls.Add(this.lblSizeY);
            this.gbInspDataMask.Controls.Add(this.label3);
            this.gbInspDataMask.Controls.Add(this.label1);
            this.gbInspDataMask.Controls.Add(this.lblSizeX);
            this.gbInspDataMask.Controls.Add(this.lvOffsetSkip);
            this.gbInspDataMask.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbInspDataMask.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbInspDataMask.Location = new System.Drawing.Point(441, 3);
            this.gbInspDataMask.Name = "gbInspDataMask";
            this.gbInspDataMask.Size = new System.Drawing.Size(305, 441);
            this.gbInspDataMask.TabIndex = 1;
            this.gbInspDataMask.TabStop = false;
            this.gbInspDataMask.Text = "검사 데이터 마스크";
            // 
            // btnDelOffset
            // 
            this.btnDelOffset.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDelOffset.Location = new System.Drawing.Point(73, 408);
            this.btnDelOffset.Name = "btnDelOffset";
            this.btnDelOffset.Size = new System.Drawing.Size(53, 24);
            this.btnDelOffset.TabIndex = 0;
            this.btnDelOffset.Text = "DEL";
            this.btnDelOffset.UseVisualStyleBackColor = true;
            this.btnDelOffset.Click += new System.EventHandler(this.btnDelOffset_Click);
            // 
            // btnUpdateSkipOffset
            // 
            this.btnUpdateSkipOffset.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnUpdateSkipOffset.Location = new System.Drawing.Point(224, 405);
            this.btnUpdateSkipOffset.Name = "btnUpdateSkipOffset";
            this.btnUpdateSkipOffset.Size = new System.Drawing.Size(74, 27);
            this.btnUpdateSkipOffset.TabIndex = 0;
            this.btnUpdateSkipOffset.Text = "UPDATE";
            this.btnUpdateSkipOffset.UseVisualStyleBackColor = true;
            this.btnUpdateSkipOffset.Click += new System.EventHandler(this.btnUpdateSkipOffset_Click);
            // 
            // btnAddOffset
            // 
            this.btnAddOffset.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddOffset.Location = new System.Drawing.Point(14, 408);
            this.btnAddOffset.Name = "btnAddOffset";
            this.btnAddOffset.Size = new System.Drawing.Size(53, 24);
            this.btnAddOffset.TabIndex = 0;
            this.btnAddOffset.Text = "ADD";
            this.btnAddOffset.UseVisualStyleBackColor = true;
            this.btnAddOffset.Click += new System.EventHandler(this.btnAddOffset_Click);
            // 
            // tbEndY
            // 
            this.tbEndY.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbEndY.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbEndY.Location = new System.Drawing.Point(118, 387);
            this.tbEndY.Name = "tbEndY";
            this.tbEndY.Size = new System.Drawing.Size(55, 15);
            this.tbEndY.TabIndex = 2;
            // 
            // tbStartY
            // 
            this.tbStartY.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbStartY.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbStartY.Location = new System.Drawing.Point(38, 387);
            this.tbStartY.Name = "tbStartY";
            this.tbStartY.Size = new System.Drawing.Size(55, 15);
            this.tbStartY.TabIndex = 2;
            // 
            // tbEndX
            // 
            this.tbEndX.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbEndX.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbEndX.Location = new System.Drawing.Point(118, 362);
            this.tbEndX.Name = "tbEndX";
            this.tbEndX.Size = new System.Drawing.Size(55, 15);
            this.tbEndX.TabIndex = 2;
            // 
            // tbStartX
            // 
            this.tbStartX.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbStartX.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbStartX.Location = new System.Drawing.Point(38, 362);
            this.tbStartX.Name = "tbStartX";
            this.tbStartX.Size = new System.Drawing.Size(55, 15);
            this.tbStartX.TabIndex = 2;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(176, 387);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(31, 16);
            this.label4.TabIndex = 1;
            this.label4.Text = "mm";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(97, 386);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(15, 16);
            this.label2.TabIndex = 1;
            this.label2.Text = "~";
            // 
            // lblSizeY
            // 
            this.lblSizeY.AutoSize = true;
            this.lblSizeY.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSizeY.Location = new System.Drawing.Point(11, 386);
            this.lblSizeY.Name = "lblSizeY";
            this.lblSizeY.Size = new System.Drawing.Size(23, 16);
            this.lblSizeY.TabIndex = 1;
            this.lblSizeY.Text = "Y :";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(176, 362);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 16);
            this.label3.TabIndex = 1;
            this.label3.Text = "mm";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(97, 361);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(15, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "~";
            // 
            // lblSizeX
            // 
            this.lblSizeX.AutoSize = true;
            this.lblSizeX.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSizeX.Location = new System.Drawing.Point(11, 361);
            this.lblSizeX.Name = "lblSizeX";
            this.lblSizeX.Size = new System.Drawing.Size(24, 16);
            this.lblSizeX.TabIndex = 1;
            this.lblSizeX.Text = "X :";
            // 
            // lvOffsetSkip
            // 
            this.lvOffsetSkip.HideSelection = false;
            this.lvOffsetSkip.Location = new System.Drawing.Point(6, 20);
            this.lvOffsetSkip.Name = "lvOffsetSkip";
            this.lvOffsetSkip.Size = new System.Drawing.Size(295, 333);
            this.lvOffsetSkip.TabIndex = 0;
            this.lvOffsetSkip.UseCompatibleStateImageBehavior = false;
            this.lvOffsetSkip.View = System.Windows.Forms.View.Details;
            // 
            // gbSizeSkip
            // 
            this.gbSizeSkip.Controls.Add(this.ucSkipParam6);
            this.gbSizeSkip.Controls.Add(this.btnUpdateSizeSkip);
            this.gbSizeSkip.Controls.Add(this.ucSkipParam5);
            this.gbSizeSkip.Controls.Add(this.ucSkipParam4);
            this.gbSizeSkip.Controls.Add(this.ucSkipParam3);
            this.gbSizeSkip.Controls.Add(this.ucSkipParam2);
            this.gbSizeSkip.Controls.Add(this.ucSkipParam1);
            this.gbSizeSkip.Controls.Add(this.cbDest);
            this.gbSizeSkip.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbSizeSkip.Location = new System.Drawing.Point(3, 4);
            this.gbSizeSkip.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbSizeSkip.Name = "gbSizeSkip";
            this.gbSizeSkip.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbSizeSkip.Size = new System.Drawing.Size(431, 438);
            this.gbSizeSkip.TabIndex = 0;
            this.gbSizeSkip.TabStop = false;
            this.gbSizeSkip.Text = "Size Skip";
            // 
            // ucSkipParam6
            // 
            this.ucSkipParam6._SkipSize = null;
            this.ucSkipParam6.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ucSkipParam6.Location = new System.Drawing.Point(214, 284);
            this.ucSkipParam6.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ucSkipParam6.Name = "ucSkipParam6";
            this.ucSkipParam6.Size = new System.Drawing.Size(211, 117);
            this.ucSkipParam6.TabIndex = 1;
            this.ucSkipParam6.Title = "Skip Data";
            // 
            // btnUpdateSizeSkip
            // 
            this.btnUpdateSizeSkip.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnUpdateSizeSkip.Location = new System.Drawing.Point(300, 405);
            this.btnUpdateSizeSkip.Name = "btnUpdateSizeSkip";
            this.btnUpdateSizeSkip.Size = new System.Drawing.Size(123, 27);
            this.btnUpdateSizeSkip.TabIndex = 0;
            this.btnUpdateSizeSkip.Text = "UPDATE";
            this.btnUpdateSizeSkip.UseVisualStyleBackColor = true;
            this.btnUpdateSizeSkip.Click += new System.EventHandler(this.btnUpdateSizeSkip_Click);
            // 
            // ucSkipParam5
            // 
            this.ucSkipParam5._SkipSize = null;
            this.ucSkipParam5.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ucSkipParam5.Location = new System.Drawing.Point(6, 284);
            this.ucSkipParam5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ucSkipParam5.Name = "ucSkipParam5";
            this.ucSkipParam5.Size = new System.Drawing.Size(211, 117);
            this.ucSkipParam5.TabIndex = 1;
            this.ucSkipParam5.Title = "Skip Data";
            // 
            // ucSkipParam4
            // 
            this.ucSkipParam4._SkipSize = null;
            this.ucSkipParam4.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ucSkipParam4.Location = new System.Drawing.Point(214, 165);
            this.ucSkipParam4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ucSkipParam4.Name = "ucSkipParam4";
            this.ucSkipParam4.Size = new System.Drawing.Size(211, 117);
            this.ucSkipParam4.TabIndex = 1;
            this.ucSkipParam4.Title = "Skip Data";
            // 
            // ucSkipParam3
            // 
            this.ucSkipParam3._SkipSize = null;
            this.ucSkipParam3.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ucSkipParam3.Location = new System.Drawing.Point(6, 165);
            this.ucSkipParam3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ucSkipParam3.Name = "ucSkipParam3";
            this.ucSkipParam3.Size = new System.Drawing.Size(211, 117);
            this.ucSkipParam3.TabIndex = 1;
            this.ucSkipParam3.Title = "Skip Data";
            // 
            // ucSkipParam2
            // 
            this.ucSkipParam2._SkipSize = null;
            this.ucSkipParam2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ucSkipParam2.Location = new System.Drawing.Point(214, 46);
            this.ucSkipParam2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ucSkipParam2.Name = "ucSkipParam2";
            this.ucSkipParam2.Size = new System.Drawing.Size(211, 117);
            this.ucSkipParam2.TabIndex = 1;
            this.ucSkipParam2.Title = "Skip Data";
            // 
            // ucSkipParam1
            // 
            this.ucSkipParam1._SkipSize = null;
            this.ucSkipParam1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ucSkipParam1.Location = new System.Drawing.Point(5, 46);
            this.ucSkipParam1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ucSkipParam1.Name = "ucSkipParam1";
            this.ucSkipParam1.Size = new System.Drawing.Size(211, 117);
            this.ucSkipParam1.TabIndex = 1;
            this.ucSkipParam1.Title = "Skip Data";
            // 
            // cbDest
            // 
            this.cbDest.FormattingEnabled = true;
            this.cbDest.Location = new System.Drawing.Point(5, 18);
            this.cbDest.Name = "cbDest";
            this.cbDest.Size = new System.Drawing.Size(232, 23);
            this.cbDest.TabIndex = 0;
            this.cbDest.SelectedIndexChanged += new System.EventHandler(this.cbDest_SelectedIndexChanged);
            // 
            // btnClose
            // 
            this.btnClose.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClose.Location = new System.Drawing.Point(556, 6);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(186, 32);
            this.btnClose.TabIndex = 0;
            this.btnClose.Text = "CLOSE";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // FormSkipParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(749, 497);
            this.ControlBox = false;
            this.Controls.Add(this.splitContainer1);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormSkipParam";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "EDIT SKIP PARAM";
            this.Load += new System.EventHandler(this.FormSkipParam_Load);
            this.VisibleChanged += new System.EventHandler(this.FormSkipParam_VisibleChanged);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.gbInspDataMask.ResumeLayout(false);
            this.gbInspDataMask.PerformLayout();
            this.gbSizeSkip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.GroupBox gbSizeSkip;
        private System.Windows.Forms.ComboBox cbDest;
        private ucSkipParam ucSkipParam6;
        private ucSkipParam ucSkipParam5;
        private ucSkipParam ucSkipParam4;
        private ucSkipParam ucSkipParam3;
        private ucSkipParam ucSkipParam2;
        private ucSkipParam ucSkipParam1;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnUpdateSizeSkip;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.GroupBox gbInspDataMask;
        private System.Windows.Forms.Button btnDelOffset;
        private System.Windows.Forms.Button btnUpdateSkipOffset;
        private System.Windows.Forms.Button btnAddOffset;
        private System.Windows.Forms.TextBox tbEndY;
        private System.Windows.Forms.TextBox tbStartY;
        private System.Windows.Forms.TextBox tbEndX;
        private System.Windows.Forms.TextBox tbStartX;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblSizeY;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblSizeX;
        private System.Windows.Forms.ListView lvOffsetSkip;
    }
}