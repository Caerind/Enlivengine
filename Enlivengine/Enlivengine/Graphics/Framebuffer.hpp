#pragma once

#include <bgfx/bgfx.h>

#include <Enlivengine/Utils/Signal.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Window/Window.hpp>

namespace en
{

class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(const Vector2u& size, bool depth = true);
	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer(const Framebuffer& other) = delete;
	~Framebuffer();

	Framebuffer& operator=(Framebuffer&& other) noexcept;
	Framebuffer& operator=(const Framebuffer& other) = delete;

	void Create(const Vector2u& size, bool depth = true);
	void Destroy();
	bool IsValid() const;

	bool IsDefaultFramebuffer() const;

	void Resize(const Vector2u& size);
	const Vector2u& GetSize() const;
	enSignal(OnResized, const Framebuffer*, const Vector2u&);

	bgfx::FrameBufferHandle GetHandle() const;

	bgfx::TextureHandle GetTexture() const;

	bool HasDepthTexture() const;
	bgfx::TextureHandle GetDepthTexture() const;

	static Framebuffer& GetDefaultFramebuffer();

private:
	Vector2u mSize;
	bgfx::FrameBufferHandle mFramebuffer;
	bgfx::TextureHandle mTextures[2];
	bool mDepthTexture;

private:
	friend class BgfxWrapper;
	static Framebuffer sDefaultFramebuffer;
	static enSlotType(Window, OnResized) sResizeWindow;
	static void ResizeDefaultFramebuffer(const Vector2u& size);
};

} // namespace en