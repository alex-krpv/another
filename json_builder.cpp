#include "json_builder.h"

namespace json {
	

	/*Builder::Builder()
	{
	}*/

	Builder& Builder::Key(std::string key) {
		return *this;
	}
	Builder& Builder::StartArray() {
		static json::Array arr;
		nodes_stack_.emplace_back(arr);
			return *this;
	}
	Builder& Builder::Value(Node::Value value) {
		if (nodes_stack_.empty()) {
			if (std::holds_alternative<double>(value)) {
				root_ = Node(std::get<double>(value));
			}
			else if (std::holds_alternative<int>(value)) {
				root_ = Node(std::get<int>(value));
			}
			else if (std::holds_alternative<bool>(value)) {
				root_ = Node(std::get<bool>(value));
			}
			else if (std::holds_alternative<std::string>(value)) {
				root_ = Node(std::get<std::string>(value));
			}
			else if (std::holds_alternative<json::Array>(value)) {
				root_ = Node(std::get<json::Array>(value));
			}
			else if (std::holds_alternative<json::Dict>(value)) {
				root_ = Node(std::get<json::Dict>(value));
			}
			else {
				root_ = Node{};
			}
			
		}
		else { 
			if (nodes_stack_.back()->IsArray()) {
				json::Array arr;
				root_ = Node{arr};
				root_.AsArray().
			}
			
		}
		return *this;
	}


	json::Node Builder::Build() {
		return root_;
	}


	/*Builder::~Builder()
	{
	}*/

}//namespace json
