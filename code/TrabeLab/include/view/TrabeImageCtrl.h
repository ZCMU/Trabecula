////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <ui/ImageCtrl.h>

////////////////////////////////////////////////////////////////////////////////

#define ICN_LBTNUP  (0x101)

class TrabeImageCtrl : public ImageCtrl
{
public:

//------------------------------------------------------------------------------
//message handler
	BEGIN_MSG_MAP(TrabeImageCtrl)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		CHAIN_MSG_MAP(ImageCtrl)
	END_MSG_MAP()

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT uFlags = (UINT)wParam;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		GetScrollOffset(pt);
		if( is_image_null() )
			return 0;
		NMIMAGEPIXEL nm;
		nm.nmh.code = ICN_LBTNUP;
		nm.nmh.idFrom = GetDlgCtrlID();
		nm.nmh.hwndFrom = m_hWnd;
		nm.x = x + pt.x;
		nm.y = y + pt.y;
		if ((nm.x < m_spImage->GetWidth()) &&
			(nm.y < m_spImage->GetHeight()))
		{
			nm.rgb = m_spImage->GetPixel(nm.x, nm.y);  // COLORREF
			memcpy(&m_pkPixel, &nm.rgb, sizeof(COLORREF));
			SendMessage(GetParent(), WM_NOTIFY, nm.nmh.idFrom, (LPARAM)&nm);
		}
		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
