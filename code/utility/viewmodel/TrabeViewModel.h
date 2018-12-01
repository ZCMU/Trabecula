////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include "../model/TrabeDataModel.h"
#include "commands/LoadCommand.h"
#include "commands/ShowPixelCommand.h"
#include "commands/StartSegmentCommand.h"
#include "commands/ClearSegmentCommand.h"
#include "commands/EraseCommand.h"
#include "sinks/TrabeViewModelSink.h"

////////////////////////////////////////////////////////////////////////////////

class TrabeViewModel : public Proxy_PropertyNotification<TrabeViewModel>,
						public Proxy_CommandNotification<TrabeViewModel>
{
public:
	TrabeViewModel() : m_spImageOriginal(std::make_shared<ATL::CImage>()),
						m_spImageProcess(std::make_shared<ATL::CImage>()),
						m_spLabel(std::make_shared<ATL::CString>()),
						m_cmdLoad(std::make_shared<LoadCommand<TrabeViewModel>>(this)),
						m_cmdShowPixel(std::make_shared<ShowPixelCommand<TrabeViewModel>>(this)),
						m_cmdStartSegment(std::make_shared<StartSegmentCommand<TrabeViewModel>>(this)),
						m_cmdClearSegment(std::make_shared<ClearSegmentCommand<TrabeViewModel>>(this)),
						m_cmdErase(std::make_shared<EraseCommand<TrabeViewModel>>(this)),
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
	std::shared_ptr<ATL::CImage> get_ImageOriginal() throw()
	{
		return m_spImageOriginal;
	}
	std::shared_ptr<ATL::CImage> get_ImageProcess() throw()
	{
		return m_spImageProcess;
	}
	std::shared_ptr<ATL::CString> get_Label() throw()
	{
		return m_spLabel;
	}

	//commands
	std::shared_ptr<ICommandBase> get_LoadCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdLoad);
	}
	std::shared_ptr<ICommandBase> get_ShowPixelCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdShowPixel);
	}
	std::shared_ptr<ICommandBase> get_StartSegmentCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdStartSegment);
	}
	std::shared_ptr<ICommandBase> get_ClearSegmentCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdClearSegment);
	}
	std::shared_ptr<ICommandBase> get_EraseCommand() throw()
	{
		return std::static_pointer_cast<ICommandBase>(m_cmdErase);
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
		ImageDataHelper::ColorDataToImage(m_spModel->get_ColorData(), *m_spImageOriginal);
		ImageDataHelper::ColorDataToImage(m_spModel->get_ColorData(), *m_spImageProcess);
	}
	void ColorDataAndMaskToImage()
	{
		ImageDataHelper::ColorDataAndMaskToImage(m_spModel->get_ColorData(), m_spModel->get_MaskData(), *m_spImageProcess);
	}
	void GrayDataToImage()
	{
		ImageDataHelper::GrayDataToImage(m_spModel->get_GrayData(), *m_spImageProcess);
	}
	bool ShowPickPixel(const std::array<UINT, 3>& pkPixel)
	{
		return m_spModel->ShowPixel(pkPixel);
	}
	// PixelDataToString
	void PixelDataToString()
	{
		const PixelData& data = m_spModel->get_PixelData();
		CString& str = *m_spLabel;
		str.Format(_T("R: %u G: %u B: %u\r\nH: %4.1f S: %4.2f V: %4.2f\r\n"),
			data.rgb.r, data.rgb.g, data.rgb.b,
			data.hsv.h, data.hsv.s, data.hsv.v
			);
	}
	bool StartImageSegment()
	{
		return m_spModel->StartSegment();
	}
	bool ClearImageSegment()
	{
		return m_spModel->ClearSegment();
	}
	bool EraseMask(const std::array<INT, 4>& rect)
	{
		return m_spModel->Erase(rect);
	}

private:
	//models
	std::shared_ptr<TrabeDataModel> m_spModel;

	//properties
	std::shared_ptr<ATL::CImage> m_spImageOriginal;
	std::shared_ptr<ATL::CImage> m_spImageProcess;
	std::shared_ptr<ATL::CString> m_spLabel;

	//commands
	std::shared_ptr<LoadCommand<TrabeViewModel>> m_cmdLoad;
	std::shared_ptr<ShowPixelCommand<TrabeViewModel>> m_cmdShowPixel;
	std::shared_ptr<StartSegmentCommand<TrabeViewModel>> m_cmdStartSegment;
	std::shared_ptr<ClearSegmentCommand<TrabeViewModel>> m_cmdClearSegment;
	std::shared_ptr<EraseCommand<TrabeViewModel>> m_cmdErase;
	//sinks
	std::shared_ptr<TrabeViewModelSink<TrabeViewModel>> m_sink;
};

////////////////////////////////////////////////////////////////////////////////
