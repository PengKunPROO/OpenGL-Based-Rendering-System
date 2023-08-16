#include"Texture.h"
#include<ImageLib/stb_image.h>
Texture::Texture()
	:Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR) {
	glGenTextures(1, &this->ID);
}
void Texture::Generate(GLuint width, GLuint height, unsigned char* data, TextureType type,int indice) {
	switch (type)
	{
	case TEXTURE_2D:
		this->Generate_Texture_2D(width, height, data,indice);
		break;
	case TEXTURE_CUBE:
		this->Generate_Texture_CUBE(width, height, data,indice);
		break;
	default:
		break;
	}
	
}
void Texture::Generate_Texture_2D(GLuint width, GLuint height, unsigned char* data,int indice)
{
	this->Width = width;
	this->Height = height;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	//参数1：指定纹理坐标
	//参数2：为纹理制定多级渐远纹理的级别
	//参数3：告诉OpenGL将纹理储存为何种格式
	//参数4，5：指定纹理高度和宽度
	//参数6：总为0
	//参数7，8：定义1源图的格式和数据类型如 GL_RGBA和unsignde_byte
	//参数9：图像数据
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	//设置纹理过滤模式和wrap轴,2D纹理只有S,T轴
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	//解绑纹理
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::Generate_Texture_CUBE(GLuint width, GLuint height, unsigned char* data,int indice)
{
	this->Width = width;
	this->Height = height;

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + indice,0,this->Internal_Format,width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE,data);
}
void Texture::Bind(TextureType type) const {
	switch (type)
	{
	case TEXTURE_2D:
		glBindTexture(GL_TEXTURE_2D, this->ID);
		break;
	case TEXTURE_CUBE:
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
		break;
	default:
		break;
	}
	
}