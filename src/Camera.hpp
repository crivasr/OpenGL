#pragma once

#include "Utility/OpenGlHeaders.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
    private:
    GLFWwindow* m_window = nullptr;

    glm::mat4 m_projection{1.0f};
    glm::mat4 m_view{1.0f};

    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_rotation{0.0f, 0.0f, 0.0f};

    float m_yaw{-90.0f};
    float m_pitch{0.0f};

    float m_sensitivity{0.2f};

    float m_fov{45.0f};
    float m_width{1280.0f};
    float m_height{720.0f};

    float m_near{0.1f};
    float m_far{100.0f};

    float m_speed{0.05f};

    void processKeyboard();
    void processMouse();

    static constexpr glm::vec3 m_front{0.0f, 0.0f, 1.0f};
    static constexpr glm::vec3 m_up{0.0f, 1.0f, 0.0f};

    public:
    explicit Camera(GLFWwindow* window);

    Camera();
    void update();

    [[nodiscard]] glm::mat4 getView() const noexcept { return m_view; }
    [[nodiscard]] glm::mat4 getProjection() const noexcept { return m_projection; }
    [[nodiscard]] glm::vec3 getPosition() const { return m_position; };
    [[nodiscard]] glm::vec3 getRotation() const { return m_rotation; };
};