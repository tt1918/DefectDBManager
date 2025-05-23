namespace MarkCompare
{
    partial class FormLotList
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnLoad = new CustomControls.RoundButton();
            this.tcLotSummary = new System.Windows.Forms.TabControl();
            this.tpLotSummery = new System.Windows.Forms.TabPage();
            this.panel1.SuspendLayout();
            this.tcLotSummary.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.btnLoad);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(403, 39);
            this.panel1.TabIndex = 1;
            this.panel1.Visible = false;
            // 
            // btnLoad
            // 
            this.btnLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnLoad.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoad.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoad.BorderColor = System.Drawing.Color.Lavender;
            this.btnLoad.BorderRadius = 5;
            this.btnLoad.BorderSize = 2;
            this.btnLoad.FlatAppearance.BorderSize = 0;
            this.btnLoad.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLoad.ForeColor = System.Drawing.Color.White;
            this.btnLoad.Location = new System.Drawing.Point(303, 2);
            this.btnLoad.Margin = new System.Windows.Forms.Padding(1);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(90, 33);
            this.btnLoad.TabIndex = 1;
            this.btnLoad.Text = "LOAD";
            this.btnLoad.TextColor = System.Drawing.Color.White;
            this.btnLoad.UseVisualStyleBackColor = false;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // tcLotSummary
            // 
            this.tcLotSummary.Controls.Add(this.tpLotSummery);
            this.tcLotSummary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tcLotSummary.Location = new System.Drawing.Point(0, 39);
            this.tcLotSummary.Name = "tcLotSummary";
            this.tcLotSummary.SelectedIndex = 0;
            this.tcLotSummary.Size = new System.Drawing.Size(403, 457);
            this.tcLotSummary.TabIndex = 2;
            // 
            // tpLotSummery
            // 
            this.tpLotSummery.Location = new System.Drawing.Point(4, 24);
            this.tpLotSummery.Name = "tpLotSummery";
            this.tpLotSummery.Padding = new System.Windows.Forms.Padding(3);
            this.tpLotSummery.Size = new System.Drawing.Size(395, 429);
            this.tpLotSummery.TabIndex = 0;
            this.tpLotSummery.Text = "INFO";
            this.tpLotSummery.UseVisualStyleBackColor = true;
            // 
            // FormLotList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(403, 496);
            this.Controls.Add(this.tcLotSummary);
            this.Controls.Add(this.panel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormLotList";
            this.Text = "FormLotList";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormLotList_FormClosing);
            this.Load += new System.EventHandler(this.FormLotList_Load);
            this.VisibleChanged += new System.EventHandler(this.FormLotList_VisibleChanged);
            this.panel1.ResumeLayout(false);
            this.tcLotSummary.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel panel1;
        private CustomControls.RoundButton btnLoad;
        private System.Windows.Forms.TabControl tcLotSummary;
        private System.Windows.Forms.TabPage tpLotSummery;
    }
}