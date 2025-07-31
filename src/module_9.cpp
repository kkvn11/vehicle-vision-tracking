// ===== #include <process.h> =====
using namespace cv;
using namespace std;
#define T_RED_A     5
#define T_RED_B     170
#define T_BLUE_LOW     100
#define T_BLUE_HIGH     110
#define HIST_IMG_ROWS  200
#define MIN_VAL 50			//通过直方图获取目标的阈值
#define WAIT_TIME 3			//等待摄像头打开的最大时间（s）
/* 用于设置输出信息的变量 */
bool showFPS = false;			//是否显示帧数
bool selectRed = false;			//是否自动选择红色
bool backprojMode = false;		//表示是否要进入反向投影模式，ture表示准备进入反向投影模式
bool showHist = true;			//是否显示直方图
volatile unsigned long changeTarget = 0;		//是否改变目标
volatile unsigned long changeColor = 0;
bool autoShoot = false;
/* 状态变量 */
bool selectObject = false;		//代表是否在选要跟踪的初始目标，true表示正在用鼠标选择
int camshiftWorking = 0;		//非0时 由camshift追踪目标
volatile long resetCamshift = 0;
Point origin;			//用于保存鼠标选择第一次单击时点的位置
Rect selection;			//用于保存鼠标选择的矩形框
Mat image;				//摄像头得到的图像
Mat hist;				//hue 的直方图
RotatedRect trackBox;	//追踪到的目标
RotatedRect preBox;
bool paused = false;
int width = 640;
int height = 480;
float boundW = 52;
float boundH = 45;
int centerX = 320;
int centerY = 300;
Rect_<float> centerBound(centerX-boundW/2.0f, centerY-boundH/2.0f,
boundW, boundH);
int vmin = 50, vmax = 256, smin = 110;
PortOperate *myPort;
Tracker *tracker;
float autoKw = 1;
float autoKh = 1;
void saveSettings()
{
FileStorage fs("C:\\robomasters\\settings.xml", FileStorage::WRITE);
fs << "width" << width;
fs << "height" << height;
fs << "centerX" << centerX;
fs << "centerY" << centerY;
fs << "selectRed" << selectRed;
fs << "vmin" << vmin;
fs << "vmax" << vmax;
fs << "smin" << smin;
fs.release();
cout << "save settings successfully" << endl;
cout << "width:" << width << "; height:" << height << "; centerX:"
<< centerX << "; centerY:" << centerY << "; selectRed:" << selectRed << endl;
}
bool loadSettings()
{
FileStorage fs("D:\\robomasters\\settings.xml", FileStorage::READ);
int w, h;
fs["width"] >> w;
fs["height"] >> h;
if (w == 0 || h == 0)
{
cout << "load settings failed" << endl;
return false;
}
width = w;
height = h;
fs["vmin"] >> vmin;
fs["vmax"] >> vmax;
fs["smin"] >> smin;
fs["centerX"] >> centerX;
fs["centerY"] >> centerY;
fs["selectRed"] >> selectRed;
//selectRed = 0;
fs.release();
tracker->setParameters(width, height, centerX, centerY, boundW, boundH);
tracker->clearPre();
centerBound.x = centerX-boundW/2.0f;
centerBound.y = centerY-boundH/2.0f;
centerBound.width = boundW;