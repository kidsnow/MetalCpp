#pragma once

namespace MTL
{
    class Device;
    class CommandQueue;
    class RenderPipelineState;
    class Buffer;
}

namespace Kidsnow
{

class RenderTarget;
class Window;

class Renderer
{
private:
    Renderer();

public:
    ~Renderer();

public:
    static Renderer* GetInstance();
    static void DestroyInstance();
    static RenderTarget* GenerateRenderTarget(Window* window);
    static void Render(RenderTarget* renderTarget);

private:
    static void initialize();
    static void buildShaders();
    static void buildBuffers();

private:
    static Renderer* m_instance;
    static MTL::Device* m_device;
    static MTL::CommandQueue* m_commandQueue;
    static MTL::RenderPipelineState* m_pipelineState;
    static MTL::Buffer* m_vertexPositionBuffer;
    static MTL::Buffer* m_vertexColorBuffer;
};

}