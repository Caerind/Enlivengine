#pragma once

#if defined(ENLIVE_STRINGIFY_EX)
	#undef ENLIVE_STRINGIFY_EX
#endif
#define ENLIVE_STRINGIFY_EX(x) #x

#if defined(ENLIVE_STRINGIFY)
	#undef ENLIVE_STRINGIFY
#endif
#define ENLIVE_STRINGIFY(x) ENLIVE_STRINGIFY_EX(x)

#if defined(ENLIVE_CONCAT_EX)
	#undef ENLIVE_CONCAT_EX
#endif
#define ENLIVE_CONCAT_EX(a, b) a ## b

#if defined(ENLIVE_CONCAT)
	#undef ENLIVE_CONCAT
#endif
#define ENLIVE_CONCAT(a, b) ENLIVE_CONCAT_EX(a, b)

#if defined(ENLIVE_VERSION_ENCODE)
	#undef ENLIVE_VERSION_ENCODE
#endif
#define ENLIVE_VERSION_ENCODE(major, minor, revision) (((major) * 1000000) + ((minor) * 1000) + (revision))

#if defined(ENLIVE_VERSION_DECODE_MAJOR)
	#undef ENLIVE_VERSION_DECODE_MAJOR
#endif
#define ENLIVE_VERSION_DECODE_MAJOR(version) ((version) / 1000000)

#if defined(ENLIVE_VERSION_DECODE_MINOR)
	#undef ENLIVE_VERSION_DECODE_MINOR
#endif
#define ENLIVE_VERSION_DECODE_MINOR(version) (((version) % 1000000) / 1000)

#if defined(ENLIVE_VERSION_DECODE_REVISION)
	#undef ENLIVE_VERSION_DECODE_REVISION
#endif
#define ENLIVE_VERSION_DECODE_REVISION(version) ((version) % 1000)

#if defined(ENLIVE_UNUSED)
#undef ENLIVE_UNUSED
#endif
#define ENLIVE_UNUSED(var) (void(var))
