//----------------------------------------------------------------------------------
//							Export_JSNP.h	   							
//						网络通讯库导出接口头文件						
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
#define WM_PACKETLOSS					WM_USER + 100	// RTP丢包自定义消息
#endif

#ifndef WM_VIDEOLOSS
#define WM_VIDEOLOSS					WM_USER + 101	// 视频丢失自定义消息
#endif

#ifndef WM_ALARM_INPUT
#define WM_ALARM_INPUT					WM_USER + 102	// 探头输入报警自定义消息
#endif

#ifndef WM_MOTIONDETECT
#define WM_MOTIONDETECT					WM_USER + 105	// 移动侦测自定义消息
#endif

#ifndef WM_ALARM_HOST
#define WM_ALARM_HOST					WM_USER + 108	// 报警主机报警
#endif


// 所有接口函数，执行成功时返回0或正值，负值表示错误。

#define ERR_NP_FUNCTION_NOT_EXIST		-100	// 函数不存在
#define ERR_NP_PARAMETER				-101	// 参数错误	
#define ERR_NP_CALL_ORDER				-102	// 调用顺序错误
#define ERR_NP_HANDLE_EXHAUSTED			-103	// 通讯句柄用尽
#define ERR_NP_PREVIEW_START			-104	// 预览失败
#define ERR_NP_DVS_CONNECT				-105	// 连接视频服务器失败
#define ERR_NP_DVS_AUTH					-106	// 视频服务器验证失败
#define ERR_NP_DVS_TYPE_INVALID			-107	// 无效的设备类型
#define ERR_NP_DVS_STREAM				-108	// 配置视频服务器流失败
#define ERR_NP_CREATE_DECODE			-109	// 创建解码器失败
#define ERR_NP_CREATE_DISPLAY			-110	// 创建显示设备失败
#define ERR_NP_CREATE_AUDIO				-111	// 创建音频设备失败
#define ERR_NP_SNAP						-112	// 抓图失败
#define ERR_NP_RECORD					-113	// 录像失败
#define ERR_NP_VOL_SET					-114	// 设置音频输出音量失败 
#define ERR_NP_VOL_GET					-115	// 获取音频输出音量失败
#define ERR_NP_DRAW_CALLBACK			-116	// 注册显示回调函数失败
#define ERR_NP_DECODE_CALLBACK			-117	// 注册解码数据回调函数失败
#define ERR_NP_SEARCH_DVS				-118	// 搜索设备失败
#define ERR_NP_DVSVER_GET				-119	// 获取设备版本信息失败
#define ERR_NP_PTZ						-120	// 云台控制操作失败
#define ERR_NP_PRESET_GET				-121	// 获取预置位失败
#define ERR_NP_PRESET_ADD				-122	// 添加预置位失败
#define ERR_NP_PRESET_DEL				-123	// 删除预置位失败
#define ERR_NP_PRESET_CALL				-124	// 调用预置位失败
#define ERR_NP_SENSOROUT_GET			-125	// 获取探头输出开关状态失败
#define ERR_NP_SENSOROUT_SET			-126	// 设置探头输出开关状态失败
#define ERR_NP_GUARD_GET				-127	// 获取看守位配置失败
#define ERR_NP_GUARD_SET				-128	// 设置看守位配置失败
#define ERR_NP_DEV_VOL_GET				-129	// 获取视频服务器声卡音量失败
#define ERR_NP_DEV_VOL_SET				-130	// 设置视频服务器声卡音量失败
#define ERR_NP_DEV_BPS_GET				-129	// 获取视频服务器码率配置失败
#define ERR_NP_DEV_BPS_SET				-130	// 设置视频服务器码率配置失败
#define ERR_NP_DEV_COLOR_GET			-131	// 获取视频服务器颜色配置失败
#define ERR_NP_DEV_COLOR_SET			-132	// 设置视频服务器颜色配置失败
#define ERR_NP_DEV_COM_GET				-133	// 获取视频服务器串口配置失败
#define ERR_NP_DEV_COM_SET				-134	// 设置视频服务器串口配置失败
#define ERR_NP_DEV_MD_GET				-135	// 获取视频服务器移动侦测配置失败
#define ERR_NP_DEV_MD_SET				-136	// 设置视频服务器移动侦测配置失败
#define ERR_NP_DEV_NET_GET				-137	// 获取视频服务器网络配置失败
#define ERR_NP_DEV_NET_SET				-138	// 设置视频服务器网络配置失败
#define ERR_NP_DEV_WIFI_GET				-139	// 获取视频服务器无线WIFI配置失败
#define ERR_NP_DEV_WIFI_SET				-140	// 设置视频服务器无线WIFI配置失败
#define ERR_NP_DEV_OSD_GET				-141	// 获取视频服务器OSD配置失败
#define ERR_NP_DEV_OSD_SET				-142	// 设置视频服务器OSD配置失败
#define ERR_NP_DEV_DEFAULT				-143	// 恢复视频服务器出厂默认值失败
#define ERR_NP_DEV_REBOOT				-144	// 重启视频服务器设备失败
#define ERR_NP_DEV_RESET				-145	// 重启视频服务器服务失败
#define ERR_NP_DEV_NAME_GET				-146	// 获取视频服务器设备名称失败
#define ERR_NP_DEV_NAME_SET				-147	// 设置视频服务器设备名称失败
#define ERR_NP_DEV_VE_INFO_GET			-148	// 获取视频服务器当前编码信息失败
#define ERR_NP_DEV_VE_INFO_SET			-149	// 设置视频服务器当前编码信息失败
#define ERR_NP_CHN_VE_INFO_GET			-150	// 获取视频服务器当前通道编码信息失败
#define ERR_NP_DEV_PORT_GET				-151	// 获取视频服务器通讯端口失败
#define ERR_NP_DEV_PORT_SET				-152	// 设置视频服务器通讯端口失败
#define ERR_NP_DEV_UPNP_GET				-153	// 获取视频服务器UPNP状态失败
#define ERR_NP_DEV_UPNP_SET				-154	// 设置视频服务器UPNP状态失败
#define ERR_NP_DEV_PPPOE_GET			-155	// 获取视频服务器PPPOE状态失败
#define ERR_NP_DEV_PPPOE_SET			-156	// 设置视频服务器PPPOE状态失败
#define ERR_NP_DEV_DDNS_GET				-157	// 获取视频服务器DDNS失败
#define ERR_NP_DEV_DDNS_SET				-158	// 设置视频服务器DDNS失败
#define ERR_NP_DEV_NTP_GET				-159	// 获取视频服务器NTP失败
#define ERR_NP_DEV_NTP_SET				-160	// 设置视频服务器NTP失败
#define ERR_NP_DEV_TIME_GET				-159	// 获取视频服务器时钟失败
#define ERR_NP_DEV_TIME_SET				-160	// 设置视频服务器时钟失败
#define ERR_NP_DEV_TIMEZONE_SET			-161	// 设置视频服务器时区失败
#define ERR_NP_DEV_VL_GET				-162	// 获取视频服务器视频丢失配置失败
#define ERR_NP_DEV_VL_SET				-163	// 设置视频服务器视频丢失配置失败
#define ERR_NP_DEV_AUTHMODE_GET			-164	// 获取视频服务器用户验证配置失败
#define ERR_NP_DEV_AUTHMODE_SET			-165	// 设置视频服务器用户验证配置失败
#define ERR_NP_DEV_USER_GET				-166	// 获取视频服务器用户失败
#define ERR_NP_DEV_USER_ADD				-167	// 添加用户失败
#define ERR_NP_DEV_USER_SET				-168	// 修改用户配置失败
#define ERR_NP_DEV_USER_DEL				-169	// 删除用户失败
#define ERR_NP_DEV_UPDATE_PACKET		-170	// 升级视频服务器包失败
#define ERR_NP_DEV_SENSOR_IN_GET		-171	// 获取视频服务器探头报警配置失败
#define ERR_NP_DEV_SENSOR_IN_SET		-172	// 设置视频服务器探头报警配置失败
#define ERR_NP_DEV_REC_STATUS_GET		-173	// 获取视频服务器前端录像状态配置失败
#define ERR_NP_DEV_DISK_STRATEGY_GET	-174	// 获取磁盘管理策略失败
#define ERR_NP_DEV_DISK_STRATEGY_SET	-175	// 设置磁盘管理策略失败
#define ERR_NP_DEV_REC_START			-176	// 控制视频服务器前端录像失败
#define ERR_NP_DEV_TRACK_GET			-177	// 获取视频服务器轨迹配置失败
#define ERR_NP_DEV_TRACK_SET			-178	// 设置视频服务器轨迹配置失败
#define ERR_NP_DEV_TRACK_CALL			-179	// 调用轨迹失败
#define ERR_NP_DEV_SEND_TP09_CMD		-180	// 发送透明端口指令失败
#define ERR_NP_DEV_AC_GET				-181	// 获取报警联动报警中心配置失败
#define ERR_NP_DEV_AC_SET				-182	// 设置报警联动报警中心配置失败
#define ERR_NP_DEV_EMAIL_GET			-183	// 获取报警联动邮件配置失败
#define ERR_NP_DEV_EMAIL_SET			-184	// 设置报警联动邮件配置失败
#define ERR_NP_DEV_FTP_GET				-185	// 获取报警联动FTP配置失败
#define ERR_NP_DEV_FTP_SET				-186	// 设置报警联动FTP配置失败
#define ERR_NP_DEV_SENSOROUT_GET		-187	// 获取报警联动探头输出配置失败
#define ERR_NP_DEV_SENSOROUT_SET		-188	// 设置报警联动探头输出配置失败
#define ERR_NP_DEV_REC_GET				-189	// 获取报警联动录像配置失败
#define ERR_NP_DEV_REC_SET				-190	// 设置报警联动录像配置失败
#define ERR_NP_DEV_SENSORIN_CA_GET		-191	// 获取探头输入报警联动配置失败
#define ERR_NP_DEV_SENSORIN_CA_SET		-192	// 设置探头输入报警联动配置失败
#define ERR_NP_DEV_MD_CA_GET			-193	// 获取移动侦测报警联动配置失败
#define ERR_NP_DEV_MD_CA_SET			-194	// 设置移动侦测报警联动配置失败
#define ERR_NP_DEV_VL_CA_GET			-195	// 获取视频丢失报警联动配置失败
#define ERR_NP_DEV_VL_CA_SET			-196	// 设置视频丢失报警联动配置失败
#define ERR_NP_DEV_VOD_SEARCH_START		-197	// 开始搜索前端录像文件失败
#define ERR_NP_DEV_VOD_SEARCH_STOP		-198	// 停止搜索前端录像文件失败
#define ERR_NP_DEV_HS_GET				-199	// 获取HS设备注册配置信息
#define ERR_NP_DEV_HS_SET				-200	// 设置HS设备注册配置信息


