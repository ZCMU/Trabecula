////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class TrabeViewModelSink : public IPropertyNotification
{
public:
	TrabeViewModelSink(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~TrabeViewModelSink() throw()
	{
	}

// IPropertyNotification
	virtual void OnPropertyChanged(const std::string& str)
	{
		if( str == "color_data" ) {
			m_pVM->ColorDataToImage();
			m_pVM->Fire_OnPropertyChanged(std::string("image"));
		}
		if( str == "color_data_seg" ) {
			m_pVM->ColorDataSegToImage();
			m_pVM->Fire_OnPropertyChanged(std::string("image"));
		}
		if( str == "gray_data" ) {
			m_pVM->GrayDataToImage();
			m_pVM->Fire_OnPropertyChanged(std::string("image"));
		}
		if( str == "pixel_data" ) {
			m_pVM->PixelDataToString();
			m_pVM->Fire_OnPropertyChanged(std::string("label"));
		}
	}

private:
	TViewModel* m_pVM;
};

////////////////////////////////////////////////////////////////////////////////
