
#pragma once

#include "SY_NET_Define.h"

#ifdef	SY_SDK
#define	SY_API	extern "C"	__declspec( dllexport )
#else
#define	SY_API	extern "C"	__declspec( dllimport )
#endif

////////////////////////////////////////////////////////////////////////////////

// 登陆设备
// pszIP		设备IP
// wPort		设备UDP单播端口
// pszUserName	登陆用户名
// pszPassword	登陆密码
// 返回值		设备登陆句柄
SY_API long WINAPI SY_NET_Login( LPCTSTR pszIP, WORD wPort, LPCTSTR pszUserName, LPCTSTR pszPassword ) ;

// 注销设备
// nCameraID	设备登陆句柄
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_Logout( long nCameraID ) ;

// 设置设备异常处理回调
// nCameraID	设备登陆句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_SetExceptionCallBack( long nCameraID, LPExceptionCallBack pCallBack, LPVOID pvParam ) ;

// 设置实时码流回调
// nCameraID	设备登陆句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_SetRealDataCallBack( long nCameraID, LPRealDataCallBack pCallBack, LPVOID pvParam ) ;

// 开始获取设备实时码流
// nCameraID	设备登陆句柄
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_RealPlay( long nCameraID ) ;

// 停止获取设备实时码流
// nCameraID	设备登陆句柄
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_StopRealPlay( long nCameraID ) ;

// 设置实时JPEG回调
// nCameraID	设备登陆句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_SetRealJpegCallBack( long nCameraID, LPRealJpegCallBack pCallBack, LPVOID pvParam ) ;

// 开始获取JPEG图像
SY_API long WINAPI SY_NET_RealJpeg( long nCameraID ) ;

// 停止获取JPEG图像
SY_API long WINAPI SY_NET_StopRealJpeg( long nCameraID ) ;

// 获取设备参数
// nDeviceID	设备登陆句柄
// syds			参数类型
// pPara		参数指针
// nParaSize	参数大小
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_GetDeviceSetting( long nDeviceID, SY_DEVICE_SETTING syds, LPVOID pPara, int nParaSize ) ;

// 设备控制
// nCameraID	设备登陆句柄
// sycc			设备控制类型
// dwParam		设备控制参数
SY_API long WINAPI SY_NET_CameraControl( long nCameraID, SY_CAMERA_CONTROL_TYPE sycc, DWORD dwParam ) ;

// 设置设备消息回调
// nCameraID	设备登陆句柄
// dwInterval	同一类型消息通知间隔(单位:毫秒)
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_NET_SetDeviceMessageCallBack( long nCameraID, DWORD dwInterval, LPDeviceMessageCallBack pCallBack, LPVOID pvParam ) ;

// 设置用户自定义数据
// nCameraID	设备登陆句柄
// pExtraData	自定义数据指针
// dwDataSize	自定义数据大小
// 返回值		1 成功, 0失败
// *注意: 自定义数据大小最大值为 1024
SY_API long WINAPI SY_NET_SetExtraData( long nCameraID, LPBYTE pExtraData, DWORD dwDataSize ) ;

////////////////////////////////////////////////////////////////////////////////
// 已经不再支持的函数

// 获取设备设置
// nCameraID	设备登陆句柄
// pCamSet		设置
// 返回值		1 成功, 0失败
// SY_API long WINAPI SY_NET_GetCameraSetting( long nCameraID, LPSY_CAMERA_SETTING pCamSet ) ;

// 获取设备解码参数
// nCameraID	设备登陆句柄
// pStatus		解码参数
// 返回值		1 成功, 0失败
// SY_API long WINAPI SY_NET_GetCodecStreamStatus( long nCameraID, LPSY_CODEC_STREAM_STATUS pStatus ) ;
// SY_API long WINAPI SY_NET_GetCodecStreamStatus2( long nCameraID, LPSY_CODEC_STREAM_STATUS2 pStatus ) ;

// 设备AlarmOut
// nCameraID		设备登陆句柄
// pAlarmOut		AlarmOut参数
// SY_API long WINAPI SY_NET_CameraAlarmOut( long nCameraID, LPSY_ALARMOUT pAlarmOut ) ;

// 设备TestAlarmOut
// nCameraID		设备登陆句柄
// tao				类型
// dwSleep			间隔
// SY_API long WINAPI SY_NET_TestAlarmOut( long nCameraID, SY_CAMERA_TEST_ALARMOUT tao, DWORD dwSleep = 0, DWORD dwOnOff = 0 ) ;

////////////////////////////////////////////////////////////////////////////////
