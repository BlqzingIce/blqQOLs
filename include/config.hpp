#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(config,
    CONFIG_VALUE(rotfix, bool, "Z-Rot Fix", false);
    CONFIG_VALUE(changeScore, bool, "Change Score to Percent", false);
    CONFIG_VALUE(percentPrecision, int, "Percent Decimal Precision", 1);
    CONFIG_VALUE(changeRank, bool, "Change Rank to PP", false);
    CONFIG_VALUE(ppPrecision, int, "PP Decimal Precision", 2);
    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(rotfix);
        CONFIG_INIT_VALUE(changeScore);
        CONFIG_INIT_VALUE(changeRank);
        CONFIG_INIT_VALUE(percentPrecision);
        CONFIG_INIT_VALUE(ppPrecision);
    )
);