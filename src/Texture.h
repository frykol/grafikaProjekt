#pragma once


#include <string>

class Texture{
    private:
        unsigned int m_TextureID;
        unsigned int m_SpecularID;

        bool m_IsTexture = false;
        bool m_IsSpecular = false;

        int m_Width;
        int m_Height;
        int m_NrChannels;
    public:
        Texture();
        void compileTexture(const std::string& path);
        void compileSpecular(const std::string& path);
        void bind();
        void unBind();
}; 