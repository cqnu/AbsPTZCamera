#pragma once

#include "Video.h"

class ZW_VIDEO_API VideoService
{
public:
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考Video.h文件中ERR_MSG的定义*/
	VideoService(
		int id,/*id为云台相机编号,同一个设备在同一个进程中的id应该唯一*/
		ZWVideoParam* param,/*视频源参数*/
		VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
		RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
		RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
		int realFixFmt,/*实时转码图像的格式*/
		TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
		TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
		int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
		void *pUserData = NULL /*用户数据*/);
	~VideoService(void);

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
	void *m_pPtzCamera;
};
