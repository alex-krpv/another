#include "json_builder.h"
using namespace std::literals;

namespace json {
		
	Builder::Builder()
		: root_()
		, nodes_stack_{ &root_ }
	{}
	//При определении словаря задаёт строковое значение ключа для очередной пары ключ - значение.
	//Следующий вызов метода обязательно должен задавать соответствующее этому ключу значение 
	//с помощью метода Value или начинать его определение с помощью StartDict или StartArray.
		
	Builder& Builder::Key(std::string key) {
		//если в векторе указателей пусто - это конец работы, нужен вызов Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("key after value"s);
		}
		//если до этого был вызван другой Key() или Key() вызван не в процессе создания словаря
		//(наличие значения в key - признак того, что Key() был вызван до этого, а Value() - нет)
		if (key_ || !std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			throw std::logic_error("Key after Key or Key in not Dict construction"s);
		}
		key_.emplace(std::move(key));
		return *this;
	}

	Builder& Builder::StartDict() {
		
		//если в векторе указателей пусто - это конец работы, нужен вызов Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Dict after value"s);
		}
		//если в векторе указателей пустой variant - это начало работы
		if (std::holds_alternative<nullptr_t>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.back()->GetValue() = json::Dict{};
			return *this;
		}

		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			if (!key_) {
				throw std::logic_error("starting Dict in Dict without key"s);
			}
			//добавляем в root_ пару {ключ: пустой словарь}
			std::get<Dict>(nodes_stack_.back()->GetValue()).insert({ *key_, json::Dict{}});

			//json::Dict temp = std::get<Dict>(nodes_stack_.back()->GetValue());
			//на вершину стека кладем указатель на пустой словарь
			nodes_stack_.push_back(&(std::get<Dict>(nodes_stack_.back()->GetValue())[*key_]));
			//обнуляем ключ в целях обнаружения ошибок вызова методов
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
		//если в векторе указателей пусто - это конец работы, нужен вызов Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Array after value"s);
		}
		//если действительно формируется Dict, то OK
		if (std::holds_alternative<json::Dict>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started Dict"s); //вызван метод окончания словаря, который не начинался
		}
		return *this;
	}

	
	Builder& Builder::StartArray() {
		
		//если в векторе указателей пусто - это конец работы, нужен вызов Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Array after value"s);
		}
		//если в векторе указателей пустой variant - это начало работы
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
		//если в векторе указателей пусто - это конец работы, нужен вызов Build()
		if (nodes_stack_.empty()) {
			throw std::logic_error("Array after value"s);
		}
		//если действительно формируется array, то OK
		if (std::holds_alternative<json::Array>(nodes_stack_.back()->GetValue())) {
			nodes_stack_.pop_back();
		}
		else {
			throw std::logic_error("end of not started array"s); //вызван метод окончания вектора, который не начинался
		}
		return *this;
	}
	   	
	Builder& Builder::Value(Node::Value value) {
		
		//В Value не может быть передано Array или Dict, т.к.
		//для создания этих типов применяются методы StartArray и StartDict
		if (nodes_stack_.empty()) {
			throw std::logic_error("value after value out of Array"s);
		}
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
			if (!key_) {
				if (!key_) {
					throw std::logic_error("making Dict value without key"s);
				}
			}
			//если в key_ хранится значение, значит можно загружать пару в словарь
			std::get<Dict>(nodes_stack_.back()->GetValue()).insert({ *key_, value_node });
			//сброс значения для последующего выявления ошибок
			key_ = std::nullopt;
		}
		
		return *this;
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
	   	
	Node* Builder::GetRootPtr() {
		return &root_;
	}
}//namespace json
