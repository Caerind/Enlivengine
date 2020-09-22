#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

#include <Enlivengine/Application/ResourceManager.hpp>

namespace en
{

// Image
class Image : public Resource<Image>, public sf::Image
{
public:
	~Image() {}
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Image); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }
};

// Texture
class Texture : public Resource<Texture>, public sf::Texture 
{
public:
	~Texture() {}
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Texture); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }
};

// Font
class Font : public Resource<Font>, public sf::Font
{
public:
	~Font() {}
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Font); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }
};

// SoundBuffer
class SoundBuffer : public Resource<SoundBuffer>, public sf::SoundBuffer
{
public:
	~SoundBuffer() {}
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Sound); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }
};

// MusicBuffer
class MusicBuffer : public Resource<MusicBuffer>, public sf::Music
{
public:
	~MusicBuffer() {}
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Music); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }
};

// Shader
class Shader : public Resource<Shader>, public sf::Shader
{
public:
	~Shader() {}
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Shader); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }
};


namespace priv
{
	template <class T>
	class SFMLResourcesLoader
	{
	public:
		SFMLResourcesLoader() = delete;
		~SFMLResourcesLoader() = delete;

		static ResourceLoader<T> FromFile(const std::string& filename)
		{
			return ResourceLoader<T>([&filename](T& r)
			{
				const bool result = r.loadFromFile(filename);
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(true, filename));
				return result;
			});
		}
	};

	class SFMLMusicBufferLoader
	{
	public:
		SFMLMusicBufferLoader() = delete;
		~SFMLMusicBufferLoader() = delete;

		static ResourceLoader<MusicBuffer> FromFile(const std::string& filename)
		{
			return ResourceLoader<MusicBuffer>([&filename](MusicBuffer& r)
			{
				const bool result = r.openFromFile(filename);
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(true, filename));
				return result;
			});
		}
	};

	/*
	class SFMLShaderLoader
	{
	public:
		SFMLShaderLoader() = delete;
		~SFMLShaderLoader() = delete;

		static ResourceLoader<Shader> FromFile(const std::string& filename, sf::Shader::Type shaderType)
		{
			return ResourceLoader<Shader>([&filename, &shaderType](Shader& r)
			{
				const bool result = r.loadFromFile(filename, shaderType);
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(true, filename));
				return result;
			});
		}

		static ResourceLoader<Shader> FromFiles(const std::string& vertexShader, const std::string& fragmentShader)
		{
			return ResourceLoader<Shader>([&vertexShader, &fragmentShader](Shader& r)
			{
				const bool result = r.loadFromFile(vertexShader, fragmentShader);
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(true, vertexShader + '\n' + fragmentShader));
				return result;
			});
		}
	};
	*/

} // namespace priv


using TexturePtr = ResourcePtr<Texture>;
using FontPtr = ResourcePtr<Font>;
using ImagePtr = ResourcePtr<Image>;
using SoundBufferPtr = ResourcePtr<SoundBuffer>;
using MusicBufferPtr = ResourcePtr<MusicBuffer>;
using ShaderPtr = ResourcePtr<Shader>;

using TextureLoader = priv::SFMLResourcesLoader<Texture>;
using FontLoader = priv::SFMLResourcesLoader<Font>;
using ImageLoader = priv::SFMLResourcesLoader<Image>;
using SoundBufferLoader = priv::SFMLResourcesLoader<SoundBuffer>;
using MusicBufferLoader = priv::SFMLMusicBufferLoader;
//using ShaderLoader = priv::SFMLShaderLoader; // TODO : ShaderLoader

} // namespace en
