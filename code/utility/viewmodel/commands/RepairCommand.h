////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class RepairCommand : public ICommandBase
{
public:
	RepairCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~RepairCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		std::array<INT, 4>& rect = *(std::any_cast<std::array<INT, 4>>(&m_param));
		bool bRet = m_pVM->RepairMask(rect);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
