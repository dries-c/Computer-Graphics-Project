#ifndef OPENGL_PROJECT_INTERACTABLEMODEL_H
#define OPENGL_PROJECT_INTERACTABLEMODEL_H

#include "Model.h"
#include "../../utils/Interactable.h"

class InteractableModel : public Model, public Interactable {
public:
    InteractableModel(glm::mat4 modelMatrix, Shader *shader, const std::vector<Mesh *> &meshes, bool collision = true) : Model(modelMatrix, shader, meshes, collision) {};
    std::vector<AxisAlignedBB> getBoundingBoxes() const override;
    glm::mat4 getModelMatrix() const override;
};


#endif
