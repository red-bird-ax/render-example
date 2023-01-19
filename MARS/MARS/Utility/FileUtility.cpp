#include <Utility/FileUtility.h>

#include <fstream>
#include <sstream>

std::string ReadText(const std::string& path)
{
	std::ifstream file{ path, std::ios::in };

	std::ostringstream stream;
	stream << file.rdbuf();

	return stream.str();
}

std::vector<uint8_t> ReadBytes(const std::string& path)
{
	std::ifstream file{ path, std::ios::binary };

	file.unsetf(std::ios::skipws);

	file.seekg(0, std::ios::end);
	std::streampos size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8_t> result;

	result.reserve(size);
	result.insert(
		result.begin(),
		std::istream_iterator<uint8_t>{ file},
		std::istream_iterator<uint8_t>{}
	);

	return result;
}

std::string ClipFileName(const std::string& path)
{
	uint32_t offset = path.find_last_of("/") + 1;
	if (offset == 0)
	{
		offset = path.find_last_of("\\") + 1;
	}

	const uint32_t count = path.find_first_of(".") - offset;
	return path.substr(offset, count);
}