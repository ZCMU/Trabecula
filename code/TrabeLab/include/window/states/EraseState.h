////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class EraseState : public IStateBase
{
public:
	EraseState(TWindow* p) throw() : m_pWindow(p)
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
				m_pWindow->m_cmdErase->SetParameter(std::any(rect));
				m_pWindow->m_cmdErase->Exec();
			}
		}
		else if( uEvent == EVT_ERASE ) {
			//
		}
		else if( uEvent == EVT_START ) {
			m_pWindow->m_imageCtrlProcess.SetSelectMode(false);
			return STATE_START;
		}
		return STATE_ERASE;
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
