#include "dpp_utils.hpp"

/* 버튼생성함수 */
dpp::component create_button(const std::string& label, dpp::component_type type, dpp::component_style style, const std::string& id) {
    return dpp::component()
        .set_label(label)
        .set_type(type)
        .set_style(style)
        .set_id(id);
}