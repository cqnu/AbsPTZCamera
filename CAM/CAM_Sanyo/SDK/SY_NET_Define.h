
#pragma once

// ʵʱ�����ص���������
// nCameraID		�豸��½���
// pvRealData		��������ָ��
// dwRealDataSize	�������ݴ�С
// pvParam			ʵʱ�����ص���������
typedef void ( CALLBACK *LPRealDataCallBack )( int nCameraID, LPBYTE pvRealData, DWORD dwRealDataSize, LPVOID pvParam ) ;

// ʵʱJPEG�ص���������
// nCameraID		�豸��½���
// pvRealJpeg		JPEG����ָ��
// dwRealJpegSize	JPEG���ݴ�С
// pvParam			ʵʱJPEG�ص���������
typedef void ( CALLBACK *LPRealJpegCallBack )( int nCameraID, LPBYTE pvRealJpeg, DWORD dwRealJpegSize, LPVOID pvParam ) ;

// �쳣����ص���������
// nCameraID		�豸��½���
// nExceptionCode	�쳣��Ϣ����
// pvParam			�쳣����ص���������
typedef void ( CALLBACK *LPExceptionCallBack )( int nCameraID, int nExceptionCode, LPVOID pvParam ) ;
#define SY_EXCEPTION_RECV_REALDATA_TIMEOUT		0xF001	// ʵʱ��������������߶��ж�
#define SY_EXCEPTION_CAMERA_OFFLINE				0xF002	// �������
#define SY_EXCEPTION_CAMERA_REONLINE			0xF003	// ������ߺ�ָ�
#define SY_EXCEPTION_CAMERA_REALDATA_RECOVERY	0xF004	// ʵʱ������������ָ�������

// �豸��Ϣ�ص���������
// nCameraID		�豸��½���
// nMsgID			��Ϣ����
// nMsgParam		��Ϣ����
// pvParam			�豸��Ϣ�ص���������
typedef void ( CALLBACK *LPDeviceMessageCallBack )( int nCameraID, int nMsgID, int nMsgParam, LPVOID pvParam ) ;
#define SY_DEVMSG_DI							0xE001	// AlarmIn����	����: AlarmIn ���
#define SY_DEVMSG_DO							0xE002	// AlarmOut����	����: AlarmOut ���
#define SY_DEVMSG_MOTION						0xE003	// �ƶ���ⱨ�� ����: ��
#define SY_DEVMSG_DI_AND_MOTION					0xE004	// AlarmIn & �ƶ���ⱨ�� ����: AlarmIn ���

// HD4000PC ֧�ֵı������� SY_DEVMSG_DI��SY_DEVMSG_MOTION��SY_DEVMSG_DI_AND_MOTION
// HD5600PC ֧�ֵı������� SY_DEVMSG_DI��SY_DEVMSG_DO��SY_DEVMSG_MOTION

#pragma pack( push )

////////////////////////////////////////////////////////////////////////////////
// Device Setting

enum SY_DEVICE_SETTING
{
	SYDS_CAMERA_SETTING = 0xC001,	// �豸����
	SYDS_CODEC_STREAM_STATUS,		// ��������
	SYDS_ALARM_SETTING,				// ��������
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
	// �豸�������
	SYCCT_PRESET_REGISTRY = 8,		// ����: Ԥ��λ���
	SYCCT_PRESET_GOTO = 11,			// ����: Ԥ��λ���
	SYPTZ_PRESET_REMOVE = -11,		// ����: Ԥ��λ���
	
	SYCCT_ZOOM_TELE_ONE_STEP = 18,	// ������
	SYCCT_ZOOM_WIDE_ONE_STEP = 19,	// ������
	SYCCT_ZOOM_DIRECT = 52,			// ����: λ��
	SYCCT_ZOOM_IN = 53,				// ����: �ٶ� 1 - 4
	SYCCT_ZOOM_OUT = 54,			// ����: �ٶ� 1 - 4
	SYCCT_ZOOM_STOP = 55,			// ������
	
	SYCCT_FOCUS = 49,				// ����: 0 : NEAR, 1 : FAR, 2 : AF
	SYCCT_IRIS = 50,				// ����: 0 : OPEN, 1 : CLOSE
	SYCCT_IRIS_LEVEL = 56,			// ����: 0 - 100

	SYCCT_TILT_UP = 2,				// ����: �ٶ�
	SYCCT_TILT_DOWN = 7,			// ����: �ٶ�
	SYCCT_PAN_LEFT = 4,				// ����: �ٶ�
	SYCCT_PAN_RIGHT = 5,			// ����: �ٶ�
	SYCCT_PAN_TILT_LEFTUP = 1,		// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
	SYCCT_PAN_TILT_RIGHTUP = 3,		// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
	SYCCT_PAN_TILT_LEFTDOWN = 6,	// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
	SYCCT_PAN_TILT_RIGHTDOWN = -8,	// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
	SYCCT_PAN_TILT_STOP = 12,		// ������
	
	// �豸��չ����
// 	SYPTZ_STOP = 1000,				// ������
// 
// 	SYPTZ_TILT_UP,					// ����: �ٶ�
// 	SYPTZ_TILT_DOWN,				// ����: �ٶ�
// 	SYPTZ_PAN_LEFT,					// ����: �ٶ�
// 	SYPTZ_PAN_RIGHT,				// ����: �ٶ�
// 	SYPTZ_TILTPAN_UPLEFT,			// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
// 	SYPTZ_TILTPAN_UPRIGHT,			// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
// 	SYPTZ_TILTPAN_DOWNLEFT,			// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
// 	SYPTZ_TILTPAN_DOWNRIGHT,		// ����: MAKEWPARAM( TILT�ٶ�, PAN�ٶ� )
	
//	SYPTZ_ZOOM_IN,					// ������
//	SYPTZ_ZOOM_OUT,					// ������
	
//	SYPTZ_FOCUS_FAR,				// ������
//	SYPTZ_FOCUS_NEAR,				// ������
	
//	SYPTZ_IRIS_OPEN,				// ������
//	SYPTZ_IRIS_CLOSE,				// ������
	
//	SYPTZ_LIGHT_OPEN,				// ������
//	SYPTZ_LIGHT_CLOSE,				// ������
	
//	SYPTZ_PRESET_SET,				// ����: Ԥ��λ���
//	SYPTZ_PRESET_DEL,				// ����: Ԥ��λ���
//	SYPTZ_PRESET_GOTO,				// ����: Ԥ��λ���
};

////////////////////////////////////////////////////////////////////////////////
// �Ѿ�����֧�ֵĶ���

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
