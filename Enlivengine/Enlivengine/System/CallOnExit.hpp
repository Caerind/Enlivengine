#pragma once

#include <functional>

namespace en
{

class CallOnExit
{
	public:
		using Func = std::function<void()>;

		CallOnExit(Func&& func = nullptr)
			: mFunc(func)
		{
		}

		CallOnExit(const CallOnExit&) = delete;
		CallOnExit(CallOnExit&&) = delete;

		~CallOnExit()
		{
			Call();
		}
		
		void Call()
		{
			if (IsValid())
			{
				mFunc();
			}
		}

		void Reset(Func&& func = nullptr)
		{
			mFunc = func;
		}

		bool IsValid() const
		{
			return static_cast<bool>(mFunc);
		}

		CallOnExit& operator=(const CallOnExit&) = delete;
		CallOnExit& operator=(CallOnExit&&) = default;

	private:
		Func mFunc;
};

} // namespace en
