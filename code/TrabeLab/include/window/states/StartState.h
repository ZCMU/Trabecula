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
		else if( uEvent == EVT_LOAD ) {
			CFileDialog dlg(TRUE);
			if( dlg.DoModal() == IDOK ) {//弹出对话框
				CWaitCursor wac;
				USES_CONVERSION;
				m_pWindow->m_cmdLoad->SetParameter(std::any(std::string(T2A(dlg.m_szFileName))));
				m_pWindow->m_cmdLoad->Exec();
			}
		}
		else if( uEvent == EVT_START ) {
			if( m_pWindow->m_imageCtrl.GetSelectColor() != CLR_INVALID ) {
				std::array<UINT, 3> rgb;
				rgb[0] = (UINT)(GetRValue(m_pWindow->m_imageCtrl.GetSelectColor()));
				rgb[1] = (UINT)(GetGValue(m_pWindow->m_imageCtrl.GetSelectColor()));
				rgb[2] = (UINT)(GetBValue(m_pWindow->m_imageCtrl.GetSelectColor()));
				CWaitCursor wac;
				m_pWindow->m_cmdStartSegment->SetParameter(std::any(rgb));
				m_pWindow->m_cmdStartSegment->Exec();
			}
		}
		else if( uEvent == EVT_CLEAR ) {
			CWaitCursor wac;
			m_pWindow->m_cmdClearSegment->SetParameter(NULL);
			m_pWindow->m_cmdClearSegment->Exec();
		}
		else if( uEvent == EVT_ERASE ) {
			if( m_pWindow->m_imageCtrl.is_image_null() == false) {
				m_pWindow->m_imageCtrl.SetSelectMode(true);
				return STATE_ERASE;
			}
		}
		return STATE_START;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
