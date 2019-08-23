#include "pch.h"
#include "Mesh.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

Mesh::Mesh(std::string t_fileIn) {
	struct inpoint{
		int v;
		int n;
		int t;
	};

	std::list<Vector3> vertPos;
	std::list<Vector3> vertNormal;
	std::list<Color> vertCol;
	std::list<inpoint> indexedPoints;

	std::ifstream file(t_fileIn);
	std::string line;

	if (file.fail()) {
		std::cerr << "Couldn't open file" << std::endl;
		exit(1);
	}

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::vector<std::string> tokens;
			size_t prev = 0, pos = 0;

			do {
				pos = line.find(" ", prev);

				if (pos == std::string::npos) {
					pos = line.length();
				}
				std::string token = line.substr(prev, pos - prev);
				if (!token.empty()) {
					tokens.push_back(token);
				}
				prev = pos + 1;

			} while (pos < line.length() && prev < line.length());

			std::string::size_type sz;
			//Now we to parse the vector of strings
			std::string frontToken = tokens.front();

			if (frontToken.compare("v") == 0) {
				//We have a vertex
				float x = std::stof(tokens.at(1), &sz);
				float y = std::stof(tokens.at(2), &sz);
				float z = std::stof(tokens.at(3), &sz);
				vertPos.push_back({ x, y, z });
			}
			else if (frontToken.compare("vn") == 0) {
				//Vertex normal
				float x = std::stof(tokens.at(1), &sz);
				float y = std::stof(tokens.at(2), &sz);
				float z = std::stof(tokens.at(3), &sz);
				vertNormal.push_back({ x, y, z });
			}
			else if (frontToken.compare("vt") == 0){
				//UV position

			}
			else if (frontToken.compare("f") == 0) {
				//We have a face / triangle
				if (tokens.at(1).find("//") != std::string::npos) {
					//f v1//vn1 v2//vn2 v3//vn3
					int occurPos1 = tokens.at(1).find("//");
					int occurPos2 = tokens.at(2).find("//");
					int occurPos3 = tokens.at(3).find("//");

					int v1 = std::stoi(tokens.at(1).substr(0, occurPos1)) - 1;
					int v2 = std::stoi(tokens.at(2).substr(0, occurPos2)) - 1;
					int v3 = std::stoi(tokens.at(3).substr(0, occurPos3)) - 1;

					int n1 = std::stoi(tokens.at(1).substr(occurPos1 + 2, occurPos1 + 2 - tokens.at(1).size())) - 1;
					int n2 = std::stoi(tokens.at(2).substr(occurPos2 + 2, occurPos2 + 2 - tokens.at(2).size())) - 1;
					int n3 = std::stoi(tokens.at(3).substr(occurPos3 + 2, occurPos3 + 2 - tokens.at(3).size())) - 1;
					indexedPoints.push_back({ v1, n1, -1 });
					indexedPoints.push_back({ v2, n2, -1 });
					indexedPoints.push_back({ v3, n3, -1 });
				}
				else if (tokens.at(1).find("/") != std::string::npos) {
					//Do nothing, don't worry just yet
				}
				else {
					int v1 = std::stoi(tokens.at(1));
					int v2 = std::stoi(tokens.at(2));
					int v3 = std::stoi(tokens.at(3));

					indexedPoints.push_back({ v1, -1, -1 });
					indexedPoints.push_back({ v2, -1, -1 });
					indexedPoints.push_back({ v3, -1, -1 });
				}
			}
		}

		Vector3* verts = new Vector3[vertPos.size()];
		Vector3* norms = new Vector3[vertPos.size()];
		VertexAttribute* points = new VertexAttribute[indexedPoints.size()];

		std::copy(vertPos.begin(), vertPos.end(), verts);
		std::copy(vertNormal.begin(), vertNormal.end(), norms);

		int i = 0;
		for (inpoint const &p : indexedPoints) {
			VertexAttribute point = { verts[p.v],  norms[p.n], { } };
			points[i] = point;
			i++;
		}

		m_data = new Triangle[indexedPoints.size() / 3];
		triCount = indexedPoints.size() / 3;

		for (unsigned int j = 0; j < indexedPoints.size(); j += 3) {

			points[j    ].VertexColor = { 255, 255, 255 };
			points[j + 1].VertexColor = { 255, 255, 255 };
			points[j + 2].VertexColor = { 255, 255, 255 };

			m_data[j / 3].a = points[j];
			m_data[j / 3].b = points[j + 1];
			m_data[j / 3].c = points[j + 2];
		}
	}

	file.close();
}