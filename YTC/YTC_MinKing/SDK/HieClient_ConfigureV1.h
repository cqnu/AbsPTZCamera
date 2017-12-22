#ifndef HIE_CLIENT_CONFIGURE_V1_H

#define HIE_CLIENT_CONFIGURE_V1_H

#define HY_V1_DVR_MAX_CHANNUM 	    16			/*���ͨ����*/
#define HY_V1_DVR_NAME_LEN			32			/*�û�����󳤶�*/
#define HY_V1_DVR_SERIALNO_LEN 	48			/*���к���󳤶�*/
#define HY_V1_DVR_MACADDR_LEN 		8			/*�豸�����ַ����*/
#define HY_V1_DVR_PASSWD_LEN 		16			/*�û�������󳤶�*/
#define HY_V1_DVR_MAX_ALARMIN 	    16			/*��󱨾��������*/
#define HY_V1_DVR_MAX_ALARMOUT 	4			/*��󱨾��������*/
#define HY_V1_DVR_MAX_STREAM_TYPES 2			/*�������ʽ����*/
#define HY_V1_DVR_MAX_TIMESEGMENT  4			/*���ʱ��θ���*/
#define HY_V1_DVR_MAX_PRESET 		128			/*���Ԥ�õ����*/
#define HY_V1_DVR_MAX_DAYS 		7			/*���������*/
#define HY_V1_DVR_MAX_SHELTERNUM   4			/*�ڵ�������*/
#define HY_V1_DVR_MAX_EXCEPTIONNUM 16			/*����쳣����*/
#define HY_V1_DVR_MAX_PTZ_NUMS     40			/*���PTZ����*/
#define HY_V1_DVR_PTZ_NAME_LEN     10			/*���PTZ���ֳ���*/

/*PTZЭ����Ϣ*/
typedef struct {
   	BYTE    PTZNum;							/*ϵͳ֧��PTZЭ��ĸ���(ֻ��)*/ 
	char    PTZName[HY_V1_DVR_MAX_PTZ_NUMS][HY_V1_DVR_PTZ_NAME_LEN];/*ϵͳ��ǰ֧�ֵ�PTZЭ������(ֻ��)*/
}HY_V1_DVR_PTZ, *LPHY_V1_DVR_PTZ;

/*ʱ��������Ϣ*/
typedef struct{
	WORD dwYear;	//��
	WORD dwMonth;	//��
	WORD dwDay;		//��
	WORD dwHour;	//ʱ
	WORD dwMinute;	//��
	WORD dwSecond;	//��
}HY_V1_DVR_TIME,*LPHY_V1_DVR_TIME;

/*����������Ϣ*/
typedef struct
{
	char  sDVRIP[16];						/* ������IP��ַ */
	char  sDVRIPMask[16];					/* ������IP��ַ���� */
	char  sGatewayIP[16];					/* ���ص�ַ */
	BYTE  byMACAddr[HY_V1_DVR_MACADDR_LEN];	/* �������������ַ(ֻ��)*/
	DWORD  dwPPPOE;							/* 0-������,1-���� */
	BYTE  sPPPoEUser[HY_V1_DVR_NAME_LEN];		/* PPPoE�û��� */
	char  sPPPoEPassword[HY_V1_DVR_PASSWD_LEN];/* PPPoE���� */
	char  sPPPoEIP[16];						/* PPPoE IP��ַ(ֻ��) */
	char  sPrimaryDNS[16];					/* ��DNS������ */
	char  sSecondaryDNS[16];				/* ����DNS */
	WORD  dwAutoReg;						/* 0-�������Զ�ע��, 1-�����Զ�ע�� */
	WORD  dwAutoRegIntervals;				/* ע����ʱ��, 1-999�� */
	BYTE  sManageHostIP[16];				/* Զ�̹���������ַ */
	DWORD  wManageHostPort;				    /* Զ�̹��������˿� */
	WORD  wLocalCmdPort;					/* ��������������˿� */
	WORD  wLocalMediaPort;					/* ����������ý��˿� */
	DWORD dwNAT;							/* 0--������NAT, 1-����NAT(ͣ��)*/
	BYTE  sNATIP[16];						/* NAT��ַ(ͣ��)*/
	WORD  byLinkMode;						/* 0��TCP	1��UDP	2���ಥ(ͣ��)*/
}HY_V1_DVR_NETCFG, *LPHY_V1_DVR_NETCFG;

/*�豸������Ϣ*/
typedef struct{ 
	DWORD dwSize;
	BYTE sDVRName[HY_V1_DVR_NAME_LEN];			/* ���������� */ 
	BYTE sSerialNumber[HY_V1_DVR_SERIALNO_LEN];/* ���к�(ֻ��)*/ 
	DWORD dwSystemVideoStandard;			/*��������Ƶ��ʽ��0--NTSC 1--PAL 2--SECAMA*/
	DWORD dwDVRID;						    /* ������ ID������ң������-1Ϊδ���ã����÷�Χ0-99 */ 
	DWORD dwRecycleRecord; 				    /* �Ƿ�ѭ��¼��0�����ǣ�1���� */ 
	DWORD dwSoftwareVersion;  				/* ����汾��,���ָ�8λ�����汾,���ָ�8λ�Ǵΰ汾�ţ� ���ֵ�8λ���޶��汾(ֻ��)*/ 
	DWORD dwSoftwareBuildDate; 				/* �����������,0xYYYYMMDD(ֻ��)*/ 
	DWORD dwDSPSoftwareVersion; 			/* DSP����汾,���ָ�8λ�����汾,���ָ�8λ�Ǵΰ汾�ţ� ���ֵ�8λ���޶��汾(ֻ��)*/
	DWORD dwDSPSoftwareBuildDate;			/* DSP�����������,0xYYYYMMDD(ֻ��)*/
	DWORD dwPanelVersion; 					/* ǰ���汾,��16λ�����汾,��16λ�Ǵΰ汾(ֻ��)*/ 
	DWORD dwHardwareVersion; 				/* Ӳ���汾,��16λ�����汾,��16λ�Ǵΰ汾(ֻ��)*/ 
	BYTE byAlarmInPortNum; 					/* �����������������(ֻ��)*/ 
	BYTE byAlarmOutPortNum; 				/* �����������������(ֻ��)*/ 
	BYTE byRS232Num; 						/* ������232���ڸ���(ֻ��)*/ 
	BYTE byRS485Num; 						/* ������485���ڸ���(ֻ��)*/ 
	BYTE byDiskCtrlNum; 					/* ������Ӳ�̿���������(ֻ��)*/ 
	BYTE byDiskNum;   						/* ������Ӳ�̸���(ֻ��)*/ 
	BYTE byDVRType; 						/* ���������ͣ�(ֻ��)
                                            1�� 1500 4CIF DVR
                                            2�� 1500 8CIF DVR
                                            3�� 1500 12CIF DVR
                                            4�� 1500 16CIF DVR
                                            5�� 1500 4D1 DVR
                                            6�� 1500 8D1 DVR
                                            7�� 8950 4CIF DVR
                                            8�� 1700 8CIF DVR
                                            9�� 1700 16CIF DVR
                                            10��1700 4D1 DVR
                                            11��1700 8D1 DVR
                                            12��1700 16D1 DVR
                                            13��1D1 DVS
                                            14��2D1 DVS
                                            15��4CIF DVS
                                            16��1D1 DVS-ATA
                                            17��2D1 DVS-ATA
                                            18��4CIF DVS-ATA*/
	BYTE byChanNum; 						/* ������ͨ������(ֻ��)*/ 
	BYTE byDecordChans; 					/* ����������·��(ֻ��)*/ 
	BYTE byVGANum; 							/* VGA�ڵĸ���(ֻ��)*/ 
	BYTE byUSBNum; 							/* USB�ڵĸ���(ֻ��)*/ 
	char reservedData[3]; 					/* ���� */
}HY_V1_DVR_DEVICECFG,*LPHY_V1_DVR_DEVICECFG;

