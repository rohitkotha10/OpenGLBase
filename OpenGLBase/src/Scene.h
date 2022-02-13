#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "./Program.h"
#include "./VertexArray.h"
#include "./TexBuffer.h"
#include "./BUffer.h"

namespace OpenGLBase
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture> texs)
			:vertices(vert), indices(ind), textures(texs) {
			setupMesh();
		};
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		void setupMesh();
		void drawMesh(Program& program);
	private:
		VertexArray vao;
		VertexBuffer vbo;
		IndexBuffer ebo;

	};

	class Model
	{
	public:
		std::vector<int> meshIndices;
		Model* children;
	};

	class Scene
	{
	public:
		void source(std::string path);
		void drawScene(Program& program);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		int numMeshes = 0;
		int numMaterials = 0;
		std::vector<Mesh> meshes;
		std::vector<Texture> textures_loaded;
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		Model* rootNode;
		std::string directory;
	};
}

