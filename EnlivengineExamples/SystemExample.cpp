#include <Enlivengine/System/Preprocessor.hpp>
#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/CompilerDetection.hpp>
#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/ByteUnits.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Endianness.hpp>
#include <Enlivengine/System/Debugger.hpp>
#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeTraits.hpp>

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>


class A {};
namespace en {
ENLIVE_DEFINE_TYPE_TRAITS_NAME(A);
} // namespace en

template <typename T>
class B {};
namespace en {
ENLIVE_DEFINE_TYPE_TRAITS_NAME_TEMPLATE(B);
} // namespace en

class C {};

int main()
{
#ifdef ENLIVE_ENABLE_LOG
	en::LogManager::GetInstance().Initialize();
#endif // ENLIVE_ENABLE_LOG

	LogInfo(en::LogChannel::System, 9, "ENLIVE_FUNCTION : %s", ENLIVE_FUNCTION);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_NAME : %s", ENLIVE_COMPILER_NAME);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_VERSION : %s", ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION));
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_STRING : %s", ENLIVE_COMPILER_STRING);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_PLATFORM_NAME : %s", ENLIVE_PLATFORM_NAME);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_PLATFORM_DESCRIPTION : %s", ENLIVE_PLATFORM_DESCRIPTION);

	LogInfo(en::LogChannel::System, 9, "A: %s", en::TypeName<A>::name);
	LogInfo(en::LogChannel::System, 9, "B<A>: %s", en::TypeName<B<A>>::name);
	LogInfo(en::LogChannel::System, 9, "B<en::U32>: %s", en::TypeName<B<en::U32>>::name);
	LogInfo(en::LogChannel::System, 9, "B<C>: %s", en::TypeName<B<C>>::name);



	return 0;
}
