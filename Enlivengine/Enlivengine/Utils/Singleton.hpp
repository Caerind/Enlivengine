#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_UTILS

#define ENLIVE_SINGLETON(className) \
	private: \
		className(); \
	public: \
		className(const className&) = delete; \
		className& operator=(const className&) = delete; \
		className(className&&) = delete; \
		className& operator=(className&&) = delete; \
		static className& GetInstance() { \
			static className instance; \
			return instance; \
		}

#endif // ENLIVE_MODULE_UTILS