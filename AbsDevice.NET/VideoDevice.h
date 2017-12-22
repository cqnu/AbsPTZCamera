// Parking.Adl.h

#pragma once

#include <string>  
#include <msclr\gcroot.h> 
#include <msclr\marshal_cppstd.h> 
#include "Video.h"
#include "VideoService.h"

using namespace msclr;
using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace AbsDevice{
namespace NET {

	public struct ImageData
	{
		int Width;

		int Height;

		int PixFmt;

		IntPtr Plane1;
		IntPtr Plane2;
		IntPtr Plane3;

		int LineSize1;
		int LineSize2;
		int LineSize3;
	};

	public delegate void ManagedRealtimeImageReadyHandler(ImageData img);

	private delegate void RealtimeImageReadyHandler(void *pUserData, ZWImageData* img);

	public delegate void ManagedTrigerImageReadyHandler(ImageData img, IntPtr data, int func);

	private delegate void TrigerImageReadyHandler(void *pUserData, ZWImageData* img, void* data, int func);

	public ref class VideoDevice
	{
	private:
		VideoService *m_pVideoService; 
		gcroot<VideoDevice^> *m_gcrThis;
		RealtimeImageReadyHandler^ m_hOnRealImage;
		GCHandle      m_gchOnRealImageReady;
		TrigerImageReadyHandler^ m_hOnTriggerImage;
		GCHandle      m_gchOnTriggerImageReady;
		ManagedRealtimeImageReadyHandler ^m_mhOnRealImageReady;
		ManagedTrigerImageReadyHandler ^m_mhOnTriggerImageReady;

	public:
		VideoDevice(
			int id,
			String ^cameraIp,
			String ^localIp,
			int cameraPort,
			String ^userName,
			String ^password,
			int addressNum,	
			String ^videoUrl,
			ManagedRealtimeImageReadyHandler ^mhOnRealImageReady,
			ManagedTrigerImageReadyHandler ^mhOnOnTriggerImageReady
			)
		{	
			m_mhOnRealImageReady = mhOnRealImageReady;
			m_mhOnTriggerImageReady = mhOnOnTriggerImageReady;

		    ZWVideoParam param;
			memset(&param,0,sizeof(ZWVideoParam));

			param.CameraPort = cameraPort;
			param.AddressNum = addressNum;

			if(cameraIp == nullptr)
				cameraIp = String::Empty;

			if(localIp == nullptr)
				localIp = String::Empty;

			if(videoUrl == nullptr)
				videoUrl = String::Empty;

			std::string cameraIpCpp = marshal_as<std::string>(cameraIp);
			std::string localIpCpp = marshal_as<std::string>(localIp);
			std::string userNameCpp = marshal_as<std::string>(userName);
			std::string passwordCpp = marshal_as<std::string>(password);
			std::string videUrlCpp = marshal_as<std::string>(videoUrl);

			int len = cameraIpCpp.size();
			len = len > 16 ? 16 : len;
			memcpy(param.CameraIp,cameraIpCpp.c_str(),len);

			len = localIpCpp.size();
			len = len > 16 ? 16 : len;
			memcpy(param.LocalIp,localIpCpp.c_str(),len);

			len = userNameCpp.size();
			len = len > 32 ? 32 : len;
			memcpy(param.UserName, userNameCpp.c_str(), len);

			len = passwordCpp.size();
			len = len > 32 ? 32 : len;
			memcpy(param.Password, passwordCpp.c_str(), len);

			len = videUrlCpp.size();
			len = len > 256 ? 256 : len;
			memcpy(param.Url,videUrlCpp.c_str(),len);

			m_hOnRealImage = gcnew RealtimeImageReadyHandler(OnRealImageReady);
			m_gchOnRealImageReady = GCHandle::Alloc(m_hOnRealImage);
			IntPtr ipOnRealImage = Marshal::GetFunctionPointerForDelegate(m_hOnRealImage);
			RealTimeImageCallback pfnOnRealImageReady = static_cast<RealTimeImageCallback>(ipOnRealImage.ToPointer());

			m_hOnTriggerImage = gcnew TrigerImageReadyHandler(OnTriggerImageReady);
			m_gchOnTriggerImageReady = GCHandle::Alloc(m_hOnTriggerImage);
			IntPtr ipOnTriggerImage = Marshal::GetFunctionPointerForDelegate(m_hOnTriggerImage);
			TriggerImageCallback pfnOnTriggerImageReady= static_cast<TriggerImageCallback>(ipOnTriggerImage.ToPointer());

			m_gcrThis = new gcroot<VideoDevice^>;
			*m_gcrThis = this;
			m_pVideoService = new VideoService(
				id,
				&param,
				NULL,
				pfnOnRealImageReady,
				NULL,
				ZW_PIX_FMT_NONE,
				pfnOnTriggerImageReady,
				NULL,
				ZW_PIX_FMT_NONE,
				m_gcrThis);
		}

		~VideoDevice()
		{
			delete m_pVideoService;
			m_gchOnRealImageReady.Free();
			m_gchOnTriggerImageReady.Free();
		}

		int  StartVideoSource(IntPtr hPlayWnd)
		{
			return m_pVideoService->StartVideoSource(hPlayWnd.ToPointer());
		}

		/*触发视频设备外设功能，返回0失败，1成功。*/
		/*如果设备不支持或者应用不需要，就不用实现这个接口*/
		int  TriggerPeripheral(int func)
		{
			return m_pVideoService->TriggerPeripheral(func);
		}

		void StopVideoSource()
		{
			m_pVideoService->StopVideoSource();
		}

	private:
		static void OnRealImageReady(void *pUserData, ZWImageData* img)
		{
			gcroot<VideoDevice^>* pVideoDevice =(gcroot<VideoDevice^>*)pUserData;
			VideoDevice^ videoDevice = *pVideoDevice;
			if (videoDevice->m_mhOnRealImageReady != nullptr)
			{
				ImageData image;
				image.Width = img->Width;
				image.Height = img->Height;
				image.PixFmt = img->PixFmt;
				image.Plane1 = IntPtr::IntPtr(img->PlaneData[0]);
				image.Plane2 = IntPtr::IntPtr(img->PlaneData[1]);
				image.Plane3 = IntPtr::IntPtr(img->PlaneData[2]);
				image.LineSize1 = img->LineSize[0];
				image.LineSize2 = img->LineSize[1];
				image.LineSize3 = img->LineSize[2];
				videoDevice->m_mhOnRealImageReady(image);
			}
		}

		static void OnTriggerImageReady(void *pUserData, ZWImageData* img, void* data, int func)
		{
			gcroot<VideoDevice^>* pVideoDevice =(gcroot<VideoDevice^>*)pUserData;
			VideoDevice^ videoDevice = *pVideoDevice;
			if (videoDevice->m_mhOnTriggerImageReady != nullptr)
			{
				ImageData image;
				image.Width = img->Width;
				image.Height = img->Height;
				image.PixFmt = img->PixFmt;
				image.Plane1 = IntPtr::IntPtr(img->PlaneData[0]);
				image.Plane2 = IntPtr::IntPtr(img->PlaneData[1]);
				image.Plane3 = IntPtr::IntPtr(img->PlaneData[2]);
				image.LineSize1 = img->LineSize[0];
				image.LineSize2 = img->LineSize[1];
				image.LineSize3 = img->LineSize[2];
				videoDevice->m_mhOnTriggerImageReady(image, IntPtr::IntPtr(data),func);
			}
		}
	};
}
}
