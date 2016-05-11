#include "model.h"

namespace wake
{
    ModelComponent ModelComponent::Invalid;

    Model::Model()
    {
    }

    Model::Model(const std::vector<ModelComponent> components)
    {
        this->components = components;
    }

    Model::Model(const Model& other)
    {
        this->components = other.components;
    }

    Model::~Model()
    {
    }

    Model& Model::operator=(const Model& other)
    {
        this->components = other.components;
        return *this;
    }

    void Model::setComponents(const std::vector<ModelComponent> components)
    {
        this->components = components;
    }

    const std::vector<ModelComponent>& Model::getComponents() const
    {
        return components;
    }

    size_t Model::getComponentCount() const
    {
        return components.size();
    }

    const ModelComponent& Model::getComponent(size_t index) const
    {
        if (index < 0 || index >= components.size())
            return ModelComponent::Invalid;

        return components[index];
    }

    void Model::setComponent(size_t index, const ModelComponent& component)
    {
        if (index < 0 || index >= components.size())
            return;

        components[index] = component;
    }

    void Model::setMesh(size_t index, MeshPtr mesh)
    {
        if (index < 0 || index >= components.size())
            return;

        components[index].mesh = mesh;
    }

    void Model::setMaterial(size_t index, MaterialPtr material)
    {
        if (index < 0 || index >= components.size())
            return;

        components[index].material = material;
    }

    void Model::removeComponent(size_t index)
    {
        if (index < 0 || index >= components.size())
            return;

        components.erase(components.begin() + index);
    }

    void Model::addComponent(const ModelComponent& component)
    {
        components.push_back(component);
    }

    void Model::draw()
    {
        for (auto& component : components)
        {
            if (component.mesh.get() == nullptr || component.material.get() == nullptr)
                continue;

            component.material->use();
            component.mesh->draw();
        }
    }
}