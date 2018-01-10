@echo off
cls

set disk=D:\
set mgr-sound=%disk%\repo\mgr-sound\Release\mgr-sound.exe
set configFiles=%disk%\repo\\mgr-sound\\config\\config_fmod.lua
set tableName="vehicle"
set SizeShmem=5
set pathFmodBank="../media/"


%mgr-sound% --size_shmem=%SizeShmem% --path_config=%configFiles% --config_name=%tableName% --path_fmod_bank=%pathFmodBank%

pause