// ===== if( selectObject && selection.width > 0 && selection.height > 0 ) =====
{
Mat roi(image, selection);
bitwise_not(roi, roi);
}
#endif // _DEBUG
Scalar color = Scalar(0, 0, 255);
if(!paused)
{
isMatched = tracking();
switch (isMatched)
{
case 1:
color = Scalar(255, 0, 0);
break;
case 2:
color = Scalar(0, 255, 0);
break;
default:
break;
}
}
//rectangle(image, centerBound, color, 3);
imshow( "CamShift MainWindow", image );
#ifdef _DEBUG
imshow( "Histogram", histimg );
#endif // _DEBUG
char c = (char)waitKey(5);
if( c == 27 )              //退出键
break;
switch(c)
{
case 'r':
InterlockedExchange(&changeColor, 1);
break;
case 'b':             //反向投影模型交替
backprojMode = !backprojMode;
break;
case 'c':            //清零跟踪目标对象
clearTrack();
break;
case 'h':          //显示直方图交替
showHist = !showHist;
if( !showHist)
destroyWindow( "Histogram" );
else
namedWindow( "Histogram", 1 );
break;
case 'p':       //暂停跟踪交替
paused = !paused;
break;
case 'x':
//changeTarget = 1;
InterlockedExchange(&changeTarget, 1);
camshiftWorking = 0;
break;
case 'f':
showFPS = !showFPS;
break;
case 's':
saveHist();
saveSettings();
break;
case 'l':
loadSettings();
//if (loadHist())
//{
//	plotHist(histimg, hsize);
//}
break;
case 'k':
autoShoot = !autoShoot;
default:
;
}
}
//delete myPort;
//delete tracker;
return 0;
}