#define NOACTION			0x0				/*����Ӧ*/
#define WARNONMONITOR		0x1				/*�������Ͼ���*/
#define WARNONAUDIOOUT		0x2				/*��������*/
#define UPTOCENTER			0x4				/*�ϴ�����*/
#define TRIGGERALARMOUT		0x8				/*�����������*/
#define EMAILNOTIFY			0x10			/*Email֪ͨ*/

/*�쳣������Ϣ*/
typedef struct
{
	DWORD	dwHandleType;			        /*����ʽ,����ʽ��"��"���*/
											/*0x00: ����Ӧ*/
											/*0x01: �������Ͼ��棨Ԥ��ͼ���ϵ��ӣ�*/
											/*0x02: ��������*/
											/*0x04: �ϴ�����*/
											/*0x08: �����������*/
											/*0x10: Email֪ͨ*/
	BYTE byRelAlarmOut[HY_V1_DVR_MAX_ALARMOUT];/*�������������ͨ��,�������������,Ϊ1��ʾ���������*/
}HY_V1_DVR_HANDLEEXCEPTION;

/*ʱ���������Ϣ*/
typedef struct{
	BYTE byStartHour;                       /*��ʼʱ��*/
	BYTE byStartMin;
	BYTE byStopHour;                        /*����ʱ��*/
	BYTE byStopMin;
}HY_V1_DVR_SCHEDTIME,*LPHY_V1_DVR_SCHEDTIME;

/*��������������Ϣ*/
typedef struct{
	DWORD dwSize;
	HY_V1_DVR_HANDLEEXCEPTION struAlarmHandleType;/* ����ʽ */ 
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT]; 
	/*���߸�ʱ��Σ��ֱ�Ϊ����һ�����������ġ��塢������*/
	BYTE byAlarmType;						/* ����������,0������,1������ */
	BYTE byAlarmInHandle;					/* �Ƿ���, 0:������ ��1������ */
	BYTE byRelRecordChan[HY_V1_DVR_MAX_CHANNUM];/* ����������¼��ͨ��,Ϊ1��ʾ������ͨ�� */
	BYTE byEnablePreset[HY_V1_DVR_MAX_CHANNUM];/* �Ƿ����Ԥ�õ� */ 
	BYTE byPresetNo[HY_V1_DVR_MAX_CHANNUM];	/* ���õ���̨Ԥ�õ����,һ������������Ե��ö��ͨ������̨Ԥ�õ�, 0xff��ʾ������Ԥ�õ㡣*/
	BYTE byEnableCruise[HY_V1_DVR_MAX_CHANNUM];		/* �Ƿ����Ѳ�� */ 
	BYTE byCruiseNo[HY_V1_DVR_MAX_CHANNUM];	/* Ѳ�� */ 
	BYTE byEnablePtzTrack[HY_V1_DVR_MAX_CHANNUM];/* �Ƿ���ù켣 */ 
	BYTE byPTZTrack[HY_V1_DVR_MAX_CHANNUM];	/* ���õ���̨�Ĺ켣��� */
}HY_V1_DVR_ALARMINCFG,*LPHY_V1_DVR_ALARMINCFG;

/*�������������Ϣ*/
typedef struct{
	DWORD dwSize;
	DWORD dwAlarmOutDelay;					/* �������ʱ��(��Ч����5, 10, 30 ,60, 120, 300, 600, -1�ֶ��ر�) */
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT];
	/*���߸�ʱ��Σ��ֱ�Ϊ����һ�����������ġ��塢������*/
	BYTE byEnableTimeTable;					/*�Ƿ�����ʱ���*/
	BYTE byAlarmOutHandle;					/* �Ƿ���*/ 
}HY_V1_DVR_ALARMOUTCFG,*LPHY_V1_DVR_ALARMOUTCFG;

/*�쳣������Ϣ*/
typedef struct{
	DWORD dwSize; 
	HY_V1_DVR_HANDLEEXCEPTION struExceptionHandleType[HY_V1_DVR_MAX_EXCEPTIONNUM]; 
	/*����0-Ӳ����,1- ��������IP ��ַ��ͻ,2-Ӳ�̳���,3-�Ƿ�����, 4-���߶�*/ 
}HY_V1_DVR_EXCEPTION,*LPHY_V1_DVR_EXCEPTION; 

/*������������Ϣ*/
typedef struct{
	DWORD dwSize; 							/* �˽ṹ�Ĵ�С */ 
	DWORD dwBaudRate; 						/* ������(bps)��0��1200��1��2400��2��4800��3��9600 */ 
	BYTE byDataBit; 						/* �����м�λ 0��4λ��1��5λ��2��6λ��3��7λ��4-8λ; */ 
	BYTE byStopBit; 						/* ֹͣλ 0��1λ��1��1.5λ,  2 -2λ; */ 
	BYTE byParity; 							/* У�� 0����У�飬1����У�飬2��żУ��; */ 
	BYTE byFlowcontrol; 					/* 0����*/ 
	WORD wDecoderType;  					/* ����������*/
	WORD wDecoderAddress;					/* ��������ַ:0 - 255 */ 
	WORD wComType;	 						/* 0��COM1 RS232��1��COM2 RS485 */
	HY_V1_DVR_PTZ   strPTZInfo;				/* PTZ��Ϣ */
	BYTE bySetPreset[HY_V1_DVR_MAX_PRESET];	/* Ԥ�õ��Ƿ�����,0-û������,1-����*/ 
	BYTE bySetCruise[HY_V1_DVR_MAX_PRESET];	/* Ѳ���Ƿ�����,0-û������,1-���� */ 
	BYTE bySetTrack[HY_V1_DVR_MAX_PRESET];		/* �켣�Ƿ�����,0-û������,1-����*/ 
}HY_V1_DVR_DECODERCFG,*LPHY_V1_DVR_DECODERCFG;

