#include "ViewDelegate.h"

#include "Window/Window.h"

namespace Kidsnow
{

ViewDelegate::ViewDelegate(Window* window) :
    MTK::ViewDelegate(),
    m_window(window)
{

}

ViewDelegate::~ViewDelegate()
{

}

void ViewDelegate::drawInMTKView(MTK::View* pView)
{
    m_window->Render();
}

}