#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>

#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>

#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/World.hpp>

ENLIVE_CUSTOM_DATAFILE_SPECIALIZATION_DEFINITION(en::Entity)
ENLIVE_CUSTOM_DATAFILE_SPECIALIZATION_DEFINITION(en::EntityManager)
ENLIVE_CUSTOM_DATAFILE_SPECIALIZATION_DEFINITION(en::World)
