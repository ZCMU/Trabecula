////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#define MASK_TARGET  (1)
#define MASK_BORDER  (2)

// TrabeImageDataHelper

class TrabeImageDataHelper
{
public:
	//ColorDataAndMask->CImage
	static void ColorDataAndMaskToImage(const ColorData& cData, const GrayData& gData, CImage& image)
	{
		if( cData.IsNull() )
			return ;
		if( gData.IsNull() )
			return ;
		int iW = cData.GetWidth();
		int iH = cData.GetHeight();
		{
			ATL::CImage imgCvt;
			if( !imgCvt.Create(iW, iH, 24) )
				return ;
			image.Destroy();
			image.Attach(imgCvt.Detach());
		} //end block

		const uchar* psR = cData.GetAddressR();
		const uchar* psG = cData.GetAddressG();
		const uchar* psB = cData.GetAddressB();
		const uchar* ps = gData.GetAddress();
		BYTE* pd = (BYTE*)image.GetBits();
		for( int i = 0; i < iH; i ++ ) {
			BYTE* pdr = pd;
			for( int j = 0; j < iW; j ++ ) {
				if (*ps ++ == (uchar)MASK_TARGET) {
					*pdr ++ = (uchar)0;
					*pdr ++ = (uchar)150;
					*pdr ++ = (uchar)0;
				} else {
					*pdr ++ = *psB;
					*pdr ++ = *psG;
					*pdr ++ = *psR;
				}
				psR ++;
				psG ++;
				psB ++;
			}
			pd += image.GetPitch();
		} //end for
	}

	// SegmentByHSV
	static void SegmentByHSV(float h_min, float s_min, float v_min,
							float h_max, float s_max, float v_max,
							const ColorData& cData, GrayData& gData)
	{
		if( cData.IsNull() )
			return ;

		int iH = cData.GetHeight();
		int iW = cData.GetWidth();
		if (iH != gData.GetHeight() || iW != gData.GetWidth())
		{
			gData.Clear();
			gData.Allocate(iW, iH);
		}

		const uchar* psR = cData.GetAddressR();
		const uchar* psG = cData.GetAddressG();
		const uchar* psB = cData.GetAddressB();
		uchar* pd = gData.GetAddress();
		uchar* p0 = pd;

		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				double sR = (double)(*psR);
				double sG = (double)(*psG);
				double sB = (double)(*psB);
				float h,s,v;
				ImageColorHelper::Rgb2Hsv((float)sR/255, (float)sG/255, (float)sB/255, h, s, v);
				if (*(p0 + i*iW + j) == (uchar)MASK_TARGET)
				{
					pd ++;
					psR ++; psG ++; psB ++;
					continue;
				}
				if (h_max >= h_min) {
					if (h >= h_min && h <= h_max &&
						s >= s_min && s <= s_max &&
						v >= v_min && v <= v_max)
					{
						*pd ++ = (uchar)MASK_TARGET;
					} else {
						pd ++;
					}
				} else {
					if ((h >= h_max || h <= h_min) &&
						s >= s_min && s <= s_max &&
						v >= v_min && v <= v_max)
					{
						*pd ++ = (uchar)MASK_TARGET;
					} else {
						pd ++;
					}
				}
				psR ++; psG ++; psB ++;
			}
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
