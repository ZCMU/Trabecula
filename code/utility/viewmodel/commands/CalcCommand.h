////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////
// command对象 个封装的是viewmodel的方法。或者方法集合。由viewmodel类暴露出来，供view层对象绑定
template <class TViewModel>  // TViewModel 只是一个模板代号，TViewModel 为 TrabeViewModel
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
	virtual void SetParameter(const std::any& param)   // 设置参数
	{
		m_param = param;
	}
	virtual void Exec()                                // 执行
	{
		bool bRet = m_pVM->CalcArea((NMIMAGEPIXEL *)&m_param);
	}

private:
	TViewModel* m_pVM;  // TViewModel=TrabeViewModel 派生于 Proxy_CommandNotification 有 Fire_OnCommandComplete
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
