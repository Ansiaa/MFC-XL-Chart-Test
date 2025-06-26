// ChartDirView.cpp: 구현 파일
//

#include "pch.h"
#include "BandCeresTest.h"
#include "BandCeresTestDlg.h"
#undef min
#undef max

#include <algorithm>
#include "chartdir.h"
#include "ChartDirView.h"

#include <numeric>   // for std::accumulate
#include <limits>    // for safety
#include <cmath>
#include "afxdialogex.h"


// ChartDirView 대화 상자
using namespace Chart;
using namespace std;
IMPLEMENT_DYNAMIC(ChartDirView, CDialogEx)

ChartDirView::ChartDirView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHART, pParent)
{
   
}

ChartDirView::~ChartDirView()
{
}

void ChartDirView::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHARTLIST, m_ChartList);
    DDX_Control(pDX, IDC_ChartViewer, m_ChartViewer);
    Init();
    DDX_Control(pDX, IDC_EDIT2, m_Subject);
}


BEGIN_MESSAGE_MAP(ChartDirView, CDialogEx)
    ON_LBN_SELCHANGE(IDC_CHARTLIST, &ChartDirView::OnSelchangeChartlist)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BOX, IDC_SCATTER, &ChartDirView::OnBnClickedRadio)
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// ChartDirView 메시지 처리기
void ChartDirView::Init()
{
    for (const CString& str : HeaderList)
    {
        m_ChartList.AddString(str);  
    }
}
void ChartDirView::SetListCtrls(CListCtrl* list1, CListCtrl* list2)
{
    m_NGList = list1;
    m_OKList = list2;
}
void ChartDirView::GetListCtrls(int col)
{
    int targetCol = col;
    
    X_data.clear();
    NG_data.clear();
    OK_data.clear();
    if (m_NGList && ::IsWindow(m_NGList->GetSafeHwnd()))
    {
        int count = m_NGList->GetItemCount();
        for (int i = 0; i < count; ++i)
        {
            CString value = m_NGList->GetItemText(i, targetCol);
            NG_data.push_back(_wtof(value));

        }
    }

    if (m_OKList && ::IsWindow(m_OKList->GetSafeHwnd()))
    {
        int count = m_OKList->GetItemCount();
        for (int i = 0; i < count; ++i)
        {
            CString value = m_OKList->GetItemText(i, targetCol);
            OK_data.push_back(_wtof(value));
        }
    }
}
afx_msg void ChartDirView::OnBnClickedRadio(UINT id)
{
    switch (id)
    {
    case IDC_BOX:
        m_radioMode = TRUE;
        m_radio = 0;
        break;
    case IDC_SCATTER:
        m_radioMode = TRUE;
        m_radio = 1;
        break;
    default:
        break;
    }
}
void ChartDirView::SelectListRow(void (ChartDirView::* func)())
{
    if (m_radioMode == TRUE)
    {
        if (selectedText == L"A")
        {
            GetListCtrls(0);
            (this->*func)();
        }
        else if (selectedText == L"mu")
        {
            GetListCtrls(1);
            (this->*func)();
        }
        else if (selectedText == L"sigma")
        {
            GetListCtrls(2);
            (this->*func)();
        }
        else if (selectedText == L"b")
        {
            GetListCtrls(3);
            (this->*func)();
        }
        else if (selectedText == L"offset")
        {
            GetListCtrls(4);
            (this->*func)();
        }
        else if (selectedText == L"RMSE")
        {
            GetListCtrls(5);
            (this->*func)();
        }
    }
    else {
        AfxMessageBox(L"Chart 유형 Button Click 필요");
    }
   
}
void ChartDirView::DrawChart()
{
    
    for (size_t i = 0; i < NG_data.size(); ++i)
        X_data.push_back(static_cast<double>(i + 1));

    // Chart 생성
    XYChart* c = new XYChart(800, 600);
    //c->setPlotArea(70, 40, 480, 300);
    c->setPlotArea(70, 40, 580, 400);
    
   /* // 선
    c->addLineLayer(DoubleArray(NG_data.data(), NG_data.size()), 0xff0000, "NG");
    c->addLineLayer(DoubleArray(OK_data.data(), OK_data.size()), 0x00cc00, "OK");*/
    
    // NG: 빨간 점
    c->addScatterLayer(DoubleArray(X_data.data(), X_data.size()),
        DoubleArray(NG_data.data(), NG_data.size()),
        "NG", Chart::Cross2Shape(), 9, 0xff0000);

    // OK: 초록 원
    c->addScatterLayer(DoubleArray(X_data.data(), X_data.size()),
        DoubleArray(OK_data.data(), OK_data.size()),
        "OK", Chart::CircleShape, 9, 0x00cc00);

    // 제목, 축, 범례
    c->xAxis()->setTitle("Index");
    c->yAxis()->setTitle("value");
    c->addLegend(10, 10)->setBackground(Chart::Transparent);

    // ChartViewer에 세팅
    m_ChartViewer.setChart(c);
}
// Y축 눈금 간격을 자동으로 계산하는 함수
double estimateStep(double minVal, double maxVal, int desiredSteps = 10)
{
    double range = maxVal - minVal;
    if (range <= 0) return 1.0;

    double roughStep = range / desiredSteps;
    double exponent = std::floor(std::log10(roughStep));
    double base = std::pow(10.0, exponent);
    double fraction = roughStep / base;

    double niceFraction;
    if (fraction < 1.5)
        niceFraction = 1;
    else if (fraction < 3)
        niceFraction = 2;
    else if (fraction < 7)
        niceFraction = 5;
    else
        niceFraction = 10;

    return niceFraction * base;
}
void  ChartDirView::BoxChart()
{
    // 1. 사분위수 계산 함수
    auto calcStats = [](const std::vector<double>& data, double& min, double& q1, double& med, double& q3, double& max)
    {
        std::vector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());
        int n = static_cast<int>(sorted.size());

        if (n == 0) return;

        min = sorted.front();
        max = sorted.back();
        q1 = sorted[n / 4];
        med = sorted[n / 2];
        q3 = sorted[(3 * n) / 4];
    };

    // 2. NG, OK 데이터에서 계산
    double ng_min = 0, ng_q1 = 0, ng_med = 0, ng_q3 = 0, ng_max = 0;
    double ok_min = 0, ok_q1 = 0, ok_med = 0, ok_q3 = 0, ok_max = 0;

    if (!NG_data.empty()) calcStats(NG_data, ng_min, ng_q1, ng_med, ng_q3, ng_max);
    if (!OK_data.empty()) calcStats(OK_data, ok_min, ok_q1, ok_med, ok_q3, ok_max);

    // 3. Chart 생성
    XYChart* c = new XYChart(800, 600);
    //c->setPlotArea(70, 40, 480, 300);
    c->setPlotArea(70, 40, 580, 400);
    // 4. 박스 플롯 계층 생성
    BoxWhiskerLayer* layer = c->addBoxWhiskerLayer(
        DoubleArray(new double[2]{ ng_min, ok_min }, 2),   // min
        DoubleArray(new double[2]{ ng_q1,  ok_q1 }, 2),   // Q1
        DoubleArray(new double[2]{ ng_med, ok_med }, 2),   // Median
        DoubleArray(new double[2]{ ng_q3,  ok_q3 }, 2),   // Q3
        DoubleArray(new double[2]{ ng_max, ok_max }, 2),   // max
        Chart::SameAsMainColor  // whisker color
    );

    // 5. 박스 색상 눈에 띄게 지정
    int colors[] = { 0xff6666, 0x66cc66 }; // NG: 빨강, OK: 초록
    //layer->setDataColor(Chart::SameAsMainColor);
    for (int i = 0; i < 2; ++i)
        layer->setBoxColor(i, colors[i]);  // 각 박스 색상 지정
    //layer->setLineWidth(2); // 윤곽선 두껍게
    
    // 6. 라벨 및 제목 설정
    const char* labels[] = { "NG", "OK" };
    c->xAxis()->setLabels(StringArray(labels, 2));
    c->xAxis()->setTitle("Group");
    c->yAxis()->setTitle("Value");
   // c->addLegend(50, 10)->setBackground(Chart::Transparent);

    // 7. 강조를 위한 y축 범위 수동 설정 (옵션)
    double globalMin = std::min(ng_min, ok_min);
    double globalMax = std::max(ng_max, ok_max);
    double range = globalMax - globalMin;
    double margin = std::max(0.5, range * 0.1);  // 범위의 10% 마진
    double autoStep = estimateStep(globalMin - margin, globalMax + margin);

    // 2. Y축 적용
    c->yAxis()->setLabelFormat("{value|2f}");
    c->yAxis()->setLinearScale(globalMin - margin, globalMax + margin, autoStep);
   // c->yAxis()->setLinearScale(globalMin - 5, globalMax + 5);  // 약간 margin 추가*/

    // 8. ChartViewer에 설정
    m_ChartViewer.setChart(c);

}


