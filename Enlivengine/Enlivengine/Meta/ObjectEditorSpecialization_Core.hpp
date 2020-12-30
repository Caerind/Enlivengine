#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>

#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/CameraComponent.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/PhysicComponent2D.hpp>

ENLIVE_CUSTOM_OBJECTEDITOR_SPECIALIZATION_DEFINITION(en::Entity)
ENLIVE_CUSTOM_OBJECTEDITOR_SPECIALIZATION_DEFINITION(en::EntityManager)
ENLIVE_CUSTOM_OBJECTEDITOR_SPECIALIZATION_DEFINITION(en::TransformComponent)
ENLIVE_CUSTOM_OBJECTEDITOR_SPECIALIZATION_DEFINITION(en::CameraComponent)
ENLIVE_CUSTOM_OBJECTEDITOR_SPECIALIZATION_DEFINITION(en::PhysicComponent2D)