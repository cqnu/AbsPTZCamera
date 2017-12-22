// YTControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YTControl.h"
#include "YTControlDlg.h"
#include <stdio.h>
#include "SDL_main.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CYTControlDlg::CYTControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYTControlDlg::IDD, pParent)
	, m_rcDisplay(0,0,0,0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(m_paramPTZ.DeviceIp,0,16);
	memset(m_paramPTZ.UserName, 0, 32);
	memset(m_paramPTZ.Password, 0, 32);
	memset(m_paramVideo.CameraIp,0,16);
	memset(m_paramVideo.LocalIp,0,16);
	memset(m_paramVideo.UserName, 0, 32);
	memset(m_paramVideo.Password, 0, 32);
	memset(m_paramVideo.Url,0,256);
	
	m_nPresetNum = 1;
	m_nSpeed = 1;
	m_nSdlInited = -1;
	m_bEnable3D = false;
	m_bPlaying = false;
	m_bDriectTo = false;

	m_fDisplayZoom = 1.0f;
	m_bHasPtz = false;
	m_bHasVideo = false;
	m_pVideoService = NULL;
	m_PTZService = NULL;
	//surface = NULL;

	//m_hDisplayEvent = NULL;
	//m_hDisplayThread = NULL;
	//m_showing = 0;
	//pic = NULL;
	//renderer = NULL;
	//win = NULL;
	m_nFrameRateCount = 0;
	m_nImgWidth = 0;
	m_nImgHeight = 0;
	m_nMaxFrameRate = 0;
	m_naErrorNum[0] = 0;
	m_naErrorNum[1] = 0;
	m_naErrorNum[2] = 0;
	m_naErrorNumCount[0] = 0;
	m_naErrorNumCount[1] = 0;
	m_naErrorNumCount[2] = 0;
}

void CYTControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX,IDC_COMBO_PRESETNUM,m_nPresetNum);
	DDX_Slider(pDX,IDC_SLIDER_SPPED,m_nSpeed);

	CString strPtzIP = m_paramPTZ.DeviceIp;
	DDX_Text(pDX,IDC_IPADDRESS_YT,strPtzIP);
	memset(m_paramPTZ.DeviceIp,0, ZW_LENGTH_IP_ADDR);
	memcpy(m_paramPTZ.DeviceIp,strPtzIP.GetBuffer(),strPtzIP.GetLength());
	strPtzIP.ReleaseBuffer();
	DDX_Text(pDX,IDC_EDIT_YTPORT,m_paramPTZ.DevicePort);
	int com = m_paramPTZ.Number-1;
	DDX_CBIndex(pDX,IDC_CBO_NUM,com);
	m_paramPTZ.Number = com+1;
	DDX_Text(pDX,IDC_EDIT_ADDRESS_NUM,m_paramPTZ.Address);
	DDX_Text(pDX,IDC_CBO_DATA,m_paramPTZ.ByteSize);
	int parity = m_paramPTZ.Parity;
	DDX_CBIndex(pDX,IDC_CBO_VALIDATE,parity);
	m_paramPTZ.Parity = parity;
	int stopbits = m_paramPTZ.StopBits;
	DDX_CBIndex(pDX,IDC_CBO_STOPBIT,stopbits);
	m_paramPTZ.StopBits = stopbits;

	DDX_Text(pDX,IDC_CBO_RATE,m_paramPTZ.BaudRate );

    CString strCameraIP = m_paramVideo.CameraIp;
	DDX_Text(pDX,IDC_IPADDRESS_CAMERA,strCameraIP);
	memset(m_paramVideo.CameraIp,0,16);
    memcpy(m_paramVideo.CameraIp,strCameraIP.GetBuffer(),strCameraIP.GetLength());
	strCameraIP.ReleaseBuffer();

	int port = m_paramVideo.CameraPort;
	DDX_Text(pDX,IDC_EDIT_CAMPORT, port);
	m_paramVideo.CameraPort = port;

	DDX_Text(pDX,IDC_EDIT_CHANNEL_NUM,m_paramVideo.AddressNum);

	CString strUrl = m_paramVideo.Url;
	memset(m_paramVideo.Url,0,256);;
	DDX_Text(pDX,IDC_VIDEO_URL,strUrl);
	strUrl.Trim();
	int len = strUrl.GetLength();
	len = len>255?255:len;
	memcpy(m_paramVideo.Url,strUrl.GetBuffer(),len);
	strUrl.ReleaseBuffer();

	//优先填充VideoUrl字段，如果要使用AdressName字段需将url留空
	if(strUrl.IsEmpty())
	{
		CString strAddressName = m_paramVideo.AddressName;
		memset(m_paramVideo.AddressName,0,256);
		DDX_Text(pDX,IDC_EDIT_CHANNEL_NAME,strAddressName);
		strAddressName.Trim();
		len = strAddressName.GetLength();
		len = len>255?255:len;
		memcpy(m_paramVideo.AddressName,strAddressName.GetBuffer(),len);
		strAddressName.ReleaseBuffer();
	}

	DDX_Control(pDX, IDC_COMBO_PRESETNUM, m_cboPresetNum);
	DDX_Control(pDX, IDC_SLIDER_SPPED,m_sliSpeed);
	DDX_Control(pDX, IDC_BUTTON_MOVE_LEFT, m_btnLeft);
	DDX_Control(pDX, IDC_BUTTON_MOVE_RIGHT, m_btnRight);
	DDX_Control(pDX, IDC_BUTTON_MOVE_UP, m_btnUp);
	DDX_Control(pDX, IDC_BUTTON_MOVE_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_IN, m_btnZoomIn);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_OUT, m_btnZoomOut);
	DDX_Control(pDX, IDC_BUTTON_LEFT_UP, m_btnLeftUp);
	DDX_Control(pDX, IDC_BUTTON_RIGHT_UP, m_btnRightUp);
    DDX_Control(pDX, IDC_BUTTON_RIGHT_DOWN, m_btnRightDown);
    DDX_Control(pDX, IDC_BUTTON_LEFT_DOWN, m_btnLeftDown);
	DDX_Control(pDX, IDC_BUTTON_IRIS_OPEN, m_btnIrisOpen);
    DDX_Control(pDX, IDC_BUTTON_IRIS_CLOSE, m_btnIrisClose);
	DDX_Control(pDX, IDC_BUTTON_FOCUS_NEAR, m_btnFocusNear);
    DDX_Control(pDX, IDC_BUTTON_FOCUS_FAR, m_btnFocusFar);
	DDX_Control(pDX, IDC_BUTTON_WIPE,m_btnWipe);
	DDX_Control(pDX, IDC_BUTTON_LIGHT, m_btnLight);
	DDX_Control(pDX, IDC_BUTTON_3D, m_btn3D);
}

