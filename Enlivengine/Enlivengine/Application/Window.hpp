#pragma once

#include <functional>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Touch.hpp>

#include <Enlivengine/System/Signal.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>
#include <Enlivengine/Graphics/View.hpp>

namespace en
{

class Window
{
	public:
		Window();
		Window(sf::VideoMode mode, const std::string& title = "", sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

		void create();
		void create(sf::VideoMode mode, const std::string& title = "", sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
		void toFullscreen();
		void toNonFullscreen();
		void toggleFullscreen();
		void close();

		bool isFullscreen() const;
		const sf::VideoMode& getVideoMode() const;
		U32 getStyle() const;
		const std::string& getTitle() const;
		bool isVerticalSyncEnabled() const;
		bool isVisible() const;
		bool isKeyRepeatEnabled() const;
		float getJoystickThreshold() const;
		bool isMouseCursorGrabbed() const;
		const sf::Image& getIcon() const;
		const std::string& getIconPath() const;
		Vector2f getPosition() const;
		const sf::ContextSettings& getSettings() const;
		Vector2f getSize() const;
		sf::WindowHandle getSystemHandle() const;
		bool hasFocus() const;

		void setTitle(const std::string& title);
		void setVerticalSyncEnabled(bool enabled);
		void setVisible(bool visible);
		void setKeyRepeatEnabled(bool enabled);
		void setJoystickThreshold(float threshold);
		void setMouseCursorGrabbed(bool grabbed);
		void setIcon(const std::string& icon);
		void setIcon(const sf::Image& image);
		void setPosition(const Vector2f& position);
		void setSize(const Vector2f& size);
		void setActive(bool active = true);
		void requestFocus();

		bool isOpen() const;
		bool pollEvent(sf::Event& event);
		void clear();
		void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
		void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
		void display();

		void setView(const View& view);
		View getView() const;
		void setMainView(const View& view);
		View& getMainView();
		const View& getMainView() const;
		void applyMainView();
		sf::IntRect getViewport(const View& view) const; 
		Vector2f mapPixelToCoords(const Vector2f& point, const View& view = View());
		Vector2f mapCoordsToPixel(const Vector2f& point, const View& view = View());
		
		void setCursorPosition(const Vector2f& position);
		void setCursorPositionView(const Vector2f& position, const View& view);
		Vector2f getCursorPosition(U32 touchIndex = 0) const;
		Vector2f getCursorPositionView(const View& view, U32 touchIndex = 0);

		enum Cursor { Default = 0, None, Custom };
		Cursor getCursor() const;
		void setCursor(Cursor cursor);
		void hideCursor();
		void useDefaultCursor();
		void useCustomCursor(const std::string& texture, const sf::IntRect& textureRect = sf::IntRect(), const sf::Vector2f& origin = sf::Vector2f());
		void useCustomCursor(sf::Texture* texture, const std::string& textureData = "", const sf::IntRect& textureRect = sf::IntRect(), const sf::Vector2f& origin = sf::Vector2f());
		sf::Texture* getCursorTexture() const; 
		const std::string& getCursorTextureData() const;
		const sf::IntRect& getCursorTextureRect() const;
		const sf::Vector2f& getCursorOrigin() const;
		const sf::Vector2f& getCursorScale() const;
		float getCursorRotation() const;
		const sf::Color& getCursorColor() const;
		void setCursorTexture(const std::string& texture);
		void setCursorTexture(sf::Texture* texture, const std::string& textureData = "");
		void setCursorTextureRect(const sf::IntRect& textureRect);
		void setCursorOrigin(const sf::Vector2f& origin);
		void setCursorScale(const sf::Vector2f& scale);
		void setCursorRotation(float rotation);
		void setCursorColor(const sf::Color& color);

		sf::RenderWindow& getHandle();

		enSignal(onWindowResized, const Window*, U32, U32);
		enSignal(onWindowClosed, const Window*);
		enSignal(onWindowGainedFocus, const Window*);
		enSignal(onWindowLostFocus, const Window*);

	private:
		void init();

		void targetSignals(const sf::Event& event);

		class RWindow : public sf::RenderWindow
		{
			public:
				RWindow(en::Window& handle);

				virtual void onResize();
				void init();

			private:
				en::Window& mHandle;
		};

	private:
		RWindow mWindow;

		std::string mTitle;
		bool mVisible;
		bool mVerticalSyncEnabled;
		bool mKeyRepeatEnabled;
		float mJoystickThreshold;
		bool mMouseCursorGrabbed;
		sf::ContextSettings mSettings;
		bool mFullscreen;
		sf::VideoMode mFullscreenVideoMode;
		sf::VideoMode mNonFullscreenVideoMode;
		sf::Uint32 mNonFullscreenStyle;
		View mMainView;

		sf::Image mIcon;
		std::string mIconPath;

		Cursor mCursor;
		std::string mCursorTextureData;
		sf::Texture* mCursorTexture;
		bool mCursorTextureCreated;
		sf::Sprite mCursorSprite;
};

} // namespace en