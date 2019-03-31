#pragma once

class Color {
public:
	short R;
	short G;
	short B;

	short Luminance();

	Color(short, short, short);
	Color();
};	

Color operator+(Color&, Color&);
Color operator-(Color&, Color&);
Color operator*(Color&, Color&);
Color operator*(float, Color&);
Color operator*(Color&, float);