
#pragma once

#include "SY_PLAY_Define.h"

#ifdef	SY_SDK
#define	SY_API	extern "C"	__declspec( dllexport )
#else
#define	SY_API	extern "C"	__declspec( dllimport )
#endif

////////////////////////////////////////////////////////////////////////////////
// 编解码器

// 创建播放器
// 返回值		播放器句柄
SY_API long WINAPI SY_PLAY_CreatePlayer( SY_CODEC_STREAM_STATUS status ) ;
SY_API long WINAPI SY_PLAY_CreatePlayerEx( SY_CODEC_STREAM_STATUS status, DWORD dwPlayerVersion ) ;

// 关闭播放器
// nPlayerID	播放器句柄
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PLAY_ClosePlayer( long nPlayerID ) ;

// 设置播放器显示窗口
// nPlayerID	播放器句柄
// hWnd			窗口句柄
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PLAY_SetPlayerWindow( long nPlayerID, HWND hWnd ) ;

// 设置显示回调函数
// nPlayerID	播放器句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PLAY_SetDisplayCallBack( long nPlayerID, LPDisplayCallBack pCallBack, LPVOID pvParam ) ;

// 设置解码回调函数
// nPlayerID	播放器句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PLAY_SetDecodeCallBack( long nPlayerID, LPDecodeCallBack pCallBack, LPVOID pvParam ) ;

// 设置解码灰度图回调函数
// nPlayerID	播放器句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PLAY_SetDecodeGrayCallBack( long nPlayerID, LPDecodeCallBack pCallBack, LPVOID pvParam ) ;

// 解码并显示实时码流
// nPlayerID		播放器句柄
// pRealData		码流数据指针
// dwRealDataSize	码流数据大小
// 返回值			1 成功, 0失败
SY_API long WINAPI SY_PLAY_PlayerInputRealData( long nPlayerID, LPBYTE pRealData, DWORD dwRealDataSize ) ;

// 输入音频数据
// nPlayerID		播放器句柄
// pRealData		码流数据指针
// dwRealDataSize	码流数据大小
// 返回值			1 成功, 0失败
SY_API long WINAPI SY_PLAY_PlayerInputAudioData( long nPlayerID, LPBYTE pRealData, DWORD dwRealDataSize ) ;

// 开启或关闭播放器的解码器
// nPlayerID	播放器句柄
// bEnable		1 开启解码器, 0 关闭解码器
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PLAY_EnableDecoder( long nPlayerID, BOOL bEnable ) ;

// 设置播放器解码模式
// nPlayerID	播放器句柄
// modeDecode	解码模式
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PLAY_SetDecoderMode( long nPlayerID, SY_DECODE_MODE mode ) ;

// 设置播放速度
// nPlayerID	播放器句柄
// speed		速度模式
SY_API long WINAPI SY_PLAY_SetSpeed( long nPlayerID, SY_PLAY_SPEED speed ) ;

// 文件模式下的播放控制
// nPlayerID	播放器句柄
// ctrl			控制类型
// dwValue		控制参数
SY_API long WINAPI SY_PLAY_FileControl( long nPlayerID, SY_PLAY_FILECTRL ctrl, DWORD dwValue ) ;

// 开始将实时码流存储到文件中
// nPlayerID		播放器句柄
// pszFile			存储文件的全路径
// 返回值			1 成功, 0失败
SY_API long WINAPI SY_PLAY_StartSaveRealData( long nPlayerID, LPCTSTR pszFile ) ;

// 停止将实时码流存储到文件中
// nPlayerID		播放器句柄
// 返回值			1 成功, 0失败
SY_API long WINAPI SY_PLAY_StopSaveRealData( long nPlayerID ) ;

// 将当前图像保存为BMP图片
// nPlayerID		播放器句柄
// pszBmpFile		BMP图片文件路径
// 返回值			1 成功, 0失败
SY_API long WINAPI SY_PLAY_SaveBMPFile( long nPlayerID, LPCTSTR pszBmpFile ) ;

// 区域显示
// nPlayerID		播放器句柄
// pRECT			区域
SY_API long WINAPI SY_PLAY_StretchDisplay( long nPlayerID, LPRECT pRECT ) ;

////////////////////////////////////////////////////////////////////////////////
// 帧解析器

// 创建帧解析器
// 返回值		帧解析器句柄
SY_API long WINAPI SY_PARSE_CreateParser() ;

// 创建帧解析器
// 返回值			帧解析器句柄
// dwParserInSize	帧解析器入口缓冲区大小(单位:字节)
// dwParserOutSize	帧解析器出口缓冲区大小(单位:字节)
SY_API long WINAPI SY_PARSE_CreateParserEx( DWORD dwParserInSize = PBS_2MB, DWORD dwParserOutSize = PBS_1MB ) ;

// 关闭帧解析器
// nParserID	帧解析器句柄
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PARSE_CloseParser( long nParserID ) ;

// 设置帧解析回调
// nParserID	帧解析器句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PARSE_SetParseCallBack( long nParserID, LPParseCallBack pCallBack, LPVOID pvParam ) ;

// 设置音频回调
// nParserID	帧解析器句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PARSE_SetAudioParseCallBack( long nParserID, LPParseCallBack pCallBack, LPVOID pvParam ) ;

// 设置索引回调
// nParserID	帧解析器句柄
// pCallBack	回调函数地址
// pvParam		回调函数参数
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PARSE_SetIndexCallBack( long nParserID, LPIndexCallBack pCallBack, LPVOID pvParam ) ;

// 向帧解析器中添加数据
// nParserID	帧解析器句柄
// pData		数据指针
// dwDataSize	数据大小
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_PARSE_ParserInputData( long nParserID, LPBYTE pData, DWORD dwDataSize ) ;

////////////////////////////////////////////////////////////////////////////////
// 文件存储

// 创建存储文件
// pszFileName	存储文件名称
// nFileLength	文件预置大小, 单位:字节, 可选默认为不设置文件自动增长模式
// 返回值		存储文件句柄
SY_API long WINAPI SY_FILE_Create( LPCTSTR pszFileName, ULONGLONG nFileLength = 0 ) ;

// 向存储文件中添加数据
// nFileID		存储文件句柄
// pData		数据指针
// dwDataSize	数据大小
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_FILE_Write( long nFileID, LPBYTE pData, DWORD dwDataSize ) ;

// 关闭存储文件
// nFileID		存储文件句柄
// 返回值		1 成功, 0失败
SY_API long WINAPI SY_FILE_Close( long nFileID ) ;

////////////////////////////////////////////////////////////////////////////////
