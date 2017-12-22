#ifndef __VIDEO_H__
#define __VIDEO_H__

#if defined(_MSC_VER) || defined(__MINGW32__)
  #define HELPER_DLL_IMPORT __declspec(dllimport)
  #define HELPER_DLL_EXPORT __declspec(dllexport)
  #define HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define HELPER_DLL_IMPORT __attribute__((dllimport))
    #define HELPER_DLL_EXPORT __attribute__((dllexport))
    #define HELPER_DLL_LOCAL
  #endif
#endif

#ifdef ZW_VIDEO_DLL_EXPORTS
  #define ZW_VIDEO_API HELPER_DLL_EXPORT
#elif ZW_VIDEO_DLL_IMPORTS
  #define ZW_VIDEO_API HELPER_DLL_IMPORT
#endif

#define ZW_VIDEO_LOCAL HELPER_DLL_LOCAL


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ZW_ERR_MSG
#define ZW_ERR_MSG

/*δ�������*/
#define ZW_ERR_NOT_DEFINED        (int(-1))
/*���·��Ϊ��*/
#define ZW_ERR_PATH_IS_EMPTY      (int(0))
/*���������*/
#define ZW_ERR_PATH_NOT_EXIST     (int(1))
/*���DLL����ʧ��*/
#define ZW_ERR_LOAD_DLL_FAIL      (int(2))
/*������Ч��PTZ���*/
#define ZW_ERR_PTZ_ADDIN_FAULT    (int(3))
/*������Ч��VIDEO���*/
#define ZW_ERR_VIDEO_ADDIN_FAULT  (int(4))
/*PTZ�豸1����*/
#define ZW_ERR_PTZ1_FAULT         (int(5))
/*PTZ�豸2����*/
#define ZW_ERR_PTZ2_FAULT         (int(6))
/*VIDEO�豸����*/
#define ZW_ERR_VIDEO_FAULT        (int(7))
/*�豸�Ѿ�����*/
#define ZW_ERR_ALREADY_OPEN       (int(8))
/*��֧�ֵĵ���*/
#define ZW_ERR_UNSUPPORT_CALL     (int(9))

#endif

#ifndef ZW_HANDLE_DEFINED
#define ZW_HANDLE_DEFINED

/*������Ͷ���*/
typedef void*  handle;

#define ZW_LENGTH_IP_ADDR              16
#define ZW_LENGTH_USER_NAME            32
#define ZW_LENGTH_PASSWORD             32
#define ZW_LENGTH_URL                  256

#endif


/*ͼ�����ظ�ʽ*/

#define ZW_PIX_FMT_NONE           0

/*  planar format */

/*  planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples) */
#define ZW_PIX_FMT_YUV420P        1 
/*  as above, but U and V plane are swapped   ***ffmpeg��֧��*** */
#define ZW_PIX_FMT_YV12           2      
/*  planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples) */
#define ZW_PIX_FMT_YUV422P        3      
/*  as above, but U and V plane are swapped   ***ffmpeg��֧��*** */
#define ZW_PIX_FMT_YV16           4      
/*  planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples) */
#define ZW_PIX_FMT_YUV444P        5  
/*  planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)  ***ffmpeg��֧��*** */
#define ZW_PIX_FMT_YVU444P        6      


/*  packed format */

/*  packed RGB 8:8:8, 24bpp, RGBRGB... */
#define ZW_PIX_FMT_RGB24          51     
/*  packed RGB 8:8:8, 24bpp, BGRBGR... */
#define ZW_PIX_FMT_BGR24          52     
/*  YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, */
/*  which are interleaved (first byte U and the following byte V) */
#define ZW_PIX_FMT_NV12           53  
/*  as above, but U and V bytes are swapped */
#define ZW_PIX_FMT_NV21           54     


/*���¸�ʽ��֧����Ϊ���������������֧����ΪInitialVideoSource��realFixFmt��triggerFixFmt����*/

