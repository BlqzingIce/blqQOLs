#include "ui/MainSettings.hpp"
#include "main.hpp"
#include "config.hpp"

DEFINE_TYPE(blqQOLs::MainSettings, SettingsViewController);

void blqQOLs::MainSettings::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) 
{
    if (firstActivation) 
    {       
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

        UnityEngine::UI::Toggle* useSettings = AddConfigValueToggle(container->get_transform(), getconfig().useSettings);
        QuestUI::BeatSaberUI::AddHoverHint(useSettings->get_gameObject(), "Uses the mod's controller settings instead of the base game's (values applied globally)");

        UnityEngine::UI::Toggle* customOrder = AddConfigValueToggle(container->get_transform(), getconfig().customOrder);
        QuestUI::BeatSaberUI::AddHoverHint(customOrder->get_gameObject(), "Instead of applying rotation and then position, applies Z rotation, position, and then XY rotations");

        UnityEngine::UI::Toggle* oculusAdjustment = AddConfigValueToggle(container->get_transform(), getconfig().oculusAdjustment);
        QuestUI::BeatSaberUI::AddHoverHint(oculusAdjustment->get_gameObject(), "Enables the base oculus controller adjustment of +5.5 z-pos, -40 x-rot");
        
        UnityEngine::UI::Toggle* changeScore = AddConfigValueToggle(container->get_transform(), getconfig().changeScore);
        QuestUI::BeatSaberUI::AddHoverHint(changeScore->get_gameObject(), "Displays best percentage instead of best score under leaderboard");

        QuestUI::IncrementSetting* percentPrecision = AddConfigValueIncrementInt(container->get_transform(), getconfig().percentPrecision, 1, 0, 4);
        QuestUI::BeatSaberUI::AddHoverHint(percentPrecision->get_gameObject(), "Changes decimal precision for percent, ex: 1 = 93.3%, 2 = 93.28%");

        UnityEngine::UI::Toggle* changeRank = AddConfigValueToggle(container->get_transform(), getconfig().changeRank);
        QuestUI::BeatSaberUI::AddHoverHint(changeRank->get_gameObject(), "Displays PP value of highest score instead of rank under leaderboard");

        QuestUI::IncrementSetting* ppPrecision = AddConfigValueIncrementInt(container->get_transform(), getconfig().ppPrecision, 1, 0, 4);
        QuestUI::BeatSaberUI::AddHoverHint(ppPrecision->get_gameObject(), "Changes decimal precision for PP, standard is 2");
    }
}