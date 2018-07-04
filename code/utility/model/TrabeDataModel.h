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
	const GrayData& get_GrayData() const throw()
	{
		return m_gData;
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
		ImageDataHelper::ImageToColorData(image, m_cData);  // 图像到彩色像素，只是引用 m_cData，在函数内部分配空间
        ImageDataHelper::ImageToColorData(image, m_cDataS);  // 备份
        // ImageDataHelper::ColorDataToGrayData(m_cData, m_gData);

		//notify
		Fire_OnPropertyChanged(std::string("color_data"));  // 属性改变，通知显示（get_ColorData数据） -> ViewModel
        // Fire_OnPropertyChanged(std::string("gray_data"));  // 属性改变，通知显示（get_GrayData数据） -> ViewModel
		return true;
	}
    
    //calc
    bool Calc(NMIMAGEPIXEL* pick)
    {
        NMIMAGEPIXEL *pnm = pick;
        CString str;
        UINT r,g,b;
        float h,s,v;
        r = (UINT)(GetRValue(pnm->rgb));
        g = (UINT)(GetGValue(pnm->rgb));
        b = (UINT)(GetBValue(pnm->rgb));
        /* 计算 HSV */
        ImageDataHelper::Rgb2Hsv((float)r/255, (float)g/255, (float)b/255, h, s, v);
        str.Format(_T("X: %d Y: %d R: %u G: %u B: %u H: %4.1f S: %4.2f V: %4.2f\r\n"),
            pnm->x, pnm->y, r, g, b, h, s, v);
        OutputDebugPrintf(str);

        // HSV阈值算法 H(355,12) V(0.85,1)
        HSVPIXEL min,max;
        // min.h = (float)355; min.s = (float)0.4; min.v = (float)0.85;
        // max.h = (float)20; max.s = (float)0.8; max.v = (float)1;
        
        if (h >= Threshold_H) {
            min.h = h-Threshold_H;
            max.h = h+Threshold_H;
            if (max.h > 360) {
                max.h -= 360;
            }
        } else {
            min.h = 360-(Threshold_H-h);
            max.h = h+Threshold_H;
        }
        
        if (s >= Threshold_S) {
            min.s = s-Threshold_S;
            max.s = s+Threshold_S;
            if (max.s > 1) {
                max.s = 1;
            }
        } else {
            min.s = 0;
            max.h = s+Threshold_S;
        }
        
        if (v >= Threshold_V) {
            min.v = v-Threshold_V;
            max.v = v+Threshold_V;
            if (max.v > 1) {
                max.v = 1;
            }
        } else {
            min.v = 0;
            max.h = v+Threshold_V;
        }
        
        CString str2;
        str2.Format(_T("min.h: %4.1f max.h: %4.1f min.s: %4.2f max.s: %4.2f min.v: %4.2f max.v: %4.2f\r\n"),
            min.h, max.h, min.s, max.s, min.v, max.v);
        OutputDebugPrintf(str2);
        ImageDataHelper::SegmentByHSV(min, max, m_cData, m_gData);
        ImageDataHelper::SegmentByHSV(min, max, m_cData, m_gDataS);
        // 腐蚀
        ImageProcessHelper::Erode(m_gData, m_gDataErode);
        // 膨胀
        ImageProcessHelper::Dilate(m_gDataErode, m_gDataDilate);
        // 连通域标记
        // 获取边框
        // 标记边框
        
        // 加权相加
        HSVPIXEL hsvm;
        RGBPIXEL rgbm;
        float mR,mG,mB;
        hsvm.h = h+120;
        if (hsvm.h > 360) {
            hsvm.h -= 360;
        }
        hsvm.s = s+(float)0.5;
        if (hsvm.s > 1) {
            hsvm.s -= 1;
        }
        hsvm.v = v+(float)0.5;
        if (hsvm.v > 1) {
            hsvm.v -= 1;
        }
        ImageDataHelper::Hsv2Rgb(hsvm.h, hsvm.s, hsvm.v, mR, mG, mB);
        rgbm.r = (UINT)mR;
        rgbm.g = (UINT)mG;
        rgbm.b = (UINT)mB;
        ImageDataHelper::ColorDataAddGrayData(m_cData, m_gDataDilate, rgbm, m_cDataAdd);

        // ImageDataHelper::GrayDataToGrayData(m_gDataDilate,m_gData);
        // Fire_OnPropertyChanged(std::string("gray_data"));  // 属性改变，通知显示（get_GrayData数据） -> ViewModel
        
        ImageDataHelper::ColorDataToColorData(m_cDataAdd, m_cData);
        Fire_OnPropertyChanged(std::string("color_data"));  // 属性改变，通知显示 -> ViewModel
        return true;
    }
    
    // clear
    bool Clear()
    {
        ImageDataHelper::ColorDataToColorData(m_cDataS, m_cData);
        Fire_OnPropertyChanged(std::string("color_data"));  // 属性改变，通知显示 -> ViewModel
        return true;
    }

private:
	ColorData m_cData;
    ColorData m_cDataS;  // 源彩色数据，备份
    ColorData m_cDataAdd;
    GrayData m_gData;
    GrayData m_gDataS;
    GrayData m_gDataErode;
    GrayData m_gDataDilate;
};

////////////////////////////////////////////////////////////////////////////////
