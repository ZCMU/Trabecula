﻿////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <ui/ImageCtrl.h>

////////////////////////////////////////////////////////////////////////////////

class TrabeImageCtrl : public ImageCtrl
{
public:

//------------------------------------------------------------------------------
//message handler
	BEGIN_MSG_MAP(TrabeImageCtrl)
		CHAIN_MSG_MAP(ImageCtrl)
	END_MSG_MAP()
};

////////////////////////////////////////////////////////////////////////////////
