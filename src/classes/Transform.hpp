#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glm {
    static glm::mat4 Identity4{1.f};
    glm::mat4 shear(glm::mat4 Model, glm::vec2 values, glm::vec3 axis);
}

#endif // TRANSFORM_H_
