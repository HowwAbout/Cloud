#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

struct JenkinsConfigData;

using JenkinsConfig = struct  
{
    std::string jenkinsToken;
    std::string jenkinsUser;
    std::string jenkinsUrl;

    std::string frontWebhookToken;
    std::string backWebhookToken;
    std::string aiWebhookToken;

    std::string frontJobName;
    std::string backJobName;
    std::string aiJobName;

};

struct JenkinsConfigData {
    std::string api_url_f;   
    std::string api_url_b;   
    std::string api_url_ai;  

    std::string stop_url_f;  
    std::string stop_url_b;  
    std::string stop_url_ai; 

    std::string status_url_f; 
    std::string status_url_b; 
    std::string status_url_ai;

    std::string username;     
    std::string token;        
};
class AuthConfig {
private:
    std::string botToken;
    JenkinsConfig jenkinsConfig;
    AuthConfig();
    void loadConfig(const std::string &filePath);
public:
    AuthConfig(const std::string &filePath);
    ~AuthConfig();
    AuthConfig(const AuthConfig &c);
    AuthConfig& operator=(const AuthConfig &c);
    std::string getBotToken() const;
    std::string getJenkinsToken() const;
    std::string getJenkinsUrl() const;
    std::string getJenkinsUser() const;
    std::string getJenkinsFrontName() const;
    std::string getJenkinsBackName() const;
    std::string getJenkinsAiName() const;
    std::string getJenkinsAiWebhookToken() const;
    std::string getJenkinsFrontWebhookToken() const;
    std::string getJenkinsBackWebhookToken() const; 
};

#endif