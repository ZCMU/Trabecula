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
					if (pd[i*iW + j] == (uchar)MASK_TARGET) {
						pd[i*iW + j] = 0;
					}
				}
			}
		}
	}
	static void GrayDataRepair(GrayData& data, const std::array<INT, 4>& rect)
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
					if (pd[i*iW + j] != (uchar)MASK_TARGET) {
						pd[i*iW + j] = (uchar)MASK_TARGET;
					}
				}
			}
		}
	}
	static void TargetDataCalc(GrayData& data, UINT& num)
	{
		if( data.IsNull() )
		{
			num = 0;
			return ;
		}

		int iW = data.GetWidth();
		int iH = data.GetHeight();

		uchar* pd = data.GetAddress();
		num = 0;

		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				if (pd[i*iW + j] == (uchar)MASK_TARGET)
				{
					num ++;
				}
			}
		}
	}
	static void GrayDataFilter(GrayData& data, const std::array<UINT, 1>& quantity)
	{
		if( data.IsNull() )
			return ;

		int iW = data.GetWidth();
		int iH = data.GetHeight();
		uchar* pd = data.GetAddress();

		std::vector<int> matrix;
		std::vector<int> vecArea;
		ImageProcessHelper::Label(data, matrix, vecArea);

		// std::map<int,int>::iterator it;
		// for( it = noise.begin(); it != noise.end(); it ++) {
			// OutputDebugPrintf(_T("%d %d"), it->first, it->second);
		// }

		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				if (vecArea[matrix[i * iW + j]] < quantity[0]) {
					pd[i*iW + j] = (uchar)0;
				}
			}
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
