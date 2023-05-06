#include "InteractableModel.h"
#include <iostream>

std::vector<AxisAlignedBB> InteractableModel::getBoundingBoxes() const {
    return Model::getBoundingBoxes(getModelMatrix());
}

glm::mat4 InteractableModel::getModelMatrix() const {
    return Model::getModelMatrices()[0];
}
