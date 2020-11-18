#include <Enlivengine/Graphics/Texture.hpp>

#include <cstdio>

#include <bx/allocator.h>
#include <bimg/decode.h>

namespace en
{

Texture::Texture()
	: mTexture(BGFX_INVALID_HANDLE)
	, mInfo()
{
}

Texture::~Texture()
{
	Destroy();
}

bool Texture::Initialize(const char* filename, U64 flags)
{
	if (bgfx::isValid(mTexture))
	{
		bgfx::destroy(mTexture);
	}

	mTexture = BGFX_INVALID_HANDLE;
	mInfo = bgfx::TextureInfo();

#ifdef ENLIVE_COMPILER_MSVC
	FILE* file;
	fopen_s(&file, filename, "rb");
#else
	FILE* file = fopen(filename, "rb");
#endif // ENLIVE_COMPILER_MSVC
	if (file == nullptr)
	{
		return false;
	}
	fseek(file, 0, SEEK_END);
	std::size_t size = static_cast<std::size_t>(ftell(file));
	fseek(file, 0, SEEK_SET);
	char* data = (char*)malloc(size);
	if (data == nullptr)
	{
		return false;
	}
	fread(data, 1, size, file);
	fclose(file);

	static bx::DefaultAllocator defaultAllocator;
	bx::AllocatorI* allocator = &defaultAllocator;
	bimg::ImageContainer* imageContainer = bimg::imageParse(allocator, (const void*)data, (uint32_t)size);
	if (imageContainer == nullptr)
	{
		return false;
	}

	const bgfx::Memory* mem = bgfx::makeRef(imageContainer->m_data, imageContainer->m_size, ImageReleaseCallback, imageContainer);
	free(data);

	if (imageContainer->m_cubeMap)
	{
		mTexture = bgfx::createTextureCube(
			uint16_t(imageContainer->m_width)
			, 1 < imageContainer->m_numMips
			, imageContainer->m_numLayers
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
			, flags
			, mem
		);
	}
	else if (imageContainer->m_depth > 1)
	{
		mTexture = bgfx::createTexture3D(
			uint16_t(imageContainer->m_width)
			, uint16_t(imageContainer->m_height)
			, uint16_t(imageContainer->m_depth)
			, 1 < imageContainer->m_numMips
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
			, flags
			, mem
		);
	}
	else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), flags))
	{
		mTexture = bgfx::createTexture2D(
			uint16_t(imageContainer->m_width)
			, uint16_t(imageContainer->m_height)
			, 1 < imageContainer->m_numMips
			, imageContainer->m_numLayers
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
			, flags
			, mem
		);
	}

	if (bgfx::isValid(mTexture))
	{
		bgfx::setName(mTexture, filename);

		bgfx::calcTextureSize(
			mInfo
			, uint16_t(imageContainer->m_width)
			, uint16_t(imageContainer->m_height)
			, uint16_t(imageContainer->m_depth)
			, imageContainer->m_cubeMap
			, 1 < imageContainer->m_numMips
			, imageContainer->m_numLayers
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
		);

		return true;
	}
	else
	{
		return false;
	}
}

void Texture::Destroy()
{
	if (bgfx::isValid(mTexture))
	{
		bgfx::destroy(mTexture);
		mTexture = BGFX_INVALID_HANDLE;
	}
}

bool Texture::IsValid() const
{
	return bgfx::isValid(mTexture);
}

U32 Texture::GetMemSize() const
{
	return mInfo.storageSize;
}

Vector3u Texture::GetSize() const
{
	return Vector3u(
		static_cast<U32>(mInfo.width),
		static_cast<U32>(mInfo.height),
		static_cast<U32>(mInfo.depth)
	);
}

U32 Texture::GetWidth() const
{
	return static_cast<U32>(mInfo.width);
}

U32 Texture::GetHeight() const
{
	return static_cast<U32>(mInfo.height);
}

U32 Texture::GetDepth() const
{
	return static_cast<U32>(mInfo.depth);
}

U32 Texture::GetLayers() const
{
	return static_cast<U32>(mInfo.numLayers);
}

U32 Texture::GetMips() const
{
	return static_cast<U32>(mInfo.numMips);
}

U32 Texture::GetBitsPerPixel() const
{
	return static_cast<U32>(mInfo.bitsPerPixel);
}

bool Texture::IsCubeMap() const
{
	return mInfo.cubeMap;
}

void Texture::ImageReleaseCallback(void* ptr, void* userData)
{
	BX_UNUSED(ptr);
	bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)userData;
	bimg::imageFree(imageContainer);
}

} // namespace en