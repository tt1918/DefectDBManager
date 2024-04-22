namespace DefectDBManager
{
    partial class FormPreCompDB
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormPreCompDB));
            this.lblLotName = new System.Windows.Forms.Label();
            this.tbLotName = new System.Windows.Forms.TextBox();
            this.gbBasicInfo = new System.Windows.Forms.GroupBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tbMKCDModelName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSearchPTRY0P_Today = new System.Windows.Forms.Button();
            this.btnUpdateMarkingData = new System.Windows.Forms.Button();
            this.btnSearchDB = new System.Windows.Forms.Button();
            this.btnMKCDModel = new System.Windows.Forms.Button();
            this.btnShowSkipParam = new System.Windows.Forms.Button();
            this.gbDBConnection = new System.Windows.Forms.GroupBox();
            this.lblDbConnStateIcon = new System.Windows.Forms.Label();
            this.btnDBConn = new System.Windows.Forms.Button();
            this.lblDbConnState = new System.Windows.Forms.Label();
            this.btnReset = new System.Windows.Forms.Button();
            this.listViewBCNO = new System.Windows.Forms.ListView();
            this.gbPTRYLP = new System.Windows.Forms.GroupBox();
            this.listViewPTRYLP = new System.Windows.Forms.ListView();
            this.gbMRKCTLMST = new System.Windows.Forms.GroupBox();
            this.listViewPTRY0P_TODAY = new System.Windows.Forms.ListView();
            this.gbPTRY0P = new System.Windows.Forms.GroupBox();
            this.listViewPTRY0P = new System.Windows.Forms.ListView();
            this.gbINSPDAT = new System.Windows.Forms.GroupBox();
            this.listViewINSPDAT = new System.Windows.Forms.ListView();
            this.gbFAULTDAT = new System.Windows.Forms.GroupBox();
            this.listViewFAULTDAT = new System.Windows.Forms.ListView();
            this.lblGRANK = new System.Windows.Forms.Label();
            this.lblTotalFaultPage = new System.Windows.Forms.Label();
            this.tbGRank = new System.Windows.Forms.TextBox();
            this.tbFaultPage = new System.Windows.Forms.TextBox();
            this.btnNextFaultPage = new System.Windows.Forms.Button();
            this.btnPrevFaultPage = new System.Windows.Forms.Button();
            this.lblDownloadResult = new System.Windows.Forms.Label();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.splitContainer4 = new System.Windows.Forms.SplitContainer();
            this.splitContainer5 = new System.Windows.Forms.SplitContainer();
            this.splitContainer6 = new System.Windows.Forms.SplitContainer();
            this.splitContainer7 = new System.Windows.Forms.SplitContainer();
            this.gbBasicInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.gbDBConnection.SuspendLayout();
            this.gbPTRYLP.SuspendLayout();
            this.gbMRKCTLMST.SuspendLayout();
            this.gbPTRY0P.SuspendLayout();
            this.gbINSPDAT.SuspendLayout();
            this.gbFAULTDAT.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer4)).BeginInit();
            this.splitContainer4.Panel1.SuspendLayout();
            this.splitContainer4.Panel2.SuspendLayout();
            this.splitContainer4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer5)).BeginInit();
            this.splitContainer5.Panel1.SuspendLayout();
            this.splitContainer5.Panel2.SuspendLayout();
            this.splitContainer5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer6)).BeginInit();
            this.splitContainer6.Panel1.SuspendLayout();
            this.splitContainer6.Panel2.SuspendLayout();
            this.splitContainer6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer7)).BeginInit();
            this.splitContainer7.Panel1.SuspendLayout();
            this.splitContainer7.Panel2.SuspendLayout();
            this.splitContainer7.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblLotName
            // 
            this.lblLotName.BackColor = System.Drawing.SystemColors.Control;
            this.lblLotName.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLotName.Location = new System.Drawing.Point(124, 44);
            this.lblLotName.Name = "lblLotName";
            this.lblLotName.Size = new System.Drawing.Size(66, 17);
            this.lblLotName.TabIndex = 0;
            this.lblLotName.Text = "점착 LOT";
            this.lblLotName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbLotName
            // 
            this.tbLotName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbLotName.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbLotName.Location = new System.Drawing.Point(208, 43);
            this.tbLotName.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbLotName.Name = "tbLotName";
            this.tbLotName.Size = new System.Drawing.Size(138, 16);
            this.tbLotName.TabIndex = 1;
            this.tbLotName.TextChanged += new System.EventHandler(this.tbLotName_TextChanged);
            // 
            // gbBasicInfo
            // 
            this.gbBasicInfo.Controls.Add(this.splitContainer1);
            this.gbBasicInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbBasicInfo.Font = new System.Drawing.Font("Gulim", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbBasicInfo.Location = new System.Drawing.Point(0, 0);
            this.gbBasicInfo.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbBasicInfo.Name = "gbBasicInfo";
            this.gbBasicInfo.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbBasicInfo.Size = new System.Drawing.Size(1180, 100);
            this.gbBasicInfo.TabIndex = 5;
            this.gbBasicInfo.TabStop = false;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.Location = new System.Drawing.Point(3, 16);
            this.splitContainer1.Margin = new System.Windows.Forms.Padding(1);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tbMKCDModelName);
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            this.splitContainer1.Panel1.Controls.Add(this.btnSearchPTRY0P_Today);
            this.splitContainer1.Panel1.Controls.Add(this.btnUpdateMarkingData);
            this.splitContainer1.Panel1.Controls.Add(this.btnSearchDB);
            this.splitContainer1.Panel1.Controls.Add(this.btnMKCDModel);
            this.splitContainer1.Panel1.Controls.Add(this.btnShowSkipParam);
            this.splitContainer1.Panel1.Controls.Add(this.gbDBConnection);
            this.splitContainer1.Panel1.Controls.Add(this.btnReset);
            this.splitContainer1.Panel1.Controls.Add(this.tbLotName);
            this.splitContainer1.Panel1.Controls.Add(this.lblLotName);
            this.splitContainer1.Panel1.Font = new System.Drawing.Font("Gulim", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.listViewBCNO);
            this.splitContainer1.Size = new System.Drawing.Size(1174, 82);
            this.splitContainer1.SplitterDistance = 780;
            this.splitContainer1.TabIndex = 13;
            // 
            // tbMKCDModelName
            // 
            this.tbMKCDModelName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbMKCDModelName.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbMKCDModelName.Location = new System.Drawing.Point(208, 15);
            this.tbMKCDModelName.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbMKCDModelName.Name = "tbMKCDModelName";
            this.tbMKCDModelName.Size = new System.Drawing.Size(138, 16);
            this.tbMKCDModelName.TabIndex = 16;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.SystemColors.Control;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(123, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 17);
            this.label1.TabIndex = 15;
            this.label1.Text = "MKCD 모델";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnSearchPTRY0P_Today
            // 
            this.btnSearchPTRY0P_Today.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSearchPTRY0P_Today.Location = new System.Drawing.Point(353, 9);
            this.btnSearchPTRY0P_Today.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSearchPTRY0P_Today.Name = "btnSearchPTRY0P_Today";
            this.btnSearchPTRY0P_Today.Size = new System.Drawing.Size(73, 67);
            this.btnSearchPTRY0P_Today.TabIndex = 14;
            this.btnSearchPTRY0P_Today.Text = "생산 데이터 검색";
            this.btnSearchPTRY0P_Today.UseVisualStyleBackColor = true;
            this.btnSearchPTRY0P_Today.Click += new System.EventHandler(this.btnSearchPTRY0P_Today_Click);
            // 
            // btnUpdateMarkingData
            // 
            this.btnUpdateMarkingData.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnUpdateMarkingData.Location = new System.Drawing.Point(506, 42);
            this.btnUpdateMarkingData.Name = "btnUpdateMarkingData";
            this.btnUpdateMarkingData.Size = new System.Drawing.Size(89, 33);
            this.btnUpdateMarkingData.TabIndex = 13;
            this.btnUpdateMarkingData.Text = "UPDATE DATA";
            this.btnUpdateMarkingData.UseVisualStyleBackColor = true;
            this.btnUpdateMarkingData.Click += new System.EventHandler(this.btnUpdateMarkingData_Click);
            // 
            // btnSearchDB
            // 
            this.btnSearchDB.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSearchDB.Location = new System.Drawing.Point(430, 9);
            this.btnSearchDB.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSearchDB.Name = "btnSearchDB";
            this.btnSearchDB.Size = new System.Drawing.Size(73, 66);
            this.btnSearchDB.TabIndex = 8;
            this.btnSearchDB.Text = "LOT 검색";
            this.btnSearchDB.UseVisualStyleBackColor = true;
            this.btnSearchDB.Click += new System.EventHandler(this.btnSearchDB_Click);
            // 
            // btnMKCDModel
            // 
            this.btnMKCDModel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnMKCDModel.Location = new System.Drawing.Point(644, 9);
            this.btnMKCDModel.Name = "btnMKCDModel";
            this.btnMKCDModel.Size = new System.Drawing.Size(64, 66);
            this.btnMKCDModel.TabIndex = 11;
            this.btnMKCDModel.Text = "MKCD MODEL";
            this.btnMKCDModel.UseVisualStyleBackColor = true;
            this.btnMKCDModel.Click += new System.EventHandler(this.btnMKCDModel_Click);
            // 
            // btnShowSkipParam
            // 
            this.btnShowSkipParam.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnShowSkipParam.Location = new System.Drawing.Point(713, 9);
            this.btnShowSkipParam.Name = "btnShowSkipParam";
            this.btnShowSkipParam.Size = new System.Drawing.Size(64, 66);
            this.btnShowSkipParam.TabIndex = 11;
            this.btnShowSkipParam.Text = "SKIP PARAM";
            this.btnShowSkipParam.UseVisualStyleBackColor = true;
            this.btnShowSkipParam.Click += new System.EventHandler(this.btnShowSkipParam_Click);
            // 
            // gbDBConnection
            // 
            this.gbDBConnection.Controls.Add(this.lblDbConnStateIcon);
            this.gbDBConnection.Controls.Add(this.btnDBConn);
            this.gbDBConnection.Controls.Add(this.lblDbConnState);
            this.gbDBConnection.Font = new System.Drawing.Font("Segoe UI Semibold", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbDBConnection.Location = new System.Drawing.Point(6, 3);
            this.gbDBConnection.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbDBConnection.Name = "gbDBConnection";
            this.gbDBConnection.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbDBConnection.Size = new System.Drawing.Size(112, 63);
            this.gbDBConnection.TabIndex = 10;
            this.gbDBConnection.TabStop = false;
            this.gbDBConnection.Text = "DB";
            // 
            // lblDbConnStateIcon
            // 
            this.lblDbConnStateIcon.AutoSize = true;
            this.lblDbConnStateIcon.Image = ((System.Drawing.Image)(resources.GetObject("lblDbConnStateIcon.Image")));
            this.lblDbConnStateIcon.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.lblDbConnStateIcon.Location = new System.Drawing.Point(3, 14);
            this.lblDbConnStateIcon.Name = "lblDbConnStateIcon";
            this.lblDbConnStateIcon.Size = new System.Drawing.Size(16, 15);
            this.lblDbConnStateIcon.TabIndex = 9;
            this.lblDbConnStateIcon.Text = "   ";
            // 
            // btnDBConn
            // 
            this.btnDBConn.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDBConn.Location = new System.Drawing.Point(6, 32);
            this.btnDBConn.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnDBConn.Name = "btnDBConn";
            this.btnDBConn.Size = new System.Drawing.Size(101, 24);
            this.btnDBConn.TabIndex = 8;
            this.btnDBConn.Text = "연결";
            this.btnDBConn.UseVisualStyleBackColor = true;
            this.btnDBConn.Click += new System.EventHandler(this.btnDBConn_Click);
            // 
            // lblDbConnState
            // 
            this.lblDbConnState.AutoSize = true;
            this.lblDbConnState.BackColor = System.Drawing.SystemColors.Control;
            this.lblDbConnState.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDbConnState.Location = new System.Drawing.Point(24, 14);
            this.lblDbConnState.Name = "lblDbConnState";
            this.lblDbConnState.Size = new System.Drawing.Size(84, 15);
            this.lblDbConnState.TabIndex = 0;
            this.lblDbConnState.Text = "DISCONECTED";
            this.lblDbConnState.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnReset
            // 
            this.btnReset.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnReset.Location = new System.Drawing.Point(506, 9);
            this.btnReset.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(89, 33);
            this.btnReset.TabIndex = 8;
            this.btnReset.Text = "초기화";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // listViewBCNO
            // 
            this.listViewBCNO.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewBCNO.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewBCNO.HideSelection = false;
            this.listViewBCNO.Location = new System.Drawing.Point(0, 0);
            this.listViewBCNO.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewBCNO.Name = "listViewBCNO";
            this.listViewBCNO.Size = new System.Drawing.Size(390, 82);
            this.listViewBCNO.TabIndex = 7;
            this.listViewBCNO.UseCompatibleStateImageBehavior = false;
            // 
            // gbPTRYLP
            // 
            this.gbPTRYLP.Controls.Add(this.listViewPTRYLP);
            this.gbPTRYLP.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbPTRYLP.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbPTRYLP.Location = new System.Drawing.Point(0, 0);
            this.gbPTRYLP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRYLP.Name = "gbPTRYLP";
            this.gbPTRYLP.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRYLP.Size = new System.Drawing.Size(589, 144);
            this.gbPTRYLP.TabIndex = 6;
            this.gbPTRYLP.TabStop = false;
            this.gbPTRYLP.Text = "PTRYLP";
            // 
            // listViewPTRYLP
            // 
            this.listViewPTRYLP.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewPTRYLP.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewPTRYLP.HideSelection = false;
            this.listViewPTRYLP.Location = new System.Drawing.Point(3, 20);
            this.listViewPTRYLP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewPTRYLP.Name = "listViewPTRYLP";
            this.listViewPTRYLP.Size = new System.Drawing.Size(583, 122);
            this.listViewPTRYLP.TabIndex = 7;
            this.listViewPTRYLP.UseCompatibleStateImageBehavior = false;
            // 
            // gbMRKCTLMST
            // 
            this.gbMRKCTLMST.Controls.Add(this.listViewPTRY0P_TODAY);
            this.gbMRKCTLMST.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbMRKCTLMST.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbMRKCTLMST.Location = new System.Drawing.Point(0, 0);
            this.gbMRKCTLMST.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbMRKCTLMST.Name = "gbMRKCTLMST";
            this.gbMRKCTLMST.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbMRKCTLMST.Size = new System.Drawing.Size(1180, 130);
            this.gbMRKCTLMST.TabIndex = 6;
            this.gbMRKCTLMST.TabStop = false;
            this.gbMRKCTLMST.Text = "PTRY0P_TODAY";
            // 
            // listViewPTRY0P_TODAY
            // 
            this.listViewPTRY0P_TODAY.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewPTRY0P_TODAY.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewPTRY0P_TODAY.HideSelection = false;
            this.listViewPTRY0P_TODAY.Location = new System.Drawing.Point(3, 20);
            this.listViewPTRY0P_TODAY.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewPTRY0P_TODAY.Name = "listViewPTRY0P_TODAY";
            this.listViewPTRY0P_TODAY.Size = new System.Drawing.Size(1174, 108);
            this.listViewPTRY0P_TODAY.TabIndex = 7;
            this.listViewPTRY0P_TODAY.UseCompatibleStateImageBehavior = false;
            // 
            // gbPTRY0P
            // 
            this.gbPTRY0P.Controls.Add(this.listViewPTRY0P);
            this.gbPTRY0P.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbPTRY0P.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbPTRY0P.Location = new System.Drawing.Point(0, 0);
            this.gbPTRY0P.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRY0P.Name = "gbPTRY0P";
            this.gbPTRY0P.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRY0P.Size = new System.Drawing.Size(587, 144);
            this.gbPTRY0P.TabIndex = 6;
            this.gbPTRY0P.TabStop = false;
            this.gbPTRY0P.Text = "PTRY0P";
            // 
            // listViewPTRY0P
            // 
            this.listViewPTRY0P.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewPTRY0P.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewPTRY0P.HideSelection = false;
            this.listViewPTRY0P.Location = new System.Drawing.Point(3, 20);
            this.listViewPTRY0P.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewPTRY0P.Name = "listViewPTRY0P";
            this.listViewPTRY0P.Size = new System.Drawing.Size(581, 122);
            this.listViewPTRY0P.TabIndex = 7;
            this.listViewPTRY0P.UseCompatibleStateImageBehavior = false;
            // 
            // gbINSPDAT
            // 
            this.gbINSPDAT.Controls.Add(this.listViewINSPDAT);
            this.gbINSPDAT.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbINSPDAT.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbINSPDAT.Location = new System.Drawing.Point(0, 0);
            this.gbINSPDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbINSPDAT.Name = "gbINSPDAT";
            this.gbINSPDAT.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbINSPDAT.Size = new System.Drawing.Size(1180, 131);
            this.gbINSPDAT.TabIndex = 6;
            this.gbINSPDAT.TabStop = false;
            this.gbINSPDAT.Text = "INSPDAT";
            // 
            // listViewINSPDAT
            // 
            this.listViewINSPDAT.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewINSPDAT.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewINSPDAT.HideSelection = false;
            this.listViewINSPDAT.Location = new System.Drawing.Point(3, 20);
            this.listViewINSPDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewINSPDAT.Name = "listViewINSPDAT";
            this.listViewINSPDAT.Size = new System.Drawing.Size(1174, 109);
            this.listViewINSPDAT.TabIndex = 7;
            this.listViewINSPDAT.UseCompatibleStateImageBehavior = false;
            // 
            // gbFAULTDAT
            // 
            this.gbFAULTDAT.Controls.Add(this.listViewFAULTDAT);
            this.gbFAULTDAT.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbFAULTDAT.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbFAULTDAT.Location = new System.Drawing.Point(0, 0);
            this.gbFAULTDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbFAULTDAT.Name = "gbFAULTDAT";
            this.gbFAULTDAT.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbFAULTDAT.Size = new System.Drawing.Size(1180, 257);
            this.gbFAULTDAT.TabIndex = 6;
            this.gbFAULTDAT.TabStop = false;
            this.gbFAULTDAT.Text = "FAULTDAT";
            // 
            // listViewFAULTDAT
            // 
            this.listViewFAULTDAT.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listViewFAULTDAT.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listViewFAULTDAT.HideSelection = false;
            this.listViewFAULTDAT.Location = new System.Drawing.Point(6, 18);
            this.listViewFAULTDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewFAULTDAT.Name = "listViewFAULTDAT";
            this.listViewFAULTDAT.Size = new System.Drawing.Size(1167, 232);
            this.listViewFAULTDAT.TabIndex = 7;
            this.listViewFAULTDAT.UseCompatibleStateImageBehavior = false;
            // 
            // lblGRANK
            // 
            this.lblGRANK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lblGRANK.AutoSize = true;
            this.lblGRANK.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblGRANK.Location = new System.Drawing.Point(852, 11);
            this.lblGRANK.Name = "lblGRANK";
            this.lblGRANK.Size = new System.Drawing.Size(52, 17);
            this.lblGRANK.TabIndex = 10;
            this.lblGRANK.Text = "GRANK";
            this.lblGRANK.Visible = false;
            // 
            // lblTotalFaultPage
            // 
            this.lblTotalFaultPage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lblTotalFaultPage.AutoSize = true;
            this.lblTotalFaultPage.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTotalFaultPage.Location = new System.Drawing.Point(1068, 11);
            this.lblTotalFaultPage.Name = "lblTotalFaultPage";
            this.lblTotalFaultPage.Size = new System.Drawing.Size(39, 17);
            this.lblTotalFaultPage.TabIndex = 10;
            this.lblTotalFaultPage.Text = "/1000";
            // 
            // tbGRank
            // 
            this.tbGRank.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbGRank.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbGRank.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbGRank.Location = new System.Drawing.Point(911, 10);
            this.tbGRank.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbGRank.Name = "tbGRank";
            this.tbGRank.Size = new System.Drawing.Size(43, 18);
            this.tbGRank.TabIndex = 11;
            this.tbGRank.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbGRank.Visible = false;
            // 
            // tbFaultPage
            // 
            this.tbFaultPage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbFaultPage.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbFaultPage.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbFaultPage.Location = new System.Drawing.Point(1022, 10);
            this.tbFaultPage.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbFaultPage.Name = "tbFaultPage";
            this.tbFaultPage.Size = new System.Drawing.Size(43, 18);
            this.tbFaultPage.TabIndex = 11;
            this.tbFaultPage.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbFaultPage.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbFaultPage_KeyDown);
            // 
            // btnNextFaultPage
            // 
            this.btnNextFaultPage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnNextFaultPage.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnNextFaultPage.Location = new System.Drawing.Point(1118, 5);
            this.btnNextFaultPage.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnNextFaultPage.Name = "btnNextFaultPage";
            this.btnNextFaultPage.Size = new System.Drawing.Size(55, 28);
            this.btnNextFaultPage.TabIndex = 12;
            this.btnNextFaultPage.Text = "NEXT";
            this.btnNextFaultPage.UseVisualStyleBackColor = true;
            this.btnNextFaultPage.Click += new System.EventHandler(this.btnNextFaultPage_Click);
            // 
            // btnPrevFaultPage
            // 
            this.btnPrevFaultPage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnPrevFaultPage.Font = new System.Drawing.Font("Segoe UI Semibold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnPrevFaultPage.Location = new System.Drawing.Point(962, 5);
            this.btnPrevFaultPage.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnPrevFaultPage.Name = "btnPrevFaultPage";
            this.btnPrevFaultPage.Size = new System.Drawing.Size(55, 28);
            this.btnPrevFaultPage.TabIndex = 12;
            this.btnPrevFaultPage.Text = "PREV";
            this.btnPrevFaultPage.UseVisualStyleBackColor = true;
            this.btnPrevFaultPage.Click += new System.EventHandler(this.btnPrevFaultPage_Click);
            // 
            // lblDownloadResult
            // 
            this.lblDownloadResult.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblDownloadResult.AutoSize = true;
            this.lblDownloadResult.Font = new System.Drawing.Font("Segoe UI Semibold", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDownloadResult.Location = new System.Drawing.Point(10, 10);
            this.lblDownloadResult.Name = "lblDownloadResult";
            this.lblDownloadResult.Size = new System.Drawing.Size(94, 15);
            this.lblDownloadResult.TabIndex = 13;
            this.lblDownloadResult.Text = "DownloadResult";
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer2.IsSplitterFixed = true;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.splitContainer3);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.lblDownloadResult);
            this.splitContainer2.Panel2.Controls.Add(this.btnPrevFaultPage);
            this.splitContainer2.Panel2.Controls.Add(this.lblGRANK);
            this.splitContainer2.Panel2.Controls.Add(this.btnNextFaultPage);
            this.splitContainer2.Panel2.Controls.Add(this.lblTotalFaultPage);
            this.splitContainer2.Panel2.Controls.Add(this.tbFaultPage);
            this.splitContainer2.Panel2.Controls.Add(this.tbGRank);
            this.splitContainer2.Size = new System.Drawing.Size(1180, 819);
            this.splitContainer2.SplitterDistance = 778;
            this.splitContainer2.TabIndex = 14;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.IsSplitterFixed = true;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Margin = new System.Windows.Forms.Padding(1);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.gbBasicInfo);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.splitContainer4);
            this.splitContainer3.Size = new System.Drawing.Size(1180, 778);
            this.splitContainer3.SplitterDistance = 100;
            this.splitContainer3.TabIndex = 0;
            // 
            // splitContainer4
            // 
            this.splitContainer4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer4.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer4.Location = new System.Drawing.Point(0, 0);
            this.splitContainer4.Name = "splitContainer4";
            this.splitContainer4.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer4.Panel1
            // 
            this.splitContainer4.Panel1.Controls.Add(this.gbMRKCTLMST);
            // 
            // splitContainer4.Panel2
            // 
            this.splitContainer4.Panel2.Controls.Add(this.splitContainer5);
            this.splitContainer4.Size = new System.Drawing.Size(1180, 674);
            this.splitContainer4.SplitterDistance = 130;
            this.splitContainer4.TabIndex = 0;
            // 
            // splitContainer5
            // 
            this.splitContainer5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer5.Location = new System.Drawing.Point(0, 0);
            this.splitContainer5.Name = "splitContainer5";
            this.splitContainer5.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer5.Panel1
            // 
            this.splitContainer5.Panel1.Controls.Add(this.splitContainer6);
            // 
            // splitContainer5.Panel2
            // 
            this.splitContainer5.Panel2.Controls.Add(this.splitContainer7);
            this.splitContainer5.Size = new System.Drawing.Size(1180, 540);
            this.splitContainer5.SplitterDistance = 144;
            this.splitContainer5.TabIndex = 0;
            // 
            // splitContainer6
            // 
            this.splitContainer6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer6.IsSplitterFixed = true;
            this.splitContainer6.Location = new System.Drawing.Point(0, 0);
            this.splitContainer6.Name = "splitContainer6";
            // 
            // splitContainer6.Panel1
            // 
            this.splitContainer6.Panel1.Controls.Add(this.gbPTRYLP);
            // 
            // splitContainer6.Panel2
            // 
            this.splitContainer6.Panel2.Controls.Add(this.gbPTRY0P);
            this.splitContainer6.Size = new System.Drawing.Size(1180, 144);
            this.splitContainer6.SplitterDistance = 589;
            this.splitContainer6.TabIndex = 0;
            // 
            // splitContainer7
            // 
            this.splitContainer7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer7.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer7.Location = new System.Drawing.Point(0, 0);
            this.splitContainer7.Name = "splitContainer7";
            this.splitContainer7.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer7.Panel1
            // 
            this.splitContainer7.Panel1.Controls.Add(this.gbINSPDAT);
            // 
            // splitContainer7.Panel2
            // 
            this.splitContainer7.Panel2.Controls.Add(this.gbFAULTDAT);
            this.splitContainer7.Size = new System.Drawing.Size(1180, 392);
            this.splitContainer7.SplitterDistance = 131;
            this.splitContainer7.TabIndex = 0;
            // 
            // FormPreCompDB
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1180, 819);
            this.Controls.Add(this.splitContainer2);
            this.Font = new System.Drawing.Font("Gulim", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MinimizeBox = false;
            this.Name = "FormPreCompDB";
            this.Text = "DEFECT DATA VIEWER";
            this.Load += new System.EventHandler(this.FormDB_Load);
            this.VisibleChanged += new System.EventHandler(this.FormDB_VisibleChanged);
            this.gbBasicInfo.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.gbDBConnection.ResumeLayout(false);
            this.gbDBConnection.PerformLayout();
            this.gbPTRYLP.ResumeLayout(false);
            this.gbMRKCTLMST.ResumeLayout(false);
            this.gbPTRY0P.ResumeLayout(false);
            this.gbINSPDAT.ResumeLayout(false);
            this.gbFAULTDAT.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
            this.splitContainer3.ResumeLayout(false);
            this.splitContainer4.Panel1.ResumeLayout(false);
            this.splitContainer4.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer4)).EndInit();
            this.splitContainer4.ResumeLayout(false);
            this.splitContainer5.Panel1.ResumeLayout(false);
            this.splitContainer5.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer5)).EndInit();
            this.splitContainer5.ResumeLayout(false);
            this.splitContainer6.Panel1.ResumeLayout(false);
            this.splitContainer6.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer6)).EndInit();
            this.splitContainer6.ResumeLayout(false);
            this.splitContainer7.Panel1.ResumeLayout(false);
            this.splitContainer7.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer7)).EndInit();
            this.splitContainer7.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label lblLotName;
        private System.Windows.Forms.TextBox tbLotName;
        private System.Windows.Forms.GroupBox gbBasicInfo;
        private System.Windows.Forms.Button btnSearchDB;
        private System.Windows.Forms.ListView listViewBCNO;
        private System.Windows.Forms.GroupBox gbPTRYLP;
        private System.Windows.Forms.GroupBox gbMRKCTLMST;
        private System.Windows.Forms.GroupBox gbPTRY0P;
        private System.Windows.Forms.GroupBox gbINSPDAT;
        private System.Windows.Forms.GroupBox gbFAULTDAT;
        private System.Windows.Forms.Label lblGRANK;
        private System.Windows.Forms.Label lblTotalFaultPage;
        private System.Windows.Forms.TextBox tbGRank;
        private System.Windows.Forms.TextBox tbFaultPage;
        private System.Windows.Forms.Button btnNextFaultPage;
        private System.Windows.Forms.Button btnPrevFaultPage;
        private System.Windows.Forms.Label lblDownloadResult;
        private System.Windows.Forms.ListView listViewPTRYLP;
        private System.Windows.Forms.ListView listViewPTRY0P_TODAY;
        private System.Windows.Forms.ListView listViewPTRY0P;
        private System.Windows.Forms.ListView listViewINSPDAT;
        private System.Windows.Forms.ListView listViewFAULTDAT;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.Button btnDBConn;
        private System.Windows.Forms.GroupBox gbDBConnection;
        private System.Windows.Forms.Label lblDbConnState;
        private System.Windows.Forms.Label lblDbConnStateIcon;
        private System.Windows.Forms.Button btnShowSkipParam;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.SplitContainer splitContainer4;
        private System.Windows.Forms.SplitContainer splitContainer5;
        private System.Windows.Forms.SplitContainer splitContainer6;
        private System.Windows.Forms.SplitContainer splitContainer7;
        private System.Windows.Forms.Button btnUpdateMarkingData;
        private System.Windows.Forms.Button btnSearchPTRY0P_Today;
        private System.Windows.Forms.TextBox tbMKCDModelName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnMKCDModel;
    }
}