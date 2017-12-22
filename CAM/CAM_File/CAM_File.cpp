
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
};

#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")

 
 const int SWS_FAST_FLAGS = SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2 | SWS_CPU_CAPS_3DNOW | SWS_CPU_CAPS_ALTIVEC | SWS_CPU_CAPS_BFIN ;


#define SYSTEM_WIN32
#include <windows.h>
#include <process.h>
#include "Video.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#include "SDL.h"
#include "SDL_main.h"

#define RECT_WIDTH(lpRect) ((lpRect)->right-(lpRect)->left)
#define RECT_HEIGHT(lpRect) ((lpRect)->bottom-(lpRect)->top)


 int MapPixelFormat(AVPixelFormat fmt)
 {
	 int pixfmt = ZW_PIX_FMT_NONE;
	 switch (fmt)
	 {
	 case AV_PIX_FMT_RGB24:
		 pixfmt = ZW_PIX_FMT_RGB24;
		 break;
	 case AV_PIX_FMT_BGR24:
		 pixfmt = ZW_PIX_FMT_BGR24;
		 break;
	 case AV_PIX_FMT_YUV420P:
		 pixfmt = ZW_PIX_FMT_YUV420P;
		 break;
     /*ZW_PIX_FMT_YV12 ffmpeg不支持*/
	 case AV_PIX_FMT_NV12:
		 pixfmt = ZW_PIX_FMT_NV12;
		 break;
	 case AV_PIX_FMT_NV21:
		 pixfmt = ZW_PIX_FMT_NV21;
		 break;
	 case AV_PIX_FMT_YUV422P:
		 pixfmt = ZW_PIX_FMT_YUV422P;
		 break;
	 /*ZW_PIX_FMT_YV16 ffmpeg不支持*/
	 case AV_PIX_FMT_NV16:
		 pixfmt = ZW_PIX_FMT_NV16;
		 break;
     /*ZW_PIX_FMT_NV61 ffmpeg不支持*/
	 case AV_PIX_FMT_YUYV422:
		 pixfmt = ZW_PIX_FMT_YUYV422;
		 break;
     /*ZW_PIX_FMT_YVYU422 ffmpeg不支持*/
	 case AV_PIX_FMT_UYVY422:
		 pixfmt = ZW_PIX_FMT_UYVY422;
		 break;
     /*ZW_PIX_FMT_VYUY422 ffmpeg不支持*/
	 case AV_PIX_FMT_YUV444P:
		 pixfmt = ZW_PIX_FMT_YUV444P;
		 break;
     /*ZW_PIX_FMT_YVU444P ffmpeg不支持*/
	 default:
		 pixfmt = ZW_PIX_FMT_NONE;
		 break;
	 }

	 return pixfmt;
 }

 enum AVPixelFormat MapPixelFormat2(int fmt, int *swapCbCr)
 {
	 int swap = 0;
	 int swap2 = *swapCbCr;
	 enum AVPixelFormat pixfmt = AV_PIX_FMT_NONE;
	 switch (fmt)
	 {
	 case ZW_PIX_FMT_YUV420P:
		 pixfmt = AV_PIX_FMT_YUV420P;
		 break;
	 case ZW_PIX_FMT_YV12:
		 swap = 1;
		 pixfmt = AV_PIX_FMT_YUV420P; /*ffmpeg不支持*/
		 break;
	 case  ZW_PIX_FMT_YUV422P:
		 pixfmt = AV_PIX_FMT_YUV422P;
		 break;
	 case  ZW_PIX_FMT_YV16:
		 swap = 1;
		 pixfmt = AV_PIX_FMT_YUV422P; /*ffmpeg不支持*/
		 break;
	 case ZW_PIX_FMT_YUV444P:
		 pixfmt = AV_PIX_FMT_YUV444P;
		 break;
	 case ZW_PIX_FMT_YVU444P:
		 swap = 1;
		 pixfmt = AV_PIX_FMT_YUV444P; /*ffmpeg不支持*/
		 break;


	 case ZW_PIX_FMT_RGB24:
		 swap = 1;
		 pixfmt = swap2 == 1 ? AV_PIX_FMT_BGR24 : AV_PIX_FMT_RGB24;
		 break;
	 case ZW_PIX_FMT_BGR24:
		 swap = 1;
		 pixfmt = swap2 == 1 ? AV_PIX_FMT_RGB24 : AV_PIX_FMT_BGR24;
		 break;
	 case ZW_PIX_FMT_NV12:
		 swap = 1;
		 pixfmt = swap2 == 1 ? AV_PIX_FMT_NV21 : AV_PIX_FMT_NV12;
		 break;
	 case ZW_PIX_FMT_NV21:
		 swap = 1;
		 pixfmt = swap2 == 1 ? AV_PIX_FMT_NV12 : AV_PIX_FMT_NV21;
		 break;


	 case ZW_PIX_FMT_NV16:
		 pixfmt = AV_PIX_FMT_NV16;
		 break;
	 case ZW_PIX_FMT_NV61:
		 swap = 1;
		 pixfmt = AV_PIX_FMT_NV16; /*ffmpeg不支持*/
		 break;
	 case ZW_PIX_FMT_YUYV422:
		 pixfmt = AV_PIX_FMT_YUYV422;
		 break;
	 case ZW_PIX_FMT_YVYU422:
		 swap = 1;
		 pixfmt = AV_PIX_FMT_YUYV422; /*ffmpeg不支持*/
		 break;
	 case ZW_PIX_FMT_UYVY422:
		 pixfmt = AV_PIX_FMT_UYVY422;
		 break;
	 case ZW_PIX_FMT_VYUY422:
		 swap = 1;
		 pixfmt = AV_PIX_FMT_UYVY422; /*ffmpeg不支持*/
		 break;

	 default:
		 pixfmt = AV_PIX_FMT_NONE;
		 break;
	 }

	 *swapCbCr = swap;

	 return pixfmt;
 }


 int MapVideoCodec(enum AVCodecID codec)
 {
	 int avCodec = ZW_CODEC_ID_NONE;
	 switch (codec)
	 {
	 case AV_CODEC_ID_H264:
		 avCodec = ZW_CODEC_ID_H264;
		 break;
	 case AV_CODEC_ID_MPEG4:
		 avCodec = ZW_CODEC_ID_MPEG4;
		 break;
	 case AV_CODEC_ID_MJPEG:
		 avCodec = ZW_CODEC_ID_MJPEG;
		 break;
	 default:
		 avCodec = ZW_CODEC_ID_NONE;
		 break;
	 }

	 return avCodec;
 }

 enum AVCodecID MapVideoCodec2(int codec)
 {
	 enum AVCodecID avCodec = AV_CODEC_ID_NONE;
	 switch (codec)
	 {
	 case  ZW_CODEC_ID_H264:
		 avCodec = AV_CODEC_ID_H264;
		 break;
	 case ZW_CODEC_ID_MPEG4:
		 avCodec = AV_CODEC_ID_MPEG4;
		 break;
	 case ZW_CODEC_ID_MJPEG:
		 avCodec = AV_CODEC_ID_MJPEG;
		 break;
	 default:
		 avCodec = AV_CODEC_ID_NONE;
		 break;
	 }

	 return avCodec;
 }

 void  CalcDisplayRect(LPRECT lpWndRect, int width, int height, LPRECT displayRect)
 {
	 int winWidth = RECT_WIDTH(lpWndRect);
	 int winHeight = RECT_HEIGHT(lpWndRect);

	 float a = (float)width / (float)height;
	 float b = (float)winWidth / (float)winHeight;

	 if (a > b)
	 {
		 if (width > winWidth)
		 {
			 displayRect->left = lpWndRect->left;
			 displayRect->right = lpWndRect->right;
			 int c = (height*RECT_WIDTH(displayRect)) / width;
			 displayRect->top = lpWndRect->top + (winHeight - c) / 2;
			 displayRect->bottom = displayRect->top + c;
		 }
		 else
		 {
			 displayRect->left = lpWndRect->left + (winWidth - width) / 2;
			 displayRect->right = displayRect->left + width;
			 displayRect->top = lpWndRect->top + (winHeight - height) / 2;
			 displayRect->bottom = displayRect->top + height;
		 }
	 }
	 else
	 {
		 if (height > winHeight)
		 {
			 displayRect->top = lpWndRect->top;
			 displayRect->bottom = displayRect->top + winHeight;
			 int c = (width*winHeight) / height;
			 displayRect->left = lpWndRect->left + (winWidth - c) / 2;
			 displayRect->right = displayRect->left + c;
		 }
		 else
		 {
			 displayRect->top = lpWndRect->top + (winHeight - height) / 2;
			 displayRect->bottom = displayRect->top + height;
			 displayRect->left = lpWndRect->left + (winWidth - width) / 2;
			 displayRect->right = displayRect->left + width;
		 }
	 }
 }

