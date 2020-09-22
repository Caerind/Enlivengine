#include <Enlivengine/Graphics/Animation.hpp>

#include <fstream>
#include <filesystem>
#include <json/json.hpp>

namespace en
{

Animation::Frame::Frame(const Rectu& rect, Time duration)
	: mRect(rect)
	, mDuration(duration)
{
}

Animation::Clip::Clip(const std::string& name, U32 from, U32 to, Direction direction)
	: mName(name)
	, mHashedName(Hash::SlowHash(name.c_str()))
	, mFrom(from)
	, mTo(to)
	, mDirection(direction)
{
}

U32 Animation::Clip::GetFrameCount() const
{
   	enAssert(mTo >= mFrom);
    const U32 distance = (mTo - mFrom);
    if (mDirection != Direction::PingPong)
    {
        return distance + 1;
    }
    else
    {
        return distance * 2 + 1;
    }
}

U32 Animation::Clip::GetFrameIndex(U32 index) const
{
	enAssert(index < GetFrameCount());
    if (mDirection == Direction::Forward)
    {
        return mFrom + index;
    }
    else if (mDirection == Direction::Reverse)
    {
        return mTo - index;
    }
    else if (mDirection == Direction::PingPong)
    {
        if (index <= mTo)
        {
            return mFrom + index;
        }
        else
        {
            return 2 * mTo - index;
        }
    }
	enAssert(false);
    return 0;
}

Animation::Animation()
{
}

bool Animation::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (file)
	{
		nlohmann::json document;
		// Don't know why, but it seems the operator>> is not recognized...
		nlohmann::detail::parser<nlohmann::json>(nlohmann::detail::input_adapter(file)).parse(false, document);

		if (!document.is_null())
		{
			nlohmann::json::iterator itr = document.find("frames");
			if (itr != document.end())
			{
				for (auto& frame : itr.value())
				{
					const Rectu rect{ { frame["frame"]["x"].get<U32>(), frame["frame"]["y"].get<U32>() }, { frame["frame"]["w"].get<U32>(), frame["frame"]["h"].get<U32>() } };
					const Time duration{ Time::Milliseconds(frame["duration"].get<I32>()) };
					AddFrame(rect, duration);
				}

				// NlohmannJson sort node at the same level alphabetically... And we don't want that here.
				// We want to keep the same order like in Aseprite to be used for the clips
				if (GetFrameCount() > 0)
				{
					sort(mFrames.begin(), mFrames.end(), [](const Frame& a, const Frame& b)
					{
						if (a.GetRect().top() != b.GetRect().top())
						{
							return a.GetRect().top() < b.GetRect().top();
						}
						else
						{
							return a.GetRect().left() < b.GetRect().left();
						}
					});
				}
			}

			itr = document.find("meta");
			if (itr != document.end())
			{
				nlohmann::json meta = itr.value();
				for (auto& frameTag : meta["frameTags"])
				{
					const std::string name = frameTag["name"].get<std::string>();
					const U32 from = frameTag["from"].get<U32>();
					const U32 to = frameTag["to"].get<U32>();
					const std::string directionStr = frameTag["direction"].get<std::string>();
					Clip::Direction direction = Clip::Direction::Forward;
					if (directionStr == "forward")
					{
						direction = Clip::Direction::Forward;
					}
					else if (directionStr == "reverse")
					{
						direction = Clip::Direction::Reverse;
					}
					else if (directionStr == "pingpong")
					{
						direction = Clip::Direction::PingPong;
					}
					AddClip(name, from, to, direction);
				}

				std::string path = std::filesystem::path(filename).remove_filename().string();
				const std::string filepath = path + meta["image"].get<std::string>();
				mTexture = ResourceManager::GetInstance().GetFromFilename<Texture>(filepath);
				if (!mTexture.IsValid())
				{
					enLogWarning(en::LogChannel::Animation, "ResourceDependencyNeeded: {} from {}", filepath.c_str(), filename.c_str());
					mTexture = ResourceManager::GetInstance().Create<Texture>(GetIdentifier() + "-texture", TextureLoader::FromFile(filepath));
					if (!mTexture.IsValid())
					{
						enLogError(en::LogChannel::Animation, "Can't load tileset texture : {}", filepath.c_str());
					}
				}
			}

			return true;
		}
	}
	return false;
}

void Animation::AddFrame(const Animation::Frame& frame)
{
	mFrames.push_back(frame);
}

void Animation::AddFrame(const Rectu& rect, Time duration)
{
	mFrames.emplace_back(rect, duration);
}

U32 Animation::GetFrameCount() const
{
	return static_cast<U32>(mFrames.size());
}

const Animation::Frame& Animation::GetFrame(U32 index) const
{
	enAssert(index < GetFrameCount());
	return mFrames[index];
}

void Animation::AddClip(const Animation::Clip& clip)
{
	mClips.push_back(clip);
}

void Animation::AddClip(const std::string& name, U32 from, U32 to, Animation::Clip::Direction direction)
{
	mClips.emplace_back(name, from, to, direction);
}

U32 Animation::GetClipCount() const
{
	return static_cast<U32>(mClips.size());
}

const Animation::Clip& Animation::GetClip(U32 index) const
{
	enAssert(index < GetClipCount());
	return mClips[index];
}

TexturePtr Animation::GetTexture()
{
	return mTexture;
}

const TexturePtr& Animation::GetTexture() const
{
	return mTexture;
}

} // namespace en
