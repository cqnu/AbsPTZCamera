#ifndef __PTZLISTENER_H__
#define __PTZLISTENER_H__

#include <windows.h>
#include <process.h>

#include <assert.h>

#include "PTZ.h"

//所有的长度类型都采用X86机器的字节序表示，既小端字节序，低内存地址空间存储数据的低位。
//定义消息类型的开始值：
#define MSG_USER_START						0x9000

//枚举错误
enum EnumErrorCode
{
	MSG_SUCCESSFUL = 0,			//命令成功执行成功	
	MSG_ERROR_LENGTH = 100,		//接收到的命令长度不对
	MSG_ERROR_DB = 101,			//数据库操作有误
	MSG_ERROR_TIMEOUT = 65535,	//超时
	MSG_ERROR_IGNORE = 	102, //前端繁忙！命令无法执行，被忽略.
	MSG_IGNORE = 	103, //前端已经处在初始位置，不需要动作
};

//消息结构类型
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

//基本消息结构
//每条消息（命令）由一条命令和与该命令相关的参数构成
struct MsgBase
{
	MsgBase()
	{
		MsgSize = 0;
		MsgPackNum = 0;
		MsgCode = 0;		
		MsgSuccess = -1;
	}
	UINT32 MsgSize;				//此长度不包括本身的4个字节
	UINT32 MsgPackNum;			//包序号，从0开始递增，4个字节的长度
	UINT32 MsgCode;				//消息的类型 由Msg_FullPTZ定义
	UINT32 MsgSuccess;			//0成功，非0失败
};

struct MFPTZ : public MsgBase
{
	UINT32 nAction; //该值由PTZ.h中的ptz指令定义
	// 左上、左下、右上、右下移动需要2个参数 pamam1表示水平速度，param2表示垂直速度，
	// 其它动作类型只用param1。速度范围0到64。	
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
		const PTZLISTENERPROC lpPTZListenerFunc,//回调方法的入口
		const LPVOID lpParam//回调方法的参数
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
