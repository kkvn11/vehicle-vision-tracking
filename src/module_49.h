// ===== WaitForSingleObject( ov.hEvent, INFINITE ); =====
GetOverlappedResult( A_hCom, &ov, &dwWritten, TRUE );
}
//else{
//	printf("write done %d\n",dwWritten);
//	//for(int i = 0;i < uLen;i++)
//	//{
//	//	printf("%X\t",pBuffer[i]);
//	//}
//	//printf("\n");
//}
}
//	PurgeComm(A_hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
return 0;
}
void PortOperate::CloseComm()
{
CloseHandle(A_hCom);
printf("Com closed!\n");
A_hCom = NULL;
}
/****校验和计算函数********/
unsigned char PortOperate::CheckSum(unsigned char *uBuff, unsigned char uBuffLen)
{
//unsigned char i,uSum=0;
//for(i=3;i<uBuffLen-1;i++)
//{
//	uSum = uSum + uBuff[i];
//}
//uSum = (~uSum) + 1;
//if(uSum > 0xf0)
//	uSum -= 16;
////	printf("%X\n",uSum);
//return uSum;
unsigned char uSum = uBuff[0];
for(int i = 1; i < uBuffLen; i++)
{
uSum ^= uBuff[i];
}
return uSum;
}
/********************
操作接口函数
命令集：
1.01 底盘
2.02 角度，摩擦轮
3.03 发射
4.04 俯仰 角度
指令格式：
上位机->下位机
0xFE 55 AA 命令 数据1 数据2 数据3 数据4 校验和
下位机->上位机
0xAA 接收正确
0x66 接收错误
*********************/
bool PortOperate::Initial(const char* port)
{
if(OpenComm(port)) return true;
else return false;//running=false;
}
bool PortOperate::SendAction(char direction, char speed)
{
memset(tmpchar, 0x00, sizeof(tmpchar));
tmpchar[0] = 0xFE;
tmpchar[1] = 0x55;
tmpchar[2] = 0xAA;
tmpchar[3] = 0x01;
tmpchar[4] = speed << 7 | direction;
tmpchar[8] = CheckSum(tmpchar,9);
WriteData(tmpchar, 9);
return 0;
}
bool PortOperate::SendShoot()
{
memset(tmpchar, 0x00, sizeof(tmpchar));
tmpchar[0] = 0xFE;
tmpchar[1] = 0x55;
tmpchar[6] = 0xFF;
tmpchar[7] = CheckSum(tmpchar, 9);
WriteData(tmpchar, 8);
return 0;
}
bool PortOperate::SendLocation(unsigned short angle, char speed)
{
memset(tmpchar, 0x00, sizeof(tmpchar));
tmpchar[0] = 0xFE;
tmpchar[1] = 0x55;
tmpchar[2] = 0xAA;
tmpchar[3] = 0x02;
tmpchar[4] = angle >> 8;
tmpchar[5] = angle & 0x00ff;
tmpchar[6] = speed;
tmpchar[8] = CheckSum(tmpchar, 9);
WriteData(tmpchar, 9);
return 0;
}
bool PortOperate::SendAngle(float nHangle, float nVangle)
{
memset(tmpchar, 0x00, sizeof(tmpchar));
unsigned char *x1=(unsigned char*)&nHangle;
unsigned char *y1=(unsigned char*)&nVangle;
tmpchar[0] = *(x1);
tmpchar[1] = *(x1+1);
tmpchar[2] = *(x1+2);
tmpchar[3] = *(x1+3);
tmpchar[4] = *y1;
tmpchar[5] = *(y1+1);
tmpchar[6] = *(y1+2);
tmpchar[7] = *(y1+3);
tmpchar[8] = CheckSum(tmpchar,8);
WriteData(tmpchar, 9);
return 0;
}
bool PortOperate::SendAngleLocation(short int nHangle, short int  nVangle, short int  nPixelX, short int  nPixelY)
{
memset(tmpchar, 0x00, sizeof(tmpchar));
tmpchar[0] = 0xAA;
tmpchar[1] = 0x55;
tmpchar[2] = nHangle & 0x00ff;
tmpchar[3] = nHangle >> 8;
tmpchar[4] = nVangle & 0x00ff;
tmpchar[5] = nVangle >> 8;
tmpchar[6] = nPixelX & 0x00ff;
tmpchar[7] = nPixelX >> 8;
tmpchar[8] = nPixelY & 0x00ff;
tmpchar[9] = nPixelY >> 8;
//tmpchar[5] = speed;
//tmpchar[6] = speed;
tmpchar[10] = 0xBB;
WriteData(tmpchar, 11);
return 0;
}
bool PortOperate::SendOK()
{
memset(tmpchar, 0x00, sizeof(tmpchar));
tmpchar[0] = 0xAA;
tmpchar[1] = 0x55;
tmpchar[2] = 0x01;
tmpchar[3] = 0xBB;
//tmpchar[4] = nVangle & 0x00ff;
//tmpchar[5] = nVangle >> 8;
//tmpchar[5] = speed;
//tmpchar[6] = speed;
//tmpchar[4] = 0xBB;
WriteData(tmpchar, 4);
return 0;
}