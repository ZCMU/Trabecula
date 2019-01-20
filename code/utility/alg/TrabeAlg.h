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
	static void GrayDataRubber(GrayData& data, const std::vector<PAIR>& track)
	{
		if( data.IsNull() )
			return ;
		int iW = data.GetWidth();
		int iH = data.GetHeight();

		uchar* pd = data.GetAddress();

		std::vector<PAIR> vec(track.begin(), track.end());
		std::vector<PAIR>::iterator it;
		for ( it = vec.begin(); it != vec.end(); it++ ) {
			if ((it->first > 0 && it->first < iW - 1) && 
				(it->second > 0 && it->second < iH - 1))
			{
				pd[(it->second-1)*iW + (it->first)] = 0;
				pd[(it->second-1)*iW + (it->first-1)] = 0;
				pd[(it->second)*iW   + (it->first-1)] = 0;
				pd[it->second*iW + it->first] = 0;
				pd[(it->second+1)*iW + (it->first)] = 0;
				pd[(it->second+1)*iW + (it->first+1)] = 0;
				pd[(it->second)*iW   + (it->first+1)] = 0;
				pd[(it->second+1)*iW + (it->first-1)] = 0;
				pd[(it->second-1)*iW + (it->first+1)] = 0;
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
		std::map<int, int> noise;
		{
			std::vector<int> vecArea;
			ImageProcessHelper::Label(data, matrix, vecArea);
			for( int i = 0; i < (int)vecArea.size(); i ++ ) {
				if( vecArea[i] < (int)quantity[0] )
					noise.insert(std::pair<int, int>(i, vecArea[i]));
			}
		} //end block

		// std::map<int,int>::iterator it;
		// for( it = noise.begin(); it != noise.end(); it ++) {
			// OutputDebugPrintf(_T("%d %d"), it->first, it->second);
		// }

		for( int i = 0; i < iH; i ++ ) {
			for( int j = 0; j < iW; j ++ ) {
				if (noise.count(matrix[i * iW + j]) == 1) {
					pd[i*iW + j] = (uchar)0;
				}
			}
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
