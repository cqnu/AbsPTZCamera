#ifndef HIE_CLIENT_CONFIGURE_V1_H

#define HIE_CLIENT_CONFIGURE_V1_H

#define HY_V1_DVR_MAX_CHANNUM 	    16			/*最大通道数*/
#define HY_V1_DVR_NAME_LEN			32			/*用户名最大长度*/
#define HY_V1_DVR_SERIALNO_LEN 	48			/*序列号最大长度*/
#define HY_V1_DVR_MACADDR_LEN 		8			/*设备物理地址长度*/
#define HY_V1_DVR_PASSWD_LEN 		16			/*用户密码最大长度*/
#define HY_V1_DVR_MAX_ALARMIN 	    16			/*最大报警输入个数*/
#define HY_V1_DVR_MAX_ALARMOUT 	4			/*最大报警输出个数*/
#define HY_V1_DVR_MAX_STREAM_TYPES 2			/*最大编码格式个数*/
#define HY_V1_DVR_MAX_TIMESEGMENT  4			/*最大时间段个数*/
#define HY_V1_DVR_MAX_PRESET 		128			/*最大预置点个数*/
#define HY_V1_DVR_MAX_DAYS 		7			/*周最大天数*/
#define HY_V1_DVR_MAX_SHELTERNUM   4			/*遮挡区域数*/
#define HY_V1_DVR_MAX_EXCEPTIONNUM 16			/*最大异常个数*/
#define HY_V1_DVR_MAX_PTZ_NUMS     40			/*最大PTZ个数*/
#define HY_V1_DVR_PTZ_NAME_LEN     10			/*最大PTZ名字长度*/

/*PTZ协议信息*/
typedef struct {
   	BYTE    PTZNum;							/*系统支持PTZ协议的个数(只读)*/ 
	char    PTZName[HY_V1_DVR_MAX_PTZ_NUMS][HY_V1_DVR_PTZ_NAME_LEN];/*系统当前支持的PTZ协议名称(只读)*/
}HY_V1_DVR_PTZ, *LPHY_V1_DVR_PTZ;

/*时间配置信息*/
typedef struct{
	WORD dwYear;	//年
	WORD dwMonth;	//月
	WORD dwDay;		//日
	WORD dwHour;	//时
	WORD dwMinute;	//分
	WORD dwSecond;	//秒
}HY_V1_DVR_TIME,*LPHY_V1_DVR_TIME;

/*网络配置信息*/
typedef struct
{
	char  sDVRIP[16];						/* 服务器IP地址 */
	char  sDVRIPMask[16];					/* 服务器IP地址掩码 */
	char  sGatewayIP[16];					/* 网关地址 */
	BYTE  byMACAddr[HY_V1_DVR_MACADDR_LEN];	/* 服务器的物理地址(只读)*/
	DWORD  dwPPPOE;							/* 0-不启用,1-启用 */
	BYTE  sPPPoEUser[HY_V1_DVR_NAME_LEN];		/* PPPoE用户名 */
	char  sPPPoEPassword[HY_V1_DVR_PASSWD_LEN];/* PPPoE密码 */
	char  sPPPoEIP[16];						/* PPPoE IP地址(只读) */
	char  sPrimaryDNS[16];					/* 主DNS服务器 */
	char  sSecondaryDNS[16];				/* 备用DNS */
	WORD  dwAutoReg;						/* 0-不启用自动注册, 1-启用自动注册 */
	WORD  dwAutoRegIntervals;				/* 注册间隔时间, 1-999秒 */
	BYTE  sManageHostIP[16];				/* 远程管理主机地址 */
	DWORD  wManageHostPort;				    /* 远程管理主机端口 */
	WORD  wLocalCmdPort;					/* 服务器本地命令端口 */
	WORD  wLocalMediaPort;					/* 服务器本地媒体端口 */
	DWORD dwNAT;							/* 0--不启用NAT, 1-启用NAT(停用)*/
	BYTE  sNATIP[16];						/* NAT地址(停用)*/
	WORD  byLinkMode;						/* 0：TCP	1：UDP	2：多播(停用)*/
}HY_V1_DVR_NETCFG, *LPHY_V1_DVR_NETCFG;

/*设备配置信息*/
typedef struct{ 
	DWORD dwSize;
	BYTE sDVRName[HY_V1_DVR_NAME_LEN];			/* 服务器名称 */ 
	BYTE sSerialNumber[HY_V1_DVR_SERIALNO_LEN];/* 序列号(只读)*/ 
	DWORD dwSystemVideoStandard;			/*服务器视频制式：0--NTSC 1--PAL 2--SECAMA*/
	DWORD dwDVRID;						    /* 服务器 ID，用于遥控器，-1为未设置，设置范围0-99 */ 
	DWORD dwRecycleRecord; 				    /* 是否循环录像，0：不是；1：是 */ 
	DWORD dwSoftwareVersion;  				/* 软件版本号,高字高8位是主版本,低字高8位是次版本号， 低字低8位是修订版本(只读)*/ 
	DWORD dwSoftwareBuildDate; 				/* 软件生成日期,0xYYYYMMDD(只读)*/ 
	DWORD dwDSPSoftwareVersion; 			/* DSP软件版本,高字高8位是主版本,低字高8位是次版本号， 低字低8位是修订版本(只读)*/
	DWORD dwDSPSoftwareBuildDate;			/* DSP软件生成日期,0xYYYYMMDD(只读)*/
	DWORD dwPanelVersion; 					/* 前面板版本,高16位是主版本,低16位是次版本(只读)*/ 
	DWORD dwHardwareVersion; 				/* 硬件版本,高16位是主版本,低16位是次版本(只读)*/ 
	BYTE byAlarmInPortNum; 					/* 服务器报警输入个数(只读)*/ 
	BYTE byAlarmOutPortNum; 				/* 服务器报警输出个数(只读)*/ 
	BYTE byRS232Num; 						/* 服务器232串口个数(只读)*/ 
	BYTE byRS485Num; 						/* 服务器485串口个数(只读)*/ 
	BYTE byDiskCtrlNum; 					/* 服务器硬盘控制器个数(只读)*/ 
	BYTE byDiskNum;   						/* 服务器硬盘个数(只读)*/ 
	BYTE byDVRType; 						/* 服务器类型：(只读)
                                            1． 1500 4CIF DVR
                                            2． 1500 8CIF DVR
                                            3． 1500 12CIF DVR
                                            4． 1500 16CIF DVR
                                            5． 1500 4D1 DVR
                                            6． 1500 8D1 DVR
                                            7． 8950 4CIF DVR
                                            8． 1700 8CIF DVR
                                            9． 1700 16CIF DVR
                                            10．1700 4D1 DVR
                                            11．1700 8D1 DVR
                                            12．1700 16D1 DVR
                                            13．1D1 DVS
                                            14．2D1 DVS
                                            15．4CIF DVS
                                            16．1D1 DVS-ATA
                                            17．2D1 DVS-ATA
                                            18．4CIF DVS-ATA*/
	BYTE byChanNum; 						/* 服务器通道个数(只读)*/ 
	BYTE byDecordChans; 					/* 服务器解码路数(只读)*/ 
	BYTE byVGANum; 							/* VGA口的个数(只读)*/ 
	BYTE byUSBNum; 							/* USB口的个数(只读)*/ 
	char reservedData[3]; 					/* 保留 */
}HY_V1_DVR_DEVICECFG,*LPHY_V1_DVR_DEVICECFG;

