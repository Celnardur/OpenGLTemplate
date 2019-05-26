#include "Texture.h"
#include "../utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{
	m_uidTexture = 0;
	m_iBorderCondition = GL_REPEAT;
	m_iFilterParam = GL_NEAREST;
}

Texture::Texture(const std::string& strFile,
	int iBorderCondition, int iFilterParam)
{
	m_iBorderCondition = iBorderCondition;
	m_iFilterParam = iFilterParam;
	m_uidTexture = 0;

	int iWidth, iHeight, nChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char * data = stbi_load(strFile.c_str(), &iWidth, &iHeight, &nChannels, 0);

	if (data)
	{
		this->sendData(data, iWidth, iHeight, nChannels);
	}
	else
	{
		std::cout << "Failed to load texture " << strFile.c_str() << std::endl;
	}
	stbi_image_free(data);
}

Texture::Texture(const std::vector<unsigned char>& vchData, int width, int height, int nChannels,
	int iBorderCondition, int iFilterParam)
{
	myAssert(width * height * nChannels == int(vchData.size()), "Size of data mismatch");

	m_iBorderCondition = iBorderCondition;
	m_iFilterParam = iFilterParam;
	m_uidTexture = 0;
	this->sendData(&(vchData[0]), width, height, nChannels);
}

void Texture::sendData(const unsigned char* data, int iWidth, int iHeight, int nChannels)
{
	myAssert(nChannels < 5, "Can't have more than 4 color channels");
	myAssert(nChannels > 0, "Can't have zero or negitive color channels");

	glGenTextures(1, &m_uidTexture);
	glBindTexture(GL_TEXTURE_2D, m_uidTexture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_iBorderCondition);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_iBorderCondition);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iFilterParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iFilterParam);

	int idFlag;
	switch (nChannels)
	{
	case 1:
		idFlag = GL_RED;
		break;
	case 2:
		idFlag = GL_RG;
		break;
	case 3:
		idFlag = GL_RGB;
		break;
	default:
		idFlag = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, idFlag, iWidth, iHeight,
		0, idFlag, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activate(int iTextureUnit) const
{
	myAssert(iTextureUnit < 16, "Cannot have more than 16 textures active");

	glActiveTexture(GL_TEXTURE0 + iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_uidTexture);
}

void Texture::destroy()
{
	glDeleteTextures(1, &m_uidTexture);
}
