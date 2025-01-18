#include "Model.h"
#include "Debug.h"

#include <glad/glad.h>


namespace ModelLoader {

	GLint TextureFromFile(const char* fname, const std::string& directory) {
		std::string filename(fname);
		filename = directory + '/' + filename;
		unsigned int textureID;
		glGenTextures(1, &textureID);
		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << filename << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}



	Model::Model(const char* fname) : filePath(fname) {
	}

	void Model::Init() {
		loadModel(filePath);
	}

	void Model::Render(Shader* shader) {

		for (int i = 0; i < meshes.size(); i++) {
			meshes[i].Render(shader);
		}



	}

	void Model::loadModel(std::string path) {
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_TransformUVCoords);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);

	}


	void Model::processNode(aiNode* node, const aiScene* scene) {

		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}



	ModelLoader::Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<ModelLoader::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<ModelLoader::Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			ModelLoader::Vertex vertex;
			glm::vec3 vec;

			vec.x = mesh->mVertices[i].x;
			vec.y = mesh->mVertices[i].y;
			vec.z = mesh->mVertices[i].z;
			vertex.Position = vec;

			// Normals
			if (mesh->HasNormals()) {
				vec.x = mesh->mNormals[i].x;
				vec.y = mesh->mNormals[i].y;
				vec.z = mesh->mNormals[i].z;
				vertex.Normal = vec;
			}
			else {
				vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
			}

			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;

			}
			else {
				vertex.TexCoords = glm::vec2(0.f, 0.f);
			}

			vertices.push_back(vertex);
		}

		for (unsigned i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];

			for (unsigned j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}


		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<ModelLoader::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<ModelLoader::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());


		return ModelLoader::Mesh(vertices, indices, textures);
	}


	std::vector<ModelLoader::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<ModelLoader::Texture> textures;

		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			std::cout << textures_loaded.size() << "\n";
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)

					break;
				}
			}

			if (!skip)
			{   // If texture hasn't been loaded already, load it
				ModelLoader::Texture texture;

				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);

				textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}

		return textures;
	}


}