#define NOACTION			0x0				/*无响应*/
#define WARNONMONITOR		0x1				/*监视器上警告*/
#define WARNONAUDIOOUT		0x2				/*声音警告*/
#define UPTOCENTER			0x4				/*上传中心*/
#define TRIGGERALARMOUT		0x8				/*触发报警输出*/
#define EMAILNOTIFY			0x10			/*Email通知*/

/*异常处理信息*/
typedef struct
{
	DWORD	dwHandleType;			        /*处理方式,处理方式的"或"结果*/
											/*0x00: 无响应*/
											/*0x01: 监视器上警告（预览图像上叠加）*/
											/*0x02: 声音警告*/
											/*0x04: 上传中心*/
											/*0x08: 触发报警输出*/
											/*0x10: Email通知*/
	BYTE byRelAlarmOut[HY_V1_DVR_MAX_ALARMOUT];/*报警触发的输出通道,报警触发的输出,为1表示触发该输出*/
}HY_V1_DVR_HANDLEEXCEPTION;

/*时间段配置信息*/
typedef struct{
	BYTE byStartHour;                       /*开始时间*/
	BYTE byStartMin;
	BYTE byStopHour;                        /*结束时间*/
	BYTE byStopMin;
}HY_V1_DVR_SCHEDTIME,*LPHY_V1_DVR_SCHEDTIME;

/*报警输入配置信息*/
typedef struct{
	DWORD dwSize;
	HY_V1_DVR_HANDLEEXCEPTION struAlarmHandleType;/* 处理方式 */ 
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT]; 
	/*共七个时间段，分别为星期一、二、三、四、五、六、日*/
	BYTE byAlarmType;						/* 报警器类型,0：常开,1：常闭 */
	BYTE byAlarmInHandle;					/* 是否处理, 0:不处理 ，1：处理 */
	BYTE byRelRecordChan[HY_V1_DVR_MAX_CHANNUM];/* 报警触发的录象通道,为1表示触发该通道 */
	BYTE byEnablePreset[HY_V1_DVR_MAX_CHANNUM];/* 是否调用预置点 */ 
	BYTE byPresetNo[HY_V1_DVR_MAX_CHANNUM];	/* 调用的云台预置点序号,一个报警输入可以调用多个通道的云台预置点, 0xff表示不调用预置点。*/
	BYTE byEnableCruise[HY_V1_DVR_MAX_CHANNUM];		/* 是否调用巡航 */ 
	BYTE byCruiseNo[HY_V1_DVR_MAX_CHANNUM];	/* 巡航 */ 
	BYTE byEnablePtzTrack[HY_V1_DVR_MAX_CHANNUM];/* 是否调用轨迹 */ 
	BYTE byPTZTrack[HY_V1_DVR_MAX_CHANNUM];	/* 调用的云台的轨迹序号 */
}HY_V1_DVR_ALARMINCFG,*LPHY_V1_DVR_ALARMINCFG;

/*报警输出配置信息*/
typedef struct{
	DWORD dwSize;
	DWORD dwAlarmOutDelay;					/* 输出保持时间(有效参数5, 10, 30 ,60, 120, 300, 600, -1手动关闭) */
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT];
	/*共七个时间段，分别为星期一、二、三、四、五、六、日*/
	BYTE byEnableTimeTable;					/*是否启用时间表*/
	BYTE byAlarmOutHandle;					/* 是否处理*/ 
}HY_V1_DVR_ALARMOUTCFG,*LPHY_V1_DVR_ALARMOUTCFG;

/*异常配置信息*/
typedef struct{
	DWORD dwSize; 
	HY_V1_DVR_HANDLEEXCEPTION struExceptionHandleType[HY_V1_DVR_MAX_EXCEPTIONNUM]; 
	/*数组0-硬盘满,1- 局域网内IP 地址冲突,2-硬盘出错,3-非法访问, 4-网线断*/ 
}HY_V1_DVR_EXCEPTION,*LPHY_V1_DVR_EXCEPTION; 

/*解码器配置信息*/
typedef struct{
	DWORD dwSize; 							/* 此结构的大小 */ 
	DWORD dwBaudRate; 						/* 波特率(bps)，0－1200，1－2400，2－4800，3－9600 */ 
	BYTE byDataBit; 						/* 数据有几位 0－4位，1－5位，2－6位，3－7位，4-8位; */ 
	BYTE byStopBit; 						/* 停止位 0－1位，1－1.5位,  2 -2位; */ 
	BYTE byParity; 							/* 校验 0－无校验，1－奇校验，2－偶校验; */ 
	BYTE byFlowcontrol; 					/* 0－无*/ 
	WORD wDecoderType;  					/* 解码器类型*/
	WORD wDecoderAddress;					/* 解码器地址:0 - 255 */ 
	WORD wComType;	 						/* 0－COM1 RS232，1－COM2 RS485 */
	HY_V1_DVR_PTZ   strPTZInfo;				/* PTZ信息 */
	BYTE bySetPreset[HY_V1_DVR_MAX_PRESET];	/* 预置点是否设置,0-没有设置,1-设置*/ 
	BYTE bySetCruise[HY_V1_DVR_MAX_PRESET];	/* 巡航是否设置,0-没有设置,1-设置 */ 
	BYTE bySetTrack[HY_V1_DVR_MAX_PRESET];		/* 轨迹是否设置,0-没有设置,1-设置*/ 
}HY_V1_DVR_DECODERCFG,*LPHY_V1_DVR_DECODERCFG;

