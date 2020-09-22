#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>
#include <Enlivengine/Core/Transform.hpp>

namespace en
{

class Text : public Transform
{
public:
	Text();

	void SetFont(FontPtr font);
	FontPtr GetFont() const;

	void SetString(const std::string& string);
	const std::string& GetString() const;

	void SetCharacterSize(U32 characterSize);
	U32 GetCharacterSize() const;

	void SetLineSpacing(F32 lineSpacingFactor);
	F32 GetLineSpacing() const;

	void SetLetterSpacing(F32 letterSpacingFactor);
	F32 GetLetterSpacing() const;

	void SetStyle(U32 style);
	U32 GetStyle() const;

	void SetFillColor(Color color);
	Color GetFillColor() const;

	void SetOutlineColor(Color color);
	Color GetOutlineColor() const;

	void SetOutlineThickness(F32 thickness);
	F32 GetOutlineThickness() const;

	Rectf GetLocalBounds() const;
	Rectf GetGlobalBounds() const;

	void Render(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text mText;
	std::string mString;
	FontPtr mFont;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Text)
	ENLIVE_META_CLASS_MEMBER("Position", &en::Text::GetPosition, &en::Text::SetPosition),
	ENLIVE_META_CLASS_MEMBER("Rotation", &en::Text::GetRotation2D, &en::Text::SetRotation2D),
	ENLIVE_META_CLASS_MEMBER("Scale", &en::Text::GetScale, &en::Text::SetScale),
	ENLIVE_META_CLASS_MEMBER("Font", &en::Text::GetFont, &en::Text::SetFont),
	ENLIVE_META_CLASS_MEMBER("String", &en::Text::GetString, &en::Text::SetString),
	ENLIVE_META_CLASS_MEMBER("CharacterSize", &en::Text::GetCharacterSize, &en::Text::SetCharacterSize),
	ENLIVE_META_CLASS_MEMBER("LineSpacing", &en::Text::GetLineSpacing, &en::Text::SetLineSpacing),
	ENLIVE_META_CLASS_MEMBER("LetterSpacing", &en::Text::GetLetterSpacing, &en::Text::SetLetterSpacing),
	ENLIVE_META_CLASS_MEMBER("Style", &en::Text::GetStyle, &en::Text::SetStyle),
	ENLIVE_META_CLASS_MEMBER("FillColor", &en::Text::GetFillColor, &en::Text::SetFillColor),
	ENLIVE_META_CLASS_MEMBER("OutlineColor", &en::Text::GetOutlineColor, &en::Text::SetOutlineColor),
	ENLIVE_META_CLASS_MEMBER("OutlineThickness", &en::Text::GetOutlineThickness, &en::Text::SetOutlineThickness)
ENLIVE_META_CLASS_END()