
// MFC_Assignment1Dlg.h: 헤더 파일
//

#pragma once

#define WM_MOVE_RANDOM  (WM_USER + 100)
#define WM_RANDOM_DONE  (WM_USER + 101)

struct Circle {int cx; int cy; int r;};

// CMFCAssignment1Dlg 대화 상자
class CMFCAssignment1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCAssignment1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	// 이미지 기본값
	CImage m_image;
	// 반지름 기본값
	int m_nRadius = 5;
	// 클릭 카운터
	int m_nClickCount = 0;
	// 정원 그리기용 좌표
	CPoint m_pts[3];
	// 두께값
	int m_nThickness = 1;
	// 원 그리기용 좌표
	Circle ComputeCircle() const;


	// 드래그 상태 추척
	bool m_bDragging = false;
	int m_nDragStartX = -1;
	static constexpr int HIT_RADIUS = 5;
	void Dragging();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_ASSIGNMENT1_DIALOG };
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
	void InitImage();
	// 좌클릭 이벤트
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 좌클릭 원 그리기
	void DrawCircleAt(int x, int y, int nRadius);

	// 화면 업데이트
	void UpdateDisplay();


	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnReset();

	// 반지름값 변경
	afx_msg void OnEnChangeEditRadius();
	virtual void OnOK() override;
	afx_msg void OnStnClickedStcX();
	afx_msg void OnStnClickedStcCoord();

	// 두께값 변경
	afx_msg void OnEnChangeEditThickness();

	// 드래그 할떄 필요함
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void DrawOutCircleAt(int x, int y, int r, int thick);
	int GetCircle();

	// 랜덤 구현
	afx_msg LRESULT OnMoveRandom(WPARAM, LPARAM);
	afx_msg LRESULT OnRandomDone(WPARAM, LPARAM);
	afx_msg void OnBnClickedBtmRandom();

public:
};


