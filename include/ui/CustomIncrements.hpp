#pragma once

#include "questui/shared/QuestUI.hpp"
#include "config-utils/shared/config-utils.hpp"

QuestUI::IncrementSetting* AddConfigValueIncrementDouble(UnityEngine::Transform* parent, ConfigUtils::ConfigValue<double>& configValue, std::string name, int decimals, double increment, double min, double max);
QuestUI::IncrementSetting* AddConfigValueIncrementInt(UnityEngine::Transform* parent, ConfigUtils::ConfigValue<int>& configValue, std::string name, int increment, int min, int max);