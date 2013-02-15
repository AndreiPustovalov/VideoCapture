

namespace cvext
{
	template<class UnaryFunction>
	void callbackFun(int state, void* data)
	{
		(*static_cast<UnaryFunction>(data))(state);
	}

	template<class TrackbarCallback>
	int createTrackbar(const std::string& trackbarname, const std::string& winname,
                              int* value, int count,
                              TrackbarCallback& onChange)
	{
		return cv::createTrackbar(trackbarname,winname,value,count,callbackFun<decltype(&onChange)>, &onChange);
	}
	template<class ButtonCallback>
	int createButton( const std::string& bar_name, ButtonCallback& on_change,
                             int type=CV_PUSH_BUTTON,
                             bool initial_button_state=0)
	{
		return cv::createButton(bar_name,callbackFun<decltype(&on_change)>, &on_change, type, initial_button_state);
	}
}