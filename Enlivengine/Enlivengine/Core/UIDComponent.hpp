#pragma once

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Serializer.hpp>
#include <Enlivengine/Utils/ObjectEditor.hpp>

namespace en
{

using UID = U32;
static constexpr UID kInvalidUID = U32_Max;

class UIDComponent
{
public:
	UIDComponent();
	~UIDComponent();

	UIDComponent(UIDComponent&& other) noexcept;
	UIDComponent(const UIDComponent&) = delete;

	UIDComponent& operator=(UIDComponent&& other) noexcept;
	UIDComponent& operator=(const UIDComponent&) = delete;

	UID GetUID() const;

	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

private:
	friend class EntityManager;

	UID mUID;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::UIDComponent, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()