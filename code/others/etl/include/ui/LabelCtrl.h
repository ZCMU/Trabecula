////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

class LabelCtrl : public CWindowImpl<LabelCtrl, CStatic>
{
public:
	LabelCtrl() : m_clrText(RGB(0, 0, 0)), m_clrBack(CLR_INVALID)
	{
	}

	std::shared_ptr<CString> m_spLabel;

	COLORREF m_clrText;
	COLORREF m_clrBack;
	CBrush   m_bshBack;

	std::shared_ptr<CString> GetLabel() throw()
	{
		return m_spLabel;
	}
	void set_Label(const std::shared_ptr<CString>& sp) throw()
	{
		m_spLabel = sp;
	}

	void UpdateText() throw()
	{
		SetWindowText(*m_spLabel);
	}

	void SetColor(COLORREF clr) throw()
	{
		m_clrText = clr;
		if( m_clrBack != clr ) {
			if( !m_bshBack.IsNull() )
				m_bshBack.DeleteObject();
			m_bshBack.CreateSolidBrush(clr);
			m_clrBack = clr;
		}
	}

//------------------------------------------------------------------------------
//message handler
	BEGIN_MSG_MAP(LabelCtrl)
		MESSAGE_HANDLER(OCM_CTLCOLORSTATIC, OnCtlColorStatic)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
//------------------------------------------------------------------------------

	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hDC = (HDC)wParam;
		HWND hWnd = (HWND)lParam;
		
		UINT r,g,b;
		r = (UINT)(GetRValue(m_clrText));
		g = (UINT)(GetGValue(m_clrText));
		b = (UINT)(GetBValue(m_clrText));
		if (r > 150 && g > 150 && b > 150) {
		    SetTextColor(hDC, RGB(0, 0, 0));  // text Color
		} else {
		    SetTextColor(hDC, RGB(255, 255, 255));  // text Color
		}
		SetBkColor(hDC, m_clrText);  // text BkColor

		bHandled = TRUE;

		return (LRESULT)m_bshBack.m_hBrush;
	}
};

////////////////////////////////////////////////////////////////////////////////