struct internal_handle
{
	VideoStreamCallback OnVideoStreamReady;/*实时视频流回调*/
	RealTimeImageCallback OnRealImgReady;/*实时图像输出回调*/
	RealTimeImageCallback OnRealFixFmtImgReady;/*实时转码图像输出回调*/
	TriggerImageCallback OnTriggerImgReady;  /*外部触发抓拍图片回调*/
	TriggerImageCallback OnTriggerFixFmtImgReady;/*外部触发抓拍转码图片回调*/
	void* pUserData;

	char  szPath[ZW_LENGTH_URL];

	int width;
	int height;
	int codec;
	volatile bool started;

	CRITICAL_SECTION csEvent;
	MMRESULT timer;
	HANDLE hFrameRateEvent;
	HANDLE hVideoThread;

	handle         hPlayWnd;
	SDL_Window    *win;
	SDL_Renderer  *renderer;
	SDL_Surface   *pic;
	SDL_Rect       rcDisplayRect;
	volatile long  displaying; //是否正在显示
	RECT           rcWndRect;
	HANDLE         hDisplayEvent;
	HANDLE         hDisplayThread;

	internal_handle()
	{
		OnVideoStreamReady = NULL;
		OnRealImgReady = NULL;
		OnRealFixFmtImgReady = NULL;
		OnTriggerImgReady = NULL;
		OnTriggerFixFmtImgReady = NULL;

		pUserData = NULL;

		width = 0;
		height = 0;
		codec = ZW_CODEC_ID_NONE;
		started = false;

		displaying = 0;
		win = NULL;
		renderer = NULL;
		pic = NULL;
		rcDisplayRect = { 0 };
		rcWndRect = { 0 };

		hFrameRateEvent = NULL;
		hVideoThread = NULL;

		hDisplayThread = NULL;
	}
};

