#pragma once
#include "common.h"

#include <cassert>

class Texture {
public:
	explicit Texture(Image image)
		: image_(std::move(image)) {
	}

	Size GetSize() const {
		return GetImageSize(image_);
	}

	char GetPixelColor(Point p) const {
		//assert(p.y >= 0 && p.x >= 0);
		return image_[p.y][p.x];
	}

private:
	Image image_;
};