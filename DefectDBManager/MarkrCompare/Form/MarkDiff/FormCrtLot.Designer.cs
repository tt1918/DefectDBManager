namespace MarkrCompare
{
    partial class FormCrtLot
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvLotInfo = new System.Windows.Forms.DataGridView();
            this.lblLotName = new CustomControls.RoundLabel();
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvLotInfo)).BeginInit();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.lblLotName, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.dgvLotInfo, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(493, 355);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // dgvLotInfo
            // 
            this.dgvLotInfo.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvLotInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvLotInfo.Location = new System.Drawing.Point(3, 33);
            this.dgvLotInfo.Name = "dgvLotInfo";
            this.dgvLotInfo.RowTemplate.Height = 23;
            this.dgvLotInfo.Size = new System.Drawing.Size(487, 319);
            this.dgvLotInfo.TabIndex = 0;
            // 
            // lblLotName
            // 
            this.lblLotName.AutoSize = true;
            this.lblLotName.BkColor = System.Drawing.Color.MidnightBlue;
            this.lblLotName.BorderColor = System.Drawing.Color.LightSteelBlue;
            this.lblLotName.CornerR = 15;
            this.lblLotName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblLotName.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLotName.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblLotName.IsFillLB = false;
            this.lblLotName.IsFillLT = false;
            this.lblLotName.IsFillRB = false;
            this.lblLotName.IsFillRT = false;
            this.lblLotName.Location = new System.Drawing.Point(2, 2);
            this.lblLotName.Margin = new System.Windows.Forms.Padding(2);
            this.lblLotName.Name = "lblLotName";
            this.lblLotName.Size = new System.Drawing.Size(489, 26);
            this.lblLotName.TabIndex = 1;
            this.lblLotName.Text = "LOT : ";
            this.lblLotName.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.lblLotName.Thickness = 1;
            // 
            // FormCrtLot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(493, 355);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormCrtLot";
            this.Text = "FormCrtLot";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvLotInfo)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.DataGridView dgvLotInfo;
        private CustomControls.RoundLabel lblLotName;
    }
}