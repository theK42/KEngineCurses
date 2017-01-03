#include "CursesRenderer.h"
#include <cassert>
#include <curses.h>

KEngineCurses::CursesGraphic::CursesGraphic()
{
    mTransform = nullptr;
    mCharacters = nullptr;
}

KEngineCurses::CursesGraphic::~CursesGraphic()
{
    Deinit();
}

void KEngineCurses::CursesGraphic::Init( CursesRenderer * renderer, CharMap const * characters, KEngine2D::Transform const * transform )
{
    assert(transform != nullptr);
    assert(renderer != nullptr);
    mRenderer = renderer;
    mCharacters = characters;
    mTransform = transform;
    renderer->AddToRenderList(this);
}

void KEngineCurses::CursesGraphic::Deinit()
{
    if (mRenderer != 0)
    {
        mRenderer->RemoveFromRenderList(this);
    }
    mTransform = nullptr;
    mCharacters = nullptr;
    mRenderer = nullptr;
}

KEngineCurses::CharMap const * KEngineCurses::CursesGraphic::GetCharMap() const
{
    return mCharacters;
}

void KEngineCurses::CursesGraphic::SetCharMap( CharMap const * characters )
{
    assert(mTransform != nullptr); /// Initialized
    mCharacters = characters;
}

KEngine2D::Transform const * KEngineCurses::CursesGraphic::GetTransform() const
{
    assert(mTransform != nullptr);
    return mTransform;
}

KEngineCurses::CursesRenderer::CursesRenderer()
{
    mInitialized = false;
    mBuffer = nullptr;
}

KEngineCurses::CursesRenderer::~CursesRenderer()
{
    Deinit();
}

void KEngineCurses::CursesRenderer::Init( int width, int height )
{
    assert(!mInitialized);
    mWidth = width;
    mHeight = height;
    mBuffer = new char [width * height];
    initscr(); //Initialize the curses library
    mInitialized = true;
}

void KEngineCurses::CursesRenderer::Deinit()
{
    mInitialized = false;
    mRenderList.clear();
    endwin();//Deinit the curses library
}

void KEngineCurses::CursesRenderer::Render() const
{
    assert(mInitialized);
    //Uses the buffer to minimize flickering.
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            mBuffer[(i * mWidth) + j] = ' ';
        }
    }
    for (std::list<CursesGraphic *>::const_iterator it = mRenderList.begin(); it != mRenderList.end(); it++)
    {
        KEngine2D::Point graphicPosition =  (*it)->GetTransform()->GetTranslation(); //Need to handle other parts of the transform as well
        CharMap const * charMap = (*it)->GetCharMap();
        ///Could implement some sort of camera here
        int x = int(graphicPosition.x + .5f); //Round instead of just truncate
        int y = int(graphicPosition.y + .5f);
        for (int i = 0; i < charMap->width; i++) {
            for (int j = 0; j < charMap->height; j++) {
                char character = charMap->characters[(j * charMap->width) + i];
                if (character != ' ' && x + i >= 0 && x + i < mWidth && y + j >= 0 && y + j < mHeight)
                {
                    mBuffer[((y + j) * mWidth) + (x + i)] = character;
                }
            }
        }
    }
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            move(i, j);//Move the curses cursor
            addch( mBuffer[(i * mWidth) + j]); // output character at curses cursor
        }
    }
    refresh(); //Refresh the curses screen
}

void KEngineCurses::CursesRenderer::AddToRenderList( CursesGraphic * CursesGraphic )
{
    assert(mInitialized);
    mRenderList.push_back(CursesGraphic);
}

void KEngineCurses::CursesRenderer::RemoveFromRenderList( CursesGraphic * CursesGraphic )
{
    assert(mInitialized);
    mRenderList.remove(CursesGraphic);
}

int KEngineCurses::CursesRenderer::GetWidth() const {
    return mWidth;
}

int KEngineCurses::CursesRenderer::GetHeight() const {
    return mHeight;
}
