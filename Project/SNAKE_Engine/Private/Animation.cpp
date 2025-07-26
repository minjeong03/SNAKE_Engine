#include "Animation.h"

SpriteSheet::SpriteSheet(Texture* texture_, int frameW, int frameH)
    : texture(texture_), frameWidth(frameW), frameHeight(frameH)
{
    texWidth = texture_->GetWidth();
    texHeight = texture_->GetHeight();
    columns = texWidth / frameWidth;
    rows = texHeight / frameHeight;
}

glm::vec2 SpriteSheet::GetUVOffset(int frameIndex) const
{
    int col = frameIndex % columns;
    int row = frameIndex / columns;
    int flippedRow = (rows - 1) - row;
    float u = static_cast<float>(col * frameWidth) / texWidth;
    float v = static_cast<float>(flippedRow * frameHeight) / texHeight;
    return { u, v };
}

glm::vec2 SpriteSheet::GetUVScale() const
{
    return {
        static_cast<float>(frameWidth) / texWidth,
        static_cast<float>(frameHeight) / texHeight
    };
}

int SpriteSheet::GetFrameCount() const
{
    return columns * rows;
}

SpriteAnimator::SpriteAnimator(SpriteSheet* sheet_, float frameTime_, bool loop_)
    : sheet(sheet_), frameTime(frameTime_), loop(loop_) {
}

void SpriteAnimator::PlayClip(int start, int end, bool loop_)
{
    startFrame = start;
    endFrame = end;
    loop = loop_;
    currentFrame = startFrame;
    elapsed = 0.0f;
}

void SpriteAnimator::Update(float dt)
{
    if (!sheet) return;

    elapsed += dt;

    while (elapsed >= frameTime)
    {
        elapsed -= frameTime;
        currentFrame++;
        if (currentFrame > endFrame)
        {
            currentFrame = loop ? startFrame : endFrame;
        }
    }
}


glm::vec2 SpriteAnimator::GetUVOffset() const
{
    return sheet ? sheet->GetUVOffset(currentFrame) : glm::vec2(0.0f);
}

glm::vec2 SpriteAnimator::GetUVScale() const
{
    return sheet ? sheet->GetUVScale() : glm::vec2(1.0f);
}
