////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#define ICN_PIXEL  (0x100)
#define ICN_CLICK  (0x101)

// 像素信息结构体
struct NMIMAGEPIXEL
{
	NMHDR    nmh;  // Contains information about a notification message.
	int      x, y;
	COLORREF rgb;  // The COLORREF value is used to specify an RGB color.
};

class ImageCtrl : public ATL::CWindowImpl<ImageCtrl, CWindowEx, ATL::CControlWinTraits>,
				public CScrollImpl<ImageCtrl>
{
public:
	DECLARE_WND_CLASS(NULL)

//------------------------------------------------------------------------------
	bool m_bEnter;  //mouse enter

	std::shared_ptr<CImage> m_spImage;  // 图片句柄

	ImageCtrl() throw() : m_bEnter(false)
	{
	}

	std::shared_ptr<CImage> GetImage() throw()
	{
		return m_spImage;
	}
	void set_Image(const std::shared_ptr<CImage>& sp) throw()
	{
		m_spImage = sp;
	}
	bool is_image_null() const throw()
	{
		return m_spImage.get() == NULL || m_spImage->IsNull();
	}

	void UpdateScroll() throw()
	{
		int cx = 1;
		int cy = 1;
		if( !is_image_null() ) {
			cx = m_spImage->GetWidth();
			cy = m_spImage->GetHeight();
		}
		SetScrollSize(cx, cy, TRUE, false); // 设置滚动视图
	}

//------------------------------------------------------------------------------
//message handler
	BEGIN_MSG_MAP(ImageCtrl)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)   // 控件区域背景
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)     // 设置鼠标光标
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)     // 处理鼠标移动
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)   // 处理鼠标离开
        MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)     // 左键抬起
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
		if( iCode == HTCLIENT && !is_image_null() ) {
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
		if( !m_bEnter ) {
			m_bEnter = StartTrackMouseLeave() ? true : false;
		}
		if( is_image_null() )
			return 0;
		NMIMAGEPIXEL nm;
		nm.nmh.code = ICN_PIXEL;
		nm.nmh.idFrom = GetDlgCtrlID();
		nm.nmh.hwndFrom = m_hWnd;
		nm.x = x + pt.x;
		nm.y = y + pt.y;
        // OutputDebugPrintf(_T("pixel %d %d\n"), nm.x, nm.y);   // 鼠标落到图像之外会报错
        if ((nm.x < m_spImage->GetWidth()) &&
            (nm.y < m_spImage->GetHeight()))
        {
            nm.rgb = m_spImage->GetPixel(nm.x, nm.y);
            SendMessage(GetParent(), WM_NOTIFY, nm.nmh.idFrom, (LPARAM)&nm);  // 发送消息
        }
		return 0;
	}
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		NMIMAGEPIXEL nm;
		nm.nmh.code = ICN_PIXEL;
		nm.nmh.idFrom = GetDlgCtrlID();
		nm.nmh.hwndFrom = m_hWnd;
		nm.x = 0;
		nm.y = 0;
		nm.rgb = CLR_INVALID;
		SendMessage(GetParent(), WM_NOTIFY, nm.nmh.idFrom, (LPARAM)&nm);  // 发送消息
		//cancel
		if( CancelTrackMouse(TME_LEAVE) )
			m_bEnter = false;
		return 0;
	}
    LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
		UINT uFlags = (UINT)wParam;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		GetScrollOffset(pt);
		if( !m_bEnter ) {
			m_bEnter = StartTrackMouseLeave() ? true : false;
		}
		if( is_image_null() )
			return 0;
		NMIMAGEPIXEL nm;
		nm.nmh.code = ICN_CLICK;
		nm.nmh.idFrom = GetDlgCtrlID();
		nm.nmh.hwndFrom = m_hWnd;
		nm.x = x + pt.x;
		nm.y = y + pt.y;
        // OutputDebugPrintf(_T("pixel %d %d\n"), nm.x, nm.y);
        if ((nm.x < m_spImage->GetWidth()) &&
            (nm.y < m_spImage->GetHeight()))
        {
            nm.rgb = m_spImage->GetPixel(nm.x, nm.y);  // COLORREF
            // 计算选取点九宫格均值
            SendMessage(GetParent(), WM_NOTIFY, nm.nmh.idFrom, (LPARAM)&nm);  // 发送消息
        }
        return 0;
    }
//------------------------------------------------------------------------------
// Overrideables
	void DoPaint(CDCHandle dc)
	{
		if( !is_image_null() ) {
			int nOldMode = dc.SetStretchBltMode(COLORONCOLOR);  // 设置在指定设备内容中的伸展模式
			m_spImage->Draw(dc, _WTYPES_NS::CRect(0, 0, m_spImage->GetWidth(), m_spImage->GetHeight()));
			dc.SetStretchBltMode(nOldMode);  // 设置在指定设备内容中的伸展模式
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
