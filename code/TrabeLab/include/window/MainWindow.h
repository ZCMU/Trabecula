﻿////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <view\TrabeImageCtrl.h>
#include <view\TrabeLabelCtrl.h>
#include "sinks\MainWindowPropertySink.h"
#include "sinks\MainWindowCommandSink.h"

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS(_T("Trabe Window Class"))

	//--------------------------------------------------------------------------
	enum { IDC_BTN_LOAD = 10, 
			IDC_BTN_STARTSEGMENT,
			IDC_BTN_CLEARSEGMENT,
			IDC_TEXT_PIXEL,
			IDC_TEXT_PICKPIXEL,
			IDC_PIC };

	CButton		 m_btnLoad;
	CButton		 m_btnStartSegment;
	CButton		 m_btnClearSegment;
	TrabeImageCtrl  m_imageCtrl;
	CStatic		 m_txtPixel;
	TrabeLabelCtrl  m_labelCtrl;

	//for binding
	std::shared_ptr<ICommandBase>  m_cmdLoad;
	std::shared_ptr<ICommandBase>  m_cmdShowPixel;
	std::shared_ptr<ICommandBase>  m_cmdStartSegment;
	std::shared_ptr<ICommandBase>  m_cmdClearSegment;
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
	}
	//--------------------------------------------------------------------------

public:
//message handler
	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)  // Sent when the window background must be erased (for example, when a window is resized).
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
//------------------------------------------------------------------------------
		COMMAND_HANDLER(IDC_BTN_LOAD, BN_CLICKED, OnBtnLoadClicked)
		COMMAND_HANDLER(IDC_BTN_STARTSEGMENT, BN_CLICKED, OnBtnStartSegmentClicked)
		COMMAND_HANDLER(IDC_BTN_CLEARSEGMENT, BN_CLICKED, OnBtnClearSegmentClicked)
		NOTIFY_HANDLER(IDC_PIC, ICN_PIXEL, OnImageCtrlPixel)
		NOTIFY_HANDLER(IDC_PIC, ICN_LBTNUP, OnImageLButtonUp)
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
		m_txtPixel.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PIXEL);
		m_labelCtrl.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PICKPIXEL);
		m_imageCtrl.Create(m_hWnd, rcDefault, NULL,
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_PIC);
		m_imageCtrl.UpdateScroll();
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
			x += (60 + 10);
			m_imageCtrl.SetWindowPos(NULL, x, y, w - x - 10, h - y - 10, SWP_NOACTIVATE | SWP_NOZORDER);
			m_imageCtrl.UpdateScroll();
		}
		//----------------------------------------------------------------------
		bHandled = FALSE;
		return 1;
	}
	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hDC = (HDC)wParam;
		HWND hWnd = (HWND)lParam;
		
		if (hWnd == m_labelCtrl.m_hWnd) {
			UINT r,g,b;
			r = (UINT)(GetRValue(m_imageCtrl.m_pkPixel));
			g = (UINT)(GetGValue(m_imageCtrl.m_pkPixel));
			b = (UINT)(GetBValue(m_imageCtrl.m_pkPixel));
			if (r > 150 && g > 150 && b > 150) {
			    SetTextColor(hDC, RGB(0, 0, 0));  // text Color
			} else {
			    SetTextColor(hDC, RGB(255, 255, 255));  // text Color
			}
			SetBkColor(hDC, RGB(r, g, b));  // text BkColor
			static CBrush bsh;  // label BkColor
			if (bsh.m_hBrush != NULL)
			    bsh.DeleteObject();
			bsh.CreateSolidBrush(RGB(r, g, b));
			#if DEBUG_INFO
			CString str;
			str.Format(_T("OnCtlColorStatic R: %u G: %u B: %u"),
			    r, g, b
			);
			OutputDebugPrintf(str);
			#endif
			return (LRESULT)bsh.m_hBrush;
		}
		return 0;
	}
	//-------------------------------------------------------------------------- Load
	LRESULT OnBtnLoadClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CFileDialog dlg(TRUE);
		if( dlg.DoModal() == IDOK ) {//弹出对话框
			CWaitCursor wac;
			USES_CONVERSION;
			m_cmdLoad->SetParameter(std::any(std::string(T2A(dlg.m_szFileName))));
			m_cmdLoad->Exec();
		}
		return 0;
	}
	//-------------------------------------------------------------------------- Calc
	LRESULT OnBtnStartSegmentClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		UINT r,g,b;
		r = (UINT)(GetRValue(m_imageCtrl.m_pkPixel));
		g = (UINT)(GetGValue(m_imageCtrl.m_pkPixel));
		b = (UINT)(GetBValue(m_imageCtrl.m_pkPixel));
		if (r !=0 || g != 0 || b != 0) {
			CWaitCursor wac;
			std::array<UINT, 3> rgb = {r, g, b};
			m_cmdStartSegment->SetParameter(std::any(rgb));
			m_cmdStartSegment->Exec();
		}
		return 0;
	}
	//-------------------------------------------------------------------------- Clear
	LRESULT OnBtnClearSegmentClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CWaitCursor wac;
		m_cmdClearSegment->SetParameter(NULL);
		m_cmdClearSegment->Exec();
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
		if( pnm->rgb != CLR_INVALID ) {
			std::array<UINT, 3> rgb;
			rgb[0] = (UINT)(GetRValue(m_imageCtrl.m_pkPixel));
			rgb[1] = (UINT)(GetGValue(m_imageCtrl.m_pkPixel));
			rgb[2] = (UINT)(GetBValue(m_imageCtrl.m_pkPixel));
			m_cmdShowPixel->SetParameter(std::any(rgb));
			m_cmdShowPixel->Exec();
		}
		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
