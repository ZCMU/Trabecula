////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class MeasureState : public IStateBase
{
public:
	MeasureState(TWindow* p) throw() : m_pWindow(p)
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
			m_pWindow->m_btnMeasure.EnableWindow(FALSE);
			return STATE_START;
		}
		else if( uEvent == EVT_ADD ) {
			if( m_pWindow->m_imageCtrlProcess.is_image_null() == false) {
				return STATE_ADD;
			}
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
		return STATE_MEASURE;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
