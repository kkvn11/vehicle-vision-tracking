// ===== WaitForSingleObject( ov.hEvent, INFINITE ); =====
GetOverlappedResult( A_hCom, &ov, &dwRead, TRUE );
}
//if( dwRead != ulen )
//{
//	cout << "Failed to get all the data." << endl;
//	return 0;
//}
//cout << "Read: " << dwRead << endl;
return 1;
}
int PortOperate::WriteData(unsigned char *pBuffer, unsigned char uLen)
{
// 写入数据到串口
DWORD dwWritten;
OVERLAPPED ov;
memset( &ov, 0, sizeof( ov ) );