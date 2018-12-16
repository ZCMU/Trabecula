////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class MeasureCommand : public ICommandBase
{
public:
	MeasureCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~MeasureCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		bool bRet = m_pVM->MeasureMask();
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