/*ʱ���¼������*/
typedef struct{ 
	HY_V1_DVR_SCHEDTIME struRecordTime; 
	BYTE byRecordType;						/* 0����ʱ¼��1������¼��2���ƶ����3������|������255��������*/ 
	char reservedData[3];					/* ���� */ 
}HY_V1_DVR_RECORDSCHED,*LPHY_V1_DVR_RECORDSCHED; 

/*ȫ��¼������*/
typedef struct{ 
	WORD wAllDayRecord;						/* �Ƿ�ȫ��¼�� */ 
	BYTE byRecordType;						/* 0����ʱ¼��1������¼��2���ƶ����3������|����*/ 
	char reservedData; 
}HY_V1_DVR_RECORDDAY; 

/*¼��������Ϣ*/
typedef struct{
	DWORD dwSize;							/* �˽ṹ�Ĵ�С */ 
	DWORD dwRecord; 						/* �Ƿ�¼�� 0-�� 1-�� */ 
	DWORD dwRecordTime; 					/* ¼��ʱ�䳤�� */ 
	DWORD dwPreRecordTime;					/* Ԥ¼ʱ��0-10����λ���룩*/
    DWORD dwDelayTime;						/* �ӳ�¼��ʱ�� 0-1800(��λ����) */
	HY_V1_DVR_RECORDDAY struRecAllDay[HY_V1_DVR_MAX_DAYS + 1];/* ȫ��¼�� */
	HY_V1_DVR_RECORDSCHED struRecordSched[HY_V1_DVR_MAX_DAYS + 1][HY_V1_DVR_MAX_TIMESEGMENT - 2]; 
	/*���˸�ʱ��Σ��ֱ�Ϊ�����ա�һ�����������ġ��塢������������*/
}HY_V1_DVR_RECORDCFG,*LPHY_V1_DVR_RECORDCFG;      /* ʱ���¼�� */

//�ƶ����
typedef struct{ 
	BYTE byMotionScope[12][16]; 			/* �������,����12*16��С���,Ϊ0��ʾ�ú�����ƶ��������,1-��ʾ���� */ 
	BYTE byMotionSenstive; 					/* �ƶ����������, 0 - 2,ֵԽСԽ����,0xff�ر� */ 
	BYTE byEnableHandleMotion; 				/* �Ƿ����ƶ���� */ 
	BYTE byRelRecordChan[HY_V1_DVR_MAX_CHANNUM];/*����������¼��ͨ��,Ϊ0��ʾ��������Ϊ1��ʾ������ͨ�� */
	HY_V1_DVR_HANDLEEXCEPTION strMotionHandleType;/* ����ʽ */ 
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT];/*����ʱ��(ͣ��)*/
}HY_V1_DVR_MOTION,*LPHY_V1_DVR_MOTION;

//�źŶ�ʧ���� 
typedef struct{ 
	BYTE byEnableHandleVILost; 				/* �Ƿ����źŶ�ʧ���� */ 
	HY_V1_DVR_HANDLEEXCEPTION strVILostHandleType;	/* ����ʽ */ 
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT];/*����ʱ��(ͣ��)*/
}HY_V1_DVR_VILOST,*LPHY_V1_DVR_VILOST; 

/*�������ڵ�*/
typedef struct{ 
	WORD wHideAreaTopLeftX;					/* �����������x���� */ 
	WORD wHideAreaTopLeftY; 				/* �����������y���� */ 
	WORD wHideAreaWidth; 					/* ����������Ŀ� */ 
	WORD wHideAreaHeight; 					/* ����������ĸ� */ 
}HY_V1_DVR_MOSAIC,*LPHY_V1_DVR_MOSAIC;

/*ͨ��������Ϣ*/
typedef struct 
{ 
	DWORD dwSize;							/* �˽ṹ�Ĵ�С */
	BYTE sChanName[HY_V1_DVR_NAME_LEN];		/* ͨ������ */ 
	DWORD dwVideoFormat;					/* ֻ�� ��Ƶ��ʽ 0-NTSC 1-PAL 2-SECAMA,��Ƶ��ʽ�������豸�����е���Ƶ��ʽΪ��(ֻ��).*/ 
	BYTE byBrightness; 						/* ����,0-255 */ 
	BYTE byContrast; 						/* �Աȶ�,0-128 */ 
	BYTE bySaturation; 						/* ���Ͷ�,0-128 */ 
	BYTE byHue; 							/* ɫ��,0-255 */ 
	DWORD dwShowChanName;					/* Ԥ����ͼ�����Ƿ���ʾͨ������,0-����ʾ,1-��ʾ*/
	WORD wShowNameTopLeftX; 				/* ͨ��������ʾλ�õ�x����, 0-379 */ 
	WORD wShowNameTopLeftY; 				/* ͨ��������ʾλ�õ�y����, 0-459 */ 
	HY_V1_DVR_VILOST struVILost; 				/* �źŶ�ʧ���� */
	HY_V1_DVR_MOTION struMotion; 				/* �ƶ���� */
	DWORD dwEnableHide;						/* �Ƿ������������ڵ� ,0-��,1-��*/
	HY_V1_DVR_MOSAIC struMosaic[HY_V1_DVR_MAX_SHELTERNUM]; /*�������ڵ�*/
	/*OSD */
	DWORD dwShowOsd; 						/* Ԥ����ͼ�����Ƿ���ʾOSD,0-����ʾ,1-��ʾ*/
	WORD wOSDTopLeftX;						/* OSD��x����, 0-379 */
	WORD wOSDTopLeftY;						/* OSD��y����, 0-499 */
	BYTE byOSDType; 						/* OSD����(��Ҫ�������ո�ʽ)(ֻ��)*/
											/* 0: XXXX-XX-XX ������ */
											/* 1: XX-XX-XXXX ������ */
											/* 2: XXXX��XX��XX�� */
											/* 3: XX��XX�� XXXX�� */
											/*��Ϊ����ֵ,��ʾ�ù���δʵ��*/
	BYTE byDispWeek; 						/* �Ƿ���ʾ����(ֻ��)*/
											/*0Ϊ����ʾ,1Ϊ��ʾ,����Ϊδʵ��*/
	BYTE byOSDAttrib;						/* OSD����:͸������˸(ֻ��)*/ 
											/* 1: ͸��,��˸ */
											/* 2: ͸��,����˸ */
											/* 3: ��͸��,��˸ */
											/* 4: ��͸��,����˸ */
											/*��Ϊ����ֵ,��ʾ�ù���δʵ��*/
}HY_V1_DVR_PICCFG,*LPHY_V1_DVR_PICCFG;

#define   CAP_NORMAL_H264			0x01	/*��ͨ��H264*/
#define   CAP_ADVANCED_H264			0x02	/*��ǿ��H264*/

