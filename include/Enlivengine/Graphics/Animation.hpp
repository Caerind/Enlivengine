#pragma once

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>

#include <Enlivengine/Math/Rect.hpp>

namespace en
{

class Animation
{
	public:
		struct Frame
		{
			Frame();
			Frame(ResourceId texture, const Recti& rect, Time duration);

			ResourceId texture;
			Recti rect;
			Time duration;
		};

	public:
		Animation();

		void addFrame(const Animation::Frame& frame = Animation::Frame());
		void addFrame(ResourceId texture, const Recti& rect, Time duration);

		U32 getFrameCount() const;

		Animation::Frame& getFrame(U32 index);
		const Animation::Frame& getFrame(U32 index) const;

		void removeFrame(U32 index);

		void removeAllFrames();

		Time getDuration() const;

	private:
		std::vector<Animation::Frame> mFrames;
};

} // namespace en
