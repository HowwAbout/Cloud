#ifndef JENKINS_UTILS_HPP
#define JENKINS_UTILS_HPP

#include <string>
#include <nlohmann/json.hpp>

// std::string get_jenkins_status(const std::string &url);
std::string get_jenkins_status(const std::string &url, const std::string &username, const std::string &token);
std::string send_jenkins_post_request(const std::string &url, const std::string &username, const std::string &token);
std::string send_jenkins_post_request_direct();
std::string determine_deployment_status(const nlohmann::json& json_data);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
void send_webhook_to_jenkins(const std::string& url, const std::string post_data);
void deploy_start(const std::string &url);
void deploy_status(const std::string &url);
void deploy_stop(const std::string &url);

#endif