#pragma once

namespace NS
{
    class AutoreleasePool;
    class Window;
}

namespace Kidsnow
{

class AppDelegate;

class Window
{
public:
    Window(float posX, float posY, float width, float height);
    virtual ~Window();

protected:
    virtual void Init() = 0;
    virtual void Render() = 0;

public:
    void Run();

    void GenerateNativeWindow();
    NS::Window* GetNativeWindow() { return m_nativeWindow; }
    float GetPosX() { return m_posX; }
    float GetPosY() { return m_posY; }
    float GetWidth() { return m_width; }
    float GetHeight() { return m_height; }


private:
    float m_posX, m_posY;
    float m_width, m_height;

    AppDelegate* m_appDelegate;
    NS::AutoreleasePool* m_autoreleasePool;
    NS::Window* m_nativeWindow;

private:
    friend class AppDelegate;
    friend class ViewDelegate;
};

}