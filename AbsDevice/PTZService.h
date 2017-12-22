#pragma once

#include "PTZ.h"

class ZW_PTZ_API PTZService
{
public:
	/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
	/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
	/*idΪ��̨������,ͬһ���豸��ͬһ�������е�idӦ��Ψһ*/
	PTZService(int id, ZWPTZParam* ptzParam, ZWVideoParam* videoParam);
	~PTZService(void);

public:
	/*ִ��PTZָ�param1-pan speed������������speed��param2-tilt speed
	**pan��tilt��speedΪ1-64������������speedΪ1-8
	**�������������֧��speedֱ�Ӻ���
	**��cmdΪZW_ZOOM_STEPʱ��param1Ϊ��ֵ��ʾ�ӵ�ǰλ��ZW_ZOOM_OUT���ټ�����ֵ��ʾZW_ZOOM_IN���ټ���
	**
	**
	**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ�� -1-��ʾ��֧�ֵĲ���
	*/
	int   ExecPTZ(int cmd,int param1,int param2);

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
	void *m_pPtzCamera;
};
