#pragma once

#include "glad/gl.h"

struct position {
    GLfloat x, y, z;
    explicit position(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
};

bool operator==(const position& pos1, const position& pos2);

struct normal {
    GLfloat r, g, b;
    explicit normal(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0);
};

bool operator==(const normal& norm1, const normal& norm2);

struct color {
    GLfloat r, g, b;
    explicit color(GLfloat r = 1, GLfloat g = 1, GLfloat b = 1);
};

bool operator==(const color& col1, const color& col2);

struct uv {
    GLfloat u, v;
    explicit uv(GLfloat u = 0, GLfloat v = 0);
};

bool operator==(const uv& uv1, const uv& uv2);

struct vertex {
    position pos;
    normal norm;
    color col;
    uv uvMap;
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z);
    vertex(const struct position& pos, const struct normal& norm, const struct color& col, const struct uv& tex);
    vertex();
};

bool operator==(const vertex& v1, const vertex& v2);
