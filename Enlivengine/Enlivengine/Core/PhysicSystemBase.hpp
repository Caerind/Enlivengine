#pragma once

#include <Enlivengine/Core/System.hpp>

namespace en
{

class PhysicSystemBase : public System
{
public:
	virtual void UpdatePhysic(Time dt) = 0;
};

} // namespace en