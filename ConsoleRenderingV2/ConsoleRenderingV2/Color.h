#pragma once

struct Color {
public:
	short R;
	short G;
	short B;

	short Luminance();
};	

Color operator+(const Color&, const Color&);
Color operator-(const Color&, const Color&);
Color operator*(const Color&, const Color&);
Color operator*(float, const Color&);
Color operator*(const Color&, float);