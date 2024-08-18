#pragma once
#include "date.h"
#include "budget_manager.h"

#include <iostream>
#include <string>
#include <string_view>

using namespace std::literals;
inline const std::string& EARN_REQUEST_STRING = "Earn"s;
inline const std::string& COMPUTE_REQUEST_STRING = "ComputeIncome"s;
inline const std::string& PAY_TAX_REQUEST_STRING = "PayTax"s;
inline const std::string& SPEND_REQUEST_STRING = "Spend"s;



struct CommandDescription {
    // Определяет, задана ли команда (поле command непустое)
    explicit operator bool() const {
        return !command.empty();
    }

    bool operator!() const {
        return !operator bool();
    }
    std::string command;
    std::string date_from;
    std::string date_to;
    std::string description;
};

class Request {
public:
    Request(BudgetManager& manager) :
        manager_(manager) {}
	//чтение запроса
	//void ParseLine(std::istream& input);
    //Парсит строку в структуру CommandDescription и сохраняет результат в command_description_
    void ParseLine(std::string_view line);
    //выполняет команду
    //virtual void ApplyCommand(void) = 0;
    void ApplyCommand(std::ostream* out = nullptr);

private:
    CommandDescription command_description_;
    BudgetManager& manager_;
};

class ReadRequest : Request {
public:
    //virtual void ApplyCommand(void) override;
private:
    //const BudgetManager& budget_manager_;
};

class ModifyRequest : Request {
public:
    //virtual void ApplyCommand(void) override;
private:
    //BudgetManager& budget_manager_;

};
// напишите в этом классе код, ответственный за чтение запросов