BEGIN_MESSAGE_MAP(CYTControlDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CYTControlDlg::OnBnClickedButtonConnect)	
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CYTControlDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_VIDEO, &CYTControlDlg::OnBnClickedButtonOpenVideo)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_VIDEO, &CYTControlDlg::OnBnClickedButtonCloseVideo)
	ON_BN_CLICKED(IDC_BUTTON_3D, &CYTControlDlg::OnBnClickedButton3d)

	ON_BN_LBUTTONDOWN(IDC_BUTTON_MOVE_LEFT,&CYTControlDlg::OnBnLBDownLeft)
	ON_BN_LBUTTONDOWN(IDC_BUTTON_MOVE_RIGHT,&CYTControlDlg::OnBnLBDownRight)
	ON_BN_LBUTTONDOWN(IDC_BUTTON_MOVE_UP,&CYTControlDlg::OnBnLBDownUp)
	ON_BN_LBUTTONDOWN(IDC_BUTTON_MOVE_DOWN,&CYTControlDlg::OnBnLBDownDown)

	ON_BN_LBUTTONUP(IDC_BUTTON_MOVE_LEFT,&CYTControlDlg::OnBnLBUpLeft)
	ON_BN_LBUTTONUP(IDC_BUTTON_MOVE_RIGHT,&CYTControlDlg::OnBnLBUpRight)
	ON_BN_LBUTTONUP(IDC_BUTTON_MOVE_UP,&CYTControlDlg::OnBnLBUpUp)
	ON_BN_LBUTTONUP(IDC_BUTTON_MOVE_DOWN,&CYTControlDlg::OnBnLBUpDown)

	ON_BN_LBUTTONDOWN(IDC_BUTTON_IRIS_OPEN,&CYTControlDlg::OnBnLBDownIrisOpen)
	ON_BN_LBUTTONDOWN(IDC_BUTTON_IRIS_CLOSE,&CYTControlDlg::OnBnLBDownIrisClose)
	ON_BN_LBUTTONUP(IDC_BUTTON_IRIS_OPEN,&CYTControlDlg::OnBnLBUpIrisOpen)
	ON_BN_LBUTTONUP(IDC_BUTTON_IRIS_CLOSE,&CYTControlDlg::OnBnLBUpIrisClose)

	ON_BN_LBUTTONDOWN(IDC_BUTTON_ZOOM_IN,&CYTControlDlg::OnBnLBDownZoomIn)
	ON_BN_LBUTTONDOWN(IDC_BUTTON_ZOOM_OUT,&CYTControlDlg::OnBnLBDownZoomOut)
	ON_BN_LBUTTONUP(IDC_BUTTON_ZOOM_IN,&CYTControlDlg::OnBnLBUpZoomIn)
	ON_BN_LBUTTONUP(IDC_BUTTON_ZOOM_OUT,&CYTControlDlg::OnBnLBUpZoomOut)

	ON_BN_LBUTTONDOWN(IDC_BUTTON_FOCUS_NEAR,&CYTControlDlg::OnBnLBDownFocusNear)
	ON_BN_LBUTTONDOWN(IDC_BUTTON_FOCUS_FAR,&CYTControlDlg::OnBnLBDownFocusFar)
	ON_BN_LBUTTONUP(IDC_BUTTON_FOCUS_NEAR,&CYTControlDlg::OnBnLBUpFocusNear)
	ON_BN_LBUTTONUP(IDC_BUTTON_FOCUS_FAR,&CYTControlDlg::OnBnLBUpFocusFar)

	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_PRESETNUM, &CYTControlDlg::OnCbnSelchangeComboPresetnum)
	ON_BN_CLICKED(IDC_BUTTON_PRESET, &CYTControlDlg::OnBnClickedButtonPreset)
	ON_BN_CLICKED(IDC_BUTTON_USEPRESET, &CYTControlDlg::OnBnClickedButtonUsepreset)

	ON_BN_CLICKED(IDC_BUTTON_LIGHT, &CYTControlDlg::OnBnClickedButtonLight)
	ON_BN_CLICKED(IDC_BUTTON_WIPE, &CYTControlDlg::OnBnClickedButtonWipe)
	ON_BN_CLICKED(IDC_BUTTON_STEP_LEFT, &CYTControlDlg::OnBnClickedButtonStepLeft)
	ON_BN_CLICKED(IDC_BUTTON_STEP_RIGHT, &CYTControlDlg::OnBnClickedButtonStepRight)
	ON_BN_CLICKED(IDC_BUTTON_STEP_DOWN, &CYTControlDlg::OnBnClickedButtonStepDown)
	ON_BN_CLICKED(IDC_BUTTON_STEP_UP, &CYTControlDlg::OnBnClickedButtonStepUp)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CYTControlDlg 消息处理程序

