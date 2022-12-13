
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
	if (!m_CodeReader->Initialize(false, 2048, 2048, _T("C:\\COSS\\codeRead.vpp")))
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

	m_CsvReadingEvent = new CsvReadingEvents(this->m_hWnd);
	m_Defect->AddEndCsvReading(m_CsvReadingEvent);
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
	m_Defect->SearchDefectClass(1000091);
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
		
		std::list<cv::Rect> codePosition = FindBarcodePosition(mat);

		for (std::list<cv::Rect>::iterator iter = codePosition.begin();  iter != codePosition.end(); iter++)
			Read2DMatrix(mat(*iter));

		mat.release();
	}
#endif

//	m_Defect->ShowDefectView();
}

#ifdef USE_CODEREADERDLL
void CDllTestView::CodeRead(cv::Mat image, std::string* code, bool usePreprocess)
{
	std::string ret = m_CodeReader->CodeRead(image, image.cols, image.rows, usePreprocess);
	*code = ret;
}
#endif


void CDllTestView::Read2DMatrix(cv::Mat roiImage)
{
	cv::Mat image;
	if (roiImage.cols % 4 != 0)
	{

		int newCols = (int)(roiImage.cols / 4.0);
		newCols *= 4;
		cv::resize(roiImage, image, cv::Size(newCols, roiImage.rows));
	}
	else {
		image = roiImage.clone();
	}
	double start, end;

	start = clock();
	std::string szCode[2] = { "", "" };
	std::thread t1(std::bind(&CDllTestView::CodeRead, this, image, &szCode[0], false));
	std::thread t2(std::bind(&CDllTestView::CodeRead, this, image, &szCode[1], true));

	t1.join();
	t2.join();
	end = clock();

	/*CString str, str2(szCode[0].c_str()), str3(szCode[1].c_str());
	str.Format(_T("%.3f 1: %s 2 : %s"), (double)(end - start), str2, str3);
	AfxMessageBox(str);*/

	CRect rect = m_CodeReader->GetLastCodePosition();

	cv::Mat cImg;
	cv::cvtColor(image, cImg, cv::COLOR_GRAY2BGR);
	cv::rectangle(cImg, cv::Rect(rect.left, rect.top, rect.Width(), rect.Height()), cv::Scalar(255, 0, 0), 1, 8, 0);
	cv::putText(cImg, szCode[0].c_str(), cv::Point(rect.left, rect.top), 2, 0.5, cv::Scalar(255, 0, 0));
	cv::putText(cImg, szCode[1].c_str(), cv::Point(rect.left, rect.bottom + 10), 2, 0.5, cv::Scalar(255, 0, 0));

	cv::namedWindow("output", cv::WINDOW_NORMAL);
	cv::imshow("output", cImg);
	cv::waitKey(0);
	cImg.release();
}


