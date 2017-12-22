#include "StdAfx.h"

#include "VideoService.h"
#include "PtzCamera.h"



/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
/*捕获的异常值的定义参考Video.h文件中ERR_MSG的定义*/
VideoService::VideoService(
		int id,/*设备id,同一个设备在同一个进程中的id应该一样*/
	    ZWVideoParam* videoParam,/*视频源参数*/
	    VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
	    RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
	    RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
	    int realFixFmt,/*实时转码图像的格式*/
	    TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
	    TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
	    int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
	    void *pUserData /*= NULL /*用户数据*/
		)
{
	m_pPtzCamera = NULL;
	PtzCamera* pPtzCamera = PtzCamera::GetInstance(
		id,
		NULL,
		videoParam,
		OnVideoStreamReady,
		OnRealImgReady,
		OnRealFixFmtImgReady,
		realFixFmt,
		OnTriggerImgReady,
		OnTriggerFixFmtImgReady,
		triggerFixFmt,
		pUserData);
	m_pPtzCamera = pPtzCamera;
}

VideoService::~VideoService(void)
{
	((PtzCamera*)m_pPtzCamera)->StopVideoSource();
	((PtzCamera*)m_pPtzCamera)->ReleaseInstance();
}

int  VideoService::GetCodec()
{
	return ((PtzCamera*)m_pPtzCamera)->GetCodec();
}

int  VideoService::TriggerPeripheral(int func)
{
	return ((PtzCamera*)m_pPtzCamera)->TriggerPeripheral(func);
}

int  VideoService::StartVideoSource(handle hPlayWnd)
{
	return ((PtzCamera*)m_pPtzCamera)->StartVideoSource(hPlayWnd);
}

void  VideoService::StopVideoSource()
{
	((PtzCamera*)m_pPtzCamera)->StopVideoSource();
}
