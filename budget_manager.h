#pragma once
#include <utility>
#include "date.h"

class BudgetManager {
public:
    BudgetManager() = default;
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};
    struct DayInfo {
        double earning = 0;
        double spending = 0;
    };
    //Поступление средств на счёт за определённый период.
    //Распределяет средства равномерно по дням.
    
    void SetEarnings(const Date& date_from, const Date& date_to, double value);
    //template<typename T>
    //void SetEarnings(T&& date_from, T&& date_to, double value) {
    //    
    //    //полуинтервал элементов для заполнения
    //    //auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, date_from));
    //    //auto last = next(first, static_cast<size_t>(Date::ComputeDistance(date_from, date_to)) + 1);
    //    auto [first, last] = MakeIteratorsFromDates(std::forward<T>(date_from), std::forward<T>(date_to));
    //    double earning_per_day = value / distance(first, last);

    //    for (auto iter = first; iter != last; ++iter) {
    //        iter->earning += earning_per_day;
    //    }
    //    
    //}

    void SetSpendings(const Date& date_from, const Date& date_to, double value);

    void PayTax(const Date& date_from, const Date& date_to, size_t percent_quant);
    double GetEarnings(const Date& date_from, const Date& date_to) const;

private:
    /*template<typename RandomIt>
    std::pair<RandomIt, RandomIt> MakeIteratorsFromDates(const Date& date_from, const Date& date_to)const;*/
    template<typename RandomIt, typename T>
    std::pair<RandomIt, RandomIt> MakeIteratorsFromDates(T&& date_from, T&& date_to) const {
        auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, std::forward<T>(date_from)));
        auto last = next(first, static_cast<size_t>(Date::ComputeDistance(std::forward<T>(date_from), std::forward<T>(date_to))) + 1);
        return{ first,last };
    }
    std::vector<DayInfo> earnings_ = std::vector<DayInfo>(Date::ComputeDistance(START_DATE,END_DATE),{0});
};

//template<typename RandomIt>
//std::pair<RandomIt, RandomIt> BudgetManager::MakeIteratorsFromDates(const Date& date_from, const Date& date_to) const{
//    auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, date_from));
//    auto last = next(first, static_cast<size_t>(Date::ComputeDistance(date_from, date_to)) + 1);
//    return{ first,last };
//}
//template<typename T>
//void BudgetManager::SetEarnings(T&& date_from, T&& date_to, double value) {
//    //полуинтервал элементов для заполнения
//    //auto first = next(earnings_.begin(), Date::ComputeDistance(START_DATE, date_from));
//    //auto last = next(first, static_cast<size_t>(Date::ComputeDistance(date_from, date_to)) + 1);
//    auto [first, last] = MakeIteratorsFromDates(std::forward<T>(date_from), std::forward<T>(date_to));
//    double earning_per_day = value / distance(first, last);
//
//    for (auto iter = first; iter != last; ++iter) {
//        iter->earning += earning_per_day;
//    }
//}