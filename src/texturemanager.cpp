#include "texturemanager.h"

TextureManager::TextureManager(TextureManager&& other) noexcept {
    *this = std::move(other);
}

TextureManager& TextureManager::operator=(TextureManager&& other) noexcept {
    if(this != &other) {
        release();

        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;
    }

    return *this;
}

void TextureManager::init(const std::string& path) {
    release();

    m_id = 0;
    m_width = 0;
    m_height = 0;
    m_channels = 0;
    m_path = path;

    // Keep original atlas orientation: top of the tile stays at v=1 on the face.
    stbi_set_flip_vertically_on_load(false);

    unsigned char* data = stbi_load(
            path.c_str(),
            &m_width, &m_height, &m_channels, 0
        );
    if(data != NULL) {
        INFO("Success to load image %s", m_path.c_str());
    } else {
        ERROR("Failed to load image %s", m_path.c_str());
    }

    GLenum texFormat = GL_RGB;
    if(m_channels == 1) texFormat = GL_RED;
    if(m_channels == 2) texFormat = GL_RG;
    if(m_channels == 3) texFormat = GL_RGB;
    if(m_channels == 4) texFormat = GL_RGBA;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Use nearest-neighbor filtering for sharp pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Clamp edges to avoid texture bleeding (optional)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
                GL_TEXTURE_2D, 
                0,
                texFormat,
                m_width,
                m_height,
                0,
                texFormat,
                GL_UNSIGNED_BYTE,
                data
            );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void TextureManager::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void TextureManager::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureManager::release() {
    if(m_id != 0) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}


void TextureManager::destroy() {

}
