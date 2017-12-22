using System;
using System.Runtime.InteropServices;

namespace AbsDevice.NET.Demo
{
    public enum ZW_COLOR_TYPE : int
    {
        ZW_COLOR_UNKNOWN = 0,     //未知
        ZW_COLOR_BLUE = 1,    //蓝色
        ZW_COLOR_YELLOW = 2,    //黄色
        ZW_COLOR_WHITE = 3,    //白色
        ZW_COLOR_BLACK = 4,    //黑色
        ZW_COLOR_GREEN = 5,    //绿色


        ZW_COLOR_SILVER = 20,    //银色
        ZW_COLOR_GRAY = 21,    //灰色
        ZW_COLOR_RED = 22,    //红色
        ZW_COLOR_BROWN = 23,    //棕色
        ZW_COLOR_PINK = 24,    //粉色
        ZW_COLOR_PURPLE = 25,    //紫色
        ZW_COLOR_CYAN = 26,    //青色
        ZW_COLOR_DARKBLUE = 27,    //深蓝
        ZW_COLOR_DARKGRAY = 28,    //深灰
    }

    public enum ZW_VEHICLE_SIZE : int
    {
        ZW_VEHICLE_UNKNOWN = 0,
        ZW_VEHICLE_SMALL = 1,
        ZW_VEHICLE_MEDIUM = 2,
        ZW_VEHICLE_LARGE = 3,
    }

    public enum ZW_CODEC_ID : int
    {
        ZW_CODEC_ID_NONE = 0,
        ZW_CODEC_ID_H264 = 1,
        ZW_CODEC_ID_MPEG4 = 2,
        ZW_CODEC_ID_MJPEG = 3,
    }

    /// <summary>
    /// 目前停车管理软件只支持输出BGR24和JPG格式的像素数据
    /// </summary>
    public enum ZW_PIX_FMT : int
    {
        ZW_PIX_FMT_NONE = 0,
        /*  planar format */

        ZW_PIX_FMT_YUV420P = 1,
        ZW_PIX_FMT_YV12 = 2,
        ZW_PIX_FMT_YUV422P = 3,
        ZW_PIX_FMT_YV16 = 4,
        ZW_PIX_FMT_YUV444P = 5,
        ZW_PIX_FMT_YVU444P = 6,

        /*  packed format */

        ZW_PIX_FMT_RGB24 = 51,
        ZW_PIX_FMT_BGR24 = 52,

        ZW_PIX_FMT_NV12 = 53,
        ZW_PIX_FMT_NV21 = 54,

        /*以下格式不支持作为声明输出——即不支持作为InitialVideoSource的realFixFmt、triggerFixFmt参数*/


        ZW_PIX_FMT_NV16 = 55,
        ZW_PIX_FMT_NV61 = 56,

        ZW_PIX_FMT_YUYV422 = 57,
        ZW_PIX_FMT_YVYU422 = 58,

        ZW_PIX_FMT_UYVY422 = 59,
        ZW_PIX_FMT_VYUY422 = 60,

        /*  file format */

        ZW_PIX_FMT_JPEG = 100,
        ZW_PIX_FMT_BMP = 101,
    }


    [Flags]
    public enum ZW_PF : int
    {
        ZW_PF_NONE = 0,   /*  用于实时RGB图像输出 */
        ZW_PF_TAKE_PICTURE = 1, /*  抓拍图片，成功后可以在OnTriggerRGBReady方法中获取。 */
        ZW_PF_OPEN_GATE = 2, /*  开闸机。 */
        ZW_PF_CLOSE_GATE = 4,  /*  关闸机。 */
        ZW_PF_VEHICLE_IDENTIFY = 8,  /*  识别到车辆。此时data为 VehicleInfo*/
        ZW_PF_ENABLE_GATE = 16, /*  启用闸机控制。启用后闸机可以接收开关闸命令 */
        ZW_PF_DISABLE_GATE = 32, /*  停用闸机控制，停用后闸机不可以接收开关闸命令并保持停用前的状态 */
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct ZWVideoParam
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string CameraIp;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string LocalIp;

        [MarshalAs(UnmanagedType.U2)]
        public ushort CameraPort;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string UserName;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string Password;

        [MarshalAs(UnmanagedType.I4)]
        public int AddressNum;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string UrlOrAddressName;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct ZWRectF
    {
        [MarshalAs(UnmanagedType.R4)]
        public float X;

        [MarshalAs(UnmanagedType.R4)]
        public float Y;

        [MarshalAs(UnmanagedType.R4)]
        public float Width;

        [MarshalAs(UnmanagedType.R4)]
        public float Height;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct ZWVehicleInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string License;

        [MarshalAs(UnmanagedType.Struct)]
        public ZWRectF PlateRect;

        [MarshalAs(UnmanagedType.I4)]
        public ZW_COLOR_TYPE PlateColor;

        [MarshalAs(UnmanagedType.I4)]
        public ZW_COLOR_TYPE VehicleColor;

        [MarshalAs(UnmanagedType.I4)]
        public ZW_VEHICLE_SIZE VehicleSize;
    }


    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ZWImageData
    {

        [MarshalAs(UnmanagedType.I4)]
        public int Width;

        [MarshalAs(UnmanagedType.I4)]
        public int Height;

        [MarshalAs(UnmanagedType.I4)]
        public ZW_PIX_FMT PixFmt;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public IntPtr[] PlaneData;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public int[] LineSize;
    }


    [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Ansi)]
    public delegate void VideoStreamCallback(IntPtr cbData, IntPtr buff, long size, int frametype);

    [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Ansi)]
    public delegate void TriggerImageCallback(IntPtr cbData, ref ZWImageData img, IntPtr data, ZW_PF func);

    [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Ansi)]
    public delegate void RealTimeImageCallback(IntPtr cbData, ref ZWImageData img);

    public static class ZWVideoAPI
    {
        private const string nativeLibName = "AbsDevice.dll";


        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr InitialVideoSource(
            ref ZWVideoParam param,
            VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
            RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
            RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
            ZW_PIX_FMT realFixFmt,/*实时转码图像的格式*/
            TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
            TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
            ZW_PIX_FMT triggerFixFmt,/*外部触发抓拍转码图片的格式*/
            IntPtr cbData);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern ZW_CODEC_ID GetVideoCodec(IntPtr hVS);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int CanReConnect(IntPtr hVS);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int StartVideoSource(IntPtr hVS, IntPtr hDisplayWnd);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int TriggerPeripheral(IntPtr hVS, ZW_PF func);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int StopVideoSource(IntPtr hVS);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int FinializeVideoSource(IntPtr hVS);
    }
}
