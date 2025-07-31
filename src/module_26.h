// ===== if(isLegalRect(trackBox.boundingRect().width, trackBox.boundingRect().height)) =====
{
ellipse( image, trackBox, Scalar(255,0,0), 3, CV_AA );
}
else
{
camshiftWorking = 0;
#ifdef _DEBUG
cout << "lost" << endl;
#endif // _DEBUG
}
}
} // end if !pause
else if( camshiftWorking < 0 )
paused = false;
#ifdef _DEBUG