#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include"OpenGLINC.h"
#include<vector>
#include<glad/glad.h>
enum Direction
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
/*初始化必要参数*/
const float YAW = -90.0f;//偏航角需要看向z轴负半轴，所以一开始是-90.0度
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;//默认缩放值 即field of view(fov) 的初始值

class Camera
{
public:
	glm::vec3 Position;//摄像机自身的位置
	glm::vec3 Front;//摄像机正对的方向
	glm::vec3 Up;//摄像机自身的上轴
	glm::vec3 Right;//右轴
	glm::vec3 WorldUp;//指向上方向，一般是（0，1，0）

	float Yaw;
	float Pitch;

	float zoom;
	float camera_Speed;
	float camera_Sensitivity;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		camera_Speed(SPEED),
		camera_Sensitivity(SENSITIVITY),
		zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	};
    // constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), camera_Speed(SPEED), camera_Sensitivity(SENSITIVITY), zoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	};

	glm::mat4 getViewMatrix();
	void ProcessKeyBoard(Direction dir, float deltaTime);
	void ProcessMouseClick(float x_offset, float y_offset, GLboolean constraint=true);
	void ProcessScrollMouse(float y_offset);
private:
	void updateCameraVectors();
};

glm::mat4 Camera::getViewMatrix(){//主要功能函数，改变View矩阵
	return glm::lookAt(Position, Position + Front, Up);//变得是自身位置，front是不会变的，所以两者相加就是当前朝向的方向向量
	//参数1：摄像机位置 参数2：目标位置 参数3：世界上方向
}
void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	//摄像机位置（比如往上抬了点，往左偏了点）变了，自身的右轴和上轴也会变
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}
void Camera::ProcessKeyBoard(Direction dir, float deltaTime) {
	float velocity = deltaTime * camera_Speed;
	if (dir == FORWARD) {
		Position += Front*velocity;
	}
	if (dir == BACKWARD) {
		Position -= Front * velocity;
	}
	if (dir == RIGHT) {
		Position += Right * velocity;
	}
	if (dir == LEFT) {
		Position -= Right * velocity;
	}
}
void Camera::ProcessMouseClick(float x_offset, float y_offset, GLboolean constraint) {
	x_offset *= camera_Sensitivity;
	y_offset *= camera_Sensitivity;
	
	Yaw += x_offset;//偏航角受到鼠标的变换
	Pitch += y_offset;//俯仰角受到鼠标的变换

	if (constraint) {
		if (Pitch > 89.0f) {
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f) {
			Pitch = -89.0f;
		}
	}
	updateCameraVectors();
}
void Camera::ProcessScrollMouse(float y_offset) {//y_offset指鼠标滚轮的滚动偏移量
	zoom -= (float)y_offset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}
#endif
