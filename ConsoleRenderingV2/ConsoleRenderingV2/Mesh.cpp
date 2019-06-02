#include "Mesh.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

Mesh::Mesh(std::string t_fileIn) {
	std::list<Vector3> vertPos();
	std::list<Vector3> vertNormal();
	std::list<Color> vertCol();

	std::ifstream file(t_fileIn);
	std::string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::vector<std::string> tokens;
			size_t prev = 0, pos = 0;

			do {
				pos = line.find(" ", prev);

				if (pos == std::string::npos)
					pos = line.length();
				std::string token = line.substr(prev, pos - prev);
				if (!token.empty())
					tokens.push_back(token);
				prev = pos + 1;

			} while (pos < line.length() && prev < line.length());

			//Now we to parse the vector of strings

			if (tokens.front.compare("v") == 0) {
				//We have a vertex
			}
			else if (tokens.front.compare("f") == 0) {

			}

		}
		file.close();
	}
}