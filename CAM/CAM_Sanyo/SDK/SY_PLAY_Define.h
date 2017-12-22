
#pragma once

////////////////////////////////////////////////////////////////////////////////

#pragma pack( push )

typedef struct tagSY_EXTRADATA
{
	SYSTEMTIME	sTime		;
	LPBYTE		pExtraData	;
	DWORD		dwExtraData	;

} SY_EXTRADATA, *LPSY_EXTRADATA ;

typedef struct tagSY_FRAME
{
	LPBYTE	pData		;
	DWORD	dwDataSize	;

	LPBYTE	pRaw		;
	DWORD	dwRawSize	;
	
	LPSY_CODEC_STREAM_STATUS	pCodecStatus ;
	LPSY_EXTRADATA				pExData	;

} SY_FRAME, *LPSY_FRAME ;

#pragma pack( pop )

////////////////////////////////////////////////////////////////////////////////

// 显示回调函数定义
// dc			显示设备上下文
// nWidth		显示图像宽
// nHeight		显示图像高
// pvParam		显示回调函数参数
typedef void ( CALLBACK *LPDisplayCallBack )( HDC dc, int nWidth, int nHeight, LPVOID pvParam ) ;

// 解码回调函数定义
// nImgWidth	解码图像宽
// nImgHeight	解码图像高
// pBuffer		图像指针
// dwImgSize	当前图像大小
// pvParam		解码回调函数参数
typedef void ( CALLBACK *LPDecodeCallBack )( int nImgWidth, int nImgHeight,
											LPBYTE pBuffer, DWORD dwImgSize, LPVOID pvParam ) ;

// 帧解析回调函数定义
// nParserID	帧解析器句柄
// pFrame		帧信息
// nVersion		pFrame指向结构版本号
// pvParam		帧解析回调函数参数
typedef void ( CALLBACK *LPParseCallBack )( long nParserID, LPSY_FRAME pFrame, int nVersion, LPVOID pvParam ) ;

// 帧解析索引回调函数定义
// nParserID	帧解析器句柄
// nVer			类型号
// dwOffset		偏移
// pvParam		帧解析索引回调函数参数
typedef void ( CALLBACK *LPIndexCallBack )( long nParserID, int nVer, DWORD dwOffset, LPVOID pvParam ) ;

////////////////////////////////////////////////////////////////////////////////

// 播放器解码模式
enum SY_DECODE_MODE{ 
	SY_DECODE_MODE_NETSTREAM = 0xF001,	// 网络流模式默认
	SY_DEOCDE_MODE_FILE					// 文件模式
};

// 播放器
enum SY_PLAY_SPEED{
	SY_PLAY_SPEED_NORMAL = 0,	// 正常速度
	SY_PLAY_SPEED_FAST,			// 加速
	SY_PLAY_SPEED_SLOW			// 减速
};

// 文件模式播放控制
enum SY_PLAY_FILECTRL{
	SY_PLAY_FILECTRL_RESET = 0,	// 恢复播放
	SY_PLAY_FILECTRL_PAUSE,		// 播放暂停
	SY_PLAY_FILECTRL_FRAMEPLAY,	// 单帧播放
};

////////////////////////////////////////////////////////////////////////////////
// Parser Buffer Size

#define PBS_1MB		1024 * 1024
#define PBS_2MB		PBS_1MB * 2
#define PBS_4MB		PBS_2MB * 2

////////////////////////////////////////////////////////////////////////////////
