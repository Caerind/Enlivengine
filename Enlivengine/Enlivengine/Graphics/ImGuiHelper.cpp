#include <Enlivengine/Graphics/ImGuiHelper.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui_internal.h>

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

ImTextureID toId(bgfx::TextureHandle _handle, uint8_t _flags, uint8_t _mip)
{
	union { struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; ImTextureID id; } tex;
	tex.s.handle = _handle;
	tex.s.flags = _flags;
	tex.s.mip = _mip;
	return tex.id;
}

void Image(bgfx::TextureHandle _handle
	, uint8_t _flags
	, uint8_t _mip
	, const ImVec2& _size
	, const ImVec2& _uv0
	, const ImVec2& _uv1
	, const ImVec4& _tintCol
	, const ImVec4& _borderCol
)
{
	Image(toId(_handle, _flags, _mip), _size, _uv0, _uv1, _tintCol, _borderCol);
}

void Image(bgfx::TextureHandle _handle
	, const ImVec2& _size
	, const ImVec2& _uv0
	, const ImVec2& _uv1
	, const ImVec4& _tintCol
	, const ImVec4& _borderCol
)
{
	Image(_handle, IMGUI_FLAGS_ALPHA_BLEND, 0, _size, _uv0, _uv1, _tintCol, _borderCol);
}

bool ImageButton(bgfx::TextureHandle _handle
	, uint8_t _flags
	, uint8_t _mip
	, const ImVec2& _size
	, const ImVec2& _uv0
	, const ImVec2& _uv1
	, int _framePadding
	, const ImVec4& _bgCol
	, const ImVec4& _tintCol
)
{
	return ImageButton(toId(_handle, _flags, _mip), _size, _uv0, _uv1, _framePadding, _bgCol, _tintCol);
}

bool ImageButton(bgfx::TextureHandle _handle
	, const ImVec2& _size
	, const ImVec2& _uv0
	, const ImVec2& _uv1
	, int _framePadding
	, const ImVec4& _bgCol
	, const ImVec4& _tintCol
)
{
	return ImageButton(_handle, IMGUI_FLAGS_ALPHA_BLEND, 0, _size, _uv0, _uv1, _framePadding, _bgCol, _tintCol);
}

/*
// TODO : TOOLS UPDATE
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
*/

/*
// TODO : TOOLS UPDATE
void PreviewTexture(const en::Texture& texture, const en::Rectu& textureRect, en::F32 maxSize, bool canGrow)
{
	sf::Sprite previewSprite;
	previewSprite.setTexture(texture);

	previewSprite.setTextureRect(en::toSF(textureRect));
	const en::Vector2f spriteSize = static_cast<en::Vector2f>(textureRect.GetSize());

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
*/

/*
// TODO : TOOLS UPDATE
void PreviewAnimationClip(const en::Animation& animation, en::F32 maxSize, const en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale)
{
    en::U32 clipIndexCopy = clipIndex;
    PreviewAnimation(animation, maxSize, clipIndexCopy, clipFrameIndex, accumulator, speedScale);
	*/

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

	/*
}
*/

/*
// TODO : TOOLS UPDATE
void PreviewAnimation(const en::Animation& animation, en::F32 maxSize, en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale)
{
	if (!animation.GetTexture().IsValid())
	{
		ImGui::TextColored(en::Colors::Orange.ToImGuiColor(), "Invalid texture");
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
		ImGui::TextColored(en::Colors::Orange.ToImGuiColor(), "Invalid animation");
		ImGui::PreviewTexture(texture, maxSize);
	}
}
*/

} // namespace ImGui

#endif // ENLIVE_ENABLE_IMGUI
