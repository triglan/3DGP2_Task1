//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Shader.h"
#include <vector>
#include <chrono>

class CScene
{
public:
    CScene();
    ~CScene();

	//+플레이어 추가, 미사일 생성, 미사일-적기 충돌
	void InputPlayer(CPlayer* player);
	void CreateMissile();
	void CreateEnemyMissile(int i);
	void CheckObjectByBulletCollisions();
	void CheckPlayerByEnemyBulletCollisions();
	void Reset();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

	bool ProcessInput();
    void AnimateObjects(float fTimeElapsed);
	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);

	void ReleaseUploadBuffers();

	//+쉴드
	CGameObject* m_pShield{};
	CGameObject* m_pStartMenu{};

protected:
	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;

	CGameObject					**m_ppObjects = 0;
	int							m_nObjects = 0;

	//+
	CPlayer* m_pPlayer{};
	CGameObject** m_pMissile = 0;
	CGameObject** m_pEnemyMissile = 0;

	//+미사일 개수, 미사일 딜레이
	int m_nMissiles = 0;
	int m_nEnemyMissiles = 0;

	std::chrono::steady_clock::time_point gameStartTime;
	bool isGameStarted = false;
	bool isPlayerUntouchable = true;
public:
	//게임 시작, 스타트매뉴 메시
	bool isMainGame = false;
	float elapsedTime = 0;

};
