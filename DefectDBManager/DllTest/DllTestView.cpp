
// DllTestView.cpp : implementation of the CDllTestView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DllTest.h"
#endif

#include "DllTestDoc.h"
#include "DllTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDllTestView

IMPLEMENT_DYNCREATE(CDllTestView, CFormView)

BEGIN_MESSAGE_MAP(CDllTestView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CDllTestView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDllTestView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDllTestView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDllTestView::OnBnClickedButton4)
END_MESSAGE_MAP()

// CDllTestView construction/destruction

CDllTestView::CDllTestView() noexcept
	: CFormView(IDD_DLLTEST_FORM)
{
	// TODO: add construction code here
	m_Defect = new CallClassWrapper;
#ifdef USE_CODEREADERDLL
	m_CodeReader = new CallClassWrapperCodeReader;
	if (!m_CodeReader->Initialize(false, 2048, 2048, _T("C:\\Users\\bonaf\\Downloads\\BCR\\DataMatrixTool.vpp")))
	{
		AfxMessageBox(_T("CodeReaderDll Initialize failed"));
	}
#endif
}

CDllTestView::~CDllTestView()
{
	if (m_Defect != nullptr)
	{
		delete m_Defect;
		m_Defect = nullptr;
	}
	
#ifdef USE_CODEREADERDLL
	if (m_CodeReader != nullptr)
	{
		delete m_CodeReader;
		m_CodeReader = nullptr;
	}
#endif
}

void CDllTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CDllTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDllTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CDllTestView printing

BOOL CDllTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDllTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDllTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDllTestView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}


// CDllTestView diagnostics

#ifdef _DEBUG
void CDllTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDllTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDllTestDoc* CDllTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDllTestDoc)));
	return (CDllTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CDllTestView message handlers


void CDllTestView::OnBnClickedButton1()
{
	m_Defect->GetDefectsData();
}


void CDllTestView::OnBnClickedButton2()
{
	m_Defect->ReadDBFile("C:\\Users\\admin\\Downloads\\20220621\\20220621DN23012\\Remote.db");
}


void CDllTestView::OnBnClickedButton3()
{
	m_Defect->SerachDefectClass(1000091);
}


void CDllTestView::OnBnClickedButton4()
{
#ifdef USE_CODEREADERDLL
	CString strFile = _T("All Files(*.*)|*.*|");
	CFileDialog dlg(TRUE, _T(" * .bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFile, this);
	if (dlg.DoModal() == IDOK)
	{
		CString strName = dlg.GetPathName();
		std::string szName = CT2CA(strName);
		cv::Mat mat = cv::imread(szName, cv::IMREAD_GRAYSCALE);

		double start, end;

		start = clock();
		// 		std::string code = call->CodeRead(mat, mat.cols, mat.rows);
		std::string szCode[2];
		std::thread t1(std::bind(&CDllTestView::CodeRead, this, mat, &szCode[0], false));
		std::thread t2(std::bind(&CDllTestView::CodeRead, this, mat, &szCode[1], true));

		t1.join();
		t2.join();

		end = clock();

		mat.release();
		CString str, str2(szCode[0].c_str()), str3(szCode[1].c_str());
		str.Format(_T("%.3f 1: %s 2 : %s"), (double)(end - start), str2, str3);
		AfxMessageBox(str);
	}
#endif
}

#ifdef USE_CODEREADERDLL
void CDllTestView::CodeRead(cv::Mat image, std::string* code, bool usePreprocess)
{
	std::string ret = m_CodeReader->CodeRead(image, image.cols, image.rows, usePreprocess);
	*code = ret;
}
#endif
