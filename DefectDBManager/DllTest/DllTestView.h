
// DllTestView.h : interface of the CDllTestView class
//

#pragma once
#include "CallClassWrapper.h"
#include "CsvReadingEvents.h"
#ifdef USE_CODEREADERDLL
#include "CallClassWrapperCodeReader.h"
#endif

class CDllTestView : public CFormView
{
protected: // create from serialization only
	CDllTestView() noexcept;
	DECLARE_DYNCREATE(CDllTestView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_DLLTEST_FORM };
#endif

// Attributes
public:
	CDllTestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDllTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



protected:
	CallClassWrapper* m_Defect;
	CsvReadingEvents* m_CsvReadingEvent;
#ifdef USE_CODEREADERDLL
	CallClassWrapperCodeReader* m_CodeReader;
#endif
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
#ifdef USE_CODEREADERDLL
	void CodeRead(cv::Mat image, std::string* code, bool usePreprocess);
#endif
};

#ifndef _DEBUG  // debug version in DllTestView.cpp
inline CDllTestDoc* CDllTestView::GetDocument() const
   { return reinterpret_cast<CDllTestDoc*>(m_pDocument); }
#endif

