////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <imp\ImageData.h>

////////////////////////////////////////////////////////////////////////////////

class TrabeDataModel : public Proxy_PropertyNotification<TrabeDataModel>
{
public:
	TrabeDataModel() throw()
	{
	}
	~TrabeDataModel() throw()
	{
	}

	//properties
	const ColorData& get_ColorData() const throw()
	{
		return m_cData;
	}

	//load
	bool Load(const std::string& strFile)
	{
		ATL::CImage image;
		USES_CONVERSION;
		if( FAILED(image.Load(A2T(strFile.c_str()))) )
			return false;
		int iBPP = image.GetBPP();
		if( iBPP != 8 && iBPP != 24 )
			return false;
		ImageDataHelper::ImageToColorData(image, m_cData);
		//notify
		Fire_OnPropertyChanged(std::string("color_data"));
		return true;
	}

private:
	ColorData m_cData;
};

////////////////////////////////////////////////////////////////////////////////
