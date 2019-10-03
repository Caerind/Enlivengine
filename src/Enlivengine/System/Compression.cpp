#include <Enlivengine/System/Compression.hpp>

#include <miniz/miniz.h>
#include <miniz/miniz.c> // TODO : This is weird na ?

#include <cstring>

namespace en
{

const std::string Compression::mBase64Table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                              "abcdefghijklmnopqrstuvwxyz"
                                              "0123456789+/";

bool Compression::encode64(std::string& data)
{
    U32 count = 0;
    U8 input_bytes[3] = { '\0', '\0', '\0' };
    U8 byte_array[4];
    std::string result;
    for (U32 i = 0; i < data.size(); ++i)
    {
        input_bytes[count++] = data[i];
        if (count == 3 || (i == data.size() - 1))
        {
            byte_array[0] = input_bytes[0] >> 2;
            byte_array[1] = ((input_bytes[0] & 0x3) << 4) | (input_bytes[1] >> 4);
            byte_array[2] = ((input_bytes[1] & 0xf) << 2) | (input_bytes[2] >> 6);
            byte_array[3] = input_bytes[2] & 0x3f;
            std::memset(input_bytes, '\0', 3);
			for (U32 j = 0; j < count + 1; j++)
			{
				result += mBase64Table[byte_array[j]];
			}
			if (count != 3)
			{
				for (U32 i = count; i < 3; ++i)
				{
					result += '=';
				}
			}
			else
			{
				count = 0;
			}
        }
    }
    data = result;
    return true;
}

bool Compression::decode64(std::string& data)
{
    U32 count = 0;
    U8 input_bytes[4] = { '\0', '\0', '\0', '\0' };
    U8 byte_array[3];
    std::string result;
    for (U32 i = 0; i < data.size(); ++i)
    {
		if (isBase64(data[i]))
		{
			input_bytes[count++] = static_cast<U8>(mBase64Table.find(data[i]));
		}
        if (count == 4 || data[i] == '=')
        {
            byte_array[0] = (input_bytes[0] << 2) | ((input_bytes[1] & 0x30) >> 4);
            byte_array[1] = ((input_bytes[1] & 0xf) << 4) | ((input_bytes[2] & 0x3c) >> 2);
            byte_array[2] = ((input_bytes[2] & 0x3) << 6) | input_bytes[3];
            std::memset(input_bytes, '\0', 4);
			for (U32 j = 0; j < count - 1; j++)
			{
				result += byte_array[j];
			}
			if (count != 4)
			{
				break;
			}
			else
			{
				count = 0;
			}
        }
    }
    data = result;
    return true;
}

bool Compression::compress(std::string& data)
{
    mz_stream zs; // mz_stream is zlib's control structure
    std::memset(&zs, 0, sizeof(zs));
    if (mz_deflateInit(&zs, MZ_BEST_COMPRESSION) != MZ_OK)
    {
        return false;
    }
    zs.next_in = (U8*)data.data();
    zs.avail_in = data.size(); // set the z_stream's input
    I32 ret;
    char outbuffer[32768];
    std::string outstring;
    // retrieve the compressed bytes blockwise
    do
    {
        zs.next_out = reinterpret_cast<unsigned char*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = mz_deflate(&zs, MZ_FINISH);
        if (outstring.size() < zs.total_out)
        {
            // append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == MZ_OK);
    mz_deflateEnd(&zs);
    if (ret != MZ_STREAM_END) // an error occurred that was not EOF
    {
        return false;
    }
    data = outstring;
    return true;
}

bool Compression::decompress(std::string& data)
{
	mz_stream zstream;
	zstream.zalloc = 0;
	zstream.zfree = 0;
	zstream.opaque = 0;
	zstream.next_in = const_cast<U8*>(reinterpret_cast<const U8*>(data.data()));
	zstream.avail_in = data.size();
	I32 result(mz_inflateInit(&zstream));
	if (result != MZ_OK)
	{
		return false;
	}
	char outbuffer[32768];
	std::string outstring;
	do
	{
		zstream.next_out = reinterpret_cast<U8*>(outbuffer);
		zstream.avail_out = sizeof(outbuffer);
		result = mz_inflate(&zstream, MZ_SYNC_FLUSH);
		switch (result)
		{
		case MZ_NEED_DICT:
		case MZ_STREAM_ERROR:
			result = MZ_DATA_ERROR;
		case MZ_DATA_ERROR:
		case MZ_MEM_ERROR:
			mz_inflateEnd(&zstream);
			return false;
		}
		if (outstring.size() < zstream.total_out)
		{
			outstring.append(outbuffer, zstream.total_out - outstring.size());
		}
	} while (result != MZ_STREAM_END);
	if (zstream.avail_in != 0)
	{
		return false;
	}
	mz_inflateEnd(&zstream);
	data = outstring;
	return true;
}

bool Compression::compress64(std::string& data)
{
    std::string d(data);
    if (compress(d))
    {
        if (encode64(d))
        {
            data = d;
            return true;
        }
    }
    return false;
}

bool Compression::decompress64(std::string& data)
{
    std::string d(data);
    if (decode64(d))
    {
        if (decompress(d))
        {
            data = d;
            return true;
        }
    }
    return false;
}

bool Compression::isBase64(U8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

} // namespace en
