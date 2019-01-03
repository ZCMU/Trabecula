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
				rgb[0] = (UINT)(GetRValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				rgb[1] = (UINT)(GetGValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				rgb[2] = (UINT)(GetBValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
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
				if (m_pWindow->m_iLoadOK) {
					return STATE_START;
				} else {
					return STATE_NOPIC;
				}
			}
		}
		else if( uEvent == EVT_START ) {
			if( m_pWindow->m_imageCtrlProcess.GetSelectColor() != CLR_INVALID ) {
				std::array<UINT, 3> rgb;
				rgb[0] = (UINT)(GetRValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				rgb[1] = (UINT)(GetGValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				rgb[2] = (UINT)(GetBValue(m_pWindow->m_imageCtrlProcess.GetSelectColor()));
				CWaitCursor wac;
				m_pWindow->m_cmdStartSegment->SetParameter(std::any(rgb));
				m_pWindow->m_cmdStartSegment->Exec();
				m_pWindow->m_btnStartSegment.EnableWindow(FALSE);
				m_pWindow->m_btnClearSegment.EnableWindow(TRUE);
				m_pWindow->m_btnAdd.EnableWindow(TRUE);
				m_pWindow->m_btnErase.EnableWindow(TRUE);
				m_pWindow->m_btnRepair.EnableWindow(TRUE);
				m_pWindow->m_btnErode.EnableWindow(TRUE);
				m_pWindow->m_btnDilate.EnableWindow(TRUE);
				m_pWindow->m_btnMeasure.EnableWindow(TRUE);
				m_pWindow->m_btnFilter.EnableWindow(TRUE);
				return STATE_MEASURE;
			}
		}
		else if( uEvent == EVT_CLEAR ) {
			//
		}
		else if( uEvent == EVT_ADD ) {
			//
		}
		else if( uEvent == EVT_ERASE ) {
			//
		}
		else if( uEvent == EVT_REPAIR ) {
			//
		}
		return STATE_START;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
