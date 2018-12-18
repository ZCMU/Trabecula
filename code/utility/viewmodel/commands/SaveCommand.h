////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class SaveCommand : public ICommandBase
{
public:
	SaveCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~SaveCommand() throw()
	{
	}

// ICommandBase
	virtual void SetParameter(const std::any& param)
	{
		m_param = param;
	}
	virtual void Exec()
	{
		std::string& str = *(std::any_cast<std::string>(&m_param));
		bool bRet = m_pVM->SaveFile(str);
		m_pVM->Fire_OnCommandComplete(std::string("save"), bRet);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