BOOL CYTControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	TCHAR buffer[MAX_PATH];

	m_strConfigPath = ((CYTControlApp*)AfxGetApp())->m_strStartUpPath + _T("YTControl.ini");

	if(!::PathFileExists(m_strConfigPath))
	{
		CFile f;
		if(f.Open(m_strConfigPath,CFile::modeCreate | CFile::modeWrite))
			f.Close();
	}

	m_btnWipe.setType(FooButton::checkButton);
	m_btnLight.setType(FooButton::checkButton);
	m_btn3D.setType(FooButton::checkButton);

	memset(buffer,0,MAX_PATH);
	::GetPrivateProfileString(_T("YTControl"),_T("PtzDeviceIp"),_T(""),buffer,MAX_PATH,m_strConfigPath);
	memcpy(m_paramPTZ.DeviceIp,buffer,16);

	m_paramPTZ.DevicePort = ::GetPrivateProfileInt(_T("YTControl"),_T("PtzDevicePort"),100,m_strConfigPath);
	m_paramPTZ.Number = ::GetPrivateProfileInt(_T("YTControl"),_T("COM"),1,m_strConfigPath);
	m_paramPTZ.Address = ::GetPrivateProfileInt(_T("YTControl"),_T("Address"),1,m_strConfigPath);
	m_paramPTZ.BaudRate = ::GetPrivateProfileInt(_T("YTControl"),_T("BaudRate"),2400,m_strConfigPath);
	m_paramPTZ.Parity = ::GetPrivateProfileInt(_T("YTControl"),_T("Parity"),0,m_strConfigPath);
	m_paramPTZ.ByteSize = ::GetPrivateProfileInt(_T("YTControl"),_T("ByteSize"),8,m_strConfigPath);
	m_paramPTZ.StopBits = ::GetPrivateProfileInt(_T("YTControl"),_T("StopBits"),0,m_strConfigPath);

	memset(buffer, 0, MAX_PATH);
	::GetPrivateProfileString(_T("YTControl"), _T("UserName"), _T(""), buffer, MAX_PATH, m_strConfigPath);
	memcpy(m_paramPTZ.UserName, buffer, 32);
	memcpy(m_paramVideo.UserName, buffer, 32);

	memset(buffer, 0, MAX_PATH);
	::GetPrivateProfileString(_T("YTControl"), _T("Password"), _T(""), buffer, MAX_PATH, m_strConfigPath);
	memcpy(m_paramPTZ.Password, buffer, 32);
	memcpy(m_paramVideo.Password, buffer, 32);

	memset(buffer,0,MAX_PATH);
	::GetPrivateProfileString(_T("YTControl"),_T("CameraIP"),_T(""),buffer,MAX_PATH,m_strConfigPath);
	memcpy(m_paramVideo.CameraIp,buffer, ZW_LENGTH_IP_ADDR);

	memset(buffer, 0, MAX_PATH);
	::GetPrivateProfileString(_T("YTControl"), _T("LocalIP"), _T(""), buffer, MAX_PATH, m_strConfigPath);
	memcpy(m_paramVideo.LocalIp, buffer, ZW_LENGTH_IP_ADDR);

	m_paramVideo.CameraPort = ::GetPrivateProfileInt(_T("YTControl"),_T("CameraPort"),3939,m_strConfigPath);

	m_paramVideo.AddressNum = ::GetPrivateProfileInt(_T("YTControl"),_T("AddressNum"),0,m_strConfigPath);

	memset(buffer,0,MAX_PATH);
	::GetPrivateProfileString(_T("YTControl"),_T("VideoUrl"),_T(""),buffer,MAX_PATH,m_strConfigPath);
	memcpy(m_paramVideo.Url,buffer,255);

	memset(buffer,0,MAX_PATH);
	::GetPrivateProfileString(_T("YTControl"),_T("AddressName"),_T(""),buffer,MAX_PATH,m_strConfigPath);
	memcpy(m_paramVideo.AddressName,buffer,255);

	CString strPtzvConfigPath = ((CYTControlApp*)AfxGetApp())->m_strStartUpPath + _T("ptzv.ini");
	memset(buffer,0,MAX_PATH);
	::GetPrivateProfileString(_T("addin"),_T("VideoPlugin"),_T(""),buffer,MAX_PATH, strPtzvConfigPath);

	if(StrCmp(buffer,_T("")) != 0)
	{
		m_bHasVideo = true;
	}

	memset(buffer,0,MAX_PATH);
	::GetPrivateProfileString(_T("addin"),_T("PTZPlugin"),_T(""),buffer,MAX_PATH, strPtzvConfigPath);

	if(StrCmp(buffer,_T("")) == 0)
	{
		if(m_bHasVideo)
			::EnableWindow(GetDlgItem(IDC_BUTTON_OPEN_VIDEO     )->m_hWnd,TRUE);
		::EnableWindow(GetDlgItem(IDC_BUTTON_CONNECT     )->m_hWnd,FALSE);
	}
	else
	{
		m_bHasPtz = true;
	}

	m_hVideoWnd = GetDlgItem(IDC_VIDEO_WND)->m_hWnd;

	//char sdl_var[64];     
	//sprintf(sdl_var, "SDL_WINDOWID=%d", (int)m_hVideoWnd);//主窗口句柄   
	//SDL_putenv(sdl_var);  

	//m_nSdlInited = SDL_Init(SDL_INIT_VIDEO);

	//if(m_nSdlInited == 0)
	//{
	//	RECT  rcWnd;
	//	::GetWindowRect(m_hVideoWnd,&rcWnd);
	//	ScreenToClient(&rcWnd);
	//	m_rcDisplayWnd = rcWnd;
	//	//surface = SDL_SetVideoMode(m_rcDisplayWnd.Width(),m_rcDisplayWnd.Height(),24, SDL_NOFRAME);
	//	win = SDL_CreateWindowFrom(m_hVideoWnd);
	//	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	//}

	UpdateData(FALSE);
    m_sliSpeed.SetRange(1,63);
	m_nSpeed = 20;
	m_sliSpeed.SetPos(20);
	m_cboPresetNum.SetCurSel(0);
	
	SetTimer(0, 1000, NULL);

	//m_hDisplayEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYTControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(!m_bPlaying)
		{
			CPaintDC  vdc(GetDlgItem(IDC_VIDEO_WND));
			CBrush brush(RGB (0, 0, 0));
			RECT  rcWnd;
			rcWnd.left = 0;
			rcWnd.top = 0;
			rcWnd.right = m_rcDisplayWnd.Width();
			rcWnd.bottom = m_rcDisplayWnd.Height();
			vdc.FillRect(&rcWnd,&brush);
		}
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CYTControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CYTControlDlg::PostNcDestroy()
{
	CString temp;
	temp.Format(_T("%s"),m_paramPTZ.DeviceIp);
	::WritePrivateProfileString(_T("YTControl"),_T("PtzDeviceIp"),temp,m_strConfigPath);
	temp.Format(_T("%d"),m_paramPTZ.DevicePort);
	::WritePrivateProfileString(_T("YTControl"),_T("PtzDevicePort"),temp,m_strConfigPath);
	temp.Format(_T("%d"),m_paramPTZ.Number);
	::WritePrivateProfileString(_T("YTControl"),_T("COM"),temp,m_strConfigPath);
	temp.Format(_T("%d"),m_paramPTZ.Address);
	::WritePrivateProfileString(_T("YTControl"),_T("Address"),temp,m_strConfigPath);
	temp.Format(_T("%d"),m_paramPTZ.BaudRate);
	::WritePrivateProfileString(_T("YTControl"),_T("BaudRate"),temp,m_strConfigPath);
	temp.Format(_T("%d"),m_paramPTZ.Parity);
	::WritePrivateProfileString(_T("YTControl"),_T("Parity"),temp,m_strConfigPath);
	temp.Format(_T("%d"),m_paramPTZ.ByteSize);
	::WritePrivateProfileString(_T("YTControl"),_T("ByteSize"),temp,m_strConfigPath);
	temp.Format(_T("%d"),m_paramPTZ.StopBits);
	::WritePrivateProfileString(_T("YTControl"),_T("StopBits"),temp,m_strConfigPath);

	temp.Format(_T("%s"), m_paramVideo.CameraIp);
	::WritePrivateProfileString(_T("YTControl"),_T("CameraIP"),temp,m_strConfigPath);
	temp.Format(_T("%d"), m_paramVideo.CameraPort);
	::WritePrivateProfileString(_T("YTControl"),_T("CameraPort"),temp,m_strConfigPath);

	temp.Format(_T("%d"),m_paramVideo.AddressNum);
	::WritePrivateProfileString(_T("YTControl"),_T("AddressNum"),temp,m_strConfigPath);
	temp.Format(_T("%s"),m_paramVideo.AddressName);
	::WritePrivateProfileString(_T("YTControl"),_T("AddressName"),temp,m_strConfigPath);
	temp.Format(_T("%s"),m_paramVideo.Url);
	::WritePrivateProfileString(_T("YTControl"),_T("VideoUrl"),temp,m_strConfigPath);

	if(m_PTZService != NULL)
		delete m_PTZService;

	if(m_pVideoService != NULL)
	{
		m_pVideoService->StopVideoSource();
		delete m_pVideoService;
	}

	//if(surface != NULL)
	//	SDL_FreeSurface(surface);

	//if (renderer != NULL) {
	//	SDL_DestroyRenderer(renderer);
	//	if(win != NULL)
	//		SDL_DestroyWindow(win);
	//}

	//SDL_Quit();

	CDialog::PostNcDestroy();
}