#define   CAP_RESOLUTION_QCIF		0x01	/*QCIF*/
#define   CAP_RESOLUTION_CIF		0x02	/*CIF*/
#define   CAP_RESULUTION_D1			0x04	/*D1*/
#define   CAP_RESULUTION_SD			0x08	/*SD*/
#define   CAP_RESULUTION_4CIF		0x10	/*4CIF*/
#define   CAP_RESULUTION_MD			0x20	/*MD*/

/*������ʽ��Ϣ*/
typedef struct {
     	BYTE byStreamFormatCap; 			/* ֧�ֵ�������ʽ,����֧�ֵĸ�ʽ���Ľ��(ֻ��)*/
     	BYTE byResolutionCap[HY_V1_DVR_MAX_STREAM_TYPES];/* ÿ�ֱ����ʽ��֧�ֵķֱ��ʸ�ʽ,֧�����еĸ�ʽ���Ľ��(ֻ��)*/
		/*HY_V1_DVR_COMPRESSIONCFG	vCfg;
		��ȡ������֧��������ʽ���ӣ�
		if (vCfg.struRecordPara.strCompressCap.byStreamFormatCap & CAP_NORMAL_H264 )
			//֧����ͨ��H264
		if (vCfg.struRecordPara.strCompressCap.byStreamFormatCap & CAP_ADVANCED_H264 )
			//֧����ǿ��H264
		��ȡ������֧��������ʽ���ӣ�
		if (vCfg.struNetPara.strCompressCap.byStreamFormatCap & CAP_NORMAL_H264 )
			//֧����ͨ��H264
		if (vCfg.struNetPara.strCompressCap.byStreamFormatCap & CAP_ADVANCED_H264 )
			//֧����ǿ��H264
		��ȡ��������ǰ�����ʽ��֧�ֵķֱ��ʸ�ʽ���ӣ�
		if(vCfg.struRecordPara.strCompressCap.byResolutionCap[vCfg.struRecordPara. byStreamFormat] & CAP_RESOLUTION_CIF)
			//��������ǰ�����ʽ��֧��CIF�ֱ���ѹ��
		��ȡ��������ǰ�����ʽ��֧�ֵķֱ��ʸ�ʽ���ӣ�
		if(vCfg.struNetPara.strCompressCap.byResolutionCap[vCfg.struNetPara.byStreamFormat] & CAP_RESOLUTION_CIF)
			//��������ǰ�����ʽ��֧��CIF�ֱ���ѹ��
		*/
}HY_V1_DVR_COMPRESSCAP, *LPHY_V1_DVR_COMPRESSIONCAP;

/*ѹ����Ϣ*/
typedef struct{
	DWORD dwVideoFrameRate; 				/* ֡��
                                    		��������0-FULL; 1-1; 2-2; 3-4; 4-6; 5-8; 6-10; 7-12; 8-16; 9-20 
                                    		��������0-1;1-2;2-3;3-4;4-5;5-6;6-7;7-8;8-9;9-10;11-12;12-13;13-14;14-15 */
	BYTE byStreamType; 						/* ��������0-��Ƶ��,1-�������������������� */ 
	BYTE byResolution;						/* ���������ֱ��� 0 - CIF��1 - D1��2 - SD ��
											���������ֱ���  0 - QCIF�� 1 - CIF */
	WORD byStreamWidth; 					/* �������(ͣ��)*/
	WORD byStreamHeight;					/* �����߶�(ͣ��)*/
	BYTE byStreamFormat; 					/* ������ʽ 0-��ͨ��H.264, 1---��ǿ��H.264 */ 
    BYTE byBitrateType; 					/* ��������0�������ʣ�1�������ʣ�2�������ʣ�������������*/
	DWORD dwVideoBitrate; 					/* ��Ƶ���� 0-16K 1-32K 2-48k 3-64K 4-80K 5-96K 6-128K 7-160k 8-192K 9-224K 10-256K 11-320K 12-384K 13-448K 14-512K 15-640K 16-768K 17-896K 18-1024K 19-1280K 20-1536K 21-1792K 22-2048K������100��ֵ��ʾ�Զ���������ʵ������ֵΪdwVideoBitrate - 100����Сֵ16k ���ֵ8192k */

	BYTE byPicQuality; 						/* ͼ������ 0-��� 1-�κ� 2-�Ϻ� 3-һ�� 4-�ϲ� 5-�� ����Ϣ�ڱ�����ʱ��Ч��������������*/
	HY_V1_DVR_COMPRESSCAP strCompressCap;		/* ѹ�������� ���ڻ�ȡ����ʱ��Ч��ֻ����*/
}HY_V1_DVR_COMPRESSION_INFO,*LPHY_V1_DVR_COMPRESSION_INFO;

/*ѹ��������Ϣ*/
typedef struct{ 
	DWORD dwSize; 							/* �˽ṹ�Ĵ�С��Ŀǰ������Ϊ�Ƿ����������� ,�ò���Ϊ1ʱ��ʾ����������,Ϊ0ʱ��ʾ������������.*/
	HY_V1_DVR_COMPRESSION_INFO struRecordPara;	/* �豸����¼����������*/
	HY_V1_DVR_COMPRESSION_INFO struNetPara;	/* ������Ƶ���䣨��������*/
}HY_V1_DVR_COMPRESSIONCFG,*LPHY_V1_DVR_COMPRESSIONCFG;

#define HY_V1_DVR_MAX_RIGHT    16				/*���Ȩ�޸���*/
#define HY_V1_DVR_MAX_USERNUM  8       		/*����û�����*/

/*�û�Ȩ����Ϣ*/
typedef struct{ 
	BYTE sUserName[HY_V1_DVR_NAME_LEN];		/* �û��� */
	BYTE sPassword[HY_V1_DVR_PASSWD_LEN];		/* ���� */
	BYTE byLocalRight[HY_V1_DVR_MAX_RIGHT];	/* Ȩ�� */
											/* ����0: ���ؿ�����̨*/ 
											/* ����1: �����ֶ�¼��*/ 
											/* ����2: ���ػط�*/ 
											/* ����3: ���ر���*/
											/* ����4: ����ϵͳ����*/ 
											/* ����5: ���ظ߼�����(��־������������쳣��������������)*/
											/* ����6: �ػ�*/ 
	BYTE  byRemoteRight[HY_V1_DVR_MAX_RIGHT];	/* Ȩ�� */ 
											/* ����0: ������̨*/ 
											/* ����1: �ֶ�¼��*/ 
											/* ����2: Զ�̻ط�*/ 
											/* ����3: ���ز鿴״̬����־*/ 
											/* ����4: ����ϵͳ����*/ 			
											/* ����5: ���ظ߼�����(�������������ػ��� ��������� �쳣����)*/
											/* ����6: Զ�̷��������Խ�*/ 
											/* ����7: Զ��Ԥ��*/ 
	char sUserIP[16];						/* �û�IP��ַ(Ϊ0ʱ��ʾ�����κε�ַ) */ 
	BYTE byMACAddr[HY_V1_DVR_MACADDR_LEN];		/* �����ַ */ 
}HY_V1_DVR_USER_INFO,*LPHY_V1_DVR_USER_INFO;