/*  planar YUV 4:2:2, 16bpp, 1 plane for Y and 1 plane for the UV components, */
/*  which are interleaved (first byte U and the following byte V) */
/*  (1 Cr & Cb sample per 2x1 Y samples) */
#define ZW_PIX_FMT_NV16           55  
/*  as above, but U and V bytes are swapped  ***ffmpeg��֧��*** */
#define ZW_PIX_FMT_NV61           56     
/*  packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr */
#define ZW_PIX_FMT_YUYV422        57     
/*  packed YUV 4:2:2, 16bpp, Y0 Cr Y1 Cb   ***ffmpeg��֧��*** */
#define ZW_PIX_FMT_YVYU422        58     
/*  packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1 */
#define ZW_PIX_FMT_UYVY422        59     
/*  packed YUV 4:2:2, 16bpp, Cr Y0 Cb Y1   ***ffmpeg��֧��*** */
#define ZW_PIX_FMT_VYUY422        60     

/*  file format */

/*  JPEG JPG�ļ�  */
#define ZW_PIX_FMT_JPEG           100 
/*  windows BMP�ļ� */
#define ZW_PIX_FMT_BMP            101   



/*��Ƶ����*/
#define ZW_CODEC_ID_NONE          0
#define ZW_CODEC_ID_H264          1
#define ZW_CODEC_ID_MPEG4         2
#define ZW_CODEC_ID_MJPEG         3

/*���蹦�� PERIPHERAL_FUNCTION*/
#define ZW_PF_NONE                     0x00000000   /*  ����ʵʱRGBͼ����� */
#define ZW_PF_TAKE_PICTURE             0x00000001   /*  ץ��ͼƬ���ɹ��������OnTriggerRGBReady�����л�ȡ�� */
#define ZW_PF_OPEN_GATE                0x00000002   /*  ��բ���� */
#define ZW_PF_CLOSE_GATE               0x00000004   /*  ��բ���� */
#define ZW_PF_VEHICLE_IDENTIFY         0x00000008   /*  ʶ�𵽳�������ʱdataΪ VehicleInfo*/
#define ZW_PF_ENABLE_GATE              0x00000010   /*  ����բ�����ơ����ú�բ�����Խ��տ���բ����*/
#define ZW_PF_DISABLE_GATE             0x00000020   /*  ͣ��բ�����ƣ�ͣ�ú�բ�������Խ��տ���բ�������ͣ��ǰ��״̬*/

#pragma pack(push)
#pragma pack(1)

#ifndef ZW_VIDEO_PARAM
#define ZW_VIDEO_PARAM

typedef struct __ZWVideoParam
{
	char                CameraIp[ZW_LENGTH_IP_ADDR];/*�����������������ƵԴ�豸��ip*/
	char                LocalIp[ZW_LENGTH_IP_ADDR];/*����IP*/
	unsigned short      CameraPort;/*��ƵԴ�˿�*/
	char                UserName[ZW_LENGTH_USER_NAME];
	char                Password[ZW_LENGTH_PASSWORD];
	int                 AddressNum;/*��ַ��ţ�Ҳ��Ϊͨ����*/
	union
	{
		char            AddressName[ZW_LENGTH_URL];/*��ַ���ƣ�Ҳ��Ϊͨ������*/
		char            Url[ZW_LENGTH_URL];/*��ƵԴ��url����rtsp://192.168.1.77:8557/h264*/
	};
}ZWVideoParam;

#endif

#define ZW_IMAGE_PLANES         8

/*ͼ�����ݽṹ��*/
typedef struct __ZWImageData {
	int    Width;
	int    Height;
	int    PixFmt;
	void*  PlaneData[ZW_IMAGE_PLANES];    /* pointers to the image data planes*/
	int    LineSize[ZW_IMAGE_PLANES];     /* number of bytes per line, when PixFmt is file format LineSize[0] is file size */
} ZWImageData;

/*��ɫ*/
#define  ZW_COLOR_UNKNOWN        0     //δ֪
#define  ZW_COLOR_BLUE           1     //��ɫ
#define  ZW_COLOR_YELLOW         2     //��ɫ
#define  ZW_COLOR_WHITE          3     //��ɫ
#define  ZW_COLOR_BLACK          4     //��ɫ
#define  ZW_COLOR_GREEN          5     //��ɫ


#define  ZW_COLOR_SILVER         20    //��ɫ
#define  ZW_COLOR_GRAY           21    //��ɫ
#define  ZW_COLOR_RED            22    //��ɫ
#define  ZW_COLOR_BROWN          23    //��ɫ
#define  ZW_COLOR_PINK           24    //��ɫ
#define  ZW_COLOR_PURPLE         25    //��ɫ
#define  ZW_COLOR_CYAN           26    //��ɫ
#define  ZW_COLOR_DARKBLUE       27    //����
#define  ZW_COLOR_DARKGRAY       28    //���


