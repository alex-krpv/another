#include "json_builder.h"
#include <iostream>


using namespace std;

int main() {
    //json::Print(
    //    json::Document{
    //        json::Builder{}
    //        .StartDict()
    //            .Key("key1"s).Value(123)
    //            .Key("key2"s).Value("value2"s)
    //            .Key("key3"s).StartArray()
    //                .Value(456)
    //                .StartDict().EndDict()
    //                .StartDict()
    //                    .Key(""s)
    //                    .Value(nullptr)
    //                .EndDict()
    //                .Value(""s)
    //            .EndArray()
    //        .EndDict()
    //        .Build()
    //    },
    //    cout
    //);
    //cout << endl;
    
    json::Print(
        json::Document{
            json::Builder{}
            .Value("just a string"s)
            .Build()
        },
        std::cout
    );
    std::cout << endl;
    json::Print(
        json::Document{
            json::Builder{}
            .Value(123)
            .Build()
        },
        std::cout
    );
    std::cout << endl;
    bool tru = true;
    json::Print(
        json::Document{
            json::Builder{}
            .Value(tru)
            .Build()
        },
        std::cout
    );

    std::cout << endl;
    
    json::Print(
        json::Document{
            json::Builder{}
            .Value(nullptr)
            .Build()
        },
        std::cout
    );

    std::cout << endl;

    json::Print(
        json::Document{
            json::Builder{}
            .StartArray()
                    .Value(456)
                    .Value(123.5)
                    .Value("hello"s)
					.Value(false)
					.Value(nullptr)
            .EndArray()
            .Build()
        },
        std::cout
    );
    std::cout << endl;

	json::Print(
	    json::Document{
	        json::Builder{}
	        .StartDict()
	            .Key("key1"s).Value(123)
	            .Key("key2"s).Value("value2"s)
	            .Key("key3"s).Value(nullptr)

	        .EndDict()
	        .Build()
	    },
	    cout
	);
	cout << endl << "array in array\n"s;
    json::Print(
        json::Document{
            json::Builder{}
            .StartArray()
                    .Value(456)
                    .StartArray()
                    .Value(123.5)
                    .Value("hello"s)
                    .EndArray()
                    .Value(false)
                    .Value(nullptr)
            .EndArray()
            .Build()
        },
        std::cout
    );
}