BOOL ChartDirView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    double zoomFactor = 0.1;  // 10%씩 줌

   // 현재 뷰포트 상태 읽기
    double left = m_ChartViewer.getViewPortLeft();
    double top = m_ChartViewer.getViewPortTop();
    double width = m_ChartViewer.getViewPortWidth();
    double height = m_ChartViewer.getViewPortHeight();

    // 중심 좌표 계산 (뷰포트 좌표 기준)
    double centerX = left + width / 2;
    double centerY = top + height / 2;

    if (zDelta > 0)
    {
        // 줌 인: 뷰포트 크기 줄임
        width *= (1 - zoomFactor);
        height *= (1 - zoomFactor);
    }
    else
    {
        // 줌 아웃: 뷰포트 크기 늘림
        width *= (1 + zoomFactor);
        height *= (1 + zoomFactor);
    }

    // 최소/최대 크기 제한
    if (width < 0.1) width = 0.1;
    if (height < 0.1) height = 0.1;
    if (width > 1) width = 1;
    if (height > 1) height = 1;

    // 뷰포트 위치 재조정 (중심 고정)
    left = centerX - width / 2;
    top = centerY - height / 2;

    // 뷰포트 경계 체크
    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (left + width > 1) left = 1 - width;
    if (top + height > 1) top = 1 - height;

    // 뷰포트 설정 및 갱신
    m_ChartViewer.setViewPortLeft(left);
    m_ChartViewer.setViewPortTop(top);
    m_ChartViewer.setViewPortWidth(width);
    m_ChartViewer.setViewPortHeight(height);

   m_ChartViewer.updateViewPort(true, false);  // 뷰포트 변경 후 차트 갱신
   return TRUE;

    // 확대/축소 후 차트 다시 그리기
   // BoxChart();

    //return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
void ChartDirView::OnSelchangeChartlist()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //m_Subject.Clear();
    int index = m_ChartList.GetCurSel();  // 선택된 항목의 인덱스
    if (index != LB_ERR)
    {
        
       
        m_ChartList.GetText(index, selectedText); 
        m_Subject.SetWindowText(selectedText);
        //m_Subject.GetWindowText(selectedText);
        // AfxMessageBox(index);
         // 예: 선택된 인덱스 사용
         // AfxMessageBox(CString ::Format(_T("선택한 인덱스: %d"), index));
         //SelectListRow();
        if (m_radio == 0)
        {   //m_Subject.SetWindowText(selectedText);
            SelectListRow(&ChartDirView::BoxChart);
        }
        if (m_radio == 1)
        {
            //m_Subject->GetWindowText(selectedText);
            SelectListRow(&ChartDirView::DrawChart);
        }

    }
}