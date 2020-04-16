#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Graphics/Animation.hpp>

namespace ImGui
{

void DisabledButton(const char* text);

void PreviewTexture(const en::Texture& texture, en::F32 maxSize, bool canGrow = false);
void PreviewTexture(const en::Texture& texture, const en::Rectu& textureRect, en::F32 maxSize, bool canGrow = false);
void PreviewAnimationClip(const en::Animation& animation, en::F32 maxSize, const en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale = 1.0f);
void PreviewAnimation(const en::Animation& animation, en::F32 maxSize, en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale = 1.0f);

} // namespace ImGui

#endif // ENLIVE_ENABLE_IMGUI
