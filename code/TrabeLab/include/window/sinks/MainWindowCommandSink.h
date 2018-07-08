////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>  // TWindow 为 MainWindow
class MainWindowCommandSink : public ICommandNotification
{
public:
	MainWindowCommandSink(TWindow* p) throw() : m_pWindow(p)
	{
	}
	~MainWindowCommandSink() throw()
	{
	}

// ICommandNotification 载入图片出错
	virtual void OnCommandComplete(const std::string& str, bool bOK)
	{
		if( str == "load" ) {
			if( !bOK ) {
				AtlMessageBox(NULL, _T("error load image"), _T("error"), MB_OK);  
			}
		}
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
