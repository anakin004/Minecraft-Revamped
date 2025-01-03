#include "Camera.h"


const float GRAVITY = -9.81f                  * 2.7; // amplifying gravity
const float TERMINAL_VELOCITY = -50.0f;

//general up direction for projection stuff 
constexpr glm::vec3 g_Up(0.f, 1.f, 0.f);


Camera::Camera(glm::vec3 position, int width, int height, float fov, float nearPlane, float farPlane)
    : m_Width(width),
    m_Height(height),
    m_Position(position),
    m_MVP(glm::mat4(1.0f)),
    m_Orientation(0.0f, 0.0f, -1.0f),
    m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
    m_Yaw(-90.f),
    m_Pitch(0.f),
    m_Zoom(75.0f),
    m_Speed(5.0f),
    m_Sensitivity(0.5f)
{                                       // can do fov or zoom for this
    m_ProjectionMat = glm::perspective(glm::radians(m_Zoom), float(m_Width) / float(m_Height), nearPlane, farPlane);
    updateCameraVectors();
}

glm::mat4 Camera::CalcViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Orientation, g_Up);
}
#include <iostream>
void Camera::DispatchKeyboardEvent(MovementDir dir, float deltaTime)
{

    float velocity = m_Speed * deltaTime;

    switch (dir) {
        case FORWARD:      m_Position +=  m_Orientation * velocity; break;
        case BACK:         m_Position += -m_Orientation * velocity; break;
        case LEFT:         m_Position +=       -m_Right * velocity; break;
        case RIGHT:        m_Position +=        m_Right * velocity; break;
        case UP:           m_Position +=           g_Up * velocity; break;
        case DOWN:         m_Position +=          -g_Up * velocity; break;
        case NONE:                                                  break;
        
        default:                                                    break;

    }

    //std::cout << "dt: " << deltaTime << "\n";
}

// @param xrot -  x offset/rot from different between cur mouse x and prev mouse x
// @param yrot - y offset/rot from different between cur mouse y and prev mouse y
void Camera::DispatchMouseMoveEvent(float xrot, float yrot)
{

    //amplify rot based on sens
    xrot *= m_Sensitivity;
    yrot *= m_Sensitivity;

    // yaw is rot around y axis, so its x offset
    m_Yaw += xrot;
    m_Pitch += yrot;


    if (!(m_Pitch >= -85.0f && m_Pitch <= 85.0f)) {
        m_Pitch -= yrot;
    }
       

    std::cout << "Yaw: " << m_Yaw << "\tPitch: " << m_Pitch << "\n";


    updateCameraVectors();
}

void Camera::DispatchMouseScrollEvent(float scroll)
{
    m_Speed += scroll;
    
    // if we go too low
    m_Speed = (m_Speed < 0) ? 0 : m_Speed;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // so we can have a very high or very low yaw, wont keep cumulating, more controlled
    glm::clamp(m_Yaw, -180.f, 180.f);

    // multiply by cos m_Pitch for dampen - when looking very high or very low, x rot slower
    m_Orientation.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    // simply sin for y component
    m_Orientation.y = sin(glm::radians(m_Pitch));

    m_Orientation.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Orientation = glm::normalize(m_Orientation);
    
    // ori changed to we need to recalc up and right
    m_Right = glm::normalize(glm::cross(m_Orientation, g_Up));
    m_Up = glm::normalize(glm::cross(m_Right, m_Orientation));
}