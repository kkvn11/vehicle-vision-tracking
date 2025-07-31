// ===== #include "MyComClass.h" =====
#include <iostream>
using namespace std;
Tracker::Tracker(int width, int height,
int centerX, int centerY, float boundW, float boundH)
{
//initKalmanFilter();
setParameters(width, height,
centerX, centerY, boundW, boundH);
clearPre();
}
Tracker::~Tracker(void)
{
}
void Tracker::clearPre()
{
preTime = -1;
preLocation = Location();
preAngle = Angle();
preSpeed = Speed();
aSpeed = Speed();
matchCount = 0;
xCount = yCount = 5;
isStable = false;
clearSlow();
xSlow = ySlow = true;
//clearKalmanFilter();
}
void Tracker::setParameters(int width, int height,
int centerX, int centerY, float boundW, float boundH)
{
this->width = width;
this->height = height;
this->boundH = boundH;
this->boundW = boundW;
this->centerX = centerX;
this->centerY = centerY;
}
int Tracker::getTimeStamp(void)
{
SYSTEMTIME st;
GetLocalTime(&st);
return ((((st.wHour * 60) + st.wMinute) * 60 + st.wSecond) * 1000 + st.wMilliseconds);
}
float Tracker::getOneAngle(float dx, float dxMid, float dxMax,
float angleMin, float angleMid, float angleMax)
{
float angle;
float sign = dx > 0 ? 1.f : -1.f;
if (abs(dx) < dxMid)
{
float k1 = (angleMid-angleMin) / dxMid;
angle = k1 * dx + sign * angleMin;
//cout << "k1:" << angle << endl;
}
else
{
float k2 = (angleMax - angleMid) / (dxMax - dxMid);
angle = k2 * (dx-dxMid*sign) + sign * angleMid;
//cout << "k2:" << angle << endl;
}
if(abs(angle) > angleMax)
angle = sign * angleMax;
//cout << angle << endl;
return angle;
}
Angle Tracker::getAngle(Location curLocation)
{
return getAngle(curLocation.x, curLocation.y);
}
Angle Tracker::getAngle(float dx, float dy)
{
float xAngle, yAngle;
#ifdef _DEBUG
cout << "Xslow:" << xSlow << ", " << ySlow << endl;
#endif // _DEBUG
float xMin = xSlow ? 0 : 1;
float xMid = xSlow ? 4 : 6;
float xMax = xSlow ? 8 : 12;
float xK1 = xSlow ? 3.f : 4.f;
float yMin = ySlow ? 0 : 0;
float yMid = ySlow ? 2 : 3;
float yMax = ySlow ? 3 : 5;
//cout << "dx:" << dx << ", " << width / 4.f << endl;
xAngle = getOneAngle(dx, width/xK1, width/2.f, xMin, xMid, xMax);
yAngle = getOneAngle(dy, height/3.f, height/2.f, yMin, yMid, yMax);
return Angle(xAngle, yAngle);
}
bool Tracker::tracking(float dx, float dy, float r)
{
Location curLocation = Location(dx, dy);
Speed speed = getSpeed(curLocation);
if (abs(speed.x) > 1.0 || abs(speed.y) > 0.50)
{
//cout << "speed out" << speed.x << "," << speed.y <<endl;
clearPre();
}
bool matched = isMatched(curLocation, r);
//cout << matched << ", " << curLocation.x << "," << curLocation.y << endl;
//判断是否稳定
isStabled(matched);
if (isStable)
{
speed = speed + aSpeed * 4000;
curLocation.x += speed.x * 100;
curLocation.y += speed.y * 60;
//cout << "fast now" << endl;
clearSlow();
}
else
{
//curLocation.x += speed.x * 50;
//curLocation.y += speed.y * 20;
slower(Location(dx, dy));
}
Angle angle = (getAngle(curLocation)*5.f + preAngle*1.f) / 6.f;
//Angle angle = getAngle(curLocation);
//angle = runKalmanFilter(angle.x, angle.y);
//防止卡住
angle.x = adjustOneAngle(angle.x, 4, xCount);
//angle.y = adjustOneAngle(angle.y, 4, yCount);
preLocation = Location(dx, dy);
preAngle = angle;
return matched;
}
bool Tracker::isStabled(bool isMatched)
{
if (isMatched)
{
matchCount++;
#ifdef _DEBUG
cout << "match count:" << matchCount << endl;
#endif // _DEBUG
if (matchCount >= 16)
{
isStable = true;
}
}
else
{
matchCount = 0;
isStable = false;
//cout << "lost match" << endl;
}
return isStable;
}
bool Tracker::isMatched(float dx, float dy, float r)
{
//return abs(dx) < boundW / 2.8f && abs(dy) < boundH / 2.5f;
//return dx*dx + dy*dy < r*r;
return abs(dx) < r;
}
bool Tracker::isMatched(Location curLocation, float r)
{
return isMatched(curLocation.x, curLocation.y, r);
}
Speed Tracker::getSpeed(Location curLocation)
{
int curTime = getTimeStamp();
Speed curSpeed;
if (preTime > 0)
{
float detaTime = curTime - preTime + 0.0001;
curSpeed = (curLocation - preLocation) / detaTime;
curSpeed = (curSpeed + preSpeed) / 2.;
aSpeed = (curSpeed - preSpeed) / detaTime;
}
preSpeed = curSpeed;
preTime = curTime;
return Speed(curSpeed.x, curSpeed.y);
}
//卡死则angle 减半
float Tracker::adjustOneAngle(float angle, float threshod, int &count)
{
if (abs(angle) > threshod)
{
count--;
}
else
{
count = 3;
}
if (count == 0)
{
count = -2;
}
if (count < 0)
{
count++;
return angle / 2.f;
}
//cout << "after adjust:" << angle << endl;
return angle;
}
bool Tracker::toSlower(float curX, float preX, int &count, int &preTime)
{
int curTime = getTimeStamp();
if (curX * preX < 0)
{
count++;
}
//cout << curX << "," << preX << endl;
if (count == 1 && preTime <= 0)
{
preTime = curTime;
}
if (count >= 1)
{
cout << "slow now!!!!!!!!!#######" << endl;
return true;
}
#ifdef _DEBUG
cout << "slow count:" << count << ", " << curTime - preTime << endl;
#endif // _DEBUG
if (curTime - preTime > 1000)
{
count = 0;
}
return false;
}
void Tracker::slower(Location curLocation)
{
if (!xSlow)
{
xSlow = toSlower(curLocation.x, preLocation.x, xZeroCount, xZeroTime);
}
if (!ySlow)
{
ySlow = toSlower(curLocation.y, preLocation.y, yZeroCount, yZeroTime);
}
}
void Tracker::clearSlow()
{
xZeroCount = yZeroCount = 0;
xZeroTime = yZeroTime = -1;
xSlow = ySlow = false;
}
void Tracker::initKalmanFilter()
{
measurement = Mat::zeros(measureNum, 1, CV_32F);
state = Mat(stateNum, 1, CV_32FC1);
processNoise = Mat(stateNum, 1, CV_32F);
KF.init(4, 2);
KF.transitionMatrix = *(Mat_<float>(4, 4) <<
1,0,45,0,
0,1,0,45,
0,0,1,0,
0,0,0,1 );//元素导入矩阵，按行;
//setIdentity: 缩放的单位对角矩阵;
//!< measurement matrix (H) 观测模型
setIdentity(KF.measurementMatrix);
//!< process noise covariance matrix (Q)
// wk 是过程噪声，并假定其符合均值为零，协方差矩阵为Qk(Q)的多元正态分布;
setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
//!< measurement noise covariance matrix (R)
//vk 是观测噪声，其均值为零，协方差矩阵为Rk,且服从正态分布;
setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
//!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/  A代表F: transitionMatrix
//预测估计协方差矩阵;
setIdentity(KF.errorCovPost, Scalar::all(1));
clearKalmanFilter();
}
FloatTuple Tracker::runKalmanFilter(float x, float y)
{
//2.kalman prediction
Mat prediction = KF.predict();
predictPt = Point_<float>(prediction.at<float>(0), prediction.at<float>(1));
//3.update measurement
measurement.at<float>(0)= x;
measurement.at<float>(1) = y;
//4.update
KF.correct(measurement);
return FloatTuple(predictPt.x, predictPt.y);
}
void Tracker::clearKalmanFilter()
{
//!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
//initialize post state of kalman filter at random
randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
}