volatile long g_nSdlInit_Count = 0;//SDK初始化技术

void CALLBACK FrameRateTimer(UINT wTimerID, UINT msg,DWORD dwUser, DWORD dw1, DWORD dw2)
{
	internal_handle *pih = (internal_handle*)dwUser;
	//EnterCriticalSection(&(pih->csEvent));
	SetEvent(pih->hFrameRateEvent);
	//LeaveCriticalSection(&(pih->csEvent));	
	
}


UINT __stdcall VideoThreadFunc(LPVOID pParam)
{
	internal_handle *pih = (internal_handle*)pParam;

	if(pih->started == false) return 0;

	int ret = 0;
	AVCodecContext* suitable_dec_ctx = NULL;
	AVFormatContext* video_fmt_ctx = NULL;  
	AVCodec* codec = NULL;
	AVFrame* pFrameDecode = NULL;  
    AVFrame* pFrameFixFmt = NULL;
	uint8_t* buffer = NULL;/*RGBFrame缓存*/
	AVPacket packet; 
	SwsContext *pSWSCtx = NULL;

	av_register_all();

	if ((ret = avformat_open_input(&video_fmt_ctx, pih->szPath, NULL,NULL)) < 0) {  
		av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");  
		return ret;  
	}  

	if ((ret = avformat_find_stream_info(video_fmt_ctx,NULL)) < 0) {  
		av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");  
		avformat_close_input(&video_fmt_ctx);  
		return ret;  
	} 

	int video_stream_index = 0;
    for (unsigned int i = 0; i < video_fmt_ctx->nb_streams; i++)  
    {  
        // 找到视频码流  
		if (video_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)  
		{  
			// 初始化解码器信息  
			suitable_dec_ctx = video_fmt_ctx->streams[i]->codec;  
			pih->width = suitable_dec_ctx->width;
			pih->height = suitable_dec_ctx->height;
			pih->codec = MapVideoCodec(suitable_dec_ctx->codec_id);

			codec = avcodec_find_decoder(suitable_dec_ctx->codec_id);  

			if (NULL == codec)  
			{  
				avformat_close_input(&video_fmt_ctx);  
				return 1;  
			}  

			if(0 != avcodec_open2(suitable_dec_ctx, codec,NULL))  
			{  
				avformat_close_input(&video_fmt_ctx);  
				return 1;  
			} 

			video_stream_index = (int)i;
			break;
		}  
    } 

	pFrameDecode = av_frame_alloc();
	pFrameFixFmt = av_frame_alloc();

    int numBytes=avpicture_get_size(AV_PIX_FMT_BGR24, suitable_dec_ctx->width,suitable_dec_ctx->height);   
    buffer = new uint8_t[numBytes];  
  
    avpicture_fill((AVPicture *)pFrameFixFmt, buffer, AV_PIX_FMT_BGR24,
        suitable_dec_ctx->width, suitable_dec_ctx->height);  
  
    if (NULL == pFrameDecode || NULL == pFrameFixFmt || NULL == buffer)
    {  
        ret = -1;  
        goto exit;  
    }  

	DWORD frame_rate = video_fmt_ctx->streams[video_stream_index]->r_frame_rate.num/video_fmt_ctx->streams[video_stream_index]->r_frame_rate.den;
	DWORD frame_tm = frame_rate>0?1000/frame_rate:40;

	pih->timer = timeSetEvent((UINT)frame_tm,5,FrameRateTimer,(DWORD_PTR)pih,TIME_PERIODIC);

	DWORD prev_tm = timeGetTime();  
	DWORD cur_tm = prev_tm;
    while (pih->started && av_read_frame(video_fmt_ctx, &packet) >= 0)  
    {  
		int got_frame = 0;  
		avcodec_decode_video2(suitable_dec_ctx, pFrameDecode,&got_frame, &packet);

		if (got_frame) //一个完整的帧  
		{  
			if (pih->OnVideoStreamReady != NULL)
			{   
				int nFrametype = 3;
				if (pFrameDecode->key_frame) //I帧
				{
					nFrametype = 1;
				}
				//标准流回调
				pih->OnVideoStreamReady(pih->pUserData, (void*)packet.data, packet.size, nFrametype);
			}

			int pixfmt = MapPixelFormat(suitable_dec_ctx->pix_fmt);

			if (pixfmt != ZW_PIX_FMT_NONE && pih->OnRealImgReady != NULL)
			{
				ZWImageData imgData = { 0 };
				imgData.Width = suitable_dec_ctx->width;
				imgData.Height = suitable_dec_ctx->height;
				imgData.PixFmt = pixfmt;
				for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
					imgData.PlaneData[i] = pFrameDecode->data[i];
					imgData.LineSize[i] = pFrameDecode->linesize[i];
				}

				pih->OnRealImgReady(pih->pUserData, &imgData);
			}

			if (pSWSCtx == NULL)
			{
				pSWSCtx = sws_getContext(suitable_dec_ctx->width, suitable_dec_ctx->height,
					suitable_dec_ctx->pix_fmt, suitable_dec_ctx->width, suitable_dec_ctx->height, AV_PIX_FMT_BGR24,
					SWS_BICUBIC, NULL, NULL, NULL);
			}

			if (pSWSCtx != NULL 
				&& sws_scale(pSWSCtx, pFrameDecode->data, pFrameDecode->linesize, 0, suitable_dec_ctx->height, pFrameFixFmt->data, pFrameFixFmt->linesize))
			{

				if (pih->displaying == 0)
				{
					if (pih->pic != NULL)
					{
						SDL_FreeSurface(pih->pic);
						pih->pic = NULL;
					}
					pih->pic = SDL_CreateRGBSurfaceFrom(buffer, suitable_dec_ctx->width, suitable_dec_ctx->height, 24, pFrameFixFmt->linesize[0], 0x00ff0000, 0x0000ff00, 0x000000ff, 0);
					InterlockedIncrement(&(pih->displaying));

					SetEvent(pih->hDisplayEvent);
				}

				ZWImageData imgData = { 0 };
				imgData.Width = suitable_dec_ctx->width;
				imgData.Height = suitable_dec_ctx->height;
				imgData.PixFmt = ZW_PIX_FMT_BGR24;
				for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
					imgData.PlaneData[i] = pFrameFixFmt->data[i];
					imgData.LineSize[i] = pFrameFixFmt->linesize[i];
				}

				if (pixfmt == ZW_PIX_FMT_NONE && pih->OnRealImgReady != NULL)
				{
					pih->OnRealImgReady(pih->pUserData, &imgData);
				}

				if (pih->OnRealFixFmtImgReady != NULL) 
				{
					pih->OnRealFixFmtImgReady(pih->pUserData, &imgData);
				}
			}

			if(pih->timer == NULL)
			{
				cur_tm = timeGetTime();
				DWORD span_tm = cur_tm - prev_tm;
				if(frame_tm > span_tm)
				{
					timeBeginPeriod(1);
					Sleep(frame_tm-span_tm+30);
					timeEndPeriod(1);
				}
				prev_tm = cur_tm;
			}
			else
			{
				//EnterCriticalSection(&(pih->csEvent));
				WaitForSingleObject(pih->hFrameRateEvent,INFINITE);
				//LeaveCriticalSection(&(pih->csEvent));			
			}
		}

		av_free_packet(&packet);
    }  

	if (pih->OnVideoStreamReady != NULL)
	{
		pih->OnVideoStreamReady(pih->pUserData, NULL, 0, -1);
	}
  
