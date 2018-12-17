////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class MainWindowCommandSink : public ICommandNotification
{
public:
	MainWindowCommandSink(TWindow* p) throw() : m_pWindow(p)
	{
	}
	~MainWindowCommandSink() throw()
	{
	}

// ICommandNotification
	virtual void OnCommandComplete(const std::string& str, bool bOK)
	{
		if( str == "load" ) {
			if( !bOK ) {
				AtlMessageBox(NULL, _T("error load image"), _T("error"), MB_OK);
				m_pWindow->m_iLoadOK = false;
				m_pWindow->m_btnStartSegment.EnableWindow(FALSE);
				m_pWindow->m_btnClearSegment.EnableWindow(FALSE);
				m_pWindow->m_btnAdd.EnableWindow(FALSE);
				m_pWindow->m_btnErase.EnableWindow(FALSE);
				m_pWindow->m_btnRepair.EnableWindow(FALSE);
				m_pWindow->m_btnMeasure.EnableWindow(FALSE);
			} else {
				m_pWindow->m_iLoadOK = true;
				m_pWindow->m_btnStartSegment.EnableWindow(TRUE);
				m_pWindow->m_btnClearSegment.EnableWindow(FALSE);
				m_pWindow->m_btnAdd.EnableWindow(FALSE);
				m_pWindow->m_btnErase.EnableWindow(FALSE);
				m_pWindow->m_btnRepair.EnableWindow(FALSE);
				m_pWindow->m_btnMeasure.EnableWindow(FALSE);
			}
		}
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
