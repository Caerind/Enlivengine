#include <Enlivengine/Tools/ImGuiHelper.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui_internal.h>
#include <imgui-sfml/imgui-SFML.h>

#include <Enlivengine/Graphics/SFMLWrapper.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace ImGui
{

void DisabledButton(const char* text)
{
	if (text == nullptr)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	ImGui::Button(text);
	ImGui::PopStyleVar();
}

void PreviewTexture(const en::Texture& texture, en::F32 maxSize, bool canGrow)
{
	sf::Sprite previewSprite;
	previewSprite.setTexture(texture);

	en::Vector2f spriteSize;
	spriteSize.x = static_cast<en::F32>(texture.getSize().x);
	spriteSize.y = static_cast<en::F32>(texture.getSize().y);

	en::F32 scale = 1.0f;
	if (spriteSize.x > maxSize || (canGrow && spriteSize.x < maxSize))
	{
		scale = maxSize / spriteSize.x;
	}
	if (spriteSize.y > maxSize || (canGrow && spriteSize.y < maxSize))
	{
		if (spriteSize.y * scale > maxSize)
		{
			scale = maxSize / spriteSize.y;
		}
	}
	previewSprite.setScale(scale, scale);
	ImGui::Image(previewSprite);
}

void PreviewTexture(const en::Texture& texture, const en::Rectu& textureRect, en::F32 maxSize, bool canGrow)
{
	sf::Sprite previewSprite;
	previewSprite.setTexture(texture);

	previewSprite.setTextureRect(en::toSF(textureRect));
	const en::Vector2f spriteSize = static_cast<en::Vector2f>(textureRect.getSize());

	en::F32 scale = 1.0f;
	if (spriteSize.x > maxSize || (canGrow && spriteSize.x < maxSize))
	{
		scale = maxSize / spriteSize.x;
	}
	if (spriteSize.y > maxSize || (canGrow && spriteSize.y < maxSize))
	{
		if (spriteSize.y * scale > maxSize)
		{
			scale = maxSize / spriteSize.y;
		}
	}
	previewSprite.setScale(scale, scale);
	ImGui::Image(previewSprite);
}

void PreviewAnimationClip(const en::Animation& animation, en::F32 maxSize, const en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale /*= 1.0f*/)
{
    en::U32 clipIndexCopy = clipIndex;
    PreviewAnimation(animation, maxSize, clipIndexCopy, clipFrameIndex, accumulator, speedScale);

	/*
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		const en::Animation::Clip& clip = animation.GetClip(clipIndex);
		ImGui::Text("%s\nFrom: %d\nTo: %d", clip.GetName().c_str(), clip.GetFrom(), clip.GetTo());
		for (en::U32 i = 0; i < clip.GetFrameCount(); i++)
		{
			ImGui::Text("%d", clip.GetFrameIndex(i));
		}

		ImGui::EndTooltip();
	}
	*/
}

void PreviewAnimation(const en::Animation& animation, en::F32 maxSize, en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale /*= 1.0f*/)
{
	if (!animation.GetTexture().IsValid())
	{
		ImGui::TextColored(en::Color::Orange.toImGuiColor(), "Invalid texture");
	}

	bool animationValid = (animation.GetClipCount() > 0 && animation.GetFrameCount() > 0);
	const en::Texture& texture = animation.GetTexture().Get();

	if (animationValid)
	{
		if (clipIndex >= animation.GetClipCount())
		{
			clipIndex = 0;
		}
		if (animation.GetClip(clipIndex).GetFrameCount() <= 0)
		{
			animationValid = false;
		}
		if (clipFrameIndex >= animation.GetClip(clipIndex).GetFrameCount())
		{
			clipFrameIndex = 0;
		}
		if (animation.GetClip(clipIndex).GetFrameIndex(clipFrameIndex) > animation.GetFrameCount())
		{
			animationValid = false;
		}
	}

	if (animationValid)
	{
		const en::U32 frameIndex = animation.GetClip(clipIndex).GetFrameIndex(clipFrameIndex);
		const en::Animation::Frame& frame = animation.GetFrame(frameIndex);

		ImGui::PreviewTexture(texture, frame.GetRect(), maxSize, true);

		accumulator += en::Time::Seconds(ImGui::GetIO().DeltaTime * speedScale);
		if (accumulator > frame.GetDuration())
		{
			accumulator -= frame.GetDuration();
			clipFrameIndex++;
			if (animation.GetClip(clipIndex).GetFrameCount() <= clipFrameIndex)
			{
				clipIndex = (clipIndex + 1) % animation.GetClipCount();
				clipFrameIndex = 0;
			}
		}
	}
	if (!animationValid)
	{
		ImGui::TextColored(en::Color::Orange.toImGuiColor(), "Invalid animation");
		ImGui::PreviewTexture(texture, maxSize);
	}
}

} // namespace ImGui

#endif // ENLIVE_ENABLE_IMGUI
