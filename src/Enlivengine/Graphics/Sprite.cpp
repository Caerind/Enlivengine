#include <Enlivengine/Graphics/Sprite.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Graphics/BgfxWrapper.hpp>

namespace en
{

bgfx::VertexLayout Sprite::Vertex::kLayout;
const U16 Sprite::kIndices[6] = { 0, 2, 1, 0, 3, 2 };
Shader Sprite::kShader;
bgfx::UniformHandle Sprite::kUniformTexture = BGFX_INVALID_HANDLE;
bgfx::IndexBufferHandle Sprite::kIndexBuffer = BGFX_INVALID_HANDLE;

Sprite::Sprite()
	: mVertices()
	, mTextureRect()
	, mBuffer(BGFX_INVALID_HANDLE)
	, mTexture()
{
}

Sprite::Sprite(Sprite&& other)
	: mVertices()
	, mTextureRect(other.mTextureRect)
	, mBuffer(other.mBuffer)
	, mTexture(other.mTexture)
{
	for (U32 i = 0; i < 4; ++i)
	{
		mVertices[i] = other.mVertices[i];
	}

	other.mTextureRect = Recti();
	other.mTexture.Release();
	other.mBuffer = BGFX_INVALID_HANDLE;

	UpdateBuffer();
}

Sprite::~Sprite()
{
	if (!BgfxWrapper::IsInitialized())
	{
		return;
	}

	if (bgfx::isValid(mBuffer))
	{
		bgfx::destroy(mBuffer);
		mBuffer = BGFX_INVALID_HANDLE;
	}
}

Sprite& Sprite::operator=(Sprite&& other)
{
	if (&other != this)
	{
		for (U32 i = 0; i < 4; ++i)
		{
			mVertices[i] = other.mVertices[i];
		}
		mTextureRect = other.mTextureRect;
		mBuffer = other.mBuffer;
		mTexture = other.mTexture;

		other.mTextureRect = Recti();
		other.mTexture.Release();
		other.mBuffer = BGFX_INVALID_HANDLE;

		UpdateBuffer();
	}
	return *this;
}

void Sprite::SetTexture(TexturePtr texture)
{
	if (mTexture != texture)
	{
		mTexture = texture;
		if (Texture* texturePtr = texture.GetPtr())
		{
			SetTextureRect(Recti(0, 0, texturePtr->GetWidth(), texturePtr->GetHeight()));
		}
	}
}

TexturePtr Sprite::GetTexture() const
{
	return mTexture;
}

void Sprite::SetTextureRect(const Recti& textureRect)
{
	if (mTextureRect != textureRect)
	{
		mTextureRect = textureRect;
		UpdateVertices();
	}
}

const Recti& Sprite::GetTextureRect() const
{
	return mTextureRect;
}

Rectf Sprite::GetLocalBounds() const
{
	return Rectf(0.0f, 0.0f, 1.0f, -1.0f);
}

Rectf Sprite::GetGlobalBounds() const
{
	// TODO : Transform
	return GetLocalBounds();
}

bool Sprite::CanRender() const
{
	if (BgfxWrapper::IsInitialized() && bgfx::isValid(mBuffer) && mTexture.IsValid() && kShader.IsValid())
	{
		Texture& texture = mTexture.Get();
		if (texture.IsValid())
		{
			return true;
		}
	}
	return false;
}

void Sprite::Render() const
{
	if (BgfxWrapper::IsInitialized() && bgfx::isValid(mBuffer) && mTexture.IsValid() && kShader.IsValid())
	{
		Texture& texture = mTexture.Get();
		if (texture.IsValid())
		{
			// Common to all sprites
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_BLEND_ALPHA_TO_COVERAGE);
			bgfx::setIndexBuffer(kIndexBuffer);

			// Specific to this sprite
			bgfx::setVertexBuffer(0, mBuffer);
			bgfx::setTexture(0, kUniformTexture, texture.GetHandle());
			kShader.Submit(BgfxWrapper::GetCurrentView());
		}
	}
}

void Sprite::UpdateVertices()
{
	const Rectf localBounds = GetLocalBounds();
	for (U32 i = 0; i < 4; ++i)
	{
		mVertices[i].pos = localBounds.GetCorner(i);
	}

	if (Texture* texturePtr = mTexture.GetPtr())
	{
		if (texturePtr->IsValid())
		{
			// TODO : Find how to handle texture errors here
			enAssert(texturePtr->GetWidth() > 0);
			enAssert(texturePtr->GetHeight() > 0);
			const F32 oneOverTexWidth = 1.0f / static_cast<F32>(texturePtr->GetWidth());
			const F32 oneOverTexHeight = 1.0f / static_cast<F32>(texturePtr->GetHeight());
			const F32 left = static_cast<F32>(mTextureRect.Left()) * oneOverTexWidth;
			const F32 right = static_cast<F32>(mTextureRect.Right()) * oneOverTexWidth;
			const F32 top = static_cast<F32>(mTextureRect.Top()) * oneOverTexHeight;
			const F32 bottom = static_cast<F32>(mTextureRect.Bottom()) * oneOverTexHeight;

			mVertices[0].texCoords.Set(left, top);
			mVertices[1].texCoords.Set(right, top);
			mVertices[2].texCoords.Set(right, bottom);
			mVertices[3].texCoords.Set(left, bottom);
		}
	}

	UpdateBuffer();
}

void Sprite::UpdateBuffer()
{
	if (!BgfxWrapper::IsInitialized())
	{
		return;
	}

	// TODO : Use dynamic vertex buffer instead ?
	// TODO : => Give the choise to the user using template boolean parameter
	if (bgfx::isValid(mBuffer))
	{
		bgfx::destroy(mBuffer);
	}
	mBuffer = bgfx::createVertexBuffer(bgfx::makeRef(mVertices, sizeof(mVertices)), Vertex::kLayout);
}

bool Sprite::InitializeSprites()
{
	enAssert(BgfxWrapper::IsInitialized());

	Vertex::kLayout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();

	if (!kShader.Initialize("sprite.vs.bin", "sprite.fs.bin"))
	{
		enLogWarning(LogChannel::Graphics, "Can't initialize Sprites shader");
		return false;
	}

	kUniformTexture = bgfx::createUniform("s_uniformTexture", bgfx::UniformType::Sampler);
	if (!bgfx::isValid(kUniformTexture))
	{
		enLogWarning(LogChannel::Graphics, "Can't initialize Sprites uniform texture sampler");
		return false;
	}

	kIndexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(Sprite::kIndices, sizeof(Sprite::kIndices)));
	if (!bgfx::isValid(kIndexBuffer))
	{
		enLogWarning(LogChannel::Graphics, "Can't initialize Sprites index buffer");
		return false;
	}

	return true;
}

bool Sprite::ReleaseSprites()
{
	if (!BgfxWrapper::IsInitialized())
	{
		return true;
	}

	if (bgfx::isValid(kIndexBuffer))
	{
		bgfx::destroy(kIndexBuffer);
	}
	if (bgfx::isValid(kUniformTexture))
	{
		bgfx::destroy(kUniformTexture);
	}
	kShader.Destroy();
	return true;
}

} // namespace en