#pragma once

#include <array>
#include <stdexcept>
//#include <string>

template <typename T, size_t N>
class StackVector {
public:
	// онструктор принимает размер вектора Ч по аналогии со стандартным вектором.
	//≈сли аргумент конструктора больше вместимости вектора, конструктор должен выбрасывать исключение invalid_argument.
	//using namespace std;
	explicit StackVector(size_t a_size = 0) {
		if (a_size > N) {
			throw std::invalid_argument("");
		}
		size_ = a_size;
	}

	T& operator[](size_t index) {
		return array_[index];
	}
	const T& operator[](size_t index) const {
		return array_[index];
	}
	////ћетоды begin / end должны возвращать итераторы на начало и текущий конец вектора.
	////»х тип не указан, выберите его самосто€тельно.	
	//using Iterator = list<char>::iterator;
	//std::array<T>::iterator
	//auto begin() {
	using const_It = typename std::array<T, N>::const_iterator;
	using It = typename std::array<T, N>::iterator;

	It begin() {
		return array_.begin();
	}

	It end() {
		//так не пойдет (будет указывать на предельную емкость, а нужно на size_):
		//return array_.end();
		return next(array_.begin(),size_);
	}

	const_It begin() const {
		return array_.begin();
	}

	const_It end() const {
		//return array_.end();
		return next(array_.begin(), size_);
	}


	size_t Size() const {
		return size_;
	}
	size_t Capacity() const {
		return N;
	}
	
	//ћетод PushBack добавл€ет новый элемент в конец вектора.
	//≈сли текущий размер вектора равен его вместимости, метод PushBack должен выбрасывать стандартное исключение overflow_error.
	void PushBack(const T& value) {
		if (size_ == N) {
			throw std::overflow_error("");
		}
		//добавление элемента
		array_[size_] = value;
		++size_;
	}
	
	//ћетод PopBack уменьшает размер вектора на один и возвращает элемент вектора, который был последним.
	//≈сли вектор пуст, метод PopBack должен выбрасывать стандартное исключение underflow_error.    
	T PopBack() {
		if (size_ == 0) {
			throw std::underflow_error("");
		}
		//сохранение последнего элемента
		//auto result = array_.back();
		auto result = array_[size_-1];
		//удаление (на самом деле уменьшение размера, итератор end() определ€етс€ по size_)
		--size_;
		return result;

	}

private:
	
	//N - максимально вожможное количество элементов в векторе (вместимость)
	std::array<T, N> array_;
	//текущее количество элементов в векторе (размер)
	size_t size_;
};