/*时间段录像配置*/
typedef struct{ 
	HY_V1_DVR_SCHEDTIME struRecordTime; 
	BYTE byRecordType;						/* 0：定时录像1：报警录像2：移动侦测3：动测|报警，255：不启用*/ 
	char reservedData[3];					/* 保留 */ 
}HY_V1_DVR_RECORDSCHED,*LPHY_V1_DVR_RECORDSCHED; 

/*全天录像配置*/
typedef struct{ 
	WORD wAllDayRecord;						/* 是否全天录像 */ 
	BYTE byRecordType;						/* 0：定时录像1：报警录像2：移动侦测3：动测|报警*/ 
	char reservedData; 
}HY_V1_DVR_RECORDDAY; 

/*录像配置信息*/
typedef struct{
	DWORD dwSize;							/* 此结构的大小 */ 
	DWORD dwRecord; 						/* 是否录像 0-否 1-是 */ 
	DWORD dwRecordTime; 					/* 录象时间长度 */ 
	DWORD dwPreRecordTime;					/* 预录时间0-10（单位：秒）*/
    DWORD dwDelayTime;						/* 延迟录像时间 0-1800(单位：秒) */
	HY_V1_DVR_RECORDDAY struRecAllDay[HY_V1_DVR_MAX_DAYS + 1];/* 全天录像 */
	HY_V1_DVR_RECORDSCHED struRecordSched[HY_V1_DVR_MAX_DAYS + 1][HY_V1_DVR_MAX_TIMESEGMENT - 2]; 
	/*共八个时间段，分别为星期日、一、二、三、四、五、六、整个星期*/
}HY_V1_DVR_RECORDCFG,*LPHY_V1_DVR_RECORDCFG;      /* 时间段录像 */

//移动侦测
typedef struct{ 
	BYTE byMotionScope[12][16]; 			/* 侦测区域,共有12*16个小宏块,为0表示该宏块是移动侦测区域,1-表示不是 */ 
	BYTE byMotionSenstive; 					/* 移动侦测灵敏度, 0 - 2,值越小越灵敏,0xff关闭 */ 
	BYTE byEnableHandleMotion; 				/* 是否处理移动侦测 */ 
	BYTE byRelRecordChan[HY_V1_DVR_MAX_CHANNUM];/*报警触发的录象通道,为0表示不触发，为1表示触发该通道 */
	HY_V1_DVR_HANDLEEXCEPTION strMotionHandleType;/* 处理方式 */ 
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT];/*布防时间(停用)*/
}HY_V1_DVR_MOTION,*LPHY_V1_DVR_MOTION;

//信号丢失报警 
typedef struct{ 
	BYTE byEnableHandleVILost; 				/* 是否处理信号丢失报警 */ 
	HY_V1_DVR_HANDLEEXCEPTION strVILostHandleType;	/* 处理方式 */ 
	HY_V1_DVR_SCHEDTIME struAlarmTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_MAX_TIMESEGMENT];/*布防时间(停用)*/
}HY_V1_DVR_VILOST,*LPHY_V1_DVR_VILOST; 

/*马赛克遮挡*/
typedef struct{ 
	WORD wHideAreaTopLeftX;					/* 马赛克区域的x坐标 */ 
	WORD wHideAreaTopLeftY; 				/* 马赛克区域的y坐标 */ 
	WORD wHideAreaWidth; 					/* 马赛克区域的宽 */ 
	WORD wHideAreaHeight; 					/* 马赛克区域的高 */ 
}HY_V1_DVR_MOSAIC,*LPHY_V1_DVR_MOSAIC;

/*通道配置信息*/
typedef struct 
{ 
	DWORD dwSize;							/* 此结构的大小 */
	BYTE sChanName[HY_V1_DVR_NAME_LEN];		/* 通道名称 */ 
	DWORD dwVideoFormat;					/* 只读 视频制式 0-NTSC 1-PAL 2-SECAMA,视频制式配置以设备配置中的视频制式为主(只读).*/ 
	BYTE byBrightness; 						/* 亮度,0-255 */ 
	BYTE byContrast; 						/* 对比度,0-128 */ 
	BYTE bySaturation; 						/* 饱和度,0-128 */ 
	BYTE byHue; 							/* 色调,0-255 */ 
	DWORD dwShowChanName;					/* 预览的图象上是否显示通道名称,0-不显示,1-显示*/
	WORD wShowNameTopLeftX; 				/* 通道名称显示位置的x坐标, 0-379 */ 
	WORD wShowNameTopLeftY; 				/* 通道名称显示位置的y坐标, 0-459 */ 
	HY_V1_DVR_VILOST struVILost; 				/* 信号丢失报警 */
	HY_V1_DVR_MOTION struMotion; 				/* 移动侦测 */
	DWORD dwEnableHide;						/* 是否启动马赛克遮挡 ,0-否,1-是*/
	HY_V1_DVR_MOSAIC struMosaic[HY_V1_DVR_MAX_SHELTERNUM]; /*马赛克遮挡*/
	/*OSD */
	DWORD dwShowOsd; 						/* 预览的图象上是否显示OSD,0-不显示,1-显示*/
	WORD wOSDTopLeftX;						/* OSD的x坐标, 0-379 */
	WORD wOSDTopLeftY;						/* OSD的y坐标, 0-499 */
	BYTE byOSDType; 						/* OSD类型(主要是年月日格式)(只读)*/
											/* 0: XXXX-XX-XX 年月日 */
											/* 1: XX-XX-XXXX 日月年 */
											/* 2: XXXX年XX月XX日 */
											/* 3: XX日XX月 XXXX年 */
											/*若为其它值,表示该功能未实现*/
	BYTE byDispWeek; 						/* 是否显示星期(只读)*/
											/*0为不显示,1为显示,其它为未实现*/
	BYTE byOSDAttrib;						/* OSD属性:透明，闪烁(只读)*/ 
											/* 1: 透明,闪烁 */
											/* 2: 透明,不闪烁 */
											/* 3: 不透明,闪烁 */
											/* 4: 不透明,不闪烁 */
											/*若为其它值,表示该功能未实现*/
}HY_V1_DVR_PICCFG,*LPHY_V1_DVR_PICCFG;