void CYTControlDlg::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (UpdateData(TRUE))
	{
		try
		{
			m_PTZService = new PTZService(0,&m_paramPTZ,&m_paramVideo);
			UpdateUI(TRUE);
		}
		catch(int & msg)
		{
			switch(msg)
			{
				/*未定义错误*/
			case ZW_ERR_NOT_DEFINED:
				AfxMessageBox(_T("未定义错误！"));
				break;
				/*插件路径为空*/
			case ZW_ERR_PATH_IS_EMPTY:
				AfxMessageBox(_T("插件路径为空！"));
				break;
				/*插件不存在*/
			case ZW_ERR_PATH_NOT_EXIST:
				AfxMessageBox(_T("插件不存在！"));
				break;
				/*插件DLL加载失败*/
			case ZW_ERR_LOAD_DLL_FAIL:
				AfxMessageBox(_T("插件DLL加载失败！"));
				break;
				/*不是有效的PTZ插件*/
			case ZW_ERR_PTZ_ADDIN_FAULT:
				AfxMessageBox(_T("不是有效的PTZ插件！"));
				break;
				/*不是有效的VIDEO插件*/
			case ZW_ERR_VIDEO_ADDIN_FAULT:
				AfxMessageBox(_T("不是有效的VIDEO插件！"));
				break;
				/*PTZ设备1错误*/
			case ZW_ERR_PTZ1_FAULT:
				AfxMessageBox(_T("PTZ设备1错误！"));
				break;
				/*PTZ设备2错误*/
			case ZW_ERR_PTZ2_FAULT:
				AfxMessageBox(_T("PTZ设备2错误！"));
				break;
				/*VIDEO设备错误*/
			case ZW_ERR_VIDEO_FAULT:
				AfxMessageBox(_T("VIDEO设备错误！"));
				break;
			default:
				AfxMessageBox(_T("遇到未知的错误！"));
				break;
			}
		}
		catch(...)
		{
			AfxMessageBox(_T("遇到未处理的异常！"));
		}
	}
}

