#pragma once

#include <array>
#include <stdexcept>
//#include <string>

template <typename T, size_t N>
class StackVector {
public:
	//����������� ��������� ������ ������� � �� �������� �� ����������� ��������.
	//���� �������� ������������ ������ ����������� �������, ����������� ������ ����������� ���������� invalid_argument.
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
	////������ begin / end ������ ���������� ��������� �� ������ � ������� ����� �������.
	////�� ��� �� ������, �������� ��� ��������������.	
	//using Iterator = list<char>::iterator;
	//std::array<T>::iterator
	//auto begin() {
	using const_It = typename std::array<T, N>::const_iterator;
	using It = typename std::array<T, N>::iterator;

	It begin() {
		return array_.begin();
	}

	It end() {
		//��� �� ������ (����� ��������� �� ���������� �������, � ����� �� size_):
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
	
	//����� PushBack ��������� ����� ������� � ����� �������.
	//���� ������� ������ ������� ����� ��� �����������, ����� PushBack ������ ����������� ����������� ���������� overflow_error.
	void PushBack(const T& value) {
		if (size_ == N) {
			throw std::overflow_error("");
		}
		//���������� ��������
		array_[size_] = value;
		++size_;
	}
	
	//����� PopBack ��������� ������ ������� �� ���� � ���������� ������� �������, ������� ��� ���������.
	//���� ������ ����, ����� PopBack ������ ����������� ����������� ���������� underflow_error.    
	T PopBack() {
		if (size_ == 0) {
			throw std::underflow_error("");
		}
		//���������� ���������� ��������
		//auto result = array_.back();
		auto result = array_[size_-1];
		//�������� (�� ����� ���� ���������� �������, �������� end() ������������ �� size_)
		--size_;
		return result;

	}

private:
	
	//N - ����������� ��������� ���������� ��������� � ������� (�����������)
	std::array<T, N> array_;
	//������� ���������� ��������� � ������� (������)
	size_t size_;
};





