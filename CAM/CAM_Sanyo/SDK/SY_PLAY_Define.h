
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

// ��ʾ�ص���������
// dc			��ʾ�豸������
// nWidth		��ʾͼ���
// nHeight		��ʾͼ���
// pvParam		��ʾ�ص���������
typedef void ( CALLBACK *LPDisplayCallBack )( HDC dc, int nWidth, int nHeight, LPVOID pvParam ) ;

// ����ص���������
// nImgWidth	����ͼ���
// nImgHeight	����ͼ���
// pBuffer		ͼ��ָ��
// dwImgSize	��ǰͼ���С
// pvParam		����ص���������
typedef void ( CALLBACK *LPDecodeCallBack )( int nImgWidth, int nImgHeight,
											LPBYTE pBuffer, DWORD dwImgSize, LPVOID pvParam ) ;

// ֡�����ص���������
// nParserID	֡���������
// pFrame		֡��Ϣ
// nVersion		pFrameָ��ṹ�汾��
// pvParam		֡�����ص���������
typedef void ( CALLBACK *LPParseCallBack )( long nParserID, LPSY_FRAME pFrame, int nVersion, LPVOID pvParam ) ;

// ֡���������ص���������
// nParserID	֡���������
// nVer			���ͺ�
// dwOffset		ƫ��
// pvParam		֡���������ص���������
typedef void ( CALLBACK *LPIndexCallBack )( long nParserID, int nVer, DWORD dwOffset, LPVOID pvParam ) ;

////////////////////////////////////////////////////////////////////////////////

// ����������ģʽ
enum SY_DECODE_MODE{ 
	SY_DECODE_MODE_NETSTREAM = 0xF001,	// ������ģʽĬ��
	SY_DEOCDE_MODE_FILE					// �ļ�ģʽ
};

// ������
enum SY_PLAY_SPEED{
	SY_PLAY_SPEED_NORMAL = 0,	// �����ٶ�
	SY_PLAY_SPEED_FAST,			// ����
	SY_PLAY_SPEED_SLOW			// ����
};

// �ļ�ģʽ���ſ���
enum SY_PLAY_FILECTRL{
	SY_PLAY_FILECTRL_RESET = 0,	// �ָ�����
	SY_PLAY_FILECTRL_PAUSE,		// ������ͣ
	SY_PLAY_FILECTRL_FRAMEPLAY,	// ��֡����
};

////////////////////////////////////////////////////////////////////////////////
// Parser Buffer Size

#define PBS_1MB		1024 * 1024
#define PBS_2MB		PBS_1MB * 2
#define PBS_4MB		PBS_2MB * 2

////////////////////////////////////////////////////////////////////////////////
