#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_META
#ifdef ENLIVE_MODULE_GRAPHICS

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Meta/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Graphics/Sprite.hpp>

//////////////////////////////////////////////////////////////////
// en::AABB
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Sprite)
	ENLIVE_META_CLASS_MEMBER("texture", &en::Sprite::GetTexture, &en::Sprite::SetTexture),
	ENLIVE_META_CLASS_MEMBER("textureRect", &en::Sprite::GetTextureRect, &en::Sprite::SetTextureRect)
ENLIVE_META_CLASS_END()

#endif // ENLIVE_MODULE_GRAPHICS
#endif // ENLIVE_MODULE_META
