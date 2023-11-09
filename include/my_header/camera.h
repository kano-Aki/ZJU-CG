#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum camera_movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const glm::vec3 FRONT=glm::vec3(0.0f,0.0f,-1.0f);//默认的相机方向向量的反方向

class Camera{
public:
    glm::vec3 cam_position;
    glm::vec3 cam_front;
    glm::vec3 cam_up;
    glm::vec3 cam_right;
    glm::vec3 world_up;
    float cam_yaw;
    float cam_pitch;
    float movement_speed;
    float mouse_sensitivity;
    float view_angle;

    Camera(glm::vec3 position=glm::vec3(0.0f,0.0f,0.0f),glm::vec3 up=glm::vec3(0.0f,1.0f,0.0f),float yaw=YAW,float pitch=PITCH);
    int keyboard_input(camera_movement direction,float deltatime);//wasd修改相机位置
    int mouse_input(float xoffset,float yoffset,bool constrict_pitch=true);//鼠标修改相机方向.参数表示是否要对俯仰角做范围限制
    int mouse_scroll(float yoffset);//鼠标滚轮修改视野大小
    glm::mat4 get_view_matrix();//获取相机的观察矩阵
    int update_camera_vectors();//更新相机的方向向量
};
//关于world up向量，实际是一个常量。(组略指向相机的+y方向)，gaze×world up，实际是world up垂直gaze的分量与其叉乘，这样子得到的
//便是相机的+x轴，然后再用+x轴×gaze(-z)得到真正的+y轴
#endif