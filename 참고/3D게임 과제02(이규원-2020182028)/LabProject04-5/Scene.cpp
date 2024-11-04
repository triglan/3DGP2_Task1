//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "Player.h"

#include <iostream>

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::InputPlayer(CPlayer* player) {
	m_pPlayer = player;
	m_pPlayer->hp = 100;
}

//#define _WITH_TEXT_MODEL_FILE
#define _WITH_BINARY_MODEL_FILE

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

#ifdef _WITH_TEXT_MODEL_FILE
	CMesh* pEnemyMesh = new CMesh(pd3dDevice, pd3dCommandList, "Models/enemy.txt", true);
#endif
#ifdef _WITH_BINARY_MODEL_FILE
	CMesh* pEnemyMesh = new CMesh(pd3dDevice, pd3dCommandList, "Models/enemy.bin", false);
	CMesh* pShieldMesh = new CMesh(pd3dDevice, pd3dCommandList, "Models/shield.txt", true);
	CMesh* pMissileMesh = new CMesh(pd3dDevice, pd3dCommandList, "Models/missile.txt", true);
	CMesh* pStartMenuMesh = new CMesh(pd3dDevice, pd3dCommandList, "Models//start_menu.txt", true);
#endif

	//CMesh* pApacheModel = CMesh::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Apache.bin");
	//CApacheObject* pApacheObject = NULL;

	m_nObjects = 4;
	m_ppObjects = new CGameObject * [m_nObjects];

	CPseudoLightingShader* pShader = new CPseudoLightingShader();
	pShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	pShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//적기
	{
		m_ppObjects[0] = new CGameObject(-10.0f, 3.0f, 20.0f);
		m_ppObjects[0]->SetMesh(pEnemyMesh);
		m_ppObjects[0]->SetShader(pShader);
		m_ppObjects[0]->SetColor(XMFLOAT3(1.f, 0.0f, 0.0f));
		m_ppObjects[0]->Rotate(0.0, 180.0f, 0.0);

		m_ppObjects[1] = new CGameObject(10.0f, -3.0f, 20.0f);
		m_ppObjects[1]->SetMesh(pEnemyMesh);
		m_ppObjects[1]->SetShader(pShader);
		m_ppObjects[1]->SetColor(XMFLOAT3(1.f, 0.0f, 0.0f));
		m_ppObjects[1]->Rotate(0.0, 180.0f, 0.0);

		m_ppObjects[2] = new CGameObject(1.0f, -6.0f, 20.0f);
		m_ppObjects[2]->SetMesh(pEnemyMesh);
		m_ppObjects[2]->SetShader(pShader);
		m_ppObjects[2]->SetColor(XMFLOAT3(1.f, 0.0f, 0.0f));
		m_ppObjects[2]->Rotate(0.0, 180.0f, 0.0);

		m_ppObjects[3] = new CGameObject(-1.0f, 6.0f, 20.0f);
		m_ppObjects[3]->SetMesh(pEnemyMesh);
		m_ppObjects[3]->SetShader(pShader);
		m_ppObjects[3]->SetColor(XMFLOAT3(1.f, 0.0f, 0.0f));
		m_ppObjects[3]->Rotate(0.0, 180.0f, 0.0);
	}

	//+쉴드
	{
		m_pShield = new CGameObject(0.0, 0.0, 0.0);
		m_pShield->SetMesh(pShieldMesh);
		m_pShield->SetShader(pShader);
		m_pShield->SetColor(XMFLOAT3(0.0, 0.0, 1.0));
		m_pShield->SetScale(1.45f, 1.45f, 1.45f);//+ CGameObject에 SetScale 함수 만들어서 메쉬 크기 늘림
	}
	
	//+미사일
	{
		m_nMissiles = 100;
		m_pMissile = new CGameObject * [m_nMissiles];

		for (int i = 0; i < m_nMissiles; ++i) {
			m_pMissile[i] = new CGameObject(0.0, 100.0, 0.0);
			m_pMissile[i]->SetMesh(pMissileMesh);
			m_pMissile[i]->SetShader(pShader);
			m_pMissile[i]->SetColor(XMFLOAT3(0.1, 0.1, 0.7));
		}
	}
	//적미사일
	{
		m_nEnemyMissiles = 100;
		m_pEnemyMissile = new CGameObject * [m_nEnemyMissiles];

		for (int i = 0; i < m_nMissiles; ++i) {
			m_pEnemyMissile[i] = new CGameObject(0.0, 100.0, 0.0);
			m_pEnemyMissile[i]->SetMesh(pMissileMesh);
			m_pEnemyMissile[i]->SetShader(pShader);
			m_pEnemyMissile[i]->SetColor(XMFLOAT3(0.8, 0.2, 0.2));
		}
	}
	//+시작화면
	{
		m_pStartMenu = new CGameObject(0.0, 0.0, 0.0);
		m_pStartMenu->SetMesh(pStartMenuMesh);
		m_pStartMenu->SetShader(pShader);
		m_pStartMenu->SetColor(XMFLOAT3(0.0, 0.0, 0.0));
		m_pStartMenu->SetPosition(XMFLOAT3(0.0, 3.0, -4.0));
		m_pStartMenu->Rotate(0.0, 180.0, 0.0);
	}
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[3];
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[0].Constants.Num32BitValues = 4; //Time, ElapsedTime, xCursor, yCursor
	pd3dRootParameters[0].Constants.ShaderRegister = 0; //Time
	pd3dRootParameters[0].Constants.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 19; //16 + 3
	pd3dRootParameters[1].Constants.ShaderRegister = 1; //World
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[2].Constants.Num32BitValues = 35; //16 + 16 + 3
	pd3dRootParameters[2].Constants.ShaderRegister = 2; //Camera
	pd3dRootParameters[2].Constants.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}

