#pragma once

#include <list>
#include "Transform2D.h"
#include "Renderer2D.h"

namespace KEngineCurses
{
    struct CharMap
    {
        //CharMap();
        //CharMap(int width, int height, char const * characters);
        int width;
        int height;
        char const * characters;
    };
    
    class CursesRenderer;
    
    class CursesGraphic
    {
    public:
        CursesGraphic();
        ~CursesGraphic();
        void Init(CursesRenderer * renderer, CharMap const * characters, KEngine2D::Transform const * transform);
        void Deinit();
        CharMap const * GetCharMap() const;
        void SetCharMap(CharMap const * characters);
        KEngine2D::Transform const * GetTransform() const;
        
    protected:
        CharMap const *					mCharacters;
        KEngine2D::Transform const *	mTransform;
        CursesRenderer *				mRenderer;
    };
    
    class CursesRenderer : public KEngine2D::Renderer
    {
    public:
        CursesRenderer();
        ~CursesRenderer();
        void Init(int width, int height);
        void Deinit();
        void Render() const;
        void AddToRenderList(CursesGraphic * cursesGraphic);
        void RemoveFromRenderList(CursesGraphic * cursesGraphic);
        int GetWidth() const;
        int GetHeight() const;
    protected:
        
        std::list<CursesGraphic *>	mRenderList;
        int							mWidth;
        int							mHeight;
        bool						mInitialized;
        char *						mBuffer;
        
    };
}
