#include "Image.h"

void Image::Load(const std::string& filepath)
{

	image.loadFromFile(filepath);

}

Vec3 Image::GetColour(float u, float v)
{
	int x = image.getSize().x - 1;
	int y = image.getSize().y - 1;
	int pixelX, pixelY;
	pixelX = u * x;
	pixelY = (1.0f - v) * y;

	Vec3 colour(float(image.getPixel(pixelX, pixelY).r) / 255.0f,
		float(image.getPixel(pixelX, pixelY).g)/ 255.0f,
		float(image.getPixel(pixelX, pixelY).b)/ 255.0f);

	return colour;
}
