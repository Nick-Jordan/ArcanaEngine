#include "ImagePanel.h"

namespace Arcana
{
    ImagePanel::ImagePanel(Widget* parent) : Widget(parent), 
        _thumbSize(64), _spacing(10), _margin(10), _mouseIndex(-1), _hasBackground(false)
    {

    }

    void ImagePanel::addImage(GUIIcon* icon)
    {
        if (icon)
        {
            _images.push_back(icon);
        }
    }

    GUIIcon* ImagePanel::getImage(uint32 i) const
    {
        if (i < _images.size())
        {
            return _images[i];
        }
        return nullptr;
    }

    const ImagePanel::Images& ImagePanel::getImages() const
    {
        return _images;
    }

    ImageSelectCallback& ImagePanel::getCallback()
    {
        return _callback;
    }

    int32 ImagePanel::getThumbSize() const
    {
        return _thumbSize;
    }

    void ImagePanel::setThumbSize(int32 size)
    {
        _thumbSize = size;
    }

    int32 ImagePanel::getSpacing() const
    {
        return _spacing;
    }

    void ImagePanel::setSpacing(int32 spacing)
    {
        _spacing = spacing;
    }

    int32 ImagePanel::getMargin() const
    {
        return _margin;
    }

    void ImagePanel::setMargin(int32 margin)
    {
        _margin = margin;
    }

    bool ImagePanel::hasBackground() const
    {
        return _hasBackground;
    }

    void ImagePanel::hasBackground(bool hasBackground)
    {
        _hasBackground = hasBackground;
    }

    const Color& ImagePanel::getBackgroundColor() const
    {
        return _backgroundColor;
    }

    void ImagePanel::setBackgroundColor(const Color& backgroundColor)
    {
        _backgroundColor = backgroundColor;
    }

    Vector2i ImagePanel::getGridSize() const
    {
        int32 cols = 1 + Math::max(0, (int32)((getSize().x - 2 * _margin - _thumbSize) / (float)(_thumbSize + _spacing)));

        int32 rows = ((int32)_images.size() + cols - 1) / cols;

        return Vector2i(cols, rows);
    }

    int32 ImagePanel::getIndexForPosition(const Vector2i& p) const
    {
        Vector2f pp = ((p - getAbsolutePosition()).cast<float>() - Vector2f(_margin, _margin)) / (float)(_thumbSize + _spacing);

        float iconRegion = _thumbSize / (float)(_thumbSize + _spacing);

        bool overImage = pp.x - std::floor(pp.x) < iconRegion && pp.y - std::floor(pp.y) < iconRegion;

        Vector2i gridPos = pp.cast<int>();
        Vector2i grid = getGridSize();

        overImage &= (gridPos.x >= 0 && gridPos.y >= 0) && (gridPos.x < grid.x && gridPos.y < grid.y);
        return overImage ? (gridPos.x + gridPos.y * grid.x) : -1;
    }

    bool ImagePanel::mouseMotionEvent(const Vector2i& p, const Vector2i& rel)
    {
        _mouseIndex = getIndexForPosition(p);
        return true;
    }

    bool ImagePanel::mouseButtonEvent(const Vector2i& p, Key button, bool down, ModifierKeysState modifiers)
    {
        int32 index = getIndexForPosition(p);
        if (index >= 0 && index < (int32)_images.size() && down)
        {
            _callback.executeIfBound(index);
        }
        return true;
    }

    Vector2i ImagePanel::preferredSize(GUIRenderContext& renderContext) const
    {
        Vector2i grid = getGridSize();
        return Vector2i(
            grid.x * _thumbSize + (grid.x - 1) * _spacing + 2 * _margin,
            grid.y * _thumbSize + (grid.y - 1) * _spacing + 2 * _margin
        );
    }

    void ImagePanel::render(GUIRenderContext& renderContext)
    {
        renderContext.reset();

        if (_hasBackground)
        {
            renderContext.setPrimaryColor(_backgroundColor);
            renderContext.drawRoundedRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x, getSize().y, 0.06);
        }

        Vector2i grid = getGridSize();

        for (uint32 i = 0; i < _images.size(); i++)
        {
            GUIIcon* image = _images[i];

            Vector2i p = getAbsolutePosition() + Vector2i(_margin, _margin) +
                Vector2i((int32)i % grid.x, (int32)i / grid.x) * (_thumbSize + _spacing);
            int32 imgw = image->getWidth();
            int32 imgh = image->getHeight();

            float iw, ih, ix, iy;
            if (imgw < imgh) 
            {
                iw = _thumbSize;
                ih = iw * (float)imgh / (float)imgw;
                ix = 0;
                iy = -(ih - _thumbSize) * 0.5f;
            }
            else
            {
                ih = _thumbSize;
                iw = ih * (float)imgw / (float)imgh;
                ix = -(iw - _thumbSize) * 0.5f;
                iy = 0;
            }

            //renderContext.setPrimaryColor(Color(0, 0, 0, 128));
            //renderContext.setSecondaryColor(Color(0, 0, 0, 0));
            //renderContext.setBoxGradient(0.0f, 0.0f, 1.0f, 1.0f, true);

            //renderContext.drawRoundedRect(p.x, p.y, _thumbSize, _thumbSize, 0.167);

            renderContext.drawIcon(image, Rectf(p.x, p.y, _thumbSize, _thumbSize), Color(255, 255, 255, _mouseIndex == i ? 255 : 178), 0.2);
        }
    }
}