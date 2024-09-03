#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>

using namespace std;

AuthConfig::AuthConfig() {

}

AuthConfig::AuthConfig(const string &filePath) {
    loadConfig(filePath);
}

AuthConfig::~AuthConfig() {
}

AuthConfig::AuthConfig(const AuthConfig &c) {
    this->botToken = c.botToken;
    this->jenkinsConfig = c.jenkinsConfig;
}

AuthConfig& AuthConfig::operator=(const AuthConfig &c) {
    if (this == &c)
        return *this;
    this->botToken = c.botToken;
    this->jenkinsConfig = c.jenkinsConfig;
    return *this;
}

void AuthConfig::loadConfig(const std::string& filePath) {
    std::ifstream configFile(filePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open config file.");
    }

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream lineStream(line);
        std::string key, value;

        if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
            if (key == "BOT_TOKEN") {
                botToken = value;
            } else if (key == "JENKINS_TOKEN") {
                jenkinsConfig.jenkinsToken = value;
            } else if (key == "JENKINS_USERNAME") {
                jenkinsConfig.jenkinsUser = value;
            } else if (key == "JENKINS_URL") {
                jenkinsConfig.jenkinsUrl = value;
            } else if (key == "FRONT_WEBHOOK_TOKEN") {
                jenkinsConfig.frontWebhookToken = value;
            } else if (key == "BACK_WEBHOOK_TOKEN") {
                jenkinsConfig.backWebhookToken = value;
            } else if (key == "AI_WEBHOOK_TOKEN") {
                jenkinsConfig.aiWebhookToken = value;
            } else if (key == "FRONT_JOB_NAME") {
                jenkinsConfig.frontJobName = value;
            } else if (key == "BACK_JOB_NAME") {
                jenkinsConfig.backJobName = value;
            } else if (key == "AI_JOB_NAME") {
                jenkinsConfig.aiJobName = value;
            }
        }
    }
}

string AuthConfig::getBotToken() const {
    return botToken;
}

string AuthConfig::getJenkinsToken() const {
    return jenkinsConfig.jenkinsToken;
}

string AuthConfig::getJenkinsUrl() const {
    return jenkinsConfig.jenkinsUrl;
}

string AuthConfig::getJenkinsUser() const {
    return jenkinsConfig.jenkinsUser;
}


string AuthConfig::getJenkinsFrontName() const {
    return jenkinsConfig.frontJobName;
}

string AuthConfig::getJenkinsBackName() const {
    return jenkinsConfig.backJobName;
}

string AuthConfig::getJenkinsAiName() const {
    return jenkinsConfig.aiJobName;
}

string AuthConfig::getJenkinsAiWebhookToken() const {
    return jenkinsConfig.aiWebhookToken;
}

string AuthConfig::getJenkinsFrontWebhookToken() const {
    return jenkinsConfig.frontWebhookToken;
}

string AuthConfig::getJenkinsBackWebhookToken() const {
    return jenkinsConfig.backWebhookToken;
}

