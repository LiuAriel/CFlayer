# CFlayer
CFlayer（又名CCAV）：媒体播放库基于C/C++11和FFmpeg（2.1.3版本）
目前除了UI，渲染依赖qt之外，后续版本会将l渲染剥离出qt依赖！

CCAV1.1.2版本，提供测试exe                                                                                         
主要修复头帧丢失问题；                                                                                           
修复一些bug；                                                                                               
优化队列结构；                                                                                                     
调整数据接口。                                                                                                     

播放器也刚刚开始做，基本上就是能正常播放视频；release文件夹提供player.exe文件；

CFlayer操作平台：暂时只提供WIN7以上操作系统测试文件。

如果你开始弹出（error_pic_one、error_pic_two、error_pic_three）图片错误信息提示，
安装bin文件夹VS2013运行时库（vcredist_x86_2013.exe）即可（目前必须保证机器有音频播放设备，否则不能播放）！

使用说明：                                                                                                                   
按“M”静音;                                                                                                                  
按“P”是重播;                                                                                                                
按“N”是一帧一帧播放;                                                                                                        
按空格键是暂停;                                                                                                              
按“O”打开选择视频;                                                                                                           
按键盘左，右箭头键，快进与后退;                                                                                              
按“S”是停止;                                                                                                                 
按“T”是置顶窗口;                                                                                                            
按键盘向上和向下箭头键来改变音量。

由于网络问题，暂时不能更新最新代码；


CFlayer:  Media play library based on c/c++ and FFmpeg

Currently Player version 1.1.1, also uploaded a bin file, you can test it! 

CFlayer operating platform: win7 above operating system。

If you start a pop-up error message, install the bin folder inside the VS2013 runtime library (vcredist_x86_2013.exe) can be!

User Instructions:                                                                                                              
The letter "M" is muted；                                                                                                      
"P" is a replay；                                                                                                              
Continuously press the "N" is a one player                                                                                    ；
Space is suspended；                                                                                                            
"O" to open the Select Videos；                                                                                                 
Left and right arrow keys to fast forward；                                                                                     
"S" is to stop；                                                                                                                
"T" is a window on top；                                                                                                        
Up and down arrow keys to change the volume。                                                                                   

The project has been due to the relatively small number of developers in domestic use, such as a developer to use it to display real-time surveillance video camera.

Waring: the player has just started, the current function is relatively simple, it is able to playback videos. Also several currently known bug: when the zoom window crashes often, frequently seek sometimes crashes, the latter will be repaired.

Since c / c ++ development using, so the player's goal is cross-platform, supports various mainstream operating platform!
    
    


