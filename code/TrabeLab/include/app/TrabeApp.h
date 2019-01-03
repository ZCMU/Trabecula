////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <atldlgs.h>
#include <atlctrls.h>
#include <atlctrlx.h>

#include <atlscrl.h>

#include "window/MainWindow.h"
#include "viewmodel/TrabeViewModel.h"

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
		//objects
		m_wndMain.Initialize();
		std::shared_ptr<TrabeDataModel> spModel(std::make_shared<TrabeDataModel>());
		m_spViewModel = std::make_shared<TrabeViewModel>();
		m_spViewModel->SetDataModel(spModel);
		//binding
		// notifications
		spModel->AddPropertyNotification(m_spViewModel->get_Sink());
		// properties
		m_wndMain.m_imageCtrlOriginal.set_Image(m_spViewModel->get_ImageOriginal());
		m_wndMain.m_imageCtrlProcess.set_Image(m_spViewModel->get_ImageProcess());
		m_wndMain.m_labelCtrl.set_Label(m_spViewModel->get_Label());
		m_wndMain.m_labelCtrlMeasure.set_Label(m_spViewModel->get_Measure());
		// commands
		m_wndMain.set_LoadCommand(m_spViewModel->get_LoadCommand());
		m_wndMain.set_SaveCommand(m_spViewModel->get_SaveCommand());
		m_wndMain.set_ShowPixelCommand(m_spViewModel->get_ShowPixelCommand());
		m_wndMain.set_StartSegmentCommand(m_spViewModel->get_StartSegmentCommand());
		m_wndMain.set_ClearSegmentCommand(m_spViewModel->get_ClearSegmentCommand());
		m_wndMain.set_EraseCommand(m_spViewModel->get_EraseCommand());
		m_wndMain.set_RepairCommand(m_spViewModel->get_RepairCommand());
		m_wndMain.set_ErodeCommand(m_spViewModel->get_ErodeCommand());
		m_wndMain.set_DilateCommand(m_spViewModel->get_DilateCommand());
		m_wndMain.set_MeasureCommand(m_spViewModel->get_MeasureCommand());
		m_wndMain.set_FilterCommand(m_spViewModel->get_FilterCommand());
		// notifications
		m_spViewModel->AddPropertyNotification(m_wndMain.get_sinkProperty());
		m_spViewModel->AddCommandNotification(m_wndMain.get_sinkCommand());
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
	CAppModule&   m_module;
	CMessageLoop  m_theLoop;
//------------------------------------------------------------------------------
	MainWindow  m_wndMain;
	std::shared_ptr<TrabeViewModel>  m_spViewModel;
//------------------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////
