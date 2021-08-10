#pragma once

#include <soloud.h>

#include <Enlivengine/Resources/ResourceManager.hpp>
#include <Enlivengine/Audio/AudioSource.hpp>

namespace en
{

class AudioSourceWav : public AudioSource, public Resource<AudioSourceWav>
{
public:
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::AudioSourceWav); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }

	AudioSourceWav();
	AudioSourceWav(const AudioSourceWav& other) = delete;
	AudioSourceWav(AudioSourceWav&& other);
	~AudioSourceWav();

	AudioSourceWav& operator=(const AudioSourceWav& other) = delete;
	AudioSourceWav& operator=(AudioSourceWav&& other);

	bool Load(const char* filename);
}; 

class AudioSourceWavLoader
{
public:
	AudioSourceWavLoader() = delete;

	static ResourceLoader<AudioSourceWav> FromFile(const std::string& filename)
	{
		return ResourceLoader<AudioSourceWav>([&filename](AudioSourceWav& r)
			{
				const bool result = r.Load(filename.c_str());
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(ResourceLoadInfo::File, filename));
				return result;
			});
	}
};

using AudioSourceWavPtr = ResourcePtr<AudioSourceWav>;

} // namespace en