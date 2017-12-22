#ifndef __PTZ_H__
#define __PTZ_H__

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

#ifdef ZW_PTZ_DLL_EXPORTS
  #define ZW_PTZ_API HELPER_DLL_EXPORT
#elif ZW_PTZ_DLL_IMPORTS
  #define ZW_PTZ_API HELPER_DLL_IMPORT
#endif

#define ZW_PTZ_LOCAL HELPER_DLL_LOCAL


/*************PTZ运动性质地操作******************/

/*云台转动*/
#define ZW_TILT_UP				        0x0100
#define ZW_TILT_DOWN				    0x0101
#define ZW_PAN_LEFT				        0x0102
#define ZW_PAN_RIGHT			        0x0103

#define ZW_LEFTUP			            0x0104
#define ZW_LEFTDOWN			            0x0105
#define ZW_RIGHTUP			            0x0106
#define ZW_RIGHTDOWN		            0x0107

/*缩小焦距，效果是拉远*/
#define ZW_ZOOM_OUT                     0x0108

/*增加焦距，效果是拉近*/
#define ZW_ZOOM_IN                      0x0109

/*从当前状态将焦距拉近或者拉远一个等级*/
#define ZW_ZOOM_STEP                    0x0110


/*************非PTZ运动性质地操作******************/

#define ZW_PTZ_NONE				        0x0200
									
/*对焦到近处，让近处景物清晰*/		
#define ZW_FOCUS_NEAR		            0x0201
									
/*对焦到远处，让远处景物清晰*/		
#define ZW_FOCUS_FAR		            0x0202
									
/*加大光圈*/						
#define ZW_IRIS_OPEN		            0x0203
									
/*缩小光圈*/						
#define ZW_IRIS_CLOSE		            0x0204
									
/*补光灯开*/						
#define ZW_LIGHT_ON		                0x0205
									
/*雨刷开*/							
#define ZW_WIPER_ON		                0x0206
									
/*设置预置位*/						
#define ZW_SET_PRESET		            0x0207

/*删除预置位*/
#define ZW_DEL_PRESET		            0x0208

/*************具有停止性质地操作******************/

/*停止所有操作*/
#define ZW_STOP_ALL		                0x0300

#define ZW_STOP_PAN 		            0x0301
#define ZW_STOP_TILT		            0x0302
#define ZW_STOP_ZOOM		            0x0303
#define ZW_STOP_FOCUS		            0x0304
#define ZW_STOP_IRIS		            0x0305

#define ZW_LIGHT_OFF		            0x0306
#define ZW_WIPER_OFF		            0x0307

/*调用预置位*/
#define ZW_USE_PRESET		            0x0318
/*回自动抓拍位置*/
#define ZW_AUTO_POS    		            0x0319


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
#define ZW_ERR_UNSUPPORT_CALL        (int(9))

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

typedef struct __ZWPTZParam
{
	char  DeviceIp[ZW_LENGTH_IP_ADDR];
	char  LocalIp[ZW_LENGTH_IP_ADDR];/*本机IP*/
	int   DevicePort;
	char  UserName[ZW_LENGTH_USER_NAME];
	char  Password[ZW_LENGTH_PASSWORD];
	int   Number;/*串口编号，如COM1 COM2*/
	int   Address;/*解码器地址编号，一个串口可以连接多个解码器，每个解码器都有一个地址编号*/
	int   BaudRate;
	int   ByteSize;
	int   Parity;
	int   StopBits;
}ZWPTZParam;

#pragma pack(pop)

/*返回用于PTZ控制的句柄*/
ZW_PTZ_API handle __cdecl InitialPTZ(ZWPTZParam* ptzParam, ZWVideoParam* videoParam);

/*执行PTZ指令。param1-pan speed和其它操作的speed，param2-tilt speed
**pan和tilt的speed为1-64，其它操作的speed为1-8
**其它操作如果不支持speed直接忽略
**当cmd为ZW_ZOOM_STEP时，param1为负值表示从当前位置ZW_ZOOM_OUT多少级别，正值表示ZW_ZOOM_IN多少级别
**
**
**返回 1-表示执行成功 0-表示执行失败 -1-表示不支持的操作
*/
ZW_PTZ_API int   __cdecl ExecPTZ(handle hPTZ,int cmd,int param1,int param2);

/*返回设备是否支持3D定位。支持的话返回1，否则返回0*/
ZW_PTZ_API int    __cdecl CanDirectTo(handle hPTZ);

/*调用3D定位功能。
**zoom 表示3D定位的时候是否执行ZOOM操作，0表示不执行，1表示执行
**vw、vh 视频播放区域的宽度、高度   
**rx、ry、rw、rh   3D定位功能框选框的x坐标、y坐标、宽度、高度
**3D定位功能框选框的坐标系的定定义如下：
**坐标原点在视频播放区域的左上角，y坐标向下，x坐标向右
**
**
**返回 1-表示执行成功 0-表示执行失败
*/
ZW_PTZ_API int   __cdecl DirectTo(handle hPTZ,int zoom,int vw,int vh,int rx, int ry, int rw,int rh);

/*销毁用于PTZ控制的句柄*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ);



typedef handle (__cdecl* INITIAL_PTZ_PROC)(ZWPTZParam*, ZWVideoParam*);

typedef int    (__cdecl* EXEC_PTZ_PROC)(handle ,int ,int ,int );

typedef int    (__cdecl* CAN_DIRECT_TO_PROC)(handle);

typedef int    (__cdecl* DIRECT_TO_PROC)(handle,int ,int ,int ,int , int , int ,int );

typedef void   (__cdecl* FINIALIZE_PTZ_PROC)(handle);



#ifdef __cplusplus
}
#endif

#endif