////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////
struct RGBPIXEL
{
    UINT r;
    UINT g;
    UINT b;
};
struct HSVPIXEL
{
    float h;
    float s;
    float v;
};

//灰度数组

class GrayData
{
public:
	GrayData() throw() : m_iW(0), m_iH(0)
	{
	}
	~GrayData() throw()
	{
	}

	void Clear() throw()
	{
		m_buffer.clear();
		m_iW = m_iH = 0;
	}

	bool IsNull() const throw()
	{
		return m_buffer.empty();
	}

	BYTE* GetAddress() throw()
	{
		return m_buffer.data();
	}
	const uchar* GetAddress() const throw()
	{
		return m_buffer.data();
	}
	int GetWidth() const throw()
	{
		return m_iW;
	}
	int GetHeight() const throw()
	{
		return m_iH;
	}

	void Allocate(int iW, int iH)
	{
		assert( iW > 0 && iH > 0 );
		Clear();
		m_buffer.resize(iW * iH);  //overflow?
		m_iW = iW;
		m_iH = iH;
	}

	//copy
	void CopyTo(GrayData& dest)
	{
		if( IsNull() ) {
			dest.Clear();
			return ;
		}
		dest.Allocate(m_iW, m_iH);
		::memcpy(dest.GetAddress(), GetAddress(), m_iW * m_iH * sizeof(uchar));
	}

private:
	std::vector<uchar> m_buffer;  //gray array
	int m_iW, m_iH;//宽度、高度
};

class ColorData
{
public:
	ColorData() throw() : m_iW(0), m_iH(0)
	{
	}
	~ColorData() throw()
	{
	}

	void Clear() throw()
	{
		m_spR.clear();
		m_spG.clear();
		m_spB.clear();
		m_iW = m_iH = 0;
	}

	bool IsNull() const throw()
	{
		return m_spR.empty() || m_spG.empty() || m_spB.empty();
	}

	uchar* GetAddressR() throw()
	{
		return m_spR.data();
	}
	const uchar* GetAddressR() const throw()
	{
		return m_spR.data();
	}
	uchar* GetAddressG() throw()
	{
		return m_spG.data();
	}
	const uchar* GetAddressG() const throw()
	{
		return m_spG.data();
	}
	uchar* GetAddressB() throw()
	{
		return m_spB.data();
	}
	const uchar* GetAddressB() const throw()
	{
		return m_spB.data();
	}
	
	int GetWidth() const throw()
	{
		return m_iW;
	}
	int GetHeight() const throw()
	{
		return m_iH;
	}

	void Allocate(int iW, int iH)
	{
		ATLASSERT( iW > 0 && iH > 0 );
		Clear();
		m_spR.resize(iW * iH);
		m_spG.resize(iW * iH);
		m_spB.resize(iW * iH);
		m_iW = iW;
		m_iH = iH;
	}

private:
	std::vector<uchar> m_spR;
	std::vector<uchar> m_spG;
	std::vector<uchar> m_spB;
	int m_iW, m_iH;
};

// ImageDataHelper

