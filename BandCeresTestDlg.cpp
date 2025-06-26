
// BandCeresTestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#pragma warning(push)
#pragma warning(disable : 4996)
#include <glog/logging.h>
#pragma warning(pop)
#include <ceres/ceres.h>


#include "BandCeresTest.h"
#include "BandCeresTestDlg.h"
#include "ChartDirView.h"

#include <atlstr.h>
#include <vector>
#include <math.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
using namespace OpenXLSX;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBandCeresTestDlg 대화 상자



CBandCeresTestDlg::CBandCeresTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BANDCERESTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBandCeresTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NGLIST2, m_NGList);
	DDX_Control(pDX, IDC_OKLIST, m_OKList);
}

BEGIN_MESSAGE_MAP(CBandCeresTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CBandCeresTestDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_COPY, &CBandCeresTestDlg::OnBnClickedCopy)
	ON_BN_CLICKED(IDC_VIEW, &CBandCeresTestDlg::OnBnClickedView)
END_MESSAGE_MAP()


// CBandCeresTestDlg 메시지 처리기

BOOL CBandCeresTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	ListCtrlInit();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBandCeresTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBandCeresTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBandCeresTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CBandCeresTestDlg::ListCtrlInit()
{
	m_NGList.InsertColumn(1,_T("A"), LVCFMT_CENTER, 150);
	m_NGList.InsertColumn(2, _T("mu"), LVCFMT_CENTER, 150);
	m_NGList.InsertColumn(3, _T("sigma"), LVCFMT_CENTER, 150);
	m_NGList.InsertColumn(4, _T("b"), LVCFMT_CENTER, 150);
	m_NGList.InsertColumn(5, _T("offset"), LVCFMT_CENTER, 150);
	m_NGList.InsertColumn(6, _T("RMSE"), LVCFMT_CENTER, 150);

	m_OKList.InsertColumn(1, _T("A"), LVCFMT_CENTER, 150);
	m_OKList.InsertColumn(2, _T("mu"), LVCFMT_CENTER, 150);
	m_OKList.InsertColumn(3, _T("sigma"), LVCFMT_CENTER, 150);
	m_OKList.InsertColumn(4, _T("b"), LVCFMT_CENTER, 150);
	m_OKList.InsertColumn(5, _T("offset"), LVCFMT_CENTER, 150);
	m_OKList.InsertColumn(6, _T("RMSE"), LVCFMT_CENTER, 150);
}
void CBandCeresTestDlg::ListView(CListCtrl& m_ListView,int nItem)
{
	for (int col = 0; col < 5; ++col) {
		CString str;
		str.Format(_T("%.5f"), params[col]);
		m_ListView.SetItemText(nItem, col, str);
	}
	CString str2;
	str2.Format(_T("%.5f"), rmse);
	m_ListView.SetItemText(nItem, 5, str2);
}


void CBandCeresTestDlg::OnBnClickedOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString filter = _T("xlsx(*.xlsx)|*.xlsx|All(*.*)|*.*||");
	UINT flag = OFN_FILEMUSTEXIST;
	CFileDialog dlg(TRUE, nullptr, nullptr, flag, filter);
	dlg.DoModal();
	FileName = dlg.GetPathName();
	std::string XLFileName;
	XLFileName = FileName.GetString();
	XLDataLoading(XLFileName,"NG");
	XLDataLoading(XLFileName, "OK");
	
}
void CBandCeresTestDlg::XLDataLoading(std::string XLFileName,std::string sheetName)
{
	
	try
	{
		doc.open(XLFileName);
	}
	catch (const std::exception& e)
	{
		AfxMessageBox(CString("엑셀 파일 열기에 실패했습니다:\n") + CString(e.what()));
		return;
	}
	
	try
	{
		auto wbk = doc.workbook();
		wbs = wbk.worksheet(sheetName);
		if (sheetName == "NG")
		{
			XLData(m_NGList);
		}
		else {
			XLData(m_OKList);
		}
		XLOpen = TRUE;
	}
	catch (const std::exception& e)
	{
		AfxMessageBox(CString("시트 열기에 실패했습니다:\n") + CString(e.what()));
		doc.close(); 
		return;
	}

	
}

