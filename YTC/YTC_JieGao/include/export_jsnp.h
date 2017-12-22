//----------------------------------------------------------------------------------
//							Export_JSNP.h	   							
//						����ͨѶ�⵼���ӿ�ͷ�ļ�						
//----------------------------------------------------------------------------------

#ifndef	EXPORT_JSNP_H 
#define	EXPORT_JSNP_H

//----------------------------------------------------------------------------------

#pragma warning (disable : 4786)
#include <iostream>
#include <vector>
#include <afxtempl.h>
using namespace std;

//----------------------------------------------------------------------------------
#ifndef WM_PACKETLOSS
#define WM_PACKETLOSS					WM_USER + 100	// RTP�����Զ�����Ϣ
#endif

#ifndef WM_VIDEOLOSS
#define WM_VIDEOLOSS					WM_USER + 101	// ��Ƶ��ʧ�Զ�����Ϣ
#endif

#ifndef WM_ALARM_INPUT
#define WM_ALARM_INPUT					WM_USER + 102	// ̽ͷ���뱨���Զ�����Ϣ
#endif

#ifndef WM_MOTIONDETECT
#define WM_MOTIONDETECT					WM_USER + 105	// �ƶ�����Զ�����Ϣ
#endif

#ifndef WM_ALARM_HOST
#define WM_ALARM_HOST					WM_USER + 108	// ������������
#endif


// ���нӿں�����ִ�гɹ�ʱ����0����ֵ����ֵ��ʾ����

#define ERR_NP_FUNCTION_NOT_EXIST		-100	// ����������
#define ERR_NP_PARAMETER				-101	// ��������	
#define ERR_NP_CALL_ORDER				-102	// ����˳�����
#define ERR_NP_HANDLE_EXHAUSTED			-103	// ͨѶ����þ�
#define ERR_NP_PREVIEW_START			-104	// Ԥ��ʧ��
#define ERR_NP_DVS_CONNECT				-105	// ������Ƶ������ʧ��
#define ERR_NP_DVS_AUTH					-106	// ��Ƶ��������֤ʧ��
#define ERR_NP_DVS_TYPE_INVALID			-107	// ��Ч���豸����
#define ERR_NP_DVS_STREAM				-108	// ������Ƶ��������ʧ��
#define ERR_NP_CREATE_DECODE			-109	// ����������ʧ��
#define ERR_NP_CREATE_DISPLAY			-110	// ������ʾ�豸ʧ��
#define ERR_NP_CREATE_AUDIO				-111	// ������Ƶ�豸ʧ��
#define ERR_NP_SNAP						-112	// ץͼʧ��
#define ERR_NP_RECORD					-113	// ¼��ʧ��
#define ERR_NP_VOL_SET					-114	// ������Ƶ�������ʧ�� 
#define ERR_NP_VOL_GET					-115	// ��ȡ��Ƶ�������ʧ��
#define ERR_NP_DRAW_CALLBACK			-116	// ע����ʾ�ص�����ʧ��
#define ERR_NP_DECODE_CALLBACK			-117	// ע��������ݻص�����ʧ��
#define ERR_NP_SEARCH_DVS				-118	// �����豸ʧ��
#define ERR_NP_DVSVER_GET				-119	// ��ȡ�豸�汾��Ϣʧ��
#define ERR_NP_PTZ						-120	// ��̨���Ʋ���ʧ��
#define ERR_NP_PRESET_GET				-121	// ��ȡԤ��λʧ��
#define ERR_NP_PRESET_ADD				-122	// ���Ԥ��λʧ��
#define ERR_NP_PRESET_DEL				-123	// ɾ��Ԥ��λʧ��
#define ERR_NP_PRESET_CALL				-124	// ����Ԥ��λʧ��
#define ERR_NP_SENSOROUT_GET			-125	// ��ȡ̽ͷ�������״̬ʧ��
#define ERR_NP_SENSOROUT_SET			-126	// ����̽ͷ�������״̬ʧ��
#define ERR_NP_GUARD_GET				-127	// ��ȡ����λ����ʧ��
#define ERR_NP_GUARD_SET				-128	// ���ÿ���λ����ʧ��
#define ERR_NP_DEV_VOL_GET				-129	// ��ȡ��Ƶ��������������ʧ��
#define ERR_NP_DEV_VOL_SET				-130	// ������Ƶ��������������ʧ��
#define ERR_NP_DEV_BPS_GET				-129	// ��ȡ��Ƶ��������������ʧ��
#define ERR_NP_DEV_BPS_SET				-130	// ������Ƶ��������������ʧ��
#define ERR_NP_DEV_COLOR_GET			-131	// ��ȡ��Ƶ��������ɫ����ʧ��
#define ERR_NP_DEV_COLOR_SET			-132	// ������Ƶ��������ɫ����ʧ��
#define ERR_NP_DEV_COM_GET				-133	// ��ȡ��Ƶ��������������ʧ��
#define ERR_NP_DEV_COM_SET				-134	// ������Ƶ��������������ʧ��
#define ERR_NP_DEV_MD_GET				-135	// ��ȡ��Ƶ�������ƶ��������ʧ��
#define ERR_NP_DEV_MD_SET				-136	// ������Ƶ�������ƶ��������ʧ��
#define ERR_NP_DEV_NET_GET				-137	// ��ȡ��Ƶ��������������ʧ��
#define ERR_NP_DEV_NET_SET				-138	// ������Ƶ��������������ʧ��
#define ERR_NP_DEV_WIFI_GET				-139	// ��ȡ��Ƶ����������WIFI����ʧ��
#define ERR_NP_DEV_WIFI_SET				-140	// ������Ƶ����������WIFI����ʧ��
#define ERR_NP_DEV_OSD_GET				-141	// ��ȡ��Ƶ������OSD����ʧ��
#define ERR_NP_DEV_OSD_SET				-142	// ������Ƶ������OSD����ʧ��
#define ERR_NP_DEV_DEFAULT				-143	// �ָ���Ƶ����������Ĭ��ֵʧ��
#define ERR_NP_DEV_REBOOT				-144	// ������Ƶ�������豸ʧ��
#define ERR_NP_DEV_RESET				-145	// ������Ƶ����������ʧ��
#define ERR_NP_DEV_NAME_GET				-146	// ��ȡ��Ƶ�������豸����ʧ��
#define ERR_NP_DEV_NAME_SET				-147	// ������Ƶ�������豸����ʧ��
#define ERR_NP_DEV_VE_INFO_GET			-148	// ��ȡ��Ƶ��������ǰ������Ϣʧ��
#define ERR_NP_DEV_VE_INFO_SET			-149	// ������Ƶ��������ǰ������Ϣʧ��
#define ERR_NP_CHN_VE_INFO_GET			-150	// ��ȡ��Ƶ��������ǰͨ��������Ϣʧ��
#define ERR_NP_DEV_PORT_GET				-151	// ��ȡ��Ƶ������ͨѶ�˿�ʧ��
#define ERR_NP_DEV_PORT_SET				-152	// ������Ƶ������ͨѶ�˿�ʧ��
#define ERR_NP_DEV_UPNP_GET				-153	// ��ȡ��Ƶ������UPNP״̬ʧ��
#define ERR_NP_DEV_UPNP_SET				-154	// ������Ƶ������UPNP״̬ʧ��
#define ERR_NP_DEV_PPPOE_GET			-155	// ��ȡ��Ƶ������PPPOE״̬ʧ��
#define ERR_NP_DEV_PPPOE_SET			-156	// ������Ƶ������PPPOE״̬ʧ��
#define ERR_NP_DEV_DDNS_GET				-157	// ��ȡ��Ƶ������DDNSʧ��
#define ERR_NP_DEV_DDNS_SET				-158	// ������Ƶ������DDNSʧ��
#define ERR_NP_DEV_NTP_GET				-159	// ��ȡ��Ƶ������NTPʧ��
#define ERR_NP_DEV_NTP_SET				-160	// ������Ƶ������NTPʧ��
#define ERR_NP_DEV_TIME_GET				-159	// ��ȡ��Ƶ������ʱ��ʧ��
#define ERR_NP_DEV_TIME_SET				-160	// ������Ƶ������ʱ��ʧ��
#define ERR_NP_DEV_TIMEZONE_SET			-161	// ������Ƶ������ʱ��ʧ��
#define ERR_NP_DEV_VL_GET				-162	// ��ȡ��Ƶ��������Ƶ��ʧ����ʧ��
#define ERR_NP_DEV_VL_SET				-163	// ������Ƶ��������Ƶ��ʧ����ʧ��
#define ERR_NP_DEV_AUTHMODE_GET			-164	// ��ȡ��Ƶ�������û���֤����ʧ��
#define ERR_NP_DEV_AUTHMODE_SET			-165	// ������Ƶ�������û���֤����ʧ��
#define ERR_NP_DEV_USER_GET				-166	// ��ȡ��Ƶ�������û�ʧ��
#define ERR_NP_DEV_USER_ADD				-167	// ����û�ʧ��
#define ERR_NP_DEV_USER_SET				-168	// �޸��û�����ʧ��
#define ERR_NP_DEV_USER_DEL				-169	// ɾ���û�ʧ��
#define ERR_NP_DEV_UPDATE_PACKET		-170	// ������Ƶ��������ʧ��
#define ERR_NP_DEV_SENSOR_IN_GET		-171	// ��ȡ��Ƶ������̽ͷ��������ʧ��
#define ERR_NP_DEV_SENSOR_IN_SET		-172	// ������Ƶ������̽ͷ��������ʧ��
#define ERR_NP_DEV_REC_STATUS_GET		-173	// ��ȡ��Ƶ������ǰ��¼��״̬����ʧ��
#define ERR_NP_DEV_DISK_STRATEGY_GET	-174	// ��ȡ���̹������ʧ��
#define ERR_NP_DEV_DISK_STRATEGY_SET	-175	// ���ô��̹������ʧ��
#define ERR_NP_DEV_REC_START			-176	// ������Ƶ������ǰ��¼��ʧ��
#define ERR_NP_DEV_TRACK_GET			-177	// ��ȡ��Ƶ�������켣����ʧ��
#define ERR_NP_DEV_TRACK_SET			-178	// ������Ƶ�������켣����ʧ��
#define ERR_NP_DEV_TRACK_CALL			-179	// ���ù켣ʧ��
#define ERR_NP_DEV_SEND_TP09_CMD		-180	// ����͸���˿�ָ��ʧ��
#define ERR_NP_DEV_AC_GET				-181	// ��ȡ��������������������ʧ��
#define ERR_NP_DEV_AC_SET				-182	// ���ñ�������������������ʧ��
#define ERR_NP_DEV_EMAIL_GET			-183	// ��ȡ���������ʼ�����ʧ��
#define ERR_NP_DEV_EMAIL_SET			-184	// ���ñ��������ʼ�����ʧ��
#define ERR_NP_DEV_FTP_GET				-185	// ��ȡ��������FTP����ʧ��
#define ERR_NP_DEV_FTP_SET				-186	// ���ñ�������FTP����ʧ��
#define ERR_NP_DEV_SENSOROUT_GET		-187	// ��ȡ��������̽ͷ�������ʧ��
#define ERR_NP_DEV_SENSOROUT_SET		-188	// ���ñ�������̽ͷ�������ʧ��
#define ERR_NP_DEV_REC_GET				-189	// ��ȡ��������¼������ʧ��
#define ERR_NP_DEV_REC_SET				-190	// ���ñ�������¼������ʧ��
#define ERR_NP_DEV_SENSORIN_CA_GET		-191	// ��ȡ̽ͷ���뱨����������ʧ��
#define ERR_NP_DEV_SENSORIN_CA_SET		-192	// ����̽ͷ���뱨����������ʧ��
#define ERR_NP_DEV_MD_CA_GET			-193	// ��ȡ�ƶ���ⱨ����������ʧ��
#define ERR_NP_DEV_MD_CA_SET			-194	// �����ƶ���ⱨ����������ʧ��
#define ERR_NP_DEV_VL_CA_GET			-195	// ��ȡ��Ƶ��ʧ������������ʧ��
#define ERR_NP_DEV_VL_CA_SET			-196	// ������Ƶ��ʧ������������ʧ��
#define ERR_NP_DEV_VOD_SEARCH_START		-197	// ��ʼ����ǰ��¼���ļ�ʧ��
#define ERR_NP_DEV_VOD_SEARCH_STOP		-198	// ֹͣ����ǰ��¼���ļ�ʧ��
#define ERR_NP_DEV_HS_GET				-199	// ��ȡHS�豸ע��������Ϣ
#define ERR_NP_DEV_HS_SET				-200	// ����HS�豸ע��������Ϣ


