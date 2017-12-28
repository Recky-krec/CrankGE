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
		LOGEL << "[FILE]:";
		LOGE << "Failed to open at: " << path;
	}

	std::string line;
	while (!ist.eof())
	{
		std::getline(ist, line);
		result += line + "\n";
	}

	return result;
}

void copyFileByte(const std::string& src, const std::string& destination)
{
    // 100s for 4.3GB file
	std::ifstream ifs(src, std::ios_base::binary);
	std::ofstream ofs(destination, std::ios_base::binary);

	if(!ifs) {
		LOGEL << "[FILE]:";
		LOGE << "Failed to open at: " << src;
	} if(!ofs) {
		LOGEL << "[FILE]:";
		LOGE << "Failed to open at: " << destination;
	}

    ifs.seekg(0, ifs.end);
    long long length = ifs.tellg();
    ifs.seekg(0, ifs.beg);

	std::vector<int> v;
    v.reserve(length);

    LOGIL << "[FILE]";
    LOGI << "Reading " << length << " characters... ";

    // read from binary file
    for(int x; ifs.read(asBytes(x), sizeof(int)); )
		v.push_back(x);

    if(ifs)
        LOGI << "all characters read successfully.";
    else
    {
        LOGEL << "[ERROR]";
        LOGE << "Only " << ifs.gcount() << " could be read";
    }

	// write to binary file
	for(int x : v)
		ofs.write(asBytes(x), sizeof(x));
}

void copyFileBlock(const std::string& src, const std::string& destination)
{
    // 80s for 4.3GB file
    std::ifstream ifs(src, std::ios_base::binary);
    std::ofstream ofs(destination, std::ios_base::binary);

	if(!ifs) {
		LOGEL << "[FILE]:";
		LOGE << "Failed to open at: " << src;
	} if(!ofs) {
		LOGEL << "[FILE]:";
		LOGE << "Failed to open at: " << destination;
	}

    ifs.seekg(0, ifs.end);
    long long length = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    char* buffer = new char [length];

    LOGIL << "[FILE]";
    LOGI << "Reading " << length << " characters... ";
    ifs.read(buffer, length);

    if(ifs)
        LOGI << "all characters read successfully.";
    else
    {
        LOGEL << "[ERROR]";
        LOGE << "Only " << ifs.gcount() << " could be read";
    }

    ofs.write(buffer, length);
}

template<typename T>
char* asBytes(T& var)
{
	void* addr = &var;
	return static_cast<char*>(addr);
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
		LOGEL << "[TEXTURE]:";
		LOGE << "Failed to load at path: " << path;
		stbi_image_free(data);
	}

	return id;
}

unsigned int loadCubemap(const std::vector<std::string>& faces)
{
	// faces order: right, left, top, bottom, back, front
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, nrChannels;

	for(int i=0; i<faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if(data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			LOGE << "[CUBEMAP]:";
			LOGE << "Texture failed to load at path: " << faces[i];
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return id;
}


}  // namespace crank
