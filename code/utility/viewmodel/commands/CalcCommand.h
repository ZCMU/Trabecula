////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class CalcCommand : public ICommandBase
{
public:
	CalcCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~CalcCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		bool bRet = m_pVM->CalcArea((NMIMAGEPIXEL *)&m_param);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
