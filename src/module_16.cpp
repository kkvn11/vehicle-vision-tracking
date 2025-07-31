// ===== nextBox.y = center.y - nextBox.height/2.f; =====
return nextBox;
}
/* 鼠标响应事件 */
void onMouse( int event, int x, int y, int, void* )
{
if( selectObject )//只有当鼠标左键按下去时才有效，然后通过if里面代码就可以确定所选择的矩形区域selection了
{
selection.x = MIN(x, origin.x);//矩形左上角顶点坐标
selection.y = MIN(y, origin.y);
selection.width = std::abs(x - origin.x);//矩形宽