// ===== //assert( ov.hEvent ); =====
memset(str,0x00,sizeof(str));
if (!ReadFile(A_hCom, pBuffer, ulen, &dwRead, &ov))
{
if( GetLastError() != ERROR_IO_PENDING )
{
//cout << "Read com failed." << endl;
return 0;
}