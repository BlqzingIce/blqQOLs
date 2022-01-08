#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(config,
    CONFIG_VALUE(useSettings, bool, "Use Mod Controller Settings", false);
    CONFIG_VALUE(customOrder, bool, "Custom Adjustment Order", false);
    CONFIG_VALUE(oculusAdjustment, bool, "Base Oculus Adjustment", true);
    CONFIG_VALUE(changeScore, bool, "Change Score to Percent", false);
    CONFIG_VALUE(percentPrecision, int, "Percent Decimal Precision", 2);
    CONFIG_VALUE(changeRank, bool, "Change Rank to PP", false);
    CONFIG_VALUE(ppPrecision, int, "PP Decimal Precision", 2);

    CONFIG_VALUE(xPosR, double, "Right X Position", 0);
    CONFIG_VALUE(yPosR, double, "Right Y Position", 0);
    CONFIG_VALUE(zPosR, double, "Right Z Position", 0);
    CONFIG_VALUE(xRotR, int, "Right X Rotation", 0);
    CONFIG_VALUE(yRotR, int, "Right Y Rotation", 0);
    CONFIG_VALUE(zRotR, int, "Right Z Rotation", 0);

    CONFIG_VALUE(xPosL, double, "Left X Position", 0);
    CONFIG_VALUE(yPosL, double, "Left Y Position", 0);
    CONFIG_VALUE(zPosL, double, "Left Z Position", 0);
    CONFIG_VALUE(xRotL, int, "Left X Rotation", 0);
    CONFIG_VALUE(yRotL, int, "Left Y Rotation", 0);
    CONFIG_VALUE(zRotL, int, "Left Z Rotation", 0);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(useSettings);
        CONFIG_INIT_VALUE(customOrder);
        CONFIG_INIT_VALUE(oculusAdjustment);
        CONFIG_INIT_VALUE(changeScore);
        CONFIG_INIT_VALUE(changeRank);
        CONFIG_INIT_VALUE(percentPrecision);
        CONFIG_INIT_VALUE(ppPrecision);

        CONFIG_INIT_VALUE(xPosR);
        CONFIG_INIT_VALUE(yPosR);
        CONFIG_INIT_VALUE(zPosR);
        CONFIG_INIT_VALUE(xRotR);
        CONFIG_INIT_VALUE(yRotR);
        CONFIG_INIT_VALUE(zRotR);

        CONFIG_INIT_VALUE(xPosL);
        CONFIG_INIT_VALUE(yPosL);
        CONFIG_INIT_VALUE(zPosL);
        CONFIG_INIT_VALUE(xRotL);
        CONFIG_INIT_VALUE(yRotL);
        CONFIG_INIT_VALUE(zRotL);
    )
);