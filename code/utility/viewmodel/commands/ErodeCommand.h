////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class ErodeCommand : public ICommandBase
{
public:
	ErodeCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~ErodeCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		bool bRet = m_pVM->ErodeMask();
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
