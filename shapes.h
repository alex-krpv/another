#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
	// Фигура после создания имеет нулевые координаты и размер,
	// а также не имеет текстуры
	explicit Shape(ShapeType type):type_(std::move(type)) {
		
	}

	void SetPosition(Point pos) {
		//(void)pos;
		pos_ = std::move(pos);
	}

	void SetSize(Size size) {
		//(void)size;
		size_ = std::move(size);
	}

	void SetTexture(std::shared_ptr<Texture> texture) {
		//(void)texture;
		texture_ptr_ = texture;
	}

	// Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
	// Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
	// должны отображаться с помощью символа точка '.'
	// Части фигуры, выходящие за границы объекта image, должны отбрасываться.
	void Draw(Image& image) const {
		//(void)image;
		Size image_size = GetImageSize(image);
		
		//если фигура начинается за пределами изображения - выход
		if (pos_.x >= image_size.width || pos_.y >= image_size.height) {
			return;
		}
		//если фигура заканчивается до начала изображения 
		//(возможно при отрицательных координатах начала фигуры)
		
		if (size_.width + pos_.x <= 0 || size_.height + pos_.y <= 0) {
			return;
		}


		//определение наличия текстуры
		bool is_texture = texture_ptr_? true : false;
		switch (type_)
		{
		case ShapeType::RECTANGLE:
			
			//перемещаемся по двумерному вектору фигуры и переносим пиксели текстуры на изображение
			
			//перемещение по вертикали фигуры
			for (int i = 0; i < size_.height; i++)
			{
				//проверка выхода за границы изображения по высоте
				//(все, что выходит - не рисуем)
				if (pos_.y + i >= image_size.height) {
					break;
				}
				//если координата фигуры отрицательная, то нужно ждать, пока не перекроет изображение
				if (pos_.y + i < 0) {
					continue;
				}
				
				//перемещение по горизонтали
				for (int j = 0; j < size_.width; j++)
				{
					//проверка выхода за границы изображения по ширине
					//(все, что выходит - не рисуем)
					if (pos_.x + j >= image_size.width) {
						break;
					}
					if (pos_.x + j < 0) {
						continue;
					}
					if (is_texture) {
						//проверка выхода за границы текстуры
						//(все, что снаружи - рисуем '.')
						if ((/*pos_.y +*/ i >= texture_ptr_->GetSize().height) || (/*pos_.x +*/ j >= texture_ptr_->GetSize().width)) {
							image[pos_.y + i][pos_.x + j] = '.';
						}
						else {
							image[pos_.y + i][pos_.x + j] = texture_ptr_->GetPixelColor({ j, i });
						}
					}
					else {
						image[pos_.y + i][pos_.x + j] = '.';
					}
					
					//image[pos_.y + i][pos_.x + j] = is_texture ? texture_ptr_->GetPixelColor({ j, i }): '.';
				}
			}
			
			break;
		
		case ShapeType::ELLIPSE:
			
			//перемещение по вертикали
			for (int i = 0; i < size_.height; i++)
			{
				//проверка выхода за границы изображения по высоте
				//(все, что выходит - не рисуем)
				if (pos_.y + i >= image_size.height) {
					break;
				}
				if (pos_.y + i < 0) {
					continue;
				}

				//перемещение по горизонтали
				for (int j = 0; j < size_.width; j++)
				{
					//проверка выхода за границы изображения по ширине
					//(все, что выходит - не рисуем)
					if (pos_.x + j >= image_size.width) {
						break;
					}
					if (pos_.x + j < 0) {
						continue;
					}
					//проверка того, что точка внутри эллипса
					if (IsPointInEllipse({j,i}, size_)) {
						if (is_texture) {
							//проверка выхода за границы текстуры
							//(все, что снаружи - рисуем '.')
							if ((/*pos_.y + */i >= texture_ptr_->GetSize().height) || (/*pos_.x + */j >= texture_ptr_->GetSize().width)) {
								image[pos_.y + i][pos_.x + j] = '.';
							}
							else {
								image[pos_.y + i][pos_.x + j] = texture_ptr_->GetPixelColor({ j, i });
							}
						}
						else {
							image[pos_.y + i][pos_.x + j] = '.';
						}
					}
				}
			}
			break;
		}
	}

private:
	ShapeType type_;
	//размер фигуры
	Size size_ = { 0, 0 };
	//координаты верхнего левого угла
	Point pos_ = { 0, 0 };
	std::shared_ptr<Texture> texture_ptr_;
};