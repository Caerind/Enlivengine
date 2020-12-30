#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Graphics/ImGuiWrapper.hpp>

#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Animation/Animation.hpp>

namespace ImGui
{

void DisabledButton(const char* text);

ImTextureID toId(bgfx::TextureHandle _handle, uint8_t _flags, uint8_t _mip);

void Image(bgfx::TextureHandle _handle, uint8_t _flags, uint8_t _mip, const ImVec2& _size, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f), const ImVec2& _uv1 = ImVec2(1.0f, 1.0f), const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f), const ImVec4& _borderCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
void Image(bgfx::TextureHandle _handle, const ImVec2& _size, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f), const ImVec2& _uv1 = ImVec2(1.0f, 1.0f), const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f), const ImVec4& _borderCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

bool ImageButton(bgfx::TextureHandle _handle, uint8_t _flags, uint8_t _mip, const ImVec2& _size, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f), const ImVec2& _uv1 = ImVec2(1.0f, 1.0f), int _framePadding = -1, const ImVec4& _bgCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f), const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
bool ImageButton(bgfx::TextureHandle _handle, const ImVec2& _size, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f), const ImVec2& _uv1 = ImVec2(1.0f, 1.0f), int _framePadding = -1, const ImVec4& _bgCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f), const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

/*
// TODO : TOOLS UPDATE
void PreviewTexture(const en::Texture& texture, en::F32 maxSize, bool canGrow = false);
void PreviewTexture(const en::Texture& texture, const en::Rectu& textureRect, en::F32 maxSize, bool canGrow = false);
void PreviewAnimationClip(const en::Animation& animation, en::F32 maxSize, const en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale = 1.0f);
void PreviewAnimation(const en::Animation& animation, en::F32 maxSize, en::U32& clipIndex, en::U32& clipFrameIndex, en::Time& accumulator, en::F32 speedScale = 1.0f);
*/

} // namespace ImGui

#endif // ENLIVE_ENABLE_IMGUI
