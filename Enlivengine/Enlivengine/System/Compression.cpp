#include <Enlivengine/System/Compression.hpp>

#include <cstring>

#include <miniz/miniz.hpp>

namespace en
{

bool Compression::IsBase64(U8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

bool Compression::Encode64(const std::vector<U8>& input, std::string& output)
{
	output.clear();
	output.reserve(((input.size() / 3) + (input.size() % 3 > 0)) * 4);

	U32 temp = 0;
	auto it = input.begin();

	for (std::size_t i = 0; i < input.size() / 3; ++i)
	{
		temp = (*it++) << 16;
		temp += (*it++) << 8;
		temp += (*it++);
		output.append(1, kBase64Table[(temp & 0x00FC0000) >> 18]);
		output.append(1, kBase64Table[(temp & 0x0003F000) >> 12]);
		output.append(1, kBase64Table[(temp & 0x00000FC0) >> 6]);
		output.append(1, kBase64Table[(temp & 0x0000003F)]);
	}

	switch (input.size() % 3)
	{
	case 1:
		temp = (*it++) << 16;
		output.append(1, kBase64Table[(temp & 0x00FC0000) >> 18]);
		output.append(1, kBase64Table[(temp & 0x0003F000) >> 12]);
		output.append(2, kBase64PadCharacter);
		break;
	case 2:
		temp = (*it++) << 16;
		temp += (*it++) << 8;
		output.append(1, kBase64Table[(temp & 0x00FC0000) >> 18]);
		output.append(1, kBase64Table[(temp & 0x0003F000) >> 12]);
		output.append(1, kBase64Table[(temp & 0x00000FC0) >> 6]);
		output.append(1, kBase64PadCharacter);
		break;
	}
	return true;
}

bool Compression::Encode64String(const std::string& input, std::string& output)
{
	return Encode64(std::vector<U8>{ input.begin(), input.end() }, output);
}

bool Compression::Decode64(const std::string& input, std::vector<U8>& output)
{
	if (input.length() % 4)
	{
		return false;
	}

	U32 padding = 0;
	if (input.length())
	{
		if (input[input.length() - 1] == kBase64PadCharacter) padding++;
		if (input[input.length() - 2] == kBase64PadCharacter) padding++;
	}

	output.clear();
	output.reserve(((input.length() / 4) * 3) - padding);

	U32 temp = 0;
	auto it = input.begin();

	while (it < input.end())
	{
		for (std::size_t i = 0; i < 4; ++i)
		{
			temp <<= 6;
			if (*it >= 0x41 && *it <= 0x5A) temp |= *it - 0x41;
			else if (*it >= 0x61 && *it <= 0x7A) temp |= *it - 0x47;
			else if (*it >= 0x30 && *it <= 0x39) temp |= *it + 0x04;
			else if (*it == 0x2B)                temp |= 0x3E;
			else if (*it == 0x2F)                temp |= 0x3F;
			else if (*it == kBase64PadCharacter)
			{
				switch (input.end() - it)
				{
				case 1:
					output.push_back((temp >> 16) & 0x000000FF);
					output.push_back((temp >> 8) & 0x000000FF);
					return true;
				case 2:
					output.push_back((temp >> 10) & 0x000000FF);
					return true;
				default:
					return false;
				}
			}
			else
			{
				return false;
			}

			++it;
		}

		output.push_back((temp >> 16) & 0x000000FF);
		output.push_back((temp >> 8) & 0x000000FF);
		output.push_back((temp) & 0x000000FF);
	}

	return true;
}

bool Compression::Decode64String(const std::string& input, std::string& output)
{
	std::vector<en::U8> bytes;
	bool result = Decode64(input, bytes);
	output.assign(bytes.begin(), bytes.end());
	return result;
}

bool Compression::CompressZlib(const std::vector<U8>& input, std::vector<U8>& output)
{
	std::size_t outputSize = output.size();
	bool result = (mz_compress((unsigned char*)output.data(), (mz_ulong*)&outputSize, (const unsigned char*)input.data(), (mz_ulong)input.size()) == MZ_OK);
	if (result)
	{
		output.resize(outputSize);
	}
	return result;
}

bool Compression::DecompressZlib(const std::vector<U8>& input, std::vector<U8>& output)
{
	std::size_t outputSize = output.size();
	bool result = (mz_uncompress((unsigned char*)output.data(), (mz_ulong*)&outputSize, (const unsigned char*)input.data(), (mz_ulong)input.size()) == MZ_OK);
	if (result)
	{
		output.resize(outputSize);
	}
	return result;
}

} // namespace en
