// ===== int cm = trackWindow.width <= 0 || trackWindow.height <= 0 ? 0 : 1; =====
camshiftWorking = cm;
if (!camshiftWorking)
{
continue;
}
} //end if camshiftWorking < 0
//计算直方图的反向投影
calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
backproj &= mask;
//getBlock(backproj);
//trackWindow为鼠标选择的区域，TermCriteria为确定迭代终止的准则
preBox = trackBox;
trackBox = CamShift(backproj, trackWindow, TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
if( trackWindow.area() <= 1 )
{
//cout << "<=1";
int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
trackWindow.x + r, trackWindow.y + r) &
Rect(0, 0, cols, rows);
}
if( backprojMode )
cvtColor( backproj, image, CV_GRAY2BGR );