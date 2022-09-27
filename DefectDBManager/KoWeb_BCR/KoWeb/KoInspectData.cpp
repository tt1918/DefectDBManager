#include "stdafx.h"

TCHAR g_sInspectTypeName[MAX_INSPECT_CLASS][30]={  _T("Blob"),			
												  _T("Pattern Match"),
												  _T("Find Line"),
												  _T("Distance between two lines"),												 
												  _T("Spot"),				//@항목추가2 
												  _T("Compare"),
												  _T("OCR"),
												  _T("User Defined"),
												  _T("Stain"),
												  _T("Gap"),												  											  												  
													};

int           g_InspSet=0;						//현재 검사 Set
INSPECTDATA   g_Inspect[MAX_INSPECT_SET];		//4개
INSPECTSYSTEM g_InspectSys[MAX_INSPECT_SET];	//4개

INSPECTRESULT g_InspectResult[MAX_INSPECT];		//50개
PATTERNDATA	  g_Pattern[MAX_PATTERN];			//패턴매칭관련 사용하는 패턴관리 (최대 100개, 증가 가능함)

int SaveInspData(CString sModelFolder)
{
	int ret;
	int nSet, nId;
	CString sFileName;
	CFile f;
	
	sFileName.Format(_T("%s\\InspData.dat"), sModelFolder);
	ret=f.Open(sFileName, CFile::modeCreate|CFile::modeWrite);
	if(ret)
	{

		f.Write(g_Inspect, MAX_INSPECT_SET*sizeof(INSPECTDATA));
		f.Close();

		for(nSet=0;nSet<MAX_INSPECT_SET;nSet++)
		{
			for(nId=0;nId<MAX_INSPECT;nId++)
			{
				if(g_Inspect[nSet].m_InspSpot[nId].m_bUseMask)
				{
					sFileName.Format(_T("%s\\SpotMask%d_%d"), sModelFolder, nSet, nId);
					ret=f.Open(sFileName, CFile::modeCreate|CFile::modeWrite);
					if(ret)
					{
						f.Write(g_Inspect[nSet].m_InspSpot[nId].m_fmOrg, g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth*g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight);
						f.Write(g_Inspect[nSet].m_InspSpot[nId].m_fmMask, g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth*g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight);
						f.Close();
					}
				}
			}
		}
	}

	return ret;
}
int LoadInspData(CString sModelFolder)
{
	int ret;
	CString sFileName;
	int nSet, nId;
	CFile f;
	
	sFileName.Format(_T("%s\\InspData.dat"), sModelFolder);
	ret=f.Open(sFileName, CFile::modeRead);
	if(ret)
	{
		f.Read(g_Inspect, MAX_INSPECT_SET*sizeof(INSPECTDATA));
		f.Close();

		for(nSet=0;nSet<MAX_INSPECT_SET;nSet++)
		{
			for(nId=0;nId<MAX_INSPECT;nId++)
			{
				if(g_Inspect[nSet].m_InspSpot[nId].m_bUseMask)
				{
					sFileName.Format(_T("%s\\SpotMask%d_%d"), sModelFolder, nSet, nId);
					ret=f.Open(sFileName, CFile::modeRead);
					if(ret)
					{
						g_Inspect[nSet].m_InspSpot[nId].m_fmOrg =new BYTE[g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth*g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight];
						g_Inspect[nSet].m_InspSpot[nId].m_fmMask=new BYTE[g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth*g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight];

						f.Read(g_Inspect[nSet].m_InspSpot[nId].m_fmOrg, g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth*g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight);
						f.Read(g_Inspect[nSet].m_InspSpot[nId].m_fmMask, g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth*g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight);
						f.Close();
					}
					else
					{
						g_Inspect[nSet].m_InspSpot[nId].m_bUseMask=0;
					}
				}
			}
		}
	}

	return ret;
}

void CloseInspData()
{
	int nSet, nId;

	for(nSet=0;nSet<MAX_INSPECT_SET;nSet++)
		for(nId=0;nId<MAX_INSPECT;nId++)
			if(g_Inspect[nSet].m_InspSpot[nId].m_bUseMask)
			{				
				if(g_Inspect[nSet].m_InspSpot[nId].m_fmOrg)  delete g_Inspect[nSet].m_InspSpot[nId].m_fmOrg;
				if(g_Inspect[nSet].m_InspSpot[nId].m_fmMask) delete g_Inspect[nSet].m_InspSpot[nId].m_fmMask;				
			}
}

