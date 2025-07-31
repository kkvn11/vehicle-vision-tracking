// ===== if (lastRect.width < width || lastRect.height < height) =====
{
autoKh *= 1.2;
autoKw *= 1.3;
}
}
}
mask &= roimask;
}
#ifdef _DEBUG
//imshow("NextBox", roimask);
#endif // _DEBUG
//获取轮廓矩形
vector<vector<Point> > contours;
findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
selection = Rect();
int maxScore;
for (unsigned int i = 0; i < contours.size(); i++)
{
Rect rect = boundingRect(contours.at(i));