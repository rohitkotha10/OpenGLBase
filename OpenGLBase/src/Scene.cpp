#include "Scene.h"

namespace OpenGLBase
{
	void Mesh::setupMesh()
	{
		vao.create();
		vbo.create();
		ebo.create();

		vao.bind();
		vbo.bind();
		vbo.setvec3(&vertices[0], vertices.size() * sizeof(Vertex));

		ebo.bind();
		ebo.setIndices(&indices[0], indices.size() * sizeof(unsigned int));

		vao.setAttrib(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		vao.enable(0);
		vao.setAttrib(1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		vao.enable(1);
		vao.setAttrib(2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		vao.enable(2);
		vao.unbind();
	}

	void Mesh::drawMesh(Program& program)
	{
		int numDiff = 1;
		int numSpec = 1;
		for (int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = textures[i].type;
			if (name == "diffuse")
				number = std::to_string(numDiff++);
			program.setTexture("material." + name + number, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		vao.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		vao.unbind();
	}

	void Scene::source(std::string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ASSIMP import error: " << import.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Scene::drawScene(Program& program)
	{
		for (int i = 0; i < meshes.size(); i++)
			meshes[i].drawMesh(program);
	}

	void Scene::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Scene::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}
		
	std::vector<Texture> Scene::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				TexBuffer tex;

				tex.create();
				tex.bind();
				std::string filename = std::string(str.C_Str());
				filename = directory + '/' + filename;
				tex.setTexture(filename, true);

				texture.id = tex.getData();
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
	}
}
