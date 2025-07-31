// ===== float boxH = trackBox.boundingRect().height; =====
if(!camshiftWorking || !isLegalRect(boxW, boxH))
{
tracker->clearPre();
//cout << "tracking failded:" << camshiftWorking << "," << boxW << "," << boxH << endl;
return 0;
}
float r = MIN(boxW, boxH) / 2.0f;
r = r < 20 ? 20 :(r > 40 ? 40 : r);
centerBound.x = centerX-r/2.0f;
centerBound.y = centerY-r/2.0f;
centerBound.width = r;