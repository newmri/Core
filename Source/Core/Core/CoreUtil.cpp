#include "CoreInclude.h"
#include <boost/lexical_cast.hpp>

int32_t CoreUtil::IntRound(const float_t value)
{
	return static_cast<int32_t>(value + 0.5f);
}

int32_t CoreUtil::IntCeil(const float_t value)
{
	return static_cast<int32_t>(value + 1.0f);
}

std::string CoreUtil::GetThreadID(void)
{
	return "ThreadID:" + boost::lexical_cast<std::string>(std::this_thread::get_id());
}