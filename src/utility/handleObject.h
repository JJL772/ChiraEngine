#pragma once

#include "compilable.h"

class handleObject : public compilable {
public:
    [[nodiscard]] int getHandle() const {
        return this->handle;
    }
protected:
    int handle = -1;
};
