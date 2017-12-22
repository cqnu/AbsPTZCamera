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

/*未定义错误*/
#define ZW_ERR_NOT_DEFINED        (int(-1))
/*插件路径为空*/
#define ZW_ERR_PATH_IS_EMPTY      (int(0))
/*插件不存在*/
#define ZW_ERR_PATH_NOT_EXIST     (int(1))
/*插件DLL加载失败*/
#define ZW_ERR_LOAD_DLL_FAIL      (int(2))
/*不是有效的PTZ插件*/
#define ZW_ERR_PTZ_ADDIN_FAULT    (int(3))
/*不是有效的VIDEO插件*/
#define ZW_ERR_VIDEO_ADDIN_FAULT  (int(4))
/*PTZ设备1错误*/
#define ZW_ERR_PTZ1_FAULT         (int(5))
/*PTZ设备2错误*/
#define ZW_ERR_PTZ2_FAULT         (int(6))
/*VIDEO设备错误*/
#define ZW_ERR_VIDEO_FAULT        (int(7))
/*设备已经被打开*/
#define ZW_ERR_ALREADY_OPEN       (int(8))
/*不支持的调用*/
#define ZW_ERR_UNSUPPORT_CALL     (int(9))

#endif

#ifndef ZW_HANDLE_DEFINED
#define ZW_HANDLE_DEFINED

/*句柄类型定义*/
typedef void*  handle;

#define ZW_LENGTH_IP_ADDR              16
#define ZW_LENGTH_USER_NAME            32
#define ZW_LENGTH_PASSWORD             32
#define ZW_LENGTH_URL                  256

#endif


/*图像像素格式*/

#define ZW_PIX_FMT_NONE           0

/*  planar format */

/*  planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples) */
#define ZW_PIX_FMT_YUV420P        1 
/*  as above, but U and V plane are swapped   ***ffmpeg不支持*** */
#define ZW_PIX_FMT_YV12           2      
/*  planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples) */
#define ZW_PIX_FMT_YUV422P        3      
/*  as above, but U and V plane are swapped   ***ffmpeg不支持*** */
#define ZW_PIX_FMT_YV16           4      
/*  planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples) */
#define ZW_PIX_FMT_YUV444P        5  
/*  planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)  ***ffmpeg不支持*** */
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


/*以下格式不支持作为声明输出——即不支持作为InitialVideoSource的realFixFmt、triggerFixFmt参数*/

/*  planar YUV 4:2:2, 16bpp, 1 plane for Y and 1 plane for the UV components, */
/*  which are interleaved (first byte U and the following byte V) */
/*  (1 Cr & Cb sample per 2x1 Y samples) */
#define ZW_PIX_FMT_NV16           55  
/*  as above, but U and V bytes are swapped  ***ffmpeg不支持*** */
#define ZW_PIX_FMT_NV61           56     
/*  packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr */
#define ZW_PIX_FMT_YUYV422        57     
/*  packed YUV 4:2:2, 16bpp, Y0 Cr Y1 Cb   ***ffmpeg不支持*** */
#define ZW_PIX_FMT_YVYU422        58     
/*  packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1 */
#define ZW_PIX_FMT_UYVY422        59     
/*  packed YUV 4:2:2, 16bpp, Cr Y0 Cb Y1   ***ffmpeg不支持*** */
#define ZW_PIX_FMT_VYUY422        60     

/*  file format */

/*  JPEG JPG文件  */
#define ZW_PIX_FMT_JPEG           100 
/*  windows BMP文件 */
#define ZW_PIX_FMT_BMP            101   



/*视频编码*/
#define ZW_CODEC_ID_NONE          0
#define ZW_CODEC_ID_H264          1
#define ZW_CODEC_ID_MPEG4         2
#define ZW_CODEC_ID_MJPEG         3

/*外设功能 PERIPHERAL_FUNCTION*/
#define ZW_PF_NONE                     0x00000000   /*  用于实时RGB图像输出 */
#define ZW_PF_TAKE_PICTURE             0x00000001   /*  抓拍图片，成功后可以在OnTriggerRGBReady方法中获取。 */
#define ZW_PF_OPEN_GATE                0x00000002   /*  开闸机。 */
#define ZW_PF_CLOSE_GATE               0x00000004   /*  关闸机。 */
#define ZW_PF_VEHICLE_IDENTIFY         0x00000008   /*  识别到车辆。此时data为 VehicleInfo*/
#define ZW_PF_ENABLE_GATE              0x00000010   /*  启用闸机控制。启用后闸机可以接收开关闸命令*/
#define ZW_PF_DISABLE_GATE             0x00000020   /*  停用闸机控制，停用后闸机不可以接收开关闸命令并保持停用前的状态*/

#pragma pack(push)
#pragma pack(1)

#ifndef ZW_VIDEO_PARAM
#define ZW_VIDEO_PARAM

