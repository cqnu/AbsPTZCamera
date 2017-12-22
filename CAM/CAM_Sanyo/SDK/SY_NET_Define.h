
#pragma once

// 实时码流回调函数定义
// nCameraID		设备登陆句柄
// pvRealData		码流数据指针
// dwRealDataSize	码流数据大小
// pvParam			实时码流回调函数参数
typedef void ( CALLBACK *LPRealDataCallBack )( int nCameraID, LPBYTE pvRealData, DWORD dwRealDataSize, LPVOID pvParam ) ;

// 实时JPEG回调函数定义
// nCameraID		设备登陆句柄
// pvRealJpeg		JPEG数据指针
// dwRealJpegSize	JPEG数据大小
// pvParam			实时JPEG回调函数参数
typedef void ( CALLBACK *LPRealJpegCallBack )( int nCameraID, LPBYTE pvRealJpeg, DWORD dwRealJpegSize, LPVOID pvParam ) ;

// 异常处理回调函数定义
// nCameraID		设备登陆句柄
// nExceptionCode	异常信息代码
// pvParam			异常处理回调函数参数
typedef void ( CALLBACK *LPExceptionCallBack )( int nCameraID, int nExceptionCode, LPVOID pvParam ) ;
#define SY_EXCEPTION_RECV_REALDATA_TIMEOUT		0xF001	// 实时数据由于相机断线而中断
#define SY_EXCEPTION_CAMERA_OFFLINE				0xF002	// 相机断线
#define SY_EXCEPTION_CAMERA_REONLINE			0xF003	// 相机断线后恢复
#define SY_EXCEPTION_CAMERA_REALDATA_RECOVERY	0xF004	// 实时数据由于相机恢复而开启

// 设备消息回调函数定义
// nCameraID		设备登陆句柄
// nMsgID			消息类型
// nMsgParam		消息参数
// pvParam			设备消息回调函数参数
typedef void ( CALLBACK *LPDeviceMessageCallBack )( int nCameraID, int nMsgID, int nMsgParam, LPVOID pvParam ) ;
#define SY_DEVMSG_DI							0xE001	// AlarmIn报警	参数: AlarmIn 编号
#define SY_DEVMSG_DO							0xE002	// AlarmOut报警	参数: AlarmOut 编号
#define SY_DEVMSG_MOTION						0xE003	// 移动侦测报警 参数: 无
#define SY_DEVMSG_DI_AND_MOTION					0xE004	// AlarmIn & 移动侦测报警 参数: AlarmIn 编号

// HD4000PC 支持的报警类型 SY_DEVMSG_DI、SY_DEVMSG_MOTION、SY_DEVMSG_DI_AND_MOTION
// HD5600PC 支持的报警类型 SY_DEVMSG_DI、SY_DEVMSG_DO、SY_DEVMSG_MOTION

#pragma pack( push )

////////////////////////////////////////////////////////////////////////////////
// Device Setting

enum SY_DEVICE_SETTING
{
	SYDS_CAMERA_SETTING = 0xC001,	// 设备参数
	SYDS_CODEC_STREAM_STATUS,		// 编解码参数
	SYDS_ALARM_SETTING,				// 报警参数
};

typedef struct tagSY_CODEC_STREAM_STATUS
{
	int		nH264_Width			;
	int		nH264_Height		;
	double	fH264_Frame_Rate	;

} SY_CODEC_STREAM_STATUS, *LPSY_CODEC_STREAM_STATUS ;

typedef struct tagSY_CODEC_STREAM_STATUS2
{
	SY_CODEC_STREAM_STATUS scss ;
	int		nJPEG_Frame_Rate	;

} SY_CODEC_STREAM_STATUS2, *LPSY_CODEC_STREAM_STATUS2 ;

typedef struct tagSY_ALARM_SETTING
{
	int		nAlarmInNum			;
	int		nAlarmOutNum		;
	
} SY_ALARM_SETTING, *LPSY_ALARM_SETTING ;

typedef struct tagSY_CAMERA_SETTING
{
	BYTE bfMAC[ 6 ] ;
} SY_CAMERA_SETTING, *LPSY_CAMERA_SETTING ;

////////////////////////////////////////////////////////////////////////////////

enum SY_CAMERA_CONTROL_TYPE
{
	// 设备自身控制
	SYCCT_PRESET_REGISTRY = 8,		// 参数: 预置位编号
	SYCCT_PRESET_GOTO = 11,			// 参数: 预置位编号
	SYPTZ_PRESET_REMOVE = -11,		// 参数: 预置位编号
	
