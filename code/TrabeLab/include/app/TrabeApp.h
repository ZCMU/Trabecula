////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

class TrabeApp
{
public:
	TrabeApp(CAppModule& am) throw() : m_module(am)
	{
	}
	~TrabeApp() throw()
	{
		Term();
	}
	HRESULT Init(HINSTANCE hInstance)
	{
		HRESULT hRes = m_module.Init(NULL, hInstance);
		m_module.AddMessageLoop(&m_theLoop);
		//----------------------------------------------------------------------
		/*
		CMainFrame wndMain;

		if(wndMain.CreateEx() == NULL)
		{
			ATLTRACE(_T("Main window creation failed!\n"));
			return 0;
		}
		*/
		//----------------------------------------------------------------------
		return hRes;
	}
	void Term() throw()
	{
		m_module.Term();
	}

	int Run(int nCmdShow = SW_SHOWDEFAULT)
	{
		//----------------------------------------------------------------------
		//wndMain.ShowWindow(nCmdShow);
		//----------------------------------------------------------------------
		int nRet = m_theLoop.Run();
		m_module.RemoveMessageLoop();
		return nRet;
	}

private:
	CAppModule    m_module;
	CMessageLoop  m_theLoop;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////