#ifndef	TRACK_MAX			
#define	TRACK_MAX						16				// 轨迹最大个数
#endif

#ifndef PRESET_MAX
#define PRESET_MAX						256				// 预置位最大个数
#endif

#ifndef PRESET_NAME_MAX
#define PRESET_NAME_MAX					10				// 预置位名称最大字节数
#endif

#ifndef	STAYTIME_MAX			
#define	STAYTIME_MAX					1800			// 停留间隔最大时间(秒)
#endif

#ifndef	WM_RELEASE_BROADCAST
#define WM_RELEASE_BROADCAST			WM_USER + 107	// 释放音频广播资源消息
#endif

// 云台控制类型
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
	NP_VENC_TYPE_720P	= 3,						// 720p高清
	NP_VENC_TYPE_1080P	= 4,						// 1080p高清
} NP_VEncode_Type_E;

typedef struct NP_VEncode_Type_S_EX					// 支持的编码类型结构数组
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

typedef struct NP_VEncode_Format_S_EX				// 支持的视频压缩格式结构数组
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

typedef struct NP_VI_Standard_S_EX					// 支持的视频制式结构数组
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

typedef enum NP_E_DevRecord_Status				// 前端录像 状态
{
	NP_E_DevRecord_Stop	= 0,					// 前端录像 停止
	NP_E_DevRecord_Manual	= 1,				// 前端录像 手动录像
	NP_E_DevRecord_Alarm	= 2,				// 前端录像 报警录像
} NP_E_DevRecord_Status;

typedef enum NP_E_DevDiskStrategy				// 前端磁盘管理策率
{
	NP_E_Devdiskstrategy_Stop	= 0,			// 前端磁盘管理策率 磁盘小于保留控件时停止录像
	NP_E_Devdiskstrategy_Delete	= 1,			// 前端磁盘管理策率 磁盘小于保留控件时删除文件
} NP_E_DevDiskStrategy;

typedef enum NP_E_DevFileStrategy				// 前端文件长度管理策率
{
	NP_E_DevFileStrategy_Stop	= 0,			// 前端文件长度管理策率 文件达到最大长度时停止录像
	NP_E_DevFileStrategy_Switch	= 1,			// 前端文件长度管理策率 文件达到最大长度时切换文件
} NP_E_DevFileStrategy;

typedef enum NP_Dev_Record_Type_E				// 前端录像文件录像类型
{
	NP_Dev_RecType_Manual		= 0,			// 手动录像
	NP_Dev_RecType_Schedule		= 1,			// 定时录像
	NP_Dev_RecType_Alarm		= 2,			// 报警录像
	NP_Dev_RecType_All			= 3,			// 全部录像
} NP_Dev_Record_Type_E;

typedef enum NP_Dev_Vod_Type_E					// 搜索设备前端录像文件结构
{
	NP_Dev_Vod_Begin	= 0,					// 开始包
	NP_Dev_Vod_File		= 1,					// 过程包
	NP_Dev_Vod_End		= 2						// 结束包
}NP_Dev_Vod_Type_E;

typedef struct NP_Dev_DiskInfo_S				// 前端磁盘管理
{
	char		szDiskName[MAX_PATH];			// 磁盘名称
	DWORD		dwDiskTotal;					// 磁盘总容量(M单位)
	DWORD		dwDiskFree;						// 磁盘剩余容量(M单位)
}NP_Dev_DiskInfo_S;

typedef struct NP_Dev_DiskInfo_S_EX				// 前端磁盘结构数组
{
	int					nDiskCount;
	NP_Dev_DiskInfo_S	eDisk[_MAX_PATH];
}NP_Dev_DiskInfo_S_EX;

#ifndef _FRAME_INFO_
#define _FRAME_INFO_
typedef struct
{ 
	long		nWidth;						// 画面宽，单位为像素，如果是音频数据则为0
	long		nHeight;					// 画面高，单位为像素，如果是音频数据则为0
	long		nStamp;						// 时标信息，单位毫秒
	long		nType;						// 数据类型，见NP_SetDecodeCallBack函数说明
	long		nFrameRate;					// 编码时产生的图像帧率
}FRAME_INFO; 
#endif

#ifndef _RAW_INFO_
#define _RAW_INFO_
typedef struct
{ 
	long		nWidth;						// 画面宽，单位为像素，如果是音频数据则为0
	long		nHeight;					// 画面高，单位为像素，如果是音频数据则为0
	long		nStamp;						// 时标信息，单位毫秒
	long		nType;						// 数据类型，见NP_SetRawDataCallBack函数说明
	long		nVideoIFrame;				// 当前帧是否为视频I帧(1. 该参数只有在nType为视频类型时有效 2. 非0为I帧,否则为P帧,无B帧)
	long		nFrameRate;					// 编码时产生的图像帧率
}RAW_INFO; 
#endif

typedef struct
{
	char		szVideoComp[MAX_PATH];		// 视频编码器描述
	long		lVideoFormat;				// 视频编码类型
	long		lVideoWidth;				// 视频画面宽，单位为像素
	long		lVideoHeight;				// 视频画面高，单位为像素
	long		lVideoFrame;				// 视频帧率
	
	long		lAudioChns;					// 音频通道数
	long		lAuidoRate;					// 音频采样率
	long		lAudioBits;					// 音频比特率
	long		lAudioFormat;				// 音频格式
}AV_INFO;

typedef struct WEEK_TIME_S								// 布防规则
{	
	E_WEEK_DATE	emDate;									// 布防日期
	BOOL		bEnable;								// 启用该日期布防
	int			nTime;									// 布防时间段
}WEEK_TIME_S;

typedef struct WEEK_TIME_S_EX							// 布防规则结构数组
{
	WEEK_TIME_S	eWeekTime[7];							// 布防规则
}WEEK_TIME_S_EX;

typedef struct WIFI_AP_S								// WIFI设备结构
{
	char		szSSID[MAX_PATH];						// 设备名称
	BOOL		bEncry;									// 是否启用了加密
}WIFI_AP_S;

typedef struct WIFI_AP_S_EX								// WIFI设备结构数组
{
	int			nAPCount;
	WIFI_AP_S	eWifiAP[_MAX_PATH];
}WIFI_AP_S_EX;

typedef struct DDNS_PROVIDER_S							// DDNS提供商结构
{
	char		szDDNSProvider[_MAX_PATH];
}DDNS_PROVIDER_S;

typedef struct DDNS_PROVIDER_S_EX						// DDNS提供商结构数组
{
	int				nDdnsCount;
	DDNS_PROVIDER_S	eDDNS[_MAX_PATH];
}DDNS_PROVIDER_S_EX;

typedef struct PTZ_NAME_S								// 支持的云台控制协议
{
	char		szPtzName[_MAX_PATH];
}PTZ_NAME_S;

typedef struct PTZ_NAME_S_EX							// 支持的云台控制协议结构数组
{
	int				nPtzCount;
	PTZ_NAME_S		ePtzName[_MAX_PATH];
}PTZ_NAME_S_EX;

typedef struct NP_VEncode_Chn_S
{
	NP_VEncode_Format_E	emVEncFormat;					// 视频压缩类型 (H264,MPEG4 ...)
	NP_VEncode_Type_E	emVEncType;						// 编码器类型	(QCIF,CIF,D1 ...)
	NP_VI_Standard_E	emVStandard;					// 视频制式		(PAL NTSC AUTO ...)
	UINT				nVI;							// 视频输入通道索引(0 -- n)
} NP_VEncode_Chn_S;

typedef struct NP_VEncode_Chn_S_EX						// 编码通道结构体数组
{	
	int					nVECount;
	NP_VEncode_Chn_S	eVEncode[_MAX_PATH];
} NP_VEncode_Chn_S_EX;

typedef struct PRESET_INFO_S							// 预置位结构
{
	UINT		nPresetID;								// 预置位点
	char		szPresetName[10];						// 预置位名称,最大支持10个字符
	UINT		nStayTime;								// 停留时间
} PRESET_INFO_S;

typedef struct PRESET_INFO_S_EX							// 预置位结构数组
{
	int					nPresetCount;
	PRESET_INFO_S		ePreset[_MAX_PATH];
}PRESET_INFO_S_EX;

typedef struct NP_User_Dev_S							// 设备用户结构
{
	char			szUser[MAX_PATH];					// 用户名
	char			szPassword[MAX_PATH];				// 密码
	NP_User_Power_E	emUserPower;						// 用户所在分组
}NP_User_Dev_S;

typedef struct NP_User_Dev_S_EX							// 设备用户结构数组
{
	int				nUserCount;
	NP_User_Dev_S	eUser[_MAX_PATH];
}NP_User_Dev_S_EX;

typedef struct NP_Dev_Vod_Search_S						// 搜索前端设备录像文件结构
{
	int					nFlag;							// $ Interleave标志位 36
	NP_Dev_Vod_Type_E	emDevVodType;					// 搜索前端设备录像文件包类型
	char				szFilePath[MAX_PATH];			// 前端文件路径
}NP_Dev_Vod_Search_S;


// 视频显示回调函数
typedef  long(CALLBACK* DrawCallBack)		(long hHandle, HDC hDC, long lParam);		

// 原始音视频数据回调函数
typedef  long ( CALLBACK * DecodeCallBack ) ( long lHandle, const char * pBuf, long  nSize, FRAME_INFO * pFrameInfo, long lParam  );

// 网络接收到的完整编码音视频帧数据回调函数
typedef  long ( CALLBACK * RawDataCallBack ) ( long lHandle, const char * pBuf, long  nSize, RAW_INFO * pDataInfo, long lParam  );

// 搜索前端录像文件回调函数
typedef  long ( CALLBACK * VodSearchCallBack)(NP_Dev_Vod_Search_S* pDevVod, PVOID pParam);	

