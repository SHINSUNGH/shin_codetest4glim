
// MFC_Assignment1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_Assignment1.h"
#include "MFC_Assignment1Dlg.h"
#include "afxdialogex.h"
#include <cstdlib>
#include <ctime>

#include <random>

// 워커 스레드 프로시저
static UINT WINAPI RandomMoveThread(LPVOID pParam)
{
	auto pDlg = reinterpret_cast<CMFCAssignment1Dlg*>(pParam);

	
	const int nWidth = 640;
	const int nHeight = 480;

	BOOL ok;
	int r = pDlg->GetDlgItemInt(IDC_EDIT_RADIUS, &ok, FALSE);
	if (!ok || r <= 0) r = 10;

	// 3) 랜덤 초기화 (rand() 사용)
	srand((unsigned)time(nullptr));
	for (int i = 0; i < 10; i++) {
		// 세 개의 랜덤 점 생성
		auto pts = new CPoint[3];
		for (int j = 0; j < 3; ++j) {
			int x = r + rand() % (nWidth - 2 * r);
			int y = r + rand() % (nHeight - 2 * r);
			pts[j] = CPoint(x, y);
		}
		// 좌표 전달
		pDlg->PostMessage(WM_MOVE_RANDOM, 0, (LPARAM)pts);
		::Sleep(500); //500ms 슬립
	}

	
	pDlg->PostMessage(WM_RANDOM_DONE, 0, 0);
	return 0;
}



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

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


// CMFCAssignment1Dlg 대화 상자



CMFCAssignment1Dlg::CMFCAssignment1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_ASSIGNMENT1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAssignment1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// 좌클릭 생성 원 반지름
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_nRadius);
	// 범위제한
	DDV_MinMaxInt(pDX, m_nRadius, 1, 500);

	// 3점 원 두께
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_nThickness);
}

