////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <atldlgs.h>
#include <atlctrls.h>

#include "window/MainWindow.h"

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
		if( m_wndMain.Create(NULL, CWindow::rcDefault, _T("Trabecula Lab")) == NULL ) {
			ATLTRACE(_T("Main window creation failed!\n"));
			return 0;
		}
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
		m_wndMain.ShowWindow(nCmdShow);
		//----------------------------------------------------------------------
		int nRet = m_theLoop.Run();
		m_module.RemoveMessageLoop();
		return nRet;
	}

private:
	CAppModule    m_module;
	CMessageLoop  m_theLoop;
//------------------------------------------------------------------------------
	MainWindow  m_wndMain;
//------------------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////