void CYTControlDlg::OnBnClickedButtonDisconnect()
{
	// TODO: Add your control notification handler code here
	delete m_PTZService;
	m_PTZService = NULL;
	UpdateUI(FALSE);
	m_btn3D.check(false);
	m_bEnable3D = false;
	m_bPlaying = false;
	m_bDriectTo = false;
	::EnableWindow(GetDlgItem(IDC_BUTTON_3D         )->m_hWnd,FALSE);
}

void CYTControlDlg::OnBnClickedButtonOpenVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (UpdateData(TRUE))
	{
		try
		{
		    m_pVideoService = new VideoService(0,&m_paramVideo,NULL,OnRealImgReadyProc,NULL,ZW_PIX_FMT_NONE,NULL,NULL, ZW_PIX_FMT_NONE,(void*)this);
			m_pVideoService->StartVideoSource((handle)m_hVideoWnd);
			if(m_PTZService != NULL && m_PTZService->CanDirectTo())
				::EnableWindow(GetDlgItem(IDC_BUTTON_3D         )->m_hWnd,TRUE);
			::EnableWindow(GetDlgItem(IDC_BUTTON_OPEN_VIDEO     )->m_hWnd,FALSE);
			::EnableWindow(GetDlgItem(IDC_BUTTON_DISCONNECT     )->m_hWnd,FALSE);
			::EnableWindow(GetDlgItem(IDC_BUTTON_CLOSE_VIDEO    )->m_hWnd,TRUE);			
			m_bPlaying = true;
			//m_hDisplayThread = (HANDLE)_beginthreadex(NULL, 0, CYTControlDlg::DisplayRGB24,
			//	(LPVOID)this, 0, NULL);
		}
		catch(int & msg)
		{
			switch(msg)
			{
				/*未定义错误*/
			case ZW_ERR_NOT_DEFINED:
				AfxMessageBox(_T("未定义错误！"));
				break;
				/*插件路径为空*/
			case ZW_ERR_PATH_IS_EMPTY:
				AfxMessageBox(_T("插件路径为空！"));
				break;
				/*插件不存在*/
			case ZW_ERR_PATH_NOT_EXIST:
				AfxMessageBox(_T("插件不存在！"));
				break;
				/*插件DLL加载失败*/
			case ZW_ERR_LOAD_DLL_FAIL:
				AfxMessageBox(_T("插件DLL加载失败！"));
				break;
				/*不是有效的PTZ插件*/
			case ZW_ERR_PTZ_ADDIN_FAULT:
				AfxMessageBox(_T("不是有效的PTZ插件！"));
				break;
				/*不是有效的VIDEO插件*/
			case ZW_ERR_VIDEO_ADDIN_FAULT:
				AfxMessageBox(_T("不是有效的VIDEO插件！"));
				break;
				/*PTZ设备1错误*/
			case ZW_ERR_PTZ1_FAULT:
				AfxMessageBox(_T("PTZ设备1错误！"));
				break;
				/*PTZ设备2错误*/
			case ZW_ERR_PTZ2_FAULT:
				AfxMessageBox(_T("PTZ设备2错误！"));
				break;
				/*VIDEO设备错误*/
			case ZW_ERR_VIDEO_FAULT:
				AfxMessageBox(_T("VIDEO设备错误！"));
				break;
			default:
				AfxMessageBox(_T("遇到未知的错误！"));
				break;
			}
		}
		catch(...)
		{
			AfxMessageBox(_T("遇到未处理的异常！"));
		}	
	}
}

void CYTControlDlg::OnBnClickedButtonCloseVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pVideoService->StopVideoSource();
	delete m_pVideoService;
	m_pVideoService = NULL;
	m_btn3D.check(false);
	m_bEnable3D = false;
	m_bPlaying = false;
	//SetEvent(m_hDisplayEvent);
	//WaitForSingleObject(m_hDisplayThread, INFINITE);
	//CloseHandle(m_hDisplayThread);
	m_bDriectTo = false;
	::EnableWindow(GetDlgItem(IDC_BUTTON_3D             )->m_hWnd,FALSE);
	::EnableWindow(GetDlgItem(IDC_BUTTON_OPEN_VIDEO     )->m_hWnd,TRUE);
	if(m_PTZService != NULL )
		::EnableWindow(GetDlgItem(IDC_BUTTON_DISCONNECT     )->m_hWnd,TRUE);
	::EnableWindow(GetDlgItem(IDC_BUTTON_CLOSE_VIDEO    )->m_hWnd,FALSE);

	m_nFrameRateCount = 0;
	m_nMaxFrameRate = 0;
	m_naErrorNum[0] = 0;
	m_naErrorNum[1] = 0;
	m_naErrorNum[2] = 0;
	m_naErrorNumCount[0] = 0;
	m_naErrorNumCount[1] = 0;
	m_naErrorNumCount[2] = 0;
}

