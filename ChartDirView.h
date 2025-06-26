#pragma once
#include <vector>
#include <cmath>
// ChartDirView 대화 상자
#include "BandCeresTestDlg.h"
#include "ChartViewer.h"

class ChartDirView : public CDialogEx
{
	DECLARE_DYNAMIC(ChartDirView)

public:
	ChartDirView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ChartDirView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	CListCtrl* m_NGList = nullptr;
	CListCtrl* m_OKList = nullptr;
	DECLARE_MESSAGE_MAP()
public:
	int m_radio;
	BOOL m_radioMode = FALSE;
	CEdit m_Subject;
	CListBox m_ChartList;
	CString selectedText;
	CChartViewer m_ChartViewer;

	std::vector<CString> HeaderList;
	std::vector<CString> ListData;
	std::vector<double> NG_data;
	std::vector<double> OK_data;
	std::vector<double> X_data;
	void Init();
	void SetListCtrls(CListCtrl* list1, CListCtrl* list2);
	void GetListCtrls(int col);
	void SelectListRow(void(ChartDirView::* func)());
	void DrawChart();
	void BoxChart();

	afx_msg void OnSelchangeChartlist();
	afx_msg void OnBnClickedRadio(UINT id);
	//double estimateStep(double minVal, double maxVal, int desiredSteps = 10);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
};
