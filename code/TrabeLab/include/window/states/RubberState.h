////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class RubberState : public IStateBase
{
public:
	RubberState(TWindow* p) throw() : m_pWindow(p)
	{
	}

	//IStateBase methods
	virtual int Process(unsigned int uEvent, const std::any& param)
	{
		if( uEvent == EVT_LEFT_MOUSE_UP ) {
			std::array<INT, 4> rect;
			rect[0] = m_pWindow->m_imageCtrlProcess.GetSelectRect().left;    // x1
			rect[1] = m_pWindow->m_imageCtrlProcess.GetSelectRect().top;     // y1
			rect[2] = m_pWindow->m_imageCtrlProcess.GetSelectRect().right;   // x2
			rect[3] = m_pWindow->m_imageCtrlProcess.GetSelectRect().bottom;  // y2

			if (abs(rect[0]-rect[2]) > 3 && abs(rect[1]-rect[3]) > 3) {
				m_pWindow->m_cmdRubber->SetParameter(std::any(rect));
				m_pWindow->m_cmdRubber->Exec();
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
			m_pWindow->m_imageCtrlProcess.SetRubberMode(false);
			return STATE_START;
		}
		else if( uEvent == EVT_ADD ) {
			m_pWindow->m_imageCtrlProcess.SetRubberMode(false);
			return STATE_ADD;
		}
		else if( uEvent == EVT_ERASE ) {
			m_pWindow->m_imageCtrlProcess.SetRubberMode(false);
			m_pWindow->m_imageCtrlProcess.SetSelectMode(true);
			return STATE_ERASE;
		}
		else if( uEvent == EVT_REPAIR ) {
			m_pWindow->m_imageCtrlProcess.SetRubberMode(false);
			m_pWindow->m_imageCtrlProcess.SetSelectMode(true);
			return STATE_REPAIR;
		}
		else if( uEvent == EVT_RUBBER ) {
			//
		}
		else if( uEvent == EVT_RULER ) {
			m_pWindow->m_imageCtrlProcess.SetRubberMode(false);
			m_pWindow->m_imageCtrlProcess.SetRulerMode(true);
			return STATE_RULER;
		}
		
		return STATE_RUBBER;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
