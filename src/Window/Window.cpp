#include "Window.h"


#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "AppDelegate.h"
#include "Renderer/RenderTarget.h"


namespace Kidsnow
{

Window::Window(float posX, float posY, float width, float height) :
    m_posX(posX),
    m_posY(posY),
    m_width(width),
    m_height(height),
    m_appDelegate(nullptr),
    m_autoreleasePool(nullptr),
    m_nativeWindow(nullptr)
{
    m_autoreleasePool = NS::AutoreleasePool::alloc()->init();
}

Window::~Window()
{
    delete m_appDelegate;
    m_nativeWindow->release();
    m_autoreleasePool->release();
}

void Window::GenerateNativeWindow()
{
    CGRect frame = (CGRect){ {m_posX, m_posY}, {m_width, m_height} };
    m_nativeWindow = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled,
        NS::BackingStoreBuffered,
        false
    );
}

void Window::Run()
{
    m_appDelegate = new AppDelegate(this);

    NS::Application* pSharedApplication = NS::Application::sharedApplication();
    pSharedApplication->setDelegate(m_appDelegate);
    pSharedApplication->run();
}

}