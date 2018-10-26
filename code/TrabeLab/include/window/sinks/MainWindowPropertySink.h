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
		if( str == "label" ) {
			m_pWindow->m_labelCtrl.SetColor(m_pWindow->m_imageCtrl.m_pkPixel);
			m_pWindow->m_labelCtrl.UpdateText();
		}
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
