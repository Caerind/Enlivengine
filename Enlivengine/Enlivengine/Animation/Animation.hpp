#pragma once

#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Resources/ResourceManager.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace en
{

class Animation : public Resource<Animation>
{
public:
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Animation); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }

	class Frame
	{
    public:
		Frame(const Rectu& rect, Time duration);

        const Rectu& GetRect() const { return mRect; }
        const Time& GetDuration() const { return mDuration; }

    private:
		Rectu mRect;
		Time mDuration;
	};

	class Clip
	{
    public:
		enum class Direction
		{
			Forward,
			Reverse,
			PingPong
		};

        Clip(const std::string& name, U32 from, U32 to, Direction direction);

        const std::string& GetName() const { return mName; }
        U32 GetHashedName() const { return mHashedName; }
        U32 GetFrom() const { return mFrom; }
        U32 GetTo() const { return mTo; }
        Direction GetDirection() const { return mDirection; }

        U32 GetFrameCount() const;
        U32 GetFrameIndex(U32 index) const;

    private:
		std::string mName;
		U32 mHashedName;
		U32 mFrom;
		U32 mTo;
		Direction mDirection;
	};

public:
	Animation();

	bool LoadFromFile(const std::string& filename);

	void AddFrame(const Animation::Frame& frame);
	void AddFrame(const Rectu& rect, Time duration);
	U32 GetFrameCount() const;
	const Animation::Frame& GetFrame(U32 index) const;
		
	void AddClip(const Animation::Clip& clip);
	void AddClip(const std::string& name, U32 from, U32 to, Animation::Clip::Direction direction);
	U32 GetClipCount() const;
	const Animation::Clip& GetClip(U32 index) const;

	/*
	// TODO : ANIMATION UPDATE
	TexturePtr GetTexture();
	const TexturePtr& GetTexture() const;
	*/

private:
	std::vector<Animation::Frame> mFrames;
	std::vector<Animation::Clip> mClips;
	// TODO : ANIMATION UPDATE
	//TexturePtr mTexture;
};

using AnimationPtr = ResourcePtr<Animation>;

class AnimationLoader
{
public:
	AnimationLoader() = delete;
	~AnimationLoader() = delete;

	static ResourceLoader<Animation> FromFile(const std::string& filename)
	{
		return ResourceLoader<Animation>([&filename](Animation& r)
		{
			const bool result = r.LoadFromFile(filename);
			r.SetLoaded(result);
			r.SetLoadInfo(ResourceLoadInfo(ResourceLoadInfo::File, filename));
			return result;
		});
	}
};

} // namespace en
