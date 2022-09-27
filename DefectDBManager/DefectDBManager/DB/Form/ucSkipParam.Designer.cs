namespace DefectDBManager
{
    partial class ucSkipParam
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
            this.gbCtrlName = new System.Windows.Forms.GroupBox();
            this.tbSizeMax = new System.Windows.Forms.TextBox();
            this.tbSizeMin = new System.Windows.Forms.TextBox();
            this.tbSizeYMax = new System.Windows.Forms.TextBox();
            this.tbSizeYMin = new System.Windows.Forms.TextBox();
            this.tbSizeXMax = new System.Windows.Forms.TextBox();
            this.tbSizeXMin = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.lblMax = new System.Windows.Forms.Label();
            this.lblMin = new System.Windows.Forms.Label();
            this.lblSize = new System.Windows.Forms.Label();
            this.lblSizeY = new System.Windows.Forms.Label();
            this.lblSizeX = new System.Windows.Forms.Label();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.gbCtrlName.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbCtrlName
            // 
            this.gbCtrlName.Controls.Add(this.tbSizeMax);
            this.gbCtrlName.Controls.Add(this.tbSizeMin);
            this.gbCtrlName.Controls.Add(this.tbSizeYMax);
            this.gbCtrlName.Controls.Add(this.tbSizeYMin);
            this.gbCtrlName.Controls.Add(this.tbSizeXMax);
            this.gbCtrlName.Controls.Add(this.tbSizeXMin);
            this.gbCtrlName.Controls.Add(this.label3);
            this.gbCtrlName.Controls.Add(this.label2);
            this.gbCtrlName.Controls.Add(this.label1);
            this.gbCtrlName.Controls.Add(this.lblMax);
            this.gbCtrlName.Controls.Add(this.lblMin);
            this.gbCtrlName.Controls.Add(this.lblSize);
            this.gbCtrlName.Controls.Add(this.lblSizeY);
            this.gbCtrlName.Controls.Add(this.lblSizeX);
            this.gbCtrlName.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbCtrlName.Location = new System.Drawing.Point(4, 0);
            this.gbCtrlName.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbCtrlName.Name = "gbCtrlName";
            this.gbCtrlName.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbCtrlName.Size = new System.Drawing.Size(201, 114);
            this.gbCtrlName.TabIndex = 0;
            this.gbCtrlName.TabStop = false;
            this.gbCtrlName.Text = "name";
            // 
            // tbSizeMax
            // 
            this.tbSizeMax.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSizeMax.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.tbSizeMax.Location = new System.Drawing.Point(111, 89);
            this.tbSizeMax.Name = "tbSizeMax";
            this.tbSizeMax.Size = new System.Drawing.Size(47, 14);
            this.tbSizeMax.TabIndex = 2;
            // 
            // tbSizeMin
            // 
            this.tbSizeMin.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSizeMin.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.tbSizeMin.Location = new System.Drawing.Point(61, 89);
            this.tbSizeMin.Name = "tbSizeMin";
            this.tbSizeMin.Size = new System.Drawing.Size(47, 14);
            this.tbSizeMin.TabIndex = 2;
            // 
            // tbSizeYMax
            // 
            this.tbSizeYMax.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSizeYMax.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.tbSizeYMax.Location = new System.Drawing.Point(111, 66);
            this.tbSizeYMax.Name = "tbSizeYMax";
            this.tbSizeYMax.Size = new System.Drawing.Size(47, 14);
            this.tbSizeYMax.TabIndex = 2;
            // 
            // tbSizeYMin
            // 
            this.tbSizeYMin.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSizeYMin.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.tbSizeYMin.Location = new System.Drawing.Point(61, 66);
            this.tbSizeYMin.Name = "tbSizeYMin";
            this.tbSizeYMin.Size = new System.Drawing.Size(47, 14);
            this.tbSizeYMin.TabIndex = 2;
            // 
            // tbSizeXMax
            // 
            this.tbSizeXMax.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSizeXMax.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.tbSizeXMax.Location = new System.Drawing.Point(111, 42);
            this.tbSizeXMax.Name = "tbSizeXMax";
            this.tbSizeXMax.Size = new System.Drawing.Size(47, 14);
            this.tbSizeXMax.TabIndex = 2;
            // 
            // tbSizeXMin
            // 
            this.tbSizeXMin.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSizeXMin.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.tbSizeXMin.Location = new System.Drawing.Point(60, 42);
            this.tbSizeXMin.Name = "tbSizeXMin";
            this.tbSizeXMin.Size = new System.Drawing.Size(47, 14);
            this.tbSizeXMin.TabIndex = 2;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label3.Location = new System.Drawing.Point(164, 88);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 13);
            this.label3.TabIndex = 1;
            this.label3.Text = "mm";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label2.Location = new System.Drawing.Point(164, 65);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "mm";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.Location = new System.Drawing.Point(164, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "mm";
            // 
            // lblMax
            // 
            this.lblMax.AutoSize = true;
            this.lblMax.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblMax.Location = new System.Drawing.Point(115, 19);
            this.lblMax.Name = "lblMax";
            this.lblMax.Size = new System.Drawing.Size(41, 13);
            this.lblMax.TabIndex = 1;
            this.lblMax.Text = "Max.";
            // 
            // lblMin
            // 
            this.lblMin.AutoSize = true;
            this.lblMin.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblMin.Location = new System.Drawing.Point(66, 19);
            this.lblMin.Name = "lblMin";
            this.lblMin.Size = new System.Drawing.Size(37, 13);
            this.lblMin.TabIndex = 1;
            this.lblMin.Text = "Min.";
            // 
            // lblSize
            // 
            this.lblSize.AutoSize = true;
            this.lblSize.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSize.Location = new System.Drawing.Point(6, 88);
            this.lblSize.Name = "lblSize";
            this.lblSize.Size = new System.Drawing.Size(40, 13);
            this.lblSize.TabIndex = 1;
            this.lblSize.Text = "SIZE";
            // 
            // lblSizeY
            // 
            this.lblSizeY.AutoSize = true;
            this.lblSizeY.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSizeY.Location = new System.Drawing.Point(6, 65);
            this.lblSizeY.Name = "lblSizeY";
            this.lblSizeY.Size = new System.Drawing.Size(54, 13);
            this.lblSizeY.TabIndex = 1;
            this.lblSizeY.Text = "SIZE Y";
            // 
            // lblSizeX
            // 
            this.lblSizeX.AutoSize = true;
            this.lblSizeX.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSizeX.Location = new System.Drawing.Point(6, 41);
            this.lblSizeX.Name = "lblSizeX";
            this.lblSizeX.Size = new System.Drawing.Size(54, 13);
            this.lblSizeX.TabIndex = 0;
            this.lblSizeX.Text = "SIZE X";
            // 
            // ucSkipParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbCtrlName);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ucSkipParam";
            this.Size = new System.Drawing.Size(209, 117);
            this.Load += new System.EventHandler(this.ucSkipParam_Load);
            this.VisibleChanged += new System.EventHandler(this.ucSkipParam_VisibleChanged);
            this.gbCtrlName.ResumeLayout(false);
            this.gbCtrlName.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbCtrlName;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.TextBox tbSizeMax;
        private System.Windows.Forms.TextBox tbSizeMin;
        private System.Windows.Forms.TextBox tbSizeYMax;
        private System.Windows.Forms.TextBox tbSizeYMin;
        private System.Windows.Forms.TextBox tbSizeXMax;
        private System.Windows.Forms.TextBox tbSizeXMin;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblMax;
        private System.Windows.Forms.Label lblMin;
        private System.Windows.Forms.Label lblSize;
        private System.Windows.Forms.Label lblSizeY;
        private System.Windows.Forms.Label lblSizeX;
    }
}
