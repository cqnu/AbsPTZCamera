#pragma once


#include "PTZ.h"
#include "Video.h"

class ZW_VIDEO_API PtzCamera
{
public:
	/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
	/*������쳣ֵ�Ķ���ο�Video.h�ļ���ERR_MSG�Ķ���*/
	/*��ptzParam����NULLʱ������PTZService����*/
	/*��videoParam����NULLʱ������VideoService����*/
	/*idΪ��̨������,ͬһ���豸��ͬһ�������е�idӦ��Ψһ*/
	static PtzCamera* GetInstance(
		int id,/*idΪ��̨������,ͬһ���豸��ͬһ�������е�idӦ��Ψһ*/
		ZWPTZParam* ptzParam,/*PTZ����*/
		ZWVideoParam* videoParam,/*��ƵԴ����*/
		VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
		RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
		RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
		int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
		TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
		TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
		int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
		void *pUserData = NULL /*�û�����*/
		);

	void   ReleaseInstance();
private:
	static CMap<int, int,PtzCamera*, PtzCamera*> m_sPtzCameras;

	int     m_nRefCount;
	int     m_nId;  
private:
	bool m_bPtzPart;
	bool m_bVideoPart;
	bool m_bVideoPtz1;/*m_hVideo����m_hPtz*/
	bool m_bVideoPtz2;/*m_hVideo����m_hPtz2*/

	void CreatePtzPart(ZWPTZParam* ptzParam, ZWVideoParam* videoParam);
	void CreateVideoPart(
		ZWVideoParam* param,/*��ƵԴ����*/
		VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
		RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
		RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
		int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
		TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
		TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
		int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
		void *pUserData = NULL /*�û�����*/);

public:
	/*ִ��PTZָ�param1-pan speed������������speed��param2-tilt speed
	**pan��tilt��speedΪ1-64������������speedΪ1-8
	**�������������֧��speedֱ�Ӻ���
	**��cmdΪZW_ZOOM_STEPʱ��param1Ϊ��ֵ��ʾ�ӵ�ǰλ��ZW_ZOOM_OUT���ټ�����ֵ��ʾZW_ZOOM_IN���ټ���
	**
	**
	**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ�� -1-��ʾ��֧�ֵĲ���
	*/
	int    ExecPTZ(int cmd,int param1,int param2);

	/*�����豸�Ƿ�֧��3D��λ��֧�ֵĻ�����1�����򷵻�0*/
	int    CanDirectTo();

	/*����3D��λ���ܡ�
	**zoom ��ʾ3D��λ��ʱ���Ƿ�ִ��ZOOM������0��ʾ��ִ�У�1��ʾִ��
	**vw��vh ��Ƶ��������Ŀ�ȡ��߶�   
	**rx��ry��rw��rh   3D��λ���ܿ�ѡ���x���ꡢy���ꡢ��ȡ��߶�
	**3D��λ���ܿ�ѡ�������ϵ�Ķ��������£�
	**����ԭ������Ƶ������������Ͻǣ�y�������£�x��������
	**
	**
	**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ��
	*/
	int   DirectTo(int zoom,int vw,int vh,int rx, int ry, int rw,int rh);

private:
	handle m_hPTZ2;
	handle m_hPTZ;

	void* m_pPTZActivator;
	void* m_pPTZActivator2;

	ZWPTZParam       m_PTZParam;	
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
	static void __stdcall OnVideoStreamReadyProc(void *pUserData, void* buff, long size, int frametype);
	static void __stdcall OnRealImgReadyProc(void *pUserData, ZWImageData* img);
	static void __stdcall OnRealFixFmtImgReadyProc(void *pUserData, ZWImageData* img);
	static void __stdcall OnTriggerImgReadyProc(void *pUserData, ZWImageData* img, void* data, int func);
	static void __stdcall OnTriggerFixFmtImgReadyProc(void *pUserData, ZWImageData* img, void* data, int func);
	static UINT __stdcall ReconnectThreadFunc(LPVOID lpParam);

private:
	CRITICAL_SECTION m_csInitial;
	bool m_bRunReconnect;//�Ƿ����������̣߳�ֻҪVideoService�����ɹ����ֶξ���true
	bool m_bManualStarted;//��ƵԴ�Ƿ��ⲿ�ֶ�����StartVideoSource������
	bool m_bVideoStarted;//��ƵԴ�Ƿ���
	bool InitialVideoSource();
	void FinializeVideoSource();

	bool InitialPtzDevice();
	void FinializePtzDevice();

	int  StartVideoSource(bool manual, handle hPlayWnd);
	void StopVideoSource(bool manual);

private:
	int  m_nCodec;

private:
	VideoStreamCallback m_pfnOnVideoStreamReady;/*ʵʱ��Ƶ���ص�*/
	RealTimeImageCallback m_pfnOnRealImgReady;/*ʵʱͼ������ص�*/
	RealTimeImageCallback m_pfnOnRealFixFmtImgReady;/*ʵʱת��ͼ������ص�*/
	TriggerImageCallback m_pfnOnTriggerImgReady;  /*�ⲿ����ץ��ͼƬ�ص�*/
	TriggerImageCallback m_pfnOnTriggerFixFmtImgReady;/*�ⲿ����ץ��ת��ͼƬ�ص�*/

	int m_nRealFixFmt;/*ʵʱת��ͼ��ĸ�ʽ*/
	int m_nTriggerFixFmt;/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/

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

