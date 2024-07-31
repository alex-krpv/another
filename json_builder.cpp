#include "json_builder.h"

namespace json {
	
	using namespace std::literals;

	//При определении словаря задаёт строковое значение ключа для очередной пары ключ - значение.
	//Следующий вызов метода обязательно должен задавать соответствующее этому ключу значение 
	//с помощью метода Value или начинать его определение с помощью StartDict или StartArray.
		
	Builder& Builder::Key(std::string key) {
		//если до этого был вызван другой Key() или Key() вызван не в процессе создания словаря
		//(наличие значения в key - признак того, что Key() был вызван до этого, а Value() - нет)
		if (key_ || !std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			throw std::logic_error("key after key or not Dict construction"s);
		}
		key_.emplace(std::move(key));
		return *this;
	}

	Builder& Builder::StartDict() {
		//если в векторе указателей пусто - это начало работы
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.back()->GetValue() = json::Dict{};
		}
		//если не пусто - создаем локальную ноду, кладем указатель в nodes_stack_
		//(не сработает - при выходе из метода указатель инвалидируется)

		return *this;
	}

	Builder& Builder::EndDict() {
		
		//если действительно формируется Dict, то OK
		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started Dict"); //вызван метод окончания словаря, который не начинался
		}
		return *this;
	}

	
	Builder& Builder::StartArray() {
		
		//если в векторе указателей пусто - это начало работы
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
		//		throw std::logic_error("end of not started array"); //вызван метод окончания вектора, который не начинался
		//						
		//	}
		//	node = Node{ *(nodes_stack_.back()) };
		//	arr_node.push_back(*(nodes_stack_.back()));
		//	nodes_stack_.pop_back();

		//}
		//nodes_stack_.push_back(&Node{ arr_node });
		//если действительно формируется array, то OK
		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started array"s); //вызван метод окончания вектора, который не начинался
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
		
		//В Value не может быть передано Array или Dict, т.к.
		//для создания этих типов применяются методы StartArray и StartDict
		
		//не было создано ни словаря, ни массива - значит, простое значение
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.back()->GetValue() = value;
			nodes_stack_.pop_back();
			return *this;
		}

		Node value_node;
		value_node.GetValue() = value;

		//Если в nodes_stack_.back() хранится Array, добавляем туда значение
		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(value_node);
		}

		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			//если в key_ хранится значение, значит можно загружать пару в словарь
			std::get<Dict>(nodes_stack_.back()->GetValue()).insert({ *key_, value_node });
			//сброс значения для последующего выявления ошибок
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
		//попытка вызова при незаконченных словарях или массивах или сразу после конструктора 
		//(когда root_ не закончена)
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
