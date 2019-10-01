#include <Enlivengine/Graphics/Animation.hpp>

namespace en
{

Animation::Frame::Frame()
	: texture(0)
	, rect(Recti())
	, duration(Time::Zero)
{
}

Animation::Frame::Frame(ResourceId texture, const Recti& rect, Time duration)
	: texture(texture)
	, rect(rect)
	, duration(duration)
{
}

Animation::Animation()
{
}

void Animation::addFrame(const Animation::Frame& frame)
{
	mFrames.push_back(frame);
}

void Animation::addFrame(ResourceId texture, const Recti& rect, Time duration)
{
	mFrames.emplace_back(texture, rect, duration);
}

U32 Animation::getFrameCount() const
{
	return mFrames.size();
}

Animation::Frame& Animation::getFrame(U32 index)
{
	return mFrames.at(index);
}

const Animation::Frame& Animation::getFrame(U32 index) const
{
	return mFrames.at(index);
}

void Animation::removeFrame(U32 index)
{
	if (0 <= index && index < mFrames.size())
	{
		mFrames.erase(index + mFrames.begin());
	}
}

void Animation::removeAllFrames()
{
	mFrames.clear();
}

Time Animation::getDuration() const
{
	Time duration;
	for (const auto& frame : mFrames)
	{
		duration += frame.duration;
	}
	return duration;
}

} // namespace en