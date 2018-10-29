////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <imp\ImageData.h>
#include <imp\ImageProcess.h>

////////////////////////////////////////////////////////////////////////////////
#define Threshold_H  (float)10  // +-10
#define Threshold_S  (float)0.2 // +-0.2
#define Threshold_V  (float)0.1 // +-0.1

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
	const GrayData& get_MaskData() const throw()
	{
		return m_gMask;
	}
	const GrayData& get_GrayData() const throw()
	{
		return m_gData;
	}
	const PixelData& get_PixelData() const throw()
	{
		return m_pData;
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

	//showpixel
	bool ShowPixel(const std::array<UINT, 3>& pkPixel)
	{
		UINT r,g,b;
		float h=0; float s=0; float v=0;
		r = (UINT)pkPixel[0];
		g = (UINT)pkPixel[1];
		b = (UINT)pkPixel[2];
		ImageDataHelper::Rgb2Hsv((float)r/255, (float)g/255, (float)b/255, h, s, v);
		m_pData.rgb.r = r;
		m_pData.rgb.g = g;
		m_pData.rgb.b = b;
		m_pData.hsv.h = h;
		m_pData.hsv.s = s;
		m_pData.hsv.v = v;

		//notify
		Fire_OnPropertyChanged(std::string("pixel_data"));
		return true;
	}

	//startsegment
	bool StartSegment(const std::array<UINT, 3>& pkPixel)
	{
		RgbPixel rgb;
		HsvPixel hsv;
		rgb.r = pkPixel[0];
		rgb.g = pkPixel[1];
		rgb.b = pkPixel[2];
		/* 计算 HSV */
		ImageDataHelper::Rgb2Hsv((float)rgb.r/255, (float)rgb.g/255, (float)rgb.b/255,
			hsv.h, hsv.s, hsv.v);
		// HSV阈值算法
		HsvPixel hsvMin, hsvMax;
		// hsvMin.h = (float)355; hsvMin.s = (float)0.4; hsvMin.v = (float)0.85;
		// hsvMax.h = (float)20; hsvMax.s = (float)0.8; hsvMax.v = (float)1;
		
		if (hsv.h >= Threshold_H) {
			hsvMin.h = hsv.h-Threshold_H;
			hsvMax.h = hsv.h+Threshold_H;
			if (hsvMax.h > 360) {
				hsvMax.h -= 360;
		 	}
		} else {
			hsvMin.h = 360-(Threshold_H-hsv.h);
			hsvMax.h = hsv.h+Threshold_H;
		}
		
		if (hsv.s >= Threshold_S) {
			hsvMin.s = hsv.s-Threshold_S;
			hsvMax.s = hsv.s+Threshold_S;
			if (hsvMax.s > 1) {
				hsvMax.s = 1;
			}
		} else {
			hsvMin.s = 0;
			hsvMax.h = hsv.s+Threshold_S;
		}
		
		if (hsv.v >= Threshold_V) {
			hsvMin.v = hsv.v-Threshold_V;
			hsvMax.v = hsv.v+Threshold_V;
			if (hsvMax.v > 1) {
				hsvMax.v = 1;
			}
		} else {
			hsvMin.v = 0;
			hsvMax.h = hsv.v+Threshold_V;
		}
		
		ImageDataHelper::SegmentByHSV(hsvMin, hsvMax, m_cData, m_gMask);
		ImageProcessHelper::ExtractBorder(m_gMask);
		// 腐蚀
		// ImageProcessHelper::Erode(m_gData, m_gDataErode);
		// 膨胀
		// ImageProcessHelper::Dilate(m_gDataErode, m_gDataDilate);
		// 连通域标记
		// 获取边框
		// 标记边框
		
		// 加权相加
		// HsvPixel hsvAdd;
		// RgbPixel rgbAdd;
		// float r,g,b;
		// hsvAdd.h = hsv.h+120;
		// if (hsvAdd.h > 360) {
			// hsvAdd.h -= 360;
		// }
		// hsvAdd.s = hsv.s+(float)0.5;
		// if (hsvAdd.s > 1) {
			// hsvAdd.s -= 1;
		// }
		// hsvAdd.v = hsv.v+(float)0.5;
		// if (hsvAdd.v > 1) {
			// hsvAdd.v -= 1;
		// }
		// ImageDataHelper::Hsv2Rgb(hsvAdd.h, hsvAdd.s, hsvAdd.v, r, g, b);

		// rgbAdd.r = (UINT)(r*255);
		// rgbAdd.g = (UINT)(g*255);
		// rgbAdd.b = (UINT)(b*255);
		// ImageDataHelper::ColorDataAddGrayData(m_cData, m_gDataDilate, rgbAdd, m_cDataAdd);

		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

	//clearsegment
	bool ClearSegment()
	{
		Fire_OnPropertyChanged(std::string("color_data"));  // -> ViewModel
		return true;
	}

private:
	ColorData m_cData;
	GrayData m_gMask;  // HSV 分割后的 Mask Data
	// ColorData m_cDataAdd;
	GrayData m_gData;
	// GrayData m_gDataErode;
	// GrayData m_gDataDilate;
	PixelData m_pData;
};

////////////////////////////////////////////////////////////////////////////////
