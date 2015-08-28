里面有三个工程：

Foundation: 需要最先编译，然后把其DLL和lib目录下的文件copy到rage中去，Foundation库封装了平台相关的系统调用，网络部分，文件系统以及各种辅助工具。

Rage: 是bt的业务逻辑部分，由于Foundation最开始的目的是隔离掉系统差异，想移植Rage的话，只需移植Foundation即可。

Diehard是用MFC写的UI部分，可以算做一个综合测试以及用法部分。Downloads里面提供的就是此客户端的静态链接版本.[DieHard.rar](http://ragebtclient.googlecode.com/files/DieHard.rar)

PS：这东西只能算是个相对完整和稳定的bt客户端，但与我当初的目标相去甚远，如果同样有人开发此类软件并能从其中得到某些帮助或教训，我会很高兴的！


有时间我会用C重构整个项目并加入DHT模块.


email： SolidusSnakeEx@gmail.com