BEGIN_MESSAGE_MAP(CMFCAssignment1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	//ON_BN_CLICKED(IDC_BUTTON2, &CMFCAssignment1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_RESET, &CMFCAssignment1Dlg::OnBnClickedBtnReset)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, &CMFCAssignment1Dlg::OnEnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDIT_THICKNESS, &CMFCAssignment1Dlg::OnEnChangeEditThickness)
	//ON_STN_CLICKED(IDC_STC_COORD, &CMFCAssignment1Dlg::OnStnClickedStcCoord)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTM_RANDOM, &CMFCAssignment1Dlg::OnBnClickedBtmRandom)
	ON_MESSAGE(WM_MOVE_RANDOM, &CMFCAssignment1Dlg::OnMoveRandom)
	ON_MESSAGE(WM_RANDOM_DONE, &CMFCAssignment1Dlg::OnRandomDone)
END_MESSAGE_MAP()


// CMFCAssignment1Dlg 메시지 처리기

BOOL CMFCAssignment1Dlg::OnInitDialog()
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

	// 이미지 초기화
	InitImage();
	//클릭횟수 0으로 초기화
	m_nClickCount = 0;
	// 바깥 원 두께 기본값 3
	SetDlgItemInt(IDC_EDIT_THICKNESS, 3);
	// 랜덤 버튼 3회 클릭 전까지 비활성화
	GetDlgItem(IDC_BTM_RANDOM)->EnableWindow(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCAssignment1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCAssignment1Dlg::OnPaint()
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
		CPaintDC dc(this);
		if (!m_image.IsNull())
			m_image.Draw(dc, 0, 0);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCAssignment1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCAssignment1Dlg::InitImage()
{

	// 초기화 안하면 크래시
	if (!m_image.IsNull())
		m_image.Destroy();

	const int nWidth = 640;
	const int nHeight = 480;
	const int nBpp = 8;

	m_image.Create(nWidth, -nHeight, nBpp);
	static RGBQUAD rgb[256];
	for (int i = 0; i < 256; i++)
		rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
	m_image.SetColorTable(0, 256, rgb);

	unsigned char* fm = (unsigned char*)m_image.GetBits();

	memset(fm, 0xFF, nWidth * nHeight);

	UpdateDisplay();
}




void CMFCAssignment1Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{

	//int radius = 20;
	CString sCoordinates;

	CDialogEx::OnLButtonDown(nFlags, point);

	++ m_nClickCount;
	
	if (m_nClickCount <= 3) {

	BOOL ok; // 엔터 쳐도 안꺼지도록
	int radius = GetDlgItemInt(IDC_EDIT_RADIUS, &ok, FALSE);
	if (!ok || radius <= 0) {
		AfxMessageBox(L"Radius must be over then 1");
		return;
	}

	sCoordinates.Format(
		L"Coordinates X = %d, Y = %d",
		point.x, 
		point.y);
	GetDlgItem(IDC_STC_COORD)->SetWindowText(sCoordinates);

	// 좌클릭 지점이 원의 중심이 되도록 -반지름
	DrawCircleAt(point.x - radius, point.y - radius, radius);

	m_pts[m_nClickCount - 1] = point;
	}
	
	if (m_nClickCount == 3) {

		// 3개 원 만들어지면 랜덤 활성화
		GetDlgItem(IDC_BTM_RANDOM)->EnableWindow(TRUE);

		Circle circ = ComputeCircle();

		// 바깥 원 두께 값
		BOOL okTh;
		m_nThickness = GetDlgItemInt(IDC_EDIT_THICKNESS, &okTh, FALSE);
		if (!okTh || m_nThickness <= 0) {
			AfxMessageBox(L"Thickness must be over then 1");
			return;
		}

		DrawOutCircleAt(circ.cx, circ.cy, circ.r, m_nThickness);

	}

	// 위 if 문에서 빠져나와야 정상캡처 가능
	for (int i = 0; i < 3; ++i) {
		int dx = point.x - m_pts[i].x;
		int dy = point.y - m_pts[i].y;
		if (dx * dx + dy * dy <= HIT_RADIUS * HIT_RADIUS) {
			m_bDragging = true;
			m_nDragStartX = i;
			SetCapture();  // 마우스 캡처
			break;
			
		}
	}
}


Circle CMFCAssignment1Dlg::ComputeCircle() const
{
	Circle c{ 0,0,0 };
	int x1 = m_pts[0].x;
	int y1 = m_pts[0].y;
	int x2 = m_pts[1].x;
	int y2 = m_pts[1].y;
	int x3 = m_pts[2].x;
	int y3 = m_pts[2].y;

	double D1 = -(double(x1) * x1 + double(y1) * y1);
	double D2 = -(double(x2) * x2 + double(y2) * y2);
	double D3 = -(double(x3) * x3 + double(y3) * y3);

	double dx21 = x2 - x1, dy21 = y2 - y1, dD21 = D2 - D1;
	double dx31 = x3 - x1, dy31 = y3 - y1, dD31 = D3 - D1;

	double denom = dx21 * dy31 - dx31 * dy21;
	if (fabs(denom) < 1e-6) {
		// 분모가 0이면 원 못그림
		AfxMessageBox(L"Can`t make Circle...");
		return c;
	}
	double A = (dD21 * dy31 - dD31 * dy21) / denom;
	double B = (dx21 * dD31 - dx31 * dD21) / denom;
	double C = D1 - A * x1 - B * y1;


	double cx = -A * 0.5;
	double cy = -B * 0.5;
	double r2 = cx * cx + cy * cy - C;
	if (r2 <= 0) {
		AfxMessageBox(L"Computed radius^2 <= 0");
		return c;
	}
	c.r = int(sqrt(r2) + 0.5);
	c.cx = int(cx + 0.5);
	c.cy = int(cy + 0.5);

	return c;
}

//int CMFCAssignment1Dlg::GetCircle() 
//{
//	int x1 = m_pts[0].x;
//	int y1 = m_pts[0].y;
//	int x2 = m_pts[1].x;
//	int y2 = m_pts[1].y;
//	int x3 = m_pts[2].x;
//	int y3 = m_pts[2].y;
//
//	double D1 = -(double(x1) * x1 + double(y1) * y1);
//	double D2 = -(double(x2) * x2 + double(y2) * y2);
//	double D3 = -(double(x3) * x3 + double(y3) * y3);
//
//	double dx21 = x2 - x1, dy21 = y2 - y1, dD21 = D2 - D1;
//	double dx31 = x3 - x1, dy31 = y3 - y1, dD31 = D3 - D1;
//
//	double denom = dx21 * dy31 - dx31 * dy21;
//	if (fabs(denom) < 1e-6) {
//		// 분모가 0이면 원 못그림
//		AfxMessageBox(L"Can`t make Circle...");
//		return;
//	}
//	double A = (dD21 * dy31 - dD31 * dy21) / denom;
//	double B = (dx21 * dD31 - dx31 * dD21) / denom;
//	double C = D1 - A * x1 - B * y1;
//
//
//	double cx = -A * 0.5;
//	double cy = -B * 0.5;
//	double r2 = cx * cx + cy * cy - C;
//	if (r2 <= 0) {
//		AfxMessageBox(L"Computed radius^2 <= 0");
//		return;
//	}
//	int ir = int(sqrt(r2) + 0.5);
//	int icx = int(cx + 0.5);
//	int icy = int(cy + 0.5);
//	
//	return icx, icy, ir;
//}


void CMFCAssignment1Dlg::DrawCircleAt(int x, int y, int nRadius)
{
	int nGray = 80;
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;

	int nPitch = m_image.GetPitch();
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// 범위 벗어나면 스킵
	for (int j = y; j < y + nRadius * 2; j++) {
		for (int i = x; i < x + nRadius * 2; i++) {
			if (i < 0 || j < 0 || i >= nWidth || j >= nHeight)
				continue;

			int dx = i - nCenterX;
			int dy = j - nCenterY;
			if (dx * dx + dy * dy <= nRadius * nRadius) {
				fm[j * nPitch + i] = nGray;
			}
		}
	}
	UpdateDisplay();
}


//void CMFCAssignment1Dlg::DrawOutCircleAt(int x, int y, int r, int thick)
//{
//	int nGray = 120;  // (0~255)
//	int nCenterX = x + r;
//	int nCenterY = y + r;
//
//	int nPitch = m_image.GetPitch();
//	int nWidth = m_image.GetWidth();
//	int nHeight = m_image.GetHeight();
//
//	unsigned char* fm = (unsigned char*)m_image.GetBits();
//
//	int outerR = r;
//	int innerR = max(0, r - thick);
//	int outerSq = outerR * outerR;
//	int innerSq = innerR * innerR;
//
//	for (int j = y; j < y + r * 2; ++j) {
//		for (int i = x; i < x + r * 2; ++i) {
//			if (i < 0 || j < 0 || i >= nWidth || j >= nHeight)
//				continue;
//
//			int dx = i - nCenterX;
//			int dy = j - nCenterY;
//			int d2 = dx * dx + dy * dy;
//
//			// 내부는 채우지 않고, 테두리 두께만큼의 영역만 그린다
//			if (d2 <= outerSq && d2 >= innerSq) {
//				fm[j * nPitch + i] = nGray;
//			}
//		}
//	}
//
//	UpdateDisplay();
//}

void CMFCAssignment1Dlg::DrawOutCircleAt(int cx, int cy, int r, int thick)
{
	int gray = 120;
	int outerR = r;
	int innerR = max(0, r - thick);
	int outerSq = outerR * outerR;
	int innerSq = innerR * innerR;

	int pitch = m_image.GetPitch();
	int W = m_image.GetWidth();
	int H = abs(m_image.GetHeight());
	BYTE* fm = (BYTE*)m_image.GetBits();

	// 중심(cx,cy) 기준으로 –r…+r 만큼 순회
	for (int dy = -outerR; dy <= outerR; ++dy) {
		int j = cy + dy;
		if (j < 0 || j >= H) continue;
		int dy2 = dy * dy;
		for (int dx = -outerR; dx <= outerR; ++dx) {
			int i = cx + dx;
			if (i < 0 || i >= W) continue;
			int d2 = dx * dx + dy2;
			// 내부 비우고 테두리만
			if (d2 >= innerSq && d2 <= outerSq) {
				fm[j * pitch + i] = gray;
			}
		}
	}

	UpdateDisplay();
}


void CMFCAssignment1Dlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}
void CMFCAssignment1Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCAssignment1Dlg::OnBnClickedBtnReset()
{
	//초기화
	//클릭 카운트 초기화
	m_nClickCount = 0;

	// 이미지 초기화
	InitImage();
	UpdateDisplay();

	// 좌표 초기화
	for (int i = 0; i < 3; ++i) {
		m_pts[i] = CPoint(0, 0);
	}
}

void CMFCAssignment1Dlg::OnEnChangeEditRadius()
{
	//UpdateData(TRUE);
}

void CMFCAssignment1Dlg::OnOK()
{
	// 엔터 눌러도 다이얼로그 안꺼지도록
}
void CMFCAssignment1Dlg::OnStnClickedStcX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCAssignment1Dlg::OnStnClickedStcCoord()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCAssignment1Dlg::OnEnChangeEditThickness()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCAssignment1Dlg::Dragging()
{
	// 화면 초기화
	InitImage();

	BOOL ok;
	int smallRad = GetDlgItemInt(IDC_EDIT_RADIUS, &ok, FALSE);
	if (!ok || smallRad <= 0) smallRad = 10;
	for (int i = 0; i < min(m_nClickCount, 3); ++i) {
		CPoint& p = m_pts[i];
		// DrawCircleAt은 "좌상단, 반지름" 방식을 썼으므로 보정
		DrawCircleAt(p.x - smallRad, p.y - smallRad, smallRad);
	}

	// 3) 세 점 모두 찍혔으면 외접원(정원) 다시 그리기
	if (m_nClickCount >= 3) {
		Circle circ = ComputeCircle();
		BOOL okTh;
		int thickness = GetDlgItemInt(IDC_EDIT_THICKNESS, &okTh, FALSE);
		if (!okTh || m_nThickness <= 0) {
			AfxMessageBox(L"Thickness must be over then 1");
			return;
		}

		// 중심(cx,cy) 기준
		DrawOutCircleAt(circ.cx, circ.cy, circ.r, thickness);
	}

	UpdateDisplay();
}

void CMFCAssignment1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags, point);

	if (m_bDragging) {
		ReleaseCapture();
		m_bDragging = false;
		m_nDragStartX = -1;
	}
}

// 마우스 이동 중에, 드래그 중이면 화면 재그리기
void CMFCAssignment1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);

	if (m_bDragging && m_nDragStartX >= 0 && m_nDragStartX < 3) {
		m_pts[m_nDragStartX] = point;
		Dragging();
	}
}
void CMFCAssignment1Dlg::OnBnClickedBtmRandom()
{
	GetDlgItem(IDC_BTM_RANDOM)->EnableWindow(FALSE);

	AfxBeginThread(RandomMoveThread, this);
}



LRESULT CMFCAssignment1Dlg::OnMoveRandom(WPARAM, LPARAM lParam)
{
	auto pts = reinterpret_cast<CPoint*>(lParam);

	// m_pts 갱신
	for (int i = 0; i < 3; ++i)
		m_pts[i] = pts[i];
	delete[] pts;

	// 2) 작은원 + 큰원 다시 그리기 
	Dragging();
	return 0;
}

LRESULT CMFCAssignment1Dlg::OnRandomDone(WPARAM, LPARAM)
{
	// 버튼 활성화
	GetDlgItem(IDC_BTM_RANDOM)->EnableWindow(TRUE);
	return 0;
}



