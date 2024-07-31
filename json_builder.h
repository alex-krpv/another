#include "json.h"

#include <optional>
#include <vector>

namespace json {
	
	class Builder
	{
	public:
		Builder() {
			nodes_stack_.push_back(&root_);
		};
		
		//��� ����������� ������� ����� ��������� �������� ����� ��� ��������� ���� ���� - ��������.
		//��������� ����� ������ ����������� ������ �������� ��������������� ����� ����� �������� 
		//� ������� ������ Value ��� �������� ��� ����������� � ������� StartDict ��� StartArray.
		Builder& Key(std::string);

		//����� ��������, ��������������� ����� ��� ����������� �������, ��������� ������� ������� ���, 
		//���� ������� ����� ����� ������������ json::Builder, �� ���������� ��������������� JSON - �������.
		//����� ��������� ��� ������� ������ � ����� ��� ������ � ��� � ����� ������ ��� �������.
		//����� Node::Value � ��� ������� ��� �������� ������ Node, ������� variant � ������� ��������� ����� - ��������.
		//�������� ��������� ����.
		Builder& Value(Node::Value);

		//�������� ����������� �������� �������� - �������.���������� � ��� �� ����������, ��� � Value.
		//��������� ������� ����������� ������ ���� Key ��� EndDict.
		Builder& StartDict();

		
		//�������� ����������� �������� �������� - �������.
		//���������� � ��� �� ����������, ��� � Value.
		//��������� ������� ����������� ������ ���� EndArray ��� �����, 
		//�������� ����� �������� : Value, StartDict ��� StartArray.
		Builder& StartArray();

		//��������� ����������� �������� �������� - �������.
		//��������� ������������� ������� Start* ������ ���� StartDict.
		Builder& EndDict();
		 
		//��������� ����������� �������� �������� - �������.
		//��������� ������������� ������� Start* ������ ���� StartArray.
		Builder& EndArray();
	
		//���������� ������ json::Node, ���������� JSON, ��������� ����������� �������� �������.
		//� ����� ������� ��� ������� Start* ������ ���� ������ ��������������� End*.
		//��� ���� ��� ������ ������ ���� ��������, �� ���� ����� json::Builder{}.Build() ����������.
		json::Node Build();

		//~Builder();

	private:
		//�������������� ������
		//std::optional<Node> root_;
		Node root_;
		//���� ���������� �� �� ������� JSON, ������� ��� �� ��������� : 
		//�� ���� ������� ����������� �������� � ������� ��� ���������.
		std::vector<Node*> nodes_stack_;
		//�������� ����� ��� �������
		std::optional<std::string> key_;
		//������ ���������� ��������
		std::vector<Builder> inner_builders_;

		Builder MakeInnerArray();
		Node* GetRootPtr();
	};

	/*Builder::Builder()
	{
	}

	Builder::~Builder()
	{
	}*/

}//namespace json

//Node Builder::Build()
//{
//	if (!nodes_stack_.empty()) {
//		throw std::logic_error("not end all arrays or dict");//�������� �������� �������� � ��������
//	}
//	if (!root_.has_value()) {
//		throw std::logic_error("func call after ctor");//�������� ������ ����� ����� ������������
//	}
//	value_start_checker = true;
//	return *root_;
//}

//������: ���������� ����� ��� ��������� ��������� �� Node ���� �� �� ����� ������ �������������� ������, �� ��� ������ "As" �������� const ?
//�����: ��� ����, ����� �������� �������� ���� �������� � ����� Json �� ����������� ������ GetValue().



//BaseContext ������ �������� �����.����� � Builder(������ �� ������� �� ������).
//�������� ������ -
//DictItemContext Value(Node::Value value) { return BaseContext::Value(std::move(value)); }


//����� ������� �������� ������ ������ � �� ������ - � ������ ����� ���������� ����������.�������� - ��� �� ������� ������

//#include <iostream>
//#include <map>
//#include <set>
//#include <string>
//
//class Json { };
//
//class Builder {
//	class BaseContext;
//	class DictValueContext;
//	class ArrayItemContext;
//public:
//	Builder() = default;
//	DictValueContext StartDict();
//	ArrayItemContext StartArray();
//	BaseContext EndDict();
//	BaseContext EndArray();
//
//private:
//	Json* json;
//	class BaseContext {
//	public:
//		BaseContext(Builder& builder)
//			: builder_(builder)
//		{
//		}
//		DictValueContext StartDict() {
//			return builder_.StartDict();
//		}
//		ArrayItemContext StartArray() {
//			return builder_.StartArray();
//		}
//		BaseContext EndDict() {
//			return builder_.EndDict();
//		}
//		BaseContext EndArray() {
//			return builder_.EndArray();
//		}
//
//	private:
//		Builder& builder_;
//	};
//
//	class DictValueContext : public BaseContext {
//	public:
//		DictValueContext(BaseContext base)
//			: BaseContext(base)
//		{
//		}
//		BaseContext EndArray() = delete;
//		DictValueContext StartDict() = delete;
//		ArrayItemContext StartArray() = delete;
//	};
//
//	class ArrayItemContext : public BaseContext {
//	public:
//		ArrayItemContext(BaseContext base) : BaseContext(base) {}
//		BaseContext EndDict() = delete;
//	};
//};
//
//Builder::DictValueContext Builder::StartDict() {
//	return BaseContext{ *this };
//}
//
//Builder::ArrayItemContext Builder::StartArray() {
//	return BaseContext{ *this };
//}
//
//Builder::BaseContext Builder::EndDict() {
//	return *this;
//}
//
//Builder::BaseContext Builder::EndArray() {
//	return *this;
//}
//
//int main() {
//	auto doc = Builder().StartArray().EndDict();
//}