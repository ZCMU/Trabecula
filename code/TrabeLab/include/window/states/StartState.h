////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class StartState : public IStateBase
{
public:
	StartState(TWindow* p) throw() : m_pWindow(p)
	{
	}

	//IStateBase methods
	virtual int Process(unsigned int uEvent, const std::any& param)
	{
		if( uEvent == EVT_LEFT_MOUSE_UP ) {
			COLORREF clr = std::any_cast<COLORREF>(param);
			if( clr != CLR_INVALID ) {
				std::array<UINT, 3> rgb;
				rgb[0] = (UINT)(GetRValue(m_pWindow->m_imageCtrl.GetSelectColor()));
				rgb[1] = (UINT)(GetGValue(m_pWindow->m_imageCtrl.GetSelectColor()));
				rgb[2] = (UINT)(GetBValue(m_pWindow->m_imageCtrl.GetSelectColor()));
				m_pWindow->m_cmdShowPixel->SetParameter(std::any(rgb));
				m_pWindow->m_cmdShowPixel->Exec();
			}
		}
		else if( uEvent == EVT_ERASE )
			return STATE_ERASE;
		return STATE_START;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
