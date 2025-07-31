// ===== rectangle( histimg, Point(static_cast<int>(i*binW), histimg.size().height), =====
Point(static_cast<int>((i+1)*binW), histimg.rows - val),
Scalar(color), -1, 8 );
}
cvtColor(histimg, histimg, CV_HSV2BGR);//将hsv又转换成bgr
}
/* 清除当前追踪的目标 */
void clearTrack()
{
InterlockedExchange(&resetCamshift, 1);
//hist = Mat();
}
bool isLegalRect(float w, float h)
{
if(w <= 5 || h <= 3) {
return false;
}
return true;
float fHW = h / w;
return w*h > 10 && (fHW > 1.34 || fHW < 0.7);
}
int tracking()
{
float boxW = trackBox.boundingRect().width;