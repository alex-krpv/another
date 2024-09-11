#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument {
public:
    IdentityDocument()
        : unique_id_(++unique_id_count_) 
    {
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    virtual ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_) 
    {
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void PrintID() const {
        //VirtPrintID(unique_id_);
        vtable_->printIDptr(unique_id_);
    }

    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

protected:
    int GetID() const {
        return unique_id_;
    }

private:
    static int unique_id_count_;
    int unique_id_;
    //������� ����������� �������
    static struct VTable {
        void(*printIDptr)(int) = VirtPrintID;
    };
    VTable* vtable_;
    
    static void VirtPrintID(int value) {
        std::cout << "IdentityDocument::PrintID() : "sv << value << std::endl;
    }
};

int IdentityDocument::unique_id_count_ = 0;