#include "Transform.hpp"

#include <math.h>

glm::mat4 glm::shear(glm::mat4 Model, glm::vec2 vals, glm::vec3 axis) {
    return Model * glm::mat4{
        1, axis.y ? vals.x : 0, axis.z ? vals.x : 0, 0,
        axis.x ? vals.x : 0, 1, axis.z ? vals.y : 0, 0,
        axis.x ? vals.y : 0, axis.y ? vals.y : 0, 1, 0,
        0, 0, 0, 1
    };
}
