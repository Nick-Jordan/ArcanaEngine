#ifndef IMAGE_PANEL_H_
#define IMAGE_PANEL_H_

#include "GUIDefines.h"

#include "Widget.h"

namespace Arcana
{
    REGISTER_CALLBACK(ImageSelectCallback, int32);

    class ARCANA_GUI_API ImagePanel : public Widget
    {
    public:

        typedef std::vector<GUIIcon*> Images;

    public:

        ImagePanel(Widget* parent = nullptr);

        void addImage(GUIIcon* icon);

        GUIIcon* getImage(uint32 i) const;

        const Images& getImages() const;

        ImageSelectCallback& getCallback();

        int32 getThumbSize() const;

        void setThumbSize(int32 size);

        int32 getSpacing() const;

        void setSpacing(int32 spacing);

        int32 getMargin() const;

        void setMargin(int32 margin);

        bool hasBackground() const;

        void hasBackground(bool hasBackground);

        const Color& getBackgroundColor() const;

        void setBackgroundColor(const Color& backgroundColor);

        virtual bool mouseMotionEvent(const Vector2i& p, const Vector2i& rel);

        virtual bool mouseButtonEvent(const Vector2i& p, Key button, bool down, ModifierKeysState modifiers) override;

        virtual Vector2i preferredSize(GUIRenderContext& renderContext) const override;

        virtual void render(GUIRenderContext& renderContext) override;

    private:

        Vector2i getGridSize() const;

        int32 getIndexForPosition(const Vector2i& p) const;

    private:

        Images _images;
        ImageSelectCallback _callback;
        int32 _thumbSize;
        int32 _spacing;
        int32 _margin;
        int32 _mouseIndex;
        bool _hasBackground;
        Color _backgroundColor;
    };
}

#endif // !IMAGE_PANEL_H_