#include "abstractComponent.h"

abstractComponent::abstractComponent(const uuids::uuid& entityId, const std::string& name_) {
    this->entity = entityId;
    this->name = name_;
}

const uuids::uuid& abstractComponent::getEntity() const {
    return this->entity;
}

const std::string& abstractComponent::getName() {
    return this->name;
}