#ifndef	TRACK_MAX			
#define	TRACK_MAX						16				// �켣������
#endif

#ifndef PRESET_MAX
#define PRESET_MAX						256				// Ԥ��λ������
#endif

#ifndef PRESET_NAME_MAX
#define PRESET_NAME_MAX					10				// Ԥ��λ��������ֽ���
#endif

#ifndef	STAYTIME_MAX			
#define	STAYTIME_MAX					1800			// ͣ��������ʱ��(��)
#endif

#ifndef	WM_RELEASE_BROADCAST
#define WM_RELEASE_BROADCAST			WM_USER + 107	// �ͷ���Ƶ�㲥��Դ��Ϣ
#endif

// ��̨��������
typedef enum NP_PTZ_Type
{
	NP_PTZ_Up			= 0,
	NP_PTZ_Down			= 1,
	NP_PTZ_Left			= 2,
	NP_PTZ_Right		= 3,
	NP_PTZ_Auto			= 4,
	NP_PTZ_IrisAdd		= 5,
	NP_PTZ_IrisReduce	= 6,
	NP_PTZ_FocusAdd		= 7,
	NP_PTZ_FocusReduce	= 8,
	NP_PTZ_ZoomAdd		= 9,
	NP_PTZ_ZoomReduce	= 10,
	NP_PTZ_Light		= 11,
	NP_PTZ_Brush		= 12,
}NP_PTZ_Type;

typedef enum
{
	NP_AUDIO_CODEC_FORMAT_BEGIN	= -1,
	NP_AUDIO_CODEC_FORMAT_PCM,				// Raw PCM
	NP_AUDIO_CODEC_FORMAT_G711A, 			// G.711 A
	NP_AUDIO_CODEC_FORMAT_G711U, 			// G.711 Mu
	NP_AUDIO_CODEC_FORMAT_ADPCM, 			// ADPCM
	NP_AUDIO_CODEC_FORMAT_G726, 			// G.726
	NP_AUDIO_CODEC_FORMAT_AMRNB, 			// AMR encoder format narrow band
	NP_AUDIO_CODEC_FORMAT_AMRWB, 			// AMR encoder formant wide band
	NP_AUDIO_CODEC_FORMAT_AAC, 				// AAC encoder 
	NP_AUDIO_CODEC_FORMAT_END
} NP_AUDIO_CODEC_FORMAT_E;

typedef enum NP_VEncode_Type_E
{
	NP_VENC_TYPE_QCIF	= 0,						// QCIF
	NP_VENC_TYPE_CIF	= 1,						// CIF
	NP_VENC_TYPE_D1		= 2,						// D1
	NP_VENC_TYPE_720P	= 3,						// 720p����
	NP_VENC_TYPE_1080P	= 4,						// 1080p����
} NP_VEncode_Type_E;

typedef struct NP_VEncode_Type_S_EX					// ֧�ֵı������ͽṹ����
{
	int					nTypeCount;
	NP_VEncode_Type_E	eVEType[_MAX_PATH];
}NP_VEncode_Type_S_EX;

typedef enum NP_VEncode_Format_E
{
	NP_VENC_FORMAT_H261  = 0,						// H261
	NP_VENC_FORMAT_H263  = 1,						// H263
	NP_VENC_FORMAT_H264  = 2,						// H264
	NP_VENC_FORMAT_MPEG2 = 3,						// MPEG2
	NP_VENC_FORMAT_MPEG4 = 4,						// MPEG4
	NP_VENC_FORMAT_MJPEG = 5,						// MOTION_JPEG
} NP_VEncode_Format_E;

typedef struct NP_VEncode_Format_S_EX				// ֧�ֵ���Ƶѹ����ʽ�ṹ����
{
	int					nFmtCount;
	NP_VEncode_Format_E	eVEFmt[_MAX_PATH];
}NP_VEncode_Format_S_EX;

typedef enum NP_VI_Standard_E
{
	NP_VI_STD_PAL	= 0,
		NP_VI_STD_NTSC	= 1,
		NP_VI_STD_AUTO	= 2,
} NP_VI_Standard_E;

typedef struct NP_VI_Standard_S_EX					// ֧�ֵ���Ƶ��ʽ�ṹ����
{
	int					nStdCount;
	NP_VI_Standard_E	eVEStd[_MAX_PATH];
}NP_VI_Standard_S_EX;

typedef enum NP_Nic_Type
{
	NP_Nic_Ethernet	= 0,
	NP_Nic_Wifi		= 1,
}NP_Nic_Type;

typedef enum E_WEEK_DATE
{
	E_WEEK_SUNDAY		= 0,
	E_WEEK_MONDAY		= 1,
	E_WEEK_TUESDAY		= 2,
	E_WEEK_WEDNESDAY	= 3,
	E_WEEK_THURSDAY		= 4,
	E_WEEK_FRIDAY		= 5,
	E_WEEK_SATURDAY		= 6
} E_WEEK_DATE;

typedef enum NP_User_Power_E
{
	NP_User_Power_All		= 0,
	NP_User_Power_Ptz		= 1,
	NP_User_Power_Normal	= 2,
} NP_User_Power_E;

typedef enum NP_E_DevRecord_Status				// ǰ��¼�� ״̬
{
	NP_E_DevRecord_Stop	= 0,					// ǰ��¼�� ֹͣ
	NP_E_DevRecord_Manual	= 1,				// ǰ��¼�� �ֶ�¼��
	NP_E_DevRecord_Alarm	= 2,				// ǰ��¼�� ����¼��
} NP_E_DevRecord_Status;

typedef enum NP_E_DevDiskStrategy				// ǰ�˴��̹������
{
	NP_E_Devdiskstrategy_Stop	= 0,			// ǰ�˴��̹������ ����С�ڱ����ؼ�ʱֹͣ¼��
	NP_E_Devdiskstrategy_Delete	= 1,			// ǰ�˴��̹������ ����С�ڱ����ؼ�ʱɾ���ļ�
} NP_E_DevDiskStrategy;

typedef enum NP_E_DevFileStrategy				// ǰ���ļ����ȹ������
{
	NP_E_DevFileStrategy_Stop	= 0,			// ǰ���ļ����ȹ������ �ļ��ﵽ��󳤶�ʱֹͣ¼��
	NP_E_DevFileStrategy_Switch	= 1,			// ǰ���ļ����ȹ������ �ļ��ﵽ��󳤶�ʱ�л��ļ�
} NP_E_DevFileStrategy;

typedef enum NP_Dev_Record_Type_E				// ǰ��¼���ļ�¼������
{
	NP_Dev_RecType_Manual		= 0,			// �ֶ�¼��
	NP_Dev_RecType_Schedule		= 1,			// ��ʱ¼��
	NP_Dev_RecType_Alarm		= 2,			// ����¼��
	NP_Dev_RecType_All			= 3,			// ȫ��¼��
} NP_Dev_Record_Type_E;

typedef enum NP_Dev_Vod_Type_E					// �����豸ǰ��¼���ļ��ṹ
{
	NP_Dev_Vod_Begin	= 0,					// ��ʼ��
	NP_Dev_Vod_File		= 1,					// ���̰�
	NP_Dev_Vod_End		= 2						// ������
}NP_Dev_Vod_Type_E;

typedef struct NP_Dev_DiskInfo_S				// ǰ�˴��̹���
{
	char		szDiskName[MAX_PATH];			// ��������
	DWORD		dwDiskTotal;					// ����������(M��λ)
	DWORD		dwDiskFree;						// ����ʣ������(M��λ)
}NP_Dev_DiskInfo_S;

typedef struct NP_Dev_DiskInfo_S_EX				// ǰ�˴��̽ṹ����
{
	int					nDiskCount;
	NP_Dev_DiskInfo_S	eDisk[_MAX_PATH];
}NP_Dev_DiskInfo_S_EX;

#ifndef _FRAME_INFO_
#define _FRAME_INFO_
typedef struct
{ 
	long		nWidth;						// �������λΪ���أ��������Ƶ������Ϊ0
	long		nHeight;					// ����ߣ���λΪ���أ��������Ƶ������Ϊ0
	long		nStamp;						// ʱ����Ϣ����λ����
	long		nType;						// �������ͣ���NP_SetDecodeCallBack����˵��
	long		nFrameRate;					// ����ʱ������ͼ��֡��
}FRAME_INFO; 
#endif

#ifndef _RAW_INFO_
#define _RAW_INFO_
typedef struct
{ 
	long		nWidth;						// �������λΪ���أ��������Ƶ������Ϊ0
	long		nHeight;					// ����ߣ���λΪ���أ��������Ƶ������Ϊ0
	long		nStamp;						// ʱ����Ϣ����λ����
	long		nType;						// �������ͣ���NP_SetRawDataCallBack����˵��
	long		nVideoIFrame;				// ��ǰ֡�Ƿ�Ϊ��ƵI֡(1. �ò���ֻ����nTypeΪ��Ƶ����ʱ��Ч 2. ��0ΪI֡,����ΪP֡,��B֡)
	long		nFrameRate;					// ����ʱ������ͼ��֡��
}RAW_INFO; 
#endif

typedef struct
{
	char		szVideoComp[MAX_PATH];		// ��Ƶ����������
	long		lVideoFormat;				// ��Ƶ��������
	long		lVideoWidth;				// ��Ƶ�������λΪ����
	long		lVideoHeight;				// ��Ƶ����ߣ���λΪ����
	long		lVideoFrame;				// ��Ƶ֡��
	
	long		lAudioChns;					// ��Ƶͨ����
	long		lAuidoRate;					// ��Ƶ������
	long		lAudioBits;					// ��Ƶ������
	long		lAudioFormat;				// ��Ƶ��ʽ
}AV_INFO;

typedef struct WEEK_TIME_S								// ��������
{	
	E_WEEK_DATE	emDate;									// ��������
	BOOL		bEnable;								// ���ø����ڲ���
	int			nTime;									// ����ʱ���
}WEEK_TIME_S;

typedef struct WEEK_TIME_S_EX							// ��������ṹ����
{
	WEEK_TIME_S	eWeekTime[7];							// ��������
}WEEK_TIME_S_EX;

typedef struct WIFI_AP_S								// WIFI�豸�ṹ
{
	char		szSSID[MAX_PATH];						// �豸����
	BOOL		bEncry;									// �Ƿ������˼���
}WIFI_AP_S;

typedef struct WIFI_AP_S_EX								// WIFI�豸�ṹ����
{
	int			nAPCount;
	WIFI_AP_S	eWifiAP[_MAX_PATH];
}WIFI_AP_S_EX;

typedef struct DDNS_PROVIDER_S							// DDNS�ṩ�̽ṹ
{
	char		szDDNSProvider[_MAX_PATH];
}DDNS_PROVIDER_S;

typedef struct DDNS_PROVIDER_S_EX						// DDNS�ṩ�̽ṹ����
{
	int				nDdnsCount;
	DDNS_PROVIDER_S	eDDNS[_MAX_PATH];
}DDNS_PROVIDER_S_EX;

