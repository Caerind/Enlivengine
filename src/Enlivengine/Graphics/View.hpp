#pragma once

#include <SFML/Graphics/View.hpp>

#include <Enlivengine/Utils/Signal.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace en
{

class View
{
public:
	View() = default;
    ~View() = default;

    bool Serialize(Serializer& serializer, const char* name);
    bool Edit(ObjectEditor& objectEditor, const char* name);

    sf::View& GetSFMLView() { return mView; }
    const sf::View& GetSFMLView() const { return mView; }

protected:
	sf::View mView;
};

} // namespace en

ENLIVE_DEFINE_TYPE_INFO(en::View, en::Type_CustomSerialization, en::Type_CustomEditor)