// ===== ov.hEvent = CreateEvent( NULL, TRUE, TRUE, NULL ); =====
if (uLen > 0)
{
dwWritten = 0;
if (!WriteFile(A_hCom, pBuffer, uLen, &dwWritten, &ov))
{
if( GetLastError() != ERROR_IO_PENDING )
{
cout << "Write com failed." << endl;
return 0;
}