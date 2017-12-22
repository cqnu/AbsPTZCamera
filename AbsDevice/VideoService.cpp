#include "StdAfx.h"

#include "VideoService.h"
#include "PtzCamera.h"



/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
/*������쳣ֵ�Ķ���ο�Video.h�ļ���ERR_MSG�Ķ���*/
VideoService::VideoService(
		int id,/*�豸id,ͬһ���豸��ͬһ�������е�idӦ��һ��*/
	    ZWVideoParam* videoParam,/*��ƵԴ����*/
	    VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
	    RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
	    RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
	    int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
	    TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
	    TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
	    int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
	    void *pUserData /*= NULL /*�û�����*/
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
