CFlayer：媒体播放库基于C/C++11和FFmpeg（2.1.3版本）
目前移植的Android版本已经发布（c++11 + SDL2（渲染） + ffmpeg （2.8.3_neon）+OpenAL（声音） + java （UI）= Android播放器）
github：https://github.com/juno52wendy/CCAV_Android
目前播放器1.1.2版本；release文件提供exe测试文件；

CFlayer操作平台：暂时只提供WIN7以上操作系统测试文件。

如果你开始弹出（error_pic_one、error_pic_two、error_pic_three）图片错误信息提示，
安装bin文件夹VS2013运行时库（vcredist_x86_2013.exe）即可！

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

该项目已经有两个开发者在国内使用，如开发人员使用它来显示实时监控视频摄像头。

注意：播放器刚刚起步，目前的功能比较简单，它能够正常同步播放视频。
也有几个目前已知的bug：缩放窗口，偶尔崩溃；频繁seek，会崩溃。

由于C/C++11的开发利用，播放器目标是跨平台，支持各种主流操作系统平台！
