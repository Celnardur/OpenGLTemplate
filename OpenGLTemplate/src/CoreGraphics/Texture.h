#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include "GLIncludes.h"

class Texture
{
public:
	Texture();
	Texture(const std::string & strFile,
		int iBorderCondition = GL_REPEAT, int iFilterParam = GL_NEAREST);
	Texture(const std::vector<unsigned char> & vchData, int width, int height, int nChannels,
		int iBorderCondition = GL_REPEAT, int iFilterParam = GL_NEAREST);

	void activate(int iTextureUnit) const;
	void destroy();

private:
	unsigned int m_uidTexture;
	int m_iBorderCondition;
	int m_iFilterParam;

	void sendData(const unsigned char * data, int iWidth, int iHeight, int nChannels);
};

#endif
