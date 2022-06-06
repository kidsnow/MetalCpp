#pragma once

#include "MetalKit/MetalKit.hpp"

namespace Kidsnow
{

class Window;

class ViewDelegate : public MTK::ViewDelegate
{
public:
    ViewDelegate(Window* window);
    virtual ~ViewDelegate() override;
    virtual void drawInMTKView(MTK::View* pView) override;

private:
    Window* m_window;
};

}