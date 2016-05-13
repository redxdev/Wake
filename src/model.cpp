#include "model.h"

#include <algorithm>

namespace wake
{
    Model::MaterialInfo Model::MaterialInfo::Invalid;
    Model::MeshInfo Model::MeshInfo::Invalid;

    Model::Model()
    {
    }

    Model::Model(const Model& other)
    {
        this->materials = other.materials;
        this->meshes = other.meshes;
    }

    Model::~Model()
    {
    }

    Model& Model::operator=(const Model& other)
    {
        this->materials = other.materials;
        this->meshes = other.meshes;
        return *this;
    }

    const ModelMetadata& Model::getMetadata()
    {
        return metadata;
    }

    void Model::setMetadata(const ModelMetadata& metadata)
    {
        this->metadata = metadata;
    }

    int32 Model::getMaterialCount() const
    {
        return materials.size();
    }

    const std::vector<Model::MaterialInfo>& Model::getMaterials() const
    {
        return materials;
    }

    const Model::MaterialInfo& Model::getMaterial(int32 index) const
    {
        if (index < 0 || (size_t) index >= materials.size())
            return MaterialInfo::Invalid;

        return materials[index];
    }

    const Model::MaterialInfo& Model::getMaterialByName(const std::string& name) const
    {
        auto found = std::find_if(materials.begin(), materials.end(), [&](const MaterialInfo& matInfo) {
            return matInfo.name == name;
        });

        if (found == materials.end())
            return MaterialInfo::Invalid;

        return *found;
    }

    int32 Model::getMaterialIndex(const std::string& name) const
    {
        auto found = std::find_if(materials.begin(), materials.end(), [&](const MaterialInfo& matInfo) {
            return matInfo.name == name;
        });

        if (found == materials.end())
            return -1;

        return (int32) std::distance(materials.begin(), found);
    }

    bool Model::setMaterial(int32 index, MaterialPtr material)
    {
        if (index < 0 || (size_t) index >= materials.size())
            return false;

        materials[index].material = material;
        return true;
    }

    bool Model::setMaterialByName(const std::string& name, MaterialPtr material)
    {
        auto found = std::find_if(materials.begin(), materials.end(), [&](const MaterialInfo& matInfo) {
            return matInfo.name == name;
        });

        if (found == materials.end())
            return false;

        found->material = material;
        return true;
    }

    bool Model::addMaterial(const std::string& name, MaterialPtr material)
    {
        auto found = std::find_if(materials.begin(), materials.end(), [&](const MaterialInfo& matInfo) {
            return matInfo.name == name;
        });

        if (found != materials.end())
            return false;

        MaterialInfo matInfo;
        matInfo.material = material;
        matInfo.name = name;

        materials.push_back(matInfo);
        return true;
    }

    bool Model::hasMaterialName(const std::string& name) const
    {
        auto found = std::find_if(materials.begin(), materials.end(), [&](const MaterialInfo& matInfo) {
            return matInfo.name == name;
        });

        return found != materials.end();
    }

    bool Model::renameMaterial(int32 index, const std::string& newName)
    {
        if (index < 0 || (size_t) index >= materials.size())
            return false;

        if (hasMaterialName(newName))
            return false;

        auto& oldMatInfo = materials[index];
        oldMatInfo.name = newName;

        return true;
    }

    bool Model::removeMaterial(int32 index)
    {
        if (index < 0 || (size_t) index >= materials.size())
            return false;

        materials.erase(materials.begin() + index);
        return true;
    }

    bool Model::removeMaterialByName(const std::string& name)
    {
        return removeMaterial(getMaterialIndex(name));
    }

    int32 Model::getMeshCount() const
    {
        return meshes.size();
    }

    const std::vector<Model::MeshInfo>& Model::getMeshes() const
    {
        return meshes;
    }

    const Model::MeshInfo& Model::getMesh(int32 index) const
    {
        if (index < 0 || (size_t) index >= meshes.size())
            return MeshInfo::Invalid;

        return meshes[index];
    }

    bool Model::setMesh(int32 index, MeshPtr mesh)
    {
        if (index < 0 || (size_t) index >= meshes.size())
            return false;

        meshes[index].mesh = mesh;
        return true;
    }

    bool Model::setMeshMaterial(int32 index, int32 materialIndex)
    {
        if (index < 0 || (size_t) index >= meshes.size())
            return false;

        meshes[index].materialIndex = materialIndex;
        return true;
    }

    bool Model::setMeshMaterialByName(int32 index, const std::string& name)
    {
        if (index < 0 || (size_t) index >= meshes.size())
            return false;

        meshes[index].materialIndex = getMaterialIndex(name);
        return true;
    }

    void Model::addMesh(MeshPtr mesh, int32 materialIndex)
    {
        MeshInfo meshInfo;
        meshInfo.mesh = mesh;
        meshInfo.materialIndex = materialIndex;

        meshes.push_back(meshInfo);
    }

    bool Model::removeMesh(int32 index)
    {
        if (index < 0 || (size_t) index >= meshes.size())
            return false;

        meshes.erase(meshes.begin() + index);
        return true;
    }

    void Model::draw()
    {
        for (auto& meshInfo : meshes)
        {
            if (meshInfo.materialIndex < 0 || (size_t) meshInfo.materialIndex >= materials.size())
                continue;

            auto& materialInfo = materials[meshInfo.materialIndex];

            materialInfo.material->use();
            meshInfo.mesh->draw();
        }
    }
}