#include "StdAfx.h"

#include "PTZService.h"
#include "PtzCamera.h"

/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
/*id为云台相机编号,同一个设备在同一个进程中的id应该唯一*/
PTZService::PTZService(int id, ZWPTZParam* ptzParam, ZWVideoParam* videoParam)
{
	m_pPtzCamera = NULL;
	PtzCamera* pPtzCamera = PtzCamera::GetInstance(
		id,
		ptzParam,
		videoParam,
		NULL,
		NULL,
		NULL,
		0,
		NULL,
		NULL,
		0,
		NULL);
	m_pPtzCamera = pPtzCamera;
}

PTZService::~PTZService(void)
{
	((PtzCamera*)m_pPtzCamera)->ReleaseInstance();
}

/*执行PTZ指令。param1-pan speed和其它操作的speed，param2-tilt speed
**pan和tilt的speed为1-64，其它操作的speed为1-8
**其它操作如果不支持speed直接忽略
**当cmd为ZW_ZOOM_STEP时，param1为负值表示从当前位置ZW_ZOOM_OUT多少级别，正值表示ZW_ZOOM_IN多少级别
**
**
**返回 1-表示执行成功 0-表示执行失败 -1-表示不支持的操作
*/
int   PTZService::ExecPTZ(int cmd,int param1,int param2)
{
	return ((PtzCamera*)m_pPtzCamera)->ExecPTZ(cmd,param1,param2);
}

/*返回设备是否支持3D定位。支持的话返回1，否则返回0*/
int    PTZService::CanDirectTo()
{
	return 	((PtzCamera*)m_pPtzCamera)->CanDirectTo();
}

/*调用3D定位功能。
**zoom 表示3D定位的时候是否执行ZOOM操作，0表示不执行，1表示执行
**vw、vh 视频播放区域的宽度、高度   
**rx、ry、rw、rh   3D定位功能框选框的x坐标、y坐标、宽度、高度
**3D定位功能框选框的坐标系的定定义如下：
**坐标原点在视频播放区域的左上角，y坐标向下，x坐标向右
**
**
**返回 1-表示执行成功 0-表示执行失败
*/
int   PTZService::DirectTo(int zoom,int vw,int vh,int rx, int ry, int rw,int rh)
{
	return ((PtzCamera*)m_pPtzCamera)->DirectTo(zoom,vw,vh,rx,ry,rw,rh);
}