// 网络消息回调函数
typedef  long( CALLBACK* NetMessageCallBack  ) ( long lHandle, long lMsg,long wParam, long lParam , PVOID pContext);


typedef struct NP_VOD_SEARCH_S
{
	VodSearchCallBack			pCallBackFuncVod;		// 回调函数
	PVOID						pCallBackVod;			// 回调参数指针
	DWORD						dwTimeStart;			// 开始时间
	DWORD						dwTimeStop;				// 结束时间
	NP_Dev_Record_Type_E		emDevRecordType;		// 前端录像文件类型
}NP_VOD_SEARCH_S;


//----------------------------------------------------------------------------------


extern "C"
{
//----------------------------------------------------------------------------------

/*
参数说明
		sFunctionName：	接口名字，指本文档中定义的接口函数名。
						不含NP_，如NP_Free应为Free。
		lCanUse：		1表示可用，其它不可用
返回值
		执行成功返回0，错误时返回错误码。
		可能的错误码有：ERR_NP_FUNCTION_NOT_EXIST		
函数说明
		查询接口是否可用
*/
long __stdcall NP_QueryFunction( const char * sFunctionName, long * lCanUse );

//----------------------------------------------------------------------------------

/*
参数说明
		空
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		初始化网络通讯库	
*/
long __stdcall NP_Initial( );

//----------------------------------------------------------------------------------

/*
参数说明
		空
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		释放网络通讯库。
*/
long __stdcall NP_Free();

//----------------------------------------------------------------------------------

/*
参数说明
		lVer：前16位为主版本号，后16位为副版本号		(OUT)
返回值
		执行成功返回0，错误时返回自定义错误码。
函数说明
		获取通讯库版本号
*/
long __stdcall NP_GetVersion( long * lVer );

//----------------------------------------------------------------------------------

/*
参数说明
		pszDVSAddr	视频服务器设备通讯地址(支持DDNS或IP地址)								(IN)
		pszChn		视频服务器通道		("1/cif" 或 "1/d1" 或 "1/720p" 或 "1/1080p")		(IN)
		nPort		视频服务器通讯端口		(默认554,范围 1 ~~ 65535)						(IN)
		nConnType	连接视频服务器协议类型	(0: UDP 1: TCP 2: MUDP)							(IN)
		pszUser		登陆视频服务器的用户名	(0 ~~ 256字节)									(IN)
		pszPwd		登陆视频服务器的密码	(0 ~~ 256字节)									(IN)
		hWndVideo	显示视频的窗口句柄				(句柄为空时不进行解码和显示输出)		(IN)
		hWndMsg		接收网络传输中消息的窗口句柄	(句柄为空时将无法接收到消息)			(IN)
		plHandle	返回当前网络连接的操作句柄												(OUT)

返回值
		执行成功返回0，错误时返回错误码。
函数说明
		连接视频服务器并预览
*/	
long __stdcall NP_StartPreview( const char* pszDVSAddr,const char* pszChn,int nPort,
								 int nConnType,const char* pszUser,const char* pszPwd, 
								 long hWndVideo,long hWndMsg,long* plHandle );

//----------------------------------------------------------------------------------
/*
参数说明
		plHandle	当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		断开与视频服务器的连接,并释放当前句柄
*/
long __stdcall NP_StopPrevew( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		sFileName		抓拍图片全路径名称			(c:\Pic\20090510.jpg)					(IN)
		bIsJPEG			是否按JPEG格式保存文件		(0: BMP格式保存 非0: JPEG格式保存)		(IN)
		nQuality		图片质量					(仅对JPEG文件有效,范围从0到100)			(IN)
		lHandle			当前网络连接的操作句柄												(IN)
返回值		
		执行成功返回0，错误时返回错误码。
函数说明
		视频抓拍
*/
long __stdcall NP_SnapShot( const char* sFileName, int bIsJPEG,int nQuality,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		sFileName		录像文件全路径名成			(c:\video\20090510.mp4)	(IN)
		lHandle			当前网络连接的操作句柄								(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		开始本地录像
*/
long __stdcall NP_StartRecord( const char* sFileName,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle			当前网络连接的操作句柄				(IN)	
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		停止本地录像
*/
long __stdcall NP_StopRecord( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lRecording		是否正在录像						(OUT)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		当前通道是否在录像
*/
long __stdcall NP_IsRecording( long* lRecording,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lPreview		是否正在预览						(OUT)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		当前通道是否在预览
*/
long __stdcall NP_IsPreview( long* lPreview,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lAudioOut		是否正在音频输出					(OUT)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		当前通道是否在音频输出
*/
long __stdcall NP_IsAudioOut( long* lAudioOut,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle			当前网络连接的操作句柄				(IN)	
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		刷新最后一帧视频
*/
long __stdcall NP_Refresh( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lVolume			音量的值，范围0-0XFFFF				(IN)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置音频输出设备音量大小
*/
long __stdcall NP_SetVolume( long lVolume ,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lVolume			音量的值，范围0-0XFFFF				(OUT)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取音频输出设备音量大小
*/
long __stdcall NP_GetVolume( long* lVolume,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		hWndMsg			接收语音对讲消息窗口句柄			(IN)
		lStart			开始或停止语音对讲					(IN)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		处理语音对讲模块
*/
long __stdcall NP_DealAudioBroadCast(long hWndMsg,long lStart,long lHandle);

//----------------------------------------------------------------------------------

/*
参数说明
		lStartAudioOut	非0: 开始音频输出 0: 停止音频输出	(IN)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		开始或停止音频输出
*/
long __stdcall NP_StartAudioOut( long lStartAudioOut,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		空	
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		释放语音对讲模块资源
*/
long __stdcall NP_UnInitAudioBroadCast();

//----------------------------------------------------------------------------------

/*
参数说明
		idx：		表示回应消息的类型，1：文件播放结束，2：抓图结束，				(IN)
										3：录像结束	 4: 实时流网络传输异常中断
		hWnd：		窗口句柄														(IN)
		lMsg：		消息ID															(IN)
		lHandle		当前网络连接的操作句柄											(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置回应消息
		对于文件结束消息LPARAM为lHandle，WPARAM无意义；
		对于抓图结束消息LPARAM为lHandle，WPARAM为文件名所在字符串地址(const char*)。
		对于录像结束消息LPARAM为lHandle，WPARAM为文件名所在字符串地址(const char*)。
*/
long __stdcall NP_SetMessage( long idx, long hWnd, long lMsg ,long lHandle);

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle			播放句柄。				(IN)
		DrawCallBack	显示回调函数			(IN)
		lParam			回调函数参数			(IN)
返回值	
		执行成功返回0，错误时返回错误码。
函数说明
		设置显示回调函数，该函数在NP_StartPreview后调用,
		在NP_StopPreview调用后自动失效。
		用来获得当前解码器绘制的设备上下文，提供客户程序在视频上叠加内容的机会。

显示回调函数参数说明
		lHandle			播放句柄。
		hDC				设备上下文。
		lParam			客户程序在调用HX_SetDrawCallBack时指定的回调函数参数。
*/
long __stdcall NP_SetDrawCallBack( long( CALLBACK* DrawCallBack  ) ( long lHandle, HDC hDC, long lParam ), long lParam ,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明	
		lHandle			当前网络通讯句柄		(IN)
		DecodeCallBack	回调函数				(IN)
		lParam			回调函数参数			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置解码回调函数，该回调返回数据为原始音视频数据.
		该函数在NP_StartPreview后调用,在NP_StopPrevew调用后自动失效。
		解码部分不控制播放速度，只要客户程序从回调函数中返回，解码器就会解码下一部分数据。

	回调函数的参数说明：
	lHandle：	当前网络通讯句柄
	pBuf：		解码后的音视频数据
	nSize：		解码后的音视频数据长度
	lParam：	客户程序在调用HX_SetDecodeCallBack时指定的回调函数参数
	pFrameInfo：图像和声音信息FRAME_INFO结构定义如下：
	typedef struct
	{ 
		long nWidth;		// 画面宽，单位为像素，如果是音频数据则为0
		long nHeight;		// 画面高，单位为像素，如果是音频数据则为0
		long nStamp;		// 时标信息，单位毫秒
		long nType;			// 数据类型，见下表
		long nFrameRate;	// 编码时产生的图像帧率
	}FRAME_INFO;  

	数据类型表
	数据类型		简写						说明
	1	AUDIO16		音频。		采样率16khz，单声道，每个采样点16位表示。
	2	RGB32		视频。		每像素4字节，排列方式与位图相似，"BGR0",第一个像素位于图像左下角
	3	YV12		视频，		yv12格式。排列顺序"Y0Y1……"、"V0V1……"、"U0-U1……"
	4	UYVY		视频，		uyvy格式。排列顺序"U0Y0V0Y1U2Y2V2Y3 … …"，第一个像素位于图像左上角
	5	YUV420		视频，		YUV420格式。排列顺序"Y0Y1……"、"U0-U1……"、"V0V1……"
	6	yuy2、yuyv	视频，		yuy2或yuyv格式。排列顺序"Y0 U0 Y1 V0 Y2 U2 Y3 V2… …"，第一个像素位于图像左上角。
	7	g711		音频,		采样率8khz，单声道，每个采样点16位。
	8	amr			音频,		采样率8khz，单声道，每个采样点16位。
*/
long __stdcall NP_SetDecodeCallBack( long ( CALLBACK * DecodeCallBack ) ( long lHandle, const char * pBuf, long  nSize, FRAME_INFO * pFrameInfo, long lParam  ), long lParam  ,long lHandle);

//----------------------------------------------------------------------------------

/*
参数说明	
		lHandle			当前网络通讯句柄		(IN)
		RawDataCallBack	回调函数				(IN)
		lParam			回调函数参数			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置网络接收到的完整编码音视频帧数据回调函数
		该函数在NP_StartPreview后调用,在NP_StopPrevew调用后自动失效。
		解码部分不控制播放速度，只要客户程序从回调函数中返回，解码器就会解码下一部分数据。

	回调函数的参数说明：
	lHandle		当前网络通讯句柄
	pBuf		压缩的音视频数据
	nSize		压缩的音视频数据长度
	lParam		客户程序在调用NP_SetRawDataCallBack时指定的回调函数参数
	pDataInfo	图像和声音信息RAW_INFO结构定义如下：
	typedef struct
	{ 
		long nWidth;		// 画面宽，单位为像素，如果是音频数据则为0
		long nHeight;		// 画面高，单位为像素，如果是音频数据则为0
		long nStamp;		// 时标信息，单位毫秒
		long nType;			// 数据类型，见下表
		long nVideoIFrame;	// 当前帧是否为视频I帧(1. 该参数只有在nType为视频类型时有效 2. 非0为I帧,否则为P帧,无B帧)				
		long nFrameRate;	// 编码时产生的图像帧率
	}NP_RAW_INFO;  

	数据类型表
	数据类型		简写						说明
	1			CODEC_ID_H264				H264视频编码数据
	2			CODEC_ID_MPEG4				MPEG4视频编码数据
	3			CODEC_ID_MJPEG				MJPEG视频编码数据
	10			CODEC_ID_PCM_MULAW			MULAW音频编码
	11			CODEC_ID_AMR				AMR音频编码
*/
long __stdcall NP_SetRawDataCallBack( long ( CALLBACK * RawDataCallBack ) ( long lHandle, const char * pBuf, long  nSize, RAW_INFO * pDataInfo, long lParam  ), long lParam  ,long lHandle);

//----------------------------------------------------------------------------------

/*
参数说明
		emType		云台控制类型					(IN)
		lStart		非0: 开始  0: 停止				(IN)
		lPtzSpeed	云台移动速度	1 ~~ 63 默认47	(IN)
		lHandle		当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		云台控制
*/
long __stdcall NP_PTZControl( NP_PTZ_Type emType , long lStart , long lPtzSpeed , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		nStartSearch	0:开始搜索 1:停止搜索					(IN)
		hWnd			接收搜索视频服务器设备信息的窗口句柄	(IN)
		lMsg			消息类型								(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		开始或停止搜索视频服务器
*/
long __stdcall NP_SearchDVS( int nStartSearch ,long hWnd,long lMsg );

//----------------------------------------------------------------------------------

/*
参数说明
		pAVInfo			当前通道音视频配置信息		(OUT)
		lHandle			当前网络连接的操作句柄		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取当前通道音视频配置信息
*/
long __stdcall NP_GetAVInfo( AV_INFO* pAVInfo , long lHandle );
//----------------------------------------------------------------------------------

/*
参数说明
		pPresets		预置位配置数组				(OUT)
		lHandle			当前网络连接的操作句柄		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取预置位配置信息
*/
long __stdcall NP_GetPresetInfo( PRESET_INFO_S_EX* pPresets , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lPresetID		预置位ID		(1 ~~ 256)							(IN)
		pszPresetName	预置位名称		(名称字符串最大10个字节,可以为空)	(IN)
		lHandle			当前网络连接的操作句柄								(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		添加预置位
*/
long __stdcall NP_AddPreset( long lPresetID , const char* pszPresetName , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lPresetID		预置位ID		(1 ~~ 256)		(IN)
		lHandle			当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		删除预置位
*/
long __stdcall NP_DelPreset( long lPresetID , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lPresetID		预置位ID		(1 ~~ 256)		(IN)
		lHandle			当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		调用预置位
*/
long __stdcall NP_CallPreset( long lPresetID , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plSensorOut1	探头输出1开关状态			(1为打开/0位关闭)	(OUT)
		plSensorOut2	探头输出2开关状态			(1为打开/0位关闭)	(OUT)
		lHandle			当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取预置位配置信息
*/
long __stdcall NP_GetSensorOutState( long* plSensorOut1 , long* plSensorOut2 , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lSensorOut1		探头输出1开关状态			(1为打开/0位关闭)	(IN)
		lSensorOut2		探头输出2开关状态			(1为打开/0位关闭)	(IN)
		lHandle			当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取预置位配置信息
*/
long __stdcall NP_SetSensorOutState( long lSensorOut1 , long lSensorOut2 , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plEnable		是否启用看守位				(1为启用/0位关闭)	(OUT)
		plTime			归看守位时间间隔			(1 ~~ 300 分钟)		(OUT)
		plPerp			看守位对应的预置位			(1 ~~ 256)			(OUT)
		lHandle			当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取看守位配置
*/
long __stdcall NP_GetGuarderInfo( long* plEnable , long* plTime , long* plPerp , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lEnable			是否启用看守位				(1为启用/0位关闭)	(IN)
		lTime			归看守位时间间隔			(1 ~~ 300 分钟)		(IN)
		lPerp			看守位对应的预置位			(1 ~~ 256)			(IN)
		lHandle			当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取看守位配置
*/
long __stdcall NP_SetGuarderInfo( long lEnable , long lTime , long lPerp , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plEnalbe		是否启用音频输入	(0:禁用 1:启用)		(OUT)
		lHandle			当前网络连接的操作句柄
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器声卡输入源状态(启用/禁止)
*/
long __stdcall NP_GetDevAudioInState( long* plEnalbe , long lHandle );

//----------------------------------------------------------------------------------


/*
参数说明
		lEnalbe			是否启用音频输入	(0:禁用 1:启用)		(IN)
		lHandle			当前网络连接的操作句柄
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器声卡输入源状态(启用/禁止)
*/
long __stdcall NP_SetDevAudioInState( long lEnalbe , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plCodec			音频编码类型,见结构NP_AUDIO_CODEC_FORMAT_E	(OUT)
		lHandle			当前网络连接的操作句柄
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取音频编码类型,暂时只支持NP_AUDIO_CODEC_FORMAT_G711U和NP_AUDIO_CODEC_FORMAT_AMRNB
*/
long __stdcall NP_GetDevAudioCodecType( NP_AUDIO_CODEC_FORMAT_E* plCodec , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lCodec			音频编码类型,见结构NP_AUDIO_CODEC_FORMAT_E	(IN)
		lHandle			当前网络连接的操作句柄
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置音频编码类型,暂时只支持NP_AUDIO_CODEC_FORMAT_G711U和NP_AUDIO_CODEC_FORMAT_AMRNB
*/
long __stdcall NP_SetDevAudioCodecType( NP_AUDIO_CODEC_FORMAT_E lCodec , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plVolumeIn		视频服务器声卡输入音量		(0 ~~ 100)	(OUT)
		plVolumeOut		视频服务器声卡输出音量		(0 ~~ 100)	(OUT)
		lHandle			当前网络连接的操作句柄					(IN)
返回值	
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器声卡音量
*/
long __stdcall NP_GetDevVolume( long* plVolumeIn , long* plVolumeOut , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lVolumeIn		视频服务器声卡输入音量		(0 ~~ 100)		(IN)
		lVolumeOut		视频服务器声卡输出音量		(0 ~~ 100)		(IN)
		lHandle			当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器声卡音量
*/
long __stdcall NP_SetDevVolume( long lVolumeIn , long lVolumeOut , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plBitrate			视频码率				(单位kbit)						(OUT)
		plMaxBitrate		视频最大码率			(单位kbit)						(OUT)
		plVBR				变码/恒码率模式			(1: 变码率模式 0: 恒码率模式)	(OUT)
		plFrameRatePrefer	帧率优先/码率优先		(1: 帧率优先 0: 码率优先)		(OUT)
		plFrameRate			视频帧率												(OUT)
		plIFrameInterval	I帧间隔													(OUT)
		lHandle				当前网络连接的操作句柄									(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器码率相关信息
*/
long __stdcall NP_GetBpsInfo( long* plBitrate , long* plMaxBitrate , long* plVBR , long* plFrameRatePrefer , 
							 long* plFrameRate , long* plIFrameInterval , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lBitrate			视频码率				(单位kbit)						(IN)
		lVBR				变码/恒码率模式			(1: 变码率模式 0: 恒码率模式)	(IN)
		lFrameRatePrefer	帧率优先/码率优先		(1: 帧率优先 0: 码率优先)		(IN)
		lFrameRate			视频帧率												(IN)
		lIFrameInterval		I帧间隔													(IN)
		lHandle				当前网络连接的操作句柄									(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器码率相关信息
*/
long __stdcall NP_SetBpsInfo( long lBitrate , long lVBR , long lFrameRatePrefer , 
							 long lFrameRate , long lIFrameInterval , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plBrightness		亮度			(0 ~~ 255)		(OUT)
		plContrast			对比度			(0 ~~ 255)		(OUT)
		plSaturation		饱和度			(0 ~~ 255)		(OUT)
		plHue				色度			(0 ~~ 255)		(OUT)
		plBrightnessDef		默认亮度		(0 ~~ 255)		(OUT)
		plContrastDef		默认对比度		(0 ~~ 255)		(OUT)
		plSaturationDef		默认饱和度		(0 ~~ 255)		(OUT)
		plHueDef			默认色度		(0 ~~ 255)		(OUT)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器颜色配置
*/
long __stdcall NP_GetVideoColor( long* plBrightness , long* plContrast , 
							   long* plSaturation , long* plHue ,
							   long* plBrightnessDef , long* plContrastDef , 
							   long* plSaturationDef , long* plHueDef ,															   
							   long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lBrightness			亮度			(0 ~~ 255)		(IN)
		lContrast			对比度			(0 ~~ 255)		(IN)
		lSaturation			饱和度			(0 ~~ 255)		(IN)
		lHue				色度			(0 ~~ 255)		(IN)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器颜色配置
*/
long __stdcall NP_SetVideoColor( long lBrightness , long lContrast , 
								   long lSaturation , long lHue ,										   
								   long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plCom				串口		(0:RS232 或 1:RS485)			(OUT)
		plStop				停止位		(1 或 2)						(OUT)
		plData				数据位		(5、6、7、8)					(OUT)
		pszCheckBit			校验位		(N、O、E、S)					(OUT)
		plBaud				波特率		(300、600、1200、2400、4800、	(OUT)
										9600、14400、19200、38400
										56000、57600、115200、
										128000、256000)
		plAddr				云台地址	(1 ~~ 255的整数)				(OUT)
		pszProt				云台协议	(非空且小于30个字节字符串)		(OUT)	
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器串口配置
*/
long __stdcall NP_GetComSet( long* plCom , long* plStop , 
							   long* plData , char* pszCheckBit ,
							   long* plBaud , long* plAddr , 
							   char* pszProt , long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lCom				串口		(0:RS232 或 1:RS485)			(IN)
		lStop				停止位		(1 或 2)						(IN)
		lData				数据位		(5、6、7、8)					(IN)
		pszCheckBit			校验位		(N、O、E、S)					(IN)
		lBaud				波特率		(300、600、1200、2400、4800、	(IN)
										9600、14400、19200、38400
										56000、57600、115200、
										128000、256000)
		lAddr				云台地址	(1 ~~ 255的整数)				(IN)
		pszProt				云台协议	(非空且小于30个字节字符串)		(IN)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器串口配置
*/
long __stdcall NP_SetComSet( long lCom , long lStop , 
						   long lData , char* pszCheckBit ,
						   long lBaud , long lAddr , 
						   char* pszProt , long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plMDEnable			是否启用移动侦测		(OUT)
		plMDLeft			移动侦测区域左坐标		(OUT)
		plMDTop				移动侦测区域顶坐标		(OUT)
		plMDWidth			移动侦测区域宽度		(OUT)
		plMDHeight			移动侦测区域高度		(OUT)
		plMDSensitivity		移动侦测灵敏度			(OUT)
		pWeekTimes			移动侦测布防规则		(OUT)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器移动侦测配置
*/
long __stdcall NP_GetMDInfo( long* plMDEnable , long* plMDLeft , long* plMDTop ,
						   long* plMDWidth, long* plMDHeight, long* plMDSensitivity ,
						   WEEK_TIME_S_EX* pWeekTimes, 
						   long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lMDEnable			是否启用移动侦测		(IN)
		lMDLeft				移动侦测区域左坐标		(IN)
		lMDTop				移动侦测区域顶坐标		(IN)
		lMDWidth			移动侦测区域宽度		(IN)
		lMDHeight			移动侦测区域高度		(IN)
		lMDSensitivity		移动侦测灵敏度			(IN)
		emWeekTimes			移动侦测布防规则		(IN)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器移动侦测配置
*/
long __stdcall NP_SetMDInfo( long lMDEnable , long lMDLeft , long lMDTop ,
						   long lMDWidth, long lMDHeight, long lMDSensitivity ,
						   WEEK_TIME_S_EX emWeekTimes,
						   long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszIP				视频服务器IP地址		(1 ~~ 260个字节的字符串)	(OUT)
		pszMac				视频服务器MAC地址		(1 ~~ 260个字节的字符串)	(OUT)
		pszSubMask			视频服务器子网掩码		(1 ~~ 260个字节的字符串)	(OUT)
		pszGateWay			视频服务器网关			(1 ~~ 260个字节的字符串)	(OUT)
		pszDNS				视频服务器域名服务器	(1 ~~ 260个字节的字符串)	(OUT)
		plDhcp				视频服务器DHCP是否启用	(1: 启用 0:关闭)			(OUT)
		lHandle				当前网络连接的操作句柄								(IN)
返回值	
		执行成功返回0，错误时返回错误码。
函数说明
		获取网络配置信息
*/
long __stdcall NP_GetNetInfo( char* pszIP, char* pszMac, char* pszSubMask,
							char* pszGateWay, char* pszDNS, long* plDhcp,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszIP				视频服务器IP地址		(1 ~~ 260个字节的字符串)(IN)
		pszMac				视频服务器MAC地址		(1 ~~ 260个字节的字符串)(IN)
		pszSubMask			视频服务器子网掩码		(1 ~~ 260个字节的字符串)(IN)
		pszGateWay			视频服务器网关			(1 ~~ 260个字节的字符串)(IN)
		pszDNS				视频服务器域名服务器	(1 ~~ 260个字节的字符串)(IN)
		lDhcp				视频服务器DHCP是否启用	(1: 启用 0:关闭)		(IN)
		lHandle				当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置网络配置信息
*/
long __stdcall NP_SetNetInfo( char* pszIP, char* pszMac, char* pszSubMask,
							char* pszGateWay, char* pszDNS, long lDhcp,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plExist				无线Wifi模块是否存在	(1: 存在 0: 不存在)		(OUT)
		pszName				无线Wifi名称									(OUT)
		plEnable			是否启用无线Wifi模块	(1: 启用 0: 不启用)		(OUT)
		pszIP				无线WifiIP地址			(1 ~~ 260个字节的字符串)(OUT)
		pszMask				无线WifiIP子网掩码		(1 ~~ 260个字节的字符串)(OUT)
		pszMac				无线Wifi MAC地址		(1 ~~ 260个字节的字符串)(OUT)
		pszGateWay			无线Wifi网关			(1 ~~ 260个字节的字符串)(OUT)
		pszSSID				无线Wifi网络名			(1 ~~ 260个字节的字符串)(OUT)
		plSecEn				安全设置 打开/关闭								(OUT)
		plSecType			安全类型 wep或wpa								(OUT)
		plWepOpt			网络验证 开放式或共享式							(OUT)
		plWepIndex			密钥索引 1 ~~ 4									(OUT)
		plWepType			密钥长度										(OUT)
		pszWepPass			密钥					(1 ~~ 260个字节的字符串)(OUT)
		plDhcp				Wifi DHCP 打开/关闭								(OUT)
		pWifiAPs			无线应用实例									(OUT)
		lHandle				当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取无线Wifi网络配置信息
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
参数说明
		lEnable				是否启用无线Wifi模块	(1: 启用 0: 不启用)			(IN)
		pszIP				无线WifiIP地址			(1 ~~ 260个字节的字符串)	(IN)
		pszMask				无线WifiIP子网掩码		(1 ~~ 260个字节的字符串)	(IN)
		pszGateWay			无线Wifi网关			(1 ~~ 260个字节的字符串)	(IN)
		pszSSID				无线Wifi网络名			(1 ~~ 260个字节的字符串)	(IN)
		lSecEn				安全设置 打开/关闭									(IN)
		plSecType			安全类型 wep或wpa									(IN)
		WepOpt				网络验证 开放式或共享式								(IN)
		lWepIndex			密钥索引 1 ~~ 4										(IN)
		lWepType			密钥长度											(IN)
		pszWepPass			密钥					(1 ~~ 260个字节的字符串)	(IN)
		lDhcp				Wifi DHCP 打开/关闭									(IN)
		lHandle				当前网络连接的操作句柄								(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置无线Wifi网络配置信息
*/
long __stdcall NP_SetWifiInfo( long lEnable, char* pszIP, char* pszMask,
							 char* pszGateWay, char* pszSSID, long lSecEn,
							 long lSecType, long lWepOpt, long lWepIndex,
							 long lWepType, char* pszWepPass, long lDhcp,
							 long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszChannelName		通道名称OSD字符串	(1 ~~ 10个字节的字符串)	(OUT)
		plShowChannel		显示或隐藏通道OSD	(1: 显示 0: 隐藏)		(OUT)
		plShowTime			显示或隐藏Time OSD	(1: 显示 0: 隐藏)		(OUT)
		plShowBps			显示或隐藏Bps OSD	(1: 显示 0: 隐藏)		(OUT)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取OSD信息
*/
long __stdcall NP_GetOSDInfo( char* pszChannelName, long* plShowChannel,
							long* plShowTime, long* plShowBps,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszChannelName		通道名称OSD字符串	(1 ~~ 10个字节的字符串)	(IN)
		lShowChannel		显示或隐藏通道OSD	(1: 显示 0: 隐藏)		(IN)
		lShowTime			显示或隐藏Time OSD	(1: 显示 0: 隐藏)		(IN)
		lShowBps			显示或隐藏Bps OSD	(1: 显示 0: 隐藏)		(IN)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置OSD信息
*/
long __stdcall NP_SetOSDInfo( char* pszChannelName, long lShowChannel,
							long  lShowTime, long lShowBps,
							long  lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle		当前网络连接的操作句柄								(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		恢复视频服务器出厂默认值
*/
long __stdcall NP_LoadDefault( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle		当前网络连接的操作句柄								(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		重启视频服务器设备
*/
long __stdcall NP_Reboot( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle		当前网络连接的操作句柄								(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		重启视频服务器服务
*/
long __stdcall NP_Reset( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszDevName		视频服务器设备名称		(1 ~~ 36个字节字符串)	(OUT)
		pszDevID		视频服务器设备ID								(OUT)
		pszKernelVer	视频服务器内核版本								(OUT)
		pszServiceVer	视频服务器服务版本								(OUT)
		pszWebVer		视频服务器网页版本								(OUT)
		lHandle			当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器名称
*/
long __stdcall NP_GetDevDes( char* pszDevName , char* pszDevID,
						   char* pszKernelVer, char* pszServiceVer,
						   char* pszWebVer, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszDevName	视频服务器设备名称(1 ~~ 36个字节字符串)		(IN)
		lHandle		当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器名称
*/
long __stdcall NP_SetDevName( char* pszDevName , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plMaxVE				设备支持的最大编码个数(RTSP连接通道最大个数)	(OUT)
		plMaxVI				设备输入最大支持个数							(OUT)
		pVETypes			支持的编码格式数组								(OUT)
		pVEFmts				支持的视频压缩格式								(OUT)
		pVectorVStdandard	视频制式										(OUT)
		pVEChns				视频编码通道格式数组							(OUT)
		lHandle				当前网络连接的操作句柄							(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器支持的编码信息
		该函数对应的是设备整理的编码能力
*/
long __stdcall NP_GetDevVEInfo( long* plMaxVE, long* plMaxVI,
							   NP_VEncode_Type_S_EX* pVETypes,
							   NP_VEncode_Format_S_EX* pVEFmts,
							   NP_VI_Standard_S_EX* pVEStds,
							   NP_VEncode_Chn_S_EX* pVEChns,
							   long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明		
		emVEChns			视频编码通道格式数组			(IN)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器当前编码配置信息
		该函数用于设置设备整体编码环境
*/
long __stdcall NP_SetDevVEInfo( NP_VEncode_Chn_S_EX emVEChns,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pEncFormat			当前通道视频编码格式		(OUT)
		pEncType			当前通道编码类型			(OUT)
		pVSTD				当前通道视频制式			(OUT)
		plVIput				当前通道视频输入源			(OUT)
		lHandle				当前网络连接的操作句柄		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器当前通道编码信息
		针对设备的某一通道
*/
long __stdcall NP_GetChnVEInfo(	NP_VEncode_Format_E* pEncFormat,
								NP_VEncode_Type_E* pEncType,
								NP_VI_Standard_E* pVSTD,
								long* plVIput,
								long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		emEncFormat			当前通道视频编码格式		(IN)
		pszString			对应的编码格式字符串		(OUT)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取编码格式对应的描述字符串,如Mpeg4 H264等
*/
long __stdcall NP_GetVEncFormatString( NP_VEncode_Format_E emEncFormat, char* pszString );

//----------------------------------------------------------------------------------

/*
参数说明
		emEncType			当前通道视频编码器类型		(IN)
		pszString			对应的编码格式字符串		(OUT)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		根据编码类型获取对应的描述字符串,如D1 Cif等
*/
long __stdcall NP_GetVEncTypeString( NP_VEncode_Type_E emEncType, char* pszString );

//----------------------------------------------------------------------------------

/*
参数说明
		emVSTD				当前通道视频制式			(IN)
		pszString			对应的编码格式字符串		(OUT)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		根据视频制式获取对应的描述字符串,如Pal Ntsc等
*/
long __stdcall NP_GetVStandardString( NP_VI_Standard_E emVSTD, char* pszString );

//----------------------------------------------------------------------------------

/*
参数说明
		pEncType			当前通道视频编码器类型		(OUT)
		pszString			对应的编码格式字符串		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		根据编码类型字符串获取对应的编码类型
*/
long __stdcall NP_GetVEncTypeFromString( NP_VEncode_Type_E* pEncType, char* pszString );

//----------------------------------------------------------------------------------

/*
参数说明
		pEncFormat			当前通道视频编码格式		(OUT)
		pszString			对应的编码格式字符串		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		根据编码格式描述字符串获取对应的类型
*/
long __stdcall NP_GetVEncFormatFromString( NP_VEncode_Format_E* pEncFormat, char* pszString );

//----------------------------------------------------------------------------------

/*
参数说明
		pVSTD				当前通道视频制式			(OUT)
		pszString			对应的编码格式字符串		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		根据视频制式描述字符串获取对应的视频制式
*/
long __stdcall NP_GetVStandardFromString( NP_VI_Standard_E* pVSTD, char* pszString );

//----------------------------------------------------------------------------------

/*
参数说明
		plRTSPPort			RTSP通讯端口		默认554		(OUT)
		plWebPort			网页通讯端口		默认80		(OUT)
		plABCPort			语音对讲端口		默认8004	(OUT)
		plUpdatePort		更新设备端口		默认8006	(OUT)
		plFtpPort			FTP通讯端口			默认21		(OUT)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器通讯端口信息
*/
long __stdcall NP_GetCommunicationPort(	long* plRTSPPort,
										long* plWebPort,
										long* plABCPort,
										long* plUpdatePort,
										long* plFtpPort,
										long lHandle  );

//----------------------------------------------------------------------------------

/*
参数说明
		lRTSPPort			RTSP通讯端口		默认554		(IN)
		lWebPort			网页通讯端口		默认80		(IN)
		lABCPort			语音对讲端口		默认8004	(IN)
		lUpdatePort			更新设备端口		默认8006	(IN)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器通讯端口信息
*/
long __stdcall NP_SetCommunicationPort(	long lRTSPPort,
										long lWebPort,
										long lABCPort,
										long lUpdatePort,
										long lHandle  );

//----------------------------------------------------------------------------------

/*
参数说明
		plRtspUpnpState		Rtsp端口UPNP是否启用		1:启用 0:未启用	(OUT)
		plWebUpnpState		Web端口UPNP是否启用			1:启用 0:未启用	(OUT)
		plAbcUpnpState		语音对讲端口UPNP是否启用	1:启用 0:未启用	(OUT)
		plUpdateUpnpState	软件更新端口UPNP是否启用	1:启用 0:未启用	(OUT)
		plFtpUpnpState		Ftp端口UPNP是否启用			1:启用 0:未启用	(OUT)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器UPNP信息
*/
long __stdcall NP_GetUPNPState( long* plRtspUpnpState, long* plWebUpnpState,
							   long* plAbcUpnpState,long* plUpdateUpnpState,
							   long* plFtpUpnpState,long lHandle  );

//----------------------------------------------------------------------------------

/*
参数说明
		lRtspUpnpState		Rtsp端口UPNP是否启用		1:启用 0:未启用	(IN)
		lWebUpnpState		Web端口UPNP是否启用			1:启用 0:未启用	(IN)
		lAbcUpnpState		语音对讲端口UPNP是否启用	1:启用 0:未启用	(IN)
		lUpdateUpnpState	软件更新端口UPNP是否启用	1:启用 0:未启用	(IN)
		lFtpUpnpState		Ftp端口UPNP是否启用			1:启用 0:未启用	(IN)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器UPNP信息
*/
long __stdcall NP_SetUPNPState( long lRtspUpnpState, long lWebUpnpState,
							   long lAbcUpnpState,long lUpdateUpnpState,
							   long lFtpUpnpState,long lHandle  );

//----------------------------------------------------------------------------------

/*
参数说明
		pNicType			PPPOE选择类型		0: 有线 1: 无线Wifi		(OUT)
		plEnable			是否启用PPPOE								(OUT)
		pszUser				拨号用户名									(OUT)
		pszPwd				拨号密码									(OUT)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器PPPOE信息
*/
long __stdcall NP_GetPPPOEInfo(  NP_Nic_Type* pNicType,
								long* plEnable, 
								char* pszUser,
								char* pszPwd,
								long lHandle  );

//----------------------------------------------------------------------------------

/*
参数说明
		emNicType			PPPOE选择类型		0: 有线 1: 无线Wifi		(IN)
		lEnable				是否启用PPPOE								(IN)
		pszUser				拨号用户名									(IN)
		pszPwd				拨号密码									(IN)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器PPPOE信息
*/
long __stdcall NP_SetPPPOEInfo(  NP_Nic_Type emNicType,
								long lEnable, 
								char* pszUser,
								char* pszPwd,
								long lHandle  );

//----------------------------------------------------------------------------------

/*
参数说明
		plEnable			是否启用DDNS							(OUT)
		pDdnsProviders		支持的DDNS服务提供商					(OUT)
		pszProvider			当前使用的DDNS服务提供商				(OUT)
		pszUser				用户名									(OUT)
		pszPwd				密码									(OUT)
		pszURL				DDNS地址								(OUT)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器DDNS信息
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
参数说明
		lEnable				是否启用DDNS							(IN)
		pszProvider			当前使用的DDNS服务提供商最大260个字节	(IN)
		pszUser				用户名最大63个字节						(IN)
		pszPwd				密码最大63个字节						(IN)
		pszURL				DDNS地址最大260个字节					(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器DDNS信息
*/
long __stdcall NP_SetDDNSInfo(  long lEnable,
								char* pszProvider,
								char* pszUser,
								char* pszPwd,
								char* pszURL,
								long lHandle  );

//----------------------------------------------------------------------------------

/*
参数说明
		plNTPEnable			NTP是否启用				1:启用 0:未启用	(OUT)
		pszNTPServer		NTP服务器地址							(OUT)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器NTP信息
*/
long __stdcall NP_GetNTPInfo(  long* plNTPEnable, char* pszNTPServer, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lNTPEnable			NTP是否启用				1:启用 0:未启用	(IN)
		pszNTPServer		NTP服务器地址							(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器NTP信息
*/
long __stdcall NP_SetNTPInfo(  long lNTPEnable, char* pszNTPServer, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plTime				当前视频服务器内部时钟					(OUT)
		plTimeZone			当前视频服务器内部时区					(OUT)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器时钟信息
*/
long __stdcall NP_GetTimeInfo(  long* plTime, long* plTimeZone, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lTime				当前视频服务器内部时钟					(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器时钟信息
*/
long __stdcall NP_SetTime(  long lTime, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lTimeZone			当前视频服务器内部时区					(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器时钟信息
*/
long __stdcall NP_SetTimeZone(  long lTimeZone, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plEnable			是否启用视频丢失检测					(OUT)
		plSensitivity		视频丢失灵敏度							(OUT)
		pWeekTimes			布防规则								(OUT)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器视频丢失配置信息
*/
long __stdcall NP_GetVideoLoss(  long* plEnable, long* plSensitivity,
								  WEEK_TIME_S_EX* pWeekTimes, 
								  long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lEnable				是否启用视频丢失检测					(IN)
		lSensitivity		视频丢失灵敏度							(IN)
		emWeekTimes			布防规则								(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器视频丢失配置信息
*/
long __stdcall NP_SetVideoLoss(  long lEnable, long lSensitivity,
								  WEEK_TIME_S_EX emWeekTimes,
								  long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plEnable			否启用用户验证							(OUT)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器是否启用用户验证
*/
long __stdcall NP_GetAuthMode(  long* plEnable, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lEnable				否启用用户验证							(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器是否启用用户验证
*/
long __stdcall NP_SetAuthMode(  long lEnable, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pUsers				视频服务器用户列表						(OUT)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器是否启用用户验证
*/
long __stdcall NP_GetAllUser( NP_User_Dev_S_EX* pUsers, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszUser				用户名					(IN)
		pszPwd				密码					(IN)
		emPower				权限					(IN)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		添加用户
*/
long __stdcall NP_AddUser( char* pszUser,char* pszPwd,
							NP_User_Power_E emPower, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszUser				用户名					(IN)
		pszPwd				密码					(IN)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		修改用户密码
*/
long __stdcall NP_SetUserPwd( char* pszUser,char* pszPwd, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszUser				用户名					(IN)
		emPower				权限					(IN)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		修改用户权限
*/
long __stdcall NP_SetUserPower( char* pszUser, NP_User_Power_E emPower, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszUser				用户名					(IN)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		删除用户
*/
long __stdcall NP_DelUser( char* pszUser, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle		当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		升级视频服务器包
*/
long __stdcall NP_UpdatePacket( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plSensor1En		探头输入1是否启用			(OUT)
		plSensor2En		探头输入2是否启用			(OUT)
		plSensor3En		探头输入3是否启用			(OUT)
		plSensor4En		探头输入4是否启用			(OUT)
		pWeekTimes		布防规则数组				(OUT)
		lHandle			当前网络连接的操作句柄		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器探头报警输入配置
*/
long __stdcall NP_GetSensorIn( long* plSensor1En, long* plSensor2En,
							 long* plSensor3En, long* plSensor4En, 
							 WEEK_TIME_S_EX* pWeekTimes, 
							 long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lSensor1En		探头输入1是否启用			(IN)
		lSensor2En		探头输入2是否启用			(IN)
		lSensor3En		探头输入3是否启用			(IN)
		lSensor4En		探头输入4是否启用			(IN)
		emWeekTimes		布防规则结构数组			(IN)
		lHandle			当前网络连接的操作句柄		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器探头报警输入配置
*/
long __stdcall NP_SetSensorIn( long lSensor1En, long lSensor2En,
							   long lSensor3En, long lSensor4En, 
							   WEEK_TIME_S_EX emWeekTimes, 
							   long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plStatus	前端录像状态			(OUT)
		lHandle		当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器前端录像状态
*/
long __stdcall NP_GetDevRecordStatus( NP_E_DevRecord_Status* plStatus, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plDiskReserve		磁盘保留空间(M单位)			(OUT)
		pDiskStrategy		磁盘管理策率				(OUT)
		plFileMaxTime		录像文件最大长度(分钟单位)	(OUT)
		pFileStrategy		文件管理策率				(OUT)
		pDisks				磁盘统计信息				(OUT)
		pWeekTimes			布防规则					(OUT)
		lHandle				当前网络连接的操作句柄		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器磁盘策略
*/
long __stdcall NP_GetDevDiskStrategy( long* plDiskReserve, NP_E_DevDiskStrategy* pDiskStrategy,
									long* plFileMaxTime, NP_E_DevFileStrategy* pFileStrategy,
									NP_Dev_DiskInfo_S_EX* pDisks,
									WEEK_TIME_S_EX* pWeekTimes,
									long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lDiskReserve		磁盘保留空间(M单位)			(IN)
		emDiskStrategy		磁盘管理策率				(IN)
		lFileMaxTime		录像文件最大长度(分钟单位)	(IN)
		emFileStrategy		文件管理策率				(IN)
		emWeekTimes			布防规则					(IN)
		lHandle				当前网络连接的操作句柄		(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器磁盘策略
*/
long __stdcall NP_SetDevDiskStrategy( long lDiskReserve, NP_E_DevDiskStrategy emDiskStrategy,
									 long lFileMaxTime, NP_E_DevFileStrategy emFileStrategy,
									 WEEK_TIME_S_EX emWeekTimes, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lStart		开始或停止前端录像		(IN)
		lHandle		当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		视频服务器前端录像控制
*/
long __stdcall NP_StartDevRecord( long lStart, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lTrackID		轨迹ID 1 ~~ 16			(IN)
		pPresets		当前轨迹对应的预置位数组(OUT)
		lHandle			当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器指定轨迹配置
*/
long __stdcall NP_GetTrackInfo( long lTrackID, 
							 PRESET_INFO_S_EX* pPresets ,
							 long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lTrackID		轨迹ID 1 ~~ 16			(IN)
		emPresets		当前轨迹对应的预置位数组(IN)
		lHandle			当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频服务器指定轨迹配置
*/
long __stdcall NP_SetTrackInfo( long lTrackID, 
								 PRESET_INFO_S_EX emPresets,
								 long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lTrackID	轨迹ID	1 ~~ 16			(IN)
		lHandle		当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		轨迹调用
*/
long __stdcall NP_CallTrack( long lTrackID, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		TP09_ON_RECV_CB	透明端口回调函数		(IN)
		pParam			自定义参数				(IN)
		lHandle			当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置透明端口回调函数
*/
long __stdcall NP_SetTP09CallBackFunc( UINT ( CALLBACK* TP09_ON_RECV_CB ) ( BYTE* pData,UINT nData,PVOID pParam), PVOID pParam , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pData			透明端口指令数据				(IN)
		lData			指令数据长度(最大2048字节)		(IN)
		lHandle			当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		发送透明端口指令
*/
long __stdcall NP_SendTP09CMD( LPBYTE pData,long lData , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszAlarmCenterAddr	报警中心地址(IP或DDNS)			(OUT)
		plPort				报警中心通讯端口  ( 1 ~ 65535)	(OUT)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取报警联动报警中心配置
*/
long __stdcall NP_GetCAAlarmCenter( char* pszAlarmCenterAddr, long* plPort , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszAlarmCenterAddr	报警中心地址(IP或DDNS)			(IN)
		lPort				报警中心通讯端口  ( 1 ~ 65535)	(IN)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置报警联动报警中心配置
*/
long __stdcall NP_SetCAAlarmCenter( char* pszAlarmCenterAddr, long lPort , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszEmailAddr		邮箱服务器地址			(OUT)
		pszUser				登陆邮箱用户名			(OUT)
		pszPwd				登陆邮箱密码			(OUT)
		pszToAddr			接收邮件的Email			(OUT)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取报警联动发送邮件配置
*/
long __stdcall NP_GetCAEmail( char* pszEmailAddr, char* pszUser, char* pszPwd ,
														   char* pszToAddr , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszEmailAddr		邮箱服务器地址			(IN)
		pszUser				登陆邮箱用户名			(IN)
		pszPwd				登陆邮箱密码			(IN)
		pszToAddr			接收邮件的Email			(IN)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置报警联动发送邮件配置
*/
long __stdcall NP_SetCAEmail( char* pszEmailAddr, char* pszUser, char* pszPwd ,
							   char* pszToAddr , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszFTPHost			FTP主机地址						(OUT)
		pszUser				登陆用户名						(OUT)
		pszPwd				登陆密码						(OUT)
		pszPath			    FTP路径							(OUT)
		plType				上传文件类型 (0: 图片 1: 录像)	(OUT)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取报警联动上传FTP配置
*/
long __stdcall NP_GetCAFTP( char* pszFTPHost, char* pszUser, char* pszPwd ,
						   char* pszPath , long* plType, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszFTPHost			FTP主机地址						(IN)
		pszUser				登陆用户名						(IN)
		pszPwd				登陆密码						(IN)
		pszPath			    FTP路径							(IN)
		lType				上传文件类型 (0: 图片 1: 录像)	(IN)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置报警联动上传FTP配置
*/
long __stdcall NP_SetCAFTP( char* pszFTPHost, char* pszUser, char* pszPwd ,
						   char* pszPath , long lType, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plSensorOut1		探头输出1状态 (0: 关闭 1: 打开)	(OUT)
		plSensorOut2		探头输出2状态 (0: 关闭 1: 打开)	(OUT)
		plHoldTime			探头输出状态保持时间			(OUT)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取报警联动探头输出配置
*/
long __stdcall NP_GetCASensorOut( long* plSensorOut1,long* plSensorOut2 ,long* plHoldTime, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lSensorOut1		探头输出1状态 (0: 关闭 1: 打开)	(IN)
		lSensorOut2		探头输出2状态 (0: 关闭 1: 打开)	(IN)
		lHoldTime		探头输出状态保持时间			(IN)
		lHandle			当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置报警联动探头输出配置
*/
long __stdcall NP_SetCASensorOut( long lSensorOut1,long lSensorOut2 ,long lHoldTime, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lSensorIn			探头输出报警			(0 ~ 3)				(IN)
		plACEn				报警时联动报警中心		(0: 关闭 1: 启用)	(OUT)
		plEmailEn			报警时联动发送邮件		(0: 关闭 1: 启用)	(OUT)
		plAO1En				报警时联动打开探头输出1 (0: 关闭 1: 启用)	(OUT)
		plAO2En				报警时联动打开探头输出2 (0: 关闭 1: 启用)	(OUT)
		plRecordEn			报警时联动录像			(0: 关闭 1: 启用)	(OUT)
		plFTPEn				报警时联动上传文件到FTP (0: 关闭 1: 启用)	(OUT)
		plSoundEn			报警时联动播放音频		(0: 关闭 1: 启用)	(OUT)
		plSnapEn			报警时联动抓图			(0: 关闭 1: 启用)	(OUT)
		plInterval			报警间隔				(5 ~ 36000 秒)		(OUT)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取探头输入报警联动配置
*/
long __stdcall NP_GetCASensorIn( long lSensorIn,long* plACEn,long* plEmailEn,
							   long* plAO1En, long* plAO2En,long* plRecordEn,
							   long* plFTPEn, long* plSoundEn,long* plSnapEn,
							   long* plInterval,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lSensorIn			探头输出报警			(0 ~ 3)				(IN)
		lACEn				报警时联动报警中心		(0: 关闭 1: 启用)	(IN)
		lEmailEn			报警时联动发送邮件		(0: 关闭 1: 启用)	(IN)
		lAO1En				报警时联动打开探头输出1 (0: 关闭 1: 启用)	(IN)
		lAO2En				报警时联动打开探头输出2 (0: 关闭 1: 启用)	(IN)
		lRecordEn			报警时联动录像			(0: 关闭 1: 启用)	(IN)
		lFTPEn				报警时联动上传文件到FTP (0: 关闭 1: 启用)	(IN)
		lSoundEn			报警时联动播放音频		(0: 关闭 1: 启用)	(IN)
		lSnapEn				报警时联动抓图			(0: 关闭 1: 启用)	(IN)
		lInterval			报警间隔				(5 ~ 36000 秒)		(IN)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置探头输入报警联动配置
*/
long __stdcall NP_SetCASensorIn( long lSensorIn,long lACEn,long lEmailEn,
							   long lAO1En, long lAO2En,long lRecordEn,
							   long lFTPEn, long lSoundEn,long lSnapEn,
							   long lInterval, long lHandle );

//----------------------------------------------------------------------------------


/*
参数说明
		plACEn				报警时联动报警中心		(0: 关闭 1: 启用)	(OUT)
		plEmailEn			报警时联动发送邮件		(0: 关闭 1: 启用)	(OUT)
		plAO1En				报警时联动打开探头输出1 (0: 关闭 1: 启用)	(OUT)
		plAO2En				报警时联动打开探头输出2 (0: 关闭 1: 启用)	(OUT)
		plRecordEn			报警时联动录像			(0: 关闭 1: 启用)	(OUT)
		plFTPEn				报警时联动上传文件到FTP (0: 关闭 1: 启用)	(OUT)
		plSoundEn			报警时联动播放音频		(0: 关闭 1: 启用)	(OUT)
		plSnapEn			报警时联动抓拍			(0: 关闭 1: 启用)	(OUT)
		plInterval			报警间隔				(5 ~ 36000 秒)		(OUT)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取移动侦测报警联动配置
*/
long __stdcall NP_GetCAMD( long* plACEn,long* plEmailEn,long* plAO1En,
						   long* plAO2En,long* plRecordEn,long* plFTPEn,
						   long* plSoundEn,long* plSnapEn,
						   long* plInterval,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lACEn				报警时联动报警中心		(0: 关闭 1: 启用)	(IN)
		lEmailEn			报警时联动发送邮件		(0: 关闭 1: 启用)	(IN)
		lAO1En				报警时联动打开探头输出1 (0: 关闭 1: 启用)	(IN)
		lAO2En				报警时联动打开探头输出2 (0: 关闭 1: 启用)	(IN)
		lRecordEn			报警时联动录像			(0: 关闭 1: 启用)	(IN)
		lFTPEn				报警时联动上传文件到FTP (0: 关闭 1: 启用)	(IN)
		lSoundEn			报警时联动播放音频		(0: 关闭 1: 启用)	(IN)
		lSnapEn				报警时联动抓图			(0: 关闭 1: 启用)	(IN)
		lInterval			报警间隔				(5 ~ 36000 秒)		(IN)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置移动侦测报警联动配置
*/
long __stdcall NP_SetCAMD( long lACEn,long lEmailEn,long lAO1En,
						   long lAO2En,long lRecordEn,long lFTPEn,
						   long lSoundEn,long lSnapEn,
						   long lInterval,long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plACEn				报警时联动报警中心		(0: 关闭 1: 启用)	(OUT)
		plEmailEn			报警时联动发送邮件		(0: 关闭 1: 启用)	(OUT)
		plAO1En				报警时联动打开探头输出1 (0: 关闭 1: 启用)	(OUT)
		plAO2En				报警时联动打开探头输出2 (0: 关闭 1: 启用)	(OUT)
		plSoundEn			报警时联动播放音频		(0: 关闭 1: 启用)	(OUT)
		plInterval			报警间隔				(5 ~ 36000 秒)		(OUT)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频丢失报警联动配置
*/
long __stdcall NP_GetCAVL( long* plACEn,long* plEmailEn,long* plAO1En,
														 long* plAO2En,long* plSoundEn,long* plInterval,
														 long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lACEn				报警时联动报警中心		(0: 关闭 1: 启用)	(IN)
		lEmailEn			报警时联动发送邮件		(0: 关闭 1: 启用)	(IN)
		lAO1En				报警时联动打开探头输出1 (0: 关闭 1: 启用)	(IN)
		lAO2En				报警时联动打开探头输出2 (0: 关闭 1: 启用)	(IN)
		lSoundEn			报警时联动播放音频		(0: 关闭 1: 启用)	(IN)
		lInterval			报警间隔				(5 ~ 36000 秒)		(IN)
		lHandle				当前网络连接的操作句柄						(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置视频丢失报警联动配置
*/
long __stdcall NP_SetCAVL( long lACEn,long lEmailEn,long lAO1En,
						 long lAO2En,long lSoundEn,long lInterval,
						 long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		plRecordTime		报警录像长度	(10 ~ 36000 秒)		(OUT)
		lHandle				当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取报警录像联动配置
*/
long __stdcall NP_GetCARecord( long* plRecordTime , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lRecordTime			报警录像长度	(10 ~ 36000 秒)		(IN)
		lHandle				当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取报警录像联动配置
*/
long __stdcall NP_SetCARecord( long lRecordTime , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		emVodSearch		搜索前端录像文件结构				(IN)
		lHandle			当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		开始搜索前端录像文件
*/
long __stdcall NP_StartVodSearch( NP_VOD_SEARCH_S emVodSearch , long lHandle );	

//----------------------------------------------------------------------------------

/*
参数说明
		lHandle		当前网络连接的操作句柄				(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		停止搜索前端录像文件
*/
long __stdcall NP_StopVodSearch( long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pPtzNames			视频服务器云台协议列表					(OUT)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器云台协议列表
*/
long __stdcall NP_GetSupportPTZProtocol( PTZ_NAME_S_EX* pPtzNames, long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		ALARM_ON_RECV_CB	报警回调函数			(IN)
		pParam				自定义参数				(IN)
		lHandle				当前网络连接的操作句柄	(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置通用报警回调函数	(支持: 0: 视频丢失报警 1: 移动侦测报警 2: 探头输入报警)
*/
long __stdcall NP_SetAlarmCallBackFunc( UINT ( CALLBACK* ALARM_ON_RECV_CB ) ( UINT nParam1 , UINT nParam2,PVOID pParam), PVOID pParam , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		pszDVSAddr	视频服务器设备通讯地址(支持DDNS或IP地址)
		nChn		视频服务器通道索引		(0 ~~~ 最大通道数 - 1)
		nPort		视频服务器通讯端口		(默认554,范围 1 ~~ 65535)
		nConnType	连接视频服务器协议类型	(0: UDP 1: TCP 2: MUDP)
		pszUser		登陆视频服务器的用户名	(0 ~~ 256字节)
		pszPwd		登陆视频服务器的密码	(0 ~~ 256字节)
		hWndMsg		接收网络传输中消息的窗口句柄	(句柄为空时将无法接收到消息)
		plHandle	返回当前网络连接的操作句柄		

返回值
		执行成功返回0，错误时返回错误码。
函数说明
		建立与视频服务器连接并进行认证,成功返回后可以配置设备参数、控制云台,但无
		音视频流处理.
*/	
long __stdcall NP_MakeAuthentication( const char* pszDVSAddr,const char* pszChn,int nPort,
									int nConnType,const char* pszUser,const char* pszPwd, 
									long hWndMsg,long* plHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lStart				开始或停止								(IN)
		lSeconds			提前录像时间长度(单位:秒 范围1~3600秒)	(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		开始或停止使用提前录像功能
*/
long __stdcall NP_SetPreRecord( long lStart , long lSeconds , long lHandle );

//----------------------------------------------------------------------------------

/*
参数说明
		lTimeOut			网络操作超时时间(秒为单位)				(IN)
		lHandle				当前网络连接的操作句柄					(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置网络操作超时时间(秒为单位,默认1秒)
*/
long __stdcall NP_SetNetTimeOut( long lTimeOut , long lHandle );

//----------------------------------------------------------------------------------

/*	
参数说明
		rc		: 指定显示的视频区域.如果区域为0,则使用原始视频尺寸	(IN)
				  显示,否则将该区域拉伸显示在指定窗口区域.
				  该区域不能超出原始视频的尺寸范围.
	    lHandle	: 当前网络连接的操作句柄							(IN)
				  
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		视频局部放大显示,需要视频已开始预览
*/
long __stdcall NP_SetVideoZoomRect(RECT rc,long lHandle);

//----------------------------------------------------------------------------------

/*
参数说明
		pszCamId			摄像头ID						(OUT)
		pszCamName			摄像头名称						(OUT)
		pszServerAddr		注册服务器地址					(OUT)
		lServerPort			注册服务器通讯端口				(OUT)
		pszAlarmCenterAddr	报警中心地址					(OUT)
		plAlarmCenterPort	报警中心通讯端口				(OUT)
		plMaxAlarmIn		可配置的报警输入最大个数		(OUT)
		plMaxAlarmOut		可配置的报警输出最大个数		(OUT)
		plMdStart			联动选项: 移动侦测开始			(OUT)
		plMdStop			联动选项: 移动侦测停止			(OUT)
		plVlStart			联动选项: 视频丢失检测开始		(OUT)
		plVlResume			联动选项: 视频丢失检测恢复		(OUT)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取HS设备注册配置信息
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
参数说明
		pszCamId			摄像头ID						(IN)
		pszCamName			摄像头称						(IN)
		pszServerAddr		注册服务器地址					(IN)
		lServerPort			注册服务器通讯端口				(IN)
		pszAlarmCenterAddr	报警中心地址					(IN)
		lAlarmCenterPort	报警中心通讯端口				(IN)
		lMaxAlarmIn			可配置的报警输入最大个数		(IN)
		lMaxAlarmOut		可配置的报警输出最大个数		(IN)
		lMdStart			联动选项: 移动侦测开始			(IN)
		lMdStop				联动选项: 移动侦测停止			(IN)
		lVlStart			联动选项: 视频丢失检测开始		(IN)
		lVlResume			联动选项: 视频丢失检测恢复		(IN)
		lHandle				当前网络连接的操作句柄			(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		设置HS设备注册配置信息,设置成功后设备会自动重新启动.
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
参数说明
		hHandle：播放句柄。
		NetMessageCallBack：消息回调函数
		pContext：回调函数参数
返回值	
		执行成功返回0，错误时返回错误码。
函数说明
		设置显示回调函数，该函数在NP_StartPreview后调用,
		在NP_StopPreview调用后自动失效。
		为客户程序提供处理网络消息的机会。
*/
long __stdcall NP_SetNetMessageCallBack(long( CALLBACK* NetMessageCallBack  ) ( long lHandle, long lMsg,long wParam, long lParam , PVOID pContext),  PVOID pContext , long lHandle);

/*
参数说明
		lVideoWndWidth	视频窗口的宽度
		lVideoWndHeigth	视频窗口的高度
		rect			目标矩形（以视频窗口左上角为坐标原点）
		lZoomMode		1 放大 / 0 缩小
		lHandle			当前网络连接的操作句柄
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取预置位配置信息
*/
long __stdcall NP_3DPositioning( long lVideoWndWidth , long lVideoWndHeigth , RECT rect, long lZoomMode, long lHandle );
//----------------------------------------------------------------------------------

/*
参数说明
		pszChn			视频服务器通道									(OUT)
		pszIP			视频服务器IP									(IN)
返回值
		执行成功返回0，错误时返回错误码。
函数说明
		获取视频服务器通道.如：355设备"0",365设备"1/qcif,1/cif,1/d1,"
*/
long __stdcall NP_GetDevChn( char* pszChn , const char* pszIP);
long __stdcall NP_PausePlay(long	lHandle);
}		// end extern "C"

#endif

