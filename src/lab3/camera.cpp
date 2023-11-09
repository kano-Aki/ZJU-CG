#include "my_header/camera.h"

Camera::Camera(glm::vec3 position,glm::vec3 up,float yaw,float pitch){
    cam_position=position;
    world_up=up;
    cam_yaw=yaw;
    cam_pitch=pitch;
    cam_front=FRONT;
    movement_speed=SPEED;
    mouse_sensitivity=SENSITIVITY;
    view_angle=ZOOM;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(cam_position,cam_position+cam_front,cam_up);
}

int Camera::keyboard_input(camera_movement direction,float deltatime)
{
    float velocity=movement_speed*deltatime;
    if(direction==FORWARD)
        cam_position+=cam_front*velocity;
    if(direction==BACKWARD)
        cam_position-=cam_front*velocity;
    if(direction==LEFT)
        cam_position-=cam_right*velocity;
    if(direction==RIGHT)
        cam_position+=cam_right*velocity;
    return 0;
}

int Camera::mouse_input(float xoffset,float yoffset,bool constrict_pitch)
{
    xoffset*=mouse_sensitivity;
    yoffset*=mouse_sensitivity;
    cam_yaw+=xoffset;
    cam_pitch+=yoffset;
    if(constrict_pitch)
    {
        if(cam_pitch>89.0f)
            cam_pitch=89.0f;
        if(cam_pitch<-89.0f)
            cam_pitch=-89.0f;
    }
    update_camera_vectors();
    return 0;
}

int Camera::update_camera_vectors()
{
    glm::vec3 front;
    front.x = sin(glm::radians(cam_yaw));
    front.y = cos(glm::radians(cam_yaw))*sin(glm::radians(cam_pitch));
    front.z = -cos(glm::radians(cam_yaw)) * cos(glm::radians(cam_pitch));
    cam_front = glm::normalize(front);

    cam_right=glm::normalize(glm::cross(cam_front,world_up));
    cam_up=glm::normalize(glm::cross(cam_right,cam_front));

    return 0;
}

int Camera::mouse_scroll(float yoffset)
{
    view_angle-=yoffset;
    if(view_angle<=1.0f)
        view_angle=1.0f;
    if(view_angle>=45.0f)
        view_angle=45.0f;

    return 0;
}