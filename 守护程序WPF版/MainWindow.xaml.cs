using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Net.Mail;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using Timer = System.Timers.Timer;
using System.ServiceProcess;
namespace DProtector
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public class AccountInfo
    {
        public string QQ;
        public string Password;
        public string Area;
        private DateTime _coolDown;
        public static Action Save;
        public DateTime CoolDown
        {
            get { return _coolDown; }
            set { _coolDown = value;
                if(Save!=null)Save();
            }
        }
    }
    public partial class MainWindow : Window
    {
        private string _autoLoginPath, _gamePath, _mainPathWithOutLogin;
        private DateTime _lastPluse;
        private TimeSpan _timeout = TimeSpan.FromSeconds(5*60);
        private int _restartFlag;
        private StreamWriter _logWriter;
        private AccountInfo[] AccountList;
        private AccountInfo _currentAccount;
        private string[] _macs;
        private bool _isRunning;
        private bool _usingFakeMAC;
        private string _netCardIndexString;
        private bool _pending;
        private readonly Timer timer = new Timer(10000);
        private readonly Encoding DefaultEncoding = Encoding.GetEncoding("GBK");
        private readonly string 秘密字符串 = Encoding.ASCII.GetString(new byte[]{100,110,102});
        public string OutputMsg
        {
            set { Dispatcher.Invoke(new Action(() => { Output.Content = value; })); }
        }
        public string LastRestartMsg
        {
            set { Dispatcher.Invoke(new Action(() => { LastRestart.Content = value; })); }
        }

        public bool IsRunning
        {
            get { return _isRunning; }
            set
            {
                _isRunning = value;
                OutputMsg = "当前帐号：" + _currentAccount.QQ+(_isRunning?"运行":_currentAccount.CoolDown>DateTime.Now?"刷完":"休息");
            }
        }

        private readonly IntPtr _consoleWindow;
        private readonly SmtpClient client;

        private  Process ChildProcess;
        private Process GameLoginProcess;
        public MainWindow()
        {
            InitializeComponent();
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
            
            AllocConsole();
            //InstallHook(0);
            _consoleWindow = GetConsoleWindow();
            Console.WriteLine(Title);
            var receiveDataThread = new Thread(ReceiveDataFromClient) { IsBackground = true };
            receiveDataThread.Start();
            try
            {
                ReadConfig();
                ReadAccountList();

                if (_usingFakeMAC)
                {
                    _macs = 配置虚假MAC地址();
                    选择一个可用帐号();
                }
                else
                {
                     读取网卡列表();
                }
                TryLaunchAutoLogin();
            }
            catch (Exception ex)
            {
                Log(ex.Message+ex.StackTrace);
                return;
            }
            
            
            
            timer.Elapsed += timer_Elapsed;
            Top = SystemParameters.PrimaryScreenHeight - 290;
            Left = SystemParameters.PrimaryScreenWidth - 350;
            Width = 350;
            Height = 250;
            timer.Start();
            MoveWindow(_consoleWindow, (int) (SystemParameters.PrimaryScreenWidth - 750),
                (int) (SystemParameters.WorkArea.Height - 250),400, 250, false);
            CB_Reason.ItemsSource = File.ReadAllLines("reason.txt");
            client = new SmtpClient
            {
                Credentials = new System.Net.NetworkCredential("dexter_1985@163.com", "rhy08917"),
                Host = "smtp.163.com",
                Port = 25
            };
            AccountInfo.Save = SaveCurrentAccountList;
        }

        private bool 选择一个可用帐号()
        {
            for (var i = 0; i < AccountList.Length; i++)
            {
                if (AccountList[i].CoolDown < DateTime.Now)
                {
                    Log("正在修改MAC地址");
                    MACHelper.ChangeMac(_netCardIndexString, _macs[i]);
                    _currentAccount = AccountList[i];
                    return true;
                }
            }
            return false;
        }

        void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
           Log(e.ExceptionObject.ToString());
        }

        private void CheckMinimized()
        {
            if (Application.Current.MainWindow.WindowState != WindowState.Minimized) return;
            Log("侦测到被最小化");
            Application.Current.MainWindow.WindowState = WindowState.Normal;
            ShowWindow(_consoleWindow, 1);
            Log("恢复窗口");
        }
        void timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine("定时器消息:"+DateTime.Now);
            Dispatcher.BeginInvoke(new Action(CheckMinimized));
            if (_isRunning)
            {
                //try
                //{
                //    foreach (
                //        var thread in
                //            ChildProcess.Threads.Cast<ProcessThread>()
                //                .Where(thread => thread.ThreadState == ThreadState.Wait || thread.ThreadState == ThreadState.Unknown))
                //    {
                //        Log("侦测到线程被挂起！恢复中");
                //        IntPtr pOpenThread = OpenThread(ThreadAccess.SUSPEND_RESUME, false, (uint) thread.Id);

                //        if (pOpenThread == IntPtr.Zero)
                //            continue;

                //        var suspendCount = 0;
                //        do
                //        {
                //            suspendCount = ResumeThread(pOpenThread);
                //        } while (suspendCount > 0);
                //        Log("已恢复线程！");
                //    }
                //}
                //catch (Exception pex)
                //{
                //    Console.ForegroundColor = ConsoleColor.DarkCyan;
                //    Console.WriteLine(pex);
                //}
                if (FindWindow(null, "<错误> 插件版本:5.1412")!=IntPtr.Zero)
                {
                    Log("侦测到错误窗口：<错误> 插件版本:5.1412");
                    LastRestartMsg = "上次重启原因：侦测到错误窗口：<错误> 插件版本:5.1412";
                    Restart(true);
                }
                else if (FindWindow(null, 秘密字符串+"2异常") != IntPtr.Zero)
                {
                    Log("侦测到错误窗口："+秘密字符串+"2异常");
                    LastRestartMsg = "上次重启原因：侦测到错误窗口 " + 秘密字符串 + "2异常";
                    Restart();
                }
                else if (FindWindow(null, 秘密字符串+"2­_no_login异常") != IntPtr.Zero)
                {
                    Log("侦测到错误窗口："+秘密字符串+"2­_no_login异常");
                    LastRestartMsg = "上次重启原因：侦测到错误窗口 "+秘密字符串+"2­_no_login异常";
                    Restart();
                }
                else if (DateTime.Now>_lastPluse+_timeout)
                {
                    if (_currentAccount.CoolDown < DateTime.Now)
                    {
                        Log("心跳超时!超时时间:{0},标志位:{1},上次心跳时间：{2}", _timeout.TotalSeconds, _restartFlag,
                            _lastPluse.ToString("MM-dd hh:mm:ss"));
                        LastRestartMsg = string.Format("上次重启原因:心跳超时!超时时间:{0},标志位:{1},上次心跳时间：{2}",
                            _timeout.TotalMilliseconds, _restartFlag, _lastPluse.ToShortTimeString());
                        Restart();
                    }
                    else
                    {
                        if (_usingFakeMAC)
                        {
                            if (选择一个可用帐号())
                            {
                                TryLaunchAutoLogin();
                            }
                            else
                            {
                                Log("心跳超时，没有可用帐号");
                            }
                        }else
                        
                        Log("心跳超时，但已刷完。");
                    }
                }
            }
            else
            {
                if (DateTime.Now <= _currentAccount.CoolDown) return;
                Log("进程不存在，但满足自动启动时间"+DateTime.Now.ToShortTimeString());
                if (TryLaunchAutoLogin())
                    LastRestartMsg = "上次重启原因：侦测到进程不存在！";
            }
        }

        private DateTime Next6()
        {
            var dateTime = DateTime.Now;
            if (dateTime.Hour >= 6)
                dateTime = dateTime.AddDays(1);
            return new DateTime(dateTime.Year,dateTime.Month,dateTime.Day,6,0,0);
        }

        private void Log(string format,params object[] args)
        {
            if (_logWriter == null) _logWriter = new StreamWriter((new FileStream("Log.txt", FileMode.Truncate)),DefaultEncoding){AutoFlush = true};
            var timeStr = "["+DateTime.Now.ToString("MM-dd hh:mm:ss")+"]";
            Console.ForegroundColor = ConsoleColor.DarkGreen;
            Console.Write(timeStr);
            _logWriter.Write(timeStr);
            Console.ForegroundColor = ConsoleColor.White;
            _logWriter.WriteLine(format, args);
            Console.WriteLine(format,args);
        }

        private void ReadConfig()
        {
            Log("读取配置文件");
            var configs = File.ReadAllLines("config.txt", DefaultEncoding);
            Log(string.Join("\r\n",configs));
            _gamePath = configs[0];
            _autoLoginPath = configs[1];
            _mainPathWithOutLogin = configs[2];
            if (configs[4][0] != '#')
            {
                RunCmd("rasdial "+configs[4]);
            }
            if (configs.Length>5 && configs[5][0] != '#')
            {
                _usingFakeMAC = true;
                _netCardIndexString = configs[5].PadLeft(4,'0');
            }
        }

        private void ReadAccountList()
        {
            Log("读取帐号列表");
            var accounts = File.ReadAllLines("Accounts.txt", DefaultEncoding);
            if (File.Exists("CoolDowns.txt"))
            {
                var cooldowns = File.ReadAllLines("CoolDowns.txt",DefaultEncoding);
                for (var i = 0; i < accounts.Length; i++)
                {
                    accounts[i] += " " + (cooldowns.Length > i ? (cooldowns[i] == "" ? "0" : cooldowns[i]) : "0");
                }
            }
            else
            {
                File.WriteAllLines("CoolDowns.txt",Enumerable.Repeat("0",accounts.Length),DefaultEncoding);
            }
            
            AccountList = accounts.Select(x =>x.Split(' ')).Select(args=>
                new AccountInfo { QQ = args[0], Password = args[1], Area = args[2], CoolDown = args.Length == 4 ? new DateTime(Convert.ToInt64(args[3])) : new DateTime(0) }
            ).ToArray();

        }

        private void LaunchGame()
        {
            Log("启动游戏");
            GameLoginProcess = Process.Start(_gamePath);
//            File.WriteAllText("startGame.bat", @"start """" """ + _gamePath + @"""
//", DefaultEncoding);
//           RunCmd("startGame.bat", false);
//            Process.Start("startGame.bat");
        }

        private void LaunchAutoLogin()
        {
            _pending = true;
            Log("启动自动登录，先执行清理");
            RunCmd("新重启前清理一下.bat");
            if (_currentAccount == null) _currentAccount = AccountList[0];
            LaunchGame();
            //var serviceController = new ServiceController("ServiceTest");
            //var serviceInstaller = new ServiceInstaller();
            //serviceInstaller.Install();
            //serviceController.Start();
            //ChildProcess = Process.Start(_autoLoginPath, string.Format("{0} {1} {2}", _currentAccount.QQ, _currentAccount.Password, _currentAccount.Area));
           // Log("设置保护进程ID");
            File.WriteAllLines("tempBat.bat",new []{ string.Format(@"start """" ""{0}"" {1} {2} {3}", _autoLoginPath, _currentAccount.QQ, _currentAccount.Password, _currentAccount.Area),"sc start rootkit"}, DefaultEncoding);
            Process.Start("tempBat.bat");
            _isRunning = true;
            _lastPluse = DateTime.Now;
            _pending = false;
        }

        private void RestartWithoutLogin()
        {
            Log("非登录重启");
            _pending = true;
            RunCmd("taskkill /F /IM "+秘密字符串+"2* /T");
           //ChildProcess= Process.Start(_mainPathWithOutLogin, string.Format("{0} {1} {2}", _currentAccount.QQ, _currentAccount.Password, _currentAccount.Area));
            File.WriteAllLines("tempBat.bat", new[] { "sc stop rootkit", string.Format(@"start """" ""{0}"" {1} {2} {3}", _mainPathWithOutLogin, _currentAccount.QQ, _currentAccount.Password, _currentAccount.Area), "sc start rootkit" }, DefaultEncoding);
            Process.Start("tempBat.bat");
           // RunCmd("tempBat.bat", false);
            _isRunning = true;
            _pending = false;
            _lastPluse = DateTime.Now;
            
        }

        private void KillAutoLogin()
        {
            Log("关闭所有程序");
            try
            {
                RunCmd("sc stop rootkit");
                RunCmd("taskkill /F /IM " + 秘密字符串 + "* /T");
                if (GameLoginProcess!=null && !GameLoginProcess.HasExited)GameLoginProcess.Kill();
                _isRunning = false;
                ChildProcess = null;
            }
            catch (Exception ex)
            {
                Log(ex.Message);
            }
        }

        public void Restart(bool force = false)
        {
            if (_restartFlag != 2 || force)
            {
                KillAutoLogin();
                LaunchAutoLogin();
            }
            else
            {
                RestartWithoutLogin();
            }
        }

        public bool TryLaunchAutoLogin()
        {
            if (_isRunning)
            {
                KillAutoLogin();
            }

            if (_currentAccount.CoolDown > DateTime.Now)
            {
                Log(_currentAccount.CoolDown == DateTime.MaxValue ? "该帐号被手动关闭，需要手动启动，自动启动无效" : ("该帐号已经刷完。下次可用时间：" + _currentAccount.CoolDown));
                return false;
            }
            Log("尝试重新启动");
            LaunchAutoLogin();

            return true;
        }
        
        private void ReceiveDataFromClient()
        {
            var _pipeServer = new NamedPipeServerStream(秘密字符串 + "2", PipeDirection.InOut, 2);
            var sr = new StreamReader(_pipeServer);
            while (true)
            {
                try
                {
                    _pipeServer.WaitForConnection(); //Waiting  
                    Log("收到管道连接！");
                    while (_pipeServer.CanRead)
                    {
                        var data = sr.ReadLine();
                        if (data == null) break;
                        var recData = data.Split(' ');
                        switch (recData[0])
                        {
                            case "pluse":
                                _timeout = TimeSpan.FromSeconds(Convert.ToInt32(recData[1]));
                                _restartFlag = Convert.ToInt32(recData[2]);
                                Log("收到心跳包{0} {1} {2}",_timeout,_restartFlag,(DateTime.Now-_lastPluse).TotalSeconds);
                                _lastPluse = DateTime.Now;
                                break;
                            case "restartWithoutLogin":
                                LastRestartMsg = "上次重启原因：收到非登录重启的命令";
                                RestartWithoutLogin();
                                break;
                            case "restart":
                                Log("收到强制重启命令");
                                LastRestartMsg = "上次重启原因：收到强制重启命令";
                                Restart(true);
                                break;
                            case "complete":
                                Log("帐号刷完");
                                _currentAccount.CoolDown = Next6();
                                if (_usingFakeMAC)
                                {
                                    if (选择一个可用帐号()) TryLaunchAutoLogin();
                                }
                                break;
                            default:
                                Log("未实现命令：{0}", string.Join(" ",recData));
                                break;
                        }
                    }
                    Thread.Sleep(1000);
                    _pipeServer.Disconnect();
                   
                    Log("管道连接断开！");
                }
                catch (Exception ex)
                {
                    Log(ex.Message);
                }
                Thread.Sleep(1000);
            }
        }
        private void SaveCurrentAccountList()
        {
            Log("设置帐号CoolDown：",string.Join(Environment.NewLine,AccountList.Select(x=>x.CoolDown.ToString())));
            File.WriteAllLines("CoolDowns.txt", AccountList.Select(x => x.CoolDown.Ticks.ToString()), DefaultEncoding);
        }

        private string[] 配置虚假MAC地址()
        {
            var macs = new List<string>();
            if (File.Exists("Mac.txt"))
            {
                macs.AddRange(File.ReadAllLines("Mac.txt"));
                if (macs.Count < AccountList.Length)
                {
                    for (var i = macs.Count; i < AccountList.Length; i++)
                    {
                        macs.Add(MACHelper.CreateNewMacAddress());
                    }
                    File.WriteAllLines("Mac.txt", macs);   
                }
                
            }
            else
            {
                for (var i = 0; i < AccountList.Length; i++)
                {
                    macs.Add(MACHelper.CreateNewMacAddress());
                }
                File.WriteAllLines("Mac.txt", macs);   
            }
            return macs.ToArray();
        }
        private void 读取网卡列表()
        {
            Log("读取网卡列表");
            var all = ReadFromPipe("ipconfig /all");
            var netCards = new List<string>();
            if (File.Exists("netcard.txt"))
            {
                netCards.AddRange(File.ReadAllLines("netcard.txt",DefaultEncoding));
            }
            var pos = 0;
            var bytesRead=0;
            var substr = strstr(all,"以太网适配器");
            while (substr != null)
            {
                var substr2 = strstr(substr, "物理地址");
                substr2 = strstr(substr2, " : ");
                var connected = strstr(substr, "媒体已断开") == null;
                var address = substr2.Substring(3, 17);
                if (!netCards.Contains(address))
                {
                    netCards.Add(address);
                }
                
                if (_currentAccount == null)
                {
                    _currentAccount = AccountList[netCards.IndexOf(address)];
                    OutputMsg = "当前帐号：" + _currentAccount.QQ;
                }
                substr = strstr(substr2, "以太网适配器");
                break;
            }
            File.WriteAllLines("netcard.txt", netCards,DefaultEncoding);
        }

        private string strstr(string src,string sub)
        {
            return src.IndexOf(sub) == -1 ? null : src.Substring(src.IndexOf(sub));
        }
        private static void RunCmd(string command,bool wait = true)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
           Console.WriteLine(command);
           //實例一個Process類，啟動一個獨立進程
           var p = new Process
           {
               StartInfo =
               {
                   FileName = "cmd.exe",
                   Arguments = "/c " + command,
                   UseShellExecute = true,
                   //RedirectStandardInput = true,
                   //RedirectStandardOutput = true,
                   //RedirectStandardError = true,
                   CreateNoWindow = false
               }
           };
           //Process類有一個StartInfo屬性，這個是ProcessStartInfo類，包括了一些屬性和方法，下面我們用到了他的幾個屬性：

            p.Start();   //啟動
            if(wait)p.WaitForExit(int.MaxValue);
           //p.StandardInput.WriteLine(command);       //也可以用這種方式輸入要執行的命令
           //p.StandardInput.WriteLine("exit");        //不過要記得加上Exit要不然下一行程式執行的時候會當機
          
       }

        private static string ReadFromPipe(string fileName,string arguments)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            Console.WriteLine(fileName+" " + arguments);
            var p = new Process
            {
                StartInfo =
                {
                    FileName = fileName,
                    Arguments = arguments,
                    UseShellExecute = false,
                    RedirectStandardInput = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    CreateNoWindow = true
                }
            };
            p.Start();   //啟動
            return p.StandardOutput.ReadToEnd();
        }
        private static string ReadFromPipe(string command)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            Console.WriteLine(command);
            var p = new Process
            {
                StartInfo =
                {
                    FileName = "cmd.exe",
                    Arguments = "/c " + command,
                    UseShellExecute = false,
                    RedirectStandardInput = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    CreateNoWindow = true
                }
            };
            p.Start();   //啟動
            return p.StandardOutput.ReadToEnd();
            //p.StandardInput.WriteLine(command);       //也可以用這種方式輸入要執行的命令
            //p.StandardInput.WriteLine("exit");        
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            KillAutoLogin();
        }

        private void StartBn_Click(object sender, RoutedEventArgs e)
        {
            _currentAccount.CoolDown = DateTime.MinValue;
            if (TryLaunchAutoLogin())
            {
                LastRestartMsg = "上次重启原因：菜单命令";
            }
        }

        private void LogBn_Click(object sender, RoutedEventArgs e)
        {
            var msg = new MailMessage
            {
                From = new MailAddress("dexter_1985@163.com", "dexter", Encoding.UTF8),
                Subject = "守护程序Log",
                SubjectEncoding = Encoding.UTF8,
                Body = File.ReadAllText("Log.txt"),
                BodyEncoding = Encoding.UTF8,
                IsBodyHtml = false,
                Priority = MailPriority.High
            };
            msg.Attachments.Add(new Attachment("login.log"));
            msg.To.Add("178529795@qq.com");
            client.Send(msg);
            MessageBox.Show("发送成功！", "提示");
        }

        private void SendGLog_Click(object sender, RoutedEventArgs e)
        {
            if(_isRunning)KillAutoLogin();
            var msg = new MailMessage
            {
                From = new MailAddress("dexter_1985@163.com", "dexter", Encoding.UTF8),
                Subject = "GLog",
                SubjectEncoding = Encoding.UTF8,
                Body = CB_Reason.Text,
                BodyEncoding = Encoding.UTF8,
                IsBodyHtml = false,
                Priority = MailPriority.High
            };
            msg.Attachments.Add(new Attachment("g_log.log"));
            msg.To.Add("2221249130@qq.com");
            client.Send(msg);
            MessageBox.Show("发送成功！", "提示");
            //client.SendAsync(msg, null);
        }

        private void MainWindow_OnClosed(object sender, EventArgs e)
        {
            KillAutoLogin();
        }

        private void OpenLogFile_OnClick(object sender, RoutedEventArgs e)
        {
            Process.Start("Log.txt");
        }

        private void OpenConfigFile_OnClick(object sender, RoutedEventArgs e)
        {
            Process.Start("config.txt");
        }

        private void OpenMACFile_OnClick(object sender, RoutedEventArgs e)
        {
            Process.Start("netcard.txt");
        }

        private void CloseAndSetCoolDown_OnClick(object sender, RoutedEventArgs e)
        {
            _currentAccount.CoolDown = DateTime.MaxValue;
            KillAutoLogin();
        }
    }
}
