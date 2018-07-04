////////////////////////////////////////////////////////////////////////////////
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
	TrabeViewModel() : m_spImage(std::make_shared<ATL::CImage>()),                          // 属性
						m_cmdLoad(std::make_shared<LoadCommand<TrabeViewModel>>(this)),     // Load命令（LoadCommand）
                        m_cmdCalc(std::make_shared<CalcCommand<TrabeViewModel>>(this)),
                        m_cmdClear(std::make_shared<ClearCommand<TrabeViewModel>>(this)),
						m_sink(std::make_shared<TrabeViewModelSink<TrabeViewModel>>(this))  // 接收者
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
	bool LoadFile(const std::string& strFile)  // DataModel 载入文件
	{
		return m_spModel->Load(strFile);
	}
	void ColorDataToImage()  // 彩色像素到图片
	{
		ImageDataHelper::ColorDataToImage(m_spModel->get_ColorData(), *m_spImage);
	}
	void GrayDataToImage()  // 灰度像素到图片
	{
        ImageDataHelper::GrayDataToImage(m_spModel->get_GrayData(), *m_spImage);
	}
    bool CalcArea(NMIMAGEPIXEL* pnm)  // DataModel 颜色分割
    {
        return m_spModel->Calc(pnm);
    }
    bool ClearResult()  // DataModel 颜色分割
    {
        return m_spModel->Clear();
    }

private:
	//models
	std::shared_ptr<TrabeDataModel> m_spModel;    // 模型

	//properties
	std::shared_ptr<ATL::CImage> m_spImage;

	//commands
	std::shared_ptr<LoadCommand<TrabeViewModel>> m_cmdLoad;  // Load命令  C++模板类使用方法，使用 TrabeViewModel 模板创建类
    std::shared_ptr<CalcCommand<TrabeViewModel>> m_cmdCalc;
    std::shared_ptr<ClearCommand<TrabeViewModel>> m_cmdClear;
	//sinks
	std::shared_ptr<TrabeViewModelSink<TrabeViewModel>> m_sink;
};

////////////////////////////////////////////////////////////////////////////////
