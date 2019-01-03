////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class FilterCommand : public ICommandBase
{
public:
	FilterCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~FilterCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		std::array<UINT, 1>& quantity = *(std::any_cast<std::array<UINT, 1>>(&m_param));
		bool bRet = m_pVM->FilterMask(quantity);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
