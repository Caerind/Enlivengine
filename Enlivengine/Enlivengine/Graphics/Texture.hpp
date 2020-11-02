#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_GRAPHICS

#include <bgfx/bgfx.h>

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Resources/ResourceManager.hpp>

namespace en
{

class Texture : public Resource<Texture>
{
public:
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Texture); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }

	Texture();
	~Texture();

	bool Initialize(const char* filename, U64 flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE);
	void Destroy();

	bool IsValid() const;

	U32 GetMemSize() const;
	Vector3u GetSize() const;
	U32 GetWidth() const;
	U32 GetHeight() const;
	U32 GetDepth() const;
	U32 GetLayers() const;
	U32 GetMips() const;
	U32 GetBitsPerPixel() const;
	bool IsCubeMap() const;

	// TODO : Remove/Clean
	bgfx::TextureHandle GetHandle() const { return mTexture; }

private:
	static void ImageReleaseCallback(void* ptr, void* userData);

private:
	bgfx::TextureHandle mTexture;
	bgfx::TextureInfo mInfo;
};

class TextureLoader
{
public:
	TextureLoader() = delete;

	static ResourceLoader<Texture> FromFile(const std::string& filename)
	{
		return ResourceLoader<Texture>([&filename](Texture& r)
			{
				const bool result = r.Initialize(filename.c_str());
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(true, filename));
				return result;
			});
	}
};

using TexturePtr = ResourcePtr<Texture>;

} // namespace en

#endif // ENLIVE_MODULE_GRAPHICS