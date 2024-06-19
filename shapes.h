#pragma once
#include "texture.h"

#include <memory>

// �������������� ���� �����: ������������� � ������
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
	// ������ ����� �������� ����� ������� ���������� � ������,
	// � ����� �� ����� ��������
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

	// ������ ������ �� ��������� �����������
	// � ����������� �� ���� ������ ������ ���������� ���� ������, ���� �������������
	// ������� ������, ��������� �� ������� ��������, � ����� � ������, ����� �������� �� ������,
	// ������ ������������ � ������� ������� ����� '.'
	// ����� ������, ��������� �� ������� ������� image, ������ �������������.
	void Draw(Image& image) const {
		//(void)image;
		Size image_size = GetImageSize(image);
		
		//���� ������ ���������� �� ��������� ����������� - �����
		if (pos_.x >= image_size.width || pos_.y >= image_size.height) {
			return;
		}
		//����������� ������� ��������
		bool is_texture = texture_ptr_? true : false;
		switch (type_)
		{
		case ShapeType::RECTANGLE:
			
			/*for (const auto& line : image) {
				for (const auto& pixel : line) {

				}
			}*/
			//������������ �� ���������� ������� ������ � ��������� ������� �������� �� �����������
			
			//����������� �� ���������
			for (int i = 0; i < size_.height; i++)
			{
				//�������� ������ �� ������� ����������� �� ������
				//(���, ��� ������� - �� ������)
				if (pos_.y + i >= image_size.height) {
					break;
				}
				
				//����������� �� �����������
				for (int j = 0; j < size_.width; j++)
				{
					//�������� ������ �� ������� ����������� �� ������
					//(���, ��� ������� - �� ������)
					if (pos_.x + j >= image_size.width) {
						break;
					}
					if (is_texture) {
						//�������� ������ �� ������� ��������
						//(���, ��� ������� - ������ '.')
						if ((pos_.y + i > texture_ptr_->GetSize().height) || (pos_.x + j > texture_ptr_->GetSize().width)) {
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
			//inline bool IsPointInEllipse(Point p, Size size)
			//����������� �� ���������
			for (int i = 0; i < size_.height; i++)
			{
				//�������� ������ �� ������� ����������� �� ������
				//(���, ��� ������� - �� ������)
				if (pos_.y + i >= image_size.height) {
					break;
				}

				//����������� �� �����������
				for (int j = 0; j < size_.width; j++)
				{
					//�������� ������ �� ������� ����������� �� ������
					//(���, ��� ������� - �� ������)
					if (pos_.x + j >= image_size.width) {
						break;
					}
					//�������� ����, ��� ����� ������ �������
					if (IsPointInEllipse({j,i}, size_)) {
						if (is_texture) {
							//�������� ������ �� ������� ��������
							//(���, ��� ������� - ������ '.')
							if ((pos_.y + i > texture_ptr_->GetSize().height) || (pos_.x + j > texture_ptr_->GetSize().width)) {
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
	//������ ������
	Size size_ = { 0, 0 };
	//���������� �������� ������ ����
	Point pos_ = { 0, 0 };
	std::shared_ptr<Texture> texture_ptr_;
};