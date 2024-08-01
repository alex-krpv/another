#include "json_builder.h"
using namespace std::literals;

namespace json {
		
	Builder::Builder()
		: root_()
		, nodes_stack_{ &root_ }
	{}
	//��� ����������� ������� ����� ��������� �������� ����� ��� ��������� ���� ���� - ��������.
	//��������� ����� ������ ����������� ������ �������� ��������������� ����� ����� �������� 
	//� ������� ������ Value ��� �������� ��� ����������� � ������� StartDict ��� StartArray.
		
	Builder& Builder::Key(std::string key) {
		//���� � ������� ���������� ����� - ��� ����� ������, ����� ����� Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("key after value"s);
		}
		//���� �� ����� ��� ������ ������ Key() ��� Key() ������ �� � �������� �������� �������
		//(������� �������� � key - ������� ����, ��� Key() ��� ������ �� �����, � Value() - ���)
		if (key_ || !std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			throw std::logic_error("Key after Key or Key in not Dict construction"s);
		}
		key_.emplace(std::move(key));
		return *this;
	}

	Builder& Builder::StartDict() {
		
		//���� � ������� ���������� ����� - ��� ����� ������, ����� ����� Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Dict after value"s);
		}
		//���� � ������� ���������� ������ variant - ��� ������ ������
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.back()->GetValue() = json::Dict{};
			return *this;
		}

		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			if (!key_) {
				throw std::logic_error("starting Dict in Dict without key"s);
			}
			//��������� � root_ ���� {����: ������ �������}
			std::get<Dict>(nodes_stack_.back()->GetValue()).insert({ *key_, json::Dict{}});

			//json::Dict temp = std::get<Dict>(nodes_stack_.back()->GetValue());
			//�� ������� ����� ������ ��������� �� ������ �������
			nodes_stack_.push_back(&(std::get<Dict>(nodes_stack_.back()->GetValue())[*key_]));
			//�������� ���� � ����� ����������� ������ ������ �������
			key_ = std::nullopt;
			return *this;
		}

		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(json::Dict{});
			nodes_stack_.push_back(&(std::get<json::Array>(nodes_stack_.back()->GetValue()).back()));
		}

		
		return *this;
	}

	Builder& Builder::EndDict() {
		//���� � ������� ���������� ����� - ��� ����� ������, ����� ����� Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Array after value"s);
		}
		//���� ������������� ����������� Dict, �� OK
		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started Dict"s); //������ ����� ��������� �������, ������� �� ���������
		}
		return *this;
	}

	
	Builder& Builder::StartArray() {
		
		//���� � ������� ���������� ����� - ��� ����� ������, ����� ����� Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Array after value"s);
		}
		//���� � ������� ���������� ������ variant - ��� ������ ������
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.back()->GetValue() = json::Array{};
			return *this;			
		}

		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(json::Array{});
			nodes_stack_.push_back(&(std::get<json::Array>(nodes_stack_.back()->GetValue()).back()));
			return *this;			
		}

		
		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			if (!key_) {
				throw std::logic_error("starting Array in Dict without key"s);
			}
			std::get<Dict>(nodes_stack_.back()->GetValue()).insert({ *key_, json::Array{} });
			
			json::Dict temp = std::get<Dict>(nodes_stack_.back()->GetValue());
			nodes_stack_.push_back(&(std::get<Dict>(nodes_stack_.back()->GetValue())[*key_]));
			key_ = std::nullopt;
		}
		return *this;

		//Builder result;
		/*inner_builders_.emplace_back(Builder{});
		inner_builders_.back().StartArray();
		nodes_stack_.emplace_back(inner_builders_.back().GetRootPtr());*/
		
		//return inner_builders_.back();
	}

	Builder& Builder::EndArray() {
		//���� � ������� ���������� ����� - ��� ����� ������, ����� ����� Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Array after value"s);
		}
		//���� ������������� ����������� array, �� OK
		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started array"s); //������ ����� ��������� �������, ������� �� ���������
		}
		return *this;
	}
	   	
	Builder& Builder::Value(Node::Value value) {
		
		//� Value �� ����� ���� �������� Array ��� Dict, �.�.
		//��� �������� ���� ����� ����������� ������ StartArray � StartDict
		if (nodes_stack_.empty()) {
			throw std::logic_error("value after value out of Array"s);
		}
		//�� ���� ������� �� �������, �� ������� - ������, ������� ��������
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			
			nodes_stack_.back()->GetValue() = value;
			nodes_stack_.pop_back();
			return *this;
		}

		Node value_node;
		value_node.GetValue() = value;

		//���� � nodes_stack_.back() �������� Array, ��������� ���� ��������
		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(value_node);
		}

		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			if (!key_) {
				if (!key_) {
					throw std::logic_error("making Dict value without key"s);
				}
			}
			//���� � key_ �������� ��������, ������ ����� ��������� ���� � �������
			std::get<Dict>(nodes_stack_.back()->GetValue()).insert({ *key_, value_node });
			//����� �������� ��� ������������ ��������� ������
			key_ = std::nullopt;
		}
		
		return *this;
	}


	json::Node Builder::Build() {
		//������� ������ ��� ������������� �������� ��� �������� ��� ����� ����� ������������ 
		//(����� root_ �� ���������)
		if (!nodes_stack_.empty()) {
			//throw std::logic_error("have not ended array or dict");
			throw std::logic_error("node not finished");
		}
			
		return root_;
	}
	   	
	Node* Builder::GetRootPtr() {
		return &root_;
	}
}//namespace json
