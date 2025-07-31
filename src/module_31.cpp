// ===== #include "opencv/highgui.h" =====
#define stateNum 4
#define measureNum 2
using namespace cv;
using namespace std;
struct FloatTuple
{
float x, y;
FloatTuple()
{
x = 0;
y = 0;
}
FloatTuple(float x, float y)
{
this->x = x;
this->y = y;
}
FloatTuple operator+(FloatTuple Other)
{
FloatTuple t_(x+Other.x, y+Other.y);
return t_;
}
FloatTuple operator-(FloatTuple Other)
{
FloatTuple t_(x-Other.x, y-Other.y);
return t_;
}
FloatTuple operator/(float d)
{
FloatTuple t_(x/d, y/d);
return t_;
}
FloatTuple operator*(float d)
{
FloatTuple t_(x*d, y*d);
return t_;
}
};
typedef FloatTuple Angle;
typedef FloatTuple Location;
typedef FloatTuple Speed;
class Tracker
{
public:
Tracker(int width, int height,
int centerX, int centerY, float boundW, float boundH);
~Tracker(void);
void setParameters(int width, int height,
int centerX, int centerY, float boundW, float boundH);
int getTimeStamp(void);
bool tracking(float x, float y, float r);
bool isMatched(float dx, float dy, float r);
bool isMatched(Location curLocation, float r);
void clearPre(void);
bool isStabled(bool isMatch);
private:
Speed getSpeed(Location curLocation);
Angle getAngle(float dx, float dy);
Angle getAngle(Location curLocation);
//分段、设定最大值
float getOneAngle(float dx, float dxMid, float dxMax,
float angleMin, float angleMid, float angleMax);
float adjustOneAngle(float angle, float threshod, int &count);
//需要减速
void slower(Location curLocation);
bool toSlower(float curX, float preX, int &count, int &preTime);
void clearSlow();
void initKalmanFilter();
FloatTuple runKalmanFilter(float x, float y);
void clearKalmanFilter();
int width;
int height;
float boundW;
float boundH;
int centerX;
int centerY;
int xCount, yCount;		//卡死计数
int xZeroCount, yZeroCount;	//过零计数
int xZeroTime, yZeroTime;
bool xSlow, ySlow;
unsigned int matchCount;
int preTime;
//1.kalman filter setup
KalmanFilter KF;
Mat state; //state(x,y,detaX,detaY)
Mat processNoise;
Mat measurement;    //measurement(x,y)
Point_<float> predictPt, curPt;
public:
Location preLocation;
Speed preSpeed;
Speed aSpeed;
Angle preAngle;
bool isStable;
};