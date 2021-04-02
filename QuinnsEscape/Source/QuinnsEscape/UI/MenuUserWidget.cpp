


#include "MenuUserWidget.h"

FString UMenuUserWidget::GetProjectVersion()
{
    // From https://answers.unrealengine.com/questions/145683/view.html

    FString projectVersion;
    if (GConfig)
    {
        GConfig->GetString(
            TEXT("/Script/EngineSettings.GeneralProjectSettings"),
            TEXT("ProjectVersion"),
            projectVersion,
            GGameIni
        );
    }
	return projectVersion;
}
