#ifndef BOT_HPP
#define BOT_HPP

#include <dpp/dpp.h>
#include "config.hpp"
#include "jenkins_utils.hpp"
#include "dpp_utils.hpp"

// std::string determine_deployment_status(const nlohmann::json& json_data);

int runBot(const AuthConfig& config, const JenkinsConfigData& jenkinsData);

#endif