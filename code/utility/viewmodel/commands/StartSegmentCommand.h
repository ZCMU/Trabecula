////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class StartSegmentCommand : public ICommandBase
{
public:
	StartSegmentCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~StartSegmentCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		bool bRet = m_pVM->StartImageSegment();
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
