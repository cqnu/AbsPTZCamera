// YTControlDlg.h : 头文件
//

// CYTControlDlg 对话框
#include "afxwin.h"

#include "ButtonEx.h"
#include "FooButton.h"
#include "..\AbsDevice\PTZService.h"
#include "..\AbsDevice\VideoService.h"

//#include "stdint.h"
#include "SDL.h"




class CYTControlDlg : public CDialog
{
// 构造
public:
	CYTControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_YTCONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	CComboBox m_cboPresetNum;
	CSliderCtrl m_sliSpeed;
	CButtonEx m_btnLeft;
	CButtonEx m_btnRight;
	CButtonEx m_btnUp;
	CButtonEx m_btnDown;
	CButtonEx m_btnLeftUp;
	CButtonEx m_btnRightUp;
	CButtonEx m_btnLeftDown;
	CButtonEx m_btnRightDown;
	CButtonEx m_btnIrisOpen;
	CButtonEx m_btnIrisClose;
	CButtonEx m_btnFocusNear;
	CButtonEx m_btnFocusFar;
	CButtonEx m_btnZoomIn;
	CButtonEx m_btnZoomOut;
	FooButton m_btnWipe;
	FooButton m_btnLight;
	FooButton m_btn3D;

private:
	CString        m_strConfigPath;
	int            m_nPresetNum;
	int            m_nSpeed;
	ZWPTZParam       m_paramPTZ;
	PTZService    *m_PTZService;
	bool           m_bHasPtz;

	void UpdateUI(BOOL bFlag);

private:
	ZWVideoParam      m_paramVideo;
	VideoService   *m_pVideoService;
	bool            m_bHasVideo;
	bool            m_bEnable3D;/*是否开启了3D定位*/

	CRect           m_rcDisplayWnd;
	CRect           m_rcDisplay;
	HWND            m_hVideoWnd;
	float           m_fDisplayZoom;
	void            CalcDisplayRect(LPRECT lpWndRect,int width, int height,CRect& displayRect);

	int           m_nSdlInited;
	bool          m_bPlaying;
	//SDL_Surface   *surface;

	//SDL_Window    *win;
	//SDL_Renderer  *renderer;
	//SDL_Surface   *pic;
	//volatile long m_showing; //是否正在显示
	//HANDLE        m_hDisplayEvent;
	//HANDLE        m_hDisplayThread;
	static UINT __stdcall DisplayRGB24(LPVOID lpParam);
	void          ReceiveRGB24(void* buff, int width,int storewidth, int height);

	bool          m_bDriectTo;
	CPoint        m_lbDownPoint;
	CPoint        m_lbUpPoint;

	//计算出pt1和pt2相对于displayRect形成的矩形的坐标,再乘以系数cof
	void          CalcAimRect(CPoint& pt1,CPoint& pt2, CRect& displayRect,float cof,CRect& aimRect);

	static void __stdcall OnRealImgReadyProc(void *pUserData, ZWImageData* img);
	int           m_nFrameRateCount;
	int           m_nImgWidth;
	int           m_nImgHeight;
	int           m_nMaxFrameRate;
	int           m_naErrorNum[3];
	int           m_naErrorNumCount[3];

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void PostNcDestroy();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonOpenVideo();
	afx_msg void OnBnClickedButtonCloseVideo();

	afx_msg void OnBnClickedButton3d();

	afx_msg void OnBnLBDownRight();
	afx_msg void OnBnLBDownLeft();
	afx_msg void OnBnLBDownUp();
	afx_msg void OnBnLBDownDown();

	afx_msg void OnBnLBUpRight();
	afx_msg void OnBnLBUpLeft();
	afx_msg void OnBnLBUpUp();
	afx_msg void OnBnLBUpDown();

	afx_msg void OnBnLBDownZoomIn();
	afx_msg void OnBnLBDownZoomOut();

	afx_msg void OnBnLBUpZoomIn();
	afx_msg void OnBnLBUpZoomOut();

	afx_msg void OnBnLBDownFocusNear();
	afx_msg void OnBnLBDownFocusFar();

	afx_msg void OnBnLBUpFocusNear();
	afx_msg void OnBnLBUpFocusFar();

	afx_msg void OnBnLBDownIrisOpen();
	afx_msg void OnBnLBDownIrisClose();

	afx_msg void OnBnLBUpIrisOpen();
	afx_msg void OnBnLBUpIrisClose();

	afx_msg void OnBnClickedButtonWipe();
	afx_msg void OnBnClickedButtonLight();

	afx_msg void OnCbnSelchangeComboPresetnum();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonPreset();
	afx_msg void OnBnClickedButtonUsepreset();

	afx_msg void OnBnClickedButtonStepLeft();
	afx_msg void OnBnClickedButtonStepRight();
	afx_msg void OnBnClickedButtonStepDown();
	afx_msg void OnBnClickedButtonStepUp();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
