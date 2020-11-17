#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

//#include <IconFontCppHeaders/IconsFontAwesome5.h> // TODO : Restore FontAwesome

#include <Enlivengine/Graphics/ImGuiWrapper.hpp>

#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Animation/Animation.hpp>

namespace ImGui
{

void DisabledButton(const char* text);

/*
// TODO : TOOLS UPDATE
void PreviewTexture(const en::Texture& texture, en::F32 maxSize, bool canGrow = false);
void PreviewTexture(const en::Texture& texture, const en::Rectu& textureRect, en::F32 maxSize, bool canGrow = false);
void PreviewAnimationClip(const en::Animation& animation, en::F32 maxSize, const en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale = 1.0f);
void PreviewAnimation(const en::Animation& animation, en::F32 maxSize, en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale = 1.0f);
*/

} // namespace ImGui

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
