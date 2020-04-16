#pragma once

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace en
{

class Animation : public Resource<Animation>
{
	public:
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

		TexturePtr GetTexture();
		const TexturePtr& GetTexture() const;

	private:
		std::vector<Animation::Frame> mFrames;
		std::vector<Animation::Clip> mClips;
		TexturePtr mTexture;
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
			r.mFilename = (result) ? filename : "";
			return result;
		});
	}
};

} // namespace en
