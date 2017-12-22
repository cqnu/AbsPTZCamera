using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using AbsDevice.NET;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

using SDL2;
using static SDL2.SDL;

namespace AbsDevice.NET.Demo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        IntPtr win = IntPtr.Zero;
        IntPtr renderer = IntPtr.Zero;

        private void Form2_Load(object sender, EventArgs e)
        {
            SDL_Init(SDL.SDL_INIT_VIDEO);
            win = SDL.SDL_CreateWindowFrom(panel1.Handle);
            renderer = SDL.SDL_CreateRenderer(win, -1, SDL_RendererFlags.SDL_RENDERER_ACCELERATED);
        }

        protected override void OnClosed(EventArgs e)
        {
            if (renderer != IntPtr.Zero)
            {
                SDL.SDL_DestroyRenderer(renderer);
                if (win != IntPtr.Zero)
                    SDL.SDL_DestroyWindow(win);
            }

            SDL.SDL_Quit();
            base.OnClosed(e);
        }

        private VideoDevice _videoDevice = null;
        private IntPtr _zwVideo = IntPtr.Zero;
        GCHandle gchOnRealImgReady;
        RealTimeImageCallback onRealImgReady;
        private void button1_Click(object sender, EventArgs e)
        {
            _videoDevice = new VideoDevice(0, null, null, 0, string.Empty, string.Empty, 0, @"E:\杂项\宝宝聊天照片\VID_20150715_153556.mp4", null, null);
            _videoDevice.StartVideoSource(panel1.Handle);
            //var param = new ZWVideoParam();
            //param.CameraIp = "192.168.100.19";
            //param.LocalIp = null;
            //param.CameraPort = 8000;
            //param.FuncMask = 15;
            //param.AddressNum = 35;
            //param.UserName = "admin";
            //param.Password = "12345";
            //param.UrlOrAddressName = @"E:\杂项\宝宝聊天照片\VID_20150715_153556.mp4";
            //onRealImgReady = new RealTimeImageCallback(OnRealImgReadyProc);
            //gchOnRealImgReady = GCHandle.Alloc(onRealImgReady);
            //_zwVideo = ZWVideoAPI.InitialVideoSource(ref param, null, onRealImgReady, null,ZW_PIX_FMT.ZW_PIX_FMT_NONE,null, null, ZW_PIX_FMT.ZW_PIX_FMT_NONE, IntPtr.Zero);
            //ZWVideoAPI.StartVideoSource(_zwVideo, panel1.Handle);
            //ZW_CODEC_ID codec = ZWVideoAPI.GetVideoCodec(_zwVideo);
        }

        private void OnRealImgReadyProc(IntPtr cbData, ref ZWImageData img)
        {
            Console.WriteLine(img.Height);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            _videoDevice.StopVideoSource();
            _videoDevice.Dispose();

            //ZWVideoAPI.StopVideoSource(_zwVideo);
            //ZWVideoAPI.FinializeVideoSource(_zwVideo);
            //gchOnRealImgReady.Free();
        }

        public void DrawRgb(IntPtr buff, int width,int storewidth, int height)
        {
            //var bitmap = new Bitmap(width, height, storewidth, PixelFormat.Format24bppRgb, buff);
            //this.Invoke(new MethodInvoker(()=>
            //    {
            //        DrawBitmap(bitmap);
            //    }));

            SDL_Rect dst = new SDL_Rect();
            dst.x = 0;
            dst.y = 0;
            dst.w = panel1.Width;
            dst.h = panel1.Height;

            IntPtr pic = SDL.SDL_CreateRGBSurfaceFrom(buff, width, height, 24, storewidth, 0x00ff0000, 0x0000ff00, 0x000000ff, 0);
            IntPtr tex = SDL.SDL_CreateTextureFromSurface(renderer, pic);
            SDL.SDL_RenderClear(renderer);
            SDL.SDL_RenderCopy(renderer, tex, IntPtr.Zero, ref dst);
            SDL.SDL_RenderPresent(renderer);

            SDL_FreeSurface(pic);
            SDL_DestroyTexture(tex);
        }

        public void DrawBitmap(Bitmap bitmap)
        {
            using(var graphics = Graphics.FromHwnd(panel1.Handle))
            {
                graphics.DrawImage(bitmap,panel1.ClientRectangle,0,0,bitmap.Width,bitmap.Height,GraphicsUnit.Pixel);
            }
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

            //IntPtr bitmapTex = IntPtr.Zero;
            //IntPtr bitmapSurface = IntPtr.Zero;

            //SDL_Rect dst = new SDL_Rect();
            //dst.x = 0;
            //dst.y = 0;
            //dst.w = panel1.Width;
            //dst.h = panel1.Height;

            //bitmapSurface = SDL.SDL_LoadBMP(@"D:\b3fb43166d224f4a7f85952e0ef790529822d1eb.bmp");
            //bitmapTex = SDL.SDL_CreateTextureFromSurface(renderer, bitmapSurface);
            //SDL.SDL_RenderClear(renderer);
            //SDL.SDL_RenderCopy(renderer, bitmapTex, IntPtr.Zero, ref dst);
            //SDL.SDL_RenderPresent(renderer);
            //SDL.SDL_DestroyTexture(bitmapTex);
            //SDL.SDL_FreeSurface(bitmapSurface);
        }
    }
}
