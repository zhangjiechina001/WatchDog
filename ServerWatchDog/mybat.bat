@echo off
setlocal

rem Define the path to the target application and the shortcut name
set "TargetPath=%1"  rem Modify this to the actual path of your application
set "ShortcutName=WatchDog"        rem Name of the shortcut

rem Create Startup shortcut
set "StartupShortcut=C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp\%ShortcutName%.lnk"
if not exist "%StartupShortcut%" (
    powershell -Command "$WshShell = New-Object -ComObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('%StartupShortcut%'); $Shortcut.TargetPath = '%TargetPath%'; $Shortcut.WorkingDirectory = [System.IO.Path]::GetDirectoryName('%TargetPath%'); $Shortcut.IconLocation = '%TargetPath%'; $Shortcut.Save()"
    echo Startup shortcut '%ShortcutName%' has been created.
) else (
    echo Startup shortcut '%ShortcutName%' already exists.
)

endlocal
