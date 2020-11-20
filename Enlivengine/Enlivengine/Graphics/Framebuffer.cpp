#include <Enlivengine/Graphics/Framebuffer.hpp>

#include <Enlivengine/Graphics/BgfxWrapper.hpp>

namespace en
{

Framebuffer Framebuffer::sDefaultFramebuffer;
enSlotType(Window, OnResized) Framebuffer::sResizeWindow;

Framebuffer::Framebuffer()
	: mSize()
	, mFramebuffer(BGFX_INVALID_HANDLE)
{
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
	: mSize(other.mSize)
	, mFramebuffer(other.mFramebuffer)
{
	other.mSize = Vector2u(0, 0);
	other.mFramebuffer = BGFX_INVALID_HANDLE;
}

Framebuffer::Framebuffer(const Vector2u& size)
	: Framebuffer()
{
	Create(size);
}

Framebuffer::~Framebuffer()
{
	if (BgfxWrapper::IsInitialized())
	{
		Destroy();
	}
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
	if (&other != this)
	{
		mSize = other.mSize;
		mFramebuffer = other.mFramebuffer;

		other.mSize = Vector2u(0, 0);
		other.mFramebuffer = BGFX_INVALID_HANDLE;
	}
	return *this;
}

void Framebuffer::Create(const Vector2u& size)
{
	enAssert(!IsValid());
	enAssert(!IsDefaultFramebuffer());

	mSize = size;
	mFramebuffer = bgfx::createFrameBuffer(static_cast<U16>(size.x), static_cast<U16>(size.y), bgfx::TextureFormat::RGBA8);
	OnResized(this, mSize);
}

void Framebuffer::Destroy()
{
	if (IsValid() && !IsDefaultFramebuffer())
	{
		bgfx::destroy(mFramebuffer);
		mFramebuffer = BGFX_INVALID_HANDLE;
		mSize = Vector2u(0, 0);
	}
}

bool Framebuffer::IsValid() const
{
	return bgfx::isValid(mFramebuffer) || IsDefaultFramebuffer();
}

bool Framebuffer::IsDefaultFramebuffer() const
{
	return this == &sDefaultFramebuffer;
}

void Framebuffer::Resize(const Vector2u& size)
{
	Destroy();
	Create(size);
}

const Vector2u& Framebuffer::GetSize() const
{
	return mSize;
}

bgfx::TextureHandle Framebuffer::GetTexture() const
{
	if (IsValid())
	{
		return bgfx::getTexture(mFramebuffer);
	}
	else
	{
		return BGFX_INVALID_HANDLE;
	}
}

bgfx::FrameBufferHandle Framebuffer::GetHandle() const
{
	return mFramebuffer;
}

Framebuffer& Framebuffer::GetDefaultFramebuffer()
{
	return sDefaultFramebuffer;
}

void Framebuffer::ResizeDefaultFramebuffer(const Vector2u& size)
{
	sDefaultFramebuffer.mSize = size;
	bgfx::reset(size.x, size.y);
}

} // namespace en