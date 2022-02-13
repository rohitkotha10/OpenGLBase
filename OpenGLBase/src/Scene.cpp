#include "Scene.h"

//void processNode(aiNode* node, const aiScene* scene)
//{
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(processMesh(mesh, scene));
//	}
//
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene);
//	}
//}
//
//Mesh processMesh(aiMesh* mesh, const aiScene* scene)
//{
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//
//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//	{
//		Vertex vertex;
//		glm::vec3 vector;
//		vector.x = mesh->mVertices[i].x;
//		vector.y = mesh->mVertices[i].y;
//		vector.z = mesh->mVertices[i].z;
//		vertex.position = vector;
//
//		vector.x = mesh->mNormals[i].x;
//		vector.y = mesh->mNormals[i].y;
//		vector.z = mesh->mNormals[i].z;
//		vertex.normal = vector;
//
//		if (mesh->mTextureCoords[0])
//		{
//			glm::vec2 vec;
//			vec.x = mesh->mTextureCoords[0][i].x;
//			vec.y = mesh->mTextureCoords[0][i].y;
//			vertex.texCoords = vec;
//		}
//		else
//			vertex.texCoords = glm::vec2(0.0f, 0.0f);
//		vertices.push_back(vertex);
//	}
//
//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//		for (unsigned int j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}
//
//	return Mesh(vertices, indices);
//}
