// ===== if(nextBox.width <= 1 && nextBox.height <= 1) =====
{
nextBox = Rect(0, 0, 2, 2);
center = Location(width/2, height/2);
}
else
{
center = Location(preBox.center.x, preBox.center.y);
}
Speed speed = tracker->preSpeed;
center.x -= speed.x * 80;
center.y -= speed.y * 50;
nextBox.width *= kw;