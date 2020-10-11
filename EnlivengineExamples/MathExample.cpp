
#include <cmath>

#include <SFML/Graphics.hpp>

#include <Enlivengine/Math/Utilities.hpp>

#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Application/Window.hpp>


void MathFunctionsSpeedComparaisonWithStd();
void MathDeltaWithStd();
void MathTrigoComparaisonGraph();

int main()
{
	MathFunctionsSpeedComparaisonWithStd();
	MathDeltaWithStd();
	//MathTrigoComparaisonGraph();

	return 0;
}

en::Time cosStd;
en::Time cosEn;
en::Time sinStd;
en::Time sinEn;
en::Time tanStd;
en::Time tanEn;
en::Time acosStd;
en::Time acosEn;
en::Time asinStd;
en::Time asinEn;
en::Time atanStd;
en::Time atanEn;
en::Time atan2Std;
en::Time atan2En;

void MathFunctionsSpeedComparaisonWithStd()
{
	en::Clock clock;
	en::F32 sumStd = 0.0f;
	en::F32 sumEn = 0.0f;

	clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.001f)
	{
		const en::F32 y = std::sin(x * en::Math::kDegToRad);
		sumStd += y;
	}
	sinStd = clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.001f)
	{
		const en::F32 y = en::Math::Sin(x);
		sumEn += y;
	}
	sinEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.001f)
	{
		const en::F32 y = std::cos(x * en::Math::kDegToRad);
		sumStd += y;
	}
	cosStd = clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.001f)
	{
		const en::F32 y = en::Math::Cos(x);
		sumEn += y;
	}
	cosEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.001f)
	{
		const en::F32 y = std::tan(x * en::Math::kDegToRad);
		sumStd += y;
	}
	tanStd = clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.001f)
	{
		const en::F32 y = en::Math::Tan(x);
		sumEn += y;
	}
	tanEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = std::asin(x) * en::Math::kRadToDeg;
		sumStd += y;
	}
	asinStd = clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = en::Math::Asin(x);
		sumEn += y;
	}
	asinEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = std::acos(x) * en::Math::kRadToDeg;
		sumStd += y;
	}
	acosStd = clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = en::Math::Acos(x);
		sumEn += y;
	}
	acosEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -2.1f; x <= 2.1f; x += 0.000001f)
	{
		const en::F32 y = std::atan(x) * en::Math::kRadToDeg;
		sumStd += y;
	}
	atanStd = clock.Restart();
	for (en::F32 x = -2.1f; x <= 2.1f; x += 0.000001f)
	{
		const en::F32 y = en::Math::Atan(x);
		sumEn += y;
	}
	atanEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -1000.0f; x <= 1000.0f; x += 1.0f)
	{
		for (en::F32 y = -1000.0f; y <= 1000.0f; y += 1.0f)
		{
			if (x != 0.0f && y != 0.0f)
			{
				const en::F32 z = std::atan2(y, x) * en::Math::kRadToDeg;
				sumStd += y;
			}
		}
	}
	atan2Std = clock.Restart();
	for (en::F32 x = -1000.0f; x <= 1000.0f; x += 1.0f)
	{
		for (en::F32 y = -1000.0f; y <= 1000.0f; y += 1.0f)
		{
			if (x != 0.0f && y != 0.0f)
			{
				const en::F32 z = en::Math::Atan2(x, y);
				sumEn += y;
			}
		}
	}
	atan2En = clock.Restart();

	enLogInfo(en::LogChannel::Math, "Sin : {} vs {} => {} {}", sinStd.AsSeconds(), sinEn.AsSeconds(), sinEn <= sinStd, sinStd.AsSeconds() / sinEn.AsSeconds());
	enLogInfo(en::LogChannel::Math, "Cos : {} vs {} => {} {}", cosStd.AsSeconds(), cosEn.AsSeconds(), cosEn <= cosStd, cosStd.AsSeconds() / cosEn.AsSeconds());
	enLogInfo(en::LogChannel::Math, "Tan : {} vs {} => {} {}", tanStd.AsSeconds(), tanEn.AsSeconds(), tanEn <= tanStd, tanStd.AsSeconds() / tanEn.AsSeconds());
	enLogInfo(en::LogChannel::Math, "Asin : {} vs {} => {} {}", asinStd.AsSeconds(), asinEn.AsSeconds(), asinEn <= asinStd, asinStd.AsSeconds() / asinEn.AsSeconds());
	enLogInfo(en::LogChannel::Math, "Acos : {} vs {} => {} {}", acosStd.AsSeconds(), acosEn.AsSeconds(), acosEn <= acosStd, acosStd.AsSeconds() / acosEn.AsSeconds());
	enLogInfo(en::LogChannel::Math, "Atan : {} vs {} => {} {}", atanStd.AsSeconds(), atanEn.AsSeconds(), atanEn <= atanStd, atanStd.AsSeconds() / atanEn.AsSeconds());
	enLogInfo(en::LogChannel::Math, "Atan2 : {} vs {} => {} {}", atan2Std.AsSeconds(), atan2En.AsSeconds(), atan2En <= atan2Std, atan2Std.AsSeconds() / atan2En.AsSeconds());
}

