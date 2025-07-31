// ===== centerBound.height = r; =====
float dx = centerX - trackBox.center.x;
float dy = centerY - trackBox.center.y;
//float dx = centerX - predictPt.x;
//float dy = centerY - predictPt.y;
if (tracker->tracking(dx, dy, r))
{
bool shoot = tracker->isStable;
#ifdef _DEBUG
//Speed speed = tracker->preSpeed;
//cout << "speed: " << speed.x << ", " << speed.y << endl;
//cout << "a:" << tracker->aSpeed.x << "," << tracker->aSpeed.y << endl;
#endif // _DEBUG
//cout << shoot << endl;
return shoot ? 2 : 1;
}
Angle angle = tracker->preAngle;
myPort->SendAngle(angle.x, angle.y);
#ifdef _DEBUG
cout << "angle: " << angle.x << ", " << angle.y << endl;
//Speed speed = tracker->preSpeed;
//cout << "speed: " << speed.x << ", " << speed.y << endl;
//cout << "a:" << tracker->aSpeed.x << "," << tracker->aSpeed.y << endl;
#endif // _DEBUG
return 0;
}
Rect getNextBox(float kw=1.3f, float kh = 1.2f)
{
Rect nextBox = preBox.boundingRect();
Location center;