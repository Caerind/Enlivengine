#include <Enlivengine/System/CallOnExit.hpp>

namespace en
{

CallOnExit::CallOnExit(Func func)
	: mFunc(func)
{
}

CallOnExit::~CallOnExit()
{
	call();
}

void CallOnExit::call()
{
	if (isValid())
	{
		mFunc();
	}
}

void CallOnExit::reset(Func func)
{
	mFunc = func;
}

bool CallOnExit::isValid() const
{
	return mFunc != nullptr;
}

CallOnExit::Func CallOnExit::getFunction() const
{
	return mFunc;
}

} // namespace en
