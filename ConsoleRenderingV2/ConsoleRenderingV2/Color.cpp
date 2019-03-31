#include "pch.h"
#include "Color.h"
#include <math.h>
#include <iostream>

short Color::Luminance() {
	return (short)sqrt(0.299 * this->R * this->R + 0.587 * this->G * this->G + 0.114 * this->B * this->B);
}

Color::Color(short R = 0, short G = 0, short B = 0) {
	this->R = R;
	this->G = G;
	this->B = B;
}

Color::Color() {
	this->R = 0;
	this->G = 0;
	this->B = 0;
}

Color operator*(Color& a, Color& b) {
	float div = 255 * 255;

	float R = (a.R * b.R) / div;
	float G = (a.G * b.G) / div;
	float B = (a.B * b.B) / div;

	return Color(255 * floor(R), 255 * floor(G), 255 * floor(B));
}

Color operator*(Color& a, float b) { 
	float R = a.R * b;
	float G = a.G * b;
	float B = a.B * b;

	Color result = Color(floor(R), floor(G) , floor(B));
	return result; 
}

Color operator*(float a, Color& b) {
	return b * a;
}

Color operator+(Color& a, Color& b) {
	return Color(a.R + b.R, a.G + b.G, a.B + b.B);
}

Color operator-(Color& a, Color& b) {
	return Color(a.R - b.R, a.G - b.G, a.B - b.B);
}

