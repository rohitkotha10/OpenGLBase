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
		std::cout << "HERE" << std::endl;
		int numDiff = 1;//3 max
		int numSpec = 1;//3 max
		for (int i = 0; i < textures.size(); i++)
		{
			std::string number;
			std::string name = textures[i].type;
			if (name == "diffuse")
				number = std::to_string(numDiff++);
			else if (name == "specular")
				number = std::to_string(numSpec++);

			textures[i].data.active(i);
			program.setTexture("material." + name + number, i);
		}

		program.setFloat("material.shininess", 32.0f);

		vao.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		vao.unbind();
	}

	void Scene::drawScene(Program& program)
	{
		Mesh*& curMesh = rootStart->head;
		while (curMesh != nullptr)
		{
			std::cout << "YES MESH" << std::endl;
			curMesh->drawMesh(program);
			curMesh = curMesh->next;
		}
		drawChilds(program, rootStart->children);
	}

	void Scene::drawChilds(Program& program, MeshList**& cur)
	{
		if (cur == nullptr) {
			std::cout << "YES" << std::endl;
			return;
		}

		while (*cur != nullptr)
		{
			Mesh*& curMesh = (*cur)->head;
			while (curMesh != nullptr)
			{
				std::cout << "YES MESH" << std::endl;
				curMesh->drawMesh(program);
				curMesh = curMesh->next;
			}
			drawChilds(program, (*cur)->children);
			*cur = (*cur)->nextList;
		}

	}


	void Scene::source(std::string path, bool flipTexture)
	{
		Assimp::Importer import;
		stbi_set_flip_vertically_on_load(flipTexture);
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ASSIMP import error: " << import.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene, rootStart);
		if (*rootStart->children == nullptr)
			std::cout << "YES NULLCHILD" << std::endl;
		//std::cout << (*rootStart->children)->numMeshes << std::endl;

	}

	void Scene::processNode(aiNode* node, const aiScene* scene, MeshList*& cur)
	{
		if (node->mNumMeshes > 0 || node->mNumChildren > 0)
			cur = new MeshList();
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			cur->add(processMesh(mesh, scene));
			cur->numMeshes++;
		}
//Problem Here: Try to reference the head in MyList** children before drawing
		if (node->mNumChildren > 0)
			cur->children = new MeshList * ();

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			*cur->children = new MeshList();
			processNode(node->mChildren[i], scene, *cur->children);
			*cur->children = (*cur->children)->nextList;
			cur->numChildren++;
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
		//1 material for each mesh
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps =
				loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			std::vector<Texture> specularMaps =
				loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");

			for (int i = 0; i < diffuseMaps.size(); i++)
				textures.push_back(diffuseMaps[i]);

			for (int i = 0; i < specularMaps.size(); i++)
				textures.push_back(specularMaps[i]);
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
					skip = true; //same path
					break;
				}
			}
			if (!skip)
			{
				Texture texture;

				texture.data.create();
				texture.data.bind();
				std::string filename = std::string(str.C_Str());
				filename = directory + '/' + filename;
				texture.data.setTexture(filename);

				texture.type = typeName;
				texture.path = str.C_Str();
				textures_loaded.push_back(texture);
				std::cout << filename << std::endl;
				textures.push_back(texture);
			}
		}
		return textures;
	}

	void MeshList::add(Mesh newMesh)
	{
		Mesh* cur = this->head;
		if (cur == nullptr)
		{
			head = new Mesh(newMesh);
			numMeshes++;
			return;
		}
		while (cur->next != nullptr)
			cur = cur->next;
		cur->next = new Mesh(newMesh);
		numMeshes++;
	}
}
