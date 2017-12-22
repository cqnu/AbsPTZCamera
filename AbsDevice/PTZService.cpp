#include "StdAfx.h"

#include "PTZService.h"
#include "PtzCamera.h"

/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
/*idΪ��̨������,ͬһ���豸��ͬһ�������е�idӦ��Ψһ*/
PTZService::PTZService(int id, ZWPTZParam* ptzParam, ZWVideoParam* videoParam)
{
	m_pPtzCamera = NULL;
	PtzCamera* pPtzCamera = PtzCamera::GetInstance(
		id,
		ptzParam,
		videoParam,
		NULL,
		NULL,
		NULL,
		0,
		NULL,
		NULL,
		0,
		NULL);
	m_pPtzCamera = pPtzCamera;
}

PTZService::~PTZService(void)
{
	((PtzCamera*)m_pPtzCamera)->ReleaseInstance();
}

/*ִ��PTZָ�param1-pan speed������������speed��param2-tilt speed
**pan��tilt��speedΪ1-64������������speedΪ1-8
**�������������֧��speedֱ�Ӻ���
**��cmdΪZW_ZOOM_STEPʱ��param1Ϊ��ֵ��ʾ�ӵ�ǰλ��ZW_ZOOM_OUT���ټ�����ֵ��ʾZW_ZOOM_IN���ټ���
**
**
**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ�� -1-��ʾ��֧�ֵĲ���
*/
int   PTZService::ExecPTZ(int cmd,int param1,int param2)
{
	return ((PtzCamera*)m_pPtzCamera)->ExecPTZ(cmd,param1,param2);
}

/*�����豸�Ƿ�֧��3D��λ��֧�ֵĻ�����1�����򷵻�0*/
int    PTZService::CanDirectTo()
{
	return 	((PtzCamera*)m_pPtzCamera)->CanDirectTo();
}

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
int   PTZService::DirectTo(int zoom,int vw,int vh,int rx, int ry, int rw,int rh)
{
	return ((PtzCamera*)m_pPtzCamera)->DirectTo(zoom,vw,vh,rx,ry,rw,rh);
}