/*����ߴ� 0-δ֪ 1-С�� 2-�г� 3-��*/
#define  ZW_VEHICLE_UNKNOWN     0
#define  ZW_VEHICLE_SMALL       1
#define  ZW_VEHICLE_MEDIUM      2
#define  ZW_VEHICLE_LARGE       3

typedef struct __ZWRectF
{
	float      X;
	float      Y;
	float      Width;
	float      Height;
}ZWRectF;


typedef struct __ZWVehicleInfo
{
	char       License[16];/*���ƺ���*/
	ZWRectF    PlateRect;/*�����ڴ�ͼ�ϵ�λ�ã�ȡֵ��Χ[0.000, 1],ͼƬ��߷���������ֵ�ֱ��һ��*/
	int        PlateColor;/*������ɫ*/
	int        VehicleColor;/*������ɫ*/
	int        VehicleSize;/*����ߴ�*/
}ZWVehicleInfo;

#pragma pack(pop)

/*��Ƶ������ص�*/
/*frametype = 1��ʾ�ǹؼ�֡�����߲��ǹؼ�֡*/
typedef void (__stdcall* VideoStreamCallback)(void *pUserData, void* buff, long size, int frametype);

/*����ͼ������ص���data����ͼƬһ�����������*/
/*ZW_PF_VEHICLE_IDENTIFY ���ܣ�dataΪ������Ϣ*/
/*ZW_PF_TAKE_PICTURE ���ܣ�dataΪץͼʱ�� ��1970-01-01 00:00:00 �����ڵ������������� long long ��64λ�з���������*/
typedef void (__stdcall* TriggerImageCallback)(void *pUserData, ZWImageData* img, void* data, int func);

/*ʵʱͼ������ص�*/
typedef void (__stdcall* RealTimeImageCallback)(void *pUserData, ZWImageData* img);



/*��ʼ����ƵԴ������ص����ڻ�ȡ�������ⲿ����ץ�ĵ�ͼƬ*/
ZW_VIDEO_API handle __cdecl  InitialVideoSource(ZWVideoParam *param,
	                                         VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
	                                         RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
	                                         RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
	                                         int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
	                                         TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
	                                         TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
	                                         int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
											 void *pUserData);

/*��ȡ��ƵԴ���룬����ZW_CODEC_ID_? */
ZW_VIDEO_API int    __cdecl  GetVideoCodec(handle hVS);


/*�����豸�Ƿ����Զ�������֧�ֵĻ�����1�����򷵻�0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS);

/*������ƵԴ������0ʧ�ܣ�1�ɹ�*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS, handle hPlayWnd);

/*������Ƶ�豸���蹦�ܣ�����0ʧ�ܣ�1�ɹ���*/
/*����豸��֧�ֻ���Ӧ�ò���Ҫ���Ͳ���ʵ������ӿ�*/
ZW_VIDEO_API int    __cdecl  TriggerPeripheral(handle hVS,int func);

/*��ͣ��ƵԴ�����øú�������Ȼ���Ե���StartVideoSource����*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS);

/*��ֹ��ƵԴģ��*/
ZW_VIDEO_API void   __cdecl  FinializeVideoSource(handle hVS);



typedef handle  (__cdecl* INITIAL_VIDEO_SOURCE_PROC)(ZWVideoParam *param,
	                                         VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
	                                         RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
	                                         RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
	                                         int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
	                                         TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
	                                         TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
	                                         int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
											 void *pUserData);

typedef int     (__cdecl* GET_VIDEO_CODEC_PROC)(handle hVS);

typedef int     (__cdecl* CAN_RECONNECT_PROC)(handle);

typedef int     (__cdecl* START_VIDEO_SOURCE_PROC)(handle hVS, handle hPlayWnd);

typedef int     (__cdecl* TRIGGER_PERIPHERAL_PROC)(handle hVS, int func);

typedef void    (__cdecl* STOP_VIDEO_SOURCE_PROC)(handle hVS);

typedef void    (__cdecl* FINIALIZE_VIDEO_SOURCE_PROC)(handle hVS);



#ifdef __cplusplus
}
#endif

#endif