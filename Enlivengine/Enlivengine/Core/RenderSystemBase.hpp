#pragma once

#include <Enlivengine/Core/System.hpp>

namespace en
{

class RenderSystemBase : public System
{
public:
	virtual void Render() = 0;
};

} // namespace en