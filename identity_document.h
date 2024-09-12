#pragma once

#include <iostream>
#include <string>
#include <string_view>

using namespace std::string_view_literals;

class IdentityDocument {
	
public:
	struct VTable {
		//void(*printIDptr)(int) = VirtPrintID;
		void(*printIDptr)(const IdentityDocument*) = VirtPrintID;
	};

    IdentityDocument()
        : unique_id_(++unique_id_count_) 
    {
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }
	
	IdentityDocument(VTable* vtable)
		: vtable_(vtable),
		unique_id_(++unique_id_count_)
	{
		std::cout << "IdentityDocument::Ctor(v) : "sv << unique_id_ << std::endl;
	}


    /*virtual*/ ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_) 
    {
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    //IdentityDocument(IdentityDocument&& other) = delete;

    void PrintID() const {
        //VirtPrintID(unique_id_);
		//функция должна быть без параметров, т.к. таблица будет подсовываться другая - от "наследников"
        //vtable_->printIDptr(unique_id_);
		vtable_->printIDptr(this);
    }

    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

//protected:
    int GetID() const {
        return unique_id_;
    }

private:
    //таблица виртуальных функций
    static VTable Ident_vtable_;

	VTable* vtable_ = &Ident_vtable_;
	
    static int unique_id_count_;
    int unique_id_;
    
	
    
    
    /*static void VirtPrintID(int value) {
        std::cout << "IdentityDocument::PrintID() : "sv << value << std::endl;
    }*/
	static void VirtPrintID(const IdentityDocument* ident_doc) {
		std::cout << "IdentityDocument::PrintID() : "sv << ident_doc->GetID() << std::endl;
	}
};

int IdentityDocument::unique_id_count_ = 0;
IdentityDocument::VTable IdentityDocument::Ident_vtable_;