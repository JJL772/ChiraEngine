#pragma once

class resource {
public:
    virtual void compile() = 0;
    virtual void discard() = 0;
};
