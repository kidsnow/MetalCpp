#include "Renderer.h"

#include "RenderTarget.h"
#include "ViewDelegate.h"
#include "Window/Window.h"

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

namespace Kidsnow
{

Renderer* Renderer::m_instance = nullptr;
MTL::Device* Renderer::m_device = nullptr;
MTL::CommandQueue* Renderer::m_commandQueue = nullptr;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    m_commandQueue->release();
    m_device->release();
}

Renderer* Renderer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new Renderer();
        m_device = MTL::CreateSystemDefaultDevice();
        m_commandQueue = m_device->newCommandQueue();
    }
    
    return m_instance;
}

void Renderer::DestroyInstance()
{
    delete m_instance;
}

RenderTarget* Renderer::GenerateRenderTarget(Window* window)
{
    float posX = window->GetPosX();
    float posY = window->GetPosY();
    float width = window->GetWidth();
    float height = window->GetHeight();
    CGRect frame = (CGRect){ {posX, posY}, {width, height} };

    MTK::View* mtkView = MTK::View::alloc()->init(frame, m_device);
    mtkView->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
    mtkView->setClearColor( MTL::ClearColor::Make( 1.0, 0.0, 0.0, 1.0 ) );

    ViewDelegate* viewDelegate = new ViewDelegate(window);
    mtkView->setDelegate(viewDelegate);

    window->GetNativeWindow()->setContentView(mtkView);

    return new RenderTarget(mtkView, viewDelegate);
}

void Renderer::Render(RenderTarget* renderTarget)
{
    MTL::CommandBuffer* commandBuffer = m_commandQueue->commandBuffer();
    MTK::View* mtkView = renderTarget->GetMTKView();
    MTL::RenderPassDescriptor* renderPassDesc = mtkView->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(renderPassDesc);
    encoder->endEncoding();
    commandBuffer->presentDrawable( mtkView->currentDrawable() );
    commandBuffer->commit();
}

}