void  CYTControlDlg::CalcDisplayRect(LPRECT lpWndRect,int width, int height,CRect& displayRect)
{
	CRect rectWnd(lpWndRect);

	float a = (float)width/(float)height;
	float b = (float)rectWnd.Width()/(float)rectWnd.Height();

	if (a > b)
	{
		if (width > rectWnd.Width())
		{
			displayRect.left = rectWnd.left;
			displayRect.right = rectWnd.right;
			int c = (height*displayRect.Width())/width;
			displayRect.top = rectWnd.top + (rectWnd.Height()-c)/2;
			displayRect.bottom = displayRect.top+c;
		}
		else
		{
			displayRect.left =rectWnd.left + (rectWnd.Width()-width)/2;
			displayRect.right = displayRect.left + width;
			displayRect.top = rectWnd.top + (rectWnd.Height()-height)/2;
			displayRect.bottom = displayRect.top + height;
		}
	}
	else
	{
		if (height > rectWnd.Height())
		{
			displayRect.top = rectWnd.top;
			displayRect.bottom = displayRect.top+rectWnd.Height();
			int c = (width*rectWnd.Height())/height;
			displayRect.left = rectWnd.left + (rectWnd.Width()-c)/2;
			displayRect.right = displayRect.left+c;
		}
		else
		{
			displayRect.top = rectWnd.top+(rectWnd.Height()-height)/2;
			displayRect.bottom = displayRect.top + height;
			displayRect.left = rectWnd.left + (rectWnd.Width()-width)/2;
			displayRect.right = displayRect.left + width;
		}
	}
}

UINT CYTControlDlg::DisplayRGB24(LPVOID lpParam)
{
	//CYTControlDlg* pDlg = (CYTControlDlg*)lpParam;
	//while (pDlg->m_bPlaying)
	//{
	//	if (pDlg->m_showing == 1)
	//	{
	//		if (pDlg->m_bDriectTo)
	//		{
	//			Uint32 c_red = SDL_MapRGB(pDlg->pic->format, 255, 0, 0);
	//			CRect aimRect;

	//			pDlg->CalcAimRect(pDlg->m_lbDownPoint, pDlg->m_lbUpPoint, pDlg->m_rcDisplay, pDlg->m_fDisplayZoom, aimRect);
	//			SDL_Rect sdlAimRect;
	//			sdlAimRect.x = aimRect.left;
	//			sdlAimRect.y = aimRect.top;
	//			sdlAimRect.w = aimRect.Width();
	//			sdlAimRect.h = aimRect.Height();
	//			SDL_FillRect(pDlg->pic,
	//				&sdlAimRect,
	//				c_red);
	//		}

	//		SDL_Texture   *tex = SDL_CreateTextureFromSurface(pDlg->renderer, pDlg->pic);

	//		SDL_Rect dst;
	//		dst.x = pDlg->m_rcDisplay.left;
	//		dst.y = pDlg->m_rcDisplay.top;
	//		dst.w = pDlg->m_rcDisplay.Width();
	//		dst.h = pDlg->m_rcDisplay.Height();

	//		SDL_RenderClear(pDlg->renderer);
	//		SDL_RenderCopy(pDlg->renderer, tex, NULL, &dst);
	//		SDL_RenderPresent(pDlg->renderer);

	//		//SDL_SoftStretch(pic,NULL,surface,&dst);
	//		//SDL_Flip(surface);

	//		SDL_DestroyTexture(tex);

	//		InterlockedDecrement(&(pDlg->m_showing));
	//	}
	//	else
	//	{
	//		WaitForSingleObject(pDlg->m_hDisplayEvent, INFINITE);
	//	}
	//}

	return 0;
}

void  CYTControlDlg::ReceiveRGB24(void* buff, int width,int storewidth, int height)
{
	//if(m_rcDisplay.Width() == 0)
	//{
	//	RECT  rcWnd;
	//	rcWnd.left = 0;
	//	rcWnd.top = 0;
	//	rcWnd.right = m_rcDisplayWnd.Width();
	//	rcWnd.bottom = m_rcDisplayWnd.Height();

	//	m_nImgWidth = width;
	//	m_nImgHeight = height;

	//	CalcDisplayRect(&rcWnd,width,height,m_rcDisplay);
	//	m_fDisplayZoom = (float)width/ (float)(m_rcDisplay.Width());
	//}

	//if (m_nSdlInited == 0)
	//{
	//	if (m_showing == 0) 
	//	{
	//		if(pic != NULL)
	//			SDL_FreeSurface(pic);
	//		pic = SDL_CreateRGBSurfaceFrom(buff, width, height, 24, storewidth, 0x00ff0000, 0x0000ff00, 0x000000ff, 0);
	//		InterlockedIncrement(&m_showing);

	//		SetEvent(m_hDisplayEvent);
	//	}
	//}
	//else
	//{
	//	//BITMAPINFO bmpinfo;
	//	//::ZeroMemory(&bmpinfo, sizeof(BITMAPINFO));
	//	//bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	//	//bmpinfo.bmiHeader.biWidth = width;
	//	//bmpinfo.bmiHeader.biHeight = height;
	//	//bmpinfo.bmiHeader.biCompression = BI_RGB;
	//	//bmpinfo.bmiHeader.biPlanes = 1;
	//	//bmpinfo.bmiHeader.biBitCount = 24;

	//	//CWnd* pWin = GetDlgItem(IDC_VIDEO_WND);
	//	//CDC * pDc = pWin->GetDC();
	//	//::SetStretchBltMode(pDc->m_hDC, COLORONCOLOR);
	//	//::StretchDIBits(pDc->m_hDC, m_rcDisplay.left, m_rcDisplay.top, m_rcDisplay.Width(), m_rcDisplay.Height(),
	//	//	0, 0, width, height,
	//	//	buff, (LPBITMAPINFO)(&bmpinfo), DIB_RGB_COLORS, SRCCOPY);
	//	//pDc->DeleteDC();
	//}


}

