//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Shader.h"
#include <vector>
#include <chrono>

#define MAX_LIGHTS			16 
#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

//struct LIGHT
//{
//	XMFLOAT4				m_xmf4Ambient;
//	XMFLOAT4				m_xmf4Diffuse;
//	XMFLOAT4				m_xmf4Specular;
//	XMFLOAT3				m_xmf3Position;
//	float 					m_fFalloff;
//	XMFLOAT3				m_xmf3Direction;
//	float 					m_fTheta; //cos(m_fTheta)
//	XMFLOAT3				m_xmf3Attenuation;
//	float					m_fPhi; //cos(m_fPhi)
//	bool					m_bEnable;
//	int						m_nType;
//	float					m_fRange;
//	float					padding;
//};
//
//struct LIGHTS
//{
//	LIGHT					m_pLights[MAX_LIGHTS];
//	XMFLOAT4				m_xmf4GlobalAmbient;
//	int						m_nLights;
//};

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

	//조명 관련 함수
	//void BuildDefaultLightsAndMaterials();
	//LIGHT* m_pLights = NULL;
	//int							m_nLights = 0;
	//XMFLOAT4					m_xmf4GlobalAmbient;
	//ID3D12Resource* m_pd3dcbLights = NULL;
	//LIGHTS* m_pcbMappedLights = NULL;

	//+쉴드, 메뉴, 지형
	CGameObject* m_pShield{};
	CGameObject* m_Terrain{};
	CGameObject* m_pStartMenu{};

	// 렌더 타겟 뷰 설정
	ID3D12Resource* m_pd3dRenderTargetResource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE m_pd3dRenderTargetView;
protected:
	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;

	CGameObject					**m_ppObjects = 0;
	int							m_nObjects = 0;

	//최대 적의 수
	int							MAXENEMY = 100;
	int							CurEnemy = 0;

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
