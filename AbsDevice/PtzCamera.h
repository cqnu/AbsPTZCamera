#pragma once


#include "PTZ.h"
#include "Video.h"

class ZW_VIDEO_API PtzCamera
{
public:
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考Video.h文件中ERR_MSG的定义*/
	/*当ptzParam等于NULL时不创建PTZService部分*/
	/*当videoParam等于NULL时不创建VideoService部分*/
	/*id为云台相机编号,同一个设备在同一个进程中的id应该唯一*/
	static PtzCamera* GetInstance(
		int id,/*id为云台相机编号,同一个设备在同一个进程中的id应该唯一*/
		ZWPTZParam* ptzParam,/*PTZ参数*/
		ZWVideoParam* videoParam,/*视频源参数*/
		VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
		RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
		RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
		int realFixFmt,/*实时转码图像的格式*/
		TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
		TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
		int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
		void *pUserData = NULL /*用户数据*/
		);

	void   ReleaseInstance();
private:
	static CMap<int, int,PtzCamera*, PtzCamera*> m_sPtzCameras;

	int     m_nRefCount;
	int     m_nId;  
private:
	bool m_bPtzPart;
	bool m_bVideoPart;
	bool m_bVideoPtz1;/*m_hVideo等于m_hPtz*/
	bool m_bVideoPtz2;/*m_hVideo等于m_hPtz2*/

	void CreatePtzPart(ZWPTZParam* ptzParam, ZWVideoParam* videoParam);
	void CreateVideoPart(
		ZWVideoParam* param,/*视频源参数*/
		VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
		RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
		RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
		int realFixFmt,/*实时转码图像的格式*/
		TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
		TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
		int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
		void *pUserData = NULL /*用户数据*/);

public:
	/*执行PTZ指令。param1-pan speed和其它操作的speed，param2-tilt speed
	**pan和tilt的speed为1-64，其它操作的speed为1-8
	**其它操作如果不支持speed直接忽略
	**当cmd为ZW_ZOOM_STEP时，param1为负值表示从当前位置ZW_ZOOM_OUT多少级别，正值表示ZW_ZOOM_IN多少级别
	**
	**
	**返回 1-表示执行成功 0-表示执行失败 -1-表示不支持的操作
	*/
	int    ExecPTZ(int cmd,int param1,int param2);

	/*返回设备是否支持3D定位。支持的话返回1，否则返回0*/
	int    CanDirectTo();

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
	int   DirectTo(int zoom,int vw,int vh,int rx, int ry, int rw,int rh);

private:
	handle m_hPTZ2;
	handle m_hPTZ;

	void* m_pPTZActivator;
	void* m_pPTZActivator2;

	ZWPTZParam       m_PTZParam;	
public:
	/*返回值由ZW_CODEC_ID_?定义*/
	int  GetCodec();
	/*触发视频设备外设功能，返回0失败，1成功。*/
	/*如果设备不支持或者应用不需要，就不用实现这个接口*/
	int  TriggerPeripheral(int func);
	/*开启视频源，返回0失败，1成功*/
	int  StartVideoSource(handle hPlayWnd);
	/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
	void StopVideoSource();

private:
	static void __stdcall OnVideoStreamReadyProc(void *pUserData, void* buff, long size, int frametype);
	static void __stdcall OnRealImgReadyProc(void *pUserData, ZWImageData* img);
	static void __stdcall OnRealFixFmtImgReadyProc(void *pUserData, ZWImageData* img);
	static void __stdcall OnTriggerImgReadyProc(void *pUserData, ZWImageData* img, void* data, int func);
	static void __stdcall OnTriggerFixFmtImgReadyProc(void *pUserData, ZWImageData* img, void* data, int func);
	static UINT __stdcall ReconnectThreadFunc(LPVOID lpParam);

private:
	CRITICAL_SECTION m_csInitial;
	bool m_bRunReconnect;//是否运行重连线程，只要VideoService创建成功该字段就是true
	bool m_bManualStarted;//视频源是否被外部手动调用StartVideoSource开启过
	bool m_bVideoStarted;//视频源是否开启
	bool InitialVideoSource();
	void FinializeVideoSource();

	bool InitialPtzDevice();
	void FinializePtzDevice();

	int  StartVideoSource(bool manual, handle hPlayWnd);
	void StopVideoSource(bool manual);

private:
	int  m_nCodec;

private:
	VideoStreamCallback m_pfnOnVideoStreamReady;/*实时视频流回调*/
	RealTimeImageCallback m_pfnOnRealImgReady;/*实时图像输出回调*/
	RealTimeImageCallback m_pfnOnRealFixFmtImgReady;/*实时转码图像输出回调*/
	TriggerImageCallback m_pfnOnTriggerImgReady;  /*外部触发抓拍图片回调*/
	TriggerImageCallback m_pfnOnTriggerFixFmtImgReady;/*外部触发抓拍转码图片回调*/

	int m_nRealFixFmt;/*实时转码图像的格式*/
	int m_nTriggerFixFmt;/*外部触发抓拍转码图片的格式*/

	void*            m_pUserData;
	handle           m_hVideo;
	void*            m_pVideoActivator;

	handle           m_hPlayWnd;

	ZWVideoParam       m_VideoParam;	
	time_t           m_tmtLastRecvVideo;
	HANDLE           m_hReconnectThread;

private:
	PtzCamera(const PtzCamera &ins)
	{
		return;
	}
	PtzCamera(int id);
	~PtzCamera(void);
};

