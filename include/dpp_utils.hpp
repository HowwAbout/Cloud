#ifndef DPP_UTILS_HPP
#define DPP_UTILS_HPP

#include <dpp/dpp.h>
#include <string>

dpp::component create_button(const std::string& label, dpp::component_type type, dpp::component_style style, const std::string& id);

#endif 