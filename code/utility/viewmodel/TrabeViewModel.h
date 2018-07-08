﻿////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include "../model/TrabeDataModel.h"
#include "commands/LoadCommand.h"
#include "commands/CalcCommand.h"
#include "commands/ClearCommand.h"
#include "sinks/TrabeViewModelSink.h"

////////////////////////////////////////////////////////////////////////////////

class TrabeViewModel : public Proxy_PropertyNotification<TrabeViewModel>,
						public Proxy_CommandNotification<TrabeViewModel>
{
public:
	TrabeViewModel() : m_spImage(std::make_shared<ATL::CImage>()),
						m_cmdLoad(std::make_shared<LoadCommand<TrabeViewModel>>(this)),
                        m_cmdCalc(std::make_shared<CalcCommand<TrabeViewModel>>(this)),
                        m_cmdClear(std::make_shared<ClearCommand<TrabeViewModel>>(this)),
						m_sink(std::make_shared<TrabeViewModelSink<TrabeViewModel>>(this))
	{
	}
	~TrabeViewModel() throw()
	{
	}

	//models
	void SetDataModel(const std::shared_ptr<TrabeDataModel>& sp) throw()
	{
		m_spModel = sp;
	}

	//properties
	std::shared_ptr<ATL::CImage> get_Image() throw()
	{
		return m_spImage;
	}

	//commands
	std::shared_ptr<ICommandBase> get_LoadCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdLoad);
	}
	std::shared_ptr<ICommandBase> get_CalcCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdCalc);
	}
	std::shared_ptr<ICommandBase> get_ClearCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdClear);
	}
    
	//sinks
	std::shared_ptr<IPropertyNotification> get_Sink() throw()
	{
		return std::static_pointer_cast<IPropertyNotification>(m_sink);
	}

	//methods
	bool LoadFile(const std::string& strFile)
	{
		return m_spModel->Load(strFile);
	}
	void ColorDataToImage()
	{
		ImageDataHelper::ColorDataToImage(m_spModel->get_ColorData(), *m_spImage);
	}
	void GrayDataToImage()
	{
        ImageDataHelper::GrayDataToImage(m_spModel->get_GrayData(), *m_spImage);
	}
    bool CalcArea(NMIMAGEPIXEL* pnm)
    {
        return m_spModel->Calc(pnm);
    }
    bool ClearResult()
    {
        return m_spModel->Clear();
    }

private:
	//models
	std::shared_ptr<TrabeDataModel> m_spModel;

	//properties
	std::shared_ptr<ATL::CImage> m_spImage;

	//commands
	std::shared_ptr<LoadCommand<TrabeViewModel>> m_cmdLoad;
    std::shared_ptr<CalcCommand<TrabeViewModel>> m_cmdCalc;
    std::shared_ptr<ClearCommand<TrabeViewModel>> m_cmdClear;
	//sinks
	std::shared_ptr<TrabeViewModelSink<TrabeViewModel>> m_sink;
};

////////////////////////////////////////////////////////////////////////////////