void MathDeltaWithStd()
{
	en::F32 percent = 0.0f;
	en::F32 firstImprecision = 999999.0f;
	en::F32 lastImprecision = -999999.0f;
	for (en::F32 x = 0.0f; x <= 10.0f; x += 0.00001f)
	{
		const en::F32 yStd = std::atan(x) * en::Math::kRadToDeg;
		const en::F32 yEn = en::Math::Atan(x);
		if (!en::Math::Equals(yStd, yEn))
		{
			firstImprecision = en::Math::Min(firstImprecision, x);
			lastImprecision = en::Math::Max(lastImprecision, x);
			percent += 1.0f;
		}
	}
	percent /= (10 * (1.0f / 0.00001f));
	percent *= 100.0f;
	enLogInfo(en::LogChannel::Math, "Percent imprecision => {}% : [{}, {}]", percent, firstImprecision, lastImprecision);
}



template <typename T>
class Function
{
public:
	Function(std::function<T(T)> fct) { mFunction = fct; mColor = sf::Color::Black; }

	virtual T value(const T& x) const { return mFunction(x); }

	void setColor(sf::Color const& color) { mColor = color; }
	sf::Color getColor() const { return mColor; }

	void render(sf::RenderTarget& target) { target.draw(&mVertexArray[0], mVertexArray.size(), sf::Lines); }
	void update(T const& xmin, T const& xmax, T const& d)
	{
		mVertexArray.clear();
		en::U32 points = (xmax - xmin) * d;
		mVertexArray.reserve(points);
		for (T x = xmin; x <= xmax; x += d)
		{
			mVertexArray.emplace_back(sf::Vector2f(x, -value(x)), mColor);
			if (x != xmin && x != xmax)
			{
				mVertexArray.emplace_back(sf::Vector2f(x, -value(x)), mColor);
			}
		}
	}

protected:
	std::function<T(T)> mFunction;
	std::vector<sf::Vertex> mVertexArray;
	sf::Color mColor;
};

template <typename T>
class Graph
{
public:
	Graph()
	{
		mX.resize(2);
		mY.resize(2);

		mX[0].color = sf::Color::Black;
		mX[1].color = sf::Color::Black;
		mY[0].color = sf::Color::Black;
		mY[1].color = sf::Color::Black;

		mX[0].position = sf::Vector2f(-1000000.f, 0.f);
		mX[1].position = sf::Vector2f(1000000.f, 0.f);
		mY[0].position = sf::Vector2f(0.f, -1000000.f);
		mY[1].position = sf::Vector2f(0.f, 1000000.f);

		setView(-10.f, 10.f, -10.f, 10.f);

		setCenter(sf::Vector2f(0, 0));
	}

	void setView(float xmin, float xmax, float ymin, float ymax)
	{
		mView.setCenter((xmax - xmin) * 0.5f, (ymax - ymin) * 0.5f);
		mView.setSize(xmax - xmin, ymax - ymin);
		update();
	}

	void moveView(sf::Vector2f const& mvt)
	{
		mView.move(mvt);
		update();
	}

	void setCenter(sf::Vector2f const& center)
	{
		mView.setCenter(center);
		update();
	}

	void zoom(float factor)
	{
		mView.zoom(factor);
		update();
	}

	sf::View& getView() { return mView; }

	void addFunction(Function<T>* f)
	{
		if (f != nullptr)
		{
			mFunctions.push_back(f);
			T min = (T)(mView.getCenter().x - mView.getSize().x * 0.5f);
			T max = (T)(mView.getCenter().x + mView.getSize().x * 0.5f);
			f->update(min, max, mView.getSize().x / (max - min));
		}
	}

