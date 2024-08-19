#include <Enlivengine/Graphics/View.hpp>

namespace en
{

bool View::Serialize(Serializer& serializer, const char* name)
{
    ENLIVE_UNUSED(serializer);
    ENLIVE_UNUSED(name);
    // TODO
    return false;
}

bool View::Edit(ObjectEditor& objectEditor, const char* name)
{
    ENLIVE_UNUSED(objectEditor);
    ENLIVE_UNUSED(name);
    // TODO
    return false;
}

} // namespace en