void CBandCeresTestDlg::XLData(CListCtrl& m_ListView)
{
	int resultRow = 0;
	const int MAX_ROWS = 1000;
	const int MAX_COLS = 90; 

	for (int col = 1; col <= MAX_COLS; ++col)  
	{
		X_data.clear();
		Y_data.clear();

		for (int row = 1; row <= MAX_ROWS; ++row) 
		{
			XLCell cell = wbs.cell(XLCellReference(row, col));
			auto& val = cell.value();

			if (val.type() == XLValueType::Float)
			{
				Y_data.push_back(val.get<double>());
				X_data.push_back(static_cast<double>(row));
			}
			else if (val.type() == XLValueType::Integer)
			{
				Y_data.push_back(static_cast<double>(val.get<int64_t>()));
				X_data.push_back(static_cast<double>(row));
			}
		}

		if (Y_data.size() >= 5)
		{
			RunGaussianWithBiasFitting();

			int nItem = m_ListView.InsertItem(resultRow, _T("열 %d"), col); 
			ListView(m_ListView, nItem);
			resultRow++;
		}
	}

	doc.close();
}

void CBandCeresTestDlg::RunGaussianWithBiasFitting() {
	//double params[5] = { 10.0, 4.0, 1.0, 0.0, 1.0 };
	double x_min = *std::min_element(X_data.begin(), X_data.end());
	double x_max = *std::max_element(X_data.begin(), X_data.end());
	double y_min = *std::min_element(Y_data.begin(), Y_data.end());
	double y_max = *std::max_element(Y_data.begin(), Y_data.end());

	double x_avg = std::accumulate(X_data.begin(), X_data.end(), 0.0) / X_data.size();

	// [1] A: 진폭 (y의 range 정도로)
	params[0] = y_max - y_min;

	// [2] mu: 중앙값 또는 평균값
	params[1] = x_avg;

	// [3] sigma: x범위 / 6 정도
	params[2] = (x_max - x_min) / 6.0;
	if (params[2] < 1e-6) params[2] = 1e-6;  // 너무 작으면 기본값 설정
	//params[2] = fabs(params[2]);
	// [4] b: 직선 기울기 → 처음엔 0
	params[3] = 0.0;

	// [5] c: 절편 → y 평균 또는 최소값
	params[4] = y_min;

	
	// 문제 정의

	ceres::Problem problem;
	
	for (size_t i = 0; i < X_data.size(); ++i) {

		problem.AddResidualBlock(

			new ceres::NumericDiffCostFunction<GaussianWithBiasResidual, ceres::CENTRAL, 1, 5>(

				new GaussianWithBiasResidual(X_data[i], Y_data[i])
				
				),

			nullptr, params
		);
	}
	problem.SetParameterLowerBound(params, 2, 1e-6);

	ceres::Solver::Options options;

	options.linear_solver_type = ceres::DENSE_NORMAL_CHOLESKY;

	options.minimizer_progress_to_stdout = true;
	// 고정밀 수렴을 위한 핵심 옵션들:
	options.max_num_iterations = 200;             // 기본값 50 → 더 많은 반복
	options.function_tolerance = 1e-12;           // 기본값 1e-6 → 더 엄격하게 수렴 조건 설정
	options.gradient_tolerance = 1e-12;           // 기본값 1e-10 (충분히 작음)
	options.parameter_tolerance = 1e-12;          // 파라미터 변화 감지 기준
	options.minimizer_progress_to_stdout = false;

	// 일부 경우, 선형 솔버의 정밀도 향상도 유효
	options.num_threads = 4;                      // 멀티스레드 허용
	options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;

	ceres::Solver::Summary summary;

	ceres::Solve(options, &problem, &summary);

	double total_squared_error = 0.0;

	for (int i = 0; i < X_data.size(); i++) {

		double residual = 0.0;
		GaussianWithBiasResidual functor(X_data[i], Y_data[i]);
		bool ok = functor(params, &residual);
		if (!ok) {
			CString msg;
			msg.Format(_T("Residual evaluation failed at index %d"), i);
			AfxMessageBox(msg);
			return;
		}
		total_squared_error += residual * residual;

	}

	rmse = std::sqrt(total_squared_error / X_data.size());


}
void CBandCeresTestDlg::CopyHeader(int colCount,CListCtrl& listCtrl)
{
	// 1. 헤더 삽입
	for (int col = 0; col < colCount; ++col) {
		TCHAR headerText[256];
		HDITEM hdi = { 0 };
		hdi.mask = HDI_TEXT;
		hdi.pszText = headerText;
		hdi.cchTextMax = 255;
		listCtrl.GetHeaderCtrl()->GetItem(col, &hdi);
		clipboardText += headerText;
		if (col < colCount - 1) clipboardText += _T("\t");
	}
}
void CBandCeresTestDlg::CopyListCtrlToClipboard(CListCtrl& listCtrl, const CString& title)
{
	// 0. 타이틀 삽입
	clipboardText += title + _T("\r\n");

	int rowCount = listCtrl.GetItemCount();
	int colCount = listCtrl.GetHeaderCtrl()->GetItemCount();
	
	// 1. 헤더 삽입
	CopyHeader(colCount,listCtrl);
	clipboardText += _T("\r\n");

	// 2. 리스트 데이터 삽입
	for (int row = 0; row < rowCount; ++row) {
		for (int col = 0; col < colCount; ++col) {
			CString text = listCtrl.GetItemText(row, col);
			clipboardText += text;
			if (col < colCount - 1) clipboardText += _T("\t");
		}
		clipboardText += _T("\r\n");
	}

	clipboardText += _T("\r\n"); // 리스트 간 공백
	
}