	SYCCT_ZOOM_TELE_ONE_STEP = 18,	// 参数无
	SYCCT_ZOOM_WIDE_ONE_STEP = 19,	// 参数无
	SYCCT_ZOOM_DIRECT = 52,			// 参数: 位置
	SYCCT_ZOOM_IN = 53,				// 参数: 速度 1 - 4
	SYCCT_ZOOM_OUT = 54,			// 参数: 速度 1 - 4
	SYCCT_ZOOM_STOP = 55,			// 参数无
	
	SYCCT_FOCUS = 49,				// 参数: 0 : NEAR, 1 : FAR, 2 : AF
	SYCCT_IRIS = 50,				// 参数: 0 : OPEN, 1 : CLOSE
	SYCCT_IRIS_LEVEL = 56,			// 参数: 0 - 100

	SYCCT_TILT_UP = 2,				// 参数: 速度
	SYCCT_TILT_DOWN = 7,			// 参数: 速度
	SYCCT_PAN_LEFT = 4,				// 参数: 速度
	SYCCT_PAN_RIGHT = 5,			// 参数: 速度
	SYCCT_PAN_TILT_LEFTUP = 1,		// 参数: MAKEWPARAM( TILT速度, PAN速度 )
	SYCCT_PAN_TILT_RIGHTUP = 3,		// 参数: MAKEWPARAM( TILT速度, PAN速度 )
	SYCCT_PAN_TILT_LEFTDOWN = 6,	// 参数: MAKEWPARAM( TILT速度, PAN速度 )
	SYCCT_PAN_TILT_RIGHTDOWN = -8,	// 参数: MAKEWPARAM( TILT速度, PAN速度 )
	SYCCT_PAN_TILT_STOP = 12,		// 参数无
	
	// 设备扩展控制
// 	SYPTZ_STOP = 1000,				// 参数无
// 
// 	SYPTZ_TILT_UP,					// 参数: 速度
// 	SYPTZ_TILT_DOWN,				// 参数: 速度
// 	SYPTZ_PAN_LEFT,					// 参数: 速度
// 	SYPTZ_PAN_RIGHT,				// 参数: 速度
// 	SYPTZ_TILTPAN_UPLEFT,			// 参数: MAKEWPARAM( TILT速度, PAN速度 )
// 	SYPTZ_TILTPAN_UPRIGHT,			// 参数: MAKEWPARAM( TILT速度, PAN速度 )
// 	SYPTZ_TILTPAN_DOWNLEFT,			// 参数: MAKEWPARAM( TILT速度, PAN速度 )
// 	SYPTZ_TILTPAN_DOWNRIGHT,		// 参数: MAKEWPARAM( TILT速度, PAN速度 )
	
//	SYPTZ_ZOOM_IN,					// 参数无
//	SYPTZ_ZOOM_OUT,					// 参数无
	
//	SYPTZ_FOCUS_FAR,				// 参数无
//	SYPTZ_FOCUS_NEAR,				// 参数无
	
//	SYPTZ_IRIS_OPEN,				// 参数无
//	SYPTZ_IRIS_CLOSE,				// 参数无
	
//	SYPTZ_LIGHT_OPEN,				// 参数无
//	SYPTZ_LIGHT_CLOSE,				// 参数无
	
//	SYPTZ_PRESET_SET,				// 参数: 预置位编号
//	SYPTZ_PRESET_DEL,				// 参数: 预置位编号
//	SYPTZ_PRESET_GOTO,				// 参数: 预置位编号
};

////////////////////////////////////////////////////////////////////////////////
// 已经不再支持的定义

// typedef struct tagSY_ALARMOUT
// {
// 	DWORD	m_dwAlarmOut ; // 0 : off, 1 : on, 2 : remote
// 	DWORD	m_dwPolarity ; // 0 : no, 1 : nc
// 	DWORD	m_dwAlarmOutTime ;	// 0 : 2s, 1 : 5s, 2 : 10s, 3 : 15s, 4 : 30s, 5 : 45s, 6 : 1M, 7 : 2M
// 								// 8 : 3M, 9 : 4M, 10 : 5M, 11 : CC
// } SY_ALARMOUT, *LPSY_ALARMOUT ;

// enum SY_CAMERA_TEST_ALARMOUT
// {
// 	TAO_STOP = 1,
// 	TAO_LEFT = 2,
// 	TAO_RIGHT = 3,
// 	TAO_UP = 4,
// 	TAO_DOWN = 5
// };

////////////////////////////////////////////////////////////////////////////////

#pragma pack( pop )
