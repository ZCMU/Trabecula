////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class RulerState : public IStateBase
{
public:
	RulerState(TWindow* p) throw() : m_pWindow(p)
	{
	}

	//IStateBase methods
	virtual int Process(unsigned int uEvent, const std::any& param)
	{
		if( uEvent == EVT_LEFT_MOUSE_UP ) {
			//
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
			//
		}
		else if( uEvent == EVT_CLEAR ) {
			CWaitCursor wac;
			m_pWindow->m_cmdClearSegment->SetParameter(NULL);
			m_pWindow->m_cmdClearSegment->Exec();
			m_pWindow->m_btnStartSegment.EnableWindow(TRUE);
			m_pWindow->m_btnClearSegment.EnableWindow(FALSE);
			m_pWindow->m_btnAdd.EnableWindow(FALSE);
			m_pWindow->m_btnErase.EnableWindow(FALSE);
			m_pWindow->m_btnRepair.EnableWindow(FALSE);
			m_pWindow->m_btnRubber.EnableWindow(FALSE);
			m_pWindow->m_btnErode.EnableWindow(FALSE);
			m_pWindow->m_btnDilate.EnableWindow(FALSE);
			m_pWindow->m_btnMeasure.EnableWindow(FALSE);
			m_pWindow->m_btnRuler.EnableWindow(FALSE);
			m_pWindow->m_btnFilter.EnableWindow(FALSE);
			//change modes!
			m_pWindow->m_imageCtrlProcess.SetRulerMode(false);
			return STATE_START;
		}
		else if( uEvent == EVT_ADD ) {
			m_pWindow->m_imageCtrlProcess.SetRulerMode(false);
			return STATE_ADD;
		}
		else if( uEvent == EVT_ERASE ) {
			m_pWindow->m_imageCtrlProcess.SetRulerMode(false);
			m_pWindow->m_imageCtrlProcess.SetSelectMode(true);
			return STATE_ERASE;
		}
		else if( uEvent == EVT_REPAIR ) {
			m_pWindow->m_imageCtrlProcess.SetRulerMode(false);
			m_pWindow->m_imageCtrlProcess.SetSelectMode(true);
			return STATE_REPAIR;
		}
		else if( uEvent == EVT_RUBBER ) {
			m_pWindow->m_imageCtrlProcess.SetRulerMode(false);
			m_pWindow->m_imageCtrlProcess.SetRubberMode(true);
			return STATE_RUBBER;
		}
		else if( uEvent == EVT_RULER ) {
			//
		}

		return STATE_RULER;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