exit:  

	if (pih->timer != NULL)
	{
		timeKillEvent(pih->timer);
		pih->timer = NULL;
	}

    if(buffer != NULL) delete [] buffer; 
	if(pFrameDecode != NULL) av_free(pFrameDecode);
	if(pFrameFixFmt != NULL) av_free(pFrameFixFmt);
    if(pSWSCtx != NULL) sws_freeContext(pSWSCtx); 
	avcodec_close(suitable_dec_ctx);
	avformat_close_input(&video_fmt_ctx);

	_endthreadex(0);
	return 0;
}


/*初始化视频源，传入回调用于获取码流和外部触发抓拍的图片*/
ZW_VIDEO_API handle __cdecl  InitialVideoSource(ZWVideoParam *param,
	                                         VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
	                                         RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
	                                         RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
	                                         int realFixFmt,/*实时转码图像的格式*/
	                                         TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
	                                         TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
	                                         int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
											 void *pUserData)
{
	internal_handle *pih = new internal_handle;

	pih->OnVideoStreamReady = OnVideoStreamReady;
	pih->OnRealImgReady = OnRealImgReady;
	pih->OnRealFixFmtImgReady = OnRealFixFmtImgReady;
	pih->OnTriggerImgReady = OnTriggerImgReady;
	pih->OnTriggerFixFmtImgReady = OnTriggerFixFmtImgReady;
	pih->pUserData = pUserData;

	memcpy(pih->szPath,param->Url, ZW_LENGTH_URL);
	
	long count = InterlockedIncrement(&g_nSdlInit_Count);

	if (count == 1) 
	{
		SDL_Init(SDL_INIT_VIDEO);
	}

	pih->timer = NULL;
	pih->hFrameRateEvent = CreateEvent(NULL,FALSE,TRUE,NULL);
	if(pih->hFrameRateEvent == NULL)
	{
		delete pih;
		return NULL;
	}

	pih->hDisplayEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (pih->hDisplayEvent == NULL)
	{
		CloseHandle(pih->hFrameRateEvent);
		delete pih;
		return NULL;
	}
	InitializeCriticalSection(&(pih->csEvent));

	return (handle)pih;
}


