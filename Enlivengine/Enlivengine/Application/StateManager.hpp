#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <Enlivengine/System/Time.hpp>

namespace en
{

class Application;
class StateManager;

class State
{
	public:
		State(StateManager& manager);
		virtual ~State();

		virtual bool onInitialize();
		virtual bool onUninitialize();

		virtual void onActivate();
		virtual void onDeactivate();

		virtual bool handleEvent(const sf::Event& event);
		virtual bool update(Time dt);
		virtual void render(sf::RenderTarget& target);

		template <typename T, typename ... Args>
		void pushState(Args&& ... args);
		void popState();
		void clearStates();

		Application& getApplication();

	private:
		friend class StateManager;
		StateManager* mManager;
};

class StateManager
{
	public:
		StateManager(Application& application);
		~StateManager();

		void handleEvent(const sf::Event& event);
		void update(Time dt);
		void render(sf::RenderTarget& target);

		template <typename T, typename ... Args>
		void pushState(Args&& ... args);
		void popState();
		void clearStates();

		U32 getStateCount() const;

		Application& getApplication();

		void applyPendingChanges();

	private:
		enum Action
		{
			Push,
			Pop,
			Clear
		};

		struct PendingChange
		{
			PendingChange(Action action, State* state = nullptr);

			Action action;
			State* state;
		};

	private:
		Application& mApplication;
		std::vector<State*> mStates;
		std::vector<PendingChange> mChanges;
};

template <typename T, typename ... Args>
void State::pushState(Args&& ... args)
{
	mManager->pushState<T>(std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
void StateManager::pushState(Args&& ... args)
{
	mChanges.emplace_back(Action::Push, new T(*this, std::forward<Args>(args)...));
}

} // namespace en
