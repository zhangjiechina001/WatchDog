# WatchDog
现场程序断电无法重启，做一个看门狗程序进行重启、卡死检查

1.ServerWatchDog软件负责创建共享内存，这样共享内存只在一端软件管理即可

2.ServerWatchDog可以控制多台目标程序的启停，只需要在配置文件中增加exe路径和共享内存的Key,另外目标程序也需要做一些修改，读取ServerWatchDog创建的共享内存，并且定时将内存置零
```json
{
    "LIBS2700Server1": {
        "Enable":true,
        "ProgramPath":"E:\\Projects\\WatchDog\\Demo\\bin_vs\\Demo.exe",
        "MemoryKey":"LIBSAppServerAlreadyRunning",
		"Internal":1
    }
}
```
