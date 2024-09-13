#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

//class InternationalDrivingLicence : public DrivingLicence {
//public:
//    InternationalDrivingLicence() {
//        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
//    }
//
//    InternationalDrivingLicence(const InternationalDrivingLicence& other)
//        : DrivingLicence(other) 
//    {
//        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
//    }
//
//    ~InternationalDrivingLicence() {
//        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
//    }
//
//    void PrintID() const {
//        std::cout << "InternationalDrivingLicence::PrintID() : "sv << GetID() << std::endl;
//    }
//};

class InternationalDrivingLicence {
public:
	struct VTable {
		//void(*printIDptr)(int, const tm&) = VirtPrintID;
		//void(*printIDptr)(const Passport*) = VirtPrintID;
		void(*printIDptr)(void*) = VirtPrintID;
	};

	InternationalDrivingLicence() {
		std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
	}

	InternationalDrivingLicence(const InternationalDrivingLicence& other)
		: driving_licence_(other.driving_licence_)
	{
		std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
	}

	~InternationalDrivingLicence() {
		std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
	}

	/*void PrintID() const {
		std::cout << "InternationalDrivingLicence::PrintID() : "sv << GetID() << std::endl;
	}*/
	void PrintID() {
		vtable_->printIDptr(this);
	}
	operator DrivingLicence() {
		return driving_licence_;
	}

private:
	static VTable int_licence_vtable_;
	VTable* vtable_ = &int_licence_vtable_;
	DrivingLicence driving_licence_;

	static void VirtPrintID(void* licence) {
		auto ptr = reinterpret_cast<InternationalDrivingLicence*>(licence);
		std::cout << "InternationalDrivingLicence::PrintID() : "sv << ptr->driving_licence_.GetID() << std::endl;
	}
};

InternationalDrivingLicence::VTable InternationalDrivingLicence::int_licence_vtable_;