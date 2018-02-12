////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>
class LoadCommand : public ICommandBase
{
public:
	LoadCommand(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~LoadCommand() throw()
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
		bool bRet = m_pVN->LoadFile(str);
		m_pVM->Fire_OnCommandNotification(std::string("load"), bRet);
	}

private:
	TViewModel* m_pVM;
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
