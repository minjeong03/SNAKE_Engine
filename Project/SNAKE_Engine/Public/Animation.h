#pragma once
#include "vec2.hpp"
#include "Texture.h"

class SpriteSheet
{
    Texture* texture;
    int frameWidth, frameHeight;
    int columns, rows;
    int texWidth = 0, texHeight = 0;

public:
    SpriteSheet(Texture* texture_, int frameW, int frameH);

    glm::vec2 GetUVOffset(int frameIndex) const;
    glm::vec2 GetUVScale() const;
    int GetFrameCount() const;
    Texture* GetTexture() const { return texture; }
};

class SpriteAnimator
{
    SpriteSheet* sheet;
    float frameTime;
    float elapsed = 0.0f;
    int currentFrame = 0;
    int startFrame = 0;
    int endFrame = 0;
    bool loop = true;

public:
    SpriteAnimator(SpriteSheet* sheet_, float frameTime_, bool loop_ = true);

    void PlayClip(int start, int end, bool loop_ = true);
    void Update(float dt);

    glm::vec2 GetUVOffset() const;
    glm::vec2 GetUVScale() const;

    Texture* GetTexture() { return sheet->GetTexture();}
    void SetFrame(int frame) { currentFrame = frame; }
    int GetCurrentFrame() const { return currentFrame; }
};
