////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class NoPicState : public IStateBase
{
public:
	NoPicState(TWindow* p) throw() : m_pWindow(p)
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
		else if( uEvent == EVT_RUBBER ) {
			//
		}
		return STATE_NOPIC;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