void CScene::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
	}
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput()
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	if(isMainGame)
		elapsedTime += fTimeElapsed;

	if (elapsedTime > 1) {
		isPlayerUntouchable = false;
	}
	cout << elapsedTime << endl;

	if (isMainGame) {
		//적기 애니메이션
		for (int j = 0; j < m_nObjects; j++)
		{
			m_ppObjects[j]->x_pos += fTimeElapsed * m_ppObjects[j]->move_direction * 10;

			if (m_ppObjects[j]->x_pos > 20.0 || m_ppObjects[j]->x_pos < -20.0)
				m_ppObjects[j]->move_direction *= -1;

			m_ppObjects[j]->LookAt(m_pPlayer->GetPosition(), m_pPlayer->GetUp());
			m_ppObjects[j]->SetPosition(m_ppObjects[j]->x_pos, m_ppObjects[j]->y_pos, m_ppObjects[j]->z_pos);

			//적미사일 생성
			m_ppObjects[j]->Animate(fTimeElapsed);

			if (m_ppObjects[j]->EnemyMissileDelay <= 0)
				CreateEnemyMissile(j);

			m_ppObjects[j]->UpdateBoundingBox();
		}

		//+쉴드 애니메이션
		m_pShield->AnimateShield(m_pPlayer->GetPosition(), fTimeElapsed);

		//+미사일 애니메이션
		for (int i = 0; i < m_nMissiles; ++i) {
			if (m_pMissile[i]->isMissile) {
				m_pMissile[i]->AnimateMissile(fTimeElapsed);
				m_pMissile[i]->UpdateBoundingBox();
			}
		}

		//+적미사일 애니메이션
		for (int i = 0; i < m_nEnemyMissiles; ++i) {
			if (m_pEnemyMissile[i]->isMissile) {
				m_pEnemyMissile[i]->AnimateEnemyMissile(fTimeElapsed);
				m_pEnemyMissile[i]->UpdateBoundingBox();
			}
		}

		CheckObjectByBulletCollisions();
		CheckPlayerByEnemyBulletCollisions();
	}
}

void CScene::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	if (isMainGame) {
		for (int j = 0; j < m_nObjects; j++)
		{
			if (m_ppObjects[j] && m_ppObjects[j]->isAlive) m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}

		//+쉴드 렌더링
		if (m_pPlayer->isShield)
			m_pShield->Render(pd3dCommandList, pCamera);

		//+미사일 렌더링
		for (int i = 0; i < m_nMissiles; ++i) {
			if (m_pMissile[i]->isMissile)
				m_pMissile[i]->Render(pd3dCommandList, pCamera);
		}
		//+적미사일 렌더링
		for (int i = 0; i < m_nEnemyMissiles; ++i) {
			if (m_pEnemyMissile[i]->isMissile)
				m_pEnemyMissile[i]->Render(pd3dCommandList, pCamera);
		}
	}else
		m_pStartMenu->Render(pd3dCommandList, pCamera);
}

