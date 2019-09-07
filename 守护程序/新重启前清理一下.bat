@ ECHO off
rasdial 宽带连接 /DISCONNECT
taskkill /f /im QQLogin.exe
taskkill /f /im TXPlatform.exe
taskkill /f /im TesSafe.exe
taskkill /f /im DNFMulti.exe
taskkill /f /im QQDL.exe
taskkill /f /im DNF.exe
taskkill /f /im DNFchina.exe
taskkill /f /im TesSafe_1.exe
taskkill /f /im TesSafe_2.exe
taskkill /f /im CheckProcess.exe
taskkill /f /im bsj.exe
taskkill /f /im DNF.exe
taskkill /f /im DNFchina.exe
taskkill /f /im TenioDL.exe
taskkill /f /im Tencentdl.exe
taskkill /f /im TASLogin.exe
taskkill /f /im Client.exe
echo 正在清理系统垃圾文件，请稍等......
del /f /s /q %systemdrive%\*.tmp
del /f /s /q %systemdrive%\*._mp
del /f /s /q %systemdrive%\*.log
del /f /s /q %systemdrive%\*.gid
del /f /s /q %systemdrive%\*.chk
del /f /s /q %systemdrive%\*.old
del /f /s /q %systemdrive%\recycled\*.*
del /f /s /q %windir%\*.bak
del /f /s /q %windir%\prefetch\*.*
rd /s /q %windir%\temp & md %windir%\temp
del /f /q %userprofile%\cookies\*.*
del /f /q %userprofile%\recent\*.*
del /f /s /q "%userprofile%\Local Settings\Temporary Internet Files\*.*"
del /f /s /q "%userprofile%\Local Settings\Temp\*.*"
del /f /s /q "%userprofile%\recent\*.*"
del C:\地下城与勇士\DNF_CHINA.cfg
rasdial 宽带连接 username password

exit
echo 清理系统垃圾完成！