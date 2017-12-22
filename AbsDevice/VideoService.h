#pragma once

#include "Video.h"

class ZW_VIDEO_API VideoService
{
public:
	/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
	/*������쳣ֵ�Ķ���ο�Video.h�ļ���ERR_MSG�Ķ���*/
	VideoService(
		int id,/*idΪ��̨������,ͬһ���豸��ͬһ�������е�idӦ��Ψһ*/
		ZWVideoParam* param,/*��ƵԴ����*/
		VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
		RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
		RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
		int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
		TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
		TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
		int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
		void *pUserData = NULL /*�û�����*/);
	~VideoService(void);

public:
	/*����ֵ��ZW_CODEC_ID_?����*/
	int  GetCodec();
	/*������Ƶ�豸���蹦�ܣ�����0ʧ�ܣ�1�ɹ���*/
	/*����豸��֧�ֻ���Ӧ�ò���Ҫ���Ͳ���ʵ������ӿ�*/
	int  TriggerPeripheral(int func);
	/*������ƵԴ������0ʧ�ܣ�1�ɹ�*/
	int  StartVideoSource(handle hPlayWnd);
	/*��ͣ��ƵԴ�����øú�������Ȼ���Ե���StartVideoSource����*/
	void StopVideoSource();

private:
	void *m_pPtzCamera;
};
