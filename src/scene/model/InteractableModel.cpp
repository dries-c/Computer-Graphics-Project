#include "InteractableModel.h"
#include <iostream>

void InteractableModel::onAttack() {
    std::cout << "InteractableModel::onAttack()" << std::endl;
}

void InteractableModel::onInteract() {
    std::cout << "InteractableModel::onInteract()" << std::endl;
    kill();
}

std::vector<AxisAlignedBB> InteractableModel::getBoundingBoxes() const {
    return Model::getBoundingBoxes(getModelMatrix());
}

glm::mat4 InteractableModel::getModelMatrix() const {
    return Model::getModelMatrices()[0];
}
