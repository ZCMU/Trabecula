////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

template <class TViewModel>  // TViewModel 只是一个模板代号，TViewModel 为 TrabeViewModel
class TrabeViewModelSink : public IPropertyNotification
{
public:
	TrabeViewModelSink(TViewModel* pVM) throw() : m_pVM(pVM)
	{
	}
	~TrabeViewModelSink() throw()
	{
	}

// IPropertyNotification
// 数据相当于“属性”的改变，在model里面才会有触发属性改变的通知，
// 这里viewmodel是接收者 sink 所以在sink对象里面接受并进行了数据转换，
// 吧colordata转换为可以用来绘制的CImage，并再次触发属性改变通知，
// 因为图片是viewmodel的属性。
	virtual void OnPropertyChanged(const std::string& str)
	{
		if( str == "color_data" ) {
			m_pVM->ColorDataToImage();  // 彩色像素到图像
			m_pVM->Fire_OnPropertyChanged(std::string("image"));
		}
		if( str == "gray_data" ) {
			m_pVM->GrayDataToImage();  // 灰度像素到图像
			m_pVM->Fire_OnPropertyChanged(std::string("image"));
		}
	}

private:
	TViewModel* m_pVM;
};

////////////////////////////////////////////////////////////////////////////////
