// ===== if( selection.width > 0 && selection.height > 0 ) =====
camshiftWorking = -1;
break;
}
}
/* camshift 目标丢失后自动找回 */
void autoSetSelection(const Mat &hsv)
{
if(hist.cols <= 0 || hist.rows <= 0)
{
return;
}
Mat hsvFrame;
hsv.copyTo(hsvFrame);
int channels = hsvFrame.channels();
int nRows = hsvFrame.rows;
int nCols = hsvFrame.cols * channels;
if (hsvFrame.isContinuous())//连续存储的数据，按一行处理
{
nCols *= nRows;
nRows = 1;
}
//保存单通道图片的Mat
Mat mask(hsvFrame.rows, hsvFrame.cols, CV_8UC1, Scalar(0, 0, 0));
uchar *DstData = mask.data;
//遍历Mat 找出红色或者蓝色存到sigleImage
int i,j;
uchar* p;
for( i = 0; i < nRows; ++i)
{
p = hsvFrame.ptr<uchar>(i);
for ( j = 0; j < nCols; j+=channels)
{
uchar nH = p[j];
uchar nS = p[j+1];
uchar nV = p[j+2];
int dataIndex = i * nCols + j / channels;
if (!(nV > vmin && nS > smin))
{
DstData[dataIndex] = 0;
}
else
{
float val = hist.at<float>(nH);
if(val > MIN_VAL)
{
DstData[dataIndex] = 255;
}
else
{
DstData[dataIndex] = 0;
}
}
}
}
//开运算，闭运算
//内核矩形
Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
morphologyEx(mask, mask, MORPH_OPEN, element, Point(-1, -1), 2);
morphologyEx(mask, mask, MORPH_CLOSE, element, Point(-1, -1), 1);
//Mat roimask = Mat::ones(mask.rows, mask.cols, CV_8U);
if (!resetCamshift)
{
Mat roimask = Mat(mask.rows, mask.cols, CV_8U, Scalar(255));
Rect bound(0, 0, mask.cols, mask.rows);
if (changeTarget)
{
Rect roi = preBox.boundingRect() & bound;
roimask(roi) = 0;
}
else
{
Rect lastRect = getNextBox(autoKw, autoKh) & bound;
//cout << lastRect << endl;