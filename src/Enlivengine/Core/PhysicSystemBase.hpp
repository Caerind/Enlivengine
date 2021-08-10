#pragma once

#include <Enlivengine/Core/System.hpp>

namespace en
{

class PhysicSystemBase : public System
{
public:
	virtual void UpdatePhysic() = 0;
};

} // namespace en