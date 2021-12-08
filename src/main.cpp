#include "main.hpp"
#include "config.hpp"
#include "pp.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "config-utils/shared/config-utils.hpp"

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
    if(getconfig().rotfix.GetValue())
    { 
        if(node == UnityEngine::XR::XRNode::LeftHand) 
        {
            rotation.z = -rotation.z;
        }
    }
    AdjustControllerTransform(self, node, transform, position, rotation);
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

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation) 
    {
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

        UnityEngine::UI::Toggle* rotfix = AddConfigValueToggle(container->get_transform(), getconfig().rotfix);
        QuestUI::BeatSaberUI::AddHoverHint(rotfix->get_gameObject(), "Correctly mirrors the z rotation to the left controller");

        UnityEngine::UI::Toggle* changeScore = AddConfigValueToggle(container->get_transform(), getconfig().changeScore);
        QuestUI::BeatSaberUI::AddHoverHint(changeScore->get_gameObject(), "Displays best percentage instead of best score");

        QuestUI::IncrementSetting* percentPrecision = AddConfigValueIncrementInt(container->get_transform(), getconfig().percentPrecision, 1, 0, 4);
        QuestUI::BeatSaberUI::AddHoverHint(percentPrecision->get_gameObject(), "Changes decimal precision for percent, ex: 1 = 93.3%, 2 = 93.28%");

        UnityEngine::UI::Toggle* changeRank = AddConfigValueToggle(container->get_transform(), getconfig().changeRank);
        QuestUI::BeatSaberUI::AddHoverHint(changeRank->get_gameObject(), "Displays pp value of highest score instead of rank");

        QuestUI::IncrementSetting* ppPrecision = AddConfigValueIncrementInt(container->get_transform(), getconfig().ppPrecision, 1, 0, 4);
        QuestUI::BeatSaberUI::AddHoverHint(ppPrecision->get_gameObject(), "Changes decimal precision for PP, standard is 2");
    }
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), AdjustControllerTransform);
    INSTALL_HOOK(getLogger(), ShowStats);
    INSTALL_HOOK(getLogger(), AppStart);
    getLogger().info("Installed all hooks!");
}