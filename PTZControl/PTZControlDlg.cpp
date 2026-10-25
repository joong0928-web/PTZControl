
	// Copy back and save
	for (int i = 0; i < CWebcamController::NUM_PRESETS; ++i)
	{
		for (int j = 0; j < CPTZControlDlg::NUM_MAX_WEBCAMS; ++j)
		{
			m_strTooltips[j][i] = dlg.m_strTooltip[j][i];
			CString str;
			str.Format(REG_TOOLTIP, i + 1 + j*100);
			theApp.WriteProfileString(REG_WINDOW, str, m_strTooltips[j][i]);
		}
	}

	// Camera control
	GetCurrentWebCam().UseLogitechMotionControl(dlg.m_bLogitechCameraControl!=0);
	theApp.WriteProfileInt(REG_DEVICE, REG_USELOGOTECHMOTIONCONTROL, dlg.m_bLogitechCameraControl);
	GetCurrentWebCam().SetMotorIntervalTimer(dlg.m_iMotorIntervalTimer);
	theApp.WriteProfileInt(REG_DEVICE, REG_USELOGOTECHMOTIONCONTROL, dlg.m_bLogitechCameraControl);

	// Set tooltips again
	SetActiveCam(m_iCurrentWebCam);
}

LRESULT CPTZControlDlg::OnAppCommand(WPARAM wParam, LPARAM lParam)
{
	// Check if we know this camera.
	int iCamera = LOWORD(wParam);
	if (iCamera<0 || iCamera>=m_iNumWebCams)
		return 0;

	// Preset must not exceed
	UINT uiOption = HIWORD(lParam);
	if (LOWORD(lParam)=='M' || LOWORD(lParam)=='S')
	{
		// Memory position. We have one for each preset button. Must not exceed the number of presets.
		if (uiOption>=CWebcamController::NUM_PRESETS)
			return 0;
	}
	else
	{
		// Other operations may use a count. We limit it to 10.
		if (uiOption==0)
			uiOption = 1; // Default is 1 time
		else if (uiOption>10)
			uiOption = 10; // Max 10 times)
	}

	// Save the current camera. We will restore it at the end of this function.
	// And select the one we want to control.
	int iActiveCam = m_iCurrentWebCam;
	SetActiveCam(iCamera);

	// Check the command (lParam)
	//  ZoomIn= +, ZoomOut = -, 
	//  PanLeft = L, PanRight = R, 
	//  TiltUp = U, TiltDown = D, 
	//  Home = H, 
	//  Restore MemoryPos = M (HIWORD = memory position),
	//  Store MemoryPos = S (HIWORD = memory position)
	int iRC = 1;

	switch (LOWORD(lParam))
	{
	case '+':
		while (uiOption--)
			OnBtZoomIn();
		break;
	case '-':
		while (uiOption--)
			OnBtZoomOut();
		break;
	case 'L':
		while (uiOption--)
			OnBtLeft();
		break;
	case 'R':
		while (uiOption--)
			OnBtRight();
		break;
	case 'U':
		while (uiOption--)
			OnBtUp();
		break;
	case 'D':
		while (uiOption--)
			OnBtDown();
		break;
	case 'H':
		OnBtHome();
		break;
	case 'M':
		OnBtPreset(m_btPreset[uiOption].GetDlgCtrlID());
		break;
	case 'S':
		OnBtMemory();
		OnBtPreset(m_btPreset[uiOption].GetDlgCtrlID());
		break;
	default:
		// Unknown command
		iRC = 0;
		break;
	}

	// Restore old camera
	SetActiveCam(iActiveCam);

	return iRC;
}
