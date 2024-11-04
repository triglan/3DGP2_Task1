//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

class CShader;

class CGameObject
{
public:
	CGameObject(float x, float y, float z);
	CGameObject();
	virtual ~CGameObject();

public:
	XMFLOAT4X4						m_xmf4x4World;
	CMesh							*m_pMesh = NULL;
	CShader							*m_pShader = NULL;
	XMFLOAT3						m_xmf3Color = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//+쉴드 애니메이션
	void AnimateShield(XMFLOAT3 position, float fTimeElapsed);
	//+ 미사일 이동, 미사일 방향,  미사일 움직이는 방향 세팅, 미사일 애니메이션, 적미사일
	void Move(XMFLOAT3& vDirection, float fSpeed);
	void LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection);
	void AnimateMissile(float fTimeElapsed);
	void AnimateEnemyMissile(float fTimeElapsed);

	//+ 움직이는 방향 및 위치
	float x_pos, y_pos, z_pos;
	int move_direction;

	//+ 미사일 사용 여부, 미사일 방향, 미사일 최대 이동 거리, 딜레이
	bool isMissile = true;
	XMFLOAT3 MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	double moveDistance{};
	float EnemyMissileDelay{};
	float DefaultEnemyMissileDelay = 50;

	//+ 바운딩 박스
	BoundingOrientedBox m_OBB = BoundingOrientedBox();
	void UpdateBoundingBox();
	//+적기 생존 여부
	bool isAlive = true;
	float hp = 100;

	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);
	void SetMesh(CMesh *pMesh);
	void SetShader(CShader *pShader);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	void SetColor(XMFLOAT3 xmf3Color) { m_xmf3Color = xmf3Color; }

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
};

class CUfoObject : public CGameObject
{
public:
	CUfoObject();
	virtual ~CUfoObject();
};