#define   CAP_NORMAL_H264			0x01	/*普通型H264*/
#define   CAP_ADVANCED_H264			0x02	/*增强型H264*/

#define   CAP_RESOLUTION_QCIF		0x01	/*QCIF*/
#define   CAP_RESOLUTION_CIF		0x02	/*CIF*/
#define   CAP_RESULUTION_D1			0x04	/*D1*/
#define   CAP_RESULUTION_SD			0x08	/*SD*/
#define   CAP_RESULUTION_4CIF		0x10	/*4CIF*/
#define   CAP_RESULUTION_MD			0x20	/*MD*/

/*码流格式信息*/
typedef struct {
     	BYTE byStreamFormatCap; 			/* 支持的码流格式,所有支持的格式相或的结果(只读)*/
     	BYTE byResolutionCap[HY_V1_DVR_MAX_STREAM_TYPES];/* 每种编码格式下支持的分辨率格式,支持所有的格式相或的结果(只读)*/
		/*HY_V1_DVR_COMPRESSIONCFG	vCfg;
		获取主码流支持码流格式例子：
		if (vCfg.struRecordPara.strCompressCap.byStreamFormatCap & CAP_NORMAL_H264 )
			//支持普通型H264
		if (vCfg.struRecordPara.strCompressCap.byStreamFormatCap & CAP_ADVANCED_H264 )
			//支持增强型H264
		获取子码流支持码流格式例子：
		if (vCfg.struNetPara.strCompressCap.byStreamFormatCap & CAP_NORMAL_H264 )
			//支持普通型H264
		if (vCfg.struNetPara.strCompressCap.byStreamFormatCap & CAP_ADVANCED_H264 )
			//支持增强型H264
		获取主码流当前编码格式下支持的分辨率格式例子：
		if(vCfg.struRecordPara.strCompressCap.byResolutionCap[vCfg.struRecordPara. byStreamFormat] & CAP_RESOLUTION_CIF)
			//主码流当前编码格式下支持CIF分辨率压缩
		获取子码流当前编码格式下支持的分辨率格式例子：
		if(vCfg.struNetPara.strCompressCap.byResolutionCap[vCfg.struNetPara.byStreamFormat] & CAP_RESOLUTION_CIF)
			//子码流当前编码格式下支持CIF分辨率压缩
		*/
}HY_V1_DVR_COMPRESSCAP, *LPHY_V1_DVR_COMPRESSIONCAP;

/*压缩信息*/
typedef struct{
	DWORD dwVideoFrameRate; 				/* 帧率
                                    		主码流：0-FULL; 1-1; 2-2; 3-4; 4-6; 5-8; 6-10; 7-12; 8-16; 9-20 
                                    		子码流：0-1;1-2;2-3;3-4;4-5;5-6;6-7;7-8;8-9;9-10;11-12;12-13;13-14;14-15 */
	BYTE byStreamType; 						/* 码流类型0-视频流,1-复合流；子码流无意义 */ 
	BYTE byResolution;						/* 主码流：分辨率 0 - CIF，1 - D1，2 - SD ；
											子码流：分辨率  0 - QCIF， 1 - CIF */
	WORD byStreamWidth; 					/* 码流宽度(停用)*/
	WORD byStreamHeight;					/* 码流高度(停用)*/
	BYTE byStreamFormat; 					/* 码流格式 0-普通型H.264, 1---增强型H.264 */ 
    BYTE byBitrateType; 					/* 码率类型0：变码率，1：定码率，2：限码率；子码流无意义*/
	DWORD dwVideoBitrate; 					/* 视频码率 0-16K 1-32K 2-48k 3-64K 4-80K 5-96K 6-128K 7-160k 8-192K 9-224K 10-256K 11-320K 12-384K 13-448K 14-512K 15-640K 16-768K 17-896K 18-1024K 19-1280K 20-1536K 21-1792K 22-2048K。大于100的值表示自定义码流，实际码流值为dwVideoBitrate - 100。最小值16k 最大值8192k */

	BYTE byPicQuality; 						/* 图象质量 0-最好 1-次好 2-较好 3-一般 4-较差 5-差 此信息在变码流时有效；子码流无意义*/
	HY_V1_DVR_COMPRESSCAP strCompressCap;		/* 压缩能力， 仅在获取参数时有效（只读）*/
}HY_V1_DVR_COMPRESSION_INFO,*LPHY_V1_DVR_COMPRESSION_INFO;

/*压缩配置信息*/
typedef struct{ 
	DWORD dwSize; 							/* 此结构的大小，目前被复用为是否启动子码流 ,该参数为1时表示启用子码流,为0时表示不启用子码流.*/
	HY_V1_DVR_COMPRESSION_INFO struRecordPara;	/* 设备本地录像（主码流）*/
	HY_V1_DVR_COMPRESSION_INFO struNetPara;	/* 网络视频传输（子码流）*/
}HY_V1_DVR_COMPRESSIONCFG,*LPHY_V1_DVR_COMPRESSIONCFG;

#define HY_V1_DVR_MAX_RIGHT    16				/*最大权限个数*/
#define HY_V1_DVR_MAX_USERNUM  8       		/*最大用户个数*/

