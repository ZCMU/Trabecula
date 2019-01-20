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
		std::vector<PAIR>& track = *(std::any_cast<std::vector<PAIR>>(&m_param));
		bool bRet = m_pVM->RubberMask(track);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
