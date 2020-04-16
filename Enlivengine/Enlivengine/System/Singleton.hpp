#pragma once

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