/*用户权限信息*/
typedef struct{ 
	BYTE sUserName[HY_V1_DVR_NAME_LEN];		/* 用户名 */
	BYTE sPassword[HY_V1_DVR_PASSWD_LEN];		/* 密码 */
	BYTE byLocalRight[HY_V1_DVR_MAX_RIGHT];	/* 权限 */
											/* 数组0: 本地控制云台*/ 
											/* 数组1: 本地手动录象*/ 
											/* 数组2: 本地回放*/ 
											/* 数组3: 本地备份*/
											/* 数组4: 本地系统设置*/ 
											/* 数组5: 本地高级操作(日志，清除报警，异常处理，重启，升级)*/
											/* 数组6: 关机*/ 
	BYTE  byRemoteRight[HY_V1_DVR_MAX_RIGHT];	/* 权限 */ 
											/* 数组0: 控制云台*/ 
											/* 数组1: 手动录象*/ 
											/* 数组2: 远程回放*/ 
											/* 数组3: 本地查看状态、日志*/ 
											/* 数组4: 本地系统设置*/ 			
											/* 数组5: 本地高级操作(升级，重启，关机， 清除报警， 异常处理)*/
											/* 数组6: 远程发起语音对讲*/ 
											/* 数组7: 远程预览*/ 
	char sUserIP[16];						/* 用户IP地址(为0时表示允许任何地址) */ 
	BYTE byMACAddr[HY_V1_DVR_MACADDR_LEN];		/* 物理地址 */ 
}HY_V1_DVR_USER_INFO,*LPHY_V1_DVR_USER_INFO;

/*用户配置信息*/
typedef struct{ 
	DWORD dwSize; 
	HY_V1_DVR_USER_INFO struUser[HY_V1_DVR_MAX_USERNUM]; 
}HY_V1_DVR_USER,*LPHY_V1_DVR_USER;

/*用户权限信息*/
typedef struct{ 
	BYTE sUserName[HY_V1_DVR_NAME_LEN];		/* 用户名 */
	BYTE sPassword[HY_V1_DVR_PASSWD_LEN];		/* 密码 */
	DWORD dwLocalRight[HY_V1_DVR_MAX_RIGHT];	/* 权限 */
											/* 数组0: 本地控制云台*/ 
											/* 数组1: 本地手动录象*/ 
											/* 数组2: 本地回放*/ 
											/* 数组3: 本地备份*/
											/* 数组4: 本地系统设置*/ 
											/* 数组5: 本地高级操作(日志，清除报警，异常处理，重启，升级)*/
											/* 数组6: 关机*/ 
	DWORD dwRemoteRight[HY_V1_DVR_MAX_RIGHT];	/* 权限 */ 
											/* 数组0: 控制云台*/ 
											/* 数组1: 手动录象*/ 
											/* 数组2: 远程回放*/ 
											/* 数组3: 本地查看状态、日志*/ 
											/* 数组4: 本地系统设置*/ 			
											/* 数组5: 本地高级操作(升级，重启，关机， 清除报警， 异常处理)*/
											/* 数组6: 远程发起语音对讲*/ 
											/* 数组7: 远程预览*/ 
	char sUserIP[16];						/* 用户IP地址(为0时表示允许任何地址) */ 
	BYTE byMACAddr[HY_V1_DVR_MACADDR_LEN];		/* 物理地址 */ 
}HY_V1_DVR_USER_INFO_EX,*LPHY_V1_DVR_USER_INFO_EX;

/*用户配置信息*/
typedef struct{ 
	DWORD dwSize; 
	HY_V1_DVR_USER_INFO_EX struUser[HY_V1_DVR_MAX_USERNUM]; 
}HY_V1_DVR_USER_EX,*LPHY_V1_DVR_USER_EX;

#define HY_V1_DVR_MAX_HARDDISK_NUM	8				/*服务器最大硬盘数*/
/*硬盘状态信息*/
typedef struct
{
	DWORD dwCapacity;							/* 容量,M为单位(只读)*/
	DWORD dwSpare;								/* 剩余容量(只读)*/
	BYTE  byWriteState;							/* 当前写入状态 0表示无写入，1表示正在写入(只读)*/
}HY_V1_DVR_HARDDISK_STATE, *LPHY_V1_DVR_HARDDISK_STATE;

/*通道状态信息*/
typedef struct
{
	DWORD dwVideoBitrate;						/* 主码流码率(只读)*/
	DWORD dwChildVideoBitrate;					/* 子码流码率(只读)*/
	BYTE  byVideo;								/* 录像状态(只读)*/
	BYTE  bySignal;								/* 信号状态(只读)*/
	BYTE  byUseChildBitrate;					/* 是否启动子码流(只读)*/
}HY_V1_DVR_CHANNEL_STATE, *LPHY_V1_DVR_CHANNEL_STATE;

/*设备状态配置信息*/
typedef struct
{
	HY_V1_DVR_HARDDISK_STATE cHardState[HY_V1_DVR_MAX_HARDDISK_NUM];
												/*硬盘状态*/
	HY_V1_DVR_CHANNEL_STATE cChanState[HY_V1_DVR_MAX_CHANNUM];
												/*通道状态*/
}HY_V1_DVR_DEVICE_STATE, *LPHY_V1_DVR_DEVICE_STATE;

#define HY_V1_DVR_MAX_DDNS				128			//最大动态域名长度
#define HY_V1_DVR_MAX_DDNS_USER		64			//最大动态域名用户长度
#define HY_V1_DVR_MAX_DDNS_PASS		64			//最大动态域名密码长度

/*设备DDNS配置信息*/
typedef struct  
{
	BYTE bytState;								/*状态,0为不支持,1为启用,2为禁用*/
	char sDDNS[HY_V1_DVR_MAX_DDNS];				/*动态域名地址*/
	char sUserName[HY_V1_DVR_MAX_DDNS_USER];		/*动态域名用户名称*/
	char sUserPass[HY_V1_DVR_MAX_DDNS_PASS];		/*动态域名用户密码*/
	BYTE bytReserve[3];							/*保留位*/
}HY_V1_DVR_DDNS_CFG, *LPHY_V1_DVR_DDNS_CFG;

/*设备HTTP配置信息*/
typedef struct  
{
	BYTE bytState;								/*状态,0为不支持,1为启用,2为禁用*/
	BYTE bytReserve;							/*保留位*/
	WORD wHTTPort;								/*HTTP端口*/
}HY_V1_DVR_HTTP_CFG, *LPHY_V1_DVR_HTTP_CFG;

#define HY_V1_DVR_RECORDSEG_DAY		24 * 2		//每天录像时间段

