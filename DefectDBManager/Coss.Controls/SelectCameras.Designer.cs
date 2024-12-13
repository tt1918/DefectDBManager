namespace Coss.Controls
{
    partial class SelectCameras
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.checkBoxOptic = new System.Windows.Forms.CheckBox();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.SuspendLayout();
            // 
            // checkBoxOptic
            // 
            this.checkBoxOptic.AutoSize = true;
            this.checkBoxOptic.Dock = System.Windows.Forms.DockStyle.Top;
            this.checkBoxOptic.Location = new System.Drawing.Point(0, 0);
            this.checkBoxOptic.Name = "checkBoxOptic";
            this.checkBoxOptic.Size = new System.Drawing.Size(218, 25);
            this.checkBoxOptic.TabIndex = 0;
            this.checkBoxOptic.Text = "OpticName";
            this.checkBoxOptic.UseVisualStyleBackColor = true;
            this.checkBoxOptic.CheckedChanged += new System.EventHandler(this.checkBoxOptic_CheckedChanged);
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoSize = true;
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 25);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(218, 58);
            this.flowLayoutPanel1.TabIndex = 1;
            // 
            // SelectCameras
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.Controls.Add(this.flowLayoutPanel1);
            this.Controls.Add(this.checkBoxOptic);
            this.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "SelectCameras";
            this.Size = new System.Drawing.Size(218, 83);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkBoxOptic;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
    }
}
