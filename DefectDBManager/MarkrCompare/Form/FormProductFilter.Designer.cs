namespace MarkrCompare
{
    partial class FormProductFilter
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
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.btnDelMaterial = new CustomControls.RoundButton();
            this.btnAddMaterial = new CustomControls.RoundButton();
            this.btnCancel = new CustomControls.RoundButton();
            this.btnOK = new CustomControls.RoundButton();
            this.btnClear = new CustomControls.RoundButton();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvFilter = new System.Windows.Forms.DataGridView();
            this.lblTitle = new CustomControls.RoundLabel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.cbSyncDuration = new System.Windows.Forms.CheckBox();
            this.lblSyncDuration = new CustomControls.RoundLabel();
            this.comboSyncDuration = new System.Windows.Forms.ComboBox();
            this.tableLayoutPanel5.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvFilter)).BeginInit();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 6;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.Controls.Add(this.btnDelMaterial, 1, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnAddMaterial, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnCancel, 4, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnOK, 5, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnClear, 2, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 511);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(591, 51);
            this.tableLayoutPanel5.TabIndex = 3;
            // 
            // btnDelMaterial
            // 
            this.btnDelMaterial.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelMaterial.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelMaterial.BorderColor = System.Drawing.Color.Lavender;
            this.btnDelMaterial.BorderRadius = 1;
            this.btnDelMaterial.BorderSize = 2;
            this.btnDelMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelMaterial.FlatAppearance.BorderSize = 0;
            this.btnDelMaterial.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDelMaterial.ForeColor = System.Drawing.Color.White;
            this.btnDelMaterial.Location = new System.Drawing.Point(101, 1);
            this.btnDelMaterial.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelMaterial.Name = "btnDelMaterial";
            this.btnDelMaterial.Size = new System.Drawing.Size(98, 49);
            this.btnDelMaterial.TabIndex = 1;
            this.btnDelMaterial.Text = "DEL";
            this.btnDelMaterial.TextColor = System.Drawing.Color.White;
            this.btnDelMaterial.UseVisualStyleBackColor = false;
            this.btnDelMaterial.Click += new System.EventHandler(this.btnDelMaterial_Click);
            // 
            // btnAddMaterial
            // 
            this.btnAddMaterial.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddMaterial.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddMaterial.BorderColor = System.Drawing.Color.Lavender;
            this.btnAddMaterial.BorderRadius = 1;
            this.btnAddMaterial.BorderSize = 2;
            this.btnAddMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddMaterial.FlatAppearance.BorderSize = 0;
            this.btnAddMaterial.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddMaterial.ForeColor = System.Drawing.Color.White;
            this.btnAddMaterial.Location = new System.Drawing.Point(1, 1);
            this.btnAddMaterial.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddMaterial.Name = "btnAddMaterial";
            this.btnAddMaterial.Size = new System.Drawing.Size(98, 49);
            this.btnAddMaterial.TabIndex = 1;
            this.btnAddMaterial.Text = "ADD";
            this.btnAddMaterial.TextColor = System.Drawing.Color.White;
            this.btnAddMaterial.UseVisualStyleBackColor = false;
            this.btnAddMaterial.Click += new System.EventHandler(this.btnAddMaterial_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.BorderColor = System.Drawing.Color.Lavender;
            this.btnCancel.BorderRadius = 1;
            this.btnCancel.BorderSize = 2;
            this.btnCancel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnCancel.FlatAppearance.BorderSize = 0;
            this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCancel.ForeColor = System.Drawing.Color.White;
            this.btnCancel.Location = new System.Drawing.Point(392, 1);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(1);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(98, 49);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "CANCEL";
            this.btnCancel.TextColor = System.Drawing.Color.White;
            this.btnCancel.UseVisualStyleBackColor = false;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOK.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOK.BorderColor = System.Drawing.Color.Lavender;
            this.btnOK.BorderRadius = 1;
            this.btnOK.BorderSize = 2;
            this.btnOK.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnOK.FlatAppearance.BorderSize = 0;
            this.btnOK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnOK.ForeColor = System.Drawing.Color.White;
            this.btnOK.Location = new System.Drawing.Point(492, 1);
            this.btnOK.Margin = new System.Windows.Forms.Padding(1);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(98, 49);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.TextColor = System.Drawing.Color.White;
            this.btnOK.UseVisualStyleBackColor = false;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnClear
            // 
            this.btnClear.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClear.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClear.BorderColor = System.Drawing.Color.Lavender;
            this.btnClear.BorderRadius = 1;
            this.btnClear.BorderSize = 2;
            this.btnClear.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClear.FlatAppearance.BorderSize = 0;
            this.btnClear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnClear.ForeColor = System.Drawing.Color.White;
            this.btnClear.Location = new System.Drawing.Point(201, 1);
            this.btnClear.Margin = new System.Windows.Forms.Padding(1);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(98, 49);
            this.btnClear.TabIndex = 2;
            this.btnClear.Text = "CLEAR";
            this.btnClear.TextColor = System.Drawing.Color.White;
            this.btnClear.UseVisualStyleBackColor = false;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.dgvFilter, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel5, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.lblTitle, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 51F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(591, 562);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // dgvFilter
            // 
            this.dgvFilter.AllowUserToAddRows = false;
            this.dgvFilter.AllowUserToDeleteRows = false;
            this.dgvFilter.AllowUserToResizeColumns = false;
            this.dgvFilter.AllowUserToResizeRows = false;
            this.dgvFilter.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvFilter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvFilter.Location = new System.Drawing.Point(1, 61);
            this.dgvFilter.Margin = new System.Windows.Forms.Padding(1);
            this.dgvFilter.Name = "dgvFilter";
            this.dgvFilter.RowTemplate.Height = 23;
            this.dgvFilter.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dgvFilter.Size = new System.Drawing.Size(589, 449);
            this.dgvFilter.TabIndex = 10;
            this.dgvFilter.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvFilter_CellValueChanged);
            this.dgvFilter.CurrentCellDirtyStateChanged += new System.EventHandler(this.dgvFilter_CurrentCellDirtyStateChanged);
            // 
            // lblTitle
            // 
            this.lblTitle.AutoSize = true;
            this.lblTitle.BkColor = System.Drawing.Color.DarkSlateBlue;
            this.lblTitle.BorderColor = System.Drawing.Color.MidnightBlue;
            this.lblTitle.CornerR = 10;
            this.lblTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblTitle.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTitle.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblTitle.IsFillLB = false;
            this.lblTitle.IsFillLT = false;
            this.lblTitle.IsFillRB = false;
            this.lblTitle.IsFillRT = false;
            this.lblTitle.Location = new System.Drawing.Point(2, 2);
            this.lblTitle.Margin = new System.Windows.Forms.Padding(2);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(587, 26);
            this.lblTitle.TabIndex = 1;
            this.lblTitle.Text = "PRODUCT SETTING";
            this.lblTitle.Thickness = 1;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 4;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.tableLayoutPanel2.Controls.Add(this.cbSyncDuration, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.lblSyncDuration, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.comboSyncDuration, 3, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 30);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(591, 30);
            this.tableLayoutPanel2.TabIndex = 11;
            // 
            // cbSyncDuration
            // 
            this.cbSyncDuration.AutoSize = true;
            this.cbSyncDuration.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbSyncDuration.Location = new System.Drawing.Point(454, 3);
            this.cbSyncDuration.Name = "cbSyncDuration";
            this.cbSyncDuration.Size = new System.Drawing.Size(14, 24);
            this.cbSyncDuration.TabIndex = 0;
            this.cbSyncDuration.UseVisualStyleBackColor = true;
            // 
            // lblSyncDuration
            // 
            this.lblSyncDuration.AutoSize = true;
            this.lblSyncDuration.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblSyncDuration.BorderColor = System.Drawing.Color.Silver;
            this.lblSyncDuration.CornerR = 10;
            this.lblSyncDuration.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblSyncDuration.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSyncDuration.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblSyncDuration.IsFillLB = false;
            this.lblSyncDuration.IsFillLT = false;
            this.lblSyncDuration.IsFillRB = false;
            this.lblSyncDuration.IsFillRT = false;
            this.lblSyncDuration.Location = new System.Drawing.Point(333, 2);
            this.lblSyncDuration.Margin = new System.Windows.Forms.Padding(2);
            this.lblSyncDuration.Name = "lblSyncDuration";
            this.lblSyncDuration.Size = new System.Drawing.Size(116, 26);
            this.lblSyncDuration.TabIndex = 3;
            this.lblSyncDuration.Text = "Sync Duration";
            this.lblSyncDuration.Thickness = 1;
            // 
            // comboSyncDuration
            // 
            this.comboSyncDuration.Dock = System.Windows.Forms.DockStyle.Fill;
            this.comboSyncDuration.FormattingEnabled = true;
            this.comboSyncDuration.Location = new System.Drawing.Point(474, 3);
            this.comboSyncDuration.Name = "comboSyncDuration";
            this.comboSyncDuration.Size = new System.Drawing.Size(114, 23);
            this.comboSyncDuration.TabIndex = 4;
            // 
            // FormProductFilter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(591, 562);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormProductFilter";
            this.Text = "FormLNCD";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormProductFilter_FormClosing);
            this.Load += new System.EventHandler(this.FormProductFilter_Load);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvFilter)).EndInit();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private CustomControls.RoundButton btnCancel;
        private CustomControls.RoundButton btnOK;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private CustomControls.RoundLabel lblTitle;
        private CustomControls.RoundButton btnAddMaterial;
        private CustomControls.RoundButton btnDelMaterial;
        private System.Windows.Forms.DataGridView dgvFilter;
        private CustomControls.RoundButton btnClear;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.CheckBox cbSyncDuration;
        private CustomControls.RoundLabel lblSyncDuration;
        private System.Windows.Forms.ComboBox comboSyncDuration;
    }
}