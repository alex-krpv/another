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
//Earn 2000-01-02 2000-01-06 20
//ComputeIncome 2000-01-01 2001-01-01
//PayTax 2000-01-02 2000-01-03 13
//ComputeIncome 2000-01-01 2001-01-01
//Spend 2000-12-30 2001-01-02 14
//ComputeIncome 2000-01-01 2001-01-01
//PayTax 2000-12-30 2000-12-30 13
//ComputeIncome 2000-01-01 2001-01-01

//Вывод
//20
//18.96
//8.46
//8.46

	//Earn 2001 - 02 - 12 2001 - 02 - 23 12000
//ComputeIncome 2001 - 02 - 05 2001 - 02 - 16
//PayTax 2001 - 02 - 01 2001 - 02 - 14 43
//ComputeIncome 2001 - 02 - 03 2001 - 02 - 13
//PayTax 2001 - 02 - 07 2001 - 02 - 23 28
//ComputeIncome 2001 - 02 - 12 2001 - 02 - 23
//
//Вывод:
//5000
//1140
//7711.2
	void RequestApply() {
		
		//проверка правильности расчета налогов
		{
			BudgetManager manager;
			ParseAndProcessQuery(manager, "Earn 2001-02-12 2001-02-23 12000"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2001-02-05 2001-02-16"s); //5000
			ParseAndProcessQuery(manager, "PayTax 2001-02-01 2001-02-14 43"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2001-02-03 2001-02-13"s); //1140
			ParseAndProcessQuery(manager, "PayTax 2001-02-07 2001-02-23 28"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2001-02-12 2001-02-23"s); //7711.2
		}
		//проверка правильности расчета трат
		{
			BudgetManager manager;
			ParseAndProcessQuery(manager, "Earn 2001-02-12 2001-02-23 12000"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2001-02-12 2001-02-23"s); 	
			ParseAndProcessQuery(manager, "Spend 2001-02-12 2001-02-23 10000"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2001-02-12 2001-02-23"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2001-02-12 2001-02-23"s);
		}
		//проверка примера из условия задачи
		{
			BudgetManager manager;
			
			ParseAndProcessQuery(manager, "Earn 2000-01-02 2000-01-06 20"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2000-01-02 2001-01-01"s);
			ParseAndProcessQuery(manager, "PayTax 2000-01-02 2000-01-03 13"s);

			ParseAndProcessQuery(manager, "ComputeIncome 2000-01-01 2001-01-01"s);
			ParseAndProcessQuery(manager, "Spend 2000-12-30 2001-01-02 14"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2000-01-01 2001-01-01"s);

			ParseAndProcessQuery(manager, "PayTax 2000-12-30 2000-12-30 13"s);
			ParseAndProcessQuery(manager, "ComputeIncome 2000-01-01 2001-01-01"s);
			


		}
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
