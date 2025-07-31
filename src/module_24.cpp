// ===== if(isLegalRect(selection.width, selection.height)) =====
{
autoKh = autoKw = 1.1;
//changeTarget = 0;
InterlockedExchange(&changeTarget, 0);
InterlockedExchange(&resetCamshift, 0);
camshiftWorking = -2;
#ifdef _DEBUG
cout << "auto set selection" << endl;
cvtColor(hsvFrame, hsvFrame, CV_HSV2BGR);//将hsv又转换成bgr
rectangle(hsvFrame, selection, Scalar(255, 0, 0), 3);
imshow( "AutoSet", hsvFrame );
#endif // _DEBUG
}
}
unsigned __stdcall ListenThread(PVOID lpvoid)
{
PortOperate * myPort=(PortOperate *)lpvoid;
static unsigned char preChar = 0xaa;
unsigned char buf[2]= {0xaa,0};
while(true)
{
myPort->ReadData(buf,1);
if (buf[0]==0x00 && preChar==0xaa)
{
//InterlockedExchange(&changeTarget, 1);
//InterlockedExchange(&changeColor, 1);
InterlockedExchange(&resetCamshift, 1);
cout << "reset camshift" << endl;
}
preChar = buf[0];
//buf[0] = 0;
//cout << "0x" << hex << int(buf[0]) << endl;
}
}
void loadColor()
{
if (selectRed)
{
loadHist("C:\\robomasters\\red.xml");
}
else
{
loadHist("C:\\robomasters\\blue.xml");
}
}
bool doChangeColor()
{
if (!changeColor)
{
return false;
}
clearTrack();
selectRed = !selectRed;
saveSettings();
loadColor();
InterlockedExchange(&changeColor, 0);
return true;
}
//void getBlock(Mat &bp)
//{
//	Mat mask;
//	inRange(bp, Scalar(1), Scalar(255), mask);
//
//	//开运算，闭运算
//	//内核矩形
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	dilate(mask, mask, element);
//	dilate(mask, mask, element);
//	dilate(mask, mask, element);
//	//dilate(mask, mask, element);
//	//morphologyEx(mask, mask, MORPH_OPEN, element, Point(-1, -1), 3);
//	//morphologyEx(mask, mask, MORPH_CLOSE, element, Point(-1, -1), 1);
//
//	//获取轮廓矩形
//	vector<vector<Point> > contours;
//	Mat tmp = mask.clone();
//	findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//
//	for(int i = 0; i < contours.size(); i++)
//	{
//		Rect rect = boundingRect(contours.at(i));
//		rectangle(mask, rect, Scalar(255), 2);
//	}
//	imshow("block", mask);
//}
int main()
{
tracker = new Tracker(width, height, centerX, centerY, boundW, boundH);
myPort = new PortOperate();
unsigned char buf[100]="";
char com[5] = "COM1";
while ((com[3] - '0') < 10 && !myPort->Initial(com))
{
cout << "open com failed:" << com << endl;
com[3]++;
}
if(com[3]-'0' >= 10)
{
//return -1;
}
else
{
cout << "open com succ:" << com << endl;
}
unsigned int readThreadId;
HANDLE readThreadHandle = (HANDLE)_beginthreadex(NULL, 0, ListenThread, myPort, 0, &readThreadId);
if(readThreadHandle!=NULL)
cout << "thread succ" << endl;
VideoCapture cap;
int hsize = 180;	//颜色直方图的大小
float hranges[] = {0,180};
const float* phranges = hranges;
int isMatched = 0;
char *video = "E:\\RoboMasters\\test_video\\blue0.avi";
cap.open(video);
if( !cap.isOpened() )
{
cout << "***Could not initialize capturing...***\n";
return -1;
}
#ifdef _DEBUG
namedWindow( "Histogram", 0 );	//直方图
namedWindow( "AutoSet", 0 );	//自动设置窗口
//namedWindow("NextBox", 0);
#endif // _DEBUG
//设置主窗口以及鼠标监听事件
namedWindow( "CamShift MainWindow", 0 );
setMouseCallback( "CamShift MainWindow", onMouse, 0 );	//消息响应机制
createTrackbar( "Vmin", "CamShift MainWindow", &vmin, 255, 0 );
createTrackbar( "Vmax", "CamShift MainWindow", &vmax, 255, 0 );
createTrackbar( "Smin", "CamShift MainWindow", &smin, 255, 0 );
Mat frame, hsv, hue, mask, histimg = Mat::zeros(HIST_IMG_ROWS, 320, CV_8UC3), backproj;
Rect trackWindow;
//载入设置
loadSettings();
loadColor();
#ifdef _DEBUG
plotHist(histimg, hsize);
#endif // _DEBUG
while (true)
{
if (doChangeColor())
{
cout << "change color:" << selectRed << endl;
}
if( !paused )
{
cap >> frame;
if( frame.empty() )
{
continue;
}
frame.copyTo(image);
cvtColor(image, hsv, CV_BGR2HSV);	//将rgb摄像头帧转化成hsv空间的
if (changeTarget)
{
camshiftWorking = 0;
}
if( !camshiftWorking || resetCamshift)
{
//自动获取目标
autoSetSelection(hsv);
}
else
{
/*
inRange函数的功能是检查输入数组每个元素大小是否在2个给定数值之间，可以有多通道,mask保存0通道的最小值，也就是h分量
这里利用了hsv的3个通道，比较h,0~180,s,smin~256,v,min(vmin,vmax),max(vmin,vmax)。如果3个通道都在对应的范围内，则
mask对应的那个点的值全为1(0xff)，否则为0(0x00).
*/
if (selectRed)
{
Mat mask1, mask2;
inRange(hsv, Scalar(0, smin, MIN(vmin,vmax)),
Scalar(180, 255, MAX(vmin, vmax)), mask);
inRange(hsv, Scalar(0, 0, 0),
Scalar(8, 255, 255), mask1);
inRange(hsv, Scalar(174, 0, 0),
Scalar(180, 255, 255), mask2);
mask = (mask1 | mask2) & mask;
}
else
{
inRange(hsv, Scalar(100, smin, MIN(vmin,vmax)),
Scalar(110, 255, MAX(vmin, vmax)), mask);
}
/*
将hsv第一个通道(也就是色调)的数复制到hue中
hue初始化为与hsv大小深度一样的矩阵，色调的度量是用角度表示的，红绿蓝之间相差120度，反色相差180度
*/
int ch[] = {0, 0};
hue.create(hsv.size(), hsv.depth());
mixChannels(&hsv, 1, &hue, 1, ch, 1);
//首次使用camshift 追踪
if( camshiftWorking < 0 )
{
//鼠标选择区域，或hist为空，重新计算hist
if(camshiftWorking == -1 ||  hist.cols <= 0 || hist.rows <= 0)
{
//此处的构造函数roi用的是Mat hue的矩阵头，且roi的数据指针指向hue，即共用相同的数据，select为其感兴趣的区域
Mat roi(hue, selection), maskroi(mask, selection);
//将roi的0通道计算直方图并通过mask放入hist中，hsize为每一维直方图的大小
//并将hist矩阵进行数组范围归一化，都归一化到0~255
calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
normalize(hist, hist, 0, 255, CV_MINMAX);
#ifdef _DEBUG
//绘制直方图
plotHist(histimg, hsize);
#endif // _DEBUG
} //end if camshiftWorking == -1
trackWindow = selection;