#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std::string_view_literals;

//class Passport : public IdentityDocument {
//public:
//    Passport()
//        : expiration_date_(GetExpirationDate()) 
//    {
//        std::cout << "Passport::Ctor()"sv << std::endl;
//    }
//
//    Passport(const Passport& other)
//        : IdentityDocument(other)
//        , expiration_date_(other.expiration_date_) 
//    {
//        std::cout << "Passport::CCtor()"sv << std::endl;
//    }
//
//    ~Passport() {
//        std::cout << "Passport::Dtor()"sv << std::endl;
//    }
//
//    void PrintID() const {
//        std::cout << "Passport::PrintID() : "sv << GetID();
//        std::cout << " expiration date : "sv << expiration_date_.tm_mday << "/"sv << expiration_date_.tm_mon << "/"sv
//                  << expiration_date_.tm_year + 1900 << std::endl;
//    }
//
//    virtual void PrintVisa(const std::string& country) const {
//        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << GetID() << std::endl;
//    }
//
//private:
//    const struct tm expiration_date_;
//
//    tm GetExpirationDate() {
//	    time_t t = time(nullptr);
//	    tm exp_date = *localtime(&t);
//	    exp_date.tm_year += 10;
//	    mktime(&exp_date);
//	    return exp_date;
//	}
//};
class Passport {
	struct VTable {
		//void(*printIDptr)(int, const tm&) = VirtPrintID;
		//void(*printIDptr)(const Passport*) = VirtPrintID;
		void(*printIDptr)(void*) = VirtPrintID;
		void(*deleteptr)(void*) = VirtDelete;
		void(*destructptr)(void*) = VirtDestruct;
	};
public:
	Passport()
		: identity_document_(IdentityDocument(reinterpret_cast<IdentityDocument::VTable*>(vtable_))),
		expiration_date_(GetExpirationDate())
	{
		std::cout << "Passport::Ctor()"sv << std::endl;
	}

	Passport(const Passport& other)
		: identity_document_(other.identity_document_)
		, expiration_date_(other.expiration_date_)
	{
		std::cout << "Passport::CCtor()"sv << std::endl;
	}

	~Passport() {
	//Порядок действий при вызове деструктора:
	//Просто удаляем объект и не химичим, положась на компилятор;
	//Если объект наследуемого класса, то не забыть переставить указатель vtable 
	//на виртуальную таблицу базового класса.
		std::cout << "Passport::Dtor()"sv << std::endl;
		//identity_document_.~IdentityDocument();
		identity_document_.SetVtable(identity_document_.GetVtable());
		//vtable_->destrustptr;
	}

	operator IdentityDocument() { 
		return identity_document_; 
	}

	void PrintID() /*const*/ {
		/*std::cout << "Passport::PrintID() : "sv << identity_document_.GetID();
		std::cout << " expiration date : "sv << expiration_date_.tm_mday << "/"sv << expiration_date_.tm_mon << "/"sv
			<< expiration_date_.tm_year + 1900 << std::endl;*/
		//VirtPrintID(this);
		vtable_->printIDptr(this);
		
	}

	/*virtual*/ void PrintVisa(const std::string& country) const {
		std::cout << "Passport::PrintVisa("sv << country << ") : "sv << identity_document_.GetID() << std::endl;
	}

	void PrintUniqueIDCount() {
		IdentityDocument::PrintUniqueIDCount();
	}

	void Delete() {
		//this-> ~Passport();
		vtable_->deleteptr(this);
	}

private:
	//таблица виртуальных функций
	static VTable passport_vtable_;
	VTable* vtable_ = &passport_vtable_;
	IdentityDocument identity_document_;
	const struct tm expiration_date_;

	tm GetExpirationDate() {
		time_t t = time(nullptr);
		tm exp_date = *localtime(&t);
		exp_date.tm_year += 10;
		mktime(&exp_date);
		return exp_date;
	}

	/*static void VirtPrintID(const Passport* passport) {
		std::cout << "Passport::PrintID() : "sv << passport->identity_document_.GetID();
		std::cout << " expiration date : "sv << passport->expiration_date_.tm_mday << "/"sv << passport->expiration_date_.tm_mon << "/"sv
			<< passport->expiration_date_.tm_year + 1900 << std::endl;
	}*/
	static void VirtPrintID(void* passport) {
		auto ptr = reinterpret_cast<Passport*>(passport);
		std::cout << "Passport::PrintID() : "sv << ptr->identity_document_.GetID();
		std::cout << " expiration date : "sv << ptr->expiration_date_.tm_mday << "/"sv 
			<< ptr->expiration_date_.tm_mon << "/"sv
			<< ptr->expiration_date_.tm_year + 1900 << std::endl;
	}

	static void VirtDestruct(void* passport) {
		auto ptr = reinterpret_cast<Passport*>(passport);
		ptr-> ~Passport();

	}
	static void VirtDelete(void* passport) {
		auto ptr = reinterpret_cast<Passport*>(passport);
		ptr-> ~Passport();
	}
};

Passport::VTable Passport::passport_vtable_;
