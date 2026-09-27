#include "app.h"
#include "resource.h"
#include <motheye/platform/window/win32/resource_ids.h>

using motheye::engine::Engine;

namespace app
{
    int App::Run(int nCmdShow)
    {
        window_ = CreateMainWindow();
   
        engine_.Start(window_->GetHandle());        
        game_.Start();

        int exitCode = window_->Run(nCmdShow);

        game_.Stop();
        engine_.Stop();

        return exitCode;
    }

    std::unique_ptr<Window> App::CreateMainWindow()
    {
        ResourceIds resourceIds;
        resourceIds.title = IDS_APP_TITLE;
        resourceIds.windowClass = IDC_EXPREND;
        resourceIds.icon = IDI_SCRIPTENGINE;
        resourceIds.smallIcon = IDI_SMALL;

        return std::make_unique<Window>(hInstance_, resourceIds, this->engine_);
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
}
