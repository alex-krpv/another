#pragma once
struct DayState {
public:
	// code
	double ComputeIncome() {
		return earned - spent;
	}
public:
	double earned = 0.;
	double spent = 0.;
};
inline DayState operator+(const DayState& left, const DayState& right) {
	return DayState{ left.earned + right.earned, left.spent + right.spent };
}

inline DayState& operator+=(DayState& left, const DayState& right) {
	left = left + right;
	return left;
}