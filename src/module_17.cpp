// ===== selection.height = std::abs(y - origin.y);//矩形高 =====
selection &= Rect(0, 0, image.cols, image.rows);//用于确保所选的矩形区域在图片范围内
}
switch( event )
{
case CV_EVENT_LBUTTONDOWN:
origin = Point(x,y);
selection = Rect(x,y,0,0);//鼠标刚按下去时初始化了一个矩形区域
selectObject = true;
break;
case CV_EVENT_LBUTTONUP:
selectObject = false;