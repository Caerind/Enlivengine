#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>
#include <Enlivengine/Core/Transform.hpp>

// TODO : Color ?

namespace en
{

class Sprite : public Transform
{
public:
	Sprite();

	void SetTexture(TexturePtr texture);
	TexturePtr GetTexture() const;

	void SetTextureRect(const Recti& textureRect);
	const Recti& GetTextureRect() const;

	Rectf GetLocalBounds() const;
	Rectf GetGlobalBounds() const;

	void Render(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void UpdatePositions();
	void UpdateTexCoords();

private:
	sf::Vertex mVertices[4];
	TexturePtr mTexture;
	Recti mTextureRect;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Sprite)
	ENLIVE_META_CLASS_MEMBER("Position", &en::Sprite::GetPosition, &en::Sprite::SetPosition),
	ENLIVE_META_CLASS_MEMBER("Rotation", &en::Sprite::GetRotation2D, &en::Sprite::SetRotation2D),
	ENLIVE_META_CLASS_MEMBER("Scale", &en::Sprite::GetScale, &en::Sprite::SetScale),
	ENLIVE_META_CLASS_MEMBER("Texture", &en::Sprite::GetTexture, &en::Sprite::SetTexture),
	ENLIVE_META_CLASS_MEMBER("TextureRect", &en::Sprite::GetTextureRect, &en::Sprite::SetTextureRect)
ENLIVE_META_CLASS_END()