/*设备本地录像配置信息,以30分钟为一个时间段单位*/
typedef struct  
{
	BYTE bytState;								/*状态,0为不支持,1为启用,2为禁用*/
	BYTE bytReserve[3];							/*保留位*/
	BYTE bytRecordTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_RECORDSEG_DAY];/*录像状态*/
	/*0:不录像, 1:定时录像, 2:报警录像 3:移动侦测 4:动测|报警*/
	DWORD dwPreRecordTime;						/* 预录时间0-10(单位:秒)*/
    DWORD dwDelayTime;							/* 延迟录像时间 0-1800(单位:秒) */
}HY_V1_DVR_RECORDEX_CFG, *LPHY_V1_DVR_RECORDEX_CFG;

/*如果需要控制前端摄像头,采用以下结构体*/
typedef struct
{
	WORD wBLC;									/* 自动亮度控制,0 关闭 1 打开*/
	WORD wMIRROR;								/* 镜像		0 关闭 1 打开*/
	WORD wAGC;									/* 自动增益控制	0 关闭 1 打开*/
	WORD wAE;									/* 自动曝光	0 关闭 1 打开*/
	WORD wATW;									/* 自动白平衡	保留*/
	WORD wPOSNEG;								/* 负片		保留*/
	WORD wAESHUT;								/* 电子快门	保留*/
	WORD wReserve;								/* 保留*/
}HY_V1_DVR_CAMERA_CFG,*LPHY_V1_DVR_CAMERA_CFG;

//点阵串行数组信息长度 24 * 24(24点阵双字) * 10 (最大10个双字)
#define LATTICE_ARRAY_LEN		24 * 24 * 10
/*通道名点阵信息*/
typedef struct
{
	BYTE bytLattice[LATTICE_ARRAY_LEN];			//点阵信息
}HY_V1_DVR_CHNAME_LATTICE_INFO, *LPHY_V1_DVR_CHNAME_LATTICE_INFO;

//字体名长度
#define FONT_NAME_LEN	32
/*字体信息*/
typedef struct
{
	char cFontName[FONT_NAME_LEN];				//字体名
	BYTE bytFontSize;							//字体大小
	BYTE bytReserve[7];							//保留
}HY_V1_DVR_FONT_INFO, *LPHY_V1_DVR_FONT_INFO;

//点阵通道名最大长度,含结束符
#define LATTICE_CHANNEL_NAME	20 + 1
/*点阵通道名配置*/
typedef struct
{
	char strName[LATTICE_CHANNEL_NAME];			//通道名称(字符串)
	BYTE bytReserve[3];							//保留
	WORD wWidth;								//通道名称完整字符串宽度
	WORD wHeight;								//通道名称完整字符串高度
	HY_V1_DVR_FONT_INFO	cFontInfo;				//通道名字体信息
}HY_V1_DVR_LATTICE_CHNAME_CFG, *LPHY_V1_DVR_LATTICE_CHNAME_CFG;

/*设置点阵通道名配置*/
typedef struct
{
	HY_V1_DVR_LATTICE_CHNAME_CFG		cChannelNameCfg;//通道名配置
	HY_V1_DVR_CHNAME_LATTICE_INFO	cLatticeChNameInfo;	//通道名点阵信息
}HY_V1_DVR_SET_LATTICE_CHNAME_CFG, *LPHY_V1_DVR_SET_LATTICE_CHNAME_CFG;

/*获取点阵通道名配置*/
typedef struct
{
	HY_V1_DVR_LATTICE_CHNAME_CFG		cChannelNameCfg;	//通道名配置
}HY_V1_DVR_GET_LATTICE_CHNAME_CFG, *LPHY_V1_DVR_GET_LATTICE_CHNAME_CFG;

//服务器端DDNS支持最大个数
#define MAX_DDNS_CAPABILITY		40
//服务器端DDNS信息最大长度(含结束符)
#define MAX_DDNS_ADDR_LEN			64
/*服务器端DDNS支持结构信息*/
typedef struct
{
	BYTE bytValidDNSNum;						//有效DNS个数
	BYTE bytReserve[3];							//保留
	char strDDNS[MAX_DDNS_CAPABILITY][MAX_DDNS_ADDR_LEN];//DDNS名称(字符串)
}HY_V1_DVR_DDNS_CAPABILITY, *LPHY_V1_DVR_DDNS_CAPABILITY;

typedef struct  
{
	DWORD dwSize;
	HY_V1_DVR_HANDLEEXCEPTION struAlarmHandleType;					/* 处理方式 */
	
	BYTE bytProcessTime[HY_V1_DVR_MAX_DAYS][HY_V1_DVR_RECORDSEG_DAY];	/*布防时间表,其中1为布防时间，0为撤防时间*/
	
	BYTE byAlarmType;											/*报警器类型,0：常开,1：常闭 */
	BYTE byAlarmInHandle;										/* 是否处理 */
	BYTE byRelRecordChan[HY_V1_DVR_MAX_CHANNUM];					/*报警触发的录象通道,为1表示触发该通道 */
	BYTE byEnablePreset[HY_V1_DVR_MAX_CHANNUM];					/* 是否调用预置点 */
	BYTE byPresetNo[HY_V1_DVR_MAX_CHANNUM];						/* 调用的云台预置点序号,一个报警输入可以调        用多个通道的云台预置点, 0xff表示不调用预置点。 */
	BYTE byEnableCruise[HY_V1_DVR_MAX_CHANNUM];					/* 是否调用巡航 */
	BYTE byCruiseNo[HY_V1_DVR_MAX_CHANNUM];						/* 巡航 */
	BYTE byEnablePtzTrack[HY_V1_DVR_MAX_CHANNUM];					/* 是否调用轨迹 */
	BYTE byPTZTrack[HY_V1_DVR_MAX_CHANNUM];						/* 调用的云台的轨迹序号 */
}HY_V1_DVR_ALARMINCFG_EX, *LPHY_V1_DVR_ALARMINCFG_EX;