void  CYTControlDlg::OnRealImgReadyProc(void *pUserData, ZWImageData* img)
{
	//LARGE_INTEGER PerfFreq={0};
	////获取每秒多少CPU Performance Tick 
	//QueryPerformanceFrequency(&PerfFreq); 
	//LARGE_INTEGER TimerTick;
	//VERIFY(QueryPerformanceCounter(&TimerTick));

	CYTControlDlg* pvs = (CYTControlDlg*)pUserData;
	pvs->m_nFrameRateCount++;

	pvs->m_nImgWidth = img->Width;
	pvs->m_nImgHeight = img->Height;

	//LARGE_INTEGER TimerTick2;
	//VERIFY(QueryPerformanceCounter(&TimerTick2));

	////计算时间是花费多少秒。
	//float fInterval = TimerTick2.QuadPart - TimerTick.QuadPart;
	//CString costStr;
	//costStr.Format(_T("花费:%fTicks,%f秒\n"),fInterval,fInterval/PerfFreq.QuadPart);
	//OutputDebugString(costStr);
}

void CYTControlDlg::OnBnClickedButton3d()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnable3D = m_btn3D.isChecked();
}


void CYTControlDlg::UpdateUI( BOOL bFlag )
{
	::EnableWindow(GetDlgItem(IDC_BUTTON_CONNECT    )->m_hWnd,!bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_DISCONNECT    )->m_hWnd,bFlag);

	if(m_bHasVideo)
		::EnableWindow(GetDlgItem(IDC_BUTTON_OPEN_VIDEO     )->m_hWnd,bFlag);

	::EnableWindow(GetDlgItem(IDC_BUTTON_MOVE_UP    )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_MOVE_RIGHT )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_MOVE_DOWN  )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_MOVE_LEFT  )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_COMBO_PRESETNUM   )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_PRESET     )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_USEPRESET  )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_ZOOM_IN    )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_ZOOM_OUT   )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_SLIDER_SPPED      )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_LEFT_UP    )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_LEFT_DOWN  )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_RIGHT_UP   )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_RIGHT_DOWN )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_IRIS_OPEN  )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_IRIS_CLOSE )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_FOCUS_NEAR )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_FOCUS_FAR  )->m_hWnd,bFlag);

	::EnableWindow(GetDlgItem(IDC_BUTTON_STEP_LEFT	  )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_STEP_RIGHT    )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_STEP_UP       )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_STEP_DOWN     )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_LIGHT    )->m_hWnd,bFlag);
	::EnableWindow(GetDlgItem(IDC_BUTTON_WIPE     )->m_hWnd,bFlag);
}

void CYTControlDlg::OnBnLBDownRight()
{
	m_PTZService->ExecPTZ(ZW_PAN_RIGHT,m_nSpeed,0);
}

void CYTControlDlg::OnBnLBDownLeft()
{
	m_PTZService->ExecPTZ(ZW_PAN_LEFT,m_nSpeed,0);
}

