////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

class TrabeImageProcessHelper
{
public:
	static void GrayDataErase(GrayData& data, const std::array<INT, 4>& rect)
	{
		if( data.IsNull() )
			return ;
		int iW = data.GetWidth();
		int iH = data.GetHeight();

		uchar* pd = data.GetAddress();

		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				if (j >= rect[0] && j <= rect[2] &&
					i >= rect[1] && i <= rect[3])
				{
					if (pd[i*iW + j] == (uchar)MASK_TRABE) {
						pd[i*iW + j] = 0;
					}
				}
			}
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
