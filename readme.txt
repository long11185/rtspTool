qt+mingw32+windows+ffmpeg+rtsp保证编译可用
说明:
1:此示例只是用来显示视频流,并没有处理存储视频及回放视频功能.
2:在打开项目后务必将构建里面的影子构建(Shadow build)取消.
3:实时显示视频,视频响应速度比VLC,QTAV等播放器快很多倍.
4:精简代码,在windows下是可以完整编译通过并运行的.linux下要将对应ffmpeg库文件替换.
5:支持三通道同步显示一个视频流，可以单击截图按钮截图放在最后一个通道.
6: 支持单个通道双击最大化显示.
7:代码处理并不够完整,还有很多需要优化的地方.