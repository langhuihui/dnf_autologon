using System;   
using System.Collections.Generic;   
using System.Linq;   
using System.Text;   
using Microsoft.Win32;   
using System.Net.NetworkInformation;   
using System.Management;   
using System.Threading;   
using System.Runtime.InteropServices;   
using NETCONLib;
using Shell32;

namespace Test
{
    public class MACHelper   
    {   
        [DllImport("wininet.dll")]   
        private extern static bool InternetGetConnectedState(int Description, int ReservedValue);   
        /// <summary>   
        /// 是否能连接上Internet   
        /// </summary>   
        /// <returns></returns>   
        public bool IsConnectedToInternet()   
        {   
            int Desc = 0;   
            return InternetGetConnectedState(Desc, 0);   
        }

        public static string ConnectionName = "本地连接";
        /// <summary>   
        /// 获取MAC地址   
        /// </summary>   
        public string GetMACAddress()   
        {   
            //得到 MAC的注册表键   
            var macRegistry = Registry.LocalMachine.OpenSubKey("SYSTEM").OpenSubKey("CurrentControlSet").OpenSubKey("Control")   
                .OpenSubKey("Class").OpenSubKey("{4D36E972-E325-11CE-BFC1-08002bE10318}");   
            IList<string> list = macRegistry.GetSubKeyNames().ToList();   
            IPGlobalProperties computerProperties = IPGlobalProperties.GetIPGlobalProperties();
            NetworkInterface[] nics = NetworkInterface.GetAllNetworkInterfaces();
            foreach (var networkInterface in nics)
            {
                Console.WriteLine(networkInterface.Name);
            }
            var choice = Console.ReadLine();
            return nics[Convert.ToInt32(choice)].GetPhysicalAddress().ToString();
            var adapter = nics.FirstOrDefault(o => o.Name.Contains(ConnectionName));
            return adapter==null?null:adapter.GetPhysicalAddress().ToString();
        }   
        /// <summary>   
        /// 设置MAC地址   
        /// </summary>   
        /// <param name="newMac"></param>   
        public void SetMACAddress(string index,string newMac)   
        {   
            //string macAddress;   
            //string index = GetAdapterIndex(out macAddress);
            //Console.WriteLine(macAddress);
            //Console.ReadLine();
            //if (index == null)   
            //    return;   
            //得到 MAC的注册表键   
            RegistryKey macRegistry = Registry.LocalMachine.OpenSubKey("SYSTEM").OpenSubKey("CurrentControlSet").OpenSubKey("Control")   
                .OpenSubKey("Class").OpenSubKey("{4D36E972-E325-11CE-BFC1-08002bE10318}").OpenSubKey(index.PadLeft(4,'0'), true);   
            if (string.IsNullOrEmpty(newMac))   
            {   
                macRegistry.DeleteValue("NetworkAddress");   
            }   
            else  
            {   
                macRegistry.SetValue("NetworkAddress", newMac);   
                macRegistry.OpenSubKey("Ndi", true).OpenSubKey("params", true).OpenSubKey("NetworkAddress", true).SetValue("Default", newMac);   
                macRegistry.OpenSubKey("Ndi", true).OpenSubKey("params", true).OpenSubKey("NetworkAddress", true).SetValue("ParamDesc", "Network Address");   
            }
            //ReConnect();
            //Thread oThread = new Thread(new ThreadStart(ReConnect));//new Thread to ReConnect   
            //oThread.Start();   
        }   
        
