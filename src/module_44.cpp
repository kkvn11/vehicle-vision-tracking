// ===== #include "MyComClass.h" =====
#include <iostream>
using namespace std;
int PortOperate::OpenComm(const char* port)
{
A_hCom = CreateFile(port,
GENERIC_READ|GENERIC_WRITE, //允许读和写
0, //独占方式
NULL,
OPEN_EXISTING, //打开而不是创建
FILE_FLAG_OVERLAPPED , //异步方式
NULL);
if(A_hCom == INVALID_HANDLE_VALUE)
{
DWORD dwErr = GetLastError();
//cout << "opencom failed " << dwErr << endl;
return FALSE;
}
else
{
// 打开成功，初始化串口
DCB wdcb = {0};
GetCommState(A_hCom, &wdcb); //读取当前串口参数
wdcb.BaudRate = CBR_115200;         // 波特率
wdcb.ByteSize = 8;                  // 数据位8
wdcb.fBinary  = TRUE;				// 二进制方式
wdcb.fParity  = FALSE;
wdcb.Parity   = NOPARITY;			// 无奇偶校验
wdcb.StopBits = ONESTOPBIT;        //1停止位
//	wdcb.fRtsControl = false;
//	wdcb.fDtrControl = false;
//	wdcb.fOutxCtsFlow = false;
//	wdcb.fOutxDsrFlow = false;
wdcb.XonLim = 2048;
wdcb.XoffLim = 512;
wdcb.EofChar = 0;
// 设置串口参数
SetCommState(A_hCom, &wdcb);
// 设置串口超时参数
COMMTIMEOUTS to =                   // 串口超时控制参数
{
0,                       // 读字符间隔超时时间
100,                              // 读操作时每字符的时间
0,                              // 基本的（额外的）读超时时间
MAXDWORD,                       // 写操作时每字符的时间
10                               // 基本的（额外的）写超时时间
};
SetCommTimeouts(A_hCom, &to);
// 设置串口缓冲队列
SetupComm(A_hCom, 1024, 1024);
// 清空并结束串口当前动作
PurgeComm(A_hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
//cout << "open com done" << endl;
//m_hCom = hCom;						// 保存句柄
}
return 1;
}
int PortOperate::ReadData(unsigned char* pBuffer, unsigned char ulen)
{
// 从串口读取数据
DWORD dwRead;
OVERLAPPED ov;
memset( &ov, 0, sizeof( ov ) );