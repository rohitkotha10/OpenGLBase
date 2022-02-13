//#pragma once
//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#include <glm/glm.hpp>
//#include <vector>
//#include <string>
//
//#include "./Program.h"
//
//namespace OpenGLBase
//{
//	struct Vertex
//	{
//		glm::vec3 position;
//		glm::vec3 Normal;
//		glm::vec3 TexCoords;
//	};
//
//	struct Texture
//	{
//		unsigned int id;
//		std::string type;
//	};
//
//	class Mesh
//	{
//	public:
//		Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind)
//			:vertices(vert), indices(ind) {};
//		std::vector<Vertex> vertices;
//		std::vector<unsigned int> indices;
//		std::vector<Texture> textures;
//		void drawMesh(Program& program);
//	};
//
//	class Model
//	{
//	public:
//		std::vector<int> meshIndices;
//		Model* children;
//	};
//
//	class Scene
//	{
//	public:
//		void source(std::string path);
//		void drawScene(Program& program);
//		void processModels();
//		void processMeshes();
//		std::vector<Mesh> meshes;
//		Model* rootNode;
//	};
//}
//
