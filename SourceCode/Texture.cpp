//#include "Texture.h"
//
//Texture::Texture(GLenum TextureTarget, const std::string& FileName, bool hasAlpha = false)
//{
//    // ---------
//    glGenTextures(1, &TextureTarget);
//    glBindTexture(GL_TEXTURE_2D, TextureTarget);
//    // set the texture wrapping parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // load image, create texture and generate mipmaps
//    int width, height, nrChannels;
//    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
//    unsigned char* data = stbi_load(FileName.c_str(), &width, &height, &nrChannels, 0);
//    if (data)
//    {
//        if (hasAlpha)
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        else
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        LOG("Failed to load texture: -> ", FileName);
//    }
//    stbi_image_free(data);
//}
//
//bool Texture::Load()
//{
//    return false;
//}
//
//void Texture::Bind(GLenum TextureUnit)
//{
//}
