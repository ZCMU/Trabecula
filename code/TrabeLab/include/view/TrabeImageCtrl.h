////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <ui/ImageCtrl.h>

////////////////////////////////////////////////////////////////////////////////

#define ICN_LBTNUP  (0x105)

typedef std::pair<int, int> PAIR;
struct CmpByKey
{
	bool operator()(const PAIR& lhs, const PAIR& rhs)
	{
		return lhs.first < rhs.first;
	}
};
struct CmpByValue
{
	bool operator()(const PAIR& lhs, const PAIR& rhs)
	{
		return lhs.second < rhs.second;
	}
};

class TrabeImageCtrl : public NoFlickerImageCtrlImpl<TrabeImageCtrl>
{
private:
	typedef NoFlickerImageCtrlImpl<TrabeImageCtrl>  baseClass;

public:
	TrabeImageCtrl() : m_bSelectMode(false), m_clrSelected(CLR_INVALID),
						m_bRubberMode(false),
						m_bRulerMode(false)
	{
		m_rcSelect.SetRectEmpty();
		m_rcRulerText.SetRectEmpty();
		m_rcRubber.SetRectEmpty();
	}

	void SetSelectMode(bool bSelect) throw()
	{
		m_bSelectMode = bSelect;
	}

	void SetRubberMode(bool bRubber) throw()
	{
		m_bRubberMode = bRubber;
	}

