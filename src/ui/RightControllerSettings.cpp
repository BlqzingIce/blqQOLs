#include "ui/RightControllerSettings.hpp"
#include "ui/CustomIncrements.hpp"
#include "main.hpp"
#include "config.hpp"

DEFINE_TYPE(blqQOLs::RightControllerSettings, SettingsViewController);

void blqQOLs::RightControllerSettings::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) 
{
    if (firstActivation)
    {       
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        QuestUI::ExternalComponents* externalComponents = container->GetComponent<QuestUI::ExternalComponents*>();
        UnityEngine::RectTransform* scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

        UIUtils::AddHeader(get_transform(), "Right Saber Settings", UnityEngine::Color(0.188f, 0.620f, 1.0f, 1.0f));

        QuestUI::IncrementSetting* xPosR = AddConfigValueIncrementDouble(container->get_transform(), getconfig().xPosR, "X Position", 1, 0.1, -15, 15);
        QuestUI::IncrementSetting* yPosR = AddConfigValueIncrementDouble(container->get_transform(), getconfig().yPosR, "Y Position", 1, 0.1, -15, 15);
        QuestUI::IncrementSetting* zPosR = AddConfigValueIncrementDouble(container->get_transform(), getconfig().zPosR, "Z Position", 1, 0.1, -15, 15);
        QuestUI::IncrementSetting* xRotR = AddConfigValueIncrementInt(container->get_transform(), getconfig().xRotR, "X Rotation", 1, -180, 180);
        QuestUI::IncrementSetting* yRotR = AddConfigValueIncrementInt(container->get_transform(), getconfig().yRotR, "Y Rotation", 1, -180, 180);
        QuestUI::IncrementSetting* zRotR = AddConfigValueIncrementInt(container->get_transform(), getconfig().zRotR, "Z Rotation", 1, -180, 180);
    }
}