#ifndef __PTZ_H__
#define __PTZ_H__

#if defined(_MSC_VER) || defined(__MINGW32__)
  #define HELPER_DLL_IMPORT __declspec(dllimport)
  #define HELPER_DLL_EXPORT __declspec(dllexport)
  #define HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define HELPER_DLL_IMPORT __attribute__((dllimport))
    #define HELPER_DLL_EXPORT __attribute__((dllexport))
    #define HELPER_DLL_LOCAL
  #endif
#endif

#ifdef ZW_PTZ_DLL_EXPORTS
  #define ZW_PTZ_API HELPER_DLL_EXPORT
#elif ZW_PTZ_DLL_IMPORTS
  #define ZW_PTZ_API HELPER_DLL_IMPORT
#endif

#define ZW_PTZ_LOCAL HELPER_DLL_LOCAL


/*************PTZ�˶����ʵز���******************/

/*��̨ת��*/
#define ZW_TILT_UP				        0x0100
#define ZW_TILT_DOWN				    0x0101
#define ZW_PAN_LEFT				        0x0102
#define ZW_PAN_RIGHT			        0x0103

#define ZW_LEFTUP			            0x0104
#define ZW_LEFTDOWN			            0x0105
#define ZW_RIGHTUP			            0x0106
#define ZW_RIGHTDOWN		            0x0107

/*��С���࣬Ч������Զ*/
#define ZW_ZOOM_OUT                     0x0108

/*���ӽ��࣬Ч��������*/
#define ZW_ZOOM_IN                      0x0109

/*�ӵ�ǰ״̬����������������Զһ���ȼ�*/
#define ZW_ZOOM_STEP                    0x0110


/*************��PTZ�˶����ʵز���******************/

#define ZW_PTZ_NONE				        0x0200
									
/*�Խ����������ý�����������*/		
#define ZW_FOCUS_NEAR		            0x0201
									
/*�Խ���Զ������Զ����������*/		
#define ZW_FOCUS_FAR		            0x0202
									
/*�Ӵ��Ȧ*/						
#define ZW_IRIS_OPEN		            0x0203
									
/*��С��Ȧ*/						
#define ZW_IRIS_CLOSE		            0x0204
									
/*����ƿ�*/						
#define ZW_LIGHT_ON		                0x0205
									
/*��ˢ��*/							
#define ZW_WIPER_ON		                0x0206
									
/*����Ԥ��λ*/						
#define ZW_SET_PRESET		            0x0207

/*ɾ��Ԥ��λ*/
#define ZW_DEL_PRESET		            0x0208

/*************����ֹͣ���ʵز���******************/

/*ֹͣ���в���*/
#define ZW_STOP_ALL		                0x0300

#define ZW_STOP_PAN 		            0x0301
#define ZW_STOP_TILT		            0x0302
#define ZW_STOP_ZOOM		            0x0303
#define ZW_STOP_FOCUS		            0x0304
#define ZW_STOP_IRIS		            0x0305

#define ZW_LIGHT_OFF		            0x0306
#define ZW_WIPER_OFF		            0x0307

/*����Ԥ��λ*/
#define ZW_USE_PRESET		            0x0318
/*���Զ�ץ��λ��*/
#define ZW_AUTO_POS    		            0x0319


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ZW_ERR_MSG
#define ZW_ERR_MSG

/*δ�������*/
#define ZW_ERR_NOT_DEFINED        (int(-1))
/*���·��Ϊ��*/
#define ZW_ERR_PATH_IS_EMPTY      (int(0))
/*���������*/
#define ZW_ERR_PATH_NOT_EXIST     (int(1))
/*���DLL����ʧ��*/
#define ZW_ERR_LOAD_DLL_FAIL      (int(2))
/*������Ч��PTZ���*/
#define ZW_ERR_PTZ_ADDIN_FAULT    (int(3))
/*������Ч��VIDEO���*/
#define ZW_ERR_VIDEO_ADDIN_FAULT  (int(4))
/*PTZ�豸1����*/
#define ZW_ERR_PTZ1_FAULT         (int(5))
/*PTZ�豸2����*/
#define ZW_ERR_PTZ2_FAULT         (int(6))
/*VIDEO�豸����*/
#define ZW_ERR_VIDEO_FAULT        (int(7))
/*�豸�Ѿ�����*/
#define ZW_ERR_ALREADY_OPEN       (int(8))
/*��֧�ֵĵ���*/
#define ZW_ERR_UNSUPPORT_CALL        (int(9))

