// ===== if (lastRect.height > 0 && lastRect.width > 0) =====
{
roimask = Mat::zeros(mask.rows, mask.cols, CV_8U);
roimask(lastRect) = 255;