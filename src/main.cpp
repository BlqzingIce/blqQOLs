#include "main.hpp"
#include "config.hpp"
#include "pp.hpp"

#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"
#include "ui/FlowCoordinator.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include <iomanip>
#include <sstream>

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/XR/XRNode.hpp"

#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/LevelStatsView.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/PlayerLevelStatsData.hpp"
#include "GlobalNamespace/ScoreModel.hpp"
#include "GlobalNamespace/QuestAppInit.hpp"

MAKE_HOOK_MATCH(AdjustControllerTransform, &GlobalNamespace::OculusVRHelper::AdjustControllerTransform, void, GlobalNamespace::OculusVRHelper* self, UnityEngine::XR::XRNode node, UnityEngine::Transform* transform, UnityEngine::Vector3 position, UnityEngine::Vector3 rotation) 
{
    if(getconfig().useSettings.GetValue())
    {
        if(node == UnityEngine::XR::XRNode::RightHand)
        {
            position.x = (float)getconfig().xPosR.GetValue()/100;
            position.y = (float)getconfig().yPosR.GetValue()/100;
            position.z = (float)getconfig().zPosR.GetValue()/100;
            rotation.x = getconfig().xRotR.GetValue();
            rotation.y = getconfig().yRotR.GetValue();
            rotation.z = getconfig().zRotR.GetValue();
        }
        if(node == UnityEngine::XR::XRNode::LeftHand) 
        {
            position.x = (float)getconfig().xPosL.GetValue()/100;
            position.y = (float)getconfig().yPosL.GetValue()/100;
            position.z = (float)getconfig().zPosL.GetValue()/100;
            rotation.x = getconfig().xRotL.GetValue();
            rotation.y = getconfig().yRotL.GetValue();
            rotation.z = getconfig().zRotL.GetValue();
        }
    }

    if(!getconfig().oculusAdjustment.GetValue())
    {
        position.z -= 0.055;
        rotation.x += 40;
    }

    if(!getconfig().useSettings.GetValue())
    {
        if(node == UnityEngine::XR::XRNode::LeftHand) {rotation.z = -rotation.z;}
    }

    AdjustControllerTransform(self, node, transform, position, rotation);
    
    if(getconfig().customOrder.GetValue())
    { 
        transform->Translate(UnityEngine::Vector3(-position.x, -position.y, -position.z));
        transform->Rotate(UnityEngine::Vector3(-rotation.x, -rotation.y, -rotation.z));

        transform->Rotate(UnityEngine::Vector3(0, 0, rotation.z));
        transform->Translate(position);
        transform->Rotate(UnityEngine::Vector3(rotation.x, rotation.y, 0));
    }
}

Array<TMPro::TextMeshProUGUI*>* levelStatsComponents;
int maxRawScore;
float highscore;
std::string songID;
std::string newString;
float maxPP;

std::string createString(int precision, float value)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

MAKE_HOOK_MATCH(ShowStats, &GlobalNamespace::LevelStatsView::ShowStats, void, GlobalNamespace::LevelStatsView* self, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::PlayerData* playerData) 
{
    ShowStats(self, difficultyBeatmap, playerData);
    levelStatsComponents = self->get_transform()->GetComponentsInChildren<TMPro::TextMeshProUGUI*>();
    maxRawScore = GlobalNamespace::ScoreModel::MaxRawScoreForNumberOfNotes(difficultyBeatmap->get_beatmapData()->get_cuttableNotesCount());
    highscore = (playerData->GetPlayerLevelStatsData(difficultyBeatmap))->get_highScore();
    songID = to_utf8(csstrtostr((playerData->GetPlayerLevelStatsData(difficultyBeatmap))->get_levelID()));

    getLogger().info("%s", to_utf8(csstrtostr((*levelStatsComponents)[2]->get_text())).c_str());
    getLogger().info("%s", to_utf8(csstrtostr((*levelStatsComponents)[4]->get_text())).c_str());

    if(getconfig().changeScore.GetValue())
    { 
        (*levelStatsComponents)[2]->set_text(il2cpp_utils::newcsstr("Percent"));
        if(highscore > 0)
        {
            newString = createString(getconfig().percentPrecision.GetValue(), highscore / maxRawScore * 100);
            self->highScoreText->set_text(il2cpp_utils::newcsstr(newString + "%"));
        }
    }

    if(getconfig().changeRank.GetValue())
    { 
        (*levelStatsComponents)[4]->set_text(il2cpp_utils::newcsstr("PP Earned"));
        if(highscore > 0)
        {
            maxPP = BeatmapMaxPP(songID, difficultyBeatmap->get_difficulty());
            if(maxPP != -1)
            {
                newString = createString(getconfig().ppPrecision.GetValue(), CalculatePP(maxPP, highscore / maxRawScore));
                self->maxRankText->set_text(il2cpp_utils::newcsstr(newString));
            }
            else
            {
                self->maxRankText->set_text(il2cpp_utils::newcsstr("-"));
            }
        }
    }
    getLogger().info("%s", to_utf8(csstrtostr((*levelStatsComponents)[2]->get_text())).c_str());
    getLogger().info("%s", to_utf8(csstrtostr((*levelStatsComponents)[4]->get_text())).c_str());
}

MAKE_HOOK_MATCH(AppStart, &GlobalNamespace::QuestAppInit::AppStartAndMultiSceneEditorSetup, void, GlobalNamespace::QuestAppInit* self)
{
    AppStart(self);
    getLogger().info("Initializing PP");
    ppIntialize();
    getLogger().info("PP Success!");
}

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getconfig().Init(info);
    getConfig().Reload();
    getConfig().Write();

    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    QuestUI::Init();
    QuestUI::Register::RegisterMainMenuModSettingsFlowCoordinator<blqQOLs::FlowCoordinator*>(modInfo);

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), AdjustControllerTransform);
    INSTALL_HOOK(getLogger(), ShowStats);
    INSTALL_HOOK(getLogger(), AppStart);
    getLogger().info("Installed all hooks!");
}