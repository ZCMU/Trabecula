////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class RubberCommand : public ICommandBase
{
public:
	RubberCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~RubberCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		// std::array<INT, 4>& rect = *(std::any_cast<std::array<INT, 4>>(&m_param));
		// bool bRet = m_pVM->RubberMask(rect);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
