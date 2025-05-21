#ifndef ASSETS_H
#define ASSETS_H

void loadFonts();
void unloadFonts();

void loadImages();
void unloadImages();

void drawImage(Texture2D image, Vector2 pos, float width, float height);
void drawImage(Texture2D image, Vector2 pos, float size);

sprite loadSprite(
        const std::string &file_name_prefix,
        const std::string &file_name_suffix,
        size_t frame_count = 1,
        bool loop = true,
        size_t frames_to_skip = 3
);
void unloadSprite(sprite &sprite);
void drawSprite(sprite &sprite, Vector2 pos, float width, float height);
void drawSprite(sprite &sprite, Vector2 pos, float size);

void loadSounds();
void unloadSounds();

#endif // ASSETS_H

