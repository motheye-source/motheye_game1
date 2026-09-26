#include "app.h"
#include "resource.h"
#include "engine/engine.h"
#include <motheye/platform/window/win32/resource_ids.h>

using app::engine::Engine;

namespace app
{
    int App::Run(int nCmdShow)
    {
        window_ = CreateMainWindow();
   
        int exitCode = window_->Run(nCmdShow);

        return exitCode;
    }

    std::unique_ptr<Window<App, Engine>> App::CreateMainWindow()
    {
        ResourceIds resourceIds;
        resourceIds.title = IDS_APP_TITLE;
        resourceIds.windowClass = IDC_EXPREND;
        resourceIds.icon = IDI_SCRIPTENGINE;
        resourceIds.smallIcon = IDI_SMALL;

        return std::make_unique<Window<App, Engine>>(hInstance_, resourceIds, *this, this->engine_);
    }

    //void App::OnInputContextChanged(InputContext context)
    //{
    //   /* engine_.ReleaseActiveHeldInput();

    //    if (context == InputContext::Gameplay)
    //    {
    //        window_->SetInputMode(InputMode::Raw);
    //        window_->HideMouseCursor();
    //    }
    //    else
    //    {
    //        window_->SetInputMode(InputMode::Legacy);
    //        window_->CenterMouseCursor();
    //        window_->ShowMouseCursor();
    //    }*/
    //}

    void App::OnSize(unsigned int width, unsigned int height)
    {
        //engine_.Resize(width, height);
    }

    void App::OnLegacyCaptureLost()
    {
        //engine_.ReleaseActiveHeldInput();
    }

    void App::OnFocusLost()
    {
      /*  engine_.ReleaseActiveHeldInput();

        window_->SetInputMode(InputMode::Legacy);
        window_->ShowMouseCursor();*/
    }
}
