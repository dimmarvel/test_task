#pragma once
#include <string>
#include <unordered_map>

namespace api
{
    using pattern = std::string;
    using shift = std::string;
    using change_rules = std::unordered_map<pattern, shift>;
    using file_path = std::string;
    using task_id_t = size_t;
}