	void SetRulerMode(bool bRuler) throw()
	{
		m_bRulerMode = bRuler;
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

	std::vector<PAIR> GetRubberTrack() throw()
	{
		return m_track;
	}

private:
	bool  m_bSelectMode;
	bool  m_bDown;
	POINT m_pt1;
	POINT m_pt2;
	POINT m_ptOld;
	CRect m_rcSelect;

	//rubber
	bool m_bRubberMode;
	std::vector<PAIR> m_track;
	CRect m_rcRubber;

	//ruler
	bool m_bRulerMode;
	CString m_strRulerText;
	CRect m_rcRulerText;

	COLORREF m_clrSelected;

private:
	static void _pt_generate_rect(const POINT& pt1, const POINT& pt2, LPRECT lpRect) throw()
	{
		if( pt1.x < pt2.x ) {
			lpRect->left  = pt1.x;
			lpRect->right = pt2.x;
		}
		else {
			lpRect->left  = pt2.x;
			lpRect->right = pt1.x;
		}
		if( pt1.y < pt2.y ) {
			lpRect->top    = pt1.y;
			lpRect->bottom = pt2.y;
		}
		else {
			lpRect->top    = pt2.y;
			lpRect->bottom = pt1.y;
		}
	}
	void generate_rect(LPRECT lpRect) throw()
	{
		_pt_generate_rect(m_pt1, m_pt2, lpRect);
	}
	void generate_rect_old(LPRECT lpRect) throw()
	{
		_pt_generate_rect(m_pt1, m_ptOld, lpRect);
	}

	//ruler's text
	static void _pt_generate_ruler_text(const POINT& pt1, const POINT& pt2, CString& str)
	{
		str.Empty();
		if( pt1.x == pt2.x && pt1.y == pt2.y )
			return ;
		double v = ::sqrt((double)(pt1.x - pt2.x) * (double)(pt1.x - pt2.x)
						+ (double)(pt1.y - pt2.y) * (double)(pt1.y - pt2.y));
		str.Format(_T("%lf"), v);
	}
	void generate_ruler_text()
	{
		_pt_generate_ruler_text(m_pt1, m_pt2, m_strRulerText);
	}

	static void _pt_generate_ruler_text_rect(const POINT& pt1, const POINT& pt2,
											const CString& str,
											const RECT& rcAbsClient, LPRECT lpRect) throw()
	{
		lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
		if( str.IsEmpty() )
			return ;
		CRect rcCenter, rcIntersect;
		_pt_generate_rect(pt1, pt2, &rcCenter);
		CClientDC dc(NULL);
		//top
		lpRect->left = rcCenter.left + 2;
		lpRect->right = lpRect->left + 1;
		lpRect->top = rcCenter.top - 15;
		lpRect->bottom = lpRect->top + 1;
		dc.DrawText((LPCTSTR)str, -1, lpRect, DT_CALCRECT);
		if( rcIntersect.IntersectRect(lpRect, &rcAbsClient)
			&& rcIntersect.Height() > (lpRect->bottom - lpRect->top) * 9 / 10
			&& rcIntersect.Width() > (lpRect->right - lpRect->left) * 9 / 10 )
			return ;
		//left
		lpRect->left = rcCenter.left - 70;
		lpRect->right = lpRect->left + 1;
		lpRect->top = rcCenter.top + 2;
		lpRect->bottom = lpRect->top + 1;
		dc.DrawText((LPCTSTR)str, -1, lpRect, DT_CALCRECT);
		if( rcIntersect.IntersectRect(lpRect, &rcAbsClient)
			&& rcIntersect.Width() > (lpRect->right - lpRect->left) * 9 / 10 )
			return ;
		//right
		lpRect->left = rcCenter.right + 2;
		lpRect->right = lpRect->left + 1;
		lpRect->top = rcCenter.top + 2;
		lpRect->bottom = lpRect->top + 1;
		dc.DrawText((LPCTSTR)str, -1, lpRect, DT_CALCRECT);
		if( rcIntersect.IntersectRect(lpRect, &rcAbsClient)
			&& rcIntersect.Width() > (lpRect->right - lpRect->left) * 9 / 10 )
			return ;
		//bottom
		lpRect->left = rcCenter.left + 2;
		lpRect->right = lpRect->left + 1;
		lpRect->top = rcCenter.bottom + 2;
		lpRect->bottom = lpRect->top + 1;
		dc.DrawText((LPCTSTR)str, -1, lpRect, DT_CALCRECT);
	}
	void generate_ruler_text_rect(const RECT& rcAbsClient) throw()
	{
		_pt_generate_ruler_text_rect(m_pt1, m_pt2, m_strRulerText, rcAbsClient, &m_rcRulerText);
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

		m_track.clear();
		m_rcRubber.SetRectEmpty();

		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT uFlags = (UINT)wParam;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		GetScrollOffset(pt);
		POINT pt1, pt2;

		if( m_bDown ) {
			m_pt2.x = x + pt.x;
			m_pt2.y = y + pt.y;

			if( m_bSelectMode ) {
				CRect rect;
				generate_rect(&rect);
				m_rcSelect = rect;
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
				generate_rect_old(&rect);
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
			}

			//rubber
			if( m_bRubberMode ) {
				m_track.insert(m_track.end(), PAIR(m_pt2.x, m_pt2.y));
				std::vector<PAIR> vec(m_track.begin(), m_track.end());
				sort(vec.begin(), vec.end(), CmpByKey());
				pt1.x = vec[0].first; pt2.x = vec[vec.size()-1].first;
				sort(vec.begin(), vec.end(), CmpByValue());
				pt1.y = vec[0].second; pt2.y = vec[vec.size()-1].second;

				CRect rect;
				_pt_generate_rect(pt1, pt2, rect);
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
			}

			//ruler
			if( m_bRulerMode ) {
				CRect rect;
				generate_rect(&rect);
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
				generate_rect_old(&rect);
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
				//string
				rect = m_rcRulerText;
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
				generate_ruler_text();
				GetClientRect(&rect);
				rect.OffsetRect(pt.x, pt.y);
				generate_ruler_text_rect(rect);
				rect = m_rcRulerText;
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
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
		POINT pt1, pt2;

		if( m_bDown ) {
			m_pt2.x = m_pt1.x;
			m_pt2.y = m_pt1.y;
			if( m_bSelectMode ) {
				CRect rect;
				generate_rect_old(&rect);
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);

				generate_rect(&m_rcSelect);
			}

			//rubber
			if( m_bRubberMode ) {
				if (m_track.size() > 1) {
					std::vector<PAIR> vec(m_track.begin(), m_track.end());
					sort(vec.begin(), vec.end(), CmpByKey());
					pt1.x = vec[0].first; pt2.x = vec[vec.size()-1].first;
					sort(vec.begin(), vec.end(), CmpByValue());
					pt1.y = vec[0].second; pt2.y = vec[vec.size()-1].second;

					CRect rect;
					_pt_generate_rect(pt1, pt2, rect);
					rect.OffsetRect(-pt.x, -pt.y);
					rect.InflateRect(1, 1);
					InvalidateRect(&rect);
				}
			}

			//ruler
			if( m_bRulerMode ) {
				CRect rect;
				generate_rect_old(&rect);
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);
				//string
				rect = m_rcRulerText;
				rect.OffsetRect(-pt.x, -pt.y);
				rect.InflateRect(1, 1);
				InvalidateRect(&rect);

				generate_ruler_text();
				GetClientRect(&rect);
				rect.OffsetRect(pt.x, pt.y);
				generate_ruler_text_rect(rect);
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
	void DoImageCtrlPaint(CMemoryDC& mdc, const _WTYPES_NS::CRect& rcClient)
	{
		if( m_bSelectMode ) {
			_WTYPES_NS::CRect rect(m_rcSelect);
			//rectangle
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
			HPEN hOldPen = mdc.SelectPen(pen);
			mdc.MoveTo(rect.left, rect.top);
			mdc.LineTo(rect.right, rect.top);
			mdc.MoveTo(rect.right - 1, rect.top);
			mdc.LineTo(rect.right - 1, rect.bottom);
			mdc.MoveTo(rect.right - 1, rect.bottom - 1);
			mdc.LineTo(rect.left, rect.bottom - 1);
			mdc.LineTo(rect.left, rect.top);
			mdc.SelectPen(hOldPen);
		}

		//rubber
		if( m_bRubberMode ) {
			if (m_track.size() > 1 && m_bDown) {
				CPen pen;
				pen.CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
				HPEN hOldPen = mdc.SelectPen(pen);
				std::vector<PAIR>::iterator it, it2;
				for ( it = m_track.begin(); it != m_track.end(); it++ ) {
					mdc.MoveTo(it->first, it->second);
					it2 = it; it2 ++;
					if (it2 != m_track.end()) {
						mdc.LineTo(it2->first, it2->second);
					}
				}
				mdc.SelectPen(hOldPen);
			}
		}

		//ruler
		if( m_bRulerMode ) {
			//line
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
			HPEN hOldPen = mdc.SelectPen(pen);
			mdc.MoveTo(m_pt1.x, m_pt1.y);
			mdc.LineTo(m_pt2.x, m_pt2.y);
			int iOldMode = mdc.SetBkMode(TRANSPARENT);
			COLORREF clrOld = mdc.SetTextColor(RGB(255, 255, 0));
			mdc.DrawText((LPCTSTR)m_strRulerText, -1, &m_rcRulerText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
			mdc.SetTextColor(clrOld);
			mdc.SetBkMode(iOldMode);
			mdc.SelectPen(hOldPen);
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
