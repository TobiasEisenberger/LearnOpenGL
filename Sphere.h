#pragma once

#include <vector>
#include <math.h>
#include <numbers>

class Sphere
{

public:
	Sphere();
	const std::vector<float>& GetVertices();
	const std::vector<unsigned int>& GetIndices();

private:
	const int circlesOfLatitude = 15;
	const int circlesOfLongitude = 32;
	int vertexCount;

	std::vector<float> verticeCoords;
	std::vector<unsigned int> indices;

	void CreateMesh();
};