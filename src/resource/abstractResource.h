#pragma once

class abstractResource {
public:
    virtual void compile() = 0;
    virtual void discard() = 0;
};
