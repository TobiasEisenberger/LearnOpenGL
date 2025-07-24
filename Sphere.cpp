#include "Sphere.h"

Sphere::Sphere()
{
	vertexCount = circlesOfLatitude * circlesOfLongitude + 2;
	verticeCoords.reserve(vertexCount * 3);
	indices.reserve(6 * vertexCount);

	CreateMesh();
}

void Sphere::CreateMesh()
{
	// North pole
	verticeCoords.push_back(0.0f);
	verticeCoords.push_back(1.0f);
	verticeCoords.push_back(0.0f);

	for (int i = 1; i < circlesOfLatitude + 1; i++)
	{
		float pitchAngleToCurrentLatCircle = std::numbers::pi / (circlesOfLatitude + 1) * i;
		float latStartY = cos(pitchAngleToCurrentLatCircle);
		float latStartZ = sin(pitchAngleToCurrentLatCircle);

		for (int k = 0; k < circlesOfLongitude; k++)
		{
			float yawAngleForCurrentLatPos = 2 * std::numbers::pi / circlesOfLongitude * k;
			float lonX = sin(yawAngleForCurrentLatPos);
			float lonZ = cos(yawAngleForCurrentLatPos);

			verticeCoords.push_back(lonX * latStartZ);
			verticeCoords.push_back(latStartY);
			verticeCoords.push_back(lonZ * latStartZ);

			if (i == 1) // North pole triangles
			{
				indices.push_back(k + 1);
				indices.push_back((k + 2) % circlesOfLongitude);
				indices.push_back(0);
			}
			else
			{
				// Quads
				int vertexOffsetCurrentCircle = circlesOfLongitude * (i - 1);
				int vertexOffsetLastCircle = circlesOfLongitude * (i - 2);
				indices.push_back((k + 1) + vertexOffsetCurrentCircle);
				indices.push_back((k + 2) + vertexOffsetCurrentCircle);
				indices.push_back((k + 2) + vertexOffsetLastCircle);

				indices.push_back((k + 1) + vertexOffsetCurrentCircle);
				indices.push_back((k + 2) + vertexOffsetLastCircle);
				indices.push_back((k + 1) + vertexOffsetLastCircle);

				if (i == circlesOfLatitude) // South pole triangles
				{
					indices.push_back((k + 1) + vertexOffsetCurrentCircle);
					indices.push_back(vertexCount - 1);
					indices.push_back((k + 2) + vertexOffsetCurrentCircle);
				}
			}
		}
	}

	// South pole
	verticeCoords.push_back(0.0f);
	verticeCoords.push_back(-1.0f);
	verticeCoords.push_back(0.0f);
}

const std::vector<float>& Sphere::GetVertices()
{
	return verticeCoords;
}

const std::vector<unsigned int>& Sphere::GetIndices()
{
	return indices;
}