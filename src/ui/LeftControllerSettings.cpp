#include "ui/LeftControllerSettings.hpp"
#include "ui/CustomIncrements.hpp"
#include "main.hpp"
#include "config.hpp"

DEFINE_TYPE(blqQOLs::LeftControllerSettings, SettingsViewController);

void blqQOLs::LeftControllerSettings::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) 
{
    if (firstActivation) 
    {       
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        QuestUI::ExternalComponents* externalComponents = container->GetComponent<QuestUI::ExternalComponents*>();
        UnityEngine::RectTransform* scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

        UIUtils::AddHeader(get_transform(), "Left Saber Settings", UnityEngine::Color(0.941f, 0.188f, 0.188f, 1.0f));
        
        QuestUI::IncrementSetting* xPosL = AddConfigValueIncrementDouble(container->get_transform(), getconfig().xPosL, "X Position", 1, 0.1, -15, 15);
        QuestUI::IncrementSetting* yPosL = AddConfigValueIncrementDouble(container->get_transform(), getconfig().yPosL, "Y Position", 1, 0.1, -15, 15);
        QuestUI::IncrementSetting* zPosL = AddConfigValueIncrementDouble(container->get_transform(), getconfig().zPosL, "Z Position", 1, 0.1, -15, 15);
        QuestUI::IncrementSetting* xRotL = AddConfigValueIncrementInt(container->get_transform(), getconfig().xRotL, "X Rotation", 1, -180, 180);
        QuestUI::IncrementSetting* yRotL = AddConfigValueIncrementInt(container->get_transform(), getconfig().yRotL, "Y Rotation", 1, -180, 180);
        QuestUI::IncrementSetting* zRotL = AddConfigValueIncrementInt(container->get_transform(), getconfig().zRotL, "Z Rotation", 1, -180, 180);
    }
}