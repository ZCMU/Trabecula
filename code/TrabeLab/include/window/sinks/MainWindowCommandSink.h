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
		if( !bOK ) {
			AtlMessageBox(NULL, _T("error load image"), _T("error"), MB_OK);
		}
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
