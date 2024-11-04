#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

#include "LineBrush.h"
#include "Mode0.h"

class Control : public GameObject {
private:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	float WingRotation{ 90 };
	XMFLOAT3 HeliRotation{ 0, -90, 0};
	XMFLOAT3 Tilt{0, 0, 90};

public:
	XMFLOAT3 Position{0.0, 0.0, 0.0};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };
	float GunAlpha = 1.0;

	bool UseLight{};

	LineBrush line;

	OOBB oobb;
	Range range;

	Vector Vec{};

	Control() {
		Math::InitVector(Vec);
		line.SetColor(1.0, 1.0, 1.0);
		Position.x = -8, Position.z = -15.0;
	}

	XMFLOAT3 GetUp() {
		return Vec.Up;
	}

	XMFLOAT3 GetPosition() {
		return Position;
	}

	void InputMouseMotion(HWND hwnd, POINT MotionPosition) {
		if (GetCapture() == hwnd) {
			mouse.HideCursor();
			GetCapture();

			float cxDelta = (float)(mouse.CurrentPosition().x - MotionPosition.x) / 5.0f;
			float cyDelta = (float)(mouse.CurrentPosition().y - MotionPosition.y) / 5.0f;
			mouse.UpdateMotionPosition(MotionPosition);
			
			// 카메라 회전 업데이트
			RotationDest.x += cyDelta * 0.01;
			RotationDest.y += cxDelta * 0.01;

			//// 헬기 회전 업데이트
			HeliRotation.x += cyDelta * 0.01;
			HeliRotation.y += cyDelta * 0.01;

			//HeliRotation.y += cxDelta * 0.1f;
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_DOWN:
				if (GunAlpha > 0.0)
					GunAlpha -= 0.1;
				break;

			case VK_UP:
				if (GunAlpha < 1.0)
					GunAlpha += 0.1;
				break;

			case VK_SPACE:
				if (!UseLight)
					UseLight = true;
				else
					UseLight = false;
				break;
			case 'W': MoveForward = true; break;
			case 'A': MoveLeft = true; break;
			case 'S': MoveBackward = true; break;
			case 'D': MoveRight = true; break;
			case VK_ESCAPE:// 탈출 누를 시 플레이 모드로 전환
				scene.SwitchMode(HomeMode::Start);
				break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {
			case 'W': MoveForward = false; break;
			case 'A': MoveLeft = false; break;
			case 'S': MoveBackward = false; break;
			case 'D': MoveRight = false; break;
			}
			break;
		}
	}

	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_LBUTTONDOWN:
			mouse.StartMotionCapture(hWnd);
			break;

		case WM_LBUTTONUP:
			mouse.EndMotionCapture();
			break;
		}
	}

	void Update(float FT) {
		//카메라 이동
		if (MoveForward) {
			camera.MoveForwardWithoutHeight(FT * 40);
			Position.z += FT * 40;
		}
		if (MoveBackward) {
			camera.MoveForwardWithoutHeight(-FT * 40);
			Position.z -= FT * 40;
		}
		if (MoveRight) {
			camera.MoveStrafeWithoutHeight(FT * 40);
			Position.x += FT * 40;
		}
		if (MoveLeft) {
			camera.MoveStrafeWithoutHeight(-FT * 40);
			Position.x -= FT * 40;
		}

		//카메라 회전
		Rotation.x = std::lerp(Rotation.x, RotationDest.x, FT * 15);
		Rotation.y = std::lerp(Rotation.y, RotationDest.y, FT * 15);

		
		//카메라
		//camera.Move(Position.x - 8, Position.y, Position.z - 20);
		//camera.Rotate(Rotation.x, Rotation.y, 0.0);

		//날개 회전
		WingRotation += FT * 2000;

		std::cout << Position.x << Position.y << Position.z << std::endl;
		std::cout << camera.GetPosition().x << camera.GetPosition().y << camera.GetPosition().z << std::endl << std::endl;
	}

	void Render(CommandList CmdList) {
		//헬기 몸통
		InitMatrix(CmdList, RENDER_TYPE_PERS);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);

		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);

		FlipTexture(CmdList, FLIP_TYPE_H);
		BindTexture(CmdList, HelicopterTex);
		RenderMesh(CmdList, HelicopterBodyMesh, HelicopterTex, ObjectShader, GunAlpha);

		//헬기 머리
		Transform::Rotate(TranslateMatrix, 0.0, 0, 90.0);
		Transform::Move(TranslateMatrix, 0.0, -1.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		RenderMesh(CmdList, HelicopterHeadMesh, HelicopterTex, ObjectShader, GunAlpha);



		// 바운드 스페어 출력
		//range.Update(Position, 0.3);
		//range.Render(CmdList);

		// 이미지 출력
		//InitMatrix(CmdList, RENDER_TYPE_IMAGE);
		//Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		//Transform::Move(TranslateMatrix, -0.5, -0.5, 0.0);
		//RenderMesh(CmdList, ImagePannel, WoodTex, ObjectShader);

		// 중앙선 출력
		//line.Draw(CmdList, 0.0, 0.0, mouse.x , mouse.y, 0.01);


	}
};