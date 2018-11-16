////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <ui/ImageCtrl.h>

////////////////////////////////////////////////////////////////////////////////

#define ICN_LBTNUP  (0x101)

class TrabeImageCtrl : public ImageCtrlImpl<TrabeImageCtrl>
{
private:
	typedef ImageCtrlImpl<TrabeImageCtrl>  baseClass;

public:
	TrabeImageCtrl() : m_bSelectMode(false), m_clrSelected(CLR_INVALID)
	{
	}

	void SetSelectMode(bool bSelect) throw()
	{
		m_bSelectMode = bSelect;
	}

	COLORREF GetSelectColor() const throw()
	{
		return m_clrSelected;
	}

	CRect GetSelectRect() throw()
	{
		CRect rect;
		generate_rect_old(&rect);
		return rect;
	}

private:
	bool  m_bSelectMode;
	bool  m_bDown;
	POINT m_pt1;
	POINT m_pt2;
	POINT m_ptOld;

	COLORREF m_clrSelected;

private:
	void generate_rect(LPRECT lpRect) throw()
	{
		if( m_pt1.x < m_pt2.x ) {
			lpRect->left  = m_pt1.x;
			lpRect->right = m_pt2.x;
		}
		else {
			lpRect->left  = m_pt2.x;
			lpRect->right = m_pt1.x;
		}
		if( m_pt1.y < m_pt2.y ) {
			lpRect->top    = m_pt1.y;
			lpRect->bottom = m_pt2.y;
		}
		else {
			lpRect->top    = m_pt2.y;
			lpRect->bottom = m_pt1.y;
		}
	}

	void generate_rect_old(LPRECT lpRect) throw()
	{
		if( m_pt1.x < m_ptOld.x ) {
			lpRect->left  = m_pt1.x;
			lpRect->right = m_ptOld.x;
		}
		else {
			lpRect->left  = m_ptOld.x;
			lpRect->right = m_pt1.x;
		}
		if( m_pt1.y < m_ptOld.y ) {
			lpRect->top    = m_pt1.y;
			lpRect->bottom = m_ptOld.y;
		}
		else {
			lpRect->top    = m_ptOld.y;
			lpRect->bottom = m_pt1.y;
		}
	}

public:
//------------------------------------------------------------------------------
//message handler
	BEGIN_MSG_MAP(TrabeImageCtrl)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( is_image_null() )
			return 0;

		UINT uFlags = (UINT)wParam;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		GetScrollOffset(pt);

		::SetCapture(m_hWnd);
		m_bDown = true;

		m_pt1.x = x + pt.x;
		m_pt1.y = y + pt.y;
		m_pt2 = m_pt1;
		m_ptOld = m_pt1;

		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT uFlags = (UINT)wParam;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		GetScrollOffset(pt);

		if( m_bDown ) {
			m_pt2.x = x + pt.x;
			m_pt2.y = y + pt.y;

			if( m_bSelectMode ) {
				if (abs(m_pt2.x - m_pt1.x) >= abs(m_ptOld.x - m_pt1.x) &&
					abs(m_pt2.y - m_pt1.y) >= abs(m_ptOld.y - m_pt1.y))
				{
					CRect rect;
					generate_rect(&rect);
					rect.OffsetRect(-pt.x, -pt.y);
					InvalidateRect(&rect);
				} else {
					CRect rect;
					generate_rect_old(&rect);
					rect.OffsetRect(-pt.x, -pt.y);
					InvalidateRect(&rect);
				}
			}

			m_ptOld = m_pt2;
		}

		bHandled = FALSE;
		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( is_image_null() )
			return 0;
		
		UINT uFlags = (UINT)wParam;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		GetScrollOffset(pt);

		if( m_bDown ) {
			m_pt2.x = m_pt1.x;
			m_pt2.y = m_pt1.y;
			if( m_bSelectMode ) {
				CRect rect;
				generate_rect_old(&rect);
				rect.OffsetRect(-pt.x, -pt.y);
				InvalidateRect(&rect);
			}
			::ReleaseCapture();
			m_bDown = false;
		}

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
			m_clrSelected = nm.rgb;
			SendMessage(GetParent(), WM_NOTIFY, nm.nmh.idFrom, (LPARAM)&nm);
		}
		return 0;
	}

//------------------------------------------------------------------------------
// Overrideables
	void DoPaint(CDCHandle dc)
	{
		baseClass::DoPaint(dc);

		if( m_bSelectMode ) {
			//rectangle
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
			HPEN hOldPen = dc.SelectPen(pen);
			RECT rect;
			generate_rect(&rect);
			dc.MoveTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.MoveTo(rect.right - 1, rect.top);
			dc.LineTo(rect.right - 1, rect.bottom);
			dc.MoveTo(rect.right - 1, rect.bottom - 1);
			dc.LineTo(rect.left, rect.bottom - 1);
			dc.LineTo(rect.left, rect.top);
			dc.SelectPen(hOldPen);
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
