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
		
		//При определении словаря задаёт строковое значение ключа для очередной пары ключ - значение.
		//Следующий вызов метода обязательно должен задавать соответствующее этому ключу значение 
		//с помощью метода Value или начинать его определение с помощью StartDict или StartArray.
		Builder& Key(std::string);

		//Задаёт значение, соответствующее ключу при определении словаря, очередной элемент массива или, 
		//если вызвать сразу после конструктора json::Builder, всё содержимое конструируемого JSON - объекта.
		//Может принимать как простой объект — число или строку — так и целый массив или словарь.
		//Здесь Node::Value — это синоним для базового класса Node, шаблона variant с набором возможных типов - значений.
		//Смотрите заготовку кода.
		Builder& Value(Node::Value);

		//Начинает определение сложного значения - словаря.Вызывается в тех же контекстах, что и Value.
		//Следующим вызовом обязательно должен быть Key или EndDict.
		Builder& StartDict();

		
		//Начинает определение сложного значения - массива.
		//Вызывается в тех же контекстах, что и Value.
		//Следующим вызовом обязательно должен быть EndArray или любой, 
		//задающий новое значение : Value, StartDict или StartArray.
		Builder& StartArray();

		//Завершает определение сложного значения - словаря.
		//Последним незавершённым вызовом Start* должен быть StartDict.
		Builder& EndDict();
		 
		//Завершает определение сложного значения - массива.
		//Последним незавершённым вызовом Start* должен быть StartArray.
		Builder& EndArray();
	
		//Возвращает объект json::Node, содержащий JSON, описанный предыдущими вызовами методов.
		//К этому моменту для каждого Start* должен быть вызван соответствующий End*.
		//При этом сам объект должен быть определён, то есть вызов json::Builder{}.Build() недопустим.
		json::Node Build();

		//~Builder();

	private:
		//конструируемый объект
		//std::optional<Node> root_;
		Node root_;
		//стек указателей на те вершины JSON, которые ещё не построены : 
		//то есть текущее описываемое значение и цепочка его родителей.
		std::vector<Node*> nodes_stack_;
		//хранение ключа для словаря
		std::optional<std::string> key_;
		//вектор внутренних билдеров
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
//		throw std::logic_error("not end all arrays or dict");//проверка закрытия словарей и массивов
//	}
//	if (!root_.has_value()) {
//		throw std::logic_error("func call after ctor");//проверка вызова сразу после конструктора
//	}
//	value_start_checker = true;
//	return *root_;
//}

//Вопрос: Подскажите зачем нам сохранять указатели на Node если мы не можем менять сформированные классы, тк все методы "As" помечены const ?
//ответ: Для того, чтобы изменить значение ноды добавьте в класс Json не константную версию GetValue().



//BaseContext просто вызывает соотв.метод у Builder(ссылку на который он хранит).
//Дочерние классы -
//DictItemContext Value(Node::Value value) { return BaseContext::Value(std::move(value)); }


//Нужно сначала объявить нужные классы и их методы - а только потом реализацию определять.Например - вот на подобии такого

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