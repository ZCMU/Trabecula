////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class AddState : public IStateBase
{
public:
	AddState(TWindow* p) throw() : m_pWindow(p)
	{
	}

	//IStateBase methods
	virtual int Process(unsigned int uEvent, const std::any& param)
	{
		if( uEvent == EVT_LEFT_MOUSE_UP ) {
			COLORREF clr = std::any_cast<COLORREF>(param);
			if( clr != CLR_INVALID ) {
				std::array<UINT, 3> rgb;
				rgb[0] = (UINT)(GetRValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				rgb[1] = (UINT)(GetGValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				rgb[2] = (UINT)(GetBValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				m_pWindow->m_cmdShowPixel->SetParameter(std::any(rgb));
				m_pWindow->m_cmdShowPixel->Exec();
				// StartSegment
				CWaitCursor wac;
				m_pWindow->m_cmdStartSegment->SetParameter(std::any(rgb));
				m_pWindow->m_cmdStartSegment->Exec();
			}
		}
		else if( uEvent == EVT_START ) {
			return STATE_START;
		}
		else if( uEvent == EVT_CLEAR ) {
			CWaitCursor wac;
			m_pWindow->m_cmdClearSegment->SetParameter(NULL);
			m_pWindow->m_cmdClearSegment->Exec();
			return STATE_START;
		}
		else if( uEvent == EVT_ERASE ) {
			if( m_pWindow->m_imageCtrlProcess.is_image_null() == false) {
				m_pWindow->m_imageCtrlProcess.SetSelectMode(true);
				return STATE_ERASE;
			}
		}
		else if( uEvent == EVT_REPAIR ) {
			if( m_pWindow->m_imageCtrlProcess.is_image_null() == false) {
				m_pWindow->m_imageCtrlProcess.SetSelectMode(true);
				return STATE_REPAIR;
			}
		}
		return STATE_ADD;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