int LoadInspSystemData()
{
	int ret;
	CString sFileName;
	int nSet, nId;
	CFile f;
	
	sFileName.Format(_T("%sInspSystem.cfg"), MAIN_FOLDER);
	ret=f.Open(sFileName, CFile::modeRead);
	if(ret)
	{
		f.Read(g_InspectSys, MAX_INSPECT_SET*sizeof(INSPECTDATA));
		f.Close();
	}
	else
	{
		for(nSet=0;nSet<MAX_INSPECT_SET;nSet++)
		{
			g_InspectSys[nSet].m_dScaleFactorX=1;
			g_InspectSys[nSet].m_dScaleFactorY=1;
		}
	}
	return 0;
}

bool IsInspectResultNG(int nId)
{
	int i;
	bool NG=false;
	
	switch(g_InspectResult[nId].m_nInspectType)
	{
		case INSP_BLOB:		for(i=0;i<10;i++)
								if(g_InspectResult[nId].m_nBlobResult[i]>0) NG=true;
							break;

		case INSP_PATTERN:	for(i=0;i<2;i++)
								if(g_InspectResult[nId].m_nPatternResult[i]>0) NG=true;
							break;	

		case INSP_FINDLINE:	for(i=0;i<2;i++)
								if(g_InspectResult[nId].m_nFindLineResult[i]>0) NG=true;
							break;
		case INSP_GAP2LINE:	for(i=0;i<2;i++)
								if(g_InspectResult[nId].m_nMeasureResult[i]>0) NG=true;
							break;
		case INSP_SPOT:		for(i=0;i<7;i++)											//항목추가 12
								if(g_InspectResult[nId].m_nLevelCount[i]>0) NG=true;
							break;
		case INSP_COMPARE:	for(i=0;i<2;i++)
								if(g_InspectResult[nId].m_nCompareResult[i]>0) NG=true;
							break;
		case INSP_USER:		for(i=0;i<2;i++)											//항목추가 12
								if(g_InspectResult[nId].m_nUserResult[i]>0) NG=true;
							break;
		case INSP_GAP:		break;
		case INSP_STAIN:	break;
	}

	return NG;
}


void ClosePatternData()
{
	int i;

	for(i=0;i<MAX_PATTERN;i++)
	{
		if(g_Pattern[i].m_fm[0]) delete g_Pattern[i].m_fm[0];
		if(g_Pattern[i].m_fm[1]) delete g_Pattern[i].m_fm[1];
	}
}


int LoadModel(CString sModelName)
{
	int ok=-1;
	CString sFileName;

	//Align
	sFileName.Format(_T("%s%s\\Align.dat"), MODEL_PATH, sModelName);
	LoadAlignData(sFileName);

	//Pattern
	sFileName.Format(_T("%s%s\\Pattern.dat"), MODEL_PATH, sModelName);
	LoadPatternData(sFileName);

	//Teach
	sFileName.Format(_T("%s%s"), MODEL_PATH, sModelName);
	LoadInspData(sFileName);	

	ok=0;
done:
	return ok;
}


int SaveModel()
{
	int ok=-1;
	int ret;
	CString sFileName;
	CFile f;

	//Align
	sFileName.Format(_T("%s%s\\Align.dat"), MODEL_PATH, g_Temp.m_slotName);
	ret=SaveAlignData(sFileName);
	if(ret<1) goto done;

	//Pattern
	sFileName.Format(_T("%s%s\\Pattern.dat"), MODEL_PATH, g_Temp.m_slotName);
	ret=SavePatternData(sFileName);
	if(ret<1) goto done;

	//Teach
	sFileName.Format(_T("%s%s"), MODEL_PATH, g_Temp.m_slotName);
	SaveInspData(sFileName);


	if(ret>0) ok=0;
done:
	return ok;
}

int NewModel()
{
	int i, j, ok=0;
	int ret;
	CString sFileName;
	CFile f;

	//Align
	for(i=0;i<MAX_ALIGN_MODULE;i++)
		g_AlignData[i].m_nRegistered=0;

	//Pattern
	ClosePatternData();
	for(i=0;i<MAX_PATTERN;i++)
		g_Pattern[i].m_nAddCount=0;

	//Teach
	for(i=0;i<MAX_INSPECT_SET;i++)
	{
		g_Inspect[i].m_nInspectCount=0;
		for(j=0;j<MAX_INSPECT;j++)
			g_Inspect[i].m_nTeachDone[j]=0;
	}
	
	return ok;
}