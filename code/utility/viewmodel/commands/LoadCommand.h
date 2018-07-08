﻿////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////
// command对象 个封装的是viewmodel的方法。或者方法集合。由viewmodel类暴露出来，供view层对象绑定
template <class TViewModel>  // TViewModel 只是一个模板代号，TViewModel 为 TrabeViewModel
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
	virtual void SetParameter(const std::any& param)   // 设置参数
	{
		m_param = param;
	}
	virtual void Exec()                                // 执行
	{
		std::string& str = *(std::any_cast<std::string>(&m_param));
		bool bRet = m_pVM->LoadFile(str);  // ViewModel 中 LoadFile
		m_pVM->Fire_OnCommandComplete(std::string("load"), bRet);  // 通知 load 是否成功
	}

private:
	TViewModel* m_pVM;  // TViewModel=TrabeViewModel 派生于 Proxy_CommandNotification 有 Fire_OnCommandComplete
	std::any    m_param;
};

////////////////////////////////////////////////////////////////////////////////
