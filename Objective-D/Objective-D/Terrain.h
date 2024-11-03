#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "GameResource.h"
#include "TransformUtil.h"
#include "TerrainUtil.h"


class Terrain : public GameObject {
public:
	Terrain() {
		// �ͷ��� ������ �ͷ��� �浹ó�� ��ƿ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, 0.0, -50.0, 70.0);
		Transform::Scale(ScaleMatrix, 40.0, 30.0, 40.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}

	void Render(CommandList CmdList) {
		// �ͷ����� ���� ������Ʈ�̹Ƿ� ��� �ʱ�ȭ �� ��ȯ�� �������� �ʴ´�.
		EnableLight(CmdList);
		FlipTexture(CmdList, false);
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);

		BindTexture(CmdList, TerrainTex);
		UseShader(CmdList, ObjectShader, true);
		RenderMesh(CmdList, TerrainMesh, TerrainTex, ObjectShader, 1.0f, true);

		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}
};