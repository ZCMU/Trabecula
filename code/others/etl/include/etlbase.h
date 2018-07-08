////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <any>
#include <vector>
#include <map>
#include <stack>

//types
typedef unsigned char     uchar;
typedef unsigned short    ushort;
typedef unsigned int      uint;
typedef unsigned __int64  uint64;
typedef __int64           int64;

//command

/*
class ICommandParameter
{
};
*/

class ICommandBase
{
public:
	//virtual void SetParameter(const std::shared_ptr<ICommandParameter>& param) = 0;
	virtual void SetParameter(const std::any& param) = 0;
	virtual void Exec() = 0;
/*
virtual std::shared_ptr<ICommandBase> get_Undo() = 0;
virtual std::shared_ptr<ICommandBase> get_Redo() = 0;
//use make_shared, then the virtual destructor is not necessary. (RAII)
*/
};

class CommandManager
{
public:
/*
...
*/
private:
	std::vector<std::shared_ptr<ICommandBase>> m_vecUndo;
	std::vector<std::shared_ptr<ICommandBase>> m_vecRedo;
};

//Notifications

template <class T>
class NotificationImpl  // 通知实现
{
public:
	void Clear() throw()
	{
		m_array.clear();
	}
	void AddNotification(const std::shared_ptr<T>& p)
	{
		m_array.push_back(p);  // vector的push_back操作是将一个元素插入vector的末尾。
	}
/*
	void RemoveNotification(const std::shared_ptr<T>& p) throw()
	{
		...
	}
*/

protected:
	std::vector<std::shared_ptr<T>> m_array;  // 通知实体
};

class IPropertyNotification  // 属性通知
{
public:
	virtual void OnPropertyChanged(const std::string& str) = 0;
	//virtual void OnPropertyChanged(unsigned int uPropertyID) = 0;
};

class ICommandNotification   // 命令通知
{
public:
// （1）在基类用virtual声明成员函数为虚函数。这样就可以在派生类中重新定义此函数，为它赋予新的功能，并能方便被调用。
// 在类外定义虚函数时，不必在定义virtual
// （2）在派生类中重新定义此函数，要求函数名，函数类型，函数参数个数和类型全部与基类的虚函数相同，并根据派生类的需要重新定义函数体。
// c++规定，当一个成员函数被声明为虚函数后，其派生类的同名函数都自动成为虚函数。因此在派生类重新声明该虚函数时，可以加virtual，也可以不加，
// 但习惯上一般在每层声明该函数时都加上virtual，使程序更加清晰。
	virtual void OnCommandComplete(const std::string& str, bool bOK) = 0;
	//virtual void OnCommandComplete(unsigned int uCommandID, bool bOK) = 0;
};

template <class T>  // 代理属性通知
class Proxy_PropertyNotification : public NotificationImpl<IPropertyNotification>
{
public:
	void AddPropertyNotification(const std::shared_ptr<IPropertyNotification>& p)
	{
		AddNotification(p);
	}
	void Fire_OnPropertyChanged(const std::string& str)
	{
		auto iter(m_array.begin());
		for( ; iter != m_array.end(); ++ iter ) {
			(*iter)->OnPropertyChanged(str);
		}
	}
};
template <class T>  // 代理命令通知
class Proxy_CommandNotification : public NotificationImpl<ICommandNotification>
{
public:
	void AddCommandNotification(const std::shared_ptr<ICommandNotification>& p)
	{
		AddNotification(p);
	}
	void Fire_OnCommandComplete(const std::string& str, bool bOK)
	{
        // 使用的是 NotificationImpl 的 m_array
        // ICommandNotification 只是声明了一下 OnCommandComplete
        // MainWindowCommandSink 实现 OnCommandComplete
		auto iter(m_array.begin());  // 使用auto从初始化表达式中推断出变量的数据类型
        // 给 iter 赋值等于 m_array.begin()，inter 为 ICommandNotification
		for( ; iter != m_array.end(); ++ iter ) {
			(*iter)->OnCommandComplete(str, bOK);
		}
	}
};

//state machine

class IStateBase
{
public:
	virtual int Process(unsigned int uEvent, std::any& param) = 0;
};

class StateManager
{
public:
/*
viud Add(int iState, const std::shared_ptr<IStateBase>& spState)
{
...
}
void Process(unsigned int uEvent, std::any& param)
{
...
}
...
*/
private:
	int m_iCurrentState;
	std::map<int, std::shared_ptr<IStateBase>> m_map;
};

////////////////////////////////////////////////////////////////////////////////
