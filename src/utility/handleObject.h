#pragma once

#include "../core/resource.h"

class handleObject : public resource {
public:
    handleObject(): resource() {}
    [[nodiscard]] int getHandle() const {
        return this->handle;
    }
protected:
    int handle = -1;
};