//+플레이어 미사일 생성
void CScene::CreateMissile() {
	for (int i = 0; i < m_nMissiles; ++i) {
		if (!m_pMissile[i]->isMissile) {
			XMFLOAT3 xmf3Up = m_pPlayer->GetUp();
			XMFLOAT3 xmf3Direction = m_pPlayer->GetLook();
			XMFLOAT3 xmf3Position = m_pPlayer->GetPosition();
			XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, -4.0f, false));

			m_pMissile[i]->m_xmf4x4World = Matrix4x4::Identity();

			m_pMissile[i]->LookAt(xmf3Direction, xmf3Up);
			m_pMissile[i]->SetPosition(xmf3FirePosition);
			m_pMissile[i]->SetMovingDirection(xmf3Direction);

			m_pMissile[i]->moveDistance = 0;
			m_pMissile[i]->isMissile = true;
			break;
		}
	}
}
//적미사일 생성
void CScene::CreateEnemyMissile(int i) {
	for (int j = 0; j < m_nEnemyMissiles; ++j) {
		if (!m_pEnemyMissile[j]->isMissile) {
			XMFLOAT3 xmf3Up = m_ppObjects[i]->GetUp();
			XMFLOAT3 xmf3Direction = m_ppObjects[i]->GetLook();
			XMFLOAT3 xmf3Position = m_ppObjects[i]->GetPosition();
			XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, -4.0f, false));

			m_pEnemyMissile[j]->m_xmf4x4World = Matrix4x4::Identity();

			m_pEnemyMissile[j]->LookAt(xmf3Direction, xmf3Up);
			m_pEnemyMissile[j]->SetPosition(xmf3FirePosition);
			m_pEnemyMissile[j]->SetMovingDirection(xmf3Direction);

			m_pEnemyMissile[j]->moveDistance = 0;
			m_pEnemyMissile[j]->isMissile = true;
			break;
		}
	}

	m_ppObjects[i]->EnemyMissileDelay = m_ppObjects[i]->DefaultEnemyMissileDelay;
}

//+미사일 적기 충돌
void CScene::CheckObjectByBulletCollisions() {
	for (int i = 0; i < m_nMissiles; ++i) {
		for (int j = 0; j < m_nObjects; ++j) {
			if (m_pMissile[i]->isMissile && m_ppObjects[j]->m_OBB.Intersects(m_pMissile[i]->m_OBB)) {
				m_pMissile[i]->isMissile = false;
				m_ppObjects[j]->hp -= 10;
				m_ppObjects[j]->SetColor(XMFLOAT3(m_ppObjects[j]->hp / 100, 0, 0));
				if (m_ppObjects[j]->hp <= 0)
					m_ppObjects[j]->isAlive = false;
			}
		}
	}
}

//+ 적기 미사일 - 플레이어 충돌
void CScene::CheckPlayerByEnemyBulletCollisions() {
	if (isPlayerUntouchable) return;//+ 무적일땐 건들지 마라


	for (int i = 0; i < m_nEnemyMissiles; ++i) {
		if (m_pEnemyMissile[i]->isMissile && m_pPlayer->m_OBB.Intersects(m_pEnemyMissile[i]->m_OBB)) {
			m_pEnemyMissile[i]->isMissile = false;//미사일 월드에서 제외

			if (m_pPlayer->isShield) {
				
			}
			else {
				m_pPlayer->hp -= 10;
				std::cout << m_pPlayer->hp << endl;
				// 체력이 떨어질 수록 빨간색으로 변한다, 다떨어지면 게임종료
				m_pPlayer->SetColor(XMFLOAT3(1 - m_pPlayer->hp / 100.f, 0.25f, 0.875f));
				if (m_pPlayer->hp <= 0)
					::PostQuitMessage(0);
			}

		}
	}
}


void CScene::Reset()
{
}

