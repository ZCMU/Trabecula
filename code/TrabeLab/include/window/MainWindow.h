////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <view\TrabeImageCtrl.h>
#include <view\TrabeLabelCtrl.h>

#include "sinks\MainWindowPropertySink.h"
#include "sinks\MainWindowCommandSink.h"

#include "states\StateDef.h"
#include "states\StartState.h"
#include "states\EraseState.h"

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS(_T("Trabe Window Class"))

	//--------------------------------------------------------------------------
	enum { IDC_BTN_LOAD = 10, 
			IDC_BTN_STARTSEGMENT,
			IDC_BTN_CLEARSEGMENT,
			IDC_BTN_ERASE,
			IDC_TEXT_PIXEL,
			IDC_TEXT_PICKPIXEL,
			IDC_PIC_ORIGINAL,
			IDC_PIC_PROCESS };

	CButton		 m_btnLoad;
	CButton		 m_btnStartSegment;
	CButton		 m_btnClearSegment;
	CButton		 m_btnErase;
	TrabeImageCtrl  m_imageCtrlOriginal;
	TrabeImageCtrl  m_imageCtrlProcess;
	CStatic		 m_txtPixel;
	TrabeLabelCtrl  m_labelCtrl;

	//for binding
	std::shared_ptr<ICommandBase>  m_cmdLoad;
	std::shared_ptr<ICommandBase>  m_cmdShowPixel;
	std::shared_ptr<ICommandBase>  m_cmdStartSegment;
	std::shared_ptr<ICommandBase>  m_cmdClearSegment;
	std::shared_ptr<ICommandBase>  m_cmdErase;
	std::shared_ptr<MainWindowPropertySink<MainWindow>>  m_sinkProperty;
	std::shared_ptr<MainWindowCommandSink<MainWindow>>  m_sinkCommand;

	void set_LoadCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdLoad = sp;
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
		m_stateMgr.Add(STATE_START, std::static_pointer_cast<IStateBase>(std::make_shared<StartState<MainWindow>>(this)));
		m_stateMgr.Add(STATE_ERASE, std::static_pointer_cast<IStateBase>(std::make_shared<EraseState<MainWindow>>(this)));
		m_stateMgr.SetStartState(STATE_START);
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
		COMMAND_HANDLER(IDC_BTN_STARTSEGMENT, BN_CLICKED, OnBtnStartSegmentClicked)
		COMMAND_HANDLER(IDC_BTN_CLEARSEGMENT, BN_CLICKED, OnBtnClearSegmentClicked)
		COMMAND_HANDLER(IDC_BTN_ERASE, BN_CLICKED, OnBtnEraseClicked)
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
		m_btnStartSegment.Create(m_hWnd, rcDefault, _T("Start Segment"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_STARTSEGMENT);
		m_btnClearSegment.Create(m_hWnd, rcDefault, _T("Clear Segment"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_CLEARSEGMENT);
		m_btnErase.Create(m_hWnd, rcDefault, _T("Erase"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_ERASE);
		m_txtPixel.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PIXEL);
		m_labelCtrl.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PICKPIXEL);
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
			m_btnStartSegment.SetWindowPos(NULL, x + 70, y, 110, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnClearSegment.SetWindowPos(NULL, x + 190, y, 110, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			m_labelCtrl.SetWindowPos(NULL, x + 310, y, 160, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (40 + 10);
			m_txtPixel.SetWindowPos(NULL, x, y, 60, 90, SWP_NOACTIVATE | SWP_NOZORDER);
			m_btnErase.SetWindowPos(NULL, x, y + 110, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			x += (60 + 10);
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
	LRESULT OnBtnEraseClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_stateMgr.Process(EVT_ERASE, NULL);
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
