﻿////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include "..\..\rc\resource.h"

#include <view\TrabeImageCtrl.h>
#include <view\TrabeLabelCtrl.h>

#include "sinks\MainWindowPropertySink.h"
#include "sinks\MainWindowCommandSink.h"

#include "states\StateDef.h"
#include "states\NoPicState.h"
#include "states\StartState.h"
#include "states\EraseState.h"
#include "states\AddState.h"
#include "states\RepairState.h"
#include "states\RubberState.h"
#include "states\MeasureState.h"
#include "states\RulerState.h"

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS(_T("Trabe Window Class"))

	//--------------------------------------------------------------------------
	enum { IDC_BTN_LOAD = 10, 
			IDC_BTN_SAVE,
			IDC_BTN_STARTSEGMENT,
			IDC_BTN_CLEARSEGMENT,
			IDC_BTN_ERASE,
			IDC_BTN_ADD,
			IDC_BTN_REPAIR,
			IDC_BTN_RUBBER,
			IDC_BTN_ERODE,
			IDC_BTN_DILATE,
			IDC_BTN_EDGE,
			IDC_BTN_MEASURE,
			IDC_BTN_RULER,
			IDC_BTN_FILTER,
			IDC_BTN_UNDO,
			IDC_BTN_REDO,
			IDC_TEXT_PIXEL,
			IDC_TEXT_PICKPIXEL,
			IDC_TEXT_MEASURE,
			IDC_PIC_ORIGINAL,
			IDC_PIC_PROCESS,
			IDC_EDIT_THRESHOLD};

	bool m_iLoadOK;
	CButton		 m_btnLoad;
	CButton		 m_btnSave;
	CButton		 m_btnStartSegment;
	CButton		 m_btnClearSegment;
	CButton		 m_btnErase;
	CButton		 m_btnAdd;
	CButton		 m_btnRepair;
	CButton		 m_btnRubber;
	CButton		 m_btnErode;
	CButton		 m_btnDilate;
	CButton		 m_btnEdge;
	CButton		 m_btnMeasure;
	CButton		 m_btnRuler;
	CEdit		 m_threshold;
	CButton		 m_btnFilter;
	CButton		 m_btnUndo;
	CButton		 m_btnRedo;
	TrabeImageCtrl  m_imageCtrlOriginal;
	TrabeImageCtrl  m_imageCtrlProcess;
	CStatic		 m_txtPixel;
	TrabeLabelCtrl  m_labelCtrl;
	TrabeLabelCtrl  m_labelCtrlMeasure;

	//for binding
	std::shared_ptr<ICommandBase>  m_cmdLoad;
	std::shared_ptr<ICommandBase>  m_cmdSave;
	std::shared_ptr<ICommandBase>  m_cmdShowPixel;
	std::shared_ptr<ICommandBase>  m_cmdStartSegment;
	std::shared_ptr<ICommandBase>  m_cmdClearSegment;
	std::shared_ptr<ICommandBase>  m_cmdErase;
	std::shared_ptr<ICommandBase>  m_cmdRepair;
	std::shared_ptr<ICommandBase>  m_cmdRubber;
	std::shared_ptr<ICommandBase>  m_cmdErode;
	std::shared_ptr<ICommandBase>  m_cmdDilate;
	std::shared_ptr<ICommandBase>  m_cmdMeasure;
	std::shared_ptr<ICommandBase>  m_cmdFilter;
	std::shared_ptr<MainWindowPropertySink<MainWindow>>  m_sinkProperty;
	std::shared_ptr<MainWindowCommandSink<MainWindow>>  m_sinkCommand;

	void set_LoadCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdLoad = sp;
	}
	void set_SaveCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdSave = sp;
	}
	void set_ShowPixelCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdShowPixel = sp;
	}
	void set_StartSegmentCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdStartSegment = sp;
	}
	void set_ClearSegmentCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdClearSegment = sp;
	}
	void set_EraseCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdErase = sp;
	}
	void set_RepairCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdRepair = sp;
	}
	void set_RubberCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdRubber = sp;
	}
	void set_ErodeCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdErode = sp;
	}
	void set_DilateCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdDilate = sp;
	}
	void set_MeasureCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdMeasure = sp;
	}
	void set_FilterCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdFilter = sp;
	}
	std::shared_ptr<IPropertyNotification> get_sinkProperty() throw()
	{
		// IPropertyNotification -> OnPropertyChanged
		return std::static_pointer_cast<IPropertyNotification>(m_sinkProperty);
	}
	std::shared_ptr<ICommandNotification> get_sinkCommand() throw()
	{
		// ICommandNotification -> OnCommandComplete
		return std::static_pointer_cast<ICommandNotification>(m_sinkCommand);
	}

	void Initialize()
	{
		m_sinkProperty = std::make_shared<MainWindowPropertySink<MainWindow>>(this);
		m_sinkCommand  = std::make_shared<MainWindowCommandSink<MainWindow>>(this);

		//states
		m_stateMgr.Add(STATE_NOPIC, std::static_pointer_cast<IStateBase>(std::make_shared<NoPicState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_START, std::static_pointer_cast<IStateBase>(std::make_shared<StartState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_ERASE, std::static_pointer_cast<IStateBase>(std::make_shared<EraseState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_ADD, std::static_pointer_cast<IStateBase>(std::make_shared<AddState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_REPAIR, std::static_pointer_cast<IStateBase>(std::make_shared<RepairState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_RUBBER, std::static_pointer_cast<IStateBase>(std::make_shared<RubberState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_MEASURE, std::static_pointer_cast<IStateBase>(std::make_shared<MeasureState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_RULER, std::static_pointer_cast<IStateBase>(std::make_shared<RulerState<MainWindow>>(this)));
		m_stateMgr.SetStartState(STATE_NOPIC);
		m_btnSave.EnableWindow(FALSE);
		m_btnStartSegment.EnableWindow(FALSE);
		m_btnClearSegment.EnableWindow(FALSE);
		m_btnEdge.EnableWindow(FALSE);
		m_btnAdd.EnableWindow(FALSE);
		m_btnErase.EnableWindow(FALSE);
		m_btnRepair.EnableWindow(FALSE);
		m_btnRubber.EnableWindow(FALSE);
		m_btnErode.EnableWindow(FALSE);
		m_btnDilate.EnableWindow(FALSE);
		m_btnMeasure.EnableWindow(FALSE);
		m_btnRuler.EnableWindow(FALSE);
		m_btnFilter.EnableWindow(FALSE);
		m_btnUndo.EnableWindow(FALSE);
		m_btnRedo.EnableWindow(FALSE);
	}

private:
	StateManager m_stateMgr;
	//--------------------------------------------------------------------------

public:
//message handler
	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)  // Sent when the window background must be erased (for example, when a window is resized).
		MESSAGE_HANDLER(WM_SIZE, OnSize)
//------------------------------------------------------------------------------
		COMMAND_HANDLER(IDC_BTN_LOAD, BN_CLICKED, OnBtnLoadClicked)
		COMMAND_HANDLER(IDC_BTN_SAVE, BN_CLICKED, OnBtnSaveClicked)
		COMMAND_HANDLER(IDC_BTN_STARTSEGMENT, BN_CLICKED, OnBtnStartSegmentClicked)
		COMMAND_HANDLER(IDC_BTN_CLEARSEGMENT, BN_CLICKED, OnBtnClearSegmentClicked)
		COMMAND_HANDLER(IDC_BTN_ERASE, BN_CLICKED, OnBtnEraseClicked)
		COMMAND_HANDLER(IDC_BTN_ADD, BN_CLICKED, OnBtnAddClicked)
		COMMAND_HANDLER(IDC_BTN_REPAIR, BN_CLICKED, OnBtnRepairClicked)
		COMMAND_HANDLER(IDC_BTN_RUBBER, BN_CLICKED, OnBtnRubberClicked)
		COMMAND_HANDLER(IDC_BTN_ERODE, BN_CLICKED, OnBtnErodeClicked)
		COMMAND_HANDLER(IDC_BTN_DILATE, BN_CLICKED, OnBtnDilateClicked)
		COMMAND_HANDLER(IDC_BTN_EDGE, BN_CLICKED, OnBtnEdgeClicked)
		COMMAND_HANDLER(IDC_BTN_MEASURE, BN_CLICKED, OnBtnMeasureClicked)
		COMMAND_HANDLER(IDC_BTN_RULER, BN_CLICKED, OnBtnRulerClicked)
		COMMAND_HANDLER(IDC_BTN_FILTER, BN_CLICKED, OnBtnFilterClicked)
		NOTIFY_HANDLER(IDC_PIC_PROCESS, ICN_PIXEL, OnImageCtrlPixel)
		NOTIFY_HANDLER(IDC_PIC_PROCESS, ICN_LBTNUP, OnImageLButtonUp)
		NOTIFY_HANDLER(IDC_PIC_ORIGINAL, ICN_SCROLL, OnImageOriginalScroll)
		NOTIFY_HANDLER(IDC_PIC_PROCESS, ICN_SCROLL, OnImageProcessScroll)
//------------------------------------------------------------------------------
		REFLECT_NOTIFICATIONS()
//------------------------------------------------------------------------------
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//----------------------------------------------------------------------
		m_btnLoad.Create(m_hWnd, rcDefault, _T("Load"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_LOAD);
		m_btnSave.Create(m_hWnd, rcDefault, _T("Save"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_SAVE);
		m_btnStartSegment.Create(m_hWnd, rcDefault, _T("Start Segment"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_STARTSEGMENT);
		m_btnClearSegment.Create(m_hWnd, rcDefault, _T("Clear Segment"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_CLEARSEGMENT);
		m_btnErase.Create(m_hWnd, rcDefault, _T("Erase"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_ERASE);
		m_btnAdd.Create(m_hWnd, rcDefault, _T("Add"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_ADD);
		m_btnRepair.Create(m_hWnd, rcDefault, _T("Repair"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_REPAIR);
		m_btnRubber.Create(m_hWnd, rcDefault, _T("Rubber"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_RUBBER);
		m_btnErode.Create(m_hWnd, rcDefault, _T("Erode"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_ERODE);
		m_btnDilate.Create(m_hWnd, rcDefault, _T("Dilate"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_DILATE);
		m_btnEdge.Create(m_hWnd, rcDefault, _T("Edge"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_CHECKBOX, 0,
						IDC_BTN_EDGE);
		m_btnMeasure.Create(m_hWnd, rcDefault, _T("Measure"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_MEASURE);
		m_btnRuler.Create(m_hWnd, rcDefault, _T("Ruler"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_RULER);
		m_btnFilter.Create(m_hWnd, rcDefault, _T("Filter"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_FILTER);
		m_btnUndo.Create(m_hWnd, rcDefault, _T("Undo"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_UNDO);
		m_btnRedo.Create(m_hWnd, rcDefault, _T("Redo"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_REDO);
		m_threshold.Create(m_hWnd, rcDefault, _T("20"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | ES_NUMBER, 0,
						IDC_EDIT_THRESHOLD);
		m_txtPixel.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PIXEL);
		m_labelCtrl.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PICKPIXEL);
		m_labelCtrlMeasure.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_MEASURE);
		m_imageCtrlOriginal.Create(m_hWnd, rcDefault, NULL,
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_PIC_ORIGINAL);
		m_imageCtrlOriginal.UpdateScroll();
		m_imageCtrlOriginal.SetSelectMode(false);
		m_imageCtrlProcess.Create(m_hWnd, rcDefault, NULL,
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_PIC_PROCESS);
		m_imageCtrlProcess.UpdateScroll();
		m_imageCtrlProcess.SetSelectMode(false);

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);
		//----------------------------------------------------------------------
		return 0;
	}
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 1;
	}
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0);
		return 0;
	}
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CDCHandle dc((HDC)wParam);
		RECT rcClient;
		GetClientRect(&rcClient);
		CBrush bsh;
		bsh.CreateSolidBrush(RGB(64, 224, 225));
		dc.FillRect(&rcClient, bsh);
		return 1;
		// An application should return nonzero if it erases the background; otherwise, it should return zero.
	}
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//----------------------------------------------------------------------
		if( wParam != SIZE_MINIMIZED ) {
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			int x = 10;
			int y = 10;
			m_btnLoad.SetWindowPos(NULL, x, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnSave.SetWindowPos(NULL, x + 70, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnStartSegment.SetWindowPos(NULL, x + 140, y, 110, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnClearSegment.SetWindowPos(NULL, x + 260, y, 110, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_labelCtrl.SetWindowPos(NULL, x + 380, y, 160, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnUndo.SetWindowPos(NULL, x + 550, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnRedo.SetWindowPos(NULL, x + 620, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnEdge.SetWindowPos(NULL, x + 690, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (40 + 10);
			m_txtPixel.SetWindowPos(NULL, x, y, 60, 90, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (90 + 10 + 5);
			m_btnAdd.SetWindowPos(NULL, x, y, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (30 + 5);
			m_btnErase.SetWindowPos(NULL, x, y, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (30 + 5);
			m_btnRepair.SetWindowPos(NULL, x, y, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (30 + 5);
			m_btnRubber.SetWindowPos(NULL, x, y, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (30 + 5);
			m_btnErode.SetWindowPos(NULL, x, y, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (30 + 5);
			m_btnDilate.SetWindowPos(NULL, x, y, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (30 + 5);
			m_threshold.SetWindowPos(NULL, x, y, 60, 20, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (20);
			m_btnFilter.SetWindowPos(NULL, x, y, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y = h - 10;
			m_btnMeasure.SetWindowPos(NULL, x, y - 30, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			y -= (30 + 5);
			m_labelCtrlMeasure.SetWindowPos(NULL, x, y - 95, 60, 95, SWP_NOACTIVATE | SWP_NOZORDER);
			y -= (95 + 5);
			m_btnRuler.SetWindowPos(NULL, x, y - 30, 60, 30, SWP_NOACTIVATE | SWP_NOZORDER);
			x += (60 + 10);
			y = 10 + 40 + 10;
			m_imageCtrlOriginal.SetWindowPos(NULL, x, y, (w - x)/2 - 10, h - y - 10, SWP_NOACTIVATE | SWP_NOZORDER);
			m_imageCtrlOriginal.UpdateScroll();
			m_imageCtrlProcess.SetWindowPos(NULL, x + (w - x)/2, y, (w - x)/2 - 10, h - y - 10, SWP_NOACTIVATE | SWP_NOZORDER);
			m_imageCtrlProcess.UpdateScroll();
		}
		//----------------------------------------------------------------------
		bHandled = FALSE;
		return 1;
	}
	//-------------------------------------------------------------------------- Load
	LRESULT OnBtnLoadClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_LOAD, NULL);
		return 0;
	}
	LRESULT OnBtnSaveClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString str_jpeg(_T("BMP"));
		CString str_filename(_T("result.bmp"));
		CFileDialog dlg(
			FALSE, (LPCTSTR)(str_jpeg.AllocSysString()), (LPCTSTR)(str_filename.AllocSysString()),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL
		);
		if( dlg.DoModal() == IDOK ) {//弹出对话框
			CWaitCursor wac;
			USES_CONVERSION;
			m_cmdSave->SetParameter(std::any(std::string(T2A(dlg.m_szFileName))));
			m_cmdSave->Exec();
		}
		return 0;
	}
	//-------------------------------------------------------------------------- Calc
	LRESULT OnBtnStartSegmentClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_START, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------- Clear
	LRESULT OnBtnClearSegmentClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_CLEAR, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------- Erase
	LRESULT OnBtnEraseClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_ERASE, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------- Erase
	LRESULT OnBtnAddClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_ADD, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------- Repair
	LRESULT OnBtnRepairClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_REPAIR, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------- Rubber
	LRESULT OnBtnRubberClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_RUBBER, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------- Erode
	LRESULT OnBtnErodeClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CWaitCursor wac;
		m_cmdErode->SetParameter(NULL);
		m_cmdErode->Exec();
		return 0;
	}
	//-------------------------------------------------------------------------- Dilate
	LRESULT OnBtnDilateClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CWaitCursor wac;
		m_cmdDilate->SetParameter(NULL);
		m_cmdDilate->Exec();
		return 0;
	}
	//-------------------------------------------------------------------------- Edge
	LRESULT OnBtnEdgeClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int iCheck = m_btnEdge.GetCheck();
		m_btnEdge.SetCheck(!iCheck);
		return 0;
	}
	//-------------------------------------------------------------------------- Measure
	LRESULT OnBtnMeasureClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CWaitCursor wac;
		m_cmdMeasure->SetParameter(NULL);
		m_cmdMeasure->Exec();
		return 0;
	}
	//-------------------------------------------------------------------------- Ruler
	LRESULT OnBtnRulerClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_RULER, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------- Filter
	LRESULT OnBtnFilterClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CWaitCursor wac;
		CString str;
		m_threshold.GetWindowText(str);
		std::array<UINT, 1> quantity;
		quantity[0] = (UINT)_ttoi(str);
		m_cmdFilter->SetParameter(std::any(quantity));
		m_cmdFilter->Exec();
		return 0;
	}
	//-------------------------------------------------------------------------- Move
	LRESULT OnImageCtrlPixel(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
	{
		NMIMAGEPIXEL* pnm = (NMIMAGEPIXEL*)pNMHDR;
		if( pnm->rgb == CLR_INVALID ) {
			m_txtPixel.SetWindowText(_T(""));
		}
		else {
			CString str;
			UINT r,g,b;
			r = (UINT)(GetRValue(pnm->rgb));
			g = (UINT)(GetGValue(pnm->rgb));
			b = (UINT)(GetBValue(pnm->rgb));
			str.Format(_T("X: %d\r\nY: %d\r\nR: %u\r\nG: %u\r\nB: %u"),
				pnm->x, pnm->y, r, g, b
				);
			m_txtPixel.SetWindowText(str);
		}
		return 0;
	}
	//--------------------------------------------------------------------------
	LRESULT OnImageLButtonUp(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
	{
		NMIMAGEPIXEL* pnm = (NMIMAGEPIXEL*)pNMHDR;
		m_stateMgr.Process(EVT_LEFT_MOUSE_UP, std::any(pnm->rgb));
		return 0;
	}
	//--------------------------------------------------------------------------
	LRESULT OnImageOriginalScroll(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
	{
		NMIMAGESCROLL* pnm = (NMIMAGESCROLL*)pNMHDR;
		m_imageCtrlProcess.SetScrollOffset(pnm->pt);
		return 0;
	}
	//--------------------------------------------------------------------------
	LRESULT OnImageProcessScroll(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
	{
		NMIMAGESCROLL* pnm = (NMIMAGESCROLL*)pNMHDR;
		m_imageCtrlOriginal.SetScrollOffset(pnm->pt);
		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