/*�û�������Ϣ*/
typedef struct{ 
	DWORD dwSize; 
	HY_V1_DVR_USER_INFO struUser[HY_V1_DVR_MAX_USERNUM]; 
}HY_V1_DVR_USER,*LPHY_V1_DVR_USER;

/*�û�Ȩ����Ϣ*/
typedef struct{ 
	BYTE sUserName[HY_V1_DVR_NAME_LEN];		/* �û��� */
	BYTE sPassword[HY_V1_DVR_PASSWD_LEN];		/* ���� */
	DWORD dwLocalRight[HY_V1_DVR_MAX_RIGHT];	/* Ȩ�� */
											/* ����0: ���ؿ�����̨*/ 
											/* ����1: �����ֶ�¼��*/ 
											/* ����2: ���ػط�*/ 
											/* ����3: ���ر���*/
											/* ����4: ����ϵͳ����*/ 
											/* ����5: ���ظ߼�����(��־������������쳣��������������)*/
											/* ����6: �ػ�*/ 
	DWORD dwRemoteRight[HY_V1_DVR_MAX_RIGHT];	/* Ȩ�� */ 
											/* ����0: ������̨*/ 
											/* ����1: �ֶ�¼��*/ 
											/* ����2: Զ�̻ط�*/ 
											/* ����3: ���ز鿴״̬����־*/ 
											/* ����4: ����ϵͳ����*/ 			
											/* ����5: ���ظ߼�����(�������������ػ��� ��������� �쳣����)*/
											/* ����6: Զ�̷��������Խ�*/ 
											/* ����7: Զ��Ԥ��*/ 
	char sUserIP[16];						/* �û�IP��ַ(Ϊ0ʱ��ʾ�����κε�ַ) */ 
	BYTE byMACAddr[HY_V1_DVR_MACADDR_LEN];		/* �����ַ */ 
}HY_V1_DVR_USER_INFO_EX,*LPHY_V1_DVR_USER_INFO_EX;

/*�û�������Ϣ*/
typedef struct{ 
	DWORD dwSize; 
	HY_V1_DVR_USER_INFO_EX struUser[HY_V1_DVR_MAX_USERNUM]; 
}HY_V1_DVR_USER_EX,*LPHY_V1_DVR_USER_EX;

#define HY_V1_DVR_MAX_HARDDISK_NUM	8				/*���������Ӳ����*/
/*Ӳ��״̬��Ϣ*/
typedef struct
{
	DWORD dwCapacity;							/* ����,MΪ��λ(ֻ��)*/
	DWORD dwSpare;								/* ʣ������(ֻ��)*/
	BYTE  byWriteState;							/* ��ǰд��״̬ 0��ʾ��д�룬1��ʾ����д��(ֻ��)*/
}HY_V1_DVR_HARDDISK_STATE, *LPHY_V1_DVR_HARDDISK_STATE;

/*ͨ��״̬��Ϣ*/
typedef struct
{
	DWORD dwVideoBitrate;						/* ����������(ֻ��)*/
	DWORD dwChildVideoBitrate;					/* ����������(ֻ��)*/
	BYTE  byVideo;								/* ¼��״̬(ֻ��)*/
	BYTE  bySignal;								/* �ź�״̬(ֻ��)*/
	BYTE  byUseChildBitrate;					/* �Ƿ�����������(ֻ��)*/
}HY_V1_DVR_CHANNEL_STATE, *LPHY_V1_DVR_CHANNEL_STATE;

/*�豸״̬������Ϣ*/
typedef struct
{
	HY_V1_DVR_HARDDISK_STATE cHardState[HY_V1_DVR_MAX_HARDDISK_NUM];
												/*Ӳ��״̬*/
	HY_V1_DVR_CHANNEL_STATE cChanState[HY_V1_DVR_MAX_CHANNUM];
												/*ͨ��״̬*/
}HY_V1_DVR_DEVICE_STATE, *LPHY_V1_DVR_DEVICE_STATE;

#define HY_V1_DVR_MAX_DDNS				128			//���̬��������
#define HY_V1_DVR_MAX_DDNS_USER		64			//���̬�����û�����
#define HY_V1_DVR_MAX_DDNS_PASS		64			//���̬�������볤��

/*�豸DDNS������Ϣ*/
typedef struct  
{
	BYTE bytState;								/*״̬,0Ϊ��֧��,1Ϊ����,2Ϊ����*/
	char sDDNS[HY_V1_DVR_MAX_DDNS];				/*��̬������ַ*/
	char sUserName[HY_V1_DVR_MAX_DDNS_USER];		/*��̬�����û�����*/
	char sUserPass[HY_V1_DVR_MAX_DDNS_PASS];		/*��̬�����û�����*/
	BYTE bytReserve[3];							/*����λ*/
}HY_V1_DVR_DDNS_CFG, *LPHY_V1_DVR_DDNS_CFG;

/*�豸HTTP������Ϣ*/
typedef struct  
{
	BYTE bytState;								/*״̬,0Ϊ��֧��,1Ϊ����,2Ϊ����*/
	BYTE bytReserve;							/*����λ*/
	WORD wHTTPort;								/*HTTP�˿�*/
}HY_V1_DVR_HTTP_CFG, *LPHY_V1_DVR_HTTP_CFG;

#define HY_V1_DVR_RECORDSEG_DAY		24 * 2		//ÿ��¼��ʱ���

/*�豸����¼��������Ϣ,��30����Ϊһ��ʱ��ε�λ*/
typedef struct  
{
	BYTE bytState;								/*״̬,0Ϊ��֧��,1Ϊ����,2Ϊ����*/
	BYTE bytReserve[3];							/*����λ*/
	BYTE bytRecordTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_RECORDSEG_DAY];/*¼��״̬*/
	/*0:��¼��, 1:��ʱ¼��, 2:����¼�� 3:�ƶ���� 4:����|����*/
	DWORD dwPreRecordTime;						/* Ԥ¼ʱ��0-10(��λ:��)*/
    DWORD dwDelayTime;							/* �ӳ�¼��ʱ�� 0-1800(��λ:��) */
}HY_V1_DVR_RECORDEX_CFG, *LPHY_V1_DVR_RECORDEX_CFG;

/*�����Ҫ����ǰ������ͷ,�������½ṹ��*/
typedef struct
{
	WORD wBLC;									/* �Զ����ȿ���,0 �ر� 1 ��*/
	WORD wMIRROR;								/* ����		0 �ر� 1 ��*/
	WORD wAGC;									/* �Զ��������	0 �ر� 1 ��*/
	WORD wAE;									/* �Զ��ع�	0 �ر� 1 ��*/
	WORD wATW;									/* �Զ���ƽ��	����*/
	WORD wPOSNEG;								/* ��Ƭ		����*/
	WORD wAESHUT;								/* ���ӿ���	����*/
	WORD wReserve;								/* ����*/
}HY_V1_DVR_CAMERA_CFG,*LPHY_V1_DVR_CAMERA_CFG;

