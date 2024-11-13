///
/// Created by Anonymous275 on 2/23/2021
///

#include "Logger.h"
#include "Network/network.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Options.h"
namespace fs = std::filesystem;

std::string Branch;


#ifdef __APPLE__
std::string CachingDirectory = std::string(getenv("HOME")) + "/Library/Application Support/BeamMP-Launcher/Resources";
std::string configPath = std::string(getenv("HOME")) + "/Library/Application Support/BeamMP-Launcher/Launcher.cfg";
#else
std::string CachingDirectory = "./Resources";
std::string configPath = "Launcher.cfg";
#endif

void ParseConfig(const nlohmann::json& d) {
    if (d["Port"].is_number()) {
        options.port = d["Port"].get<int>();
    }
    // Default -1
    // Release 1
    // EA 2
    // Dev 3
    // Custom 3
    if (d["Build"].is_string()) {
        Branch = d["Build"].get<std::string>();
        for (char& c : Branch)
            c = char(tolower(c));
    }
    if (d.contains("CachingDirectory") && d["CachingDirectory"].is_string()) {
        CachingDirectory = d["CachingDirectory"].get<std::string>();
        info("Mod caching directory: " + CachingDirectory);
    }

    if (d.contains("Dev") && d["Dev"].is_boolean()) {
        bool dev = d["Dev"].get<bool>();
        options.verbose = dev;
        options.no_download = dev;
        options.no_launch = dev;
        options.no_update = dev;
    }
}

void ConfigInit() {
    // Créer le répertoire de configuration sur macOS si nécessaire
    #ifdef __APPLE__
    fs::path configDir = std::string(getenv("HOME")) + "/Library/Application Support/BeamMP-Launcher";
    if (!fs::exists(configDir)) {
        fs::create_directories(configDir);
    }
    #else
    #TODO

    #endif

    if (fs::exists(configPath)) {
        std::ifstream cfg(configPath);
        if (cfg.is_open()) {
            auto Size = fs::file_size(configPath);
            std::string Buffer(Size, 0);
            cfg.read(&Buffer[0], Size);
            cfg.close();
            nlohmann::json d = nlohmann::json::parse(Buffer, nullptr, false);
            if (d.is_discarded()) {
                fatal("Config failed to parse, make sure it's valid JSON!");
            }
            ParseConfig(d);
        } else {
            fatal("Failed to open " + configPath + "!");
        }
    } else {
        std::ofstream cfg(configPath);
        if (cfg.is_open()) {
            cfg <<
                R"({
"Port": 4444,
"Build": "Default",
"CachingDirectory":)";
            cfg << " \"" << CachingDirectory << "\"\n}";
            cfg.close();
        } else {
            fatal("Failed to write config on disk!");
        }
    }
}