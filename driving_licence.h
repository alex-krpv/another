#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

//class DrivingLicence : public IdentityDocument {
//public:
//    DrivingLicence() {
//        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
//    }
//
//    DrivingLicence(const DrivingLicence& other)
//        : IdentityDocument(other) 
//    {
//        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
//    }
//
//    ~DrivingLicence() {
//        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
//    }
//
//    void PrintID() const {
//        std::cout << "DrivingLicence::PrintID() : "sv << GetID() << std::endl;
//    }
//};

class DrivingLicence {
	struct VTable {
		//void(*printIDptr)(int, const tm&) = VirtPrintID;
		//void(*printIDptr)(const Passport*) = VirtPrintID;
		void(*printIDptr)(void*) = VirtPrintID;
		void(*deleteptr)(void*) = VirtDelete;
	};
public:
	DrivingLicence() 
		: identity_document_(IdentityDocument(reinterpret_cast<IdentityDocument::VTable*>(vtable_)))
		
	{
		std::cout << "DrivingLicence::Ctor()"sv << std::endl;
	}

	DrivingLicence(const DrivingLicence& other)
		: identity_document_(other.identity_document_)
	{
		std::cout << "DrivingLicence::CCtor()"sv << std::endl;
	}

	~DrivingLicence() {
		std::cout << "DrivingLicence::Dtor()"sv << std::endl;
		identity_document_.SetVtable(identity_document_.GetVtable());
	}

	operator IdentityDocument() {
		return identity_document_;
	}



	void PrintID() {
		vtable_->printIDptr(this);
	}

	int GetID() {
		return identity_document_.GetID();
	}

	void Delete() {
		//this-> ~Passport();
		vtable_->deleteptr(this);
	}

private:
	//таблица виртуальных функций
	static VTable licence_vtable_;
	VTable* vtable_ = &licence_vtable_;
	IdentityDocument identity_document_;

	static void VirtPrintID(void* licence) {
		auto ptr = reinterpret_cast<DrivingLicence*>(licence);
		std::cout << "DrivingLicence::PrintID() : "sv << ptr->identity_document_.GetID() << std::endl;
	}

	static void VirtDelete(void* licence) {
		auto ptr = reinterpret_cast<DrivingLicence*>(licence);
		ptr-> ~DrivingLicence();
	}
};

DrivingLicence::VTable DrivingLicence::licence_vtable_;