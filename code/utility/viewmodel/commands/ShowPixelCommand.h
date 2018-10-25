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
		m_pVM->ShowPickPixel(rgb);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