class ImageDataHelper
{
public:
	//CImage->ColorData
	static void ImageToColorData(const CImage& image, ColorData& data)
	{
		data.Clear();
		if( image.IsNull() )
			return ;

		int iBPP = image.GetBPP();//得到单位像素的位数
		assert( iBPP == 8 || iBPP == 24 );  //必须是灰度图或彩色图才转换
		int iW = image.GetWidth();
		int iH = image.GetHeight();
		data.Allocate(iW, iH);  //may throw

		RGBQUAD table[256];//结构数组,调色板，彩色图没有调色板
		if( iBPP == 8 )
			image.GetColorTable(0, 256, table);//得到图像的RGB信息

		BYTE* ps = (BYTE*)image.GetBits();//返回图像数据buffer指针
		uchar* pdR = data.GetAddressR();
		uchar* pdG = data.GetAddressG();
		uchar* pdB = data.GetAddressB();

		for( int i = 0; i < iH; i ++ ) {
			BYTE* psr = ps;
			for( int j = 0; j < iW; j ++ ) {
				if( iBPP == 8 ) {//8位灰度图,每一字节是一个像素灰度级
					BYTE v = *psr ++;
					*pdR ++ = table[v].rgbRed;
					*pdG ++ = table[v].rgbGreen;
					*pdB ++ = table[v].rgbBlue;
				}
				else {//24位彩色,每三字节是一个像素RGB(存储顺序是BGR)
					*pdB ++ = *psr ++;
					*pdG ++ = *psr ++;
					*pdR ++ = *psr ++;
				}
			}
			ps += image.GetPitch();
		} //end for
	}
	//ColorData->CImage
	static void ColorDataToImage(const ColorData& data, CImage& image)
	{
		image.Destroy();
		if( data.IsNull() )
			return ;
		int iW = data.GetWidth();
		int iH = data.GetHeight();
		if( !image.Create(iW, iH, 24) )
			return ;

		const uchar* psR = data.GetAddressR();
		const uchar* psG = data.GetAddressG();
		const uchar* psB = data.GetAddressB();
		BYTE* pd = (BYTE*)image.GetBits();
		for( int i = 0; i < iH; i ++ ) {
			BYTE* pdr = pd;
			for( int j = 0; j < iW; j ++ ) {
				*pdr ++ = *psB ++;
				*pdr ++ = *psG ++;
				*pdr ++ = *psR ++;
			}
			pd += image.GetPitch();
		} //end for
	}
	//GrayData->CImage
	static void GrayDataToImage(const GrayData& data, CImage& image)
	{
		image.Destroy();
		if( data.IsNull() )
			return ;
		int iW = data.GetWidth();
		int iH = data.GetHeight();
		if( !image.Create(iW, iH, 8) )
			return ;

		RGBQUAD table[256];
		for( int i = 0; i < 256; i ++ ) {
			table[i].rgbRed = i;
			table[i].rgbGreen = i;
			table[i].rgbBlue = i;
			table[i].rgbReserved = 0;
		}
		image.SetColorTable(0, 256, table);

		const uchar* ps = data.GetAddress();
		BYTE* pd = (BYTE*)image.GetBits();
		for( int i = 0; i < iH; i ++ ) {
			BYTE* pdr = pd;
			for( int j = 0; j < iW; j ++ ) {
				*pdr ++ = *ps ++;
			}
			pd += image.GetPitch();
		} //end for
	}
    //ColorData->ColorData
	static void ColorDataToColorData(const ColorData& cDataSrc, ColorData& cDataDst)
	{
		cDataDst.Clear();
		if( cDataSrc.IsNull() )
			return ;

		int height = cDataSrc.GetHeight();
		int width = cDataSrc.GetWidth();
		cDataDst.Allocate(width, height);

		const uchar* psR = cDataSrc.GetAddressR();
		const uchar* psG = cDataSrc.GetAddressG();
		const uchar* psB = cDataSrc.GetAddressB();
        uchar* pdR = cDataDst.GetAddressR();
        uchar* pdG = cDataDst.GetAddressG();
        uchar* pdB = cDataDst.GetAddressB();

		for( int i = 0; i < height; i ++ ) {
			for( int j = 0; j < width; j ++ ) {
				uchar dR = (*psR ++);
				uchar dG = (*psG ++);
				uchar dB = (*psB ++);
                (*pdR ++) = dR;
                (*pdG ++) = dG;
                (*pdB ++) = dB;
			}
		}
	}
    //ColorData->ColorData
	static void ColorDataAddGrayData(const ColorData& cData1,
                                     const GrayData& cData2,
                                     RGBPIXEL rgb,
                                     ColorData& cDataDst)
	{
		cDataDst.Clear();
		if( cData1.IsNull() )
			return ;
		if( cData2.IsNull() )
			return ;

		int height = cData1.GetHeight();
		int width = cData1.GetWidth();
		cDataDst.Allocate(width, height);

		const uchar* psR = cData1.GetAddressR();
		const uchar* psG = cData1.GetAddressG();
		const uchar* psB = cData1.GetAddressB();
        const uchar* ps  = cData2.GetAddress();
        uchar* pdR = cDataDst.GetAddressR();
        uchar* pdG = cDataDst.GetAddressG();
        uchar* pdB = cDataDst.GetAddressB();

		for( int i = 0; i < height; i ++ ) {
			for( int j = 0; j < width; j ++ ) {
				float dR = (*psR ++);
				float dG = (*psG ++);
				float dB = (*psB ++);
                if (*ps ++ == 255) {
                    // (*pdR ++) = (uchar)(dR*(float)0.7+(float)rgb.r*(float)0.3);
                    // (*pdG ++) = (uchar)(dG*(float)0.7+(float)rgb.g*(float)0.3);
                    // (*pdB ++) = (uchar)(dB*(float)0.7+(float)rgb.b*(float)0.3);
                    (*pdR ++) = 0;
                    (*pdG ++) = 150;
                    (*pdB ++) = 0;
                } else {
                    (*pdR ++) = (uchar)dR;
                    (*pdG ++) = (uchar)dG;
                    (*pdB ++) = (uchar)dB;
                }
            }
        }
    }

