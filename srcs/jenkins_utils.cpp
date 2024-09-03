#include "jenkins_utils.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <iostream>

/* cURL에서 응답데이터 받아오는 함수 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/*
Parameters:
    json_data: API로 받아온 json데이터
*/
std::string determine_deployment_status(const nlohmann::json& json_data) {
    bool building = false;
    bool in_progress = false;
    std::string result = "UNKNOWN";

    /* 'building' 키가 존재하는지 확인 */ 
    if (json_data.contains("building")) {
        building = json_data["building"];
    } else {
        throw std::runtime_error("'building' key not found in JSON data.");
    }

    /* 'inProgress' 키가 존재하는지 확인 */
    if (json_data.contains("inProgress")) {
        in_progress = json_data["inProgress"];
    } else {
        throw std::runtime_error("'inProgress' key not found in JSON data.");
    }

    /* 'result' 키가 존재하는지 확인 */
    if (json_data.contains("result") && !json_data["result"].is_null()) {
        result = json_data["result"];
    }

    /* 상태 평가 */ 
    if (result == "ABORTED" && building) {
        return "배포 중단중입니다.";
    } else if (result == "ABORTED") {
        return "배포가 중단되었습니다.";
    } else if (building && in_progress) {
        return "배포 중입니다.";
    } else if (result == "SUCCESS") {
        return "배포가 성공적으로 완료되었습니다.";
    } else if (result == "FAILURE") {
        return "최근 배포: 실패";
    } else {
        return "배포 안 함";
    }
}

/*
Parameters:
    url: API를 받아올 포인트
*/
std::string get_jenkins_status(const std::string &url, const std::string &username, const std::string &token) {
    CURL* curl;
    CURLcode res;
    std::string response_data;

    curl = curl_easy_init();
    if(curl) {
        // URL 설정
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // 인증 정보 설정 (username:token 형식)
        std::string auth = username + ":" + token;
        curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());

        // 응답 데이터를 저장하기 위한 콜백 함수와 버퍼 설정
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // 요청 실행
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            std::cout << "HTTP 상태 코드: " << http_code << std::endl;
            std::cout << "Response data: " << response_data << std::endl;
        }

        // CURL 리소스 해제
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "CURL 초기화 실패!" << std::endl;
    }
    
    return response_data;
}

std::string send_jenkins_post_request(const std::string &url, const std::string &username, const std::string &token) {
    CURL* curl;
    CURLcode res;
    std::string response_data;

    curl = curl_easy_init();
    if(curl) {
        // URL 설정
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // POST 요청으로 설정
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

        // 인증 정보 설정 (username:token 형식)
        std::string auth = username + ":" + token;
        curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());

        // 응답 데이터를 저장하기 위한 콜백 함수와 버퍼 설정
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // 요청 실행
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // CURL 리소스 해제
        curl_easy_cleanup(curl);
    }
    
    return response_data;
}

std::string send_jenkins_post_request_direct() {
    // 명령어를 문자열로 직접 정의합니다.
    std::string command = "curl -X POST -u \"kko:11c0844d0e8de3f68ded461488f3af26af\" "
                          "-H \"Expect:\" "
                          "--http1.1 "
                          "--max-time 10 "
                          "--connect-timeout 10 "
                          "--insecure "
                          "-v "
                          "http://43.202.237.248:8080/job/HowAbout_Front/build";

    std::cout << "Executing command: " << command << std::endl;

    // 명령어를 실행하고 결과를 얻습니다.
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "popen() failed!" << std::endl;
        return "Error";
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // 파이프를 닫습니다.
    pclose(pipe);

    return result;
}

/*
Parameters:
    url: 배포서버의 주소
    post_data: 배포서버에 보낼 메세지내용
*/
void send_webhook_to_jenkins(const std::string& url, const std::string post_data) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

        /* HTTP 헤더 설정 (JSON 데이터로 보낼 경우) */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        /* 리소스 정리 */
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

/* Jenkins에 보낼 기능들 */
void deploy_start(const std::string &url) {
    const std::string data = R"({"event": "deploy_start", "message": "배포가 시작되었습니다."})";
    send_webhook_to_jenkins(url, data);
}

void deploy_status(const std::string &url) {
    const std::string data = R"({"event": "deploy_status", "message": "현재 배포 진행 중입니다."})";
    send_webhook_to_jenkins(url, data);
}

void deploy_stop(const std::string &url) {
    const std::string data = R"({"event": "deploy_stop", "message": "배포가 중단되었습니다."})";
    send_webhook_to_jenkins(url, data);
}