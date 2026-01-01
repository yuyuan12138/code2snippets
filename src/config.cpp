#include "config.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

std::string ConfigParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::string ConfigParser::extractString(const std::string& json, const std::string& key) {
    // Search for "key": "value" pattern
    std::string searchKey = "\"" + key + "\"";
    size_t keyPos = json.find(searchKey);
    if (keyPos == std::string::npos) return "";

    size_t colonPos = json.find(':', keyPos);
    if (colonPos == std::string::npos) return "";

    size_t valueStart = json.find('"', colonPos + 1);
    if (valueStart == std::string::npos) return "";
    valueStart++; // Skip opening quote

    size_t valueEnd = json.find('"', valueStart);
    if (valueEnd == std::string::npos) return "";

    return json.substr(valueStart, valueEnd - valueStart);
}

bool ConfigParser::extractBool(const std::string& json, const std::string& key, bool defaultValue) {
    std::string searchKey = "\"" + key + "\"";
    size_t keyPos = json.find(searchKey);
    if (keyPos == std::string::npos) return defaultValue;

    size_t colonPos = json.find(':', keyPos);
    if (colonPos == std::string::npos) return defaultValue;

    size_t valueStart = colonPos + 1;
    while (valueStart < json.size() && std::isspace(json[valueStart])) valueStart++;

    std::string value;
    while (valueStart < json.size() && !std::isspace(json[valueStart]) &&
           json[valueStart] != ',' && json[valueStart] != '}' && json[valueStart] != '\n') {
        value += json[valueStart++];
    }

    if (value == "true") return true;
    if (value == "false") return false;
    return defaultValue;
}

bool ConfigParser::hasKey(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\"";
    return json.find(searchKey) != std::string::npos;
}

std::string ConfigParser::findConfigFile(const std::string& customPath) {
    // Priority 1: Custom path specified
    if (!customPath.empty() && fs::exists(customPath)) {
        return customPath;
    }

    // Priority 2: .code2snippet.json in current directory
    std::string localConfig = ".code2snippet.json";
    if (fs::exists(localConfig)) {
        return localConfig;
    }

    // Priority 3: .code2snippet/config.json in current directory
    std::string localDirConfig = ".code2snippet" PATH_SEPARATOR "config.json";
    if (fs::exists(localDirConfig)) {
        return localDirConfig;
    }

    // Priority 4: User home config directory
    const char* homeDir = std::getenv("HOME");
    if (homeDir) {
        std::string userConfig = std::string(homeDir) + PATH_SEPARATOR + ".config" + PATH_SEPARATOR + "code2snippet" + PATH_SEPARATOR + "config.json";
        if (fs::exists(userConfig)) {
            return userConfig;
        }
    }

    // Priority 5: System-wide config (Unix-like)
    std::string systemConfig = "/etc/code2snippet/config.json";
    if (fs::exists(systemConfig)) {
        return systemConfig;
    }

    return "";
}

bool ConfigParser::parse(const std::string& filepath, AppConfig& config) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    // Simple JSON parsing - look for specific keys
    // Parse settings section if exists
    size_t settingsPos = content.find("\"settings\"");
    if (settingsPos != std::string::npos) {
        size_t objStart = content.find('{', settingsPos);
        size_t objEnd = content.find('}', objStart);
        if (objStart != std::string::npos && objEnd != std::string::npos) {
            std::string settingsObj = content.substr(objStart, objEnd - objStart + 1);

            if (hasKey(settingsObj, "defaultName")) {
                config.settings.defaultName = extractString(settingsObj, "defaultName");
            }
            if (hasKey(settingsObj, "defaultPrefix")) {
                config.settings.defaultPrefix = extractString(settingsObj, "defaultPrefix");
            }
            if (hasKey(settingsObj, "defaultDescription")) {
                config.settings.defaultDescription = extractString(settingsObj, "defaultDescription");
            }
            if (hasKey(settingsObj, "defaultOutputDir")) {
                config.settings.defaultOutputDir = extractString(settingsObj, "defaultOutputDir");
            }
            if (hasKey(settingsObj, "defaultAppend")) {
                config.settings.defaultAppend = extractBool(settingsObj, "defaultAppend", false);
            }
            if (hasKey(settingsObj, "verbose")) {
                config.settings.verbose = extractBool(settingsObj, "verbose", false);
            }
        }
    } else {
        // Parse without settings wrapper (flat config)
        if (hasKey(content, "defaultName")) {
            config.settings.defaultName = extractString(content, "defaultName");
        }
        if (hasKey(content, "defaultPrefix")) {
            config.settings.defaultPrefix = extractString(content, "defaultPrefix");
        }
        if (hasKey(content, "defaultDescription")) {
            config.settings.defaultDescription = extractString(content, "defaultDescription");
        }
        if (hasKey(content, "defaultOutputDir")) {
            config.settings.defaultOutputDir = extractString(content, "defaultOutputDir");
        }
        if (hasKey(content, "defaultAppend")) {
            config.settings.defaultAppend = extractBool(content, "defaultAppend", false);
        }
        if (hasKey(content, "verbose")) {
            config.settings.verbose = extractBool(content, "verbose", false);
        }
    }

    return true;
}

bool ConfigParser::loadConfig(AppConfig& config, const std::string& customPath) {
    std::string configPath = findConfigFile(customPath);
    if (configPath.empty()) {
        return false; // No config file found
    }

    return parse(configPath, config);
}
