
// BandCeresTestDlg.h: 헤더 파일
//

#pragma once
#include "OpenXLSX.hpp"
#include <string>
#include <vector>
#include "chartdir.h."

using namespace OpenXLSX;
// CBandCeresTestDlg 대화 상자
class CBandCeresTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CBandCeresTestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BANDCERESTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:

private:
	XLDocument doc;
	XLWorksheet wbs;
	CString clipboardText;
public:
	CStringA FileName;
	CListCtrl m_NGList;
	CListCtrl m_OKList;
	BOOL XLOpen = FALSE;
	std::vector<double> X_data;
	std::vector<double> Y_data;
	double params[5];
	double rmse;

	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedCopy();

	void XLData(CListCtrl& m_List);
	void RunGaussianWithBiasFitting();
	void ListCtrlInit();
	void XLDataLoading(std::string XLFileName,std::string sheetName);
	void ListView(CListCtrl& m_List,int nItem);
	void CopyHeader(int colCount,CListCtrl& listCtrl);
	void CopyListCtrlToClipboard(CListCtrl& listCtrl, const CString& title);
	void CopyListCtrlToClipboard();
	
	afx_msg void OnBnClickedView();
};



struct GaussianWithBiasResidual {

	GaussianWithBiasResidual(double x, double y) : x_(x), y_(y) {}

	template <typename T>

	bool operator()(const T* const params, T* residual) const {

		T A = params[0];

		T mu = params[1];

		T sigma = params[2];

		T b = params[3];

		T c = params[4];


		T gauss = -A * ceres::exp(-(T(x_) - mu) * (T(x_) - mu) / (T(2.0) * sigma * sigma));
		T linear = b * T(x_) + c;

		residual[0] = T(y_) - (gauss + linear);

		return true;

	}

private:

	const double x_, y_;
};

