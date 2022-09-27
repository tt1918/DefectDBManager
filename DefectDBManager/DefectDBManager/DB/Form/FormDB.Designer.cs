namespace DefectDBManager
{
    partial class FormDB
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormDB));
            this.lblLotName = new System.Windows.Forms.Label();
            this.tbLotName = new System.Windows.Forms.TextBox();
            this.btnOpenCSV = new System.Windows.Forms.Button();
            this.gbSearchTime = new System.Windows.Forms.GroupBox();
            this.btnOpenCSV2 = new System.Windows.Forms.Button();
            this.btnApplySearchTime = new System.Windows.Forms.Button();
            this.lblSearchEndTime = new System.Windows.Forms.Label();
            this.lblSearchStartTime = new System.Windows.Forms.Label();
            this.tbSearchEndTime = new System.Windows.Forms.TextBox();
            this.tbSearchStartTime = new System.Windows.Forms.TextBox();
            this.cbSetSearchTime = new System.Windows.Forms.CheckBox();
            this.cbDestination = new System.Windows.Forms.ComboBox();
            this.gbBasicInfo = new System.Windows.Forms.GroupBox();
            this.btnEditDefectClass = new System.Windows.Forms.Button();
            this.btnShowSkipParam = new System.Windows.Forms.Button();
            this.gbDBConnection = new System.Windows.Forms.GroupBox();
            this.lblDbConnStateIcon = new System.Windows.Forms.Label();
            this.btnDBConn = new System.Windows.Forms.Button();
            this.lblDbConnState = new System.Windows.Forms.Label();
            this.btnReset = new System.Windows.Forms.Button();
            this.btnExportCSV = new System.Windows.Forms.Button();
            this.btnXOFSMST = new System.Windows.Forms.Button();
            this.btnSearchDB = new System.Windows.Forms.Button();
            this.listViewBCNO = new System.Windows.Forms.ListView();
            this.gbOption = new System.Windows.Forms.GroupBox();
            this.cbUseMask = new System.Windows.Forms.CheckBox();
            this.gbMarkingGroup = new System.Windows.Forms.GroupBox();
            this.cbUseETC = new System.Windows.Forms.CheckBox();
            this.cbUseTG = new System.Windows.Forms.CheckBox();
            this.cbUseES = new System.Windows.Forms.CheckBox();
            this.lblDestination = new System.Windows.Forms.Label();
            this.gbPTRYLP = new System.Windows.Forms.GroupBox();
            this.listViewPTRYLP = new System.Windows.Forms.ListView();
            this.gbMRKCTLMST = new System.Windows.Forms.GroupBox();
            this.listViewMRKCTLMST = new System.Windows.Forms.ListView();
            this.gbPTRYOP = new System.Windows.Forms.GroupBox();
            this.listViewPTRYOP = new System.Windows.Forms.ListView();
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
            this.gbSearchTime.SuspendLayout();
            this.gbBasicInfo.SuspendLayout();
            this.gbDBConnection.SuspendLayout();
            this.gbOption.SuspendLayout();
            this.gbMarkingGroup.SuspendLayout();
            this.gbPTRYLP.SuspendLayout();
            this.gbMRKCTLMST.SuspendLayout();
            this.gbPTRYOP.SuspendLayout();
            this.gbINSPDAT.SuspendLayout();
            this.gbFAULTDAT.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblLotName
            // 
            this.lblLotName.BackColor = System.Drawing.SystemColors.Control;
            this.lblLotName.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblLotName.Location = new System.Drawing.Point(145, 22);
            this.lblLotName.Name = "lblLotName";
            this.lblLotName.Size = new System.Drawing.Size(84, 17);
            this.lblLotName.TabIndex = 0;
            this.lblLotName.Text = "점착 LOT";
            this.lblLotName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbLotName
            // 
            this.tbLotName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbLotName.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.tbLotName.Location = new System.Drawing.Point(234, 23);
            this.tbLotName.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbLotName.Name = "tbLotName";
            this.tbLotName.Size = new System.Drawing.Size(166, 14);
            this.tbLotName.TabIndex = 1;
            // 
            // btnOpenCSV
            // 
            this.btnOpenCSV.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnOpenCSV.Location = new System.Drawing.Point(708, 54);
            this.btnOpenCSV.Name = "btnOpenCSV";
            this.btnOpenCSV.Size = new System.Drawing.Size(85, 39);
            this.btnOpenCSV.TabIndex = 2;
            this.btnOpenCSV.Text = "OPEN CSV";
            this.btnOpenCSV.UseVisualStyleBackColor = true;
            this.btnOpenCSV.Click += new System.EventHandler(this.btnOpenCSV_Click);
            // 
            // gbSearchTime
            // 
            this.gbSearchTime.Controls.Add(this.btnOpenCSV2);
            this.gbSearchTime.Controls.Add(this.btnApplySearchTime);
            this.gbSearchTime.Controls.Add(this.lblSearchEndTime);
            this.gbSearchTime.Controls.Add(this.lblSearchStartTime);
            this.gbSearchTime.Controls.Add(this.tbSearchEndTime);
            this.gbSearchTime.Controls.Add(this.tbSearchStartTime);
            this.gbSearchTime.Controls.Add(this.cbSetSearchTime);
            this.gbSearchTime.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbSearchTime.Location = new System.Drawing.Point(408, 10);
            this.gbSearchTime.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbSearchTime.Name = "gbSearchTime";
            this.gbSearchTime.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbSearchTime.Size = new System.Drawing.Size(206, 59);
            this.gbSearchTime.TabIndex = 3;
            this.gbSearchTime.TabStop = false;
            // 
            // btnOpenCSV2
            // 
            this.btnOpenCSV2.Location = new System.Drawing.Point(125, -1);
            this.btnOpenCSV2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnOpenCSV2.Name = "btnOpenCSV2";
            this.btnOpenCSV2.Size = new System.Drawing.Size(26, 15);
            this.btnOpenCSV2.TabIndex = 6;
            this.btnOpenCSV2.UseVisualStyleBackColor = true;
            this.btnOpenCSV2.Visible = false;
            // 
            // btnApplySearchTime
            // 
            this.btnApplySearchTime.Enabled = false;
            this.btnApplySearchTime.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnApplySearchTime.Location = new System.Drawing.Point(152, 16);
            this.btnApplySearchTime.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnApplySearchTime.Name = "btnApplySearchTime";
            this.btnApplySearchTime.Size = new System.Drawing.Size(50, 36);
            this.btnApplySearchTime.TabIndex = 3;
            this.btnApplySearchTime.Text = "적용";
            this.btnApplySearchTime.UseVisualStyleBackColor = true;
            this.btnApplySearchTime.Click += new System.EventHandler(this.btnApplySearchTime_Click);
            // 
            // lblSearchEndTime
            // 
            this.lblSearchEndTime.AutoSize = true;
            this.lblSearchEndTime.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSearchEndTime.Location = new System.Drawing.Point(8, 37);
            this.lblSearchEndTime.Name = "lblSearchEndTime";
            this.lblSearchEndTime.Size = new System.Drawing.Size(62, 12);
            this.lblSearchEndTime.TabIndex = 2;
            this.lblSearchEndTime.Text = "종료 시간";
            // 
            // lblSearchStartTime
            // 
            this.lblSearchStartTime.AutoSize = true;
            this.lblSearchStartTime.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSearchStartTime.Location = new System.Drawing.Point(8, 18);
            this.lblSearchStartTime.Name = "lblSearchStartTime";
            this.lblSearchStartTime.Size = new System.Drawing.Size(62, 12);
            this.lblSearchStartTime.TabIndex = 2;
            this.lblSearchStartTime.Text = "시작 시간";
            // 
            // tbSearchEndTime
            // 
            this.tbSearchEndTime.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSearchEndTime.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSearchEndTime.Location = new System.Drawing.Point(78, 36);
            this.tbSearchEndTime.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbSearchEndTime.Name = "tbSearchEndTime";
            this.tbSearchEndTime.Size = new System.Drawing.Size(71, 15);
            this.tbSearchEndTime.TabIndex = 1;
            // 
            // tbSearchStartTime
            // 
            this.tbSearchStartTime.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbSearchStartTime.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSearchStartTime.Location = new System.Drawing.Point(78, 17);
            this.tbSearchStartTime.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbSearchStartTime.Name = "tbSearchStartTime";
            this.tbSearchStartTime.Size = new System.Drawing.Size(71, 15);
            this.tbSearchStartTime.TabIndex = 1;
            // 
            // cbSetSearchTime
            // 
            this.cbSetSearchTime.AutoSize = true;
            this.cbSetSearchTime.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbSetSearchTime.Location = new System.Drawing.Point(6, -1);
            this.cbSetSearchTime.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbSetSearchTime.Name = "cbSetSearchTime";
            this.cbSetSearchTime.Size = new System.Drawing.Size(120, 17);
            this.cbSetSearchTime.TabIndex = 0;
            this.cbSetSearchTime.Text = "검색 시간 설정";
            this.cbSetSearchTime.UseVisualStyleBackColor = true;
            this.cbSetSearchTime.CheckedChanged += new System.EventHandler(this.cbSetSearchTime_CheckedChanged);
            // 
            // cbDestination
            // 
            this.cbDestination.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbDestination.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.cbDestination.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.cbDestination.FormattingEnabled = true;
            this.cbDestination.Location = new System.Drawing.Point(234, 44);
            this.cbDestination.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbDestination.Name = "cbDestination";
            this.cbDestination.Size = new System.Drawing.Size(166, 20);
            this.cbDestination.TabIndex = 0;
            this.cbDestination.SelectedIndexChanged += new System.EventHandler(this.cbDestination_SelectedIndexChanged);
            // 
            // gbBasicInfo
            // 
            this.gbBasicInfo.Controls.Add(this.btnEditDefectClass);
            this.gbBasicInfo.Controls.Add(this.btnShowSkipParam);
            this.gbBasicInfo.Controls.Add(this.gbDBConnection);
            this.gbBasicInfo.Controls.Add(this.cbDestination);
            this.gbBasicInfo.Controls.Add(this.btnReset);
            this.gbBasicInfo.Controls.Add(this.btnExportCSV);
            this.gbBasicInfo.Controls.Add(this.btnXOFSMST);
            this.gbBasicInfo.Controls.Add(this.btnSearchDB);
            this.gbBasicInfo.Controls.Add(this.listViewBCNO);
            this.gbBasicInfo.Controls.Add(this.gbOption);
            this.gbBasicInfo.Controls.Add(this.gbMarkingGroup);
            this.gbBasicInfo.Controls.Add(this.lblDestination);
            this.gbBasicInfo.Controls.Add(this.lblLotName);
            this.gbBasicInfo.Controls.Add(this.tbLotName);
            this.gbBasicInfo.Controls.Add(this.gbSearchTime);
            this.gbBasicInfo.Controls.Add(this.btnOpenCSV);
            this.gbBasicInfo.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbBasicInfo.Location = new System.Drawing.Point(3, 2);
            this.gbBasicInfo.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbBasicInfo.Name = "gbBasicInfo";
            this.gbBasicInfo.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbBasicInfo.Size = new System.Drawing.Size(1174, 141);
            this.gbBasicInfo.TabIndex = 5;
            this.gbBasicInfo.TabStop = false;
            // 
            // btnEditDefectClass
            // 
            this.btnEditDefectClass.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnEditDefectClass.Location = new System.Drawing.Point(473, 82);
            this.btnEditDefectClass.Name = "btnEditDefectClass";
            this.btnEditDefectClass.Size = new System.Drawing.Size(111, 27);
            this.btnEditDefectClass.TabIndex = 12;
            this.btnEditDefectClass.Text = "EDIT CLASS";
            this.btnEditDefectClass.UseVisualStyleBackColor = true;
            this.btnEditDefectClass.Click += new System.EventHandler(this.btnEditDefectClass_Click);
            // 
            // btnShowSkipParam
            // 
            this.btnShowSkipParam.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnShowSkipParam.Location = new System.Drawing.Point(357, 82);
            this.btnShowSkipParam.Name = "btnShowSkipParam";
            this.btnShowSkipParam.Size = new System.Drawing.Size(111, 27);
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
            this.gbDBConnection.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbDBConnection.Location = new System.Drawing.Point(5, 10);
            this.gbDBConnection.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbDBConnection.Name = "gbDBConnection";
            this.gbDBConnection.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbDBConnection.Size = new System.Drawing.Size(134, 59);
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
            this.lblDbConnStateIcon.Size = new System.Drawing.Size(20, 12);
            this.lblDbConnStateIcon.TabIndex = 9;
            this.lblDbConnStateIcon.Text = "   ";
            // 
            // btnDBConn
            // 
            this.btnDBConn.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDBConn.Location = new System.Drawing.Point(23, 29);
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
            this.lblDbConnState.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblDbConnState.Location = new System.Drawing.Point(24, 14);
            this.lblDbConnState.Name = "lblDbConnState";
            this.lblDbConnState.Size = new System.Drawing.Size(103, 12);
            this.lblDbConnState.TabIndex = 0;
            this.lblDbConnState.Text = "DISCONECTED";
            this.lblDbConnState.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnReset
            // 
            this.btnReset.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnReset.Location = new System.Drawing.Point(797, 13);
            this.btnReset.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(85, 39);
            this.btnReset.TabIndex = 8;
            this.btnReset.Text = "초기화";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // btnExportCSV
            // 
            this.btnExportCSV.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnExportCSV.Location = new System.Drawing.Point(797, 53);
            this.btnExportCSV.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnExportCSV.Name = "btnExportCSV";
            this.btnExportCSV.Size = new System.Drawing.Size(85, 39);
            this.btnExportCSV.TabIndex = 8;
            this.btnExportCSV.Text = "EXPORT CSV";
            this.btnExportCSV.UseVisualStyleBackColor = true;
            this.btnExportCSV.Click += new System.EventHandler(this.btnExportCSV_Click);
            // 
            // btnXOFSMST
            // 
            this.btnXOFSMST.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnXOFSMST.Location = new System.Drawing.Point(708, 95);
            this.btnXOFSMST.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnXOFSMST.Name = "btnXOFSMST";
            this.btnXOFSMST.Size = new System.Drawing.Size(85, 39);
            this.btnXOFSMST.TabIndex = 8;
            this.btnXOFSMST.Text = "Show More";
            this.btnXOFSMST.UseVisualStyleBackColor = true;
            this.btnXOFSMST.Click += new System.EventHandler(this.btnXOFSMST_Click);
            // 
            // btnSearchDB
            // 
            this.btnSearchDB.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSearchDB.Location = new System.Drawing.Point(708, 13);
            this.btnSearchDB.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSearchDB.Name = "btnSearchDB";
            this.btnSearchDB.Size = new System.Drawing.Size(85, 39);
            this.btnSearchDB.TabIndex = 8;
            this.btnSearchDB.Text = "검색";
            this.btnSearchDB.UseVisualStyleBackColor = true;
            this.btnSearchDB.Click += new System.EventHandler(this.btnSearchDB_Click);
            // 
            // listViewBCNO
            // 
            this.listViewBCNO.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.listViewBCNO.HideSelection = false;
            this.listViewBCNO.Location = new System.Drawing.Point(886, 10);
            this.listViewBCNO.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewBCNO.Name = "listViewBCNO";
            this.listViewBCNO.Size = new System.Drawing.Size(279, 123);
            this.listViewBCNO.TabIndex = 7;
            this.listViewBCNO.UseCompatibleStateImageBehavior = false;
            // 
            // gbOption
            // 
            this.gbOption.Controls.Add(this.cbUseMask);
            this.gbOption.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbOption.Location = new System.Drawing.Point(202, 72);
            this.gbOption.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbOption.Name = "gbOption";
            this.gbOption.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbOption.Size = new System.Drawing.Size(149, 40);
            this.gbOption.TabIndex = 6;
            this.gbOption.TabStop = false;
            this.gbOption.Text = "Option";
            // 
            // cbUseMask
            // 
            this.cbUseMask.AutoSize = true;
            this.cbUseMask.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.cbUseMask.Location = new System.Drawing.Point(8, 17);
            this.cbUseMask.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbUseMask.Name = "cbUseMask";
            this.cbUseMask.Size = new System.Drawing.Size(138, 16);
            this.cbUseMask.TabIndex = 0;
            this.cbUseMask.Text = "검사 데이터 마스크";
            this.cbUseMask.UseVisualStyleBackColor = true;
            // 
            // gbMarkingGroup
            // 
            this.gbMarkingGroup.Controls.Add(this.cbUseETC);
            this.gbMarkingGroup.Controls.Add(this.cbUseTG);
            this.gbMarkingGroup.Controls.Add(this.cbUseES);
            this.gbMarkingGroup.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbMarkingGroup.Location = new System.Drawing.Point(3, 72);
            this.gbMarkingGroup.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbMarkingGroup.Name = "gbMarkingGroup";
            this.gbMarkingGroup.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbMarkingGroup.Size = new System.Drawing.Size(193, 40);
            this.gbMarkingGroup.TabIndex = 6;
            this.gbMarkingGroup.TabStop = false;
            this.gbMarkingGroup.Text = "마킹 선택";
            // 
            // cbUseETC
            // 
            this.cbUseETC.AutoSize = true;
            this.cbUseETC.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.cbUseETC.Location = new System.Drawing.Point(134, 17);
            this.cbUseETC.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbUseETC.Name = "cbUseETC";
            this.cbUseETC.Size = new System.Drawing.Size(50, 16);
            this.cbUseETC.TabIndex = 0;
            this.cbUseETC.Text = "그외";
            this.cbUseETC.UseVisualStyleBackColor = true;
            // 
            // cbUseTG
            // 
            this.cbUseTG.AutoSize = true;
            this.cbUseTG.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.cbUseTG.Location = new System.Drawing.Point(71, 17);
            this.cbUseTG.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbUseTG.Name = "cbUseTG";
            this.cbUseTG.Size = new System.Drawing.Size(50, 16);
            this.cbUseTG.TabIndex = 0;
            this.cbUseTG.Text = "점착";
            this.cbUseTG.UseVisualStyleBackColor = true;
            // 
            // cbUseES
            // 
            this.cbUseES.AutoSize = true;
            this.cbUseES.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.cbUseES.Location = new System.Drawing.Point(8, 17);
            this.cbUseES.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbUseES.Name = "cbUseES";
            this.cbUseES.Size = new System.Drawing.Size(50, 16);
            this.cbUseES.TabIndex = 0;
            this.cbUseES.Text = "연신";
            this.cbUseES.UseVisualStyleBackColor = true;
            // 
            // lblDestination
            // 
            this.lblDestination.BackColor = System.Drawing.SystemColors.Control;
            this.lblDestination.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblDestination.Location = new System.Drawing.Point(145, 46);
            this.lblDestination.Name = "lblDestination";
            this.lblDestination.Size = new System.Drawing.Size(84, 17);
            this.lblDestination.TabIndex = 0;
            this.lblDestination.Text = "출하처";
            this.lblDestination.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // gbPTRYLP
            // 
            this.gbPTRYLP.Controls.Add(this.listViewPTRYLP);
            this.gbPTRYLP.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbPTRYLP.Location = new System.Drawing.Point(3, 147);
            this.gbPTRYLP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRYLP.Name = "gbPTRYLP";
            this.gbPTRYLP.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRYLP.Size = new System.Drawing.Size(1173, 132);
            this.gbPTRYLP.TabIndex = 6;
            this.gbPTRYLP.TabStop = false;
            this.gbPTRYLP.Text = "PTRYLP";
            // 
            // listViewPTRYLP
            // 
            this.listViewPTRYLP.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.listViewPTRYLP.HideSelection = false;
            this.listViewPTRYLP.Location = new System.Drawing.Point(6, 17);
            this.listViewPTRYLP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewPTRYLP.Name = "listViewPTRYLP";
            this.listViewPTRYLP.Size = new System.Drawing.Size(1159, 108);
            this.listViewPTRYLP.TabIndex = 7;
            this.listViewPTRYLP.UseCompatibleStateImageBehavior = false;
            // 
            // gbMRKCTLMST
            // 
            this.gbMRKCTLMST.Controls.Add(this.listViewMRKCTLMST);
            this.gbMRKCTLMST.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbMRKCTLMST.Location = new System.Drawing.Point(4, 284);
            this.gbMRKCTLMST.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbMRKCTLMST.Name = "gbMRKCTLMST";
            this.gbMRKCTLMST.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbMRKCTLMST.Size = new System.Drawing.Size(582, 133);
            this.gbMRKCTLMST.TabIndex = 6;
            this.gbMRKCTLMST.TabStop = false;
            this.gbMRKCTLMST.Text = "MRKCTLMST";
            // 
            // listViewMRKCTLMST
            // 
            this.listViewMRKCTLMST.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.listViewMRKCTLMST.HideSelection = false;
            this.listViewMRKCTLMST.Location = new System.Drawing.Point(5, 18);
            this.listViewMRKCTLMST.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewMRKCTLMST.Name = "listViewMRKCTLMST";
            this.listViewMRKCTLMST.Size = new System.Drawing.Size(571, 107);
            this.listViewMRKCTLMST.TabIndex = 7;
            this.listViewMRKCTLMST.UseCompatibleStateImageBehavior = false;
            // 
            // gbPTRYOP
            // 
            this.gbPTRYOP.Controls.Add(this.listViewPTRYOP);
            this.gbPTRYOP.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbPTRYOP.Location = new System.Drawing.Point(594, 284);
            this.gbPTRYOP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRYOP.Name = "gbPTRYOP";
            this.gbPTRYOP.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbPTRYOP.Size = new System.Drawing.Size(582, 133);
            this.gbPTRYOP.TabIndex = 6;
            this.gbPTRYOP.TabStop = false;
            this.gbPTRYOP.Text = "PTRYOP";
            // 
            // listViewPTRYOP
            // 
            this.listViewPTRYOP.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.listViewPTRYOP.HideSelection = false;
            this.listViewPTRYOP.Location = new System.Drawing.Point(6, 18);
            this.listViewPTRYOP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewPTRYOP.Name = "listViewPTRYOP";
            this.listViewPTRYOP.Size = new System.Drawing.Size(571, 107);
            this.listViewPTRYOP.TabIndex = 7;
            this.listViewPTRYOP.UseCompatibleStateImageBehavior = false;
            // 
            // gbINSPDAT
            // 
            this.gbINSPDAT.Controls.Add(this.listViewINSPDAT);
            this.gbINSPDAT.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbINSPDAT.Location = new System.Drawing.Point(5, 422);
            this.gbINSPDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbINSPDAT.Name = "gbINSPDAT";
            this.gbINSPDAT.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbINSPDAT.Size = new System.Drawing.Size(1172, 135);
            this.gbINSPDAT.TabIndex = 6;
            this.gbINSPDAT.TabStop = false;
            this.gbINSPDAT.Text = "INSPDAT";
            // 
            // listViewINSPDAT
            // 
            this.listViewINSPDAT.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.listViewINSPDAT.HideSelection = false;
            this.listViewINSPDAT.Location = new System.Drawing.Point(6, 18);
            this.listViewINSPDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewINSPDAT.Name = "listViewINSPDAT";
            this.listViewINSPDAT.Size = new System.Drawing.Size(1159, 107);
            this.listViewINSPDAT.TabIndex = 7;
            this.listViewINSPDAT.UseCompatibleStateImageBehavior = false;
            // 
            // gbFAULTDAT
            // 
            this.gbFAULTDAT.Controls.Add(this.listViewFAULTDAT);
            this.gbFAULTDAT.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gbFAULTDAT.Location = new System.Drawing.Point(5, 561);
            this.gbFAULTDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbFAULTDAT.Name = "gbFAULTDAT";
            this.gbFAULTDAT.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gbFAULTDAT.Size = new System.Drawing.Size(1172, 221);
            this.gbFAULTDAT.TabIndex = 6;
            this.gbFAULTDAT.TabStop = false;
            this.gbFAULTDAT.Text = "FAULTDAT";
            // 
            // listViewFAULTDAT
            // 
            this.listViewFAULTDAT.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.listViewFAULTDAT.HideSelection = false;
            this.listViewFAULTDAT.Location = new System.Drawing.Point(6, 18);
            this.listViewFAULTDAT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listViewFAULTDAT.Name = "listViewFAULTDAT";
            this.listViewFAULTDAT.Size = new System.Drawing.Size(1159, 198);
            this.listViewFAULTDAT.TabIndex = 7;
            this.listViewFAULTDAT.UseCompatibleStateImageBehavior = false;
            // 
            // lblGRANK
            // 
            this.lblGRANK.AutoSize = true;
            this.lblGRANK.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblGRANK.Location = new System.Drawing.Point(856, 792);
            this.lblGRANK.Name = "lblGRANK";
            this.lblGRANK.Size = new System.Drawing.Size(53, 16);
            this.lblGRANK.TabIndex = 10;
            this.lblGRANK.Text = "GRANK";
            this.lblGRANK.Visible = false;
            // 
            // lblTotalFaultPage
            // 
            this.lblTotalFaultPage.AutoSize = true;
            this.lblTotalFaultPage.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTotalFaultPage.Location = new System.Drawing.Point(1072, 794);
            this.lblTotalFaultPage.Name = "lblTotalFaultPage";
            this.lblTotalFaultPage.Size = new System.Drawing.Size(46, 13);
            this.lblTotalFaultPage.TabIndex = 10;
            this.lblTotalFaultPage.Text = "/1000";
            // 
            // tbGRank
            // 
            this.tbGRank.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbGRank.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbGRank.Location = new System.Drawing.Point(915, 793);
            this.tbGRank.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbGRank.Name = "tbGRank";
            this.tbGRank.Size = new System.Drawing.Size(43, 15);
            this.tbGRank.TabIndex = 11;
            this.tbGRank.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbGRank.Visible = false;
            // 
            // tbFaultPage
            // 
            this.tbFaultPage.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbFaultPage.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbFaultPage.Location = new System.Drawing.Point(1026, 793);
            this.tbFaultPage.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbFaultPage.Name = "tbFaultPage";
            this.tbFaultPage.Size = new System.Drawing.Size(43, 15);
            this.tbFaultPage.TabIndex = 11;
            this.tbFaultPage.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbFaultPage.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbFaultPage_KeyDown);
            // 
            // btnNextFaultPage
            // 
            this.btnNextFaultPage.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnNextFaultPage.Location = new System.Drawing.Point(1122, 786);
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
            this.btnPrevFaultPage.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnPrevFaultPage.Location = new System.Drawing.Point(966, 786);
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
            this.lblDownloadResult.AutoSize = true;
            this.lblDownloadResult.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblDownloadResult.Location = new System.Drawing.Point(6, 788);
            this.lblDownloadResult.Name = "lblDownloadResult";
            this.lblDownloadResult.Size = new System.Drawing.Size(110, 12);
            this.lblDownloadResult.TabIndex = 13;
            this.lblDownloadResult.Text = "DownloadResult";
            // 
            // FormDB
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1180, 819);
            this.Controls.Add(this.lblDownloadResult);
            this.Controls.Add(this.btnPrevFaultPage);
            this.Controls.Add(this.btnNextFaultPage);
            this.Controls.Add(this.tbFaultPage);
            this.Controls.Add(this.tbGRank);
            this.Controls.Add(this.lblTotalFaultPage);
            this.Controls.Add(this.lblGRANK);
            this.Controls.Add(this.gbPTRYOP);
            this.Controls.Add(this.gbFAULTDAT);
            this.Controls.Add(this.gbINSPDAT);
            this.Controls.Add(this.gbMRKCTLMST);
            this.Controls.Add(this.gbPTRYLP);
            this.Controls.Add(this.gbBasicInfo);
            this.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormDB";
            this.Text = "DEFECT DATA VIEWER";
            this.Load += new System.EventHandler(this.FormDB_Load);
            this.VisibleChanged += new System.EventHandler(this.FormDB_VisibleChanged);
            this.gbSearchTime.ResumeLayout(false);
            this.gbSearchTime.PerformLayout();
            this.gbBasicInfo.ResumeLayout(false);
            this.gbBasicInfo.PerformLayout();
            this.gbDBConnection.ResumeLayout(false);
            this.gbDBConnection.PerformLayout();
            this.gbOption.ResumeLayout(false);
            this.gbOption.PerformLayout();
            this.gbMarkingGroup.ResumeLayout(false);
            this.gbMarkingGroup.PerformLayout();
            this.gbPTRYLP.ResumeLayout(false);
            this.gbMRKCTLMST.ResumeLayout(false);
            this.gbPTRYOP.ResumeLayout(false);
            this.gbINSPDAT.ResumeLayout(false);
            this.gbFAULTDAT.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblLotName;
        private System.Windows.Forms.TextBox tbLotName;
        private System.Windows.Forms.Button btnOpenCSV;
        private System.Windows.Forms.GroupBox gbSearchTime;
        private System.Windows.Forms.Label lblSearchEndTime;
        private System.Windows.Forms.Label lblSearchStartTime;
        private System.Windows.Forms.TextBox tbSearchEndTime;
        private System.Windows.Forms.TextBox tbSearchStartTime;
        private System.Windows.Forms.CheckBox cbSetSearchTime;
        private System.Windows.Forms.Button btnApplySearchTime;
        private System.Windows.Forms.Button btnOpenCSV2;
        private System.Windows.Forms.ComboBox cbDestination;
        private System.Windows.Forms.GroupBox gbBasicInfo;
        private System.Windows.Forms.GroupBox gbOption;
        private System.Windows.Forms.CheckBox cbUseMask;
        private System.Windows.Forms.Button btnSearchDB;
        private System.Windows.Forms.ListView listViewBCNO;
        private System.Windows.Forms.Label lblDestination;
        private System.Windows.Forms.GroupBox gbPTRYLP;
        private System.Windows.Forms.GroupBox gbMRKCTLMST;
        private System.Windows.Forms.GroupBox gbPTRYOP;
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
        private System.Windows.Forms.ListView listViewMRKCTLMST;
        private System.Windows.Forms.ListView listViewPTRYOP;
        private System.Windows.Forms.ListView listViewINSPDAT;
        private System.Windows.Forms.ListView listViewFAULTDAT;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.Button btnExportCSV;
        private System.Windows.Forms.Button btnXOFSMST;
        private System.Windows.Forms.Button btnDBConn;
        private System.Windows.Forms.GroupBox gbDBConnection;
        private System.Windows.Forms.Label lblDbConnState;
        private System.Windows.Forms.Label lblDbConnStateIcon;
        private System.Windows.Forms.GroupBox gbMarkingGroup;
        private System.Windows.Forms.CheckBox cbUseETC;
        private System.Windows.Forms.CheckBox cbUseTG;
        private System.Windows.Forms.CheckBox cbUseES;
        private System.Windows.Forms.Button btnShowSkipParam;
        private System.Windows.Forms.Button btnEditDefectClass;
    }
}