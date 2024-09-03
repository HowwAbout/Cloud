#include "bot.hpp"

using namespace std;
// using json = nlohmann::json;

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


int runBot(const AuthConfig& config, const JenkinsConfigData& jenkinsData) {
    /* Setup the bot */
    dpp::cluster bot(config.getBotToken(), dpp::i_default_intents | dpp::i_message_content);
    // dpp::cluster bot(BOT_TOKEN);
    
    /* 디버그용 로그출력 */
    bot.on_log(dpp::utility::cout_logger()); 
    /* The event is fired when someone issues your commands */
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        /* Check which command they ran */
        if (event.command.get_command_name() == "embed") {
            /* Create an embed */
            dpp::embed embed = dpp::embed()
                .set_color(dpp::colors::sti_blue)
                .set_title("Howwabout_bot")
                .set_url("https://github.com/orgs/HowwAbout/dashboard")
                .set_author("Howwabout_bot", "https://github.com/orgs/HowwAbout/dashboard", "https://dpp.dev/DPP-Logo.png")
                .set_description("hi this test!")
                .set_thumbnail("https://dpp.dev/DPP-Logo.png")
                .add_field(
                    "Regular field title",
                    "Some value here"
                )
                .add_field(
                    "Inline field title",
                    "Some value here",
                    true
                )
                .add_field(
                    "Inline field title",
                    "Some value here",
                    true
                )
                .set_image("https://dpp.dev/DPP-Logo.png")
                .set_footer(
                    dpp::embed_footer()
                    .set_text("test image")
                    .set_icon("https://dpp.dev/DPP-Logo.png")
                )
                .set_timestamp(time(0));
 
            /* Create a message with the content as our new embed. */
            dpp::message msg(event.command.channel_id, embed);
 
            /* Reply to the user with the message, containing our embed. */
            event.reply(msg);
        }

        /* 처음만든 테스트용 함수들 */
        else if(event.command.get_command_name() == "ping") {
            event.reply("Pong!!!");
        }
        // else if (event.command.get_command_name() == "배포시작") {
        //     event.reply("Jenkins로 프론트배포를 시작합니다.");
        //     deploy_start(JENKINS_WEBHOOK_URL_F);
        // }
        // else if (event.command.get_command_name() == "배포현황") {
        //     event.reply("Jenkins로 프론트배포상태를 받습니다.");
        //     deploy_status(JENKINS_WEBHOOK_URL_F);
        // }
        // else if (event.command.get_command_name() == "배포중단") {
        //     event.reply("Jenkins로 프론트배포를 중단합니다.");
        //     deploy_stop(JENKINS_WEBHOOK_URL_F);
        // }
        /* 처음만든 테스트용 함수들 */

        /* 서버기능-버튼 */
        else if (event.command.get_command_name() == "서버") {
            dpp::message msg(event.command.channel_id, "원하시는 기능을 선택해주세요");
            /* 버튼들을 한줄로 정렬 */
            // msg.add_component(
            //     dpp::component()
            //             .add_component(create_button("배포", dpp::cot_button, dpp::cos_primary, "배포상세"))
            //             .add_component(create_button("중단", dpp::cot_button, dpp::cos_danger, "중단상세"))
            //             .add_component(create_button("배포상태", dpp::cot_button, dpp::cos_success, "상태상세"))
            // );
            
            /* 버튼들을 각각의 줄로 정렬 */
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("배포", dpp::cot_button, dpp::cos_primary, "배포상세"))
            );
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("중단", dpp::cot_button, dpp::cos_danger, "중단상세"))
            );
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("배포상태", dpp::cot_button, dpp::cos_success, "상태상세"))
            );
            event.reply(msg);
        }
    });

    /* 버튼눌렸을경우 처리파트 */
    bot.on_button_click([&bot, &jenkinsData, &config](const dpp::button_click_t &event) {
        /* 배포에 관한 처리 */
        if (event.custom_id == "배포상세") {
            dpp::message msg(event.command.channel_id, "배포할 서버를 선택하세요");
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("프론트 서버 배포하기", dpp::cot_button, dpp::cos_primary, "프론트 배포"))
            );
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("백엔드 서버 배포하기", dpp::cot_button, dpp::cos_primary, "백엔드 배포"))
            );
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("AI 서버 배포하기", dpp::cot_button, dpp::cos_primary, "AI 배포"))
            );
            event.reply(dpp::ir_update_message, msg);
        }

        /* 배포에 관한 처리 */
        else if (event.custom_id == "프론트 배포") {
            try {
                std::string response = send_jenkins_post_request(jenkinsData.api_url_f, config.getJenkinsUser(), config.getJenkinsToken());
                // std::string response = send_jenkins_post_request_direct();
                std::cout << response << std::endl;
                if (response.empty()) {
                    dpp::message msg("프론트엔드 서버의 배포를 성공적으로 시작했습니다.");
                    event.reply(dpp::ir_update_message, msg);
                } else {
                    dpp::message msg("프론트엔드 서버 배포 시작에 실패했습니다. 응답: " + response);
                    event.reply(dpp::ir_update_message, msg);
                }
            } catch (const std::exception &e) {
                dpp::message msg("프론트엔드 서버 배포 중 에러가 발생했습니다: " + std::string(e.what()));
                event.reply(dpp::ir_update_message, msg);
            }
        }
        else if (event.custom_id == "백엔드 배포") {
            try {
                std::string response = send_jenkins_post_request(jenkinsData.api_url_b, config.getJenkinsUser(), config.getJenkinsToken());
                
                if (response.empty()) {
                    dpp::message msg("백엔드 서버의 배포를 성공적으로 시작했습니다.");
                    event.reply(dpp::ir_update_message, msg);
                } else {
                    dpp::message msg("백엔드 서버 배포 시작에 실패했습니다. 응답: " + response);
                    event.reply(dpp::ir_update_message, msg);
                }
            } catch (const std::exception &e) {
                dpp::message msg("백엔드 서버 배포 중 에러가 발생했습니다: " + std::string(e.what()));
                event.reply(dpp::ir_update_message, msg);
            }
        }
        else if (event.custom_id == "AI 배포") {
            try {
                std::string response = send_jenkins_post_request(jenkinsData.api_url_ai, config.getJenkinsUser(), config.getJenkinsToken());
                
                if (response.empty()) {
                    dpp::message msg("AI 서버의 배포를 성공적으로 시작했습니다.");
                    event.reply(dpp::ir_update_message, msg);
                } else {
                    dpp::message msg("AI 서버 배포 시작에 실패했습니다. 응답: " + response);
                    event.reply(dpp::ir_update_message, msg);
                }
            } catch (const std::exception &e) {
                dpp::message msg("AI 서버 배포 중 에러가 발생했습니다: " + std::string(e.what()));
                event.reply(dpp::ir_update_message, msg);
            }
        }

        /* 정지에 관한 처리 */
        else if (event.custom_id == "중단상세") {
            dpp::message msg(event.command.channel_id, "중지할 서버를 선택하세요");
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("프론트 서버 중지하기", dpp::cot_button, dpp::cos_primary, "프론트 중지"))
            );
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("백엔드 서버 중지하기", dpp::cot_button, dpp::cos_primary, "백엔드 중지"))
            );
            msg.add_component(
                dpp::component()
                    .set_type(dpp::cot_action_row)
                    .add_component(create_button("AI 서버 중지하기", dpp::cot_button, dpp::cos_primary, "AI 중지"))
            );
            event.reply(dpp::ir_update_message, msg);
        } else if (event.custom_id == "프론트 정지") {
            try {
                std::string response = send_jenkins_post_request(jenkinsData.stop_url_f, config.getJenkinsUser(), config.getJenkinsToken());
                
                // 만약 response_data가 비어있다면 정지 요청이 성공한 것으로 간주
                if (response.empty()) {
                    dpp::message msg("프론트엔드 서버가 성공적으로 정지되었습니다.");
                    event.reply(dpp::ir_update_message, msg);
                } else {
                    dpp::message msg("프론트엔드 서버 정지에 실패했습니다. 응답: " + response);
                    event.reply(dpp::ir_update_message, msg);
                }
            } catch (const std::exception &e) {
                // 예외 발생 시 에러 메시지 출력
                dpp::message msg("프론트엔드 서버 정지 중 에러가 발생했습니다: " + std::string(e.what()));
                event.reply(dpp::ir_update_message, msg);
            }
        } else if (event.custom_id == "백엔드 정지") {
            try {
                std::string response = send_jenkins_post_request(jenkinsData.stop_url_b, config.getJenkinsUser(), config.getJenkinsToken());
                
                if (response.empty()) {
                    dpp::message msg("백엔드 서버가 성공적으로 정지되었습니다.");
                    event.reply(dpp::ir_update_message, msg);
                } else {
                    dpp::message msg("백엔드 서버 정지에 실패했습니다. 응답: " + response);
                    event.reply(dpp::ir_update_message, msg);
                }
            } catch (const std::exception &e) {
                dpp::message msg("백엔드 서버 정지 중 에러가 발생했습니다: " + std::string(e.what()));
                event.reply(dpp::ir_update_message, msg);
            }
        }
        else if (event.custom_id == "AI 정지") {
            try {
                std::string response = send_jenkins_post_request(jenkinsData.stop_url_b, config.getJenkinsUser(), config.getJenkinsToken());
                
                if (response.empty()) {
                    dpp::message msg("AI 서버가 성공적으로 정지되었습니다.");
                    event.reply(dpp::ir_update_message, msg);
                } else {
                    dpp::message msg("AI 서버 정지에 실패했습니다. 응답: " + response);
                    event.reply(dpp::ir_update_message, msg);
                }
            } catch (const std::exception &e) {
                dpp::message msg("AI 서버 정지 중 에러가 발생했습니다: " + std::string(e.what()));
                event.reply(dpp::ir_update_message, msg);
            }
        }

        /* 상태에 관한 처리 */
        else if (event.custom_id == "상태상세") {
            // 초기 로딩 메시지
            event.reply(dpp::ir_update_message, "⏳로딩 중...");

            // 프론트엔드 서버 상태
            try {
                std::string response_f = get_jenkins_status(jenkinsData.status_url_f, config.getJenkinsUser(), config.getJenkinsToken());
                auto json_data_f = nlohmann::json::parse(response_f);
                std::string status_f = determine_deployment_status(json_data_f);
                event.edit_response(dpp::message(event.command.channel_id, "프론트엔드 서버 상태: " + status_f + "\n로딩 중..."));
                // event.reply(dpp::ir_update_message, "프론트엔드 서버 상태: " + status_f + "\n로딩 중...");

                // 백엔드 서버 상태
                std::string response_b = get_jenkins_status(jenkinsData.status_url_b, config.getJenkinsUser(), config.getJenkinsToken());
                auto json_data_b = nlohmann::json::parse(response_b);
                std::string status_b = determine_deployment_status(json_data_b);
                // event.edit_response(dpp::message(event.command.channel_id, "프론트엔드 서버 상태: " + status_f + "\n백엔드 서버 상태: " + status_b + "\n로딩 중..."));
                event.edit_response(dpp::message(event.command.channel_id, "프론트엔드 서버 상태: " + status_f + "\n백엔드 서버 상태: " + status_b));
                // event.reply(dpp::ir_update_message, "백엔드 서버 상태: " + status_b);

                // AI 서버 상태
                // std::string response_ai = get_jenkins_status(jenkinsData.status_url_ai, config.getJenkinsUser(), config.getJenkinsToken());
                // auto json_data_ai = nlohmann::json::parse(response_ai);
                // std::string status_ai = determine_deployment_status(json_data_ai);

                // 최종 메시지
                // event.edit_response(dpp::message(event.command.channel_id, "프론트엔드 서버 상태: " + status_f + "\n백엔드 서버 상태: " + status_b + "\nAI 서버 상태: " + status_ai));
                // event.edit_response(dpp::message(event.command.channel_id, "프론트엔드 서버 상태: " + status_f + "\n백엔드 서버 상태: " + status_b));
            } catch (const std::exception &e) {
                // event.reply(dpp::ir_update_message, "에러 발생: " + std::string(e.what()));
                dpp::message error_msg(event.command.channel_id, "⚠️ 상호작용 실패: " + std::string(e.what()));
                error_msg.set_flags(dpp::m_ephemeral); // 메시지를 임시로 설정(선택 사항)
                error_msg.components.clear(); // 버튼 삭제
                event.edit_response(error_msg);
            }

        }
        
        else {
            dpp::message msg("잘못된작동");
            event.reply(dpp::ir_update_message, msg);
        }
    });

    /* 봇의 초기동작설정 */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            /* Create and register a command when the bot is ready */
            // bot.global_bulk_command_delete();
            bot.global_command_create(dpp::slashcommand("embed", "Send a test embed!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("ping", "ping pong!", bot.me.id));
            // bot.global_command_create(dpp::slashcommand("배포시작", "배포 시작하기", bot.me.id));
            // bot.global_command_create(dpp::slashcommand("배포현황", "배포 상태보기", bot.me.id));
            // bot.global_command_create(dpp::slashcommand("배포중단", "배포 중단하기", bot.me.id));
            bot.global_command_create(dpp::slashcommand("서버", "서버의 기능들 버튼으로 생성", bot.me.id));
        }
    });
    bot.start(dpp::st_wait);
 
    return 0;
}
