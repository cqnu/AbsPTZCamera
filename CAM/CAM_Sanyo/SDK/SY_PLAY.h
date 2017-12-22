
#pragma once

#include "SY_PLAY_Define.h"

#ifdef	SY_SDK
#define	SY_API	extern "C"	__declspec( dllexport )
#else
#define	SY_API	extern "C"	__declspec( dllimport )
#endif

////////////////////////////////////////////////////////////////////////////////
// �������

// ����������
// ����ֵ		���������
SY_API long WINAPI SY_PLAY_CreatePlayer( SY_CODEC_STREAM_STATUS status ) ;
SY_API long WINAPI SY_PLAY_CreatePlayerEx( SY_CODEC_STREAM_STATUS status, DWORD dwPlayerVersion ) ;

// �رղ�����
// nPlayerID	���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_ClosePlayer( long nPlayerID ) ;

// ���ò�������ʾ����
// nPlayerID	���������
// hWnd			���ھ��
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_SetPlayerWindow( long nPlayerID, HWND hWnd ) ;

// ������ʾ�ص�����
// nPlayerID	���������
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_SetDisplayCallBack( long nPlayerID, LPDisplayCallBack pCallBack, LPVOID pvParam ) ;

// ���ý���ص�����
// nPlayerID	���������
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_SetDecodeCallBack( long nPlayerID, LPDecodeCallBack pCallBack, LPVOID pvParam ) ;

// ���ý���Ҷ�ͼ�ص�����
// nPlayerID	���������
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_SetDecodeGrayCallBack( long nPlayerID, LPDecodeCallBack pCallBack, LPVOID pvParam ) ;

// ���벢��ʾʵʱ����
// nPlayerID		���������
// pRealData		��������ָ��
// dwRealDataSize	�������ݴ�С
// ����ֵ			1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_PlayerInputRealData( long nPlayerID, LPBYTE pRealData, DWORD dwRealDataSize ) ;

// ������Ƶ����
// nPlayerID		���������
// pRealData		��������ָ��
// dwRealDataSize	�������ݴ�С
// ����ֵ			1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_PlayerInputAudioData( long nPlayerID, LPBYTE pRealData, DWORD dwRealDataSize ) ;

// ������رղ������Ľ�����
// nPlayerID	���������
// bEnable		1 ����������, 0 �رս�����
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_EnableDecoder( long nPlayerID, BOOL bEnable ) ;

// ���ò���������ģʽ
// nPlayerID	���������
// modeDecode	����ģʽ
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_SetDecoderMode( long nPlayerID, SY_DECODE_MODE mode ) ;

// ���ò����ٶ�
// nPlayerID	���������
// speed		�ٶ�ģʽ
SY_API long WINAPI SY_PLAY_SetSpeed( long nPlayerID, SY_PLAY_SPEED speed ) ;

// �ļ�ģʽ�µĲ��ſ���
// nPlayerID	���������
// ctrl			��������
// dwValue		���Ʋ���
SY_API long WINAPI SY_PLAY_FileControl( long nPlayerID, SY_PLAY_FILECTRL ctrl, DWORD dwValue ) ;

// ��ʼ��ʵʱ�����洢���ļ���
// nPlayerID		���������
// pszFile			�洢�ļ���ȫ·��
// ����ֵ			1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_StartSaveRealData( long nPlayerID, LPCTSTR pszFile ) ;

// ֹͣ��ʵʱ�����洢���ļ���
// nPlayerID		���������
// ����ֵ			1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_StopSaveRealData( long nPlayerID ) ;

// ����ǰͼ�񱣴�ΪBMPͼƬ
// nPlayerID		���������
// pszBmpFile		BMPͼƬ�ļ�·��
// ����ֵ			1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PLAY_SaveBMPFile( long nPlayerID, LPCTSTR pszBmpFile ) ;

// ������ʾ
// nPlayerID		���������
// pRECT			����
SY_API long WINAPI SY_PLAY_StretchDisplay( long nPlayerID, LPRECT pRECT ) ;

////////////////////////////////////////////////////////////////////////////////
// ֡������

// ����֡������
// ����ֵ		֡���������
SY_API long WINAPI SY_PARSE_CreateParser() ;

// ����֡������
// ����ֵ			֡���������
// dwParserInSize	֡��������ڻ�������С(��λ:�ֽ�)
// dwParserOutSize	֡���������ڻ�������С(��λ:�ֽ�)
SY_API long WINAPI SY_PARSE_CreateParserEx( DWORD dwParserInSize = PBS_2MB, DWORD dwParserOutSize = PBS_1MB ) ;

// �ر�֡������
// nParserID	֡���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PARSE_CloseParser( long nParserID ) ;

// ����֡�����ص�
// nParserID	֡���������
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PARSE_SetParseCallBack( long nParserID, LPParseCallBack pCallBack, LPVOID pvParam ) ;

// ������Ƶ�ص�
// nParserID	֡���������
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PARSE_SetAudioParseCallBack( long nParserID, LPParseCallBack pCallBack, LPVOID pvParam ) ;

// ���������ص�
// nParserID	֡���������
// pCallBack	�ص�������ַ
// pvParam		�ص���������
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PARSE_SetIndexCallBack( long nParserID, LPIndexCallBack pCallBack, LPVOID pvParam ) ;

// ��֡���������������
// nParserID	֡���������
// pData		����ָ��
// dwDataSize	���ݴ�С
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_PARSE_ParserInputData( long nParserID, LPBYTE pData, DWORD dwDataSize ) ;

////////////////////////////////////////////////////////////////////////////////
// �ļ��洢

// �����洢�ļ�
// pszFileName	�洢�ļ�����
// nFileLength	�ļ�Ԥ�ô�С, ��λ:�ֽ�, ��ѡĬ��Ϊ�������ļ��Զ�����ģʽ
// ����ֵ		�洢�ļ����
SY_API long WINAPI SY_FILE_Create( LPCTSTR pszFileName, ULONGLONG nFileLength = 0 ) ;

// ��洢�ļ����������
// nFileID		�洢�ļ����
// pData		����ָ��
// dwDataSize	���ݴ�С
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_FILE_Write( long nFileID, LPBYTE pData, DWORD dwDataSize ) ;

// �رմ洢�ļ�
// nFileID		�洢�ļ����
// ����ֵ		1 �ɹ�, 0ʧ��
SY_API long WINAPI SY_FILE_Close( long nFileID ) ;

////////////////////////////////////////////////////////////////////////////////
