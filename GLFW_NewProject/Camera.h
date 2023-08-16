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
/*��ʼ����Ҫ����*/
const float YAW = -90.0f;//ƫ������Ҫ����z�Ḻ���ᣬ����һ��ʼ��-90.0��
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;//Ĭ������ֵ ��field of view(fov) �ĳ�ʼֵ

class Camera
{
public:
	glm::vec3 Position;//����������λ��
	glm::vec3 Front;//��������Եķ���
	glm::vec3 Up;//��������������
	glm::vec3 Right;//����
	glm::vec3 WorldUp;//ָ���Ϸ���һ���ǣ�0��1��0��

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

glm::mat4 Camera::getViewMatrix(){//��Ҫ���ܺ������ı�View����
	return glm::lookAt(Position, Position + Front, Up);//���������λ�ã�front�ǲ����ģ�����������Ӿ��ǵ�ǰ����ķ�������
	//����1�������λ�� ����2��Ŀ��λ�� ����3�������Ϸ���
}
void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	//�����λ�ã���������̧�˵㣬����ƫ�˵㣩���ˣ���������������Ҳ���
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
	
	Yaw += x_offset;//ƫ�����ܵ����ı任
	Pitch += y_offset;//�������ܵ����ı任

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
void Camera::ProcessScrollMouse(float y_offset) {//y_offsetָ�����ֵĹ���ƫ����
	zoom -= (float)y_offset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}
#endif
