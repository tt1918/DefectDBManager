namespace Coss.Controls
{
    partial class Rollmap
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
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.SuspendLayout();
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Dock = System.Windows.Forms.DockStyle.Right;
            this.vScrollBar1.Location = new System.Drawing.Point(129, 0);
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(17, 149);
            this.vScrollBar1.TabIndex = 7;
            this.vScrollBar1.Visible = false;
            this.vScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar1_Scroll);
            // 
            // Rollmap
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.vScrollBar1);
            this.Name = "Rollmap";
            this.Size = new System.Drawing.Size(146, 149);
            this.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.Rollmap_MouseDoubleClick);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Rollmap_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Rollmap_MouseMove);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Rollmap_MouseUp);
            this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.Rollmap_MouseScroll);
            this.Resize += new System.EventHandler(this.Rollmap_Resize);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.VScrollBar vScrollBar1;
    }
}
