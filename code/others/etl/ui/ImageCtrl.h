////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#define ICN_PIXEL  (0x100)

struct NMIMAGEPIXEL
{
	NMHDR    nmh;
	int      x, y;
	COLORREF rgb;
};

class ImageCtrl : public ATL::CWindowImpl<ImageCtrl, ATL::CWindow, ATL::CControlWinTraits>,
				public CScrollImpl<ImageCtrl>
{
public:
	DECLARE_WND_CLASS(NULL)

//------------------------------------------------------------------------------
	CImage m_image;

	CImage& GetImage() throw()
	{
		return m_image;
	}

	void UpdateScroll() throw()
	{
		int cx = 1;
		int cy = 1;
		if( !m_image.IsNull() ) {
			cx = m_image.GetWidth();
			cy = m_image.GetHeight();
		}
		SetScrollSize(cx, cy, TRUE, false);//设置滚动视图的大小
	}

//------------------------------------------------------------------------------
//message handler
	BEGIN_MSG_MAP(ImageCtrl)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		CHAIN_MSG_MAP(CScrollImpl<ImageCtrl>)
	END_MSG_MAP()

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CDCHandle dc((HDC)wParam);
		RECT rcClient;
		GetClientRect(&rcClient);
		CBrush bsh;
		bsh.CreateSolidBrush(RGB(64, 64, 64));
		dc.FillRect(&rcClient, bsh);
		return 1;
	}
	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int iCode = LOWORD(lParam);
		if( iCode == HTCLIENT && !m_image.IsNull() ) {
			::SetCursor(::LoadCursor(NULL, IDC_CROSS));
			bHandled = TRUE;
			return 1;
		}
		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT uFlags = (UINT)wParam;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		GetScrollOffset(pt);
		if( m_image.IsNull() )
			return 0;
		NMIMAGEPIXEL nm;
		nm.nmh.code = ICN_PIXEL;
		nm.nmh.idFrom = GetDlgCtrlID();
		nm.nmh.hwndFrom = m_hWnd;
		nm.x = x + pt.x;
		nm.y = y + pt.y;
		nm.rgb = m_image.GetPixel(nm.x, nm.y);
		SendMessage(GetParent(), WM_NOTIFY, nm.nmh.idFrom, (LPARAM)&nm);
		return 0;
	}

//------------------------------------------------------------------------------
// Overrideables
	void DoPaint(CDCHandle dc)
	{
		if( !m_image.IsNull() ) {
			int nOldMode = dc.SetStretchBltMode(COLORONCOLOR);
			m_image.Draw(dc, _WTYPES_NS::CRect(0, 0, m_image.GetWidth(), m_image.GetHeight()));
			dc.SetStretchBltMode(nOldMode);
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