//������������Ϣ���� 24 * 24(24����˫��) * 10 (���10��˫��)
#define LATTICE_ARRAY_LEN		24 * 24 * 10
/*ͨ����������Ϣ*/
typedef struct
{
	BYTE bytLattice[LATTICE_ARRAY_LEN];			//������Ϣ
}HY_V1_DVR_CHNAME_LATTICE_INFO, *LPHY_V1_DVR_CHNAME_LATTICE_INFO;

//����������
#define FONT_NAME_LEN	32
/*������Ϣ*/
typedef struct
{
	char cFontName[FONT_NAME_LEN];				//������
	BYTE bytFontSize;							//�����С
	BYTE bytReserve[7];							//����
}HY_V1_DVR_FONT_INFO, *LPHY_V1_DVR_FONT_INFO;

//����ͨ������󳤶�,��������
#define LATTICE_CHANNEL_NAME	20 + 1
/*����ͨ��������*/
typedef struct
{
	char strName[LATTICE_CHANNEL_NAME];			//ͨ������(�ַ���)
	BYTE bytReserve[3];							//����
	WORD wWidth;								//ͨ�����������ַ������
	WORD wHeight;								//ͨ�����������ַ����߶�
	HY_V1_DVR_FONT_INFO	cFontInfo;				//ͨ����������Ϣ
}HY_V1_DVR_LATTICE_CHNAME_CFG, *LPHY_V1_DVR_LATTICE_CHNAME_CFG;

/*���õ���ͨ��������*/
typedef struct
{
	HY_V1_DVR_LATTICE_CHNAME_CFG		cChannelNameCfg;//ͨ��������
	HY_V1_DVR_CHNAME_LATTICE_INFO	cLatticeChNameInfo;	//ͨ����������Ϣ
}HY_V1_DVR_SET_LATTICE_CHNAME_CFG, *LPHY_V1_DVR_SET_LATTICE_CHNAME_CFG;

/*��ȡ����ͨ��������*/
typedef struct
{
	HY_V1_DVR_LATTICE_CHNAME_CFG		cChannelNameCfg;	//ͨ��������
}HY_V1_DVR_GET_LATTICE_CHNAME_CFG, *LPHY_V1_DVR_GET_LATTICE_CHNAME_CFG;

//��������DDNS֧��������
#define MAX_DDNS_CAPABILITY		40
//��������DDNS��Ϣ��󳤶�(��������)
#define MAX_DDNS_ADDR_LEN			64
/*��������DDNS֧�ֽṹ��Ϣ*/
typedef struct
{
	BYTE bytValidDNSNum;						//��ЧDNS����
	BYTE bytReserve[3];							//����
	char strDDNS[MAX_DDNS_CAPABILITY][MAX_DDNS_ADDR_LEN];//DDNS����(�ַ���)
}HY_V1_DVR_DDNS_CAPABILITY, *LPHY_V1_DVR_DDNS_CAPABILITY;

typedef struct  
{
	DWORD dwSize;
	HY_V1_DVR_HANDLEEXCEPTION struAlarmHandleType;					/* ����ʽ */
	
	BYTE bytProcessTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_RECORDSEG_DAY];	/*����ʱ���,����1Ϊ����ʱ�䣬0Ϊ����ʱ��*/
	
	BYTE byAlarmType;											/*����������,0������,1������ */
	BYTE byAlarmInHandle;										/* �Ƿ��� */
	BYTE byRelRecordChan[HY_V1_DVR_MAX_CHANNUM];					/*����������¼��ͨ��,Ϊ1��ʾ������ͨ�� */
	BYTE byEnablePreset[HY_V1_DVR_MAX_CHANNUM];					/* �Ƿ����Ԥ�õ� */
	BYTE byPresetNo[HY_V1_DVR_MAX_CHANNUM];						/* ���õ���̨Ԥ�õ����,һ������������Ե�        �ö��ͨ������̨Ԥ�õ�, 0xff��ʾ������Ԥ�õ㡣 */
	BYTE byEnableCruise[HY_V1_DVR_MAX_CHANNUM];					/* �Ƿ����Ѳ�� */
	BYTE byCruiseNo[HY_V1_DVR_MAX_CHANNUM];						/* Ѳ�� */
	BYTE byEnablePtzTrack[HY_V1_DVR_MAX_CHANNUM];					/* �Ƿ���ù켣 */
	BYTE byPTZTrack[HY_V1_DVR_MAX_CHANNUM];						/* ���õ���̨�Ĺ켣��� */
}HY_V1_DVR_ALARMINCFG_EX, *LPHY_V1_DVR_ALARMINCFG_EX;

