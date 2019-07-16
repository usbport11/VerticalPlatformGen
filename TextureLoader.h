#ifndef textureloaderH
#define textureloaderH

#include "Tga.h"
#include "Png.h"

class MTextureLoader
{
private:
	stTexture* pTexture;
	MTga* Tga;
	MPng* Png;
	unsigned char* pImageData;
	unsigned long ImageWidth;
	unsigned long ImageHeight;
	unsigned int BitsPerPixel;
	
	unsigned char FileType[1];
	bool LoadFile(const char* inFileName);
	bool CreateTexture(unsigned int& Id, unsigned long Width, unsigned long Height, unsigned char* Data);
	void FreeImage();
public:
	bool LinearMode;
	MTextureLoader();
	~MTextureLoader();
	stTexture* LoadTexture(const char* inFileName, unsigned int CountX, unsigned int CountY, unsigned int CountToRead, unsigned int& ReturnCount);
	bool DeleteTexture(stTexture* Texture, unsigned int Count);
	void Close();
};

#endif
