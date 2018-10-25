////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class ClearSegmentCommand : public ICommandBase
{
public:
	ClearSegmentCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~ClearSegmentCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		bool bRet = m_pVM->ClearImageSegment();
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
