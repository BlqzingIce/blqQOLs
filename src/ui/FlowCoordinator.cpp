#include "ui/FlowCoordinator.hpp"
#include "ui/MainSettings.hpp"
#include "ui/RightControllerSettings.hpp"
#include "ui/LeftControllerSettings.hpp"
#include "ui/UiUtils.hpp"

DEFINE_TYPE(blqQOLs, FlowCoordinator);

void blqQOLs::FlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if(firstActivation)
    {
        set_showBackButton(true);
        SetTitle(il2cpp_utils::newcsstr("Blq's QOLs"), HMUI::ViewController::AnimationType::Out);
        auto* mainViewController = QuestUI::BeatSaberUI::CreateViewController<blqQOLs::MainSettings::SettingsViewController*>();
        auto* leftViewController = QuestUI::BeatSaberUI::CreateViewController<blqQOLs::LeftControllerSettings::SettingsViewController*>();
        auto* rightViewController = QuestUI::BeatSaberUI::CreateViewController<blqQOLs::RightControllerSettings::SettingsViewController*>();
        ProvideInitialViewControllers(mainViewController, leftViewController, rightViewController, nullptr, nullptr);
    }
}

void blqQOLs::FlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
{
    parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    UIUtils::SetTitleColor(Object::FindObjectOfType<HMUI::TitleViewController*>(), UnityEngine::Color(0.0f, 0.75f, 1.0f, 1.0f), true);
}