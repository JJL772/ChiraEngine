#include "abstractComponent.h"

abstractComponent::abstractComponent(const std::string& entity_, const std::string& name_) {
    this->entity = entity_;
    this->name = name_;
}