//����ƶ�������
#define MAX_MOBILE_SERVICE_NUMBER			16
//�ƶ���������󳤶�
#define MAX_MOBILE_SERVICE_NAME_LEN			64
/*�ƶ��豸������Ϣ*/
typedef struct
{
	DWORD	dwStatus;											//����״̬,	0Ϊ��֧��,1Ϊ����,2Ϊ����
	char	cServiceName[MAX_MOBILE_SERVICE_NAME_LEN];			//������
	DWORD	dwServicePort;										//����˿�
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_MOBILE_SERVICE_INFO;

/*�ƶ��豸����*/
typedef struct
{
	DWORD	dwServiceNumber;									//��Ч������
	HY_V1_DVR_MOBILE_SERVICE_INFO	cService[MAX_MOBILE_SERVICE_NUMBER];//������Ϣ
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_MOBILE_SERVICE;

//Email����û�������
#define MAX_EMAIL_USERNAME_LEN				32
//Email������볤��
#define MAX_EMAIL_PASSWORD_LEN				32
//Email��������ַ��󳤶�
#define MAX_EMAIL_SERVER_ADDR_LEN			64
//Email����ʼ���ַ����
#define MAX_EMAIL_ADDRESS_LEN				64
//Email����ռ��˸���
#define MAX_EMAIL_ADDRESSEE_NUMBER			16
/*Email��������Ϣ*/
typedef struct
{
	char	cServerAddress[MAX_EMAIL_SERVER_ADDR_LEN];			//��������ַ
	DWORD	dwServerPort;										//�������˿�
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_EMAIL_SERVER_INFO;

/*Email��������Ϣ*/
typedef struct
{
	char	cUserName[MAX_EMAIL_USERNAME_LEN];					//�û�����
	char	cPassWord[MAX_EMAIL_PASSWORD_LEN];					//�û�����
	char	cAddresser[MAX_EMAIL_ADDRESS_LEN];					//�����˵�ַ
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_EMAIL_ADDRESSER_INFO;

/*Email�ռ���*/
typedef struct
{
	char	cAddressee[MAX_EMAIL_ADDRESS_LEN];					//�ռ��˵�ַ
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_EMAIL_ADDRESSEE;

/*Email�ռ�����Ϣ*/
typedef struct
{
	DWORD	dwNumber;											//�ռ��˸���
	HY_V1_DVR_EMAIL_ADDRESSEE	cAddressee[MAX_EMAIL_ADDRESSEE_NUMBER];//�ռ���
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_EMAIL_ADDRESSEE_INFO;

/*Email֪ͨ*/
typedef struct
{
	DWORD	dwEnable;											//Email֪ͨ,	0Ϊ��֧��,1Ϊ����,2Ϊ����
	HY_V1_DVR_EMAIL_SERVER_INFO	cServerInfo;					//��������Ϣ
	HY_V1_DVR_EMAIL_ADDRESSER_INFO	cAddresserInfo;					//��������Ϣ
	HY_V1_DVR_EMAIL_ADDRESSEE_INFO	cAddresseeInfo;					//�ռ�����Ϣ
}HY_V1_DVR_EMAIL_NOTIFY_CFG;

/*����ͨ���Ƿ�����*/
typedef struct  
{
	DWORD dwLockCamera;											// ��ǰͨ���Ƿ�������0��δ������1��������
	DWORD dwReserved[8];										// �������Է��Ժ���չ
}HY_V1_DVR_CTRL_CAMERA_NODE_CFG, *LPHY_V1_DVR_CTRL_CAMERA_NODE_CFG;

/*ͨ������״̬*/
typedef struct
{
	HY_V1_DVR_CTRL_CAMERA_NODE_CFG sControl[HY_V1_DVR_MAX_CHANNUM];	// ���16��ͨ��
	DWORD dwReserved[4];
}HY_V1_DVR_CTRL_CAMERA_CFG, *LPHY_V1_DVR_CTRL_CAMERA_CFG;

//��������ַ��󳤶�
#define MAX_SERVER_ADDR_LEN				64

/*Higheasyר��DDNS����*/
typedef struct  
{
	DWORD dwEnable;												// ��ǰ�Ƿ�����DDNS����, 0Ϊ����,1Ϊ����

	char acServer[MAX_SERVER_ADDR_LEN];							// DDNS��������IP��ַ������,Ĭ��һ��Ϊhigheasy.cn
	DWORD dwPort;												// DDNS�������Ķ˿�,Ĭ��һ��Ϊ80
	DWORD dwInterval;											// ���µ�ʱ�䣬���ڵ���5��

	char acDomainName[MAX_SERVER_ADDR_LEN];						// ��ǰ�����������³ɹ�����Ի�ȡ����ǰ�豸������(ֻ��)

	DWORD acResvered[16];										// ����
}HY_V1_DVR_SPECIAL_DDNS_CFG, *LPHY_V1_DVR_SPECIAL_DDNS_CFG;

/*����ģʽ����*/
typedef struct
{
	DWORD	dwEnable;											//�Ƿ�����,	0Ϊ��֧��,1Ϊ����,2Ϊ����
	DWORD 	dwCodecResouse;									//��Դ����ģʽ
	// 4·:0-Ĭ��4·CIF; 1- 4HD1; 2- 1D1 + 3CIF; 3- 4D1
	// 8·:0-Ĭ��8·CIF; 1- 1D1 + 7CIF; 2- 2HD1 + 6CIF; 3- 8HD; 4-8D1;
	DWORD 	dwCodecResouseMask;									//ͨ������
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_ENCODER_MODE_CFG;

/*����ͨ���Ŵ���ʾ����*/
typedef struct
{
	DWORD	dwEnable;											//�Ƿ�����,	0Ϊ��֧��,1Ϊ����,2Ϊ����
	DWORD	dwChannel;										//����ͨ��,��λ��
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_CH_ENLARGE_CFG;

/*��������ͨ���Ŵ���ʾ����*/
typedef struct
{
	DWORD	dwEnable;											//�Ƿ�����,	0Ϊ��֧��,1Ϊ����,2Ϊ����
	DWORD	dwInterval;										//��ʾ���ʱ��
	HY_V1_DVR_CH_ENLARGE_CFG	cAlarmIn[HY_V1_DVR_MAX_ALARMIN];				//��������
	HY_V1_DVR_CH_ENLARGE_CFG	cMotion[HY_V1_DVR_MAX_CHANNUM];				//�ƶ����
	HY_V1_DVR_CH_ENLARGE_CFG	cVideoLost[HY_V1_DVR_MAX_CHANNUM];			//��Ƶ��ʧ
	DWORD	dwReserve[3];										//����
}HY_V1_DVR_ALARM_CH_ENLARGE_CFG;

/*���ýṹ����*/
#define HY_V1_DVR_NET_CFG_LEN				sizeof(HY_V1_DVR_NETCFG)
#define HY_V1_DVR_PIC_CFG_LEN				sizeof(HY_V1_DVR_PICCFG)
#define HY_V1_DVR_COMPRESS_CFG_LEN			sizeof(HY_V1_DVR_COMPRESSION_INFO)
#define HY_V1_DVR_COMPRESSCFG_LEN			sizeof(HY_V1_DVR_COMPRESSIONCFG)
#define HY_V1_DVR_DECODER_CFG_LEN			sizeof(HY_V1_DVR_DECODERCFG)
#define HY_V1_DVR_ALARMIN_CFG_LEN			sizeof(HY_V1_DVR_ALARMINCFG)
#define HY_V1_DVR_ALARMOUT_CFG_LEN			sizeof(HY_V1_DVR_ALARMOUTCFG)
#define HY_V1_DVR_RECORD_CFG_LEN			sizeof(HY_V1_DVR_RECORDCFG)
#define HY_V1_DVR_USER_CFG_LEN				sizeof(HY_V1_DVR_USER)
#define HY_V1_DVR_DEVICESTATE_CFG_LEN		sizeof(HY_V1_DVR_DEVICE_STATE)
#define HY_V1_DVR_RECORDCFG_LEN			sizeof(HY_V1_DVR_RECORDCFG)
#define HY_V1_DVR_EXCEPTION_CFG_LEN		sizeof(HY_V1_DVR_EXCEPTION)
#define HY_V1_DVR_DEVICEINFO_CFG_LEN		sizeof(HY_V1_DVR_DEVICECFG)
#define HY_V1_DVR_DDNS_CFG_LEN				sizeof(HY_V1_DVR_DDNS_CFG)
#define HY_V1_DVR_HTTP_CFG_LEN				sizeof(HY_V1_DVR_HTTP_CFG)
#define HY_V1_DVR_RECORDEX_CFG_LEN			sizeof(HY_V1_DVR_RECORDEX_CFG)
#define HY_V1_DVR_CAMERA_CFG_LEN			sizeof(HY_V1_DVR_CAMERA_CFG)
#define ALARMIN_CFG_EX_LEN				sizeof (HY_V1_DVR_ALARMINCFG_EX)

/*������*/
#define HY_V1_DVR_GET_NETCFG 				102 	/*��ȡ������� */
#define HY_V1_DVR_SET_NETCFG 				103 	/*�����������*/
#define HY_V1_DVR_GET_PICCFG 				104 	/*��ȡͼ����� */
#define HY_V1_DVR_SET_PICCFG 				105 	/*����ͼ����� */
#define HY_V1_DVR_GET_COMPRESSCFG 			106 	/*��ȡѹ������ */
#define HY_V1_DVR_SET_COMPRESSCFG 			107 	/*����ѹ������*/
#define HY_V1_DVR_GET_RECORDCFG 			108 	/*��ȡ����¼����� */
#define HY_V1_DVR_SET_RECORDCFG 			109 	/*���ñ���¼�����*/
#define HY_V1_DVR_GET_ALARMINCFG 			110		/*��ȡ�����������*/
#define HY_V1_DVR_SET_ALARMINCFG			111		/*���ñ����������*/
#define HY_V1_DVR_GET_ALARMOUTCFG 			112		/*��ȡ�����������*/
#define HY_V1_DVR_SET_ALARMOUTCFG			113		/*���ñ����������*/
#define HY_V1_DVR_GET_DECODERCFG			114		/*��ȡ����������*/
#define HY_V1_DVR_SET_DECODERCFG			115		/*���ý���������*/
#define HY_V1_DVR_GET_DEVICECFG			116		/*��ȡ�豸����*/
#define HY_V1_DVR_SET_DEVICECFG			117		/*�����豸����*/
#define HY_V1_DVR_CONTROL_PTZ         		118		/*��̨����*/
#define HY_V1_DVR_CONTROL_RECORD    		119		/*����¼�����*/
#define HY_V1_DVR_CONTROL_REBOOT    		120		/*Զ������*/
#define HY_V1_DVR_GET_TIMECFG				121		/*��ȡ�豸ϵͳʱ��*/
#define HY_V1_DVR_SET_TIMECFG				122		/*�����豸ϵͳʱ��*/
#define HY_V1_DVR_CONTROL_SAVEIMG			123		/*Remote SaveImg*/
#define HY_V1_DVR_CONTROL_GETUSER			124		/*����û���*/
#define HY_V1_DVR_CONTROL_SETUSER			125		/*�����û���*/
#define HY_V1_DVR_GET_EXCEPTIONCFG			126     /*��ȡ�쳣���ò���*/
#define HY_V1_DVR_SET_EXCEPTIONCFG			127     /*�����쳣���ò���*/
#define HY_V1_DVR_GET_PREVIEWCFG			128     /*��ȡԤ��*/
#define HY_V1_DVR_SET_PREVIEWCFG			129     /*����Ԥ��*/
#define HY_V1_DVR_GET_DEVICESTATE			130		/*��ȡ�豸״̬*/
#define HY_V1_DVR_SET_DEFAULTCFG			131		/*�ָ�Ĭ������*/
#define HY_V1_DVR_GET_RECORDSTATE			132		/*��ȡ�豸¼��״̬*/
#define HY_V1_DVR_GET_ALARMINSTATE			133     /*��ȡ�豸IO����״̬*/
#define HY_V1_DVR_GET_ALARMOUTSTATE		134     /*��ȡ�豸IO���״̬*/
#define HY_V1_DVR_SET_ALARMOUTSTATE		135     /*�����豸IO���״̬*/
#define HY_V1_DVR_MANUAL_RECORD			136     /*�豸�ֶ�¼��*/
#define HY_V1_DVR_ADVANCED_PTZ_CONTROL		137     /*�߼�PTZ��̨���ƹ���*/
#define HY_V1_DVR_GET_DDNSCFG				138		/*��ȡ��̬��������*/
#define HY_V1_DVR_SET_DDNSCFG				139		/*���ö�̬��������*/
#define HY_V1_DVR_GET_HTTPCFG				140		/*��ȡHTTP������Ϣ*/
#define HY_V1_DVR_SET_HTTPCFG				141		/*����HTTP������Ϣ*/
#define HY_V1_DVR_GET_RECORDEXCFG 			142 	/*��ȡ����¼����չ����*/
#define HY_V1_DVR_SET_RECORDEXCFG 			143 	/*���ñ���¼����չ����*/

#define HY_V1_DVR_GET_CAMERACFG			144		/*��ȡ����ͷ����*/
#define HY_V1_DVR_SET_CAMERACFG			145		/*��������ͷ����*/      

#define HY_V1_DVR_GET_LATTICE_CHANNEL_NAME	146		/*��ȡ����ͨ��������*/
#define HY_V1_DVR_SET_LATTICE_CHANNEL_NAME	147		/*���õ���ͨ��������*/
#define HY_V1_DVR_GET_DDNS_CAPABILITY		148		/*��ȡ������֧��DDNS������Ϣ*/

#define HY_V1_DVR_GET_ALARMINEX_CFG		149		/*��ȡ�������������(�����¶���Ĳ���ʱ���)*/
#define HY_V1_DVR_SET_ALARMINEX_CFG		150		/*���ñ������������(�����¶���Ĳ���ʱ���)*/

#define HY_V1_DVR_GET_MOBILE_SERVICE_CFG	151		/*��ȡ�ƶ��豸��������*/
#define HY_V1_DVR_SET_MOBILE_SERVICE_CFG	152		/*�����ƶ��豸��������*/

#define HY_V1_DVR_GET_EMAIL_NOTIFY_CFG		153		/*��ȡEmail֪ͨ����*/
#define HY_V1_DVR_SET_EMAIL_NOTIFY_CFG		154		/*����Email֪ͨ����*/

#define HY_V1_DVR_DISK_FORMAT				155		/*Զ�̸�ʽ��Ӳ��*/

#define HY_V1_DVR_SET_CTRL_CMAERA_CFG		156		/*���õ�ǰCAMERA�������*/
#define HY_V1_DVR_GET_CTRL_CMAERA_CFG		157		/*��ȡ��ǰCAMERA�������*/

#define HY_V1_DVR_SET_SPECIAL_DDNS_CFG		158		/*���õ�ǰHigheasyר��DDNS����*/
#define HY_V1_DVR_GET_SPECIAL_DDNS_CFG		159		/*��ȡ��ǰHigheasyר��DDNS����*/

#define HY_V1_DVR_GET_ENCODER_MODE_CFG		160		/*��ȡ����ģʽ����*/
#define HY_V1_DVR_SET_ENCODER_MODE_CFG		161		/*���ñ���ģʽ����*/

#define HY_V1_DVR_GET_ALARM_CH_ENLARGE_CFG	162		/*��ȡ��������ͨ���Ŵ�����*/
#define HY_V1_DVR_SET_ALARM_CH_ENLARGE_CFG	163		/*���ñ�������ͨ���Ŵ�����*/

#define HY_V1_DVR_CONTROL_GETUSER_EX		164		/*����û���չ����*/
#define HY_V1_DVR_CONTROL_SETUSER_EX		165		/*�����û���չ����*/

#endif
