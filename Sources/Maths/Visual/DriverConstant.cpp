#include "DriverConstant.hpp"

namespace acid
{
	DriverConstant::DriverConstant(const float &constant) :
		IDriver(1.0f),
		m_value(constant)
	{
	}

	float DriverConstant::Calculate(const float &time)
	{
		return m_value;
	}
}
