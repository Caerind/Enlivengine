
set(SRC_SYSTEM
    Enlivengine/Config.hpp
    Enlivengine/System/Array.hpp
    Enlivengine/System/Assert.hpp
    Enlivengine/System/ByteUnits.hpp
    Enlivengine/System/CallOnExit.hpp
    Enlivengine/System/ClassManager.cpp
    Enlivengine/System/ClassManager.hpp
    Enlivengine/System/CompilerDetection.hpp
    Enlivengine/System/CompilerTraits.hpp
    Enlivengine/System/Compression.cpp
    Enlivengine/System/Compression.hpp
    Enlivengine/System/DateTime.cpp
    Enlivengine/System/DateTime.hpp
    Enlivengine/System/Debugger.cpp
    Enlivengine/System/Debugger.hpp
    Enlivengine/System/Endianness.hpp
    Enlivengine/System/Hash.hpp
    Enlivengine/System/Log.cpp
    Enlivengine/System/Log.hpp
    Enlivengine/System/Macros.hpp
    Enlivengine/System/MemoryAllocator.cpp
    Enlivengine/System/MemoryAllocator.hpp
    Enlivengine/System/Meta.hpp
    Enlivengine/System/MetaEnum.hpp
    Enlivengine/System/NonCopyable.hpp
    Enlivengine/System/ParserXml.cpp
    Enlivengine/System/ParserXml.hpp
    Enlivengine/System/PlatformDetection.hpp
    Enlivengine/System/PlatformTraits.hpp
    Enlivengine/System/Preprocessor.hpp
    Enlivengine/System/PrimitiveTypes.hpp
    Enlivengine/System/Profiler.cpp
    Enlivengine/System/Profiler.hpp
    Enlivengine/System/Signal.hpp
    Enlivengine/System/Singleton.hpp
    Enlivengine/System/String.cpp
    Enlivengine/System/String.hpp
    Enlivengine/System/Time.cpp
    Enlivengine/System/Time.hpp
    Enlivengine/System/TypeInfo.hpp
    Enlivengine/System/TypeTraits.hpp
)
source_group("System" FILES ${SRC_SYSTEM})

set(SRC_MATH
	Enlivengine/Math/AABB.cpp
	Enlivengine/Math/AABB.hpp
	Enlivengine/Math/Color.cpp
	Enlivengine/Math/Color.hpp
	Enlivengine/Math/Distribution.hpp
	Enlivengine/Math/Frustum.cpp
	Enlivengine/Math/Frustum.hpp
	Enlivengine/Math/Intersection.cpp
	Enlivengine/Math/Intersection.hpp
	Enlivengine/Math/MathString.hpp
	Enlivengine/Math/Matrix3.hpp
	Enlivengine/Math/Matrix4.hpp
	Enlivengine/Math/Noise.cpp
	Enlivengine/Math/Noise.hpp
	Enlivengine/Math/OBB.cpp
	Enlivengine/Math/OBB.hpp
	Enlivengine/Math/Plane.cpp
	Enlivengine/Math/Plane.hpp
	Enlivengine/Math/Quaternion.hpp
	Enlivengine/Math/Random.cpp
	Enlivengine/Math/Random.hpp
	Enlivengine/Math/Ray.cpp
	Enlivengine/Math/Ray.hpp
	Enlivengine/Math/Rect.hpp
	Enlivengine/Math/Sphere.cpp
	Enlivengine/Math/Sphere.hpp
	Enlivengine/Math/Utilities.hpp
	Enlivengine/Math/Vector2.hpp
	Enlivengine/Math/Vector3.hpp
	Enlivengine/Math/Vector4.hpp
)
source_group("Math" FILES ${SRC_MATH})

set(SRC_GRAPHICS
	Enlivengine/Graphics/Animation.cpp
	Enlivengine/Graphics/Animation.hpp
	Enlivengine/Graphics/AnimationController.cpp
	Enlivengine/Graphics/AnimationController.hpp
	Enlivengine/Graphics/AnimationStateMachine.cpp
	Enlivengine/Graphics/AnimationStateMachine.hpp
	Enlivengine/Graphics/ColorGradient.hpp
	Enlivengine/Graphics/DebugDraw.cpp
	Enlivengine/Graphics/DebugDraw.hpp
	Enlivengine/Graphics/ScreenshotSystem.cpp
	Enlivengine/Graphics/ScreenshotSystem.hpp
	Enlivengine/Graphics/SFMLResources.cpp
	Enlivengine/Graphics/SFMLResources.hpp
	Enlivengine/Graphics/SFMLWrapper.cpp
	Enlivengine/Graphics/SFMLWrapper.hpp
	Enlivengine/Graphics/Sprite.cpp
	Enlivengine/Graphics/Sprite.hpp
	Enlivengine/Graphics/Text.cpp
	Enlivengine/Graphics/Text.hpp
	Enlivengine/Graphics/View.cpp
	Enlivengine/Graphics/View.hpp
)
source_group("Graphics" FILES ${SRC_GRAPHICS})

