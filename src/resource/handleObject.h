#pragma once

#include "abstractResource.h"

class handleObject : public abstractResource {
public:
    handleObject(): abstractResource() {}
    [[nodiscard]] int getHandle() const {
        return this->handle;
    }
protected:
    int handle = -1;
};