typedef struct PTZ_NAME_S								// ֧�ֵ���̨����Э��
{
	char		szPtzName[_MAX_PATH];
}PTZ_NAME_S;

typedef struct PTZ_NAME_S_EX							// ֧�ֵ���̨����Э��ṹ����
{
	int				nPtzCount;
	PTZ_NAME_S		ePtzName[_MAX_PATH];
}PTZ_NAME_S_EX;

typedef struct NP_VEncode_Chn_S
{
	NP_VEncode_Format_E	emVEncFormat;					// ��Ƶѹ������ (H264,MPEG4 ...)
	NP_VEncode_Type_E	emVEncType;						// ����������	(QCIF,CIF,D1 ...)
	NP_VI_Standard_E	emVStandard;					// ��Ƶ��ʽ		(PAL NTSC AUTO ...)
	UINT				nVI;							// ��Ƶ����ͨ������(0 -- n)
} NP_VEncode_Chn_S;

typedef struct NP_VEncode_Chn_S_EX						// ����ͨ���ṹ������
{	
	int					nVECount;
	NP_VEncode_Chn_S	eVEncode[_MAX_PATH];
} NP_VEncode_Chn_S_EX;

typedef struct PRESET_INFO_S							// Ԥ��λ�ṹ
{
	UINT		nPresetID;								// Ԥ��λ��
	char		szPresetName[10];						// Ԥ��λ����,���֧��10���ַ�
	UINT		nStayTime;								// ͣ��ʱ��
} PRESET_INFO_S;

typedef struct PRESET_INFO_S_EX							// Ԥ��λ�ṹ����
{
	int					nPresetCount;
	PRESET_INFO_S		ePreset[_MAX_PATH];
}PRESET_INFO_S_EX;

typedef struct NP_User_Dev_S							// �豸�û��ṹ
{
	char			szUser[MAX_PATH];					// �û���
	char			szPassword[MAX_PATH];				// ����
	NP_User_Power_E	emUserPower;						// �û����ڷ���
}NP_User_Dev_S;

typedef struct NP_User_Dev_S_EX							// �豸�û��ṹ����
{
	int				nUserCount;
	NP_User_Dev_S	eUser[_MAX_PATH];
}NP_User_Dev_S_EX;

typedef struct NP_Dev_Vod_Search_S						// ����ǰ���豸¼���ļ��ṹ
{
	int					nFlag;							// $ Interleave��־λ 36
	NP_Dev_Vod_Type_E	emDevVodType;					// ����ǰ���豸¼���ļ�������
	char				szFilePath[MAX_PATH];			// ǰ���ļ�·��
}NP_Dev_Vod_Search_S;


// ��Ƶ��ʾ�ص�����
typedef  long(CALLBACK* DrawCallBack)		(long hHandle, HDC hDC, long lParam);		

// ԭʼ����Ƶ���ݻص�����
typedef  long ( CALLBACK * DecodeCallBack ) ( long lHandle, const char * pBuf, long  nSize, FRAME_INFO * pFrameInfo, long lParam  );

// ������յ���������������Ƶ֡���ݻص�����
typedef  long ( CALLBACK * RawDataCallBack ) ( long lHandle, const char * pBuf, long  nSize, RAW_INFO * pDataInfo, long lParam  );

// ����ǰ��¼���ļ��ص�����
typedef  long ( CALLBACK * VodSearchCallBack)(NP_Dev_Vod_Search_S* pDevVod, PVOID pParam);	

// ������Ϣ�ص�����
typedef  long( CALLBACK* NetMessageCallBack  ) ( long lHandle, long lMsg,long wParam, long lParam , PVOID pContext);


typedef struct NP_VOD_SEARCH_S
{
	VodSearchCallBack			pCallBackFuncVod;		// �ص�����
	PVOID						pCallBackVod;			// �ص�����ָ��
	DWORD						dwTimeStart;			// ��ʼʱ��
	DWORD						dwTimeStop;				// ����ʱ��
	NP_Dev_Record_Type_E		emDevRecordType;		// ǰ��¼���ļ�����
}NP_VOD_SEARCH_S;


//----------------------------------------------------------------------------------


