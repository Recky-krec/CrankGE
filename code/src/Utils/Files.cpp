#include "Files.h"
#include "Utils/Debug.h"

namespace crank
{
std::string readFile(const std::string& path)
{
	std::string result;
	std::ifstream ist(path, std::ios::in);
	if (!ist)
	{
		//crank::Log::ReportingLevel(crank::Log::ERROR);
		LOGE << "\t[File]\n\tFailed to open at: " << path << std::endl;
	}

	std::string line;
	while (!ist.eof())
	{
		std::getline(ist, line);
		result += line + "\n";
	}

	return result;
}

unsigned int loadTexture(const std::string& path)
{
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrComponents;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		if (nrComponents == 1)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (nrComponents == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (nrComponents == 4)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		stbi_image_free(data);
	}
	else
	{
		//crank::Log::ReportingLevel(crank::Log::ERROR);
		LOGE << "\t[Texture]:\n\tFailed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return id;
}

}  // namespace crank
