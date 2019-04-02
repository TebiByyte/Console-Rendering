#include "pch.h"
#include "Color.h"
#include <math.h>
#include <iostream>

short Color::Luminance() {
	return (short)sqrt(0.299 * this->R * this->R + 0.587 * this->G * this->G + 0.114 * this->B * this->B);
}

Color operator*(const Color& a, const Color& b) {
	float div = 255 * 255;

	float R = (a.R * b.R) / div;
	float G = (a.G * b.G) / div;
	float B = (a.B * b.B) / div;

	Color result = { 255 * (short)floor(R), 255 * (short)floor(G), 255 * (short)floor(B) };

	return result;
}

Color operator*(const Color& a, float b) { 
	float R = a.R * b;
	float G = a.G * b;
	float B = a.B * b;

	Color result = { (short)floor(R), (short)floor(G) , (short)floor(B) };

	return result;
}

Color operator*(float a, const Color& b) {
	return b * a;
}

Color operator+(const Color& a, const Color& b) {
	Color result = { a.R + b.R, a.G + b.G, a.B + b.B };	
	return result;
}

Color operator-(const Color& a, const Color& b) {
	Color result = Color{ a.R - b.R, a.G - b.G, a.B - b.B };
	return result;
}

