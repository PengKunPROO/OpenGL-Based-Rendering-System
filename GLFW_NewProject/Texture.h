#pragma once
#include"OpenGLINC.h"
enum Texture2D_Type
{
    RGB,
    RGBA,
    RED
};
enum TextureType
{
    TEXTURE_2D,
    TEXTURE_CUBE
};
class Texture
{
public:
    
	
	GLuint ID;
    GLuint Width, Height; // Width and height of loaded image in pixels
   // Texture Format
    GLenum Internal_Format; // Format of texture object
    GLenum Image_Format; // Format of loaded image
    // Texture configuration
    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
    std::string name;
    std::string type;
    std::string path;
    Texture();
    void Bind(TextureType type) const;
    void Generate(GLuint width, GLuint height, unsigned char* data, TextureType type,int indice=0);
private:
    // Constructor (sets default texture modes)   
    // Generates texture from image data
     
    void Generate_Texture_2D(GLuint width, GLuint height, unsigned char* data,int indice=0);
    void Generate_Texture_CUBE(GLuint width, GLuint height, unsigned char* data,int indice=0);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    
    
private:
	
};