void CYTControlDlg::OnBnLBDownDown()
{
	m_PTZService->ExecPTZ(ZW_TILT_DOWN,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBDownUp()
{
	m_PTZService->ExecPTZ(ZW_TILT_UP,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpRight()
{
	m_PTZService->ExecPTZ(ZW_STOP_PAN,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpLeft()
{
	m_PTZService->ExecPTZ(ZW_STOP_PAN,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpDown()
{
	m_PTZService->ExecPTZ(ZW_STOP_TILT,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpUp()
{
	m_PTZService->ExecPTZ(ZW_STOP_TILT,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBDownZoomIn()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_ZOOM_IN,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBDownZoomOut()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_ZOOM_OUT,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpZoomIn()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_STOP_ZOOM,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpZoomOut()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_STOP_ZOOM,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBDownFocusNear()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_FOCUS_NEAR,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBDownFocusFar()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_FOCUS_FAR,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpFocusNear()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_STOP_FOCUS,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpFocusFar()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_STOP_FOCUS,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBDownIrisOpen()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_IRIS_OPEN,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBDownIrisClose()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_IRIS_CLOSE,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpIrisOpen()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_STOP_IRIS,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnLBUpIrisClose()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_STOP_IRIS,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnClickedButtonWipe()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_btnWipe.isChecked())
		m_PTZService->ExecPTZ(ZW_WIPER_ON,m_nSpeed,m_nSpeed);
	else 
		m_PTZService->ExecPTZ(ZW_WIPER_OFF,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnClickedButtonLight()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_btnLight.isChecked())
		m_PTZService->ExecPTZ(ZW_LIGHT_ON,m_nSpeed,m_nSpeed);
	else 
		m_PTZService->ExecPTZ(ZW_LIGHT_OFF,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnCbnSelchangeComboPresetnum()
{
	// TODO: Add your control notification handler code here
	m_nPresetNum = m_cboPresetNum.GetCurSel()+1;
}

void CYTControlDlg::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	m_nSpeed = m_sliSpeed.GetPos();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CYTControlDlg::OnBnClickedButtonPreset()
{
	// TODO: Add your control notification handler code here
	m_nPresetNum = GetDlgItemInt(IDC_COMBO_PRESETNUM);
	m_PTZService->ExecPTZ(ZW_SET_PRESET,m_nPresetNum,m_nSpeed);
}

void CYTControlDlg::OnBnClickedButtonUsepreset()
{
	// TODO: Add your control notification handler code here
	m_nPresetNum = GetDlgItemInt(IDC_COMBO_PRESETNUM);
	m_PTZService->ExecPTZ(ZW_USE_PRESET,m_nPresetNum,m_nSpeed);
}


void CYTControlDlg::OnBnClickedButtonStepLeft()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_PAN_LEFT,m_nSpeed,m_nSpeed);
	Sleep(10);
	m_PTZService->ExecPTZ(ZW_STOP_PAN,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnClickedButtonStepRight()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_PAN_RIGHT,m_nSpeed,m_nSpeed);
	Sleep(10);
	m_PTZService->ExecPTZ(ZW_STOP_PAN,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnClickedButtonStepDown()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_TILT_DOWN,m_nSpeed,m_nSpeed);
	Sleep(10);
	m_PTZService->ExecPTZ(ZW_STOP_TILT,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnBnClickedButtonStepUp()
{
	// TODO: Add your control notification handler code here
	m_PTZService->ExecPTZ(ZW_TILT_UP,m_nSpeed,m_nSpeed);
	Sleep(10);
	m_PTZService->ExecPTZ(ZW_STOP_TILT,m_nSpeed,m_nSpeed);
}

void CYTControlDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{ 
	m_lbDownPoint.x = point.x-m_rcDisplayWnd.left;
	m_lbDownPoint.y = point.y-m_rcDisplayWnd.top;
	
	if(m_bEnable3D && m_rcDisplay.PtInRect(m_lbDownPoint))
	{
		m_bDriectTo = true;
		m_lbUpPoint.x = m_lbDownPoint.x;
		m_lbUpPoint.y = m_lbDownPoint.y;
	}
	
	CDialog::OnLButtonDown(nFlags, point); 
}

void CYTControlDlg::CalcAimRect(CPoint& pt1,CPoint& pt2, CRect& displayRect, float cof, CRect& aimRect)
{
	if(pt1.x < pt2.x)
	{
		aimRect.left = pt1.x- displayRect.left;
		aimRect.right = pt2.x - displayRect.left;
	}
	else
	{
		aimRect.left = pt2.x - displayRect.left;
		aimRect.right = pt1.x - displayRect.left;
	}

	if(pt1.y < pt2.y)
	{
		aimRect.top = pt1.y - displayRect.top;
		aimRect.bottom = pt2.y - displayRect.top;
	}
	else
	{
		aimRect.top = pt2.y - displayRect.top;
		aimRect.bottom = pt1.y - displayRect.top;
	}

	aimRect.left *= cof;
	aimRect.top *= cof;
	aimRect.right *= cof;
	aimRect.bottom *= cof;
}

void CYTControlDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{ 
	if(m_bDriectTo)
	{
		m_bDriectTo = false;
		CRect aimRect;
		CalcAimRect(m_lbDownPoint,m_lbUpPoint, m_rcDisplay, m_fDisplayZoom,aimRect);

		m_PTZService->DirectTo(1,m_rcDisplay.Width(),
			m_rcDisplay.Height(),
			aimRect.left-m_rcDisplay.left,
			aimRect.top-m_rcDisplay.top,
			aimRect.Width(),
			aimRect.Height());
	}
	CDialog::OnLButtonDown(nFlags, point); 
}

void CYTControlDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pt(point.x-m_rcDisplayWnd.left,point.y-m_rcDisplayWnd.top);
	if(m_bDriectTo && m_rcDisplay.PtInRect(pt))
	{
		m_lbUpPoint = pt;
	}
}
void CYTControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString strMessage;

	if (m_nMaxFrameRate < m_nFrameRateCount)
	{
		m_nMaxFrameRate = m_nFrameRateCount;
		m_naErrorNum[0] = m_nMaxFrameRate*1/4;
		m_naErrorNum[1] = m_nMaxFrameRate*2/4;
		m_naErrorNum[2] = m_nMaxFrameRate*3/4;
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_naErrorNum[i] && m_nFrameRateCount < m_naErrorNum[i])
		{
			m_naErrorNumCount[i]++;
			break;
		}
	}
	
	strMessage.Format("帧率:%d/s 尺寸:%d×%d <%d:%d <%d:%d <%d:%d", m_nFrameRateCount, m_nImgWidth, m_nImgHeight,
		m_naErrorNum[0], m_naErrorNumCount[0], m_naErrorNum[1], m_naErrorNumCount[1], m_naErrorNum[2], m_naErrorNumCount[2]);
	m_nFrameRateCount = 0;
	GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(strMessage);	

	CDialog::OnTimer(nIDEvent);
}
