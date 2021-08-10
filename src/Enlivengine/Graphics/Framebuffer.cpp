#include <Enlivengine/Graphics/Framebuffer.hpp>

#include <Enlivengine/Graphics/BgfxWrapper.hpp>

namespace en
{

Framebuffer Framebuffer::sDefaultFramebuffer;
enSlotType(Window, OnResized) Framebuffer::sResizeWindow;

Framebuffer::Framebuffer()
	: mSize()
	, mFramebuffer(BGFX_INVALID_HANDLE)
	, mDepthTexture(false)
{
}

Framebuffer::Framebuffer(const Vector2u& size, bool depth)
	: Framebuffer()
{
	Create(size, depth);
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
	: mSize(other.mSize)
	, mFramebuffer(other.mFramebuffer)
	, mTextures()
	, mDepthTexture(other.mDepthTexture)
{
	mTextures[0] = other.mTextures[0];
	mTextures[1] = other.mTextures[1];

	other.mSize = Vector2u(0, 0);
	other.mFramebuffer = BGFX_INVALID_HANDLE;
	other.mTextures[0] = BGFX_INVALID_HANDLE;
	other.mTextures[1] = BGFX_INVALID_HANDLE;
	other.mDepthTexture = false;
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
		mTextures[0] = other.mTextures[0];
		mTextures[1] = other.mTextures[1];
		mDepthTexture = other.mDepthTexture;

		other.mSize = Vector2u(0, 0);
		other.mFramebuffer = BGFX_INVALID_HANDLE;
		other.mTextures[0] = BGFX_INVALID_HANDLE;
		other.mTextures[1] = BGFX_INVALID_HANDLE;
		other.mDepthTexture = false;
	}
	return *this;
}

void Framebuffer::Create(const Vector2u& size, bool depth)
{
	enAssert(!IsValid());
	enAssert(!IsDefaultFramebuffer());

	mSize = size;
	OnResized(this, mSize);

	mTextures[0] = bgfx::createTexture2D(
		static_cast<U16>(size.x),
		static_cast<U16>(size.y),
		false,
		1,
		bgfx::TextureFormat::RGBA8,
		BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
	);

	if (depth)
	{
		const uint64_t depthTextureFlags = BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;
		const bgfx::TextureFormat::Enum depthFormat =
			bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::D16, depthTextureFlags) ? bgfx::TextureFormat::D16
			: bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::D24S8, depthTextureFlags) ? bgfx::TextureFormat::D24S8
			: bgfx::TextureFormat::D32
			;

		mTextures[1] = bgfx::createTexture2D(
			static_cast<U16>(size.x),
			static_cast<U16>(size.y),
			false,
			1,
			depthFormat,
			depthTextureFlags
		);
		mDepthTexture = true;
	}
	else
	{
		mTextures[1] = BGFX_INVALID_HANDLE;
		mDepthTexture = false;
	}

	mFramebuffer = bgfx::createFrameBuffer((mDepthTexture) ? 2 : 1, mTextures, true);
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
	Create(size, mDepthTexture);
}

const Vector2u& Framebuffer::GetSize() const
{
	return mSize;
}

bgfx::FrameBufferHandle Framebuffer::GetHandle() const
{
	return mFramebuffer;
}

bgfx::TextureHandle Framebuffer::GetTexture() const
{
	if (IsValid())
	{
		return mTextures[0];
	}
	else
	{
		return BGFX_INVALID_HANDLE;
	}
}

bool Framebuffer::HasDepthTexture() const
{
	return mDepthTexture;
}

bgfx::TextureHandle Framebuffer::GetDepthTexture() const
{
	if (IsValid() && HasDepthTexture())
	{
		return mTextures[1];
	}
	else
	{
		return BGFX_INVALID_HANDLE;
	}
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