	//ColorData->GrayData
	static void ColorDataToGrayData(const ColorData& cData, GrayData& gData)
	{
		gData.Clear();
		if( cData.IsNull() )
			return ;

		int height = cData.GetHeight();
		int width = cData.GetWidth();
		gData.Allocate(width, height);

		const uchar* psR = cData.GetAddressR();
		const uchar* psG = cData.GetAddressG();
		const uchar* psB = cData.GetAddressB();
		uchar* pd  = gData.GetAddress();

		for( int i = 0; i < height; i ++ ) {
			for( int j = 0; j < width; j ++ ) {
				double dR = (double)(*psR ++);
				double dG = (double)(*psG ++);
				double dB = (double)(*psB ++);
				double v = 0.11 * dR + 0.59 * dG + 0.3 * dB;
				*pd ++ = (uchar)((v > 255.0) ? 255.0 : ((v < 0.0) ? 0.0 : v));
			}
		}
	}
    //GrayData->GrayData
	static void GrayDataToGrayData(const GrayData& gDataSrc, GrayData& gDataDst)
	{
		gDataDst.Clear();
		if( gDataSrc.IsNull() )
			return ;

		int height = gDataSrc.GetHeight();
		int width = gDataSrc.GetWidth();
		gDataDst.Allocate(width, height);

		const uchar* ps = gDataSrc.GetAddress();
		uchar* pd  = gDataDst.GetAddress();

		for( int i = 0; i < height; i ++ ) {
			for( int j = 0; j < width; j ++ ) {
                *pd ++ = *ps ++;
			}
		}
    }
	static void Invert(GrayData& gData) throw()
	{
		uchar* pd = gData.GetAddress();
		int iW = gData.GetWidth();
		int iH = gData.GetHeight();
		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				*pd = 255 - *pd;
				pd ++;
			}
		}
	}
	static void BoolInvert(GrayData& gData) throw()
	{
		uchar* pd = gData.GetAddress();
		int iW = gData.GetWidth();
		int iH = gData.GetHeight();
		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				if( *pd != 0 )
					*pd = 0;
				else
					*pd = 255;
				pd ++;
			}
		}
	}
	//然后对于大于阈值的pixel重新处理，将灰度图变为二值图
	static void ToBinary(int iThreshold, GrayData& gData) throw()
	{
		uchar* pd = gData.GetAddress();
		int height = gData.GetHeight();
		int width = gData.GetWidth();

		for( int i = 0; i < height; i ++ ) {
			for( int j = 0; j < width; j ++ ) {
				int temp = (int)(*pd);

				if( temp >= iThreshold )
					*pd = (uchar)255;
				else
					*pd = (uchar)0;
				pd++;
			}
		}
	}
    // RGB2HSV
    static void Rgb2Hsv(float R, float G, float B, float& H, float& S, float&V)
    {  
        // r,g,b values are from 0 to 1
        // h = [0,360], s = [0,1], v = [0,1]
        // if s == 0, then h = -1 (undefined)
        float min, max, delta, tmp;
        tmp = R>G?G:R;
        min = tmp>B?B:tmp;
        tmp = R>G?R:G;
        max = tmp>B?tmp:B;
        V = max; // v
        delta = max - min;

        if( max != 0 ) {
            S = delta / max; // s
        } else {
            // r = g = b = 0 // s = 0, v is undefined
            S = 0;
            H = 0;
            return;
        }

        if (delta == 0) {
            H = 0;
            return;
        } else if(R == max) {
            if (G >= B) {
                H = (G - B) / delta;     // between yellow & magenta
            } else {
                H = (G - B) / delta + 6;
            }
        } else if( G == max ) {
            H = 2 + ( B - R ) / delta; // between cyan & yellow
        } else if (B == max)  {
            H = 4 + ( R - G ) / delta; // between magenta & cyan
        }
        
        H *= 60; // degrees
    }
    // Hsv2Rgb
    static void Hsv2Rgb(float H, float S, float V, float& R, float& G, float&B)
    {
        int i;
        float f, p, q, t;

        if( S == 0 ) 
        {
            // achromatic (grey)
            R = G = B = V;
            return;
        }

        H /= 60; // sector 0 to 5
        i = (int)floor( H );
        f = H - i; // factorial part of h
        p = V * ( 1 - S );
        q = V * ( 1 - S * f );
        t = V * ( 1 - S * ( 1 - f ) );

        switch( i ) 
        {
        case 0: 
            R = V;
            G = t;
            B = p;
            break;
        case 1:
            R = q;
            G = V;
            B = p;
            break;
        case 2:
            R = p;
            G = V;
            B = t;
            break;
        case 3:
            R = p;
            G = q;
            B = V;
            break;
        case 4:
            R = t;
            G = p;
            B = V;
            break;
        default: // case 5:
            R = V;
            G = p;
            B = q;
            break;
        }
    }
    static void SegmentByHSV(HSVPIXEL min, HSVPIXEL max, ColorData& cData, GrayData& gData) throw()
    {
        gData.Clear();
		if( cData.IsNull() )
			return ;

		int iH = cData.GetHeight();
		int iW = cData.GetWidth();
		gData.Allocate(iW, iH);

		uchar* psR = cData.GetAddressR();
		uchar* psG = cData.GetAddressG();
		uchar* psB = cData.GetAddressB();
        uchar* pd  = gData.GetAddress();

		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				double dR = (double)(*psR);
				double dG = (double)(*psG);
				double dB = (double)(*psB);
                float h,s,v;
                Rgb2Hsv((float)dR/255, (float)dG/255, (float)dB/255, h, s, v);
                if (max.h >= min.h) {
                    if (h >= min.h && h <= max.h &&
                        s >= min.s && s <= max.s &&
                        v >= min.v && v <= max.v)
                    {
                        *pd ++ = (uchar)255;
                    } else {
                        *pd ++ = (uchar)0;
                    }
                } else {
                    if ((h >= max.h || h <= min.h) &&
                        s >= min.s && s <= max.s &&
                        v >= min.v && v <= max.v)
                    {
                        *pd ++ = (uchar)255;
                    } else {
                        *pd ++ = (uchar)0;
                    }
                }
                psR++;
                psG++;
                psB++;
            }
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