        /// <summary>   
        /// 重新连接   
        /// </summary>   
        public void ReConnect()   
        {   
            
            var netSharingMgr = new NetSharingManagerClass();   
            foreach (var connection in from INetConnection connection in netSharingMgr.EnumEveryConnection let connProps = netSharingMgr.NetConnectionProps[connection] where connProps.MediaType == tagNETCON_MEDIATYPE.NCM_LAN select connection)
            {
                connection.Disconnect(); //禁用网络   
                connection.Connect();    //启用网络   
            }   
        }   
        /// <summary>   
        /// 生成随机MAC地址   
        /// </summary>   
        /// <returns></returns>   
        public string CreateNewMacAddress()   
        {   
            //return "0016D3B5C493";   
            int min = 0;   
            int max = 16;   
            Random ro = new Random();   
            var sn = string.Format("{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}{11}",   
               ro.Next(min, max).ToString("x"),//0   
               ro.Next(min, max).ToString("x"),//   
               ro.Next(min, max).ToString("x"),   
               ro.Next(min, max).ToString("x"),   
               ro.Next(min, max).ToString("x"),   
               ro.Next(min, max).ToString("x"),//5   
               ro.Next(min, max).ToString("x"),   
               ro.Next(min, max).ToString("x"),   
               ro.Next(min, max).ToString("x"),   
               ro.Next(min, max).ToString("x"),   
               ro.Next(min, max).ToString("x"),//10   
               ro.Next(min, max).ToString("x")   
                ).ToUpper();   
            return sn;   
        }   
        /// <summary>   
        /// 得到Mac地址及注册表对应Index   
        /// </summary>   
        /// <param name="macAddress"></param>   
        /// <returns></returns>   
        public string GetAdapterIndex(out string macAddress)   
        {   
            ManagementClass oMClass = new ManagementClass("Win32_NetworkAdapterConfiguration");   
            ManagementObjectCollection colMObj = oMClass.GetInstances();   
            macAddress = string.Empty;   
            int indexString = 0;   
            foreach (ManagementObject objMO in colMObj)   
            {   
                if (objMO["MacAddress"] != null && (bool)objMO["IPEnabled"] == true)   
                {   
                    macAddress = objMO["MacAddress"].ToString().Replace(":", "");
                    Console.WriteLine(macAddress);
                    //break;   
                }
                indexString++;   
            }   
            if (macAddress == string.Empty)   
                return null;   
            else  
                return indexString.ToString().PadLeft(4, '0');   
        }  
        #region Temp   
        public void noting()   
        {   
            //ManagementClass oMClass = new ManagementClass("Win32_NetworkAdapterConfiguration");   
            ManagementClass oMClass = new ManagementClass("Win32_NetworkAdapter");   
            ManagementObjectCollection colMObj = oMClass.GetInstances();   
            foreach (ManagementObject objMO in colMObj)   
            {   
                if (objMO["MacAddress"] != null)   
                {   
                    if (objMO["Name"] != null)   
                    {   
                        //objMO.InvokeMethod("Reset", null);   
                        objMO.InvokeMethod("Disable", null);//Vista only   
                        objMO.InvokeMethod("Enable", null);//Vista only   
                    }   
                    //if ((bool)objMO["IPEnabled"] == true)   
                    //{   
                    //    //Console.WriteLine(objMO["MacAddress"].ToString());   
                    //    //objMO.SetPropertyValue("MacAddress", CreateNewMacAddress());   
                    //    //objMO["MacAddress"] = CreateNewMacAddress();   
                    //    //objMO.InvokeMethod("Disable", null);   
                    //    //objMO.InvokeMethod("Enable", null);   
                    //    //objMO.Path.ReleaseDHCPLease();   
                    //    var iObj = objMO.GetMethodParameters("EnableDHCP");   
                    //    var oObj = objMO.InvokeMethod("ReleaseDHCPLease", null, null);   
                    //    Thread.Sleep(100);   
                    //    objMO.InvokeMethod("RenewDHCPLease", null, null);   
                    //}   
                }   
            }   
        }   
        public void no()   
        {   
            Folder networkConnectionsFolder = GetNetworkConnectionsFolder();   
            if (networkConnectionsFolder == null)   
            {   
                Console.WriteLine("Network connections folder not found.");   
                return;   
            }   
            FolderItem2 networkConnection = GetNetworkConnection(networkConnectionsFolder, string.Empty);   
            if (networkConnection == null)   
            {   
                Console.WriteLine("Network connection not found.");   
                return;   
            }   
            FolderItemVerb verb;   
            try  
            {   
                IsNetworkConnectionEnabled(networkConnection, out verb);   
                verb.DoIt();   
                Thread.Sleep(1000);   
                IsNetworkConnectionEnabled(networkConnection, out verb);   
                verb.DoIt();   
            }   
            catch (ArgumentException ex)   
            {   
                Console.WriteLine(ex.Message);   
            }   
        }   
        /// <summary>   
        /// Gets the Network Connections folder in the control panel.   
        /// </summary>   
        /// <returns>The Folder for the Network Connections folder, or null if it was not found.</returns>   
        static Folder GetNetworkConnectionsFolder()   
        {   
            var sh = new Shell();   
            var controlPanel = sh.NameSpace(3); // Control panel   
            var items = controlPanel.Items();
            return (from FolderItem item in items where item.Name == "网络连接" select (Folder) item.GetFolder).FirstOrDefault();
        }   
        /// <summary>   
        /// Gets the network connection with the specified name from the specified shell folder.   
        /// </summary>   
        /// <param name="networkConnectionsFolder">The Network Connections folder.</param>   
        /// <param name="connectionName">The name of the network connection.</param>   
        /// <returns>The FolderItem for the network connection, or null if it was not found.</returns>   
        static FolderItem2 GetNetworkConnection(Folder networkConnectionsFolder, string connectionName)   
        {   
            var items = networkConnectionsFolder.Items();
            return items.Cast<FolderItem2>().FirstOrDefault(item => item.Name == ConnectionName);
        }   
        /// <summary>   
        /// Gets whether or not the network connection is enabled and the command to enable/disable it.   
        /// </summary>   
        /// <param name="networkConnection">The network connection to check.</param>   
        /// <param name="enableDisableVerb">On return, receives the verb used to enable or disable the connection.</param>   
        /// <returns>True if the connection is enabled, false if it is disabled.</returns>   
        static bool IsNetworkConnectionEnabled(FolderItem2 networkConnection, out FolderItemVerb enableDisableVerb)   
        {   
            var verbs = networkConnection.Verbs();   
            foreach (FolderItemVerb verb in verbs)
            {
                if (verb.Name == "启用(&A)")   
                {   
                    enableDisableVerb = verb;   
                    return false;   
                }
                if (verb.Name != "停用(&B)") continue;
                enableDisableVerb = verb;   
                return true;
            }
            throw new ArgumentException("No enable or disable verb found.");   
        }  
        #endregion   
    }   
}
