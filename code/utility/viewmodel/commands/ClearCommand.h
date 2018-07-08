////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class ClearCommand : public ICommandBase
{
public:
	ClearCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~ClearCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		bool bRet = m_pVM->ClearResult();
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
