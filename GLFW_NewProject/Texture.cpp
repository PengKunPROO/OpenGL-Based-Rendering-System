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
	//����1��ָ����������
	//����2��Ϊ�����ƶ��༶��Զ����ļ���
	//����3������OpenGL��������Ϊ���ָ�ʽ
	//����4��5��ָ������߶ȺͿ��
	//����6����Ϊ0
	//����7��8������1Դͼ�ĸ�ʽ������������ GL_RGBA��unsignde_byte
	//����9��ͼ������
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	//�����������ģʽ��wrap��,2D����ֻ��S,T��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	//�������
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