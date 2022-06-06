#include "RenderTarget.h"

#include "ViewDelegate.h"

namespace Kidsnow
{

RenderTarget::RenderTarget(MTK::View* mtkView, ViewDelegate* viewDelegate) :
    m_mtkView(mtkView),
    m_viewDelegate(viewDelegate)
{
    
}

RenderTarget::~RenderTarget()
{

}

}