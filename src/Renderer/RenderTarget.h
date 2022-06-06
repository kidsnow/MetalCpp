#pragma once

namespace MTK { class View; }

namespace Kidsnow
{

class Window;
class ViewDelegate;

class RenderTarget
{
public:
    RenderTarget(MTK::View* view, ViewDelegate* viewDelegate);
    ~RenderTarget();

    MTK::View* GetMTKView() { return m_mtkView; }

private:
    MTK::View* m_mtkView;
    ViewDelegate* m_viewDelegate;
};

}