set(SRC_MAP
	Enlivengine/Map/EllipseObject.cpp
	Enlivengine/Map/EllipseObject.hpp
	Enlivengine/Map/LayerBase.cpp
	Enlivengine/Map/LayerBase.hpp
	Enlivengine/Map/Map.cpp
	Enlivengine/Map/Map.hpp
	Enlivengine/Map/ObjectBase.cpp
	Enlivengine/Map/ObjectBase.hpp
	Enlivengine/Map/ObjectGroup.cpp
	Enlivengine/Map/ObjectGroup.hpp
	Enlivengine/Map/PointObject.cpp
	Enlivengine/Map/PointObject.hpp
	Enlivengine/Map/PolygonObject.cpp
	Enlivengine/Map/PolygonObject.hpp
	Enlivengine/Map/PropertyHolder.cpp
	Enlivengine/Map/PropertyHolder.hpp
	Enlivengine/Map/RectangleObject.cpp
	Enlivengine/Map/RectangleObject.hpp
	Enlivengine/Map/TileLayer.cpp
	Enlivengine/Map/TileLayer.hpp
	Enlivengine/Map/Tileset.cpp
	Enlivengine/Map/Tileset.hpp
)
source_group("Map" FILES ${SRC_MAP})
	
set(SRC_APPLICATION
	Enlivengine/Application/ActionSystem.cpp
	Enlivengine/Application/ActionSystem.hpp
	Enlivengine/Application/Application.cpp
	Enlivengine/Application/Application.hpp
	Enlivengine/Application/AudioSystem.cpp
	Enlivengine/Application/AudioSystem.hpp
	Enlivengine/Application/ImGuiToolManager.cpp
	Enlivengine/Application/ImGuiToolManager.hpp
	Enlivengine/Application/PathManager.cpp
	Enlivengine/Application/PathManager.hpp
	Enlivengine/Application/ResourceManager.cpp
	Enlivengine/Application/ResourceManager.hpp
	Enlivengine/Application/ResourceManager.inl
	Enlivengine/Application/StateManager.cpp
	Enlivengine/Application/StateManager.hpp
	Enlivengine/Application/Window.cpp
	Enlivengine/Application/Window.hpp
)
source_group("Application" FILES ${SRC_APPLICATION})

set(SRC_CORE
	Enlivengine/Core/ComponentManager.cpp
	Enlivengine/Core/ComponentManager.hpp
	Enlivengine/Core/Components.hpp
	Enlivengine/Core/CustomTraits.hpp
	Enlivengine/Core/DataFile.cpp
	Enlivengine/Core/DataFile.hpp
	Enlivengine/Core/DataFileSpecialization.inl
	Enlivengine/Core/Entity.cpp
	Enlivengine/Core/Entity.hpp
	Enlivengine/Core/EntityManager.cpp
	Enlivengine/Core/EntityManager.hpp
	Enlivengine/Core/EntityManagerSpecialization.hpp
	Enlivengine/Core/EntitySpecialization.hpp
	Enlivengine/Core/ObjectEditor.cpp
	Enlivengine/Core/ObjectEditor.hpp
	Enlivengine/Core/ObjectEditorSpecialization.inl
	Enlivengine/Core/PhysicComponent.cpp
	Enlivengine/Core/PhysicComponent.hpp
	Enlivengine/Core/PhysicSystem.cpp
	Enlivengine/Core/PhysicSystem.hpp
	Enlivengine/Core/System.cpp
	Enlivengine/Core/System.hpp
	Enlivengine/Core/Transform.cpp
	Enlivengine/Core/Transform.hpp
	Enlivengine/Core/Universe.cpp
	Enlivengine/Core/Universe.hpp
	Enlivengine/Core/World.cpp
	Enlivengine/Core/World.hpp
)
source_group("Core" FILES ${SRC_CORE})

set(SRC_TOOLS
	Enlivengine/Tools/ImGuiAnimationEditor.cpp
	Enlivengine/Tools/ImGuiAnimationEditor.hpp
	Enlivengine/Tools/ImGuiConsole.cpp
	Enlivengine/Tools/ImGuiConsole.hpp
	Enlivengine/Tools/ImGuiDemoWindow.cpp
	Enlivengine/Tools/ImGuiDemoWindow.hpp
	Enlivengine/Tools/ImGuiEntityBrowser.cpp
	Enlivengine/Tools/ImGuiEntityBrowser.hpp
	Enlivengine/Tools/ImGuiLogger.cpp
	Enlivengine/Tools/ImGuiLogger.hpp
	Enlivengine/Tools/ImGuiHelper.cpp
	Enlivengine/Tools/ImGuiHelper.hpp
	Enlivengine/Tools/ImGuiInputEditor.cpp
	Enlivengine/Tools/ImGuiInputEditor.hpp
	Enlivengine/Tools/ImGuiMemoryDebugger.cpp
	Enlivengine/Tools/ImGuiMemoryDebugger.hpp
	Enlivengine/Tools/ImGuiPhysic.cpp
	Enlivengine/Tools/ImGuiPhysic.hpp
	Enlivengine/Tools/ImGuiProfiler.cpp
	Enlivengine/Tools/ImGuiProfiler.hpp
	Enlivengine/Tools/ImGuiResourceBrowser.cpp
	Enlivengine/Tools/ImGuiResourceBrowser.hpp
)
source_group("Tools" FILES ${SRC_TOOLS})

add_library(Enlivengine STATIC
	${SRC_SYSTEM} 
	${SRC_MATH} 
	${SRC_GRAPHICS}
	${SRC_MAP}
	${SRC_APPLICATION}
	${SRC_CORE}
	${SRC_TOOLS}
)
if (MSVC)
	target_compile_options(Enlivengine PRIVATE /W4 /WX)
else()
	target_compile_options(Enlivengine PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(Enlivengine PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Enlivengine PUBLIC EnlivengineThirdParty)
set_target_properties(Enlivengine PROPERTIES FOLDER "Enlivengine")