std::list<cv::Rect> CDllTestView::FindBarcodePosition(cv::Mat image)
{
	cv::Mat bw, resizeImg, resizeBw;
	cv::resize(image, resizeImg, cv::Size(image.cols, image.rows / 2));
	cv::adaptiveThreshold(resizeImg, resizeBw, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, -10);
	cv::resize(resizeBw, bw, cv::Size(image.cols, image.rows));

	LPBYTE fmBinary = bw.data;
	double* sumValue = new double[bw.cols];
	ZeroMemory(sumValue, sizeof(sumValue) * bw.cols);

#pragma omp parallel for
	for (int i = 0; i < bw.cols; i++) 
	{
		for (int j = 0; j < bw.rows; j++) 
		{
			sumValue[i] += *(fmBinary + j * bw.cols + i);
		}
	}

	double maxValue = 0;
	int maxPosition = 0;

	for (int i = 0; i < bw.cols; i++)
	{
		if (sumValue[i] > maxValue)
		{
			maxValue = sumValue[i];
			maxPosition = i;
		}
	}

	delete[] sumValue;
	cv::Mat leftImg, rightImg;
	leftImg = image(cv::Rect(0, 0, maxPosition - 1, 256));
	rightImg = image(cv::Rect(maxPosition + 1, 0, image.cols - maxPosition - 1, 256));
	cv::Scalar meanL = cv::mean(leftImg);
	cv::Scalar meanR = cv::mean(rightImg);

	bool isBgLeft = false;
	int bcWidth = 500;
	cv::Mat cropedImg; // 에지영역에서 bcWidth 영역 만큼 잘라낸 이미지
	if (meanL[0] > meanR[0]) // 오른쪽에 필름
	{
		isBgLeft = true;
		if (maxPosition + bcWidth > image.cols)
			cropedImg = image(cv::Rect(maxPosition, 0, image.cols - maxPosition, image.rows));
		else
			cropedImg = image(cv::Rect(maxPosition, 0, bcWidth, image.rows));
	}
	else
	{
		if (maxPosition - bcWidth < 0)
			cropedImg = image(cv::Rect(0, 0, maxPosition, image.rows));
		else
		{
			cropedImg = image(cv::Rect(maxPosition - bcWidth, 0, bcWidth, image.rows));
		}
	}

	bw.release();
	resizeImg.release();
	resizeBw.release();
	cv::resize(cropedImg, resizeImg, cv::Size(cropedImg.cols / 2, cropedImg.rows / 2));
	cv::adaptiveThreshold(cropedImg, resizeBw, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, -5);
	cv::resize(resizeBw, bw, cv::Size(cropedImg.cols, cropedImg.rows));

	int offset = 256;
	cv::Mat vconcatImg;
	int loop = bw.rows / offset;
	for (int i = 0; i < loop; i++)
	{
		cv::Rect rt2;
		cv::Rect rt1(0, offset * i, bw.cols, offset);
		if (i != loop - 1)
		{
			rt2.x = 0;
			rt2.y = offset * (i + 1);
			rt2.width = bw.cols;
			rt2.height = offset;
		}
		else
		{
			rt2.x = 0;
			rt2.y = 0;
			rt2.width = bw.cols;
			rt2.height = offset;
		}
		cv::Mat source1 = bw(rt1);
		cv::Mat source2 = bw(rt2);
		cv::Mat diff;
		cv::subtract(source1, source2, diff);

		if (i == 0)
			vconcatImg = diff.clone();
		else
		{
			cv::Mat tempImg = vconcatImg.clone();
			cv::vconcat(tempImg, diff, vconcatImg);
		}
	}


	cv::Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
	cv::dilate(vconcatImg, vconcatImg, mask, cv::Point(-1, -1), 3);
	cv::erode(vconcatImg, vconcatImg, mask, cv::Point(-1, -1), 7);

	cv::Mat img_label, stats, centroid;
	int numOfLabels = cv::connectedComponentsWithStats(vconcatImg, img_label, stats, centroid, 8, CV_32S);
	int num = 1;
	std::list<cv::Rect> listBarcode;
	for (int i = 1; i < numOfLabels; i++)
	{
		int area = stats.at<int>(i, cv::CC_STAT_AREA);
		int left = stats.at<int>(i, cv::CC_STAT_LEFT);
		int top = stats.at<int>(i, cv::CC_STAT_TOP);
		int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
		int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);

		double areaRatio = (double)area / (double)(width * height);

		if (isBgLeft)
		{
			if (area > 1000 && width > 45 && areaRatio > 0.5) {
				//cv::rectangle(colorImg, cv::Point(left + maxPosition, top), cv::Point(left + maxPosition + width, top + height), cv::Scalar(0, 0, 255), 3);
				cv::Rect rect(cv::Point(left + maxPosition, top), cv::Point(left + maxPosition + width, top + height));
				cv::Rect validRect = SetBarcodeArea(rect, image.cols, image.rows);
				listBarcode.push_back(validRect);
			}
		}
		else
		{
			if (area > 1000 && width > 45 && areaRatio > 0.5) {
				if (maxPosition - bcWidth < 0)
				{
					//cv::rectangle(colorImg, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 0, 255), 3);
					cv::Rect rect(cv::Point(left, top), cv::Point(left + width, top + height));
					cv::Rect validRect = SetBarcodeArea(rect, image.cols, image.rows);
					listBarcode.push_back(validRect);
				}
				else
				{
					//cv::rectangle(colorImg, cv::Point(maxPosition - bcWidth + left, top), cv::Point(maxPosition - bcWidth + left + width, top + height), cv::Scalar(0, 0, 255), 3);
					cv::Rect rect(cv::Point(maxPosition - bcWidth + left, top), cv::Point(maxPosition - bcWidth + left + width, top + height));
					cv::Rect validRect = SetBarcodeArea(rect, image.cols, image.rows);
					listBarcode.push_back(validRect);
				}
			}
		}
	}

	return listBarcode;
}


cv::Rect CDllTestView::SetBarcodeArea(cv::Rect rect, int width, int height)
{
	cv::Rect modifyRect(rect);
	// X
	if (modifyRect.x > 100)
	{
		modifyRect.x -= 100;
		modifyRect.width += 100;
	}
	else
	{
		modifyRect.x = 0;
		modifyRect.width += modifyRect.x;
	}

	if (modifyRect.x + modifyRect.width < width - 100)
	{
		modifyRect.width += 100;
	}
	else
	{
		modifyRect.width = width - modifyRect.x - 1;
	}

	// Y
	if (modifyRect.y > 100)
	{
		modifyRect.y -= 100;
		modifyRect.height += 100;
	}
	else
	{
		modifyRect.y = 0;
		modifyRect.height += modifyRect.y;
	}

	if (modifyRect.y + modifyRect.height < height - 100)
	{
		modifyRect.height += 100;
	}
	else
	{
		modifyRect.height = height - modifyRect.y - 1;
	}
	return modifyRect;
}
