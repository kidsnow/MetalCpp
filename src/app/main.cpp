#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderTarget.h"


namespace Kidsnow
{

class AppWindow : public Window
{
public:
    AppWindow(float posX, float posY, float width, float height) :
        Kidsnow::Window(posX, posY, width, height),
        m_renderTarget(nullptr)
    {}
    ~AppWindow() {}

protected:
    virtual void Init() override
    {
        Renderer* renderer = Renderer::GetInstance();
        m_renderTarget = renderer->GenerateRenderTarget(this);
    };
    virtual void Render() override
    {
        Renderer* renderer = Renderer::GetInstance();
        renderer->Render(m_renderTarget);
    };

private:
    RenderTarget* m_renderTarget;
};

}

int main(int argc, char *argv[])
{
    Kidsnow::Window *window = new Kidsnow::AppWindow(100.0f, 100.0f, 512.0f, 512.0f);

    window->Run();

    delete window;

    return 0;
}