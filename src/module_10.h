// ===== centerBound.height = boundH; =====
cout << "load setting successfully" << endl;
cout << "width:" << width << "; height:" << height << "; centerX:"
<< centerX << "; centerY:" << centerY << "; selectRed:" << selectRed << endl;
cout << "vmin:" << vmin << "; vmax:" << vmax << "; smin:" << smin << endl;
return true;
}
bool saveHist(const char * filename = "C:\\robomasters\\hist.xml")
{
if (hist.rows <= 0 || hist.cols <= 0)
{
cout << "save hist faided" << endl;
return false;
}
FileStorage fs(filename, FileStorage::WRITE);
fs << "hist" << hist;
fs.release();
cout << "save hist successfully" << endl;
return true;
}
bool loadHist(const char * filename = "C:\\robomasters\\hist.xml")
{
FileStorage fs(filename, FileStorage::READ);
fs["hist"] >> hist;
if(hist.rows <= 0 || hist.cols <= 0)
{
cout << "load hist failed" << endl;
return false;
}
normalize(hist, hist, 0, 255, CV_MINMAX);
cout << "load hist successfully" << endl;
return true;
}
void plotHist(Mat &histimg, int hsize)
{
//绘制直方图
histimg = Scalar::all(0);
float binW = (float)histimg.size().width / hsize;
for( int i = 0; i < hsize; i++ )
{
Vec3b color = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
int val = static_cast<int>(hist.at<float>(i));	//at函数为返回一个指定数组元素的参考值