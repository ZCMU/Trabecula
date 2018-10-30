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
			COLORREF clr = m_pWindow->m_imageCtrl.GetSelectColor();
			UINT r,g,b;
			r = (UINT)(GetRValue(clr));
			g = (UINT)(GetGValue(clr));
			b = (UINT)(GetBValue(clr));
			COLORREF clrText;
			if (r > 150 && g > 150 && b > 150) {
				clrText = RGB(0, 0, 0);  // text Color
			} else {
				clrText = RGB(255, 255, 255);  // text Color
			}
			m_pWindow->m_labelCtrl.SetColor(clrText, clr);
			m_pWindow->m_labelCtrl.Update();
		}
	}

private:
	TWindow* m_pWindow;
};

////////////////////////////////////////////////////////////////////////////////
