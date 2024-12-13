namespace Coss.Controls
{
    partial class Jog
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
            this.labelRight = new System.Windows.Forms.Label();
            this.labelLeft = new System.Windows.Forms.Label();
            this.buttonRL = new System.Windows.Forms.Button();
            this.buttonLL = new System.Windows.Forms.Button();
            this.buttonRR = new System.Windows.Forms.Button();
            this.buttonLR = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelRight
            // 
            this.labelRight.AutoSize = true;
            this.labelRight.Location = new System.Drawing.Point(197, 22);
            this.labelRight.Name = "labelRight";
            this.labelRight.Size = new System.Drawing.Size(82, 21);
            this.labelRight.TabIndex = 6;
            this.labelRight.Text = "Pos : 0mm";
            // 
            // labelLeft
            // 
            this.labelLeft.AutoSize = true;
            this.labelLeft.Location = new System.Drawing.Point(10, 22);
            this.labelLeft.Name = "labelLeft";
            this.labelLeft.Size = new System.Drawing.Size(82, 21);
            this.labelLeft.TabIndex = 7;
            this.labelLeft.Text = "Pos : 0mm";
            // 
            // buttonRL
            // 
            this.buttonRL.Location = new System.Drawing.Point(201, 42);
            this.buttonRL.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonRL.Name = "buttonRL";
            this.buttonRL.Size = new System.Drawing.Size(65, 52);
            this.buttonRL.TabIndex = 2;
            this.buttonRL.Text = "+";
            this.buttonRL.UseVisualStyleBackColor = true;
            this.buttonRL.Click += new System.EventHandler(this.buttonRL_Click);
            this.buttonRL.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonRL_MouseDown);
            this.buttonRL.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonRL_MouseUp);
            // 
            // buttonLL
            // 
            this.buttonLL.Location = new System.Drawing.Point(10, 42);
            this.buttonLL.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonLL.Name = "buttonLL";
            this.buttonLL.Size = new System.Drawing.Size(65, 52);
            this.buttonLL.TabIndex = 3;
            this.buttonLL.Text = "-";
            this.buttonLL.UseVisualStyleBackColor = true;
            this.buttonLL.Click += new System.EventHandler(this.buttonLL_Click);
            this.buttonLL.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonLL_MouseDown);
            this.buttonLL.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonLL_MouseUp);
            // 
            // buttonRR
            // 
            this.buttonRR.Location = new System.Drawing.Point(271, 42);
            this.buttonRR.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonRR.Name = "buttonRR";
            this.buttonRR.Size = new System.Drawing.Size(65, 52);
            this.buttonRR.TabIndex = 4;
            this.buttonRR.Text = "-";
            this.buttonRR.UseVisualStyleBackColor = true;
            this.buttonRR.Click += new System.EventHandler(this.buttonRR_Click);
            this.buttonRR.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonRR_MouseDown);
            this.buttonRR.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonRR_MouseUp);
            // 
            // buttonLR
            // 
            this.buttonLR.Location = new System.Drawing.Point(82, 42);
            this.buttonLR.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonLR.Name = "buttonLR";
            this.buttonLR.Size = new System.Drawing.Size(65, 52);
            this.buttonLR.TabIndex = 5;
            this.buttonLR.Text = "+";
            this.buttonLR.UseVisualStyleBackColor = true;
            this.buttonLR.Click += new System.EventHandler(this.buttonLR_Click);
            this.buttonLR.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonLR_MouseDown);
            this.buttonLR.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonLR_MouseUp);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.labelRight);
            this.groupBox1.Controls.Add(this.labelLeft);
            this.groupBox1.Controls.Add(this.buttonRL);
            this.groupBox1.Controls.Add(this.buttonLL);
            this.groupBox1.Controls.Add(this.buttonRR);
            this.groupBox1.Controls.Add(this.buttonLR);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(351, 104);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Name";
            // 
            // Jog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Jog";
            this.Size = new System.Drawing.Size(351, 104);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelRight;
        private System.Windows.Forms.Label labelLeft;
        private System.Windows.Forms.Button buttonRL;
        private System.Windows.Forms.Button buttonLL;
        private System.Windows.Forms.Button buttonRR;
        private System.Windows.Forms.Button buttonLR;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}
