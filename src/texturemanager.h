#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <string>

#include "debug.h"

constexpr int stride = 16;
constexpr int atlasWidth = 256;
constexpr int atlasHeight = 256;
constexpr int padding = 1;

class TextureManager {
public:
    TextureManager() = default;
    explicit TextureManager(const std::string& path);
    ~TextureManager() = default;

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    
    TextureManager(TextureManager&&) noexcept;
    TextureManager& operator=(TextureManager&&) noexcept;

public:
    void init(const std::string& path);
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    int width() const { return m_width; }
    int height() const { return m_height; }
    unsigned int id() const { return m_id; }
    
    void destroy();
    
private:
    void release();

private:
    unsigned int m_id;
    int m_width;
    int m_height;
    int m_channels;

    std::string m_path;
};
