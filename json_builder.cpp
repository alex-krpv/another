#include "json_builder.h"

namespace json {
	
	using namespace std::literals;

	//��� ����������� ������� ����� ��������� �������� ����� ��� ��������� ���� ���� - ��������.
	//��������� ����� ������ ����������� ������ �������� ��������������� ����� ����� �������� 
	//� ������� ������ Value ��� �������� ��� ����������� � ������� StartDict ��� StartArray.
		
	Builder& Builder::Key(std::string key) {
		//���� �� ����� ��� ������ ������ Key() ��� Key() ������ �� � �������� �������� �������
		//(������� �������� � key - ������� ����, ��� Key() ��� ������ �� �����, � Value() - ���)
		if (key_ || !std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			throw std::logic_error("key after key or not Dict construction"s);
		}
		key_.emplace(std::move(key));
		return *this;
	}

	Builder& Builder::StartDict() {
		//���� � ������� ���������� ����� - ��� ������ ������
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.back()->GetValue() = json::Dict{};
		}
		//���� �� ����� - ������� ��������� ����, ������ ��������� � nodes_stack_
		//(�� ��������� - ��� ������ �� ������ ��������� ��������������)

		return *this;
	}

	Builder& Builder::EndDict() {
		
		//���� ������������� ����������� Dict, �� OK
		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started Dict"); //������ ����� ��������� �������, ������� �� ���������
		}
		return *this;
	}

	
	Builder& Builder::StartArray() {
		
		//���� � ������� ���������� ����� - ��� ������ ������
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.back()->GetValue() = json::Array{};
			return *this;
		}
		//Builder result;
		inner_builders_.emplace_back(Builder{});
		inner_builders_.back().StartArray();
		nodes_stack_.emplace_back(inner_builders_.back().GetRootPtr());
		
		return inner_builders_.back();
	}

	Builder& Builder::EndArray() {
		//using namespace std::literals;
		//Node node;
		//static json::Array arr_node;
		//while (!node.IsArray()) {
		//	if (nodes_stack_.empty()) {
		//		throw std::logic_error("end of not started array"); //������ ����� ��������� �������, ������� �� ���������
		//						
		//	}
		//	node = Node{ *(nodes_stack_.back()) };
		//	arr_node.push_back(*(nodes_stack_.back()));
		//	nodes_stack_.pop_back();

		//}
		//nodes_stack_.push_back(&Node{ arr_node });
		//���� ������������� ����������� array, �� OK
		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started array"s); //������ ����� ��������� �������, ������� �� ���������
		}
		return *this;
	}


	//Builder& Builder::Value(Node::Value value) {
	//	if (nodes_stack_.empty()) {
	//		if (std::holds_alternative<double>(value)) {
	//			root_ = Node(std::get<double>(value));
	//		}
	//		else if (std::holds_alternative<int>(value)) {
	//			root_ = Node(std::get<int>(value));
	//		}
	//		else if (std::holds_alternative<bool>(value)) {
	//			root_ = Node(std::get<bool>(value));
	//		}
	//		else if (std::holds_alternative<std::string>(value)) {
	//			root_ = Node(std::get<std::string>(value));
	//		}
	//		else if (std::holds_alternative<json::Array>(value)) {
	//			root_ = Node(std::get<json::Array>(value));
	//		}
	//		else if (std::holds_alternative<json::Dict>(value)) {
	//			root_ = Node(std::get<json::Dict>(value));
	//		}
	//		else {
	//			root_ = Node{};
	//		}
	//		
	//	}
	//	return *this;
	//	//nodes_stack_.push_back(value);

	//	/*if (nodes_stack_.back()->IsArray()) {
	//		
	//	}*/

	//	/*else { 
	//		if (nodes_stack_.back()->IsArray()) {
	//			json::Array arr;
	//			root_ = Node{arr};
	//			root_.AsArray().
	//		}
	//		
	//	}*/
	//	return *this;
	//}

	Builder& Builder::Value(Node::Value value) {
		
		//� Value �� ����� ���� �������� Array ��� Dict, �.�.
		//��� �������� ���� ����� ����������� ������ StartArray � StartDict
		
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
			//���� � key_ �������� ��������, ������ ����� ��������� ���� � �������
			std::get<Dict>(nodes_stack_.back()->GetValue()).insert({ *key_, value_node });
			//����� �������� ��� ������������ ��������� ������
			key_ = std::nullopt;
		}


		return *this;
		//if (nodes_stack_.empty()) {
		//if (std::holds_alternative<double>(value)) {
		//	value_node = Node(std::get<double>(value));
		//}
		//else if (std::holds_alternative<int>(value)) {
		//	value_node = Node(std::get<int>(value));
		//}
		//else if (std::holds_alternative<bool>(value)) {
		//	value_node = Node(std::get<bool>(value));
		//}
		//else if (std::holds_alternative<std::string>(value)) {
		//	value_node = Node(std::get<std::string>(value));
		//}
		//else if (std::holds_alternative<json::Array>(value)) {
		//	value_node = Node(std::get<json::Array>(value));
		//}
		//else if (std::holds_alternative<json::Dict>(value)) {
		//	value_node = Node(std::get<json::Dict>(value));
		//}
		//else {
		//	value_node = Node{};
		//}

		////}
		//nodes_stack_.push_back(&value_node);
		//return *this;
		//nodes_stack_.push_back(value);

		/*if (nodes_stack_.back()->IsArray()) {

		}*/

		/*else {
			if (nodes_stack_.back()->IsArray()) {
				json::Array arr;
				root_ = Node{arr};
				root_.AsArray().
			}

		}*/
		//return *this;
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


	/*Builder::~Builder()
	{
	}*/
	Builder Builder::MakeInnerArray() {
		Builder result;
		result.StartArray();
		return result;
	}
	
	Node* Builder::GetRootPtr() {
		return &root_;
	}
}//namespace json
