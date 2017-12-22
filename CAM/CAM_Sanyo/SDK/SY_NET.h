
#pragma once

#include "SY_NET_Define.h"

#ifdef	SY_SDK
#define	SY_API	extern "C"	__declspec( dllexport )
#else
#define	SY_API	extern "C"	__declspec( dllimport )
#endif

////////////////////////////////////////////////////////////////////////////////

// ��½�豸
// pszIP		�豸IP
// wPort		�豸UDP�����˿�
// pszUserName	��½�û���
// pszPassword	��½����
// ����ֵ		�豸��½���
SY_API long WINAPI SY_NET_Login( LPCTSTR pszIP, WORD wPort, LPCTSTR pszUserName, LPCTSTR pszPassword ) ;

// ע���豸
// nCameraID	�豸��½���
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_Logout( long nCameraID ) ;

// �����豸�쳣����ص�
// nCameraID	�豸��½���
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_SetExceptionCallBack( long nCameraID, LPExceptionCallBack pCallBack, LPVOID pvParam ) ;

// ����ʵʱ�����ص�
// nCameraID	�豸��½���
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_SetRealDataCallBack( long nCameraID, LPRealDataCallBack pCallBack, LPVOID pvParam ) ;

// ��ʼ��ȡ�豸ʵʱ����
// nCameraID	�豸��½���
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_RealPlay( long nCameraID ) ;

// ֹͣ��ȡ�豸ʵʱ����
// nCameraID	�豸��½���
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_StopRealPlay( long nCameraID ) ;

// ����ʵʱJPEG�ص�
// nCameraID	�豸��½���
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_SetRealJpegCallBack( long nCameraID, LPRealJpegCallBack pCallBack, LPVOID pvParam ) ;

// ��ʼ��ȡJPEGͼ��
SY_API long WINAPI SY_NET_RealJpeg( long nCameraID ) ;

// ֹͣ��ȡJPEGͼ��
SY_API long WINAPI SY_NET_StopRealJpeg( long nCameraID ) ;

// ��ȡ�豸����
// nDeviceID	�豸��½���
// syds			��������
// pPara		����ָ��
// nParaSize	������С
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_GetDeviceSetting( long nDeviceID, SY_DEVICE_SETTING syds, LPVOID pPara, int nParaSize ) ;

// �豸����
// nCameraID	�豸��½���
// sycc			�豸��������
// dwParam		�豸���Ʋ���
SY_API long WINAPI SY_NET_CameraControl( long nCameraID, SY_CAMERA_CONTROL_TYPE sycc, DWORD dwParam ) ;

// �����豸��Ϣ�ص�
// nCameraID	�豸��½���
// dwInterval	ͬһ������Ϣ֪ͨ���(��λ:����)
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_NET_SetDeviceMessageCallBack( long nCameraID, DWORD dwInterval, LPDeviceMessageCallBack pCallBack, LPVOID pvParam ) ;

// �����û��Զ�������
// nCameraID	�豸��½���
// pExtraData	�Զ�������ָ��
// dwDataSize	�Զ������ݴ�С
// ����ֵ		1 �ɹ�, 0ʧ��
// *ע��: �Զ������ݴ�С���ֵΪ 1024
SY_API long WINAPI SY_NET_SetExtraData( long nCameraID, LPBYTE pExtraData, DWORD dwDataSize ) ;

////////////////////////////////////////////////////////////////////////////////
// �Ѿ�����֧�ֵĺ���

// ��ȡ�豸����
// nCameraID	�豸��½���
// pCamSet		����
// ����ֵ		1 �ɹ�, 0ʧ��
// SY_API long WINAPI SY_NET_GetCameraSetting( long nCameraID, LPSY_CAMERA_SETTING pCamSet ) ;

// ��ȡ�豸�������
// nCameraID	�豸��½���
// pStatus		�������
// ����ֵ		1 �ɹ�, 0ʧ��
// SY_API long WINAPI SY_NET_GetCodecStreamStatus( long nCameraID, LPSY_CODEC_STREAM_STATUS pStatus ) ;
// SY_API long WINAPI SY_NET_GetCodecStreamStatus2( long nCameraID, LPSY_CODEC_STREAM_STATUS2 pStatus ) ;

// �豸AlarmOut
// nCameraID		�豸��½���
// pAlarmOut		AlarmOut����
// SY_API long WINAPI SY_NET_CameraAlarmOut( long nCameraID, LPSY_ALARMOUT pAlarmOut ) ;

// �豸TestAlarmOut
// nCameraID		�豸��½���
// tao				����
// dwSleep			���
// SY_API long WINAPI SY_NET_TestAlarmOut( long nCameraID, SY_CAMERA_TEST_ALARMOUT tao, DWORD dwSleep = 0, DWORD dwOnOff = 0 ) ;

////////////////////////////////////////////////////////////////////////////////
