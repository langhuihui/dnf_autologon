using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace 内部答题器
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    
    public class SocketInfo :INotifyPropertyChanged
    {
        
        public SocketInfo()
        {
            RemainTime = new TimeSpan(0,0,0,0);
        }
        public event PropertyChangedEventHandler PropertyChanged;
        public TimeSpan RemainTime { get; set; }
        public NetworkStream NetworkStream { get; set; }
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }
        public string Display {
            get { return RemainTime.ToString("ss"); }
        }
        public void Tick()
        {
            RemainTime = RemainTime.Add(TimeSpan.FromSeconds(1));
            OnPropertyChanged("Display");
        }
    }
    public partial class MainWindow : Window
    {
        public const int START = 1;
        public const int INPUT = 2;
        public const int PAUSE = 3;
        public const int CONFIRM = 4;
        public TcpListener TcpListener = new TcpListener(IPAddress.Any,2014);
        public readonly ObservableCollection<SocketInfo> SocketDataCollection = new ObservableCollection<SocketInfo>();
        public const int BufferSize = 1024*500;
        public byte[] buffer = new byte[BufferSize];
        public WriteableBitmap BitmapSource;
        public Int32Rect BitmapRect;
        public int RawStride;
        public Timer timer = new Timer(1000);
        public static byte[] WhiteBytes;
        public MainWindow()
        {
            InitializeComponent();
            var pf = PixelFormats.BlackWhite;
            RawStride = ((int)TheImage.Width * pf.BitsPerPixel + 7) / 8;
            BitmapSource = new WriteableBitmap((int)TheImage.Width,(int)TheImage.Height,70,70,pf, null);
            BitmapRect = new Int32Rect(0, 0, BitmapSource.PixelWidth, BitmapSource.PixelHeight);
            TheImage.Source = BitmapSource;
            SocketList.ItemsSource = SocketDataCollection;
            TcpListener.Start();
            timer.Elapsed += timer_Elapsed;
            WhiteBytes = Enumerable.Repeat((byte)0xFF, BitmapSource.PixelWidth*BitmapSource.PixelHeight).ToArray();
            new Action(() =>
            {
                while (true)
                {
                    var client = TcpListener.AcceptTcpClient();
                    var stream = client.GetStream();
                    Dispatcher.Invoke(new Action(() =>
                    {
                        SocketDataCollection.Add(new SocketInfo { NetworkStream = stream });
                        if (SocketList.SelectedIndex == -1)
                        {
                            SocketList.SelectedIndex = 0;
                        }
                    }));
                    stream.BeginRead(buffer, 0, BufferSize, ReadComplete, stream);
                }
            }).BeginInvoke(null, null);
            timer.Start();
        }

        void timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            foreach (var socketInfo in SocketDataCollection)
            {
                socketInfo.Tick();
            }

            //foreach (var socketInfo in SocketDataCollection.Where(x => x.RemainTime == TimeSpan.Zero).ToList())
            //{
            //    socketInfo.NetworkStream.Close();
            //    socketInfo.NetworkStream = null;
            //    Dispatcher.Invoke(new Action<SocketInfo>(info => SocketDataCollection.Remove(info)), socketInfo);
            //}
        }

        private void ReadComplete(IAsyncResult ar)
        {
            var streamToClient = ar.AsyncState as NetworkStream;
            var bytesRead = 0;
            try
            {
                lock (streamToClient)
                {
                    //读取数据
                    bytesRead = streamToClient.EndRead(ar);
                    if (bytesRead <= 0)
                    {
                        Dispatcher.Invoke(new Action(() => SocketDataCollection.Remove(SocketDataCollection.Single(x=>x.NetworkStream==streamToClient))));
                        return;
                    }
                    switch (buffer[0])
                    {
                        case 1:
                            break;
                        case 2:
                           Dispatcher.Invoke(new Action(() => BitmapSource.WritePixels(BitmapRect, buffer, RawStride,1)));
                            break;
                    }
                }
                streamToClient.BeginRead(buffer, 0, BufferSize, ReadComplete, streamToClient);
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(new Action(() =>
                {
                   var socketInfo =  SocketDataCollection.SingleOrDefault(x => x.NetworkStream == streamToClient);
                    if (socketInfo != null)
                    {
                        socketInfo.NetworkStream = null;
                        SocketDataCollection.Remove(socketInfo);
                    }
                }));
            }
        }
        private void SocketList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.RemovedItems.Count > 0 && (e.RemovedItems[0] as SocketInfo).NetworkStream!=null)
            {
                (e.RemovedItems[0] as SocketInfo).NetworkStream.Write(new byte[] { PAUSE ,0}, 0, 2);
            }
            //Dispatcher.Invoke(new Action(() => BitmapSource.WritePixels(BitmapRect, buffer, RawStride, 1)));
            BitmapSource.WritePixels(BitmapRect, WhiteBytes, RawStride, 0);
            
            var socketInfo = SocketList.SelectedItem as SocketInfo;
            if (socketInfo != null)
                socketInfo.NetworkStream.Write(new byte[] { START ,0}, 0, 2);
            InputControl.Text = "";
        }

        private void TextBox_KeyDown(object sender, KeyEventArgs e)
        {
            var socketInfo = SocketList.SelectedItem as SocketInfo;
            if (socketInfo == null) return;
            var stream = socketInfo.NetworkStream;
            var bytes = new byte[2];
            switch (e.Key)
            {
                case Key.Return:
                    bytes[0] = 4;
                    bytes[1] = 0;
                    InputControl.Text = "";
                    break;
                case Key.Back:
                    break;
                default:
                    bytes[0] = INPUT;
                    bytes[1] = (byte)e.Key;
                    break;
            }
            stream.Write(bytes, 0, 2);
        }
    }
}
