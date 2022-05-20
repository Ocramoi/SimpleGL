#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/util.hpp"

class Camera {
    private:
        glm::vec3 anchor{0.f},
            position{0.f},
            dUp{0.f, 1.f, 0.f},
            up{dUp},
            dFront{0.f},
            front{dFront};
        float step,
            sensitivity{0.3f},
            dYaw{-90.f},
            yaw{dYaw},
            dPitch{0.f},
            pitch{dPitch};
        glm::mat4 transform{1.f};
        void updateTransform();
    public:
        Camera(const glm::vec3& init, float _step) : anchor(init),
                                                     position(init),
                                                     step(_step) {
            rotate({ 0, 0 });
        };

        void setSensitivity(float _sensitivity);
        float getSensitivity();
        void setStep(float _step);
        float getStep();
        void setYaw(float _yaw);
        float getYaw();
        void setPitch(float _pitch);
        float getPitch();
        void resetTransform();
        glm::mat4 getTransform();

        void rotate(const glm::vec2& _offset);
        void move(const glm::vec3& _direction);
        void setView(GLuint program);
};

#endif // CAMERA_H_
