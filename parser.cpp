#include "parser.h"

/**
 * Удаляет пробелы в начале и конце строки
 */
std::string_view Trim(std::string_view string) {
    const auto start = string.find_first_not_of(' ');
    if (start == string.npos) {
        return {};
    }
    return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

/**
 * Разбивает строку string на n строк, с помощью указанного символа-разделителя delim
 */
std::vector<std::string_view> Split(std::string_view string, char delim) {
    std::vector<std::string_view> result;

    size_t pos = 0;
    while ((pos = string.find_first_not_of(' ', pos)) < string.length()) {
        auto delim_pos = string.find(delim, pos);
        if (delim_pos == string.npos) {
            delim_pos = string.size();
        }
        if (auto substr = Trim(string.substr(pos, delim_pos - pos)); !substr.empty()) {
            result.push_back(substr);
        }
        pos = delim_pos + 1;
    }

    return result;
}

//Три вида запросов:
//Earn 2000-01-02 2000-01-06 20
//ComputeIncome 2000-01-01 2001-01-01
//PayTax 2000-01-02 2000-01-03

CommandDescription ParseCommandDescription(std::string_view line) {
    /*auto colon_pos = line.find(':');
    if (colon_pos == line.npos) {
        return {};
    }*/
    //std::string_view trimmed_line = Trim(line);
    ////первый пробел - после команды
    //auto space_command_pos = trimmed_line.find(' ');
    //if (space_command_pos == line.npos) {
    //    return {};
    //}
    ////второй пробел - после первой даты
    //auto space_first_date_pos = trimmed_line.find(' ',space_command_pos);
    //if (space_first_date_pos == line.npos) {
    //    return {};
    //}
    ////(только для команды Earn) третий пробел - после второй даты
    //auto space_second_date_pos = trimmed_line.find(' ', space_first_date_pos);
    //if (space_second_date_pos == line.npos) {
    //    return {};
    //}
    //auto not_space = line.find_first_not_of(' ', space_pos);
    //if (not_space >= colon_pos) {
    //    return {};
    //}
    auto request_strings = Split(line, ' ');
    
    if (request_strings.size() == 3) {
        return{ std::string(request_strings[0]),
            std::string(request_strings[1]),
            std::string(request_strings[2]),
            {}
        };
    }

    if (request_strings.size() == 4) {
        return{ std::string(request_strings[0]),
            std::string(request_strings[1]),
            std::string(request_strings[2]),
            std::string(request_strings[3])
        };
    }
    else return {};
}

void Request::ParseLine(std::string_view line) {
    command_description_ = ParseCommandDescription(line);
}

void Request::ApplyCommand(std::ostream* out) {
    if (command_description_.command == EARN_REQUEST_STRING) {
        manager_.SetEarnings(Date::FromString(command_description_.date_from),
            Date::FromString(command_description_.date_to),
            std::stod(command_description_.description));
    }
    else if (command_description_.command == COMPUTE_REQUEST_STRING) {
        *out << manager_.GetEarnings(Date::FromString(command_description_.date_from),
            Date::FromString(command_description_.date_to))<<std::endl;
    }
    else if (command_description_.command == PAY_TAX_REQUEST_STRING){
        manager_.PayTax(Date::FromString(command_description_.date_from),
            Date::FromString(command_description_.date_to),
            std::stod(command_description_.description));
    }
    else manager_.SetSpendings(Date::FromString(command_description_.date_from),
        Date::FromString(command_description_.date_to),
        std::stod(command_description_.description));
}