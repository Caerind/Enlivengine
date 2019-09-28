#pragma once

#include <functional>

namespace en
{

class CallOnExit
{
	public:
		using Func = std::function<void()>;

		CallOnExit(Func func = nullptr);
		CallOnExit(const CallOnExit&) = delete;
		CallOnExit(CallOnExit&&) = delete;
		~CallOnExit();
		
		void call();
		void reset(Func func = nullptr);
		bool isValid() const;
		Func getFunction() const;

		CallOnExit& operator=(const CallOnExit&) = delete;
		CallOnExit& operator=(CallOnExit&&) = default;

	private:
		Func mFunc;
};

} // namespace en
