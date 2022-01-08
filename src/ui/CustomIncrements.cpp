#include "ui/CustomIncrements.hpp"

QuestUI::IncrementSetting* AddConfigValueIncrementDouble(UnityEngine::Transform* parent, ConfigUtils::ConfigValue<double>& configValue, std::string name, int decimals, double increment, double min, double max) {
    auto object = QuestUI::BeatSaberUI::CreateIncrementSetting(parent, name, decimals, increment, configValue.GetValue(), min, max, [&configValue](float value) {configValue.SetValue(value);});
    return object;
}

QuestUI::IncrementSetting* AddConfigValueIncrementInt(UnityEngine::Transform* parent, ConfigUtils::ConfigValue<int>& configValue, std::string name, int increment, int min, int max) {
    auto object = QuestUI::BeatSaberUI::CreateIncrementSetting(parent, name, 0, increment, configValue.GetValue(), min, max, [&configValue](float value) {configValue.SetValue((int)value);});
    return object;
}