#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>

#include "mesh.h"
#include "material.h"
#include "engineptr.h"
#include "util.h"

namespace wake
{
    struct ModelMetadata
    {
        ModelMetadata()
        {
            source = Unknown;
            version = 0;
        }

        enum
        {
            Unknown,
            WMDL,
            Assimp
        } source;

        uint32 version;

        std::string path;
    };

    // TODO: Sort components by material (and shader?) and draw them in batches
    class Model
    {
    public:
        struct MaterialInfo
        {
            static MaterialInfo Invalid;

            MaterialInfo()
            {
                material = nullptr;
                name = "";
            }

            MaterialPtr material;
            std::string name;
        };

        struct MeshInfo
        {
            static MeshInfo Invalid;

            MeshInfo()
            {
                mesh = nullptr;
                materialIndex = -1;
            }

            MeshPtr mesh;
            int32 materialIndex;
        };
    public:
        Model();

        Model(const Model& other);

        ~Model();

        Model& operator=(const Model& other);

        const ModelMetadata& getMetadata();

        void setMetadata(const ModelMetadata& metadata);

        int32 getMaterialCount() const;

        const std::vector<MaterialInfo>& getMaterials() const;

        const MaterialInfo& getMaterial(int32 index) const;

        const MaterialInfo& getMaterialByName(const std::string& name) const;

        int32 getMaterialIndex(const std::string& name) const;

        bool setMaterial(int32 index, MaterialPtr material);

        bool setMaterialByName(const std::string& name, MaterialPtr material);

        bool addMaterial(const std::string& name, MaterialPtr material);

        bool hasMaterialName(const std::string& name) const;

        bool renameMaterial(int32 index, const std::string& newName);

        bool removeMaterial(int32 index);

        bool removeMaterialByName(const std::string& name);

        int32 getMeshCount() const;

        const std::vector<MeshInfo>& getMeshes() const;

        const MeshInfo& getMesh(int32 index) const;

        bool setMesh(int32 index, MeshPtr mesh);

        bool setMeshMaterial(int32 index, int32 materialIndex);

        bool setMeshMaterialByName(int32 index, const std::string& name);

        void addMesh(MeshPtr mesh, int32 materialIndex);

        bool removeMesh(int32 index);

        // TODO: Allow arbitrary parameters to draw, and cache uniforms.
        void draw(const glm::mat4& transform);

    private:
        std::vector<MaterialInfo> materials;
        std::vector<MeshInfo> meshes;
        ModelMetadata metadata;
    };

    typedef SharedPtr<Model> ModelPtr;
}