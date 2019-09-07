using System.Runtime.InteropServices;
using System.Windows;

namespace DProtector
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        [DllImport("kernel32.dll")]
        public static extern bool FreeConsole();
        private void Application_Exit(object sender, ExitEventArgs e)
        {
            FreeConsole();
        }
    }
}