void CBandCeresTestDlg::CopyListCtrlToClipboard()
{
	if (OpenClipboard()) {
		EmptyClipboard();

		HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (clipboardText.GetLength() + 1) * sizeof(TCHAR));
		if (hGlob) {
			memcpy(GlobalLock(hGlob), (LPCTSTR)clipboardText, (clipboardText.GetLength() + 1) * sizeof(TCHAR));
			GlobalUnlock(hGlob);
#ifdef _UNICODE
			SetClipboardData(CF_UNICODETEXT, hGlob);
#else
			SetClipboardData(CF_TEXT, hGlob);
#endif
		}
		CloseClipboard();
	}
}

void CBandCeresTestDlg::OnBnClickedCopy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CopyListCtrlToClipboard(m_NGList,L"NG 결과");
	CopyListCtrlToClipboard(m_OKList, L"OK 결과");
	CopyListCtrlToClipboard();
}


void CBandCeresTestDlg::OnBnClickedView()
{
	
	ChartDirView dlgChart;
	dlgChart.SetListCtrls(&m_NGList, &m_OKList);
	int colCount = m_NGList.GetHeaderCtrl()->GetItemCount();
	dlgChart.HeaderList.clear();
	for (int col = 0; col < colCount; ++col) {
		TCHAR headerText[256];
		HDITEM hdi = { 0 };
		hdi.mask = HDI_TEXT;
		hdi.pszText = headerText;
		hdi.cchTextMax = 255;
		m_NGList.GetHeaderCtrl()->GetItem(col, &hdi);
	
		dlgChart.HeaderList.push_back(headerText);
	}
	if (XLOpen == TRUE)
	{
		dlgChart.DoModal();
	}
	else
	{
		AfxMessageBox(L"DATA X");
	}
	
}