	void render(sf::RenderTarget& target)
	{
		target.setView(mView);

		target.draw(&mX[0], 2, sf::Lines);
		target.draw(&mY[0], 2, sf::Lines);

		for (std::size_t i = 0; i < mFunctions.size(); i++)
		{
			if (mFunctions[i] != nullptr)
			{
				mFunctions[i]->render(target);
			}
		}
	}

	float getSpeed()
	{
		return mView.getSize().x * 1.5f;
	}

	void update()
	{
		for (std::size_t i = 0; i < mFunctions.size(); i++)
		{
			if (mFunctions[i] != nullptr)
			{
				T min = (T)(mView.getCenter().x - mView.getSize().x * 0.5f);
				T max = (T)(mView.getCenter().x + mView.getSize().x * 0.5f);
				mFunctions[i]->update(min, max, mView.getSize().x / (max - min));
			}
		}
	}

protected:
	sf::View mView;

	std::vector<Function<T>*> mFunctions;

	std::vector<sf::Vertex> mX;
	std::vector<sf::Vertex> mY;
};

class TrigoComparaisonGraph : public en::State
{
public:
	TrigoComparaisonGraph(en::StateManager& manager) 
		: en::State(manager)
		, mGraph()
		, mAtanStd([](en::F32 x) { return std::atan(x) * en::Math::kRadToDeg; })
		, mAtanEn([](en::F32 x) { return en::Math::Atan(x); })
	{
		mAtanStd.setColor(sf::Color::Red);
		mAtanStd.setColor(sf::Color::Green);

		mGraph.addFunction(&mAtanStd);
		mGraph.addFunction(&mAtanEn);
	}

	bool handleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			en::I32 tick = event.mouseWheelScroll.delta;
			if (tick > 0)
			{
				mGraph.zoom(0.75f);
			}
			else
			{
				mGraph.zoom(1.5f);
			}
		}
		return true;
	}

	bool update(en::Time dt)
	{
		ENLIVE_PROFILE_FUNCTION();

		if (!en::Application::GetInstance().IsRunning())
		{
			return false;
		}

		if (getApplication().GetActionSystem().IsInputActive("close"))
		{
			getApplication().Stop();
			return false;
		}

		ImGui::Begin("Info");
		ImGui::Text("x = %f",  getApplication().GetWindow().getCursorPositionView(mGraph.getView()).x);
		ImGui::Text("y = %f", -getApplication().GetWindow().getCursorPositionView(mGraph.getView()).y);
		ImGui::End();

		sf::Vector2f mvt;
		if (getApplication().GetActionSystem().IsInputActive("moveUp"))
			mvt.y--;
		if (getApplication().GetActionSystem().IsInputActive("moveLeft"))
			mvt.x--;
		if (getApplication().GetActionSystem().IsInputActive("moveDown"))
			mvt.y++;
		if (getApplication().GetActionSystem().IsInputActive("moveRight"))
			mvt.x++;
		mGraph.moveView(mvt * dt.AsSeconds() * mGraph.getSpeed());

		return true;
	}

	void render(sf::RenderTarget& target)
	{
		ENLIVE_PROFILE_FUNCTION();

		target.clear(sf::Color::White);
		mGraph.render(target);
	}

private:
	Graph<en::F32> mGraph;
	Function<en::F32> mAtanStd;
	Function<en::F32> mAtanEn;
};

void MathTrigoComparaisonGraph()
{
	auto& app = en::Application::GetInstance();
	app.Initialize();

	const auto vm = sf::VideoMode(1024, 768);
	auto& window = app.GetWindow();
	window.create(sf::VideoMode(1024, 768), "EngineExample", sf::Style::Titlebar | sf::Style::Close);
	window.getMainView().setSize({ vm.width * 1.0f, vm.height * 1.0f });
	window.getMainView().setCenter({ vm.width * 0.5f, vm.height * 0.5f });

	auto& actionSystem = app.GetActionSystem();
	actionSystem.AddInputKey("close", sf::Keyboard::Escape);
	actionSystem.AddInputKey("moveUp", sf::Keyboard::Z, en::ActionType::Hold);
	actionSystem.AddInputKey("moveLeft", sf::Keyboard::Q, en::ActionType::Hold);
	actionSystem.AddInputKey("moveDown", sf::Keyboard::S, en::ActionType::Hold);
	actionSystem.AddInputKey("moveRight", sf::Keyboard::D, en::ActionType::Hold);

	app.Start<TrigoComparaisonGraph>();

}