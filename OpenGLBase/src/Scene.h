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
#include "./Buffer.h"

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
		TexBuffer data;
		std::string type;
		std::string path;//relative to model directory
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture> texs)
			:vertices(vert), indices(ind), textures(texs) {
			setupMesh();
		};
		
		void setupMesh();
		void drawMesh(Program& program);

	private:
		//Mesh* next;
		//Mesh** children;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		VertexArray vao;
		VertexBuffer vbo;
		IndexBuffer ebo;

	};

	class Scene
	{
	public:
		void source(std::string path, bool flipTexture);
		void drawScene(Program& program);
		
	private:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Mesh> meshes;
		//Mesh* rootMesh;
		std::vector<Texture> textures_loaded;
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		std::string directory;
	};
}

