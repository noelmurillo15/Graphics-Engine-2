#include "RenderMesh.h"


RenderMesh::RenderMesh() {

}

RenderMesh::~RenderMesh() {

}

bool RenderMesh::LoadOBJfromFile(const char* path) {
	vector<unsigned int> posIndicies, uvIndicies, normIndicies;
	vector<FLOAT3> tmp_Pos;
	vector<FLOAT2> tmp_Uvs;
	vector<FLOAT3> tmp_Norms;

	FILE* file = NULL;
	errno_t err;

	if ((err = fopen_s(&file, path, "r")) != 0) {
		printf("Impossible to open!\n");
		return false;
	}
	else {
		while (true) {
			char lineHeader[128];

			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;

			//	pos
			if (strcmp(lineHeader, "v") == 0) {
				FLOAT3 pos;
				fscanf(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
				tmp_Pos.push_back(pos);
			}
			//	uvs
			else if (strcmp(lineHeader, "vt") == 0) {
				FLOAT2 uv;
				fscanf(file, "%f %f\n", &uv.u, &uv.v);
				tmp_Uvs.push_back(uv);
			}
			//	normals
			else if (strcmp(lineHeader, "vn") == 0) {
				FLOAT3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				tmp_Norms.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("Cannot be read properly!");
					return false;
				}
				posIndicies.push_back(vertexIndex[0]);
				posIndicies.push_back(vertexIndex[1]);
				posIndicies.push_back(vertexIndex[2]);
				uvIndicies.push_back(uvIndex[0]);
				uvIndicies.push_back(uvIndex[1]);
				uvIndicies.push_back(uvIndex[2]);
				normIndicies.push_back(normalIndex[0]);
				normIndicies.push_back(normalIndex[1]);
				normIndicies.push_back(normalIndex[2]);
			}
		}
	}

	for (unsigned int i = 0; i < posIndicies.size(); ++i) {
		Vert temp;
		temp.pos = tmp_Pos[posIndicies[i] - 1];
		temp.uv = tmp_Uvs[uvIndicies[i] - 1];
		temp.norm = tmp_Norms[normIndicies[i] - 1];
		temp.tan = FLOAT3(0.0f, 0.0f, 0.0f);
		
		data.interleaved.push_back(temp);
		data.indicies.push_back(i);
	}
	return true;
}