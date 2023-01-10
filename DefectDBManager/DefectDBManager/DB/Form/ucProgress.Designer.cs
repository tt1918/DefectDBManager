namespace DefectDBManager.DB.Form
{
    partial class ucProgress
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
            this.lblLED = new System.Windows.Forms.Label();
            this.lblProcessName = new System.Windows.Forms.Label();
            this.lblState = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lblLED
            // 
            this.lblLED.AutoSize = true;
            this.lblLED.Image = global::DefectDBManager.Properties.Resources.icons8_black_medium_square_16;
            this.lblLED.Location = new System.Drawing.Point(4, 2);
            this.lblLED.Name = "lblLED";
            this.lblLED.Size = new System.Drawing.Size(13, 16);
            this.lblLED.TabIndex = 0;
            this.lblLED.Text = "  ";
            this.lblLED.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblProcessName
            // 
            this.lblProcessName.AutoSize = true;
            this.lblProcessName.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblProcessName.Location = new System.Drawing.Point(22, 3);
            this.lblProcessName.Name = "lblProcessName";
            this.lblProcessName.Size = new System.Drawing.Size(29, 15);
            this.lblProcessName.TabIndex = 1;
            this.lblProcessName.Text = "Title";
            // 
            // lblState
            // 
            this.lblState.AutoSize = true;
            this.lblState.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblState.Location = new System.Drawing.Point(135, 3);
            this.lblState.Name = "lblState";
            this.lblState.Size = new System.Drawing.Size(43, 15);
            this.lblState.TabIndex = 1;
            this.lblState.Text = "READY";
            // 
            // ucProgress
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lblState);
            this.Controls.Add(this.lblProcessName);
            this.Controls.Add(this.lblLED);
            this.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ucProgress";
            this.Size = new System.Drawing.Size(212, 20);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblLED;
        private System.Windows.Forms.Label lblProcessName;
        private System.Windows.Forms.Label lblState;
    }
}
