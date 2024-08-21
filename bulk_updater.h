#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>

//struct BulkMoneyAdder {
//    double delta = {};
//};

struct BulkMoneyAdder {
	DayState delta = {};
};

//struct BulkTaxApplier {
//    double ComputeFactor() const {
//        static const double factor = 0.87;
//        return std::pow(factor, count);
//    }
//
//    int count = 0;
//};

//struct BulkTaxApplier {
//	double ComputeFactor() const {
//		//static const double factor = 0.87;
//		double factor = (100.0 - percent) / 100.0;
//		return std::pow(factor, count);
//	}
//
//	int count = 0;
//	int percent = 13;
//};

struct BulkTaxApplier {
	double factor = 1;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax) {
    }

    /*void CombineWith(const BulkLinearUpdater& other) {
        tax_.count += other.tax_.count;
        add_.delta = add_.delta * other.tax_.ComputeFactor() + other.add_.delta;
    }*/

	void CombineWith(const BulkLinearUpdater& other) {
		tax_.factor *= other.tax_.factor;
		add_.delta.earned *= other.tax_.factor;
		add_.delta += other.add_.delta;
	}

    /*double Collapse(double origin, IndexSegment segment) const {
        return origin * tax_.ComputeFactor() + add_.delta * static_cast<double>(segment.length());
    }*/
	DayState Collapse(DayState origin, IndexSegment segment) const {
		return { origin.earned * tax_.factor + add_.delta.earned * static_cast<double>(segment.length()),
			origin.spent + add_.delta.spent * static_cast<double>(segment.length()) };
	}

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
};
