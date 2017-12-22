#ifndef __PTZLISTENER_H__
#define __PTZLISTENER_H__

#include <windows.h>
#include <process.h>

#include <assert.h>

#include "PTZ.h"

//���еĳ������Ͷ�����X86�������ֽ����ʾ����С���ֽ��򣬵��ڴ��ַ�ռ�洢���ݵĵ�λ��
//������Ϣ���͵Ŀ�ʼֵ��
#define MSG_USER_START						0x9000

//ö�ٴ���
enum EnumErrorCode
{
	MSG_SUCCESSFUL = 0,			//����ɹ�ִ�гɹ�	
	MSG_ERROR_LENGTH = 100,		//���յ�������Ȳ���
	MSG_ERROR_DB = 101,			//���ݿ��������
	MSG_ERROR_TIMEOUT = 65535,	//��ʱ
	MSG_ERROR_IGNORE = 	102, //ǰ�˷�æ�������޷�ִ�У�������.
	MSG_IGNORE = 	103, //ǰ���Ѿ����ڳ�ʼλ�ã�����Ҫ����
};

//��Ϣ�ṹ����
enum Msg_FullPTZ
{
	MSG_MFPTZ    =  MSG_USER_START + 48,
	MSG_MFPTZ_FB =  MSG_USER_START + 49,
	MSG_MFMarqueeZoom =  MSG_USER_START + 50,
	MSG_MFMarqueeZoom_FB =  MSG_USER_START + 51,
	MSG_MFMarqueeCapture =  MSG_USER_START + 52,
	MSG_MFMarqueeCapture_FB =  MSG_USER_START + 53,
};

#pragma pack(push) 
#pragma pack (1)

//������Ϣ�ṹ
//ÿ����Ϣ�������һ����������������صĲ�������
struct MsgBase
{
	MsgBase()
	{
		MsgSize = 0;
		MsgPackNum = 0;
		MsgCode = 0;		
		MsgSuccess = -1;
	}
	UINT32 MsgSize;				//�˳��Ȳ����������4���ֽ�
	UINT32 MsgPackNum;			//����ţ���0��ʼ������4���ֽڵĳ���
	UINT32 MsgCode;				//��Ϣ������ ��Msg_FullPTZ����
	UINT32 MsgSuccess;			//0�ɹ�����0ʧ��
};

struct MFPTZ : public MsgBase
{
	UINT32 nAction; //��ֵ��PTZ.h�е�ptzָ���
	// ���ϡ����¡����ϡ������ƶ���Ҫ2������ pamam1��ʾˮƽ�ٶȣ�param2��ʾ��ֱ�ٶȣ�
	// ������������ֻ��param1���ٶȷ�Χ0��64��	
	INT32 nParam1;
	INT32 nParam2;
};


struct MFMarqueeZoom : public MsgBase
{
	UINT16 nPlayWndWidth;
	UINT16 nPlayWndHeight;
	UINT16 nMarqueeLeft;
	UINT16 nMarqueeTop;
	UINT16 nMarqueeWidth;
	UINT16 nMarqueeHeight;
};

struct MFMarqueeCapture:public MsgBase
{
	UINT16 nPlayWndWidth;
	UINT16 nPlayWndHeight;
	UINT16 nMarqueeLeft;
	UINT16 nMarqueeTop;
	UINT16 nMarqueeWidth;
	UINT16 nMarqueeHeight;
	char pszUserName[32];
};

#pragma pack(pop)

typedef int (CALLBACK* PTZLISTENERPROC)(LPVOID,LPVOID,int len); 

class CPTZListener
{
public:
	CPTZListener(ULONG ip,int nPort);
	~CPTZListener(void);

	BOOL Start(
		const PTZLISTENERPROC lpPTZListenerFunc,//�ص����������
		const LPVOID lpParam//�ص������Ĳ���
		);

	BOOL Stop();

private:
	SOCKET m_sock;

private:
	PTZLISTENERPROC   m_lpPTZListenerFunc;
	LPVOID            m_lpParam;
	HANDLE            m_hListenThread;
	BOOL              m_bRun;

	static UINT __stdcall ListenThreadFunc(LPVOID lpParam);
	
private:
	CPTZListener(const CPTZListener&) {};
	CPTZListener& operator=(const CPTZListener&) {};
};

#endif
