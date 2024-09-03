#include "config.hpp"
#include "bot.hpp"
#include "jenkins_utils.hpp"
#include "dpp_utils.hpp"
#include <string>
using namespace std;

JenkinsConfigData initializeJenkinsConfigData(const AuthConfig& config) {
    JenkinsConfigData data;
    data.api_url_f = config.getJenkinsUrl() + "/job/" + config.getJenkinsFrontName() + "/build";
    data.api_url_b = config.getJenkinsUrl() + "/job/" + config.getJenkinsBackName() + "/build";
    data.api_url_ai = config.getJenkinsUrl() + "/job/" + config.getJenkinsAiName() + "/build";
    data.stop_url_f = config.getJenkinsUrl() + "/job/" + config.getJenkinsFrontName() + "/lastBuild/stop";
    data.stop_url_b = config.getJenkinsUrl() + "/job/" + config.getJenkinsBackName() + "/lastBuild/stop";
    data.stop_url_ai = config.getJenkinsUrl() + "/job/" + config.getJenkinsAiName() + "/lastBuild/stop";
    data.status_url_f = config.getJenkinsUrl() + "/job/" + config.getJenkinsFrontName() + "/lastBuild/api/json";
    data.status_url_b = config.getJenkinsUrl() + "/job/" + config.getJenkinsBackName() + "/lastBuild/api/json";
    data.status_url_ai = config.getJenkinsUrl() + "/job/" + config.getJenkinsAiName() + "/lastBuild/api/json";
    return data;
}

int main() {
    try {
        AuthConfig config("../config/env");
        JenkinsConfigData jenkinsData = initializeJenkinsConfigData(config);

        runBot(config, jenkinsData);

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}