//最大移动服务数
#define MAX_MOBILE_SERVICE_NUMBER			16
//移动服务名最大长度
#define MAX_MOBILE_SERVICE_NAME_LEN			64
/*移动设备服务信息*/
typedef struct
{
	DWORD	dwStatus;											//服务状态,	0为不支持,1为启用,2为禁用
	char	cServiceName[MAX_MOBILE_SERVICE_NAME_LEN];			//服务名
	DWORD	dwServicePort;										//服务端口
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_MOBILE_SERVICE_INFO;

/*移动设备服务*/
typedef struct
{
	DWORD	dwServiceNumber;									//有效服务数
	HY_V1_DVR_MOBILE_SERVICE_INFO	cService[MAX_MOBILE_SERVICE_NUMBER];//服务信息
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_MOBILE_SERVICE;

//Email最大用户名长度
#define MAX_EMAIL_USERNAME_LEN				32
//Email最大密码长度
#define MAX_EMAIL_PASSWORD_LEN				32
//Email服务器地址最大长度
#define MAX_EMAIL_SERVER_ADDR_LEN			64
//Email最大邮件地址长度
#define MAX_EMAIL_ADDRESS_LEN				64
//Email最大收件人个数
#define MAX_EMAIL_ADDRESSEE_NUMBER			16
/*Email服务器信息*/
typedef struct
{
	char	cServerAddress[MAX_EMAIL_SERVER_ADDR_LEN];			//服务器地址
	DWORD	dwServerPort;										//服务器端口
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_EMAIL_SERVER_INFO;

/*Email发件人信息*/
typedef struct
{
	char	cUserName[MAX_EMAIL_USERNAME_LEN];					//用户名称
	char	cPassWord[MAX_EMAIL_PASSWORD_LEN];					//用户密码
	char	cAddresser[MAX_EMAIL_ADDRESS_LEN];					//发件人地址
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_EMAIL_ADDRESSER_INFO;

/*Email收件人*/
typedef struct
{
	char	cAddressee[MAX_EMAIL_ADDRESS_LEN];					//收件人地址
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_EMAIL_ADDRESSEE;

/*Email收件人信息*/
typedef struct
{
	DWORD	dwNumber;											//收件人个数
	HY_V1_DVR_EMAIL_ADDRESSEE	cAddressee[MAX_EMAIL_ADDRESSEE_NUMBER];//收件人
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_EMAIL_ADDRESSEE_INFO;

/*Email通知*/
typedef struct
{
	DWORD	dwEnable;											//Email通知,	0为不支持,1为启用,2为禁用
	HY_V1_DVR_EMAIL_SERVER_INFO	cServerInfo;					//服务器信息
	HY_V1_DVR_EMAIL_ADDRESSER_INFO	cAddresserInfo;					//发件人信息
	HY_V1_DVR_EMAIL_ADDRESSEE_INFO	cAddresseeInfo;					//收件人信息
}HY_V1_DVR_EMAIL_NOTIFY_CFG;

/*控制通道是否被锁定*/
typedef struct  
{
	DWORD dwLockCamera;											// 当前通道是否被锁定，0，未锁定，1，被锁定
	DWORD dwReserved[8];										// 保留，以防以后扩展
}HY_V1_DVR_CTRL_CAMERA_NODE_CFG, *LPHY_V1_DVR_CTRL_CAMERA_NODE_CFG;

/*通道控制状态*/
typedef struct
{
	HY_V1_DVR_CTRL_CAMERA_NODE_CFG sControl[HY_V1_DVR_MAX_CHANNUM];	// 最大16个通道
	DWORD dwReserved[4];
}HY_V1_DVR_CTRL_CAMERA_CFG, *LPHY_V1_DVR_CTRL_CAMERA_CFG;

//服务器地址最大长度
#define MAX_SERVER_ADDR_LEN				64

/*Higheasy专用DDNS服务*/
typedef struct  
{
	DWORD dwEnable;												// 当前是否启用DDNS服务, 0为禁用,1为启用

	char acServer[MAX_SERVER_ADDR_LEN];							// DDNS服务器的IP地址或域名,默认一般为higheasy.cn
	DWORD dwPort;												// DDNS服务器的端口,默认一般为80
	DWORD dwInterval;											// 更新的时间，大于等于5秒

	char acDomainName[MAX_SERVER_ADDR_LEN];						// 当前的域名，更新成功后可以获取到当前设备的域名(只读)

	DWORD acResvered[16];										// 保留
}HY_V1_DVR_SPECIAL_DDNS_CFG, *LPHY_V1_DVR_SPECIAL_DDNS_CFG;

/*编码模式配置*/
typedef struct
{
	DWORD	dwEnable;											//是否启用,	0为不支持,1为启用,2为禁用
	DWORD 	dwCodecResouse;									//资源分配模式
	// 4路:0-默认4路CIF; 1- 4HD1; 2- 1D1 + 3CIF; 3- 4D1
	// 8路:0-默认8路CIF; 1- 1D1 + 7CIF; 2- 2HD1 + 6CIF; 3- 8HD; 4-8D1;
	DWORD 	dwCodecResouseMask;									//通道掩码
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_ENCODER_MODE_CFG;

/*联动通道放大显示配置*/
typedef struct
{
	DWORD	dwEnable;											//是否启用,	0为不支持,1为启用,2为禁用
	DWORD	dwChannel;										//联动通道,按位移
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_CH_ENLARGE_CFG;

/*报警联动通道放大显示配置*/
typedef struct
{
	DWORD	dwEnable;											//是否启用,	0为不支持,1为启用,2为禁用
	DWORD	dwInterval;										//显示间隔时间
	HY_V1_DVR_CH_ENLARGE_CFG	cAlarmIn[HY_V1_DVR_MAX_ALARMIN];				//报警输入
	HY_V1_DVR_CH_ENLARGE_CFG	cMotion[HY_V1_DVR_MAX_CHANNUM];				//移动侦测
	HY_V1_DVR_CH_ENLARGE_CFG	cVideoLost[HY_V1_DVR_MAX_CHANNUM];			//视频丢失
	DWORD	dwReserve[3];										//保留
}HY_V1_DVR_ALARM_CH_ENLARGE_CFG;

/*配置结构长度*/
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

/*命令字*/
#define HY_V1_DVR_GET_NETCFG 				102 	/*获取网络参数 */
#define HY_V1_DVR_SET_NETCFG 				103 	/*设置网络参数*/
#define HY_V1_DVR_GET_PICCFG 				104 	/*获取图象参数 */
#define HY_V1_DVR_SET_PICCFG 				105 	/*设置图象参数 */
#define HY_V1_DVR_GET_COMPRESSCFG 			106 	/*获取压缩参数 */
#define HY_V1_DVR_SET_COMPRESSCFG 			107 	/*设置压缩参数*/
#define HY_V1_DVR_GET_RECORDCFG 			108 	/*获取本地录像参数 */
#define HY_V1_DVR_SET_RECORDCFG 			109 	/*设置本地录像参数*/
#define HY_V1_DVR_GET_ALARMINCFG 			110		/*获取报警输入参数*/
#define HY_V1_DVR_SET_ALARMINCFG			111		/*设置报警输入参数*/
#define HY_V1_DVR_GET_ALARMOUTCFG 			112		/*获取报警输出参数*/
#define HY_V1_DVR_SET_ALARMOUTCFG			113		/*设置报警输出参数*/
#define HY_V1_DVR_GET_DECODERCFG			114		/*获取解码器参数*/
#define HY_V1_DVR_SET_DECODERCFG			115		/*设置解码器参数*/
#define HY_V1_DVR_GET_DEVICECFG			116		/*获取设备参数*/
#define HY_V1_DVR_SET_DEVICECFG			117		/*设置设备参数*/
#define HY_V1_DVR_CONTROL_PTZ         		118		/*云台控制*/
#define HY_V1_DVR_CONTROL_RECORD    		119		/*本地录像控制*/
#define HY_V1_DVR_CONTROL_REBOOT    		120		/*远程重启*/
#define HY_V1_DVR_GET_TIMECFG				121		/*获取设备系统时间*/
#define HY_V1_DVR_SET_TIMECFG				122		/*设置设备系统时间*/
#define HY_V1_DVR_CONTROL_SAVEIMG			123		/*Remote SaveImg*/
#define HY_V1_DVR_CONTROL_GETUSER			124		/*获得用户名*/
#define HY_V1_DVR_CONTROL_SETUSER			125		/*设置用户名*/
#define HY_V1_DVR_GET_EXCEPTIONCFG			126     /*获取异常配置参数*/
#define HY_V1_DVR_SET_EXCEPTIONCFG			127     /*设置异常配置参数*/
#define HY_V1_DVR_GET_PREVIEWCFG			128     /*获取预览*/
#define HY_V1_DVR_SET_PREVIEWCFG			129     /*设置预览*/
#define HY_V1_DVR_GET_DEVICESTATE			130		/*获取设备状态*/
#define HY_V1_DVR_SET_DEFAULTCFG			131		/*恢复默认配置*/
#define HY_V1_DVR_GET_RECORDSTATE			132		/*获取设备录像状态*/
#define HY_V1_DVR_GET_ALARMINSTATE			133     /*获取设备IO输入状态*/
#define HY_V1_DVR_GET_ALARMOUTSTATE		134     /*获取设备IO输出状态*/
#define HY_V1_DVR_SET_ALARMOUTSTATE		135     /*设置设备IO输出状态*/
#define HY_V1_DVR_MANUAL_RECORD			136     /*设备手动录像*/
#define HY_V1_DVR_ADVANCED_PTZ_CONTROL		137     /*高级PTZ云台控制功能*/
#define HY_V1_DVR_GET_DDNSCFG				138		/*获取动态域名参数*/
#define HY_V1_DVR_SET_DDNSCFG				139		/*设置动态域名参数*/
#define HY_V1_DVR_GET_HTTPCFG				140		/*获取HTTP配置信息*/
#define HY_V1_DVR_SET_HTTPCFG				141		/*设置HTTP配置信息*/
#define HY_V1_DVR_GET_RECORDEXCFG 			142 	/*获取本地录像扩展参数*/
#define HY_V1_DVR_SET_RECORDEXCFG 			143 	/*设置本地录像扩展参数*/

#define HY_V1_DVR_GET_CAMERACFG			144		/*获取摄像头参数*/
#define HY_V1_DVR_SET_CAMERACFG			145		/*设置摄像头参数*/      

#define HY_V1_DVR_GET_LATTICE_CHANNEL_NAME	146		/*获取点阵通道名参数*/
#define HY_V1_DVR_SET_LATTICE_CHANNEL_NAME	147		/*设置点阵通道名参数*/
#define HY_V1_DVR_GET_DDNS_CAPABILITY		148		/*获取服务器支持DDNS能力信息*/

#define HY_V1_DVR_GET_ALARMINEX_CFG		149		/*获取报警输入的配置(包括新定义的布防时间表)*/
#define HY_V1_DVR_SET_ALARMINEX_CFG		150		/*设置报警输入的配置(包括新定义的布防时间表)*/

#define HY_V1_DVR_GET_MOBILE_SERVICE_CFG	151		/*获取移动设备服务配置*/
#define HY_V1_DVR_SET_MOBILE_SERVICE_CFG	152		/*设置移动设备服务配置*/

#define HY_V1_DVR_GET_EMAIL_NOTIFY_CFG		153		/*获取Email通知配置*/
#define HY_V1_DVR_SET_EMAIL_NOTIFY_CFG		154		/*设置Email通知配置*/

#define HY_V1_DVR_DISK_FORMAT				155		/*远程格式化硬盘*/

#define HY_V1_DVR_SET_CTRL_CMAERA_CFG		156		/*设置当前CAMERA控制情况*/
#define HY_V1_DVR_GET_CTRL_CMAERA_CFG		157		/*获取当前CAMERA控制情况*/

#define HY_V1_DVR_SET_SPECIAL_DDNS_CFG		158		/*设置当前Higheasy专用DDNS配置*/
#define HY_V1_DVR_GET_SPECIAL_DDNS_CFG		159		/*获取当前Higheasy专用DDNS配置*/

#define HY_V1_DVR_GET_ENCODER_MODE_CFG		160		/*获取编码模式配置*/
#define HY_V1_DVR_SET_ENCODER_MODE_CFG		161		/*设置编码模式配置*/

#define HY_V1_DVR_GET_ALARM_CH_ENLARGE_CFG	162		/*获取报警联动通道放大配置*/
#define HY_V1_DVR_SET_ALARM_CH_ENLARGE_CFG	163		/*设置报警联动通道放大配置*/

#define HY_V1_DVR_CONTROL_GETUSER_EX		164		/*获得用户扩展配置*/
#define HY_V1_DVR_CONTROL_SETUSER_EX		165		/*设置用户扩展配置*/

#endif