typedef struct __ZWVideoParam
{
	char                CameraIp[ZW_LENGTH_IP_ADDR];/*网络相机或者其它视频源设备的ip*/
	char                LocalIp[ZW_LENGTH_IP_ADDR];/*本机IP*/
	unsigned short      CameraPort;/*视频源端口*/
	char                UserName[ZW_LENGTH_USER_NAME];
	char                Password[ZW_LENGTH_PASSWORD];
	int                 AddressNum;/*地址编号，也作为通道号*/
	union
	{
		char            AddressName[ZW_LENGTH_URL];/*地址名称，也作为通道名称*/
		char            Url[ZW_LENGTH_URL];/*视频源的url，如rtsp://192.168.1.77:8557/h264*/
	};
}ZWVideoParam;

#endif

#define ZW_IMAGE_PLANES         8

/*图像数据结构体*/
typedef struct __ZWImageData {
	int    Width;
	int    Height;
	int    PixFmt;
	void*  PlaneData[ZW_IMAGE_PLANES];    /* pointers to the image data planes*/
	int    LineSize[ZW_IMAGE_PLANES];     /* number of bytes per line, when PixFmt is file format LineSize[0] is file size */
} ZWImageData;

/*颜色*/
#define  ZW_COLOR_UNKNOWN        0     //未知
#define  ZW_COLOR_BLUE           1     //蓝色
#define  ZW_COLOR_YELLOW         2     //黄色
#define  ZW_COLOR_WHITE          3     //白色
#define  ZW_COLOR_BLACK          4     //黑色
#define  ZW_COLOR_GREEN          5     //绿色


#define  ZW_COLOR_SILVER         20    //银色
#define  ZW_COLOR_GRAY           21    //灰色
#define  ZW_COLOR_RED            22    //红色
#define  ZW_COLOR_BROWN          23    //棕色
#define  ZW_COLOR_PINK           24    //粉色
#define  ZW_COLOR_PURPLE         25    //紫色
#define  ZW_COLOR_CYAN           26    //青色
#define  ZW_COLOR_DARKBLUE       27    //深蓝
#define  ZW_COLOR_DARKGRAY       28    //深灰


/*车身尺寸 0-未知 1-小车 2-中车 3-大车*/
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
	char       License[16];/*车牌号码*/
	ZWRectF    PlateRect;/*车牌在大图上的位置，取值范围[0.000, 1],图片宽高方向上坐标值分别归一化*/
	int        PlateColor;/*车牌颜色*/
	int        VehicleColor;/*车身颜色*/
	int        VehicleSize;/*车身尺寸*/
}ZWVehicleInfo;

#pragma pack(pop)

/*视频流输出回调*/
/*frametype = 1表示是关键帧，否者不是关键帧*/
typedef void (__stdcall* VideoStreamCallback)(void *pUserData, void* buff, long size, int frametype);

/*触发图像输出回调，data随着图片一起输出的数据*/
/*ZW_PF_VEHICLE_IDENTIFY 功能，data为车牌信息*/
/*ZW_PF_TAKE_PICTURE 功能，data为抓图时间 从1970-01-01 00:00:00 至现在的秒数，类型是 long long （64位有符号整数）*/
typedef void (__stdcall* TriggerImageCallback)(void *pUserData, ZWImageData* img, void* data, int func);

/*实时图像输出回调*/
typedef void (__stdcall* RealTimeImageCallback)(void *pUserData, ZWImageData* img);



/*初始化视频源，传入回调用于获取码流和外部触发抓拍的图片*/
ZW_VIDEO_API handle __cdecl  InitialVideoSource(ZWVideoParam *param,
	                                         VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
	                                         RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
	                                         RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
	                                         int realFixFmt,/*实时转码图像的格式*/
	                                         TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
	                                         TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
	                                         int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
											 void *pUserData);

/*获取视频源编码，返回ZW_CODEC_ID_? */
ZW_VIDEO_API int    __cdecl  GetVideoCodec(handle hVS);


/*返回设备是否能自动重连。支持的话返回1，否则返回0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS);

/*开启视频源，返回0失败，1成功*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS, handle hPlayWnd);

/*触发视频设备外设功能，返回0失败，1成功。*/
/*如果设备不支持或者应用不需要，就不用实现这个接口*/
ZW_VIDEO_API int    __cdecl  TriggerPeripheral(handle hVS,int func);

/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS);

/*终止视频源模块*/
ZW_VIDEO_API void   __cdecl  FinializeVideoSource(handle hVS);



typedef handle  (__cdecl* INITIAL_VIDEO_SOURCE_PROC)(ZWVideoParam *param,
	                                         VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
	                                         RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
	                                         RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
	                                         int realFixFmt,/*实时转码图像的格式*/
	                                         TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
	                                         TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
	                                         int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
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