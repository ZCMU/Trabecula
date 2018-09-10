////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

class LabelCtrl : public CStatic
{
public:
	std::shared_ptr<CString> m_spLabel;

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
//------------------------------------------------------------------------------
//message handler
	BEGIN_MSG_MAP(CStatic)
		CHAIN_MSG_MAP(LabelCtrl)
	END_MSG_MAP()
//------------------------------------------------------------------------------

};

////////////////////////////////////////////////////////////////////////////////
