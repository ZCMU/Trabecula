////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class ShowPixelCommand : public ICommandBase
{
public:
	ShowPixelCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~ShowPixelCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		std::array<UINT, 3>& rgb = *(std::any_cast<std::array<UINT, 3>>(&m_param));
		#if DEBUG_INFO
		CString str;
		str.Format(_T("R: %d G: %d B: %d\r\n"),
			rgb[0], rgb[1], rgb[2]
			);
		OutputDebugPrintf(str);
		#endif
		m_pVM->ShowPickPixel(rgb);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
