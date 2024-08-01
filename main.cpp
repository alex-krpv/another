#include "json_builder.h"
#include <iostream>


using namespace std;

int main() {
    json::Print(
        json::Document{
            json::Builder{}
            .StartDict()
                .Key("key1"s).Value(123)
                .Key("key2"s).Value("value2"s)
                .Key("key3"s).StartArray()
                    .Value(456)
                    .StartDict().EndDict()
                    .StartDict()
                        .Key(""s)
                        .Value(nullptr)
                    .EndDict()
                    .Value(""s)
                .EndArray()
            .EndDict()
            .Build()
        },
        cout
    );
    cout << endl;
    
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
	cout << endl << "array in Dict\n"s;
	json::Print(
	    json::Document{
	        json::Builder{}
	        .StartDict()
	            .Key("key1"s).Value(123)
	            .Key("key2"s).Value("value2"s)
	            .Key("key3"s).StartArray()
	                .Value(456)
	                .Value(""s)
	            .EndArray()
	        .EndDict()
	        .Build()
	    },
	    cout
	);
	cout << endl;
	try {
		json::Builder{}.Build();
	}
	catch (const std::logic_error& error) {
		cout << error.what() << endl;
	}
	try {
		json::Builder{}.StartDict().Build();
	}
	catch (const std::logic_error& error) {
		cout << error.what() << endl;
	}
	try {
		json::Builder{}.StartArray().Build();
	}
	catch (const std::logic_error& error) {
		cout << error.what() << endl;
	}

	try {
		json::Builder{}.StartDict().Value("2"s).EndDict();
	}
	catch (const std::logic_error& error) {
		cout << error.what() << endl;
	}
	
	//json::Builder{}.Value("s"s).Key("1"s).Build(); //OK
	//json::Builder{}.Value("s"s).Value("1"s).Build();
	json::Builder{}.Value("s"s).StartDict().Build();
	//json::Builder{}.Value("s"s).StartArray().Build();
	//json::Builder{}.Value("s"s).EndDict().Build();
	//json::Builder{}.Value("s"s).EndArray().Build();
	
	
}