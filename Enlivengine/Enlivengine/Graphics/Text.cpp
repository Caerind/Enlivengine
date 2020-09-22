#include <Enlivengine/Graphics/Text.hpp>

namespace en
{

Text::Text()
	: mText()
	, mFont()
{
}

void Text::SetFont(FontPtr font)
{
	if (font.IsValid())
	{
		mText.setFont(font.Get());
		mFont = font;
	}
}

FontPtr Text::GetFont() const
{
	return mFont;
}

void Text::SetString(const std::string& string)
{
	mString = string;
	mText.setString(string);
}

const std::string& Text::GetString() const
{
	return mString;
}

void Text::SetCharacterSize(U32 characterSize)
{
	mText.setCharacterSize(static_cast<sf::Uint32>(characterSize));
}

U32 Text::GetCharacterSize() const
{
	return static_cast<U32>(mText.getCharacterSize());
}

void Text::SetLineSpacing(F32 lineSpacingFactor)
{
	mText.setLineSpacing(static_cast<float>(lineSpacingFactor));
}

F32 Text::GetLineSpacing() const
{
	return static_cast<F32>(mText.getLineSpacing());
}

void Text::SetLetterSpacing(F32 letterSpacingFactor)
{
	mText.setLetterSpacing(static_cast<float>(letterSpacingFactor));
}

F32 Text::GetLetterSpacing() const
{
	return static_cast<F32>(mText.getLetterSpacing());
}

void Text::SetStyle(U32 style)
{
	mText.setStyle(static_cast<sf::Uint32>(style));
}

U32 Text::GetStyle() const
{
	return static_cast<U32>(mText.getStyle());
}

void Text::SetFillColor(Color color)
{
	mText.setFillColor(toSF(color));
}

Color Text::GetFillColor() const
{
	return toEN(mText.getFillColor());
}

void Text::SetOutlineColor(Color color)
{
	mText.setOutlineColor(toSF(color));
}

Color Text::GetOutlineColor() const
{
	return toEN(mText.getOutlineColor());
}

void Text::SetOutlineThickness(F32 thickness)
{
	mText.setOutlineThickness(static_cast<float>(thickness));
}

F32 Text::GetOutlineThickness() const
{
	return static_cast<F32>(mText.getOutlineThickness());
}

Rectf Text::GetLocalBounds() const
{
	return toEN(mText.getLocalBounds());
}

Rectf Text::GetGlobalBounds() const
{
	// TODO : Text::GetGlobalBounds()
	enAssert(false);
	return GetLocalBounds();
}

void Text::Render(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!mString.empty() && mFont.HasValidID())
	{
		states.transform *= toSF(GetMatrix());
		target.draw(mText, states);
	}
}

} // namespace en
