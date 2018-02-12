////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TWindow>
class MainWindowPropertySink : public IPropertyNotification
{
public:
	MainWindowPropertySink(TWindow* p) throw() : m_pWindow(p)
	{
	}
	~MainWindowPropertySink() throw()
	{
	}

// IPropertyNotification
	virtual void OnPropertyChanged(const std::string& str)
	{
		if( str == "image" ) {
			m_pWindow->m_imageCtrl.UpdateScroll();
		}
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
