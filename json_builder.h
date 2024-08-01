#pragma once

#include <string>
#include <vector>
#include "json.h"

namespace json {

class Builder {
	class BaseContext;
	class DictKeyContext;
	class DictItemContext;
	class ArrayItemContext;
	//friend class DictItemContext;
	//friend class BaseContext;
public:
    Builder();
    Node Build();
	DictKeyContext Key(std::string key);
	BaseContext Value(Node::Value value);
	DictItemContext StartDict();
	ArrayItemContext StartArray();
	BaseContext EndDict();
	BaseContext EndArray();

private:
    Node root_;
    std::vector<Node*> nodes_stack_;

    Node::Value& GetCurrentValue();
    const Node::Value& GetCurrentValue() const;
    
    void AssertNewObjectContext() const;
    void AddObject(Node::Value value, bool one_shot);
	

	class BaseContext {
	public:
		BaseContext(Builder& builder)
			: builder_(builder) {}
		Node Build() {
			return builder_.Build();
		}
		DictKeyContext Key(std::string key) {
			return builder_.Key(key);
		}
		BaseContext Value(Node::Value value) { //нужно переопределить в ряде классов
			return builder_.Value(value);
		}
		DictItemContext StartDict() {
			return builder_.StartDict();
		}
		ArrayItemContext StartArray() {
			return builder_.StartArray();
		}
		BaseContext EndDict() {
			return builder_.EndDict();
		}
		BaseContext EndArray() {
			return builder_.EndArray();
		}
		Builder& GetBuilder() {
			return builder_;
		}
	private:
		Builder& builder_;
	};

	class DictItemContext : public BaseContext {
	public:
		DictItemContext(BaseContext base)
			:BaseContext(base) {}
		//DictItemContext Value(Node::Value value); //переопределяется метод 
		//DictKeyContext Key(std::string key); //поддерживается

		Node Build() = delete;
		DictItemContext StartDict() = delete;
		ArrayItemContext StartArray() = delete;
		BaseContext Value(Node::Value value) = delete;
		//BaseContext EndDict();
		BaseContext EndArray() = delete;

	};

	class DictKeyContext : public BaseContext {
	public:
		DictKeyContext(BaseContext base)
			:BaseContext(base) {}
		DictItemContext Value(Node::Value value); //переопределяется метод 
		//DictItemContext StartDict(); //поддерживается
		//ArrayItemContext StartArray(); //поддерживается

		Node Build() = delete;
		DictKeyContext Key(std::string key) = delete;
		BaseContext EndDict() = delete;
		BaseContext EndArray() = delete;
	};


	class ArrayItemContext : public BaseContext {
	public:
		ArrayItemContext(BaseContext base)
			:BaseContext(base) {}
		ArrayItemContext Value(Node::Value value); //переопределяется метод 
		//BaseContext EndArray(); //Поддерживается
		//DictItemContext StartDict(); //поддерживается
		//ArrayItemContext StartArray(); //поддерживается

		Node Build() = delete;
		DictKeyContext Key(std::string key) = delete;
		BaseContext EndDict() = delete;
	};

};



//class ValueContext : public BaseContext {
//public:
//	ValueContext(BaseContext base)
//		:BaseContext(base) {}
//};


//class DictValueContext : public BaseContext {
//public:
//	DictValueContext(BaseContext base)
//		:BaseContext(base) {}
//	Node Build() = delete;
//
//	DictItemContext Value(Node::Value value);
//	//DictItemContext StartDict() = delete;
//	// StartArray() = delete;
//	// EndDict() = delete;
//	// EndArray() = delete;
//};

}  // namespace json
