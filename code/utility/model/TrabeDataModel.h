////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////
#define Threshold_H  (float)10  // +-10
#define Threshold_S  (float)0.2 // +-0.2
#define Threshold_V  (float)0.1 // +-0.1

#include <imp\ImageColor.h>
#include <imp\ImageData.h>
#include <imp\ImageProcess.h>

#include <alg\TrabeImageData.h>
#include <alg\TrabeAlg.h>

////////////////////////////////////////////////////////////////////////////////

struct RgbPixel
{
	UINT r;
	UINT g;
	UINT b;
};
struct HsvPixel
{
	float h;
	float s;
	float v;
};
struct PixelData
{
	RgbPixel rgb;
	HsvPixel hsv;
};

class TrabeDataModel : public Proxy_PropertyNotification<TrabeDataModel>
{
public:
	TrabeDataModel() throw() : m_MaskNum(0), m_CircNum(0), m_dRatio(0.0)
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
	const UINT get_MaskNum() const throw()
	{
		return m_MaskNum;
	}
	int get_CircNum() const throw()
	{
		return m_CircNum;
	}
	double get_Ratio() const throw()
	{
		return m_dRatio;
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
		ImageColorHelper::Rgb2Hsv((float)r/255, (float)g/255, (float)b/255, h, s, v);
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
	bool StartSegment(void)
	{
		RgbPixel rgb;
		HsvPixel hsv;
		rgb.r = m_pData.rgb.r;
		rgb.g = m_pData.rgb.g;
		rgb.b = m_pData.rgb.b;
		/* 计算 HSV */
		ImageColorHelper::Rgb2Hsv((float)rgb.r/255, (float)rgb.g/255, (float)rgb.b/255,
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
			hsvMax.s = hsv.s+Threshold_S;
		}
		
		if (hsv.v >= Threshold_V) {
			hsvMin.v = hsv.v-Threshold_V;
			hsvMax.v = hsv.v+Threshold_V;
			if (hsvMax.v > 1) {
				hsvMax.v = 1;
			}
		} else {
			hsvMin.v = 0;
			hsvMax.v = hsv.v+Threshold_V;
		}
		
		TrabeImageDataHelper::SegmentByHSV(hsvMin.h, hsvMin.s, hsvMin.v,
										hsvMax.h, hsvMax.s, hsvMax.v,
										m_cData, m_gMask);
		// 腐蚀
		// ImageProcessHelper::Erode(m_gData, m_gDataErode);
		// 膨胀
		// ImageProcessHelper::Dilate(m_gDataErode, m_gDataDilate);
		// 连通域标记
		// 获取边框
		// 标记边框

		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

	//clearsegment
	bool ClearSegment()
	{
		m_gMask.Clear();
		Fire_OnPropertyChanged(std::string("color_data"));  // -> ViewModel
		return true;
	}

	//erase
	bool Erase(const std::array<INT, 4>& rect)
	{
		TrabeImageProcessHelper::GrayDataErase(m_gMask, rect);
		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

	//repair
	bool Repair(const std::array<INT, 4>& rect)
	{
		TrabeImageProcessHelper::GrayDataRepair(m_gMask, rect);
		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

	//rubber
	bool Rubber(const std::vector<PAIR>& track)
	{
		TrabeImageProcessHelper::GrayDataRubber(m_gMask, track);
		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

	//erode
	bool Erode()
	{
		GrayData m_gTemp;
		m_gMask.CopyTo(m_gTemp);
		ImageProcessHelper::ErodeBinary(m_gTemp, MASK_TARGET, m_gMask);
		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

	//dilate
	bool Dilate()
	{
		GrayData m_gTemp;
		m_gMask.CopyTo(m_gTemp);
		ImageProcessHelper::DilateBinary(m_gTemp, MASK_TARGET, m_gMask);
		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

	// measure
	bool Measure()
	{
		TrabeImageProcessHelper::TargetDataCalc(m_gMask, m_MaskNum);
		m_CircNum = ImageProcessHelper::ExtractBorder(MASK_BORDER, m_gMask);
		ImageProcessHelper::CancelBorder(MASK_BORDER, MASK_TARGET, m_gMask);
		m_dRatio = (double)m_MaskNum / ((double)m_gMask.GetWidth() * (double)m_gMask.GetHeight());
		Fire_OnPropertyChanged(std::string("measure_data"));  // -> ViewModel
		return true;
	}

	// filter
	bool Filter(const std::array<UINT, 1>& quantity)
	{
		TrabeImageProcessHelper::GrayDataFilter(m_gMask, quantity);
		Fire_OnPropertyChanged(std::string("color_data_seg"));  // -> ViewModel
		return true;
	}

private:
	ColorData m_cData;
	GrayData m_gMask;  // HSV 分割后的 Mask Data
	GrayData m_gData;
	PixelData m_pData;
	UINT m_MaskNum;
	int  m_CircNum; //周长像素
	double m_dRatio; //骨小梁像素数和图像总像素之比
};

////////////////////////////////////////////////////////////////////////////////