extern "C"
{
//----------------------------------------------------------------------------------

/*
����˵��
		sFunctionName��	�ӿ����֣�ָ���ĵ��ж���Ľӿں�������
						����NP_����NP_FreeӦΪFree��
		lCanUse��		1��ʾ���ã�����������
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
		���ܵĴ������У�ERR_NP_FUNCTION_NOT_EXIST		
����˵��
		��ѯ�ӿ��Ƿ����
*/
long __stdcall NP_QueryFunction( const char * sFunctionName, long * lCanUse );

//----------------------------------------------------------------------------------

/*
����˵��
		��
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ʼ������ͨѶ��	
*/
long __stdcall NP_Initial( );

//----------------------------------------------------------------------------------

/*
����˵��
		��
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�ͷ�����ͨѶ�⡣
*/
long __stdcall NP_Free();

//----------------------------------------------------------------------------------

/*
����˵��
		lVer��ǰ16λΪ���汾�ţ���16λΪ���汾��		(OUT)
����ֵ
		ִ�гɹ�����0������ʱ�����Զ�������롣
����˵��
		��ȡͨѶ��汾��
*/
long __stdcall NP_GetVersion( long * lVer );

//----------------------------------------------------------------------------------

/*
����˵��
		pszDVSAddr	��Ƶ�������豸ͨѶ��ַ(֧��DDNS��IP��ַ)								(IN)
		pszChn		��Ƶ������ͨ��		("1/cif" �� "1/d1" �� "1/720p" �� "1/1080p")		(IN)
		nPort		��Ƶ������ͨѶ�˿�		(Ĭ��554,��Χ 1 ~~ 65535)						(IN)
		nConnType	������Ƶ������Э������	(0: UDP 1: TCP 2: MUDP)							(IN)
		pszUser		��½��Ƶ���������û���	(0 ~~ 256�ֽ�)									(IN)
		pszPwd		��½��Ƶ������������	(0 ~~ 256�ֽ�)									(IN)
		hWndVideo	��ʾ��Ƶ�Ĵ��ھ��				(���Ϊ��ʱ�����н������ʾ���)		(IN)
		hWndMsg		�������紫������Ϣ�Ĵ��ھ��	(���Ϊ��ʱ���޷����յ���Ϣ)			(IN)
		plHandle	���ص�ǰ�������ӵĲ������												(OUT)

����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������Ԥ��
*/	
long __stdcall NP_StartPreview( const char* pszDVSAddr,const char* pszChn,int nPort,
								 int nConnType,const char* pszUser,const char* pszPwd, 
								 long hWndVideo,long hWndMsg,long* plHandle );

//----------------------------------------------------------------------------------
/*
����˵��
		plHandle	��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�Ͽ�����Ƶ������������,���ͷŵ�ǰ���
*/
long __stdcall NP_StopPrevew( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		sFileName		ץ��ͼƬȫ·������			(c:\Pic\20090510.jpg)					(IN)
		bIsJPEG			�Ƿ�JPEG��ʽ�����ļ�		(0: BMP��ʽ���� ��0: JPEG��ʽ����)		(IN)
		nQuality		ͼƬ����					(����JPEG�ļ���Ч,��Χ��0��100)			(IN)
		lHandle			��ǰ�������ӵĲ������												(IN)
����ֵ		
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��Ƶץ��
*/
long __stdcall NP_SnapShot( const char* sFileName, int bIsJPEG,int nQuality,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		sFileName		¼���ļ�ȫ·������			(c:\video\20090510.mp4)	(IN)
		lHandle			��ǰ�������ӵĲ������								(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ʼ����¼��
*/
long __stdcall NP_StartRecord( const char* sFileName,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle			��ǰ�������ӵĲ������				(IN)	
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		ֹͣ����¼��
*/
long __stdcall NP_StopRecord( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lRecording		�Ƿ�����¼��						(OUT)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ǰͨ���Ƿ���¼��
*/
long __stdcall NP_IsRecording( long* lRecording,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lPreview		�Ƿ�����Ԥ��						(OUT)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ǰͨ���Ƿ���Ԥ��
*/
long __stdcall NP_IsPreview( long* lPreview,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lAudioOut		�Ƿ�������Ƶ���					(OUT)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ǰͨ���Ƿ�����Ƶ���
*/
long __stdcall NP_IsAudioOut( long* lAudioOut,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle			��ǰ�������ӵĲ������				(IN)	
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		ˢ�����һ֡��Ƶ
*/
long __stdcall NP_Refresh( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lVolume			������ֵ����Χ0-0XFFFF				(IN)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ����豸������С
*/
long __stdcall NP_SetVolume( long lVolume ,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lVolume			������ֵ����Χ0-0XFFFF				(OUT)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ����豸������С
*/
long __stdcall NP_GetVolume( long* lVolume,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		hWndMsg			���������Խ���Ϣ���ھ��			(IN)
		lStart			��ʼ��ֹͣ�����Խ�					(IN)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���������Խ�ģ��
*/
long __stdcall NP_DealAudioBroadCast(long hWndMsg,long lStart,long lHandle);

//----------------------------------------------------------------------------------

/*
����˵��
		lStartAudioOut	��0: ��ʼ��Ƶ��� 0: ֹͣ��Ƶ���	(IN)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ʼ��ֹͣ��Ƶ���
*/
long __stdcall NP_StartAudioOut( long lStartAudioOut,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		��	
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�ͷ������Խ�ģ����Դ
*/
long __stdcall NP_UnInitAudioBroadCast();

//----------------------------------------------------------------------------------

/*
����˵��
		idx��		��ʾ��Ӧ��Ϣ�����ͣ�1���ļ����Ž�����2��ץͼ������				(IN)
										3��¼�����	 4: ʵʱ�����紫���쳣�ж�
		hWnd��		���ھ��														(IN)
		lMsg��		��ϢID															(IN)
		lHandle		��ǰ�������ӵĲ������											(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���û�Ӧ��Ϣ
		�����ļ�������ϢLPARAMΪlHandle��WPARAM�����壻
		����ץͼ������ϢLPARAMΪlHandle��WPARAMΪ�ļ��������ַ�����ַ(const char*)��
		����¼�������ϢLPARAMΪlHandle��WPARAMΪ�ļ��������ַ�����ַ(const char*)��
*/
long __stdcall NP_SetMessage( long idx, long hWnd, long lMsg ,long lHandle);

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle			���ž����				(IN)
		DrawCallBack	��ʾ�ص�����			(IN)
		lParam			�ص���������			(IN)
����ֵ	
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������ʾ�ص��������ú�����NP_StartPreview�����,
		��NP_StopPreview���ú��Զ�ʧЧ��
		������õ�ǰ���������Ƶ��豸�����ģ��ṩ�ͻ���������Ƶ�ϵ������ݵĻ��ᡣ

��ʾ�ص���������˵��
		lHandle			���ž����
		hDC				�豸�����ġ�
		lParam			�ͻ������ڵ���HX_SetDrawCallBackʱָ���Ļص�����������
*/
long __stdcall NP_SetDrawCallBack( long( CALLBACK* DrawCallBack  ) ( long lHandle, HDC hDC, long lParam ), long lParam ,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��	
		lHandle			��ǰ����ͨѶ���		(IN)
		DecodeCallBack	�ص�����				(IN)
		lParam			�ص���������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ý���ص��������ûص���������Ϊԭʼ����Ƶ����.
		�ú�����NP_StartPreview�����,��NP_StopPrevew���ú��Զ�ʧЧ��
		���벿�ֲ����Ʋ����ٶȣ�ֻҪ�ͻ�����ӻص������з��أ��������ͻ������һ�������ݡ�

	�ص������Ĳ���˵����
	lHandle��	��ǰ����ͨѶ���
	pBuf��		����������Ƶ����
	nSize��		����������Ƶ���ݳ���
	lParam��	�ͻ������ڵ���HX_SetDecodeCallBackʱָ���Ļص���������
	pFrameInfo��ͼ���������ϢFRAME_INFO�ṹ�������£�
	typedef struct
	{ 
		long nWidth;		// �������λΪ���أ��������Ƶ������Ϊ0
		long nHeight;		// ����ߣ���λΪ���أ��������Ƶ������Ϊ0
		long nStamp;		// ʱ����Ϣ����λ����
		long nType;			// �������ͣ����±�
		long nFrameRate;	// ����ʱ������ͼ��֡��
	}FRAME_INFO;  

	�������ͱ�
	��������		��д						˵��
	1	AUDIO16		��Ƶ��		������16khz����������ÿ��������16λ��ʾ��
	2	RGB32		��Ƶ��		ÿ����4�ֽڣ����з�ʽ��λͼ���ƣ�"BGR0",��һ������λ��ͼ�����½�
	3	YV12		��Ƶ��		yv12��ʽ������˳��"Y0Y1����"��"V0V1����"��"U0-U1����"
	4	UYVY		��Ƶ��		uyvy��ʽ������˳��"U0Y0V0Y1U2Y2V2Y3 �� ��"����һ������λ��ͼ�����Ͻ�
	5	YUV420		��Ƶ��		YUV420��ʽ������˳��"Y0Y1����"��"U0-U1����"��"V0V1����"
	6	yuy2��yuyv	��Ƶ��		yuy2��yuyv��ʽ������˳��"Y0 U0 Y1 V0 Y2 U2 Y3 V2�� ��"����һ������λ��ͼ�����Ͻǡ�
	7	g711		��Ƶ,		������8khz����������ÿ��������16λ��
	8	amr			��Ƶ,		������8khz����������ÿ��������16λ��
*/
long __stdcall NP_SetDecodeCallBack( long ( CALLBACK * DecodeCallBack ) ( long lHandle, const char * pBuf, long  nSize, FRAME_INFO * pFrameInfo, long lParam  ), long lParam  ,long lHandle);

//----------------------------------------------------------------------------------

/*
����˵��	
		lHandle			��ǰ����ͨѶ���		(IN)
		RawDataCallBack	�ص�����				(IN)
		lParam			�ص���������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����������յ���������������Ƶ֡���ݻص�����
		�ú�����NP_StartPreview�����,��NP_StopPrevew���ú��Զ�ʧЧ��
		���벿�ֲ����Ʋ����ٶȣ�ֻҪ�ͻ�����ӻص������з��أ��������ͻ������һ�������ݡ�

	�ص������Ĳ���˵����
	lHandle		��ǰ����ͨѶ���
	pBuf		ѹ��������Ƶ����
	nSize		ѹ��������Ƶ���ݳ���
	lParam		�ͻ������ڵ���NP_SetRawDataCallBackʱָ���Ļص���������
	pDataInfo	ͼ���������ϢRAW_INFO�ṹ�������£�
	typedef struct
	{ 
		long nWidth;		// �������λΪ���أ��������Ƶ������Ϊ0
		long nHeight;		// ����ߣ���λΪ���أ��������Ƶ������Ϊ0
		long nStamp;		// ʱ����Ϣ����λ����
		long nType;			// �������ͣ����±�
		long nVideoIFrame;	// ��ǰ֡�Ƿ�Ϊ��ƵI֡(1. �ò���ֻ����nTypeΪ��Ƶ����ʱ��Ч 2. ��0ΪI֡,����ΪP֡,��B֡)				
		long nFrameRate;	// ����ʱ������ͼ��֡��
	}NP_RAW_INFO;  

	�������ͱ�
	��������		��д						˵��
	1			CODEC_ID_H264				H264��Ƶ��������
	2			CODEC_ID_MPEG4				MPEG4��Ƶ��������
	3			CODEC_ID_MJPEG				MJPEG��Ƶ��������
	10			CODEC_ID_PCM_MULAW			MULAW��Ƶ����
	11			CODEC_ID_AMR				AMR��Ƶ����
*/
long __stdcall NP_SetRawDataCallBack( long ( CALLBACK * RawDataCallBack ) ( long lHandle, const char * pBuf, long  nSize, RAW_INFO * pDataInfo, long lParam  ), long lParam  ,long lHandle);

//----------------------------------------------------------------------------------

/*
����˵��
		emType		��̨��������					(IN)
		lStart		��0: ��ʼ  0: ֹͣ				(IN)
		lPtzSpeed	��̨�ƶ��ٶ�	1 ~~ 63 Ĭ��47	(IN)
		lHandle		��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��̨����
*/
long __stdcall NP_PTZControl( NP_PTZ_Type emType , long lStart , long lPtzSpeed , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		nStartSearch	0:��ʼ���� 1:ֹͣ����					(IN)
		hWnd			����������Ƶ�������豸��Ϣ�Ĵ��ھ��	(IN)
		lMsg			��Ϣ����								(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ʼ��ֹͣ������Ƶ������
*/
long __stdcall NP_SearchDVS( int nStartSearch ,long hWnd,long lMsg );

//----------------------------------------------------------------------------------

/*
����˵��
		pAVInfo			��ǰͨ������Ƶ������Ϣ		(OUT)
		lHandle			��ǰ�������ӵĲ������		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��ǰͨ������Ƶ������Ϣ
*/
long __stdcall NP_GetAVInfo( AV_INFO* pAVInfo , long lHandle );
//----------------------------------------------------------------------------------

/*
����˵��
		pPresets		Ԥ��λ��������				(OUT)
		lHandle			��ǰ�������ӵĲ������		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡԤ��λ������Ϣ
*/
long __stdcall NP_GetPresetInfo( PRESET_INFO_S_EX* pPresets , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lPresetID		Ԥ��λID		(1 ~~ 256)							(IN)
		pszPresetName	Ԥ��λ����		(�����ַ������10���ֽ�,����Ϊ��)	(IN)
		lHandle			��ǰ�������ӵĲ������								(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���Ԥ��λ
*/
long __stdcall NP_AddPreset( long lPresetID , const char* pszPresetName , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lPresetID		Ԥ��λID		(1 ~~ 256)		(IN)
		lHandle			��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		ɾ��Ԥ��λ
*/
long __stdcall NP_DelPreset( long lPresetID , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lPresetID		Ԥ��λID		(1 ~~ 256)		(IN)
		lHandle			��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����Ԥ��λ
*/
long __stdcall NP_CallPreset( long lPresetID , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plSensorOut1	̽ͷ���1����״̬			(1Ϊ��/0λ�ر�)	(OUT)
		plSensorOut2	̽ͷ���2����״̬			(1Ϊ��/0λ�ر�)	(OUT)
		lHandle			��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡԤ��λ������Ϣ
*/
long __stdcall NP_GetSensorOutState( long* plSensorOut1 , long* plSensorOut2 , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lSensorOut1		̽ͷ���1����״̬			(1Ϊ��/0λ�ر�)	(IN)
		lSensorOut2		̽ͷ���2����״̬			(1Ϊ��/0λ�ر�)	(IN)
		lHandle			��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡԤ��λ������Ϣ
*/
long __stdcall NP_SetSensorOutState( long lSensorOut1 , long lSensorOut2 , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plEnable		�Ƿ����ÿ���λ				(1Ϊ����/0λ�ر�)	(OUT)
		plTime			�鿴��λʱ����			(1 ~~ 300 ����)		(OUT)
		plPerp			����λ��Ӧ��Ԥ��λ			(1 ~~ 256)			(OUT)
		lHandle			��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ����λ����
*/
long __stdcall NP_GetGuarderInfo( long* plEnable , long* plTime , long* plPerp , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lEnable			�Ƿ����ÿ���λ				(1Ϊ����/0λ�ر�)	(IN)
		lTime			�鿴��λʱ����			(1 ~~ 300 ����)		(IN)
		lPerp			����λ��Ӧ��Ԥ��λ			(1 ~~ 256)			(IN)
		lHandle			��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ����λ����
*/
long __stdcall NP_SetGuarderInfo( long lEnable , long lTime , long lPerp , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plEnalbe		�Ƿ�������Ƶ����	(0:���� 1:����)		(OUT)
		lHandle			��ǰ�������ӵĲ������
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������������Դ״̬(����/��ֹ)
*/
long __stdcall NP_GetDevAudioInState( long* plEnalbe , long lHandle );

//----------------------------------------------------------------------------------


/*
����˵��
		lEnalbe			�Ƿ�������Ƶ����	(0:���� 1:����)		(IN)
		lHandle			��ǰ�������ӵĲ������
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������������Դ״̬(����/��ֹ)
*/
long __stdcall NP_SetDevAudioInState( long lEnalbe , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plCodec			��Ƶ��������,���ṹNP_AUDIO_CODEC_FORMAT_E	(OUT)
		lHandle			��ǰ�������ӵĲ������
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������,��ʱֻ֧��NP_AUDIO_CODEC_FORMAT_G711U��NP_AUDIO_CODEC_FORMAT_AMRNB
*/
long __stdcall NP_GetDevAudioCodecType( NP_AUDIO_CODEC_FORMAT_E* plCodec , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lCodec			��Ƶ��������,���ṹNP_AUDIO_CODEC_FORMAT_E	(IN)
		lHandle			��ǰ�������ӵĲ������
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������,��ʱֻ֧��NP_AUDIO_CODEC_FORMAT_G711U��NP_AUDIO_CODEC_FORMAT_AMRNB
*/
long __stdcall NP_SetDevAudioCodecType( NP_AUDIO_CODEC_FORMAT_E lCodec , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plVolumeIn		��Ƶ������������������		(0 ~~ 100)	(OUT)
		plVolumeOut		��Ƶ�����������������		(0 ~~ 100)	(OUT)
		lHandle			��ǰ�������ӵĲ������					(IN)
����ֵ	
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������������
*/
long __stdcall NP_GetDevVolume( long* plVolumeIn , long* plVolumeOut , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lVolumeIn		��Ƶ������������������		(0 ~~ 100)		(IN)
		lVolumeOut		��Ƶ�����������������		(0 ~~ 100)		(IN)
		lHandle			��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������������
*/
long __stdcall NP_SetDevVolume( long lVolumeIn , long lVolumeOut , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plBitrate			��Ƶ����				(��λkbit)						(OUT)
		plMaxBitrate		��Ƶ�������			(��λkbit)						(OUT)
		plVBR				����/������ģʽ			(1: ������ģʽ 0: ������ģʽ)	(OUT)
		plFrameRatePrefer	֡������/��������		(1: ֡������ 0: ��������)		(OUT)
		plFrameRate			��Ƶ֡��												(OUT)
		plIFrameInterval	I֡���													(OUT)
		lHandle				��ǰ�������ӵĲ������									(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ���������������Ϣ
*/
long __stdcall NP_GetBpsInfo( long* plBitrate , long* plMaxBitrate , long* plVBR , long* plFrameRatePrefer , 
							 long* plFrameRate , long* plIFrameInterval , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lBitrate			��Ƶ����				(��λkbit)						(IN)
		lVBR				����/������ģʽ			(1: ������ģʽ 0: ������ģʽ)	(IN)
		lFrameRatePrefer	֡������/��������		(1: ֡������ 0: ��������)		(IN)
		lFrameRate			��Ƶ֡��												(IN)
		lIFrameInterval		I֡���													(IN)
		lHandle				��ǰ�������ӵĲ������									(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ���������������Ϣ
*/
long __stdcall NP_SetBpsInfo( long lBitrate , long lVBR , long lFrameRatePrefer , 
							 long lFrameRate , long lIFrameInterval , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plBrightness		����			(0 ~~ 255)		(OUT)
		plContrast			�Աȶ�			(0 ~~ 255)		(OUT)
		plSaturation		���Ͷ�			(0 ~~ 255)		(OUT)
		plHue				ɫ��			(0 ~~ 255)		(OUT)
		plBrightnessDef		Ĭ������		(0 ~~ 255)		(OUT)
		plContrastDef		Ĭ�϶Աȶ�		(0 ~~ 255)		(OUT)
		plSaturationDef		Ĭ�ϱ��Ͷ�		(0 ~~ 255)		(OUT)
		plHueDef			Ĭ��ɫ��		(0 ~~ 255)		(OUT)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������ɫ����
*/
long __stdcall NP_GetVideoColor( long* plBrightness , long* plContrast , 
							   long* plSaturation , long* plHue ,
							   long* plBrightnessDef , long* plContrastDef , 
							   long* plSaturationDef , long* plHueDef ,															   
							   long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lBrightness			����			(0 ~~ 255)		(IN)
		lContrast			�Աȶ�			(0 ~~ 255)		(IN)
		lSaturation			���Ͷ�			(0 ~~ 255)		(IN)
		lHue				ɫ��			(0 ~~ 255)		(IN)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������ɫ����
*/
long __stdcall NP_SetVideoColor( long lBrightness , long lContrast , 
								   long lSaturation , long lHue ,										   
								   long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plCom				����		(0:RS232 �� 1:RS485)			(OUT)
		plStop				ֹͣλ		(1 �� 2)						(OUT)
		plData				����λ		(5��6��7��8)					(OUT)
		pszCheckBit			У��λ		(N��O��E��S)					(OUT)
		plBaud				������		(300��600��1200��2400��4800��	(OUT)
										9600��14400��19200��38400
										56000��57600��115200��
										128000��256000)
		plAddr				��̨��ַ	(1 ~~ 255������)				(OUT)
		pszProt				��̨Э��	(�ǿ���С��30���ֽ��ַ���)		(OUT)	
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������������
*/
long __stdcall NP_GetComSet( long* plCom , long* plStop , 
							   long* plData , char* pszCheckBit ,
							   long* plBaud , long* plAddr , 
							   char* pszProt , long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lCom				����		(0:RS232 �� 1:RS485)			(IN)
		lStop				ֹͣλ		(1 �� 2)						(IN)
		lData				����λ		(5��6��7��8)					(IN)
		pszCheckBit			У��λ		(N��O��E��S)					(IN)
		lBaud				������		(300��600��1200��2400��4800��	(IN)
										9600��14400��19200��38400
										56000��57600��115200��
										128000��256000)
		lAddr				��̨��ַ	(1 ~~ 255������)				(IN)
		pszProt				��̨Э��	(�ǿ���С��30���ֽ��ַ���)		(IN)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������������
*/
long __stdcall NP_SetComSet( long lCom , long lStop , 
						   long lData , char* pszCheckBit ,
						   long lBaud , long lAddr , 
						   char* pszProt , long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plMDEnable			�Ƿ������ƶ����		(OUT)
		plMDLeft			�ƶ��������������		(OUT)
		plMDTop				�ƶ������������		(OUT)
		plMDWidth			�ƶ����������		(OUT)
		plMDHeight			�ƶ��������߶�		(OUT)
		plMDSensitivity		�ƶ����������			(OUT)
		pWeekTimes			�ƶ���Ⲽ������		(OUT)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ�������ƶ��������
*/
long __stdcall NP_GetMDInfo( long* plMDEnable , long* plMDLeft , long* plMDTop ,
						   long* plMDWidth, long* plMDHeight, long* plMDSensitivity ,
						   WEEK_TIME_S_EX* pWeekTimes, 
						   long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lMDEnable			�Ƿ������ƶ����		(IN)
		lMDLeft				�ƶ��������������		(IN)
		lMDTop				�ƶ������������		(IN)
		lMDWidth			�ƶ����������		(IN)
		lMDHeight			�ƶ��������߶�		(IN)
		lMDSensitivity		�ƶ����������			(IN)
		emWeekTimes			�ƶ���Ⲽ������		(IN)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ�������ƶ��������
*/
long __stdcall NP_SetMDInfo( long lMDEnable , long lMDLeft , long lMDTop ,
						   long lMDWidth, long lMDHeight, long lMDSensitivity ,
						   WEEK_TIME_S_EX emWeekTimes,
						   long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszIP				��Ƶ������IP��ַ		(1 ~~ 260���ֽڵ��ַ���)	(OUT)
		pszMac				��Ƶ������MAC��ַ		(1 ~~ 260���ֽڵ��ַ���)	(OUT)
		pszSubMask			��Ƶ��������������		(1 ~~ 260���ֽڵ��ַ���)	(OUT)
		pszGateWay			��Ƶ����������			(1 ~~ 260���ֽڵ��ַ���)	(OUT)
		pszDNS				��Ƶ����������������	(1 ~~ 260���ֽڵ��ַ���)	(OUT)
		plDhcp				��Ƶ������DHCP�Ƿ�����	(1: ���� 0:�ر�)			(OUT)
		lHandle				��ǰ�������ӵĲ������								(IN)
����ֵ	
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ����������Ϣ
*/
long __stdcall NP_GetNetInfo( char* pszIP, char* pszMac, char* pszSubMask,
							char* pszGateWay, char* pszDNS, long* plDhcp,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszIP				��Ƶ������IP��ַ		(1 ~~ 260���ֽڵ��ַ���)(IN)
		pszMac				��Ƶ������MAC��ַ		(1 ~~ 260���ֽڵ��ַ���)(IN)
		pszSubMask			��Ƶ��������������		(1 ~~ 260���ֽڵ��ַ���)(IN)
		pszGateWay			��Ƶ����������			(1 ~~ 260���ֽڵ��ַ���)(IN)
		pszDNS				��Ƶ����������������	(1 ~~ 260���ֽڵ��ַ���)(IN)
		lDhcp				��Ƶ������DHCP�Ƿ�����	(1: ���� 0:�ر�)		(IN)
		lHandle				��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��������������Ϣ
*/
long __stdcall NP_SetNetInfo( char* pszIP, char* pszMac, char* pszSubMask,
							char* pszGateWay, char* pszDNS, long lDhcp,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plExist				����Wifiģ���Ƿ����	(1: ���� 0: ������)		(OUT)
		pszName				����Wifi����									(OUT)
		plEnable			�Ƿ���������Wifiģ��	(1: ���� 0: ������)		(OUT)
		pszIP				����WifiIP��ַ			(1 ~~ 260���ֽڵ��ַ���)(OUT)
		pszMask				����WifiIP��������		(1 ~~ 260���ֽڵ��ַ���)(OUT)
		pszMac				����Wifi MAC��ַ		(1 ~~ 260���ֽڵ��ַ���)(OUT)
		pszGateWay			����Wifi����			(1 ~~ 260���ֽڵ��ַ���)(OUT)
		pszSSID				����Wifi������			(1 ~~ 260���ֽڵ��ַ���)(OUT)
		plSecEn				��ȫ���� ��/�ر�								(OUT)
		plSecType			��ȫ���� wep��wpa								(OUT)
		plWepOpt			������֤ ����ʽ����ʽ							(OUT)
		plWepIndex			��Կ���� 1 ~~ 4									(OUT)
		plWepType			��Կ����										(OUT)
		pszWepPass			��Կ					(1 ~~ 260���ֽڵ��ַ���)(OUT)
		plDhcp				Wifi DHCP ��/�ر�								(OUT)
		pWifiAPs			����Ӧ��ʵ��									(OUT)
		lHandle				��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ����Wifi����������Ϣ
*/
long __stdcall NP_GetWifiInfo( long* plExist, char* pszName, long* plEnable,
							 char* pszIP, char* pszMask, char* pszMac,
							 char* pszGateWay, char* pszSSID, long* plSecEn,
							 long* plSecType, long* plWepOpt, long* plWepIndex,
							 long* plWepType, char* pszWepPass, long* plDhcp,
							 WIFI_AP_S_EX* pWifiAPs,
							 long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lEnable				�Ƿ���������Wifiģ��	(1: ���� 0: ������)			(IN)
		pszIP				����WifiIP��ַ			(1 ~~ 260���ֽڵ��ַ���)	(IN)
		pszMask				����WifiIP��������		(1 ~~ 260���ֽڵ��ַ���)	(IN)
		pszGateWay			����Wifi����			(1 ~~ 260���ֽڵ��ַ���)	(IN)
		pszSSID				����Wifi������			(1 ~~ 260���ֽڵ��ַ���)	(IN)
		lSecEn				��ȫ���� ��/�ر�									(IN)
		plSecType			��ȫ���� wep��wpa									(IN)
		WepOpt				������֤ ����ʽ����ʽ								(IN)
		lWepIndex			��Կ���� 1 ~~ 4										(IN)
		lWepType			��Կ����											(IN)
		pszWepPass			��Կ					(1 ~~ 260���ֽڵ��ַ���)	(IN)
		lDhcp				Wifi DHCP ��/�ر�									(IN)
		lHandle				��ǰ�������ӵĲ������								(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��������Wifi����������Ϣ
*/
long __stdcall NP_SetWifiInfo( long lEnable, char* pszIP, char* pszMask,
							 char* pszGateWay, char* pszSSID, long lSecEn,
							 long lSecType, long lWepOpt, long lWepIndex,
							 long lWepType, char* pszWepPass, long lDhcp,
							 long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszChannelName		ͨ������OSD�ַ���	(1 ~~ 10���ֽڵ��ַ���)	(OUT)
		plShowChannel		��ʾ������ͨ��OSD	(1: ��ʾ 0: ����)		(OUT)
		plShowTime			��ʾ������Time OSD	(1: ��ʾ 0: ����)		(OUT)
		plShowBps			��ʾ������Bps OSD	(1: ��ʾ 0: ����)		(OUT)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡOSD��Ϣ
*/
long __stdcall NP_GetOSDInfo( char* pszChannelName, long* plShowChannel,
							long* plShowTime, long* plShowBps,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszChannelName		ͨ������OSD�ַ���	(1 ~~ 10���ֽڵ��ַ���)	(IN)
		lShowChannel		��ʾ������ͨ��OSD	(1: ��ʾ 0: ����)		(IN)
		lShowTime			��ʾ������Time OSD	(1: ��ʾ 0: ����)		(IN)
		lShowBps			��ʾ������Bps OSD	(1: ��ʾ 0: ����)		(IN)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����OSD��Ϣ
*/
long __stdcall NP_SetOSDInfo( char* pszChannelName, long lShowChannel,
							long  lShowTime, long lShowBps,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle		��ǰ�������ӵĲ������								(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�ָ���Ƶ����������Ĭ��ֵ
*/
long __stdcall NP_LoadDefault( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle		��ǰ�������ӵĲ������								(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ�������豸
*/
long __stdcall NP_Reboot( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle		��ǰ�������ӵĲ������								(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ����������
*/
long __stdcall NP_Reset( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszDevName		��Ƶ�������豸����		(1 ~~ 36���ֽ��ַ���)	(OUT)
		pszDevID		��Ƶ�������豸ID								(OUT)
		pszKernelVer	��Ƶ�������ں˰汾								(OUT)
		pszServiceVer	��Ƶ����������汾								(OUT)
		pszWebVer		��Ƶ��������ҳ�汾								(OUT)
		lHandle			��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ����������
*/
long __stdcall NP_GetDevDes( char* pszDevName , char* pszDevID,
						   char* pszKernelVer, char* pszServiceVer,
						   char* pszWebVer, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszDevName	��Ƶ�������豸����(1 ~~ 36���ֽ��ַ���)		(IN)
		lHandle		��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ����������
*/
long __stdcall NP_SetDevName( char* pszDevName , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plMaxVE				�豸֧�ֵ����������(RTSP����ͨ��������)	(OUT)
		plMaxVI				�豸�������֧�ָ���							(OUT)
		pVETypes			֧�ֵı����ʽ����								(OUT)
		pVEFmts				֧�ֵ���Ƶѹ����ʽ								(OUT)
		pVectorVStdandard	��Ƶ��ʽ										(OUT)
		pVEChns				��Ƶ����ͨ����ʽ����							(OUT)
		lHandle				��ǰ�������ӵĲ������							(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������֧�ֵı�����Ϣ
		�ú�����Ӧ�����豸����ı�������
*/
long __stdcall NP_GetDevVEInfo( long* plMaxVE, long* plMaxVI,
							   NP_VEncode_Type_S_EX* pVETypes,
							   NP_VEncode_Format_S_EX* pVEFmts,
							   NP_VI_Standard_S_EX* pVEStds,
							   NP_VEncode_Chn_S_EX* pVEChns,
							   long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��		
		emVEChns			��Ƶ����ͨ����ʽ����			(IN)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������ǰ����������Ϣ
		�ú������������豸������뻷��
*/
long __stdcall NP_SetDevVEInfo( NP_VEncode_Chn_S_EX emVEChns,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pEncFormat			��ǰͨ����Ƶ�����ʽ		(OUT)
		pEncType			��ǰͨ����������			(OUT)
		pVSTD				��ǰͨ����Ƶ��ʽ			(OUT)
		plVIput				��ǰͨ����Ƶ����Դ			(OUT)
		lHandle				��ǰ�������ӵĲ������		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������ǰͨ��������Ϣ
		����豸��ĳһͨ��
*/
long __stdcall NP_GetChnVEInfo(	NP_VEncode_Format_E* pEncFormat,
								NP_VEncode_Type_E* pEncType,
								NP_VI_Standard_E* pVSTD,
								long* plVIput,
								long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		emEncFormat			��ǰͨ����Ƶ�����ʽ		(IN)
		pszString			��Ӧ�ı����ʽ�ַ���		(OUT)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ�����ʽ��Ӧ�������ַ���,��Mpeg4 H264��
*/
long __stdcall NP_GetVEncFormatString( NP_VEncode_Format_E emEncFormat, char* pszString );

//----------------------------------------------------------------------------------

/*
����˵��
		emEncType			��ǰͨ����Ƶ����������		(IN)
		pszString			��Ӧ�ı����ʽ�ַ���		(OUT)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ݱ������ͻ�ȡ��Ӧ�������ַ���,��D1 Cif��
*/
long __stdcall NP_GetVEncTypeString( NP_VEncode_Type_E emEncType, char* pszString );

//----------------------------------------------------------------------------------

/*
����˵��
		emVSTD				��ǰͨ����Ƶ��ʽ			(IN)
		pszString			��Ӧ�ı����ʽ�ַ���		(OUT)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��ʽ��ȡ��Ӧ�������ַ���,��Pal Ntsc��
*/
long __stdcall NP_GetVStandardString( NP_VI_Standard_E emVSTD, char* pszString );

//----------------------------------------------------------------------------------

/*
����˵��
		pEncType			��ǰͨ����Ƶ����������		(OUT)
		pszString			��Ӧ�ı����ʽ�ַ���		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ݱ��������ַ�����ȡ��Ӧ�ı�������
*/
long __stdcall NP_GetVEncTypeFromString( NP_VEncode_Type_E* pEncType, char* pszString );

//----------------------------------------------------------------------------------

/*
����˵��
		pEncFormat			��ǰͨ����Ƶ�����ʽ		(OUT)
		pszString			��Ӧ�ı����ʽ�ַ���		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ݱ����ʽ�����ַ�����ȡ��Ӧ������
*/
long __stdcall NP_GetVEncFormatFromString( NP_VEncode_Format_E* pEncFormat, char* pszString );

//----------------------------------------------------------------------------------

/*
����˵��
		pVSTD				��ǰͨ����Ƶ��ʽ			(OUT)
		pszString			��Ӧ�ı����ʽ�ַ���		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��ʽ�����ַ�����ȡ��Ӧ����Ƶ��ʽ
*/
long __stdcall NP_GetVStandardFromString( NP_VI_Standard_E* pVSTD, char* pszString );

//----------------------------------------------------------------------------------

/*
����˵��
		plRTSPPort			RTSPͨѶ�˿�		Ĭ��554		(OUT)
		plWebPort			��ҳͨѶ�˿�		Ĭ��80		(OUT)
		plABCPort			�����Խ��˿�		Ĭ��8004	(OUT)
		plUpdatePort		�����豸�˿�		Ĭ��8006	(OUT)
		plFtpPort			FTPͨѶ�˿�			Ĭ��21		(OUT)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������ͨѶ�˿���Ϣ
*/
long __stdcall NP_GetCommunicationPort(	long* plRTSPPort,
										long* plWebPort,
										long* plABCPort,
										long* plUpdatePort,
										long* plFtpPort,
										long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		lRTSPPort			RTSPͨѶ�˿�		Ĭ��554		(IN)
		lWebPort			��ҳͨѶ�˿�		Ĭ��80		(IN)
		lABCPort			�����Խ��˿�		Ĭ��8004	(IN)
		lUpdatePort			�����豸�˿�		Ĭ��8006	(IN)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������ͨѶ�˿���Ϣ
*/
long __stdcall NP_SetCommunicationPort(	long lRTSPPort,
										long lWebPort,
										long lABCPort,
										long lUpdatePort,
										long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		plRtspUpnpState		Rtsp�˿�UPNP�Ƿ�����		1:���� 0:δ����	(OUT)
		plWebUpnpState		Web�˿�UPNP�Ƿ�����			1:���� 0:δ����	(OUT)
		plAbcUpnpState		�����Խ��˿�UPNP�Ƿ�����	1:���� 0:δ����	(OUT)
		plUpdateUpnpState	������¶˿�UPNP�Ƿ�����	1:���� 0:δ����	(OUT)
		plFtpUpnpState		Ftp�˿�UPNP�Ƿ�����			1:���� 0:δ����	(OUT)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������UPNP��Ϣ
*/
long __stdcall NP_GetUPNPState( long* plRtspUpnpState, long* plWebUpnpState,
							   long* plAbcUpnpState,long* plUpdateUpnpState,
							   long* plFtpUpnpState,long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		lRtspUpnpState		Rtsp�˿�UPNP�Ƿ�����		1:���� 0:δ����	(IN)
		lWebUpnpState		Web�˿�UPNP�Ƿ�����			1:���� 0:δ����	(IN)
		lAbcUpnpState		�����Խ��˿�UPNP�Ƿ�����	1:���� 0:δ����	(IN)
		lUpdateUpnpState	������¶˿�UPNP�Ƿ�����	1:���� 0:δ����	(IN)
		lFtpUpnpState		Ftp�˿�UPNP�Ƿ�����			1:���� 0:δ����	(IN)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������UPNP��Ϣ
*/
long __stdcall NP_SetUPNPState( long lRtspUpnpState, long lWebUpnpState,
							   long lAbcUpnpState,long lUpdateUpnpState,
							   long lFtpUpnpState,long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		pNicType			PPPOEѡ������		0: ���� 1: ����Wifi		(OUT)
		plEnable			�Ƿ�����PPPOE								(OUT)
		pszUser				�����û���									(OUT)
		pszPwd				��������									(OUT)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������PPPOE��Ϣ
*/
long __stdcall NP_GetPPPOEInfo(  NP_Nic_Type* pNicType,
								long* plEnable, 
								char* pszUser,
								char* pszPwd,
								long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		emNicType			PPPOEѡ������		0: ���� 1: ����Wifi		(IN)
		lEnable				�Ƿ�����PPPOE								(IN)
		pszUser				�����û���									(IN)
		pszPwd				��������									(IN)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������PPPOE��Ϣ
*/
long __stdcall NP_SetPPPOEInfo(  NP_Nic_Type emNicType,
								long lEnable, 
								char* pszUser,
								char* pszPwd,
								long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		plEnable			�Ƿ�����DDNS							(OUT)
		pDdnsProviders		֧�ֵ�DDNS�����ṩ��					(OUT)
		pszProvider			��ǰʹ�õ�DDNS�����ṩ��				(OUT)
		pszUser				�û���									(OUT)
		pszPwd				����									(OUT)
		pszURL				DDNS��ַ								(OUT)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������DDNS��Ϣ
*/
long __stdcall NP_GetDDNSInfo(  long* plEnable, 
								DDNS_PROVIDER_S_EX* pDdnsProviders,
								char* pszProvider,
								char* pszUser,
								char* pszPwd,
								char* pszURL,
								long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		lEnable				�Ƿ�����DDNS							(IN)
		pszProvider			��ǰʹ�õ�DDNS�����ṩ�����260���ֽ�	(IN)
		pszUser				�û������63���ֽ�						(IN)
		pszPwd				�������63���ֽ�						(IN)
		pszURL				DDNS��ַ���260���ֽ�					(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������DDNS��Ϣ
*/
long __stdcall NP_SetDDNSInfo(  long lEnable,
								char* pszProvider,
								char* pszUser,
								char* pszPwd,
								char* pszURL,
								long lHandle  );

//----------------------------------------------------------------------------------

/*
����˵��
		plNTPEnable			NTP�Ƿ�����				1:���� 0:δ����	(OUT)
		pszNTPServer		NTP��������ַ							(OUT)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������NTP��Ϣ
*/
long __stdcall NP_GetNTPInfo(  long* plNTPEnable, char* pszNTPServer, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lNTPEnable			NTP�Ƿ�����				1:���� 0:δ����	(IN)
		pszNTPServer		NTP��������ַ							(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������NTP��Ϣ
*/
long __stdcall NP_SetNTPInfo(  long lNTPEnable, char* pszNTPServer, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plTime				��ǰ��Ƶ�������ڲ�ʱ��					(OUT)
		plTimeZone			��ǰ��Ƶ�������ڲ�ʱ��					(OUT)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������ʱ����Ϣ
*/
long __stdcall NP_GetTimeInfo(  long* plTime, long* plTimeZone, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lTime				��ǰ��Ƶ�������ڲ�ʱ��					(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������ʱ����Ϣ
*/
long __stdcall NP_SetTime(  long lTime, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lTimeZone			��ǰ��Ƶ�������ڲ�ʱ��					(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������ʱ����Ϣ
*/
long __stdcall NP_SetTimeZone(  long lTimeZone, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plEnable			�Ƿ�������Ƶ��ʧ���					(OUT)
		plSensitivity		��Ƶ��ʧ������							(OUT)
		pWeekTimes			��������								(OUT)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������Ƶ��ʧ������Ϣ
*/
long __stdcall NP_GetVideoLoss(  long* plEnable, long* plSensitivity,
								  WEEK_TIME_S_EX* pWeekTimes, 
								  long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lEnable				�Ƿ�������Ƶ��ʧ���					(IN)
		lSensitivity		��Ƶ��ʧ������							(IN)
		emWeekTimes			��������								(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������Ƶ��ʧ������Ϣ
*/
long __stdcall NP_SetVideoLoss(  long lEnable, long lSensitivity,
								  WEEK_TIME_S_EX emWeekTimes,
								  long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plEnable			�������û���֤							(OUT)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ�������Ƿ������û���֤
*/
long __stdcall NP_GetAuthMode(  long* plEnable, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lEnable				�������û���֤							(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ�������Ƿ������û���֤
*/
long __stdcall NP_SetAuthMode(  long lEnable, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pUsers				��Ƶ�������û��б�						(OUT)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ�������Ƿ������û���֤
*/
long __stdcall NP_GetAllUser( NP_User_Dev_S_EX* pUsers, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszUser				�û���					(IN)
		pszPwd				����					(IN)
		emPower				Ȩ��					(IN)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����û�
*/
long __stdcall NP_AddUser( char* pszUser,char* pszPwd,
							NP_User_Power_E emPower, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszUser				�û���					(IN)
		pszPwd				����					(IN)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�޸��û�����
*/
long __stdcall NP_SetUserPwd( char* pszUser,char* pszPwd, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszUser				�û���					(IN)
		emPower				Ȩ��					(IN)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�޸��û�Ȩ��
*/
long __stdcall NP_SetUserPower( char* pszUser, NP_User_Power_E emPower, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszUser				�û���					(IN)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		ɾ���û�
*/
long __stdcall NP_DelUser( char* pszUser, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle		��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��������
*/
long __stdcall NP_UpdatePacket( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plSensor1En		̽ͷ����1�Ƿ�����			(OUT)
		plSensor2En		̽ͷ����2�Ƿ�����			(OUT)
		plSensor3En		̽ͷ����3�Ƿ�����			(OUT)
		plSensor4En		̽ͷ����4�Ƿ�����			(OUT)
		pWeekTimes		������������				(OUT)
		lHandle			��ǰ�������ӵĲ������		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������̽ͷ������������
*/
long __stdcall NP_GetSensorIn( long* plSensor1En, long* plSensor2En,
							 long* plSensor3En, long* plSensor4En, 
							 WEEK_TIME_S_EX* pWeekTimes, 
							 long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lSensor1En		̽ͷ����1�Ƿ�����			(IN)
		lSensor2En		̽ͷ����2�Ƿ�����			(IN)
		lSensor3En		̽ͷ����3�Ƿ�����			(IN)
		lSensor4En		̽ͷ����4�Ƿ�����			(IN)
		emWeekTimes		��������ṹ����			(IN)
		lHandle			��ǰ�������ӵĲ������		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������̽ͷ������������
*/
long __stdcall NP_SetSensorIn( long lSensor1En, long lSensor2En,
							   long lSensor3En, long lSensor4En, 
							   WEEK_TIME_S_EX emWeekTimes, 
							   long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plStatus	ǰ��¼��״̬			(OUT)
		lHandle		��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������ǰ��¼��״̬
*/
long __stdcall NP_GetDevRecordStatus( NP_E_DevRecord_Status* plStatus, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plDiskReserve		���̱����ռ�(M��λ)			(OUT)
		pDiskStrategy		���̹������				(OUT)
		plFileMaxTime		¼���ļ���󳤶�(���ӵ�λ)	(OUT)
		pFileStrategy		�ļ��������				(OUT)
		pDisks				����ͳ����Ϣ				(OUT)
		pWeekTimes			��������					(OUT)
		lHandle				��ǰ�������ӵĲ������		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ���������̲���
*/
long __stdcall NP_GetDevDiskStrategy( long* plDiskReserve, NP_E_DevDiskStrategy* pDiskStrategy,
									long* plFileMaxTime, NP_E_DevFileStrategy* pFileStrategy,
									NP_Dev_DiskInfo_S_EX* pDisks,
									WEEK_TIME_S_EX* pWeekTimes,
									long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lDiskReserve		���̱����ռ�(M��λ)			(IN)
		emDiskStrategy		���̹������				(IN)
		lFileMaxTime		¼���ļ���󳤶�(���ӵ�λ)	(IN)
		emFileStrategy		�ļ��������				(IN)
		emWeekTimes			��������					(IN)
		lHandle				��ǰ�������ӵĲ������		(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ���������̲���
*/
long __stdcall NP_SetDevDiskStrategy( long lDiskReserve, NP_E_DevDiskStrategy emDiskStrategy,
									 long lFileMaxTime, NP_E_DevFileStrategy emFileStrategy,
									 WEEK_TIME_S_EX emWeekTimes, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lStart		��ʼ��ֹͣǰ��¼��		(IN)
		lHandle		��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��Ƶ������ǰ��¼�����
*/
long __stdcall NP_StartDevRecord( long lStart, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lTrackID		�켣ID 1 ~~ 16			(IN)
		pPresets		��ǰ�켣��Ӧ��Ԥ��λ����(OUT)
		lHandle			��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������ָ���켣����
*/
long __stdcall NP_GetTrackInfo( long lTrackID, 
							 PRESET_INFO_S_EX* pPresets ,
							 long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lTrackID		�켣ID 1 ~~ 16			(IN)
		emPresets		��ǰ�켣��Ӧ��Ԥ��λ����(IN)
		lHandle			��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ������ָ���켣����
*/
long __stdcall NP_SetTrackInfo( long lTrackID, 
								 PRESET_INFO_S_EX emPresets,
								 long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lTrackID	�켣ID	1 ~~ 16			(IN)
		lHandle		��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�켣����
*/
long __stdcall NP_CallTrack( long lTrackID, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		TP09_ON_RECV_CB	͸���˿ڻص�����		(IN)
		pParam			�Զ������				(IN)
		lHandle			��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����͸���˿ڻص�����
*/
long __stdcall NP_SetTP09CallBackFunc( UINT ( CALLBACK* TP09_ON_RECV_CB ) ( BYTE* pData,UINT nData,PVOID pParam), PVOID pParam , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pData			͸���˿�ָ������				(IN)
		lData			ָ�����ݳ���(���2048�ֽ�)		(IN)
		lHandle			��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����͸���˿�ָ��
*/
long __stdcall NP_SendTP09CMD( LPBYTE pData,long lData , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszAlarmCenterAddr	�������ĵ�ַ(IP��DDNS)			(OUT)
		plPort				��������ͨѶ�˿�  ( 1 ~ 65535)	(OUT)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��������������������
*/
long __stdcall NP_GetCAAlarmCenter( char* pszAlarmCenterAddr, long* plPort , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszAlarmCenterAddr	�������ĵ�ַ(IP��DDNS)			(IN)
		lPort				��������ͨѶ�˿�  ( 1 ~ 65535)	(IN)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ñ�������������������
*/
long __stdcall NP_SetCAAlarmCenter( char* pszAlarmCenterAddr, long lPort , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszEmailAddr		�����������ַ			(OUT)
		pszUser				��½�����û���			(OUT)
		pszPwd				��½��������			(OUT)
		pszToAddr			�����ʼ���Email			(OUT)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ�������������ʼ�����
*/
long __stdcall NP_GetCAEmail( char* pszEmailAddr, char* pszUser, char* pszPwd ,
														   char* pszToAddr , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszEmailAddr		�����������ַ			(IN)
		pszUser				��½�����û���			(IN)
		pszPwd				��½��������			(IN)
		pszToAddr			�����ʼ���Email			(IN)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ñ������������ʼ�����
*/
long __stdcall NP_SetCAEmail( char* pszEmailAddr, char* pszUser, char* pszPwd ,
							   char* pszToAddr , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszFTPHost			FTP������ַ						(OUT)
		pszUser				��½�û���						(OUT)
		pszPwd				��½����						(OUT)
		pszPath			    FTP·��							(OUT)
		plType				�ϴ��ļ����� (0: ͼƬ 1: ¼��)	(OUT)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ���������ϴ�FTP����
*/
long __stdcall NP_GetCAFTP( char* pszFTPHost, char* pszUser, char* pszPwd ,
						   char* pszPath , long* plType, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszFTPHost			FTP������ַ						(IN)
		pszUser				��½�û���						(IN)
		pszPwd				��½����						(IN)
		pszPath			    FTP·��							(IN)
		lType				�ϴ��ļ����� (0: ͼƬ 1: ¼��)	(IN)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ñ��������ϴ�FTP����
*/
long __stdcall NP_SetCAFTP( char* pszFTPHost, char* pszUser, char* pszPwd ,
						   char* pszPath , long lType, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plSensorOut1		̽ͷ���1״̬ (0: �ر� 1: ��)	(OUT)
		plSensorOut2		̽ͷ���2״̬ (0: �ر� 1: ��)	(OUT)
		plHoldTime			̽ͷ���״̬����ʱ��			(OUT)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��������̽ͷ�������
*/
long __stdcall NP_GetCASensorOut( long* plSensorOut1,long* plSensorOut2 ,long* plHoldTime, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lSensorOut1		̽ͷ���1״̬ (0: �ر� 1: ��)	(IN)
		lSensorOut2		̽ͷ���2״̬ (0: �ر� 1: ��)	(IN)
		lHoldTime		̽ͷ���״̬����ʱ��			(IN)
		lHandle			��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		���ñ�������̽ͷ�������
*/
long __stdcall NP_SetCASensorOut( long lSensorOut1,long lSensorOut2 ,long lHoldTime, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lSensorIn			̽ͷ�������			(0 ~ 3)				(IN)
		plACEn				����ʱ������������		(0: �ر� 1: ����)	(OUT)
		plEmailEn			����ʱ���������ʼ�		(0: �ر� 1: ����)	(OUT)
		plAO1En				����ʱ������̽ͷ���1 (0: �ر� 1: ����)	(OUT)
		plAO2En				����ʱ������̽ͷ���2 (0: �ر� 1: ����)	(OUT)
		plRecordEn			����ʱ����¼��			(0: �ر� 1: ����)	(OUT)
		plFTPEn				����ʱ�����ϴ��ļ���FTP (0: �ر� 1: ����)	(OUT)
		plSoundEn			����ʱ����������Ƶ		(0: �ر� 1: ����)	(OUT)
		plSnapEn			����ʱ����ץͼ			(0: �ر� 1: ����)	(OUT)
		plInterval			�������				(5 ~ 36000 ��)		(OUT)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ̽ͷ���뱨����������
*/
long __stdcall NP_GetCASensorIn( long lSensorIn,long* plACEn,long* plEmailEn,
							   long* plAO1En, long* plAO2En,long* plRecordEn,
							   long* plFTPEn, long* plSoundEn,long* plSnapEn,
							   long* plInterval,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lSensorIn			̽ͷ�������			(0 ~ 3)				(IN)
		lACEn				����ʱ������������		(0: �ر� 1: ����)	(IN)
		lEmailEn			����ʱ���������ʼ�		(0: �ر� 1: ����)	(IN)
		lAO1En				����ʱ������̽ͷ���1 (0: �ر� 1: ����)	(IN)
		lAO2En				����ʱ������̽ͷ���2 (0: �ر� 1: ����)	(IN)
		lRecordEn			����ʱ����¼��			(0: �ر� 1: ����)	(IN)
		lFTPEn				����ʱ�����ϴ��ļ���FTP (0: �ر� 1: ����)	(IN)
		lSoundEn			����ʱ����������Ƶ		(0: �ر� 1: ����)	(IN)
		lSnapEn				����ʱ����ץͼ			(0: �ر� 1: ����)	(IN)
		lInterval			�������				(5 ~ 36000 ��)		(IN)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����̽ͷ���뱨����������
*/
long __stdcall NP_SetCASensorIn( long lSensorIn,long lACEn,long lEmailEn,
							   long lAO1En, long lAO2En,long lRecordEn,
							   long lFTPEn, long lSoundEn,long lSnapEn,
							   long lInterval, long lHandle );

//----------------------------------------------------------------------------------


/*
����˵��
		plACEn				����ʱ������������		(0: �ر� 1: ����)	(OUT)
		plEmailEn			����ʱ���������ʼ�		(0: �ر� 1: ����)	(OUT)
		plAO1En				����ʱ������̽ͷ���1 (0: �ر� 1: ����)	(OUT)
		plAO2En				����ʱ������̽ͷ���2 (0: �ر� 1: ����)	(OUT)
		plRecordEn			����ʱ����¼��			(0: �ر� 1: ����)	(OUT)
		plFTPEn				����ʱ�����ϴ��ļ���FTP (0: �ر� 1: ����)	(OUT)
		plSoundEn			����ʱ����������Ƶ		(0: �ر� 1: ����)	(OUT)
		plSnapEn			����ʱ����ץ��			(0: �ر� 1: ����)	(OUT)
		plInterval			�������				(5 ~ 36000 ��)		(OUT)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ�ƶ���ⱨ����������
*/
long __stdcall NP_GetCAMD( long* plACEn,long* plEmailEn,long* plAO1En,
						   long* plAO2En,long* plRecordEn,long* plFTPEn,
						   long* plSoundEn,long* plSnapEn,
						   long* plInterval,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lACEn				����ʱ������������		(0: �ر� 1: ����)	(IN)
		lEmailEn			����ʱ���������ʼ�		(0: �ر� 1: ����)	(IN)
		lAO1En				����ʱ������̽ͷ���1 (0: �ر� 1: ����)	(IN)
		lAO2En				����ʱ������̽ͷ���2 (0: �ر� 1: ����)	(IN)
		lRecordEn			����ʱ����¼��			(0: �ر� 1: ����)	(IN)
		lFTPEn				����ʱ�����ϴ��ļ���FTP (0: �ر� 1: ����)	(IN)
		lSoundEn			����ʱ����������Ƶ		(0: �ر� 1: ����)	(IN)
		lSnapEn				����ʱ����ץͼ			(0: �ر� 1: ����)	(IN)
		lInterval			�������				(5 ~ 36000 ��)		(IN)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�����ƶ���ⱨ����������
*/
long __stdcall NP_SetCAMD( long lACEn,long lEmailEn,long lAO1En,
						   long lAO2En,long lRecordEn,long lFTPEn,
						   long lSoundEn,long lSnapEn,
						   long lInterval,long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plACEn				����ʱ������������		(0: �ر� 1: ����)	(OUT)
		plEmailEn			����ʱ���������ʼ�		(0: �ر� 1: ����)	(OUT)
		plAO1En				����ʱ������̽ͷ���1 (0: �ر� 1: ����)	(OUT)
		plAO2En				����ʱ������̽ͷ���2 (0: �ر� 1: ����)	(OUT)
		plSoundEn			����ʱ����������Ƶ		(0: �ر� 1: ����)	(OUT)
		plInterval			�������				(5 ~ 36000 ��)		(OUT)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��ʧ������������
*/
long __stdcall NP_GetCAVL( long* plACEn,long* plEmailEn,long* plAO1En,
														 long* plAO2En,long* plSoundEn,long* plInterval,
														 long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lACEn				����ʱ������������		(0: �ر� 1: ����)	(IN)
		lEmailEn			����ʱ���������ʼ�		(0: �ر� 1: ����)	(IN)
		lAO1En				����ʱ������̽ͷ���1 (0: �ر� 1: ����)	(IN)
		lAO2En				����ʱ������̽ͷ���2 (0: �ر� 1: ����)	(IN)
		lSoundEn			����ʱ����������Ƶ		(0: �ر� 1: ����)	(IN)
		lInterval			�������				(5 ~ 36000 ��)		(IN)
		lHandle				��ǰ�������ӵĲ������						(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������Ƶ��ʧ������������
*/
long __stdcall NP_SetCAVL( long lACEn,long lEmailEn,long lAO1En,
						 long lAO2En,long lSoundEn,long lInterval,
						 long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		plRecordTime		����¼�񳤶�	(10 ~ 36000 ��)		(OUT)
		lHandle				��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ����¼����������
*/
long __stdcall NP_GetCARecord( long* plRecordTime , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lRecordTime			����¼�񳤶�	(10 ~ 36000 ��)		(IN)
		lHandle				��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ����¼����������
*/
long __stdcall NP_SetCARecord( long lRecordTime , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		emVodSearch		����ǰ��¼���ļ��ṹ				(IN)
		lHandle			��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ʼ����ǰ��¼���ļ�
*/
long __stdcall NP_StartVodSearch( NP_VOD_SEARCH_S emVodSearch , long lHandle );	

//----------------------------------------------------------------------------------

/*
����˵��
		lHandle		��ǰ�������ӵĲ������				(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		ֹͣ����ǰ��¼���ļ�
*/
long __stdcall NP_StopVodSearch( long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pPtzNames			��Ƶ��������̨Э���б�					(OUT)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ��������̨Э���б�
*/
long __stdcall NP_GetSupportPTZProtocol( PTZ_NAME_S_EX* pPtzNames, long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		ALARM_ON_RECV_CB	�����ص�����			(IN)
		pParam				�Զ������				(IN)
		lHandle				��ǰ�������ӵĲ������	(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����ͨ�ñ����ص�����	(֧��: 0: ��Ƶ��ʧ���� 1: �ƶ���ⱨ�� 2: ̽ͷ���뱨��)
*/
long __stdcall NP_SetAlarmCallBackFunc( UINT ( CALLBACK* ALARM_ON_RECV_CB ) ( UINT nParam1 , UINT nParam2,PVOID pParam), PVOID pParam , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		pszDVSAddr	��Ƶ�������豸ͨѶ��ַ(֧��DDNS��IP��ַ)
		nChn		��Ƶ������ͨ������		(0 ~~~ ���ͨ���� - 1)
		nPort		��Ƶ������ͨѶ�˿�		(Ĭ��554,��Χ 1 ~~ 65535)
		nConnType	������Ƶ������Э������	(0: UDP 1: TCP 2: MUDP)
		pszUser		��½��Ƶ���������û���	(0 ~~ 256�ֽ�)
		pszPwd		��½��Ƶ������������	(0 ~~ 256�ֽ�)
		hWndMsg		�������紫������Ϣ�Ĵ��ھ��	(���Ϊ��ʱ���޷����յ���Ϣ)
		plHandle	���ص�ǰ�������ӵĲ������		

����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��������Ƶ���������Ӳ�������֤,�ɹ����غ���������豸������������̨,����
		����Ƶ������.
*/	
long __stdcall NP_MakeAuthentication( const char* pszDVSAddr,const char* pszChn,int nPort,
									int nConnType,const char* pszUser,const char* pszPwd, 
									long hWndMsg,long* plHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lStart				��ʼ��ֹͣ								(IN)
		lSeconds			��ǰ¼��ʱ�䳤��(��λ:�� ��Χ1~3600��)	(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ʼ��ֹͣʹ����ǰ¼����
*/
long __stdcall NP_SetPreRecord( long lStart , long lSeconds , long lHandle );

//----------------------------------------------------------------------------------

/*
����˵��
		lTimeOut			���������ʱʱ��(��Ϊ��λ)				(IN)
		lHandle				��ǰ�������ӵĲ������					(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		�������������ʱʱ��(��Ϊ��λ,Ĭ��1��)
*/
long __stdcall NP_SetNetTimeOut( long lTimeOut , long lHandle );

//----------------------------------------------------------------------------------

/*	
����˵��
		rc		: ָ����ʾ����Ƶ����.�������Ϊ0,��ʹ��ԭʼ��Ƶ�ߴ�	(IN)
				  ��ʾ,���򽫸�����������ʾ��ָ����������.
				  �������ܳ���ԭʼ��Ƶ�ĳߴ緶Χ.
	    lHandle	: ��ǰ�������ӵĲ������							(IN)
				  
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��Ƶ�ֲ��Ŵ���ʾ,��Ҫ��Ƶ�ѿ�ʼԤ��
*/
long __stdcall NP_SetVideoZoomRect(RECT rc,long lHandle);

//----------------------------------------------------------------------------------

/*
����˵��
		pszCamId			����ͷID						(OUT)
		pszCamName			����ͷ����						(OUT)
		pszServerAddr		ע���������ַ					(OUT)
		lServerPort			ע�������ͨѶ�˿�				(OUT)
		pszAlarmCenterAddr	�������ĵ�ַ					(OUT)
		plAlarmCenterPort	��������ͨѶ�˿�				(OUT)
		plMaxAlarmIn		�����õı�������������		(OUT)
		plMaxAlarmOut		�����õı������������		(OUT)
		plMdStart			����ѡ��: �ƶ���⿪ʼ			(OUT)
		plMdStop			����ѡ��: �ƶ����ֹͣ			(OUT)
		plVlStart			����ѡ��: ��Ƶ��ʧ��⿪ʼ		(OUT)
		plVlResume			����ѡ��: ��Ƶ��ʧ���ָ�		(OUT)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡHS�豸ע��������Ϣ
*/
long __stdcall NP_GetHSRegCfg(	char*	pszCamId,			char* pszCamName,
								  char*	pszServerAddr,		long* plServerPort,
								  char*	pszAlarmCenterAddr,	long* plAlarmCenterPort,
								  long*	plMaxAlarmIn ,		long* plMaxAlarmOut, 
								  long*	plMdStart,			long* plMdStop,
								  long*	plVlStart,			long* plVlResume,
								  long	lHandle  );
											 
//----------------------------------------------------------------------------------

/*
����˵��
		pszCamId			����ͷID						(IN)
		pszCamName			����ͷ��						(IN)
		pszServerAddr		ע���������ַ					(IN)
		lServerPort			ע�������ͨѶ�˿�				(IN)
		pszAlarmCenterAddr	�������ĵ�ַ					(IN)
		lAlarmCenterPort	��������ͨѶ�˿�				(IN)
		lMaxAlarmIn			�����õı�������������		(IN)
		lMaxAlarmOut		�����õı������������		(IN)
		lMdStart			����ѡ��: �ƶ���⿪ʼ			(IN)
		lMdStop				����ѡ��: �ƶ����ֹͣ			(IN)
		lVlStart			����ѡ��: ��Ƶ��ʧ��⿪ʼ		(IN)
		lVlResume			����ѡ��: ��Ƶ��ʧ���ָ�		(IN)
		lHandle				��ǰ�������ӵĲ������			(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		����HS�豸ע��������Ϣ,���óɹ����豸���Զ���������.
*/
long __stdcall NP_SetHSRegCfg(  char*	pszCamId,			char* pszCamName,
								char*	pszServerAddr,		long lServerPort,
								char*	pszAlarmCenterAddr,	long lAlarmCenterPort,
								long	lMaxAlarmIn ,		long lMaxAlarmOut, 
								long	lMdStart,			long lMdStop,
								long	lVlStart,			long lVlResume,
								long	lHandle  );			
	
//----------------------------------------------------------------------------------

/*
����˵��
		hHandle�����ž����
		NetMessageCallBack����Ϣ�ص�����
		pContext���ص���������
����ֵ	
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		������ʾ�ص��������ú�����NP_StartPreview�����,
		��NP_StopPreview���ú��Զ�ʧЧ��
		Ϊ�ͻ������ṩ����������Ϣ�Ļ��ᡣ
*/
long __stdcall NP_SetNetMessageCallBack(long( CALLBACK* NetMessageCallBack  ) ( long lHandle, long lMsg,long wParam, long lParam , PVOID pContext),  PVOID pContext , long lHandle);

/*
����˵��
		lVideoWndWidth	��Ƶ���ڵĿ��
		lVideoWndHeigth	��Ƶ���ڵĸ߶�
		rect			Ŀ����Σ�����Ƶ�������Ͻ�Ϊ����ԭ�㣩
		lZoomMode		1 �Ŵ� / 0 ��С
		lHandle			��ǰ�������ӵĲ������
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡԤ��λ������Ϣ
*/
long __stdcall NP_3DPositioning( long lVideoWndWidth , long lVideoWndHeigth , RECT rect, long lZoomMode, long lHandle );
//----------------------------------------------------------------------------------

/*
����˵��
		pszChn			��Ƶ������ͨ��									(OUT)
		pszIP			��Ƶ������IP									(IN)
����ֵ
		ִ�гɹ�����0������ʱ���ش����롣
����˵��
		��ȡ��Ƶ������ͨ��.�磺355�豸"0",365�豸"1/qcif,1/cif,1/d1,"
*/
long __stdcall NP_GetDevChn( char* pszChn , const char* pszIP);
long __stdcall NP_PausePlay(long	lHandle);
}		// end extern "C"

#endif

