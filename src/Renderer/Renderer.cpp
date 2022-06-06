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

#include <simd/simd.h>

namespace Kidsnow
{

Renderer* Renderer::m_instance = nullptr;
MTL::Device* Renderer::m_device = nullptr;
MTL::CommandQueue* Renderer::m_commandQueue = nullptr;
MTL::RenderPipelineState* Renderer::m_pipelineState = nullptr;
MTL::Buffer* Renderer::m_vertexPositionBuffer = nullptr;
MTL::Buffer* Renderer::m_vertexColorBuffer = nullptr;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    m_vertexPositionBuffer->release();
    m_vertexColorBuffer->release();
    m_pipelineState->release();
    m_commandQueue->release();
    m_device->release();
}

Renderer* Renderer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new Renderer();
        initialize();
    }
    
    return m_instance;
}

void Renderer::initialize()
{
    m_device = MTL::CreateSystemDefaultDevice();
    m_commandQueue = m_device->newCommandQueue();

    buildShaders();
    buildBuffers();
}

void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        struct v2f
        {
            float4 position [[position]];
            half3 color;
        };

        v2f vertex vertexMain( uint vertexId [[vertex_id]],
                               device const float3* positions [[buffer(0)]],
                               device const float3* colors [[buffer(1)]] )
        {
            v2f o;
            o.position = float4( positions[ vertexId ], 1.0 );
            o.color = half3 ( colors[ vertexId ] );
            return o;
        }

        half4 fragment fragmentMain( v2f in [[stage_in]] )
        {
            return half4( in.color, 1.0 );
        }
    )";

    NS::Error* error = nullptr;
    MTL::Library* library = m_device->newLibrary( NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &error );
    if ( !library )
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    MTL::Function* vertFunc = library->newFunction( NS::String::string("vertexMain", UTF8StringEncoding) );
    MTL::Function* fragFunc = library->newFunction( NS::String::string("fragmentMain", UTF8StringEncoding) );

    MTL::RenderPipelineDescriptor* pipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    pipelineDesc->setVertexFunction( vertFunc );
    pipelineDesc->setFragmentFunction( fragFunc );
    pipelineDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );

    m_pipelineState = m_device->newRenderPipelineState( pipelineDesc, &error );
    if ( !m_pipelineState )
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    vertFunc->release();
    fragFunc->release();
    pipelineDesc->release();
    library->release();
}

void Renderer::buildBuffers()
{
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] =
    {
        { -0.8f,  0.8f, 0.0f },
        {  0.0f, -0.8f, 0.0f },
        { +0.8f,  0.8f, 0.0f }
    };

    simd::float3 colors[NumVertices] =
    {
        {  1.0, 0.3f, 0.2f },
        {  0.8f, 1.0, 0.0f },
        {  0.8f, 0.0f, 1.0 }
    };

    const size_t positionsDataSize = NumVertices * sizeof( simd::float3 );
    const size_t colorDataSize = NumVertices * sizeof( simd::float3 );

    MTL::Buffer* vertexPositionBuffer = m_device->newBuffer( positionsDataSize, MTL::ResourceStorageModeManaged );
    MTL::Buffer* vertexColorBuffer = m_device->newBuffer( colorDataSize, MTL::ResourceStorageModeManaged );

    m_vertexPositionBuffer = vertexPositionBuffer;
    m_vertexColorBuffer = vertexColorBuffer;

    memcpy( vertexPositionBuffer->contents(), positions, positionsDataSize );
    memcpy( vertexColorBuffer->contents(), colors, colorDataSize );

    m_vertexPositionBuffer->didModifyRange( NS::Range::Make( 0, m_vertexPositionBuffer->length() ) );
    m_vertexColorBuffer->didModifyRange( NS::Range::Make( 0, m_vertexColorBuffer->length() ) );
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

    encoder->setRenderPipelineState(m_pipelineState);
    encoder->setVertexBuffer(m_vertexPositionBuffer, 0, 0);
    encoder->setVertexBuffer(m_vertexColorBuffer, 0, 1);
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

    encoder->endEncoding();
    commandBuffer->presentDrawable( mtkView->currentDrawable() );
    commandBuffer->commit();
}

}