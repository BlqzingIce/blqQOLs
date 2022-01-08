#include "pp.hpp"

const int PP_ARRAY_LENGTH = 19;

float ppCurve[PP_ARRAY_LENGTH][2] = {
    {0.0f, 0.0f},
    {0.45f, 0.015f},
    {0.5f, 0.03f},
    {0.55f, 0.06f},
    {0.6f, 0.105f},
    {0.65f, 0.15f},
    {0.7f, 0.22f},
    {0.75f, 0.3f},
    {0.8f, 0.42f},
    {0.86f, 0.6f},
    {0.9f, 0.78f},
    {0.925f, 0.905f},
    {0.945f, 1.015f},
    {0.95f, 1.046f},
    {0.96f, 1.115f},
    {0.97f, 1.2f},
    {0.98f, 1.29f},
    {0.99f, 1.39f},
    {1.0f, 1.5f}
};

float ppCurveSlopes[PP_ARRAY_LENGTH - 1];

void ppIntialize()
{
    request = UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::createcsstr("https://cdn.pulselane.dev/raw_pp.json"));
    request->SetRequestHeader(il2cpp_utils::createcsstr("User-Agent"), il2cpp_utils::createcsstr(std::string(ID) + " " + VERSION));
    request->SendWebRequest()->add_completed(il2cpp_utils::MakeDelegate<DownloadCompleteDelegate>(
        classof(DownloadCompleteDelegate), (void*)nullptr, HandleWebRequestCompleted
    ));

    for(auto i = 0; i < PP_ARRAY_LENGTH - 1; i++) 
    {
        auto x1 = ppCurve[i][0];
        auto y1 = ppCurve[i][1];
        auto x2 = ppCurve[i+1][0];
        auto y2 = ppCurve[i+1][1];

        auto m = (y2 - y1) / (x2 - x1);
        ppCurveSlopes[i] = m;
    }
}

void HandleWebRequestCompleted() {
    auto response = to_utf8(csstrtostr(request->get_downloadHandler()->GetText()));
    rapidjson::Document document;
    document.Parse(response.c_str());

    for (auto itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        RawPPData data;
        if (itr->value.HasMember("_Easy_SoloStandard"))
            data._Easy_SoloStandard = itr->value["_Easy_SoloStandard"].GetFloat();
        if (itr->value.HasMember("_Normal_SoloStandard"))
            data._Normal_SoloStandard = itr->value["_Normal_SoloStandard"].GetFloat();
        if (itr->value.HasMember("_Hard_SoloStandard"))
            data._Hard_SoloStandard = itr->value["_Hard_SoloStandard"].GetFloat();
        if (itr->value.HasMember("_Expert_SoloStandard"))
            data._Expert_SoloStandard = itr->value["_Expert_SoloStandard"].GetFloat();
        if (itr->value.HasMember("_ExpertPlus_SoloStandard"))
            data._ExpertPlus_SoloStandard = itr->value["_ExpertPlus_SoloStandard"].GetFloat();
        index.insert({std::string(itr->name.GetString()), data});
    }
}

float RatioOfMaxPP(float accuracy) {
    if (accuracy >= 1.0f) return 1.5f;
    if (accuracy <= 0.0f) return 0.0f;

    int i = 0;
    for (; i < PP_ARRAY_LENGTH; i++)
        if (i == PP_ARRAY_LENGTH - 1 || ppCurve[i + 1][0] > accuracy) break;

    auto accuracyFloor = ppCurve[i][0];
    auto ppFloor = ppCurve[i][1];
    return ppCurveSlopes[i] * (accuracy - accuracyFloor) + ppFloor;
}

std::string SongIDToHash(std::string songID) {
    if (!songID.starts_with("custom_level_")) return "";
    for (auto& c: songID) c = toupper(c);
    return songID.substr(13);
}

float CalculatePP(float maxPP, float accuracy) {
    return maxPP * RatioOfMaxPP(accuracy);
}

float BeatmapMaxPP(std::string songID, GlobalNamespace::BeatmapDifficulty difficulty) {
    auto itr = index.find(SongIDToHash(songID));
    if (itr == index.end()) return -1;

    switch ((int)difficulty) {
        case 0:
            return itr->second._Easy_SoloStandard;
        case 1:
            return itr->second._Normal_SoloStandard;
        case 2:
            return itr->second._Hard_SoloStandard;
        case 3:
            return itr->second._Expert_SoloStandard;
        case 4:
            return itr->second._ExpertPlus_SoloStandard;
        default:
            return -1;
    }
}