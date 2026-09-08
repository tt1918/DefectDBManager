namespace MarkCompare
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
            this.btnAddMaterial = new CustomControls.ShadedButton();
            this.btnDelMaterial = new CustomControls.ShadedButton();
            this.btnClear = new CustomControls.ShadedButton();
            this.btnCancel = new CustomControls.ShadedButton();
            this.btnOK = new CustomControls.ShadedButton();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.lblTitle = new CustomControls.ShadedLabel();
            this.dgvFilter = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.cbSyncDuration = new System.Windows.Forms.CheckBox();
            this.comboSyncDuration = new System.Windows.Forms.ComboBox();
            this.lblSyncDuration = new CustomControls.ShadedLabel();
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
            this.tableLayoutPanel5.Controls.Add(this.btnAddMaterial, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnDelMaterial, 1, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnClear, 2, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnCancel, 4, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnOK, 5, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 511);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(591, 51);
            this.tableLayoutPanel5.TabIndex = 3;
            // 
            // btnAddMaterial
            // 
            this.btnAddMaterial.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnAddMaterial.BackColor = System.Drawing.Color.Transparent;
            this.btnAddMaterial.BorderColor = System.Drawing.Color.SlateGray;
            this.btnAddMaterial.BorderRadius = 5;
            this.btnAddMaterial.BorderSize = 2;
            this.btnAddMaterial.ButtonImage = null;
            this.btnAddMaterial.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnAddMaterial.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnAddMaterial.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnAddMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddMaterial.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddMaterial.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnAddMaterial.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnAddMaterial.IconPadding = 6;
            this.btnAddMaterial.IconSize = 20;
            this.btnAddMaterial.Location = new System.Drawing.Point(1, 1);
            this.btnAddMaterial.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddMaterial.Name = "btnAddMaterial";
            this.btnAddMaterial.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddMaterial.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnAddMaterial.ShowAccentLine = false;
            this.btnAddMaterial.Size = new System.Drawing.Size(98, 49);
            this.btnAddMaterial.TabIndex = 16;
            this.btnAddMaterial.Text = "ADD";
            this.btnAddMaterial.TextColor = System.Drawing.Color.White;
            this.btnAddMaterial.TextOffsetX = 0;
            this.btnAddMaterial.TextOffsetY = 0;
            this.btnAddMaterial.Click += new System.EventHandler(this.btnAddMaterial_Click);
            // 
            // btnDelMaterial
            // 
            this.btnDelMaterial.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnDelMaterial.BackColor = System.Drawing.Color.Transparent;
            this.btnDelMaterial.BorderColor = System.Drawing.Color.SlateGray;
            this.btnDelMaterial.BorderRadius = 5;
            this.btnDelMaterial.BorderSize = 2;
            this.btnDelMaterial.ButtonImage = null;
            this.btnDelMaterial.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnDelMaterial.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnDelMaterial.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnDelMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelMaterial.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDelMaterial.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnDelMaterial.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnDelMaterial.IconPadding = 6;
            this.btnDelMaterial.IconSize = 20;
            this.btnDelMaterial.Location = new System.Drawing.Point(101, 1);
            this.btnDelMaterial.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelMaterial.Name = "btnDelMaterial";
            this.btnDelMaterial.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelMaterial.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnDelMaterial.ShowAccentLine = false;
            this.btnDelMaterial.Size = new System.Drawing.Size(98, 49);
            this.btnDelMaterial.TabIndex = 16;
            this.btnDelMaterial.Text = "DEL";
            this.btnDelMaterial.TextColor = System.Drawing.Color.White;
            this.btnDelMaterial.TextOffsetX = 0;
            this.btnDelMaterial.TextOffsetY = 0;
            this.btnDelMaterial.Click += new System.EventHandler(this.btnDelMaterial_Click);
            // 
            // btnClear
            // 
            this.btnClear.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnClear.BackColor = System.Drawing.Color.Transparent;
            this.btnClear.BorderColor = System.Drawing.Color.SlateGray;
            this.btnClear.BorderRadius = 5;
            this.btnClear.BorderSize = 2;
            this.btnClear.ButtonImage = null;
            this.btnClear.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnClear.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnClear.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnClear.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClear.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClear.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnClear.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnClear.IconPadding = 6;
            this.btnClear.IconSize = 20;
            this.btnClear.Location = new System.Drawing.Point(201, 1);
            this.btnClear.Margin = new System.Windows.Forms.Padding(1);
            this.btnClear.Name = "btnClear";
            this.btnClear.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClear.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnClear.ShowAccentLine = false;
            this.btnClear.Size = new System.Drawing.Size(98, 49);
            this.btnClear.TabIndex = 16;
            this.btnClear.Text = "CLEAR";
            this.btnClear.TextColor = System.Drawing.Color.White;
            this.btnClear.TextOffsetX = 0;
            this.btnClear.TextOffsetY = 0;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnCancel.BackColor = System.Drawing.Color.Transparent;
            this.btnCancel.BorderColor = System.Drawing.Color.SlateGray;
            this.btnCancel.BorderRadius = 5;
            this.btnCancel.BorderSize = 2;
            this.btnCancel.ButtonImage = null;
            this.btnCancel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnCancel.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnCancel.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnCancel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnCancel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnCancel.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnCancel.IconPadding = 6;
            this.btnCancel.IconSize = 20;
            this.btnCancel.Location = new System.Drawing.Point(392, 1);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(1);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnCancel.ShowAccentLine = false;
            this.btnCancel.Size = new System.Drawing.Size(98, 49);
            this.btnCancel.TabIndex = 16;
            this.btnCancel.Text = "CANCEL";
            this.btnCancel.TextColor = System.Drawing.Color.White;
            this.btnCancel.TextOffsetX = 0;
            this.btnCancel.TextOffsetY = 0;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnOK.BackColor = System.Drawing.Color.Transparent;
            this.btnOK.BorderColor = System.Drawing.Color.SlateGray;
            this.btnOK.BorderRadius = 5;
            this.btnOK.BorderSize = 2;
            this.btnOK.ButtonImage = null;
            this.btnOK.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnOK.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnOK.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnOK.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnOK.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnOK.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnOK.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnOK.IconPadding = 6;
            this.btnOK.IconSize = 20;
            this.btnOK.Location = new System.Drawing.Point(492, 1);
            this.btnOK.Margin = new System.Windows.Forms.Padding(1);
            this.btnOK.Name = "btnOK";
            this.btnOK.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOK.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnOK.ShowAccentLine = false;
            this.btnOK.Size = new System.Drawing.Size(98, 49);
            this.btnOK.TabIndex = 16;
            this.btnOK.Text = "OK";
            this.btnOK.TextColor = System.Drawing.Color.White;
            this.btnOK.TextOffsetX = 0;
            this.btnOK.TextOffsetY = 0;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.lblTitle, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.dgvFilter, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel5, 0, 3);
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
            // lblTitle
            // 
            this.lblTitle.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblTitle.BackColor = System.Drawing.Color.Transparent;
            this.lblTitle.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblTitle.BorderRadius = 3;
            this.lblTitle.BorderSize = 1;
            this.lblTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblTitle.FillColor = System.Drawing.Color.DarkSlateBlue;
            this.lblTitle.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTitle.ImagePadding = 6;
            this.lblTitle.ImageSize = 18;
            this.lblTitle.LabelImage = null;
            this.lblTitle.Location = new System.Drawing.Point(1, 1);
            this.lblTitle.Margin = new System.Windows.Forms.Padding(1);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.ShowAccentLine = false;
            this.lblTitle.Size = new System.Drawing.Size(589, 28);
            this.lblTitle.TabIndex = 14;
            this.lblTitle.Text = "PRODUCT SETTING";
            this.lblTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblTitle.TextColor = System.Drawing.Color.White;
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
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 4;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.tableLayoutPanel2.Controls.Add(this.cbSyncDuration, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.comboSyncDuration, 3, 0);
            this.tableLayoutPanel2.Controls.Add(this.lblSyncDuration, 1, 0);
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
            // comboSyncDuration
            // 
            this.comboSyncDuration.Dock = System.Windows.Forms.DockStyle.Fill;
            this.comboSyncDuration.FormattingEnabled = true;
            this.comboSyncDuration.Location = new System.Drawing.Point(474, 3);
            this.comboSyncDuration.Name = "comboSyncDuration";
            this.comboSyncDuration.Size = new System.Drawing.Size(114, 23);
            this.comboSyncDuration.TabIndex = 4;
            // 
            // lblSyncDuration
            // 
            this.lblSyncDuration.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblSyncDuration.BackColor = System.Drawing.Color.Transparent;
            this.lblSyncDuration.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblSyncDuration.BorderRadius = 5;
            this.lblSyncDuration.BorderSize = 1;
            this.lblSyncDuration.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblSyncDuration.FillColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.lblSyncDuration.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSyncDuration.ImagePadding = 6;
            this.lblSyncDuration.ImageSize = 18;
            this.lblSyncDuration.LabelImage = null;
            this.lblSyncDuration.Location = new System.Drawing.Point(332, 1);
            this.lblSyncDuration.Margin = new System.Windows.Forms.Padding(1);
            this.lblSyncDuration.Name = "lblSyncDuration";
            this.lblSyncDuration.ShowAccentLine = false;
            this.lblSyncDuration.Size = new System.Drawing.Size(118, 28);
            this.lblSyncDuration.TabIndex = 9;
            this.lblSyncDuration.Text = "Sync Duration";
            this.lblSyncDuration.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblSyncDuration.TextColor = System.Drawing.Color.White;
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
            ((System.ComponentModel.ISupportInitialize)(this.dgvFilter)).EndInit();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.DataGridView dgvFilter;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.CheckBox cbSyncDuration;
        private System.Windows.Forms.ComboBox comboSyncDuration;
        private CustomControls.ShadedLabel lblTitle;
        private CustomControls.ShadedLabel lblSyncDuration;
        private CustomControls.ShadedButton btnAddMaterial;
        private CustomControls.ShadedButton btnDelMaterial;
        private CustomControls.ShadedButton btnClear;
        private CustomControls.ShadedButton btnCancel;
        private CustomControls.ShadedButton btnOK;
    }
}