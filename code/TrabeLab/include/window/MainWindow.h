////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <view\TrabeImageCtrl.h>
#include <imp\ImageData.h>

#include "sinks\MainWindowPropertySink.h"
#include "sinks\MainWindowCommandSink.h"

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS(_T("Trabe Window Class"))

	//--------------------------------------------------------------------------
	enum { IDC_BTN_LOAD = 10, 
           IDC_BTN_CALC,
           IDC_BTN_CLEAR,
           IDC_TEXT_PIXEL,
           IDC_NOW_COLOR,
           IDC_PIC };

	CButton         m_btnLoad;
    CButton         m_btnCalc;
    CButton         m_btnClear;
	TrabeImageCtrl  m_imageCtrl;
	CStatic         m_nowPixel;
    CStatic         m_nowColor;
    CBrush          m_nowBkgnd;
    CStatic         m_pickPixel;
    CStatic         m_pickColor;
    NMIMAGEPIXEL    m_pickPNM;

	//for binding
	std::shared_ptr<ICommandBase>  m_cmdLoad;
    std::shared_ptr<ICommandBase>  m_cmdCalc;
    std::shared_ptr<ICommandBase>  m_cmdClear;
	std::shared_ptr<MainWindowPropertySink<MainWindow>>  m_sinkProperty;
	std::shared_ptr<MainWindowCommandSink<MainWindow>>  m_sinkCommand;

	void set_LoadCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdLoad = sp;
	}
	void set_CalcCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdCalc = sp;
	}
	void set_ClearCommand(const std::shared_ptr<ICommandBase>& sp) throw()
	{
		m_cmdClear = sp;
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
        COMMAND_HANDLER(IDC_BTN_CALC, BN_CLICKED, OnBtnCalcClicked)
        COMMAND_HANDLER(IDC_BTN_CLEAR, BN_CLICKED, OnBtnClearClicked)
		NOTIFY_HANDLER(IDC_PIC, ICN_PIXEL, OnImageCtrlPixel)
        NOTIFY_HANDLER(IDC_PIC, ICN_CLICK, OnImageBtnClicked)
//------------------------------------------------------------------------------
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//----------------------------------------------------------------------
		m_btnLoad.Create(m_hWnd, rcDefault, _T("Load"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_LOAD);
		m_btnCalc.Create(m_hWnd, rcDefault, _T("Calc"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_CALC);
		m_btnClear.Create(m_hWnd, rcDefault, _T("Clear"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_CLEAR);
		m_nowPixel.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PIXEL);
		m_nowColor.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_NOW_COLOR);
		m_pickPixel.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PIXEL);
		m_pickColor.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_NOW_COLOR);
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
        if (m_nowBkgnd.m_hBrush != NULL)
            m_nowBkgnd.DeleteObject();
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
            m_btnCalc.SetWindowPos(NULL, x + 70, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
            m_btnClear.SetWindowPos(NULL, x + 140, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);

            m_pickPixel.SetWindowPos(NULL, x + 210, y, 160, 40, SWP_NOACTIVATE | SWP_NOZORDER);
            m_pickColor.SetWindowPos(NULL, x + 370, y, 40, 40, SWP_NOACTIVATE | SWP_NOZORDER);

			y += (40 + 10);
			m_nowPixel.SetWindowPos(NULL, x, y, 60, /*90*/140, SWP_NOACTIVATE | SWP_NOZORDER);
            m_nowColor.SetWindowPos(NULL, x, y + 160, 60, 60, SWP_NOACTIVATE | SWP_NOZORDER);
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
        if (hWnd == m_nowColor.m_hWnd) {
            if (m_nowBkgnd.m_hBrush == NULL)
                m_nowBkgnd.CreateSolidBrush(RGB(255, 255, 255));
            return (LRESULT)m_nowBkgnd.m_hBrush;
        }
        if (hWnd == m_pickColor.m_hWnd) {
            if (m_nowBkgnd.m_hBrush == NULL)
                m_nowBkgnd.CreateSolidBrush(RGB(255, 255, 255));
            return (LRESULT)m_nowBkgnd.m_hBrush;
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
	LRESULT OnBtnCalcClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
        UINT r,g,b;
        r = (UINT)(GetRValue(m_pickPNM.rgb));
        g = (UINT)(GetGValue(m_pickPNM.rgb));
        b = (UINT)(GetBValue(m_pickPNM.rgb));
        if (r !=0 || g != 0 || b != 0) {
            CWaitCursor wac;
            m_cmdCalc->SetParameter(std::any(m_pickPNM));
            m_cmdCalc->Exec();
        }
		return 0;
	}
	//-------------------------------------------------------------------------- Clear
	LRESULT OnBtnClearClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
        UINT r,g,b;
        r = (UINT)(GetRValue(m_pickPNM.rgb));
        g = (UINT)(GetGValue(m_pickPNM.rgb));
        b = (UINT)(GetBValue(m_pickPNM.rgb));
        if (r !=0 || g != 0 || b != 0) {
            CWaitCursor wac;
            m_cmdClear->SetParameter(std::any(m_pickPNM));
            m_cmdClear->Exec();
        }
		return 0;
	}
    //-------------------------------------------------------------------------- Move
	LRESULT OnImageCtrlPixel(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
	{
		NMIMAGEPIXEL* pnm = (NMIMAGEPIXEL*)pNMHDR;
		if( pnm->rgb == CLR_INVALID ) {
			m_nowPixel.SetWindowText(_T(""));
            if (m_nowBkgnd.m_hBrush != NULL)
                m_nowBkgnd.DeleteObject();
            m_nowBkgnd.CreateSolidBrush(RGB(255, 255, 255));
            m_nowColor.SetWindowText(_T(""));
		}
		else {
			CString str;
            UINT r,g,b;
            float h,s,v;
            r = (UINT)(GetRValue(pnm->rgb));
            g = (UINT)(GetGValue(pnm->rgb));
            b = (UINT)(GetBValue(pnm->rgb));
            /* HSV */
            ImageDataHelper::Rgb2Hsv((float)r/255, (float)g/255, (float)b/255, h, s, v);
			str.Format(_T("X: %d\r\nY: %d\r\nR: %u\r\nG: %u\r\nB: %u\r\nH: %4.1f\r\nS: %4.2f\r\nV: %4.2f"),
				pnm->x, pnm->y, r, g, b, h, s, v
				);
			m_nowPixel.SetWindowText(str);
            if (m_nowBkgnd.m_hBrush != NULL)
                m_nowBkgnd.DeleteObject();
            m_nowBkgnd.CreateSolidBrush(RGB(r, g, b));
            m_nowColor.SetWindowText(_T(""));
		}
		return 0;
	}
	//--------------------------------------------------------------------------
	LRESULT OnImageBtnClicked(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
	{
		NMIMAGEPIXEL* pnm = (NMIMAGEPIXEL*)pNMHDR;
        memcpy(&m_pickPNM, pNMHDR, sizeof(NMIMAGEPIXEL));
		if( pnm->rgb == CLR_INVALID ) {
			m_pickPixel.SetWindowText(_T(""));
            if (m_nowBkgnd.m_hBrush != NULL)
                m_nowBkgnd.DeleteObject();
            m_nowBkgnd.CreateSolidBrush(RGB(255, 255, 255));
            m_pickColor.SetWindowText(_T(""));
		}
		else {
			CString str;
            UINT r,g,b;
            float h,s,v;
            r = (UINT)(GetRValue(pnm->rgb));
            g = (UINT)(GetGValue(pnm->rgb));
            b = (UINT)(GetBValue(pnm->rgb));
            /* HSV */
            ImageDataHelper::Rgb2Hsv((float)r/255, (float)g/255, (float)b/255, h, s, v);
			str.Format(_T("X: %d Y: %d R: %u G: %u B: %u H: %4.1f S: %4.2f V: %4.2f\r\n"),
				pnm->x, pnm->y, r, g, b, h, s, v
				);
            CString str2;
			str2.Format(_T("R: %u G: %u B: %u\r\nH: %4.1f S: %4.2f V: %4.2f\r\n"),
				r, g, b, h, s, v
				);
            m_pickPixel.SetWindowText(str2);
            if (m_nowBkgnd.m_hBrush != NULL)
                m_nowBkgnd.DeleteObject();
            m_nowBkgnd.CreateSolidBrush(RGB(r, g, b));
            m_pickColor.SetWindowText(_T(""));
		}
		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
