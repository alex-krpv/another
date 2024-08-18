#include "budget_manager.h"

#include <numeric>

//Поступление средств на счёт за определённый период.
//Распределяет средства равномерно по дням.
//обе передаваемые даты входят в диапазон

//void BudgetManager::SetEarnings(const Date& date_from, const Date& date_to, double value) {
//    //с какого элемента начнем заполнение
//    int start_index = Date::ComputeDistance(START_DATE, date_from);
//    //сколько элементов нужно заполнить
//    int length = Date::ComputeDistance(date_from, date_to) + 1;
//    double earning_per_day = value / length;
//    for (size_t i = start_index; i <= length; ++i) {
//        earnings_[i].earning += earning_per_day;
//    }
//}

void BudgetManager::SetEarnings(const Date& date_from, const Date& date_to, double value) {
    //полуинтервал элементов для заполнения
    auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, date_from));
    auto last = next(first, static_cast<size_t>(Date::ComputeDistance(date_from, date_to)) + 1);
    //auto [first, last]  = MakeIteratorsFromDates(date_from, date_to);
    double earning_per_day = value / distance(first,last);

    for (auto iter = first; iter != last; ++iter) {
        iter->earning += earning_per_day;
    }
}

void BudgetManager::SetSpendings(const Date& date_from, const Date& date_to, double value) {
    //полуинтервал элементов для заполнения
    auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, date_from));
    auto last = next(first, static_cast<size_t>(Date::ComputeDistance(date_from, date_to)) + 1);
    //auto [first, last]  = MakeIteratorsFromDates(date_from, date_to);
    double spending_per_day = value / distance(first, last);
    for (auto iter = first; iter != last; ++iter) {
        iter->spending += spending_per_day;
    }
}

double BudgetManager::GetEarnings(const Date& date_from, const Date& date_to) const{
    auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, date_from));
    auto last = next(first, static_cast<size_t>(Date::ComputeDistance(date_from, date_to)) + 1);
    return std::accumulate(first, last, 0., 
        [](double sum, const DayInfo& rhs) {return sum + rhs.earning - rhs.spending; });
}

void BudgetManager::PayTax(const Date& date_from, const Date& date_to, size_t percent_quant) {

    //auto start_index = Date::ComputeDistance(START_DATE, date_from);
    //int length = Date::ComputeDistance(date_from, date_to) + 1;
    auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, date_from));
    auto last = next(first, static_cast<size_t>(Date::ComputeDistance(date_from, date_to)) + 1);

    /*for (size_t i = start_index; i <= length; ++i) {
        earnings_[i].earning *= 0.87;
    }*/
     
    for (auto iter = first; iter != last; ++iter) {
        iter->earning *= (1.0-static_cast<double>(percent_quant)/100);
    }
}

