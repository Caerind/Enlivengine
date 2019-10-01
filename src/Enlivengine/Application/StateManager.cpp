#include <Enlivengine/Application/StateManager.hpp>

#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/System/Assert.hpp>

namespace en
{

State::State(StateManager& manager)
	: mManager(&manager)
{
}

State::~State()
{
}

void State::onActivate()
{
}

void State::onDeactivate()
{
}

bool State::handleEvent(const sf::Event& event)
{
	return false;
}

bool State::update(Time dt)
{
	return false;
}

void State::render(sf::RenderTarget& target)
{
}

void State::popState()
{
	assert(mManager != nullptr);
	mManager->popState();
}

void State::clearStates()
{
	assert(mManager != nullptr);
	mManager->clearStates();
}

Application& State::getApplication()
{
	assert(mManager != nullptr);
	return mManager->getApplication();
}

StateManager::StateManager(Application& application)
	: mApplication(application)
{
}

StateManager::~StateManager()
{
	for (auto itr = mStates.begin(); itr != mStates.end(); ++itr)
	{
		(*itr)->onDeactivate();
	}
	mStates.clear();
}

void StateManager::handleEvent(const sf::Event& event)
{
	for (auto itr = mStates.rbegin(); itr != mStates.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
		{
			break;
		}
	}
	applyPendingChanges();
}

void StateManager::update(Time dt)
{
	for (auto itr = mStates.rbegin(); itr != mStates.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
		{
			break;
		}
	}
	applyPendingChanges();
}

void StateManager::render(sf::RenderTarget& target)
{
	for (auto itr = mStates.begin(); itr != mStates.end(); ++itr)
	{
		(*itr)->render(target);
	}
}

void StateManager::popState()
{
	mChanges.emplace_back(Action::Pop, nullptr);
}

void StateManager::clearStates()
{
	mChanges.emplace_back(Action::Clear, nullptr);
}

U32 StateManager::getStateCount() const
{
	return mStates.size();
}

Application& StateManager::getApplication()
{
	return mApplication;
}

void StateManager::applyPendingChanges()
{
	for (const PendingChange& change : mChanges)
	{
		switch (change.action)
		{
			case Action::Push:
				if (mStates.size() > 0)
				{
					mStates.back()->onDeactivate();
				}
				mStates.push_back(change.state);
				mStates.back()->onActivate();
				break;

			case Action::Pop:
				if (mStates.size() > 0)
				{
					State* state = mStates.back();
					state->onDeactivate();
					delete state;
					mStates.pop_back();
				}
				if (mStates.size() > 0)
				{
					mStates.back()->onActivate();
				}
				break;

			case Action::Clear:
				if (mStates.size() > 0)
				{
					mStates.back()->onDeactivate();
				}
				for (auto itr = mStates.begin(); itr != mStates.end(); ++itr)
				{
					delete (*itr);
				}
				mStates.clear();
				break;
		}
	}
	mChanges.clear();
}

StateManager::PendingChange::PendingChange(Action action, State* state)
	: action(action)
	, state(state)
{
}

} // namespace en