#endif

#ifndef ZW_HANDLE_DEFINED
#define ZW_HANDLE_DEFINED

/*������Ͷ���*/
typedef void*  handle;

#define ZW_LENGTH_IP_ADDR              16
#define ZW_LENGTH_USER_NAME            32
#define ZW_LENGTH_PASSWORD             32
#define ZW_LENGTH_URL                  256

#endif

#pragma pack(push)
#pragma pack(1)

#ifndef ZW_VIDEO_PARAM
#define ZW_VIDEO_PARAM

typedef struct __ZWVideoParam
{
	char                CameraIp[ZW_LENGTH_IP_ADDR];/*�����������������ƵԴ�豸��ip*/
	char                LocalIp[ZW_LENGTH_IP_ADDR];/*����IP*/
	unsigned short      CameraPort;/*��ƵԴ�˿�*/
	char                UserName[ZW_LENGTH_USER_NAME];
	char                Password[ZW_LENGTH_PASSWORD];
	int                 AddressNum;/*��ַ��ţ�Ҳ��Ϊͨ����*/
	union
	{
		char            AddressName[ZW_LENGTH_URL];/*��ַ���ƣ�Ҳ��Ϊͨ������*/
		char            Url[ZW_LENGTH_URL];/*��ƵԴ��url����rtsp://192.168.1.77:8557/h264*/
	};
}ZWVideoParam;

#endif

typedef struct __ZWPTZParam
{
	char  DeviceIp[ZW_LENGTH_IP_ADDR];
	char  LocalIp[ZW_LENGTH_IP_ADDR];/*����IP*/
	int   DevicePort;
	char  UserName[ZW_LENGTH_USER_NAME];
	char  Password[ZW_LENGTH_PASSWORD];
	int   Number;/*���ڱ�ţ���COM1 COM2*/
	int   Address;/*��������ַ��ţ�һ�����ڿ������Ӷ����������ÿ������������һ����ַ���*/
	int   BaudRate;
	int   ByteSize;
	int   Parity;
	int   StopBits;
}ZWPTZParam;

#pragma pack(pop)

/*��������PTZ���Ƶľ��*/
ZW_PTZ_API handle __cdecl InitialPTZ(ZWPTZParam* ptzParam, ZWVideoParam* videoParam);

/*ִ��PTZָ�param1-pan speed������������speed��param2-tilt speed
**pan��tilt��speedΪ1-64������������speedΪ1-8
**�������������֧��speedֱ�Ӻ���
**��cmdΪZW_ZOOM_STEPʱ��param1Ϊ��ֵ��ʾ�ӵ�ǰλ��ZW_ZOOM_OUT���ټ�����ֵ��ʾZW_ZOOM_IN���ټ���
**
**
**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ�� -1-��ʾ��֧�ֵĲ���
*/
ZW_PTZ_API int   __cdecl ExecPTZ(handle hPTZ,int cmd,int param1,int param2);

/*�����豸�Ƿ�֧��3D��λ��֧�ֵĻ�����1�����򷵻�0*/
ZW_PTZ_API int    __cdecl CanDirectTo(handle hPTZ);

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
ZW_PTZ_API int   __cdecl DirectTo(handle hPTZ,int zoom,int vw,int vh,int rx, int ry, int rw,int rh);

/*��������PTZ���Ƶľ��*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ);



typedef handle (__cdecl* INITIAL_PTZ_PROC)(ZWPTZParam*, ZWVideoParam*);

typedef int    (__cdecl* EXEC_PTZ_PROC)(handle ,int ,int ,int );

typedef int    (__cdecl* CAN_DIRECT_TO_PROC)(handle);

typedef int    (__cdecl* DIRECT_TO_PROC)(handle,int ,int ,int ,int , int , int ,int );

typedef void   (__cdecl* FINIALIZE_PTZ_PROC)(handle);



#ifdef __cplusplus
}
#endif

#endif