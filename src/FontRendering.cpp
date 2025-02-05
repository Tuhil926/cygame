#include "cygame.h"
#include "freetype/freetype.h"
#include "glad/glad.h"
#include <ostream>
#include "fallback_font.cpp"

Font::Font(std::string filename, int size) {
    this->size = size;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
                  << std::endl;
        return;
    }

    if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font file, switching to default font" << std::endl;
        if (FT_New_Memory_Face(ft, fonts_PixelOperator8_ttf, fonts_PixelOperator8_ttf_len, 0,
                               &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load fallback font" << std::endl;
            return;
        }
    }
    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                     face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x};
        characters.insert(std::pair<char, Character>(c, character));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // std::cout << (face->size->metrics.ascender >> 6) << " "
        //           << (face->size->metrics.descender >> 6) << std::endl;
    }
}

int Font::get_string_width(std::string &str) {
    int tot_width = 0;
    for (char c : str) {
        tot_width += characters[c].advance >> 6;
    }
    return tot_width;
}

int Font::get_ascent() { return face->size->metrics.ascender >> 6; }

int Font::get_descent() { return face->size->metrics.descender >> 6; }

void Font::cleanup() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
