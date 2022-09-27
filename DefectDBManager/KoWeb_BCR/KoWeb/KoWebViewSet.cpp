#include "stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"




void CKoWebView::OpenBlobSet(int nSet, int nID)
{
	if(m_pBlobSet!=NULL)
	{
		if(m_pBlobSet->IsWindowVisible()) return;
		else
		{
			delete m_pBlobSet;
			m_pBlobSet=NULL;
		}
	}
	m_pBlobSet=new CBlobSet(this);
	m_pBlobSet->SetInspectID(nSet, nID);
	m_pBlobSet->Create(IDD_DIALOG_BLOB, this);
}

void CKoWebView::OpenPatternSet(int nSet, int nID)
{
	if(m_pPatternSet!=NULL)
	{
		if(m_pPatternSet->IsWindowVisible()) return;
		else
		{
			delete m_pPatternSet;
			m_pPatternSet=NULL;
		}
	}
	m_pPatternSet=new CPatternSet(this);
	m_pPatternSet->SetInspectID(nSet, nID);
	m_pPatternSet->Create(IDD_DIALOG_PATTERNSET, this);
}
void CKoWebView::OpenFindLineSet(int nSet, int nID)
{
	if(m_pFindLineSet!=NULL)
	{
		if(m_pFindLineSet->IsWindowVisible()) return;
		else
		{
			delete m_pFindLineSet;
			m_pFindLineSet=NULL;
		}
	}
	m_pFindLineSet=new CFindLineSet(this);
	m_pFindLineSet->SetInspectID(nSet, nID);
	m_pFindLineSet->Create(IDD_DIALOG_FINDLINE, this);
}
void CKoWebView::OpenMeasureSet(int nSet, int nID)
{
	if(m_pMeasureSet!=NULL)
	{
		if(m_pMeasureSet->IsWindowVisible()) return;
		else
		{
			delete m_pMeasureSet;
			m_pMeasureSet=NULL;
		}
	}
	m_pMeasureSet=new CMeasureSet(this);
	m_pMeasureSet->SetInspectID(nSet, nID);
	m_pMeasureSet->Create(IDD_DIALOG_MEASURE, this);
}

//@항목추가4  : 새로운항목을 티칭하기위한 다이얼로그 생성
void CKoWebView::OpenSpotSet(int nSet, int nID)
{
	if(m_pSpotSet!=NULL)
	{
		if(m_pSpotSet->IsWindowVisible()) return;
		else
		{
			delete m_pSpotSet;
			m_pSpotSet=NULL;
		}
	}
	m_pSpotSet=new CSpotSet(this);
	m_pSpotSet->SetInspectID(nSet, nID);			
	m_pSpotSet->Create(IDD_DIALOG_SPOT, this);
}

void CKoWebView::OpenOCRSet(int nSet, int nID)
{
	if(m_pOCRSet!=NULL)
	{
		if(m_pOCRSet->IsWindowVisible()) return;
		else
		{
			delete m_pOCRSet;
			m_pOCRSet=NULL;
		}
	}
	m_pOCRSet=new COCRSet(this);
	m_pOCRSet->SetInspectID(nSet, nID);			
	m_pOCRSet->Create(IDD_DIALOG_INSPECTOCR, this);
}
void CKoWebView::OpenCompareSet(int nSet, int nID)
{
	if(m_pCompareSet!=NULL)
	{
		if(m_pCompareSet->IsWindowVisible()) return;
		else
		{
			delete m_pCompareSet;
			m_pCompareSet=NULL;
		}
	}
	m_pCompareSet=new CCompareSet(this);
	m_pCompareSet->SetInspectID(nSet, nID);
	m_pCompareSet->Create(IDD_DIALOG_COMPARE, this);
}
void CKoWebView::OpenUserSet(int nSet, int nID)
{
	if(m_pUserSet!=NULL)
	{
		if(m_pUserSet->IsWindowVisible()) return;
		else
		{
			delete m_pUserSet;
			m_pUserSet=NULL;
		}
	}
	m_pUserSet=new CUserSet(this);
	m_pUserSet->SetInspectID(nSet, nID);
	m_pUserSet->Create(IDD_DIALOG_USER, this);
}