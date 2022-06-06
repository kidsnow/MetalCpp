#pragma once

#include "AppKit/AppKit.hpp"

namespace Kidsnow
{

class Window;

class AppDelegate : public NS::ApplicationDelegate
{
public:
    AppDelegate(Window* window);
    ~AppDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching( ::NS::Notification* pNotification ) override;
    virtual void applicationDidFinishLaunching( ::NS::Notification* pNotification ) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed( ::NS::Application* pSender ) override;

private:
    Window* m_window;
};

}