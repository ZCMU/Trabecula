////////////////////////////////////////////////////////////////////////////////
#prgama once
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
};

////////////////////////////////////////////////////////////////////////////////
