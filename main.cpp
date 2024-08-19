#include "parser.h"

#include <cassert>
#include <iostream>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    auto query = ParseQuery(line);

    if (!query) {
        return;
    }

    query->ProcessAndPrint(manager, cout);
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

namespace tests {
    inline const double EPSILON = 1e-6;
    //void ManagerEarningsSetGet() {
    //    //istringstream in("2000 - 01 - 02 2000 - 01 - 06 20");
    //    {
    //        BudgetManager manager;
    //        manager.SetEarnings(Date{ 2000,1,2 }, Date{ 2000,1,6 }, 20);
    //        assert(manager.GetEarnings(Date{ 2000,1,2 }, Date{ 2000,1,6 }) == 20);
    //        assert(manager.GetEarnings(Date{ 2000,1,2 }, Date{ 2000,1,5 }) == 16);
    //        manager.PayTax(Date{ 2000,1,2 }, Date{ 2000,1,3 }, 13);
    //        assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2001,1,1 }) - 18.96) < EPSILON);
    //        manager.SetEarnings(Date{ 2000,1,3 }, Date{ 2000,1,3 }, 10);
    //        assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2001,1,1 }) - 28.96) < EPSILON);
    //        cout << "Tests1 passed" << endl;
    //    }
    //    //тестирование граничных значений дат
    //    {
    //        BudgetManager manager;
    //        manager.SetEarnings(Date{ 2000,1,1 }, Date{ 2000,1,1 }, 20);
    //        assert(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2099,12,31 }) == 20);
    //        manager.PayTax(Date{ 2000,1,1 }, Date{ 2000,1,3 }, 13);
    //        assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2099,12,13 }) - 17.4) < EPSILON);
    //        manager.SetEarnings(Date{ 2099,12,31 }, Date{ 2099,12,31 }, 10);
    //        assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2099,12,31 }) - 27.4) < EPSILON);
    //        manager.PayTax(Date{ 2099,12,31 }, Date{ 2099,12,31 }, 13);
    //        assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2099,12,31 }) - 26.1) < EPSILON);
    //        cout << "Tests2 passed" << endl;
    //    }


    //}
    void RequestApply() {
        BudgetManager manager;
        ParseAndProcessQuery(manager, "Earn 2000-01-02 2000-01-06 20"s);
        //assert(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2099,12,31 }) == 20);
        ParseAndProcessQuery(manager, "ComputeIncome 2000-01-02 2000-01-06"s);
        ParseAndProcessQuery(manager, "PayTax 2000-01-02 2000-01-03 13"s);
        //assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2001,1,1 }) - 18.96) < EPSILON);
        ParseAndProcessQuery(manager, "Spend 2000-12-30 2001-01-02 14"s);
        //assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2001,1,1 }) - 8.46) < EPSILON);
        ParseAndProcessQuery(manager, "PayTax 2000-12-30 2000-12-30 13"s);
        //assert(std::abs(manager.GetEarnings(Date{ 2000,1,1 }, Date{ 2001,1,1 }) - 8.46) < EPSILON);

    }
}


int main() {
    tests::RequestApply();
    /*BudgetManager manager;

    const int query_count = ReadNumberOnLine(cin);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(cin, line);
        ParseAndProcessQuery(manager, line);
    }*/
}
