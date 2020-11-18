#pragma once

#define ENLIVE_STRINGIFY_EX(x) #x
#define ENLIVE_STRINGIFY(x) ENLIVE_STRINGIFY_EX(x)

#define ENLIVE_CONCAT_EX(a, b) a ## b
#define ENLIVE_CONCAT(a, b) ENLIVE_CONCAT_EX(a, b)

#define ENLIVE_VERSION_ENCODE(major, minor, revision) (((major) * 1000000) + ((minor) * 1000) + (revision))
#define ENLIVE_VERSION_DECODE_MAJOR(version) ((version) / 1000000)
#define ENLIVE_VERSION_DECODE_MINOR(version) (((version) % 1000000) / 1000)
#define ENLIVE_VERSION_DECODE_REVISION(version) ((version) % 1000)

#define ENLIVE_UNUSED(var) (void(var))
