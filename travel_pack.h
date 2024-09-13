#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

//class TravelPack : public IdentityDocument {
//public:
//    TravelPack()
//        : identity_doc1_(new Passport())
//        , identity_doc2_(new DrivingLicence()) 
//    {
//        std::cout << "TravelPack::Ctor()"sv << std::endl;
//    }
//
//    TravelPack(const TravelPack& other)
//        : IdentityDocument(other)
//        , identity_doc1_(new Passport(*dynamic_cast<Passport*>(other.identity_doc1_)))
//        , identity_doc2_(new DrivingLicence(*dynamic_cast<DrivingLicence*>(other.identity_doc2_)))
//        , additional_pass_(other.additional_pass_)
//        , additional_dr_licence_(other.additional_dr_licence_) 
//    {
//        std::cout << "TravelPack::CCtor()"sv << std::endl;
//    }
//
//    ~TravelPack() {
//        delete identity_doc1_;
//        delete identity_doc2_;
//        std::cout << "TravelPack::Dtor()"sv << std::endl;
//    }
//
//    void PrintID() const {
//        identity_doc1_->PrintID();
//        identity_doc2_->PrintID();
//        additional_pass_.PrintID();
//        additional_dr_licence_.PrintID();
//    }
//
//private:
//    IdentityDocument* identity_doc1_;
//    IdentityDocument* identity_doc2_;
//    Passport additional_pass_;
//    DrivingLicence additional_dr_licence_;
//};

class TravelPack{
	struct VTable {
		//void(*printIDptr)(int, const tm&) = VirtPrintID;
		//void(*printIDptr)(const Passport*) = VirtPrintID;
		void(*printIDptr)(void*) = VirtPrintID;
		void(*deleteptr)(void*) = VirtDelete;
		//void(*destrustptr)(void*) = VirtDestruct;
	};
public:
	TravelPack()
		/*: identity_doc1_(reinterpret_cast <IdentityDocument*> (new Passport()))
		, identity_doc2_(reinterpret_cast <IdentityDocument*> (new DrivingLicence()))*/
		: identity_document_(IdentityDocument(reinterpret_cast<IdentityDocument::VTable*>(vtable_)))
		, identity_doc1_((IdentityDocument*)(new Passport()))
		, identity_doc2_((IdentityDocument*)(new DrivingLicence()))
	{
		std::cout << "TravelPack::Ctor()"sv << std::endl;
	}

	TravelPack(const TravelPack& other)
		//: IdentityDocument(other)
		:identity_document_(other.identity_document_)
		, identity_doc1_(reinterpret_cast <IdentityDocument*> 
			(new Passport(*reinterpret_cast <Passport*>(other.identity_doc1_))))
		
		, identity_doc2_(reinterpret_cast <IdentityDocument*>
		(new Passport(*reinterpret_cast <Passport*>(other.identity_doc2_))))
		, additional_pass_(other.additional_pass_)
		, additional_dr_licence_(other.additional_dr_licence_)
	{
		std::cout << "TravelPack::CCtor()"sv << std::endl;
	}

	~TravelPack() {
		delete identity_doc1_;
		delete identity_doc2_;
		std::cout << "TravelPack::Dtor()"sv << std::endl;
	}

	void PrintID() /*const*/ {
		/*identity_doc1_->PrintID();
		identity_doc2_->PrintID();
		additional_pass_.PrintID();
		additional_dr_licence_.PrintID();*/
		vtable_->printIDptr(this);
	}
	
	void Delete() {
		//this-> ~Passport();
		vtable_->deleteptr(this);
	}
private:
	//таблица виртуальных функций
	static VTable pack_vtable_;
	VTable* vtable_ = &pack_vtable_;
	
	IdentityDocument identity_document_;
	IdentityDocument* identity_doc1_;
	IdentityDocument* identity_doc2_;
	Passport additional_pass_;
	DrivingLicence additional_dr_licence_;

	static void VirtPrintID(void* pack) {
		auto ptr = reinterpret_cast<TravelPack*>(pack);
		//std::cout << "Passport::PrintID() : "sv << ptr->identity_document_.GetID();
		//std::cout << " expiration date : "sv << ptr->expiration_date_.tm_mday << "/"sv
		ptr->identity_doc1_->PrintID();
		ptr->identity_doc2_->PrintID();
		ptr->additional_pass_.PrintID();
		ptr->additional_dr_licence_.PrintID();
		
	}

	/*static void VirtDestruct(void* passport) {
		auto ptr = reinterpret_cast<Passport*>(passport);
		std::cout << "Passport::Dtor()"sv << std::endl;

	}*/
	static void VirtDelete(void* pack) {
		auto ptr = reinterpret_cast<TravelPack*>(pack);
		ptr-> ~TravelPack();
	}

};

TravelPack::VTable TravelPack::pack_vtable_;