/*返回设备是否能自动重连。支持的话返回1，否则返回0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS)
{
	return 0;
}

/*获取视频源编码，返回ZW_CODEC_ID_? */
ZW_VIDEO_API int   __cdecl  GetVideoCodec(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;

	return pih->codec;	
}

UINT __stdcall DisplayVideo(LPVOID lpParam)
{
	internal_handle *pih = (internal_handle*)lpParam;
	while (pih->started)
	{
		if (pih->displaying == 1)
		{
			RECT  rcWnd;
			::GetClientRect((HWND)pih->hPlayWnd, &rcWnd);
			if (RECT_WIDTH(&rcWnd) != RECT_WIDTH(&(pih->rcWndRect))
				|| RECT_HEIGHT(&rcWnd) != RECT_HEIGHT(&(pih->rcWndRect)))
			{
				RECT  rcDisplay;

				pih->rcWndRect.left = rcWnd.left;
				pih->rcWndRect.top = rcWnd.top;
				pih->rcWndRect.right = rcWnd.right;
				pih->rcWndRect.bottom = rcWnd.bottom;

				CalcDisplayRect(&(pih->rcWndRect), pih->width, pih->height, &rcDisplay);
				SDL_SetWindowSize(pih->win, RECT_WIDTH(&rcWnd), RECT_HEIGHT(&rcWnd));
				if (pih->renderer != NULL)
					SDL_DestroyRenderer(pih->renderer);
				pih->renderer = SDL_CreateRenderer(pih->win, -1, SDL_RENDERER_ACCELERATED);

				pih->rcDisplayRect.x = rcDisplay.left;
				pih->rcDisplayRect.y = rcDisplay.top;
				pih->rcDisplayRect.w = RECT_WIDTH(&rcDisplay);
				pih->rcDisplayRect.h = RECT_HEIGHT(&rcDisplay);
			}

			SDL_Texture   *tex = SDL_CreateTextureFromSurface(pih->renderer, pih->pic);

			SDL_RenderClear(pih->renderer);
			SDL_RenderCopy(pih->renderer, tex, NULL, &(pih->rcDisplayRect));
			SDL_RenderPresent(pih->renderer);

			SDL_DestroyTexture(tex);

			InterlockedDecrement(&(pih->displaying));
		}
		else 
		{
			WaitForSingleObject(pih->hDisplayEvent, INFINITE);
		}
	}

	return 0;
}

/*开启视频源，返回0失败，1成功*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS, handle hPlayWnd)
{
	internal_handle *pih = (internal_handle*)hVS;

	if (pih->started)
		return 0;

	pih->started = true;
	pih->hVideoThread = (HANDLE)_beginthreadex(NULL, 0, VideoThreadFunc,
		(LPVOID)pih, 0, NULL);
	
	if (pih->hVideoThread == 0)
	{
		pih->started = false;
		return 0;
	}

	if (hPlayWnd != NULL)
	{
		//SDL 缺陷  SDL_DestroyRenderer 和 SDL_DestroyWindow 都会导致本地窗体对象被销毁，
		//本地窗体对象为外部传入，不应在内部被销毁
		//
		//所以判断一下当前传入的窗体是否为上次传入的窗体，如果是就直接使用创建好的sdl对象渲染
		if (pih->hPlayWnd != hPlayWnd || pih->win == NULL)
		{
			pih->hPlayWnd = hPlayWnd;
			if (pih->win != NULL)
			{
				if (pih->renderer != NULL)
					SDL_DestroyRenderer(pih->renderer);

				SDL_DestroyWindow(pih->win);
				pih->renderer = NULL;
				pih->win = NULL;
			}
			::ShowWindow((HWND)hPlayWnd, SW_SHOWNORMAL);
			::SendMessage((HWND)hPlayWnd, WM_PAINT, 0, 0);
			pih->win = SDL_CreateWindowFrom((HWND)hPlayWnd);
		}
		if (pih->win != NULL)
		{
			pih->hDisplayThread = (HANDLE)_beginthreadex(NULL, 0, DisplayVideo,
				(LPVOID)pih, 0, NULL);
		}
	}

	return 1;
}

/*触发功能*/
ZW_VIDEO_API int    __cdecl  TriggerPeripheral(handle hVS, int func)
{
	internal_handle *pih = (internal_handle*)hVS;

	return 0;
}

/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;

	if (!pih->started)
		return;

	pih->started = false;

	if (pih->hDisplayThread != NULL)
	{
		SetEvent(pih->hDisplayEvent);
		WaitForSingleObject(pih->hDisplayThread, INFINITE);
		CloseHandle(pih->hDisplayThread);
		pih->hDisplayThread = NULL;
	}

	if (pih->timer != NULL)
		timeKillEvent(pih->timer);

	if (pih->hVideoThread != NULL)
	{
		SetEvent(pih->hFrameRateEvent);
		WaitForSingleObject(pih->hVideoThread, INFINITE);
		CloseHandle(pih->hVideoThread);
		pih->hVideoThread = NULL;
	}

	if (pih->pic != NULL)
	{
		SDL_FreeSurface(pih->pic);
		pih->pic = NULL;
	}
}

/*终止视频源模块*/
ZW_VIDEO_API void   __cdecl  FinializeVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;

	StopVideoSource(hVS);

	if (pih->hDisplayEvent != NULL)
		CloseHandle(pih->hDisplayEvent);
	if (pih->hFrameRateEvent != NULL)
		CloseHandle(pih->hFrameRateEvent);
	DeleteCriticalSection(&(pih->csEvent));

	if (pih->win != NULL)
	{
		if (pih->renderer != NULL)
			SDL_DestroyRenderer(pih->renderer);
		pih->renderer = NULL;
		pih->rcWndRect.left = 0;
		pih->rcWndRect.top = 0;
		pih->rcWndRect.right = 0;
		pih->rcWndRect.bottom = 0;
		SDL_DestroyWindow(pih->win);		
		pih->win = NULL;
	}

	long count = InterlockedDecrement(&g_nSdlInit_Count);
	if (0 == count)
	{
		SDL_Quit();
	}

	delete pih;
}

