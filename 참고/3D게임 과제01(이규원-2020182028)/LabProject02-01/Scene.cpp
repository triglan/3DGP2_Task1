#include "stdafx.h"
#include "Scene.h"
#include "GraphicsPipeline.h"

CScene::CScene(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

CScene::~CScene()
{
}

void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 200.0f;
	CWallMesh* pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

	m_pWallsObject = new CWallsObject();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);
	m_pWallsObject->SetColor(RGB(0, 0, 0));
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);
	m_pWallsObject->m_xmOOBBPlayerMoveCheck = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.05f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CCubeMesh* pCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	CAirplaneMesh* pAirplainMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);
	//+ 적기
	m_nObjects = 6;
	m_ppObjects = new CGameObject * [m_nObjects];

	CExplosiveObject* pExplosiveObject;
	{
		pExplosiveObject = new CExplosiveObject();
		pExplosiveObject->SetMesh(pAirplainMesh);
		pExplosiveObject->SetColor(RGB(255, 0, 0));
		pExplosiveObject->SetPosition(20.f, 10.0f, 30.f);
		pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pExplosiveObject->SetRotationSpeed(0.0f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(10.0f, 0.0f, 0.0f));
		pExplosiveObject->SetMovingSpeed(-10.0f);
		pExplosiveObject->Rotate(-90.f, 0.f, 0.f);
		m_ppObjects[0] = pExplosiveObject;

		pExplosiveObject = new CExplosiveObject();
		pExplosiveObject->SetMesh(pAirplainMesh);
		pExplosiveObject->SetColor(RGB(0, 155, 155));
		pExplosiveObject->SetPosition(-18.f, 13.0f, 30.f);
		pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pExplosiveObject->SetRotationSpeed(0.0f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(10.0f, 0.0f, 0.0f));
		pExplosiveObject->SetMovingSpeed(13.0f);
		pExplosiveObject->Rotate(-90.f, 0.f, 0.f);
		m_ppObjects[1] = pExplosiveObject;


		pExplosiveObject = new CExplosiveObject();
		pExplosiveObject->SetMesh(pAirplainMesh);
		pExplosiveObject->SetColor(RGB(0, 255, 0));
		pExplosiveObject->SetPosition(-15.f, 0.0f, 30.f);
		pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pExplosiveObject->SetRotationSpeed(0.0f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(10.0f, 0.0f, 0.0f));
		pExplosiveObject->SetMovingSpeed(8.0f);
		pExplosiveObject->Rotate(-90.f, 0.f, 0.f);
		m_ppObjects[2] = pExplosiveObject;

		pExplosiveObject = new CExplosiveObject();
		pExplosiveObject->SetMesh(pAirplainMesh);
		pExplosiveObject->SetColor(RGB(0, 255, 255));
		pExplosiveObject->SetPosition(-7.f, 3.0f, 30.f);
		pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pExplosiveObject->SetRotationSpeed(0.0f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(00.0f, 1.0f, 0.0f));
		pExplosiveObject->SetMovingSpeed(8.0f);
		pExplosiveObject->Rotate(-90.f, 0.f, 0.f);
		m_ppObjects[3] = pExplosiveObject;


		pExplosiveObject = new CExplosiveObject();
		pExplosiveObject->SetMesh(pAirplainMesh);
		pExplosiveObject->SetColor(RGB(255, 0, 255));
		pExplosiveObject->SetPosition(-10.f, -10.0f, 30.f);
		pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pExplosiveObject->SetRotationSpeed(0.0f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(00.0f, 1.0f, 1.0f));
		pExplosiveObject->SetMovingSpeed(13.0f);
		pExplosiveObject->Rotate(-90.f, 0.f, 0.f);
		m_ppObjects[4] = pExplosiveObject;

		pExplosiveObject = new CExplosiveObject();
		pExplosiveObject->SetMesh(pAirplainMesh);
		pExplosiveObject->SetColor(RGB(255, 255, 0));
		pExplosiveObject->SetPosition(-10.f, -10.0f, 30.f);
		pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pExplosiveObject->SetRotationSpeed(0.0f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(10.0f, 0.0f, 0.0f));
		pExplosiveObject->SetMovingSpeed(13.0f);
		pExplosiveObject->Rotate(-90.f, 0.f, 0.f);
		m_ppObjects[5] = pExplosiveObject;
	}
	
	//+ startMenu
	m_sObjects = startObjectsNum;
	m_StartObjects = new CGameObject * [m_sObjects];

	
	CExplosiveObject* pStartObject;
	for (int i = 0; i < startObjectsNum; i++) {
		pStartObject = new CExplosiveObject();
		pStartObject->SetMesh(pCubeMesh);
		pStartObject->SetColor(RGB(255, 0, 0));
		pStartObject->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
		pStartObject->SetRotationSpeed(60.0f);
		pStartObject->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pStartObject->SetMovingSpeed(0.0f);
		pStartObject->Rotate(-90.f, 0.f, 0.f);
		switch (i)
		{
		//+S
		case 0:
			pStartObject->SetPosition(-36.f, 8.0f, 50.f);
			break;
		case 1:
			pStartObject->SetPosition(-32.f, 8.0f, 50.f);
			break;
		case 2:
			pStartObject->SetPosition(-28.f, 8.0f, 50.f);
			break;
		case 3:
			pStartObject->SetPosition(-36.f, 4.0f, 50.f);
			break;
		case 4:
			pStartObject->SetPosition(-36.f, 0.0f, 50.f);
			break;
		case 5:
			pStartObject->SetPosition(-32.f, 0.0f, 50.f);
			break;
		case 6:
			pStartObject->SetPosition(-28.f, 0.0f, 50.f);
			break;
		case 7:
			pStartObject->SetPosition(-28.f, -4.0f, 50.f);
			break;
		case 8:
			pStartObject->SetPosition(-28.f, -8.0f, 50.f);
			break;
		case 9:
			pStartObject->SetPosition(-32.f, -8.0f, 50.f);
			break;
		case 10:
			pStartObject->SetPosition(-36.f, -8.0f, 50.f);
			break;
		//+T
		case 11:
			pStartObject->SetPosition(-20.f, 8.0f, 50.f);
			break;
		case 12:
			pStartObject->SetPosition(-16.f, 8.0f, 50.f);
			break;
		case 13:
			pStartObject->SetPosition(-12.f, 8.0f, 50.f);
			break;
		case 14:
			pStartObject->SetPosition(-16.f, 4.0f, 50.f);
			break;
		case 15:
			pStartObject->SetPosition(-16.f, 0.0f, 50.f);
			break;
		case 16:
			pStartObject->SetPosition(-16.f, -4.0f, 50.f);
			break;
		case 17:
			pStartObject->SetPosition(-16.f, -8.0f, 50.f);
			break;
		//+A
		case 18:
			pStartObject->SetPosition(-0.f, 8.0f, 50.f);
			break;
		case 19:
			pStartObject->SetPosition(-4.f, 4.0f, 50.f);
			break;
		case 20:
			pStartObject->SetPosition(-0.f, 4.0f, 50.f);
			break;
		case 21:
			pStartObject->SetPosition(4.f, 4.0f, 50.f);
			break;
		case 22:
			pStartObject->SetPosition(-4.f, 0.0f, 50.f);
			break;
		case 23:
			pStartObject->SetPosition(4.f, 0.0f, 50.f);
			break;
		case 24:
			pStartObject->SetPosition(-4.f, -4.0f, 50.f);
			break;
		case 25:
			pStartObject->SetPosition(-0.f, -4.0f, 50.f);
			break;
		case 26:
			pStartObject->SetPosition(4.f, -4.0f, 50.f);
			break;
		case 27:
			pStartObject->SetPosition(-4.f, -8.0f, 50.f);
			break;
		case 28:
			pStartObject->SetPosition(4.f, -8.0f, 50.f);
			break;
		//+R
		case 29:
			pStartObject->SetPosition(12.f, 8.0f, 50.f);
			break;
		case 30:
			pStartObject->SetPosition(16.f, 8.0f, 50.f);
			break;
		case 31:
			pStartObject->SetPosition(16.f, -4.0f, 50.f);
			break;
		case 32:
			pStartObject->SetPosition(12.f, 4.0f, 50.f);
			break;
		case 33:
			pStartObject->SetPosition(20.f, 4.0f, 50.f);
			break;
		case 34:
			pStartObject->SetPosition(12.f, 0.0f, 50.f);
			break;
		case 35:
			pStartObject->SetPosition(16.f, 0.0f, 50.f);
			break;
		case 36:
			pStartObject->SetPosition(20.f, 0.0f, 50.f);
			break;
		case 37:
			pStartObject->SetPosition(12.f, -4.0f, 50.f);
			break;
		case 38:
			pStartObject->SetPosition(20.f, -8.0f, 50.f);
			break;
		case 39:
			pStartObject->SetPosition(12.f, -8.0f, 50.f);
			break;
		case 40:
			pStartObject->SetPosition(20.f, -8.0f, 50.f);
			break;

			//+T
		case 41:
			pStartObject->SetPosition(28.f, 8.0f, 50.f);
			break;
		case 42:
			pStartObject->SetPosition(32.f, 8.0f, 50.f);
			break;
		case 43:
			pStartObject->SetPosition(36.f, 8.0f, 50.f);
			break;
		case 44:
			pStartObject->SetPosition(32.f, 4.0f, 50.f);
			break;
		case 45:
			pStartObject->SetPosition(32.f, 0.0f, 50.f);
			break;
		case 46:
			pStartObject->SetPosition(32.f, -4.0f, 50.f);
			break;
		case 47:
			pStartObject->SetPosition(32.f, -8.0f, 50.f);
			break;
		default:
			break;
		}

		m_StartObjects[i] = pStartObject;
	}


#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) if (m_ppObjects[i]) delete m_ppObjects[i];

	if (m_ppObjects) delete[] m_ppObjects;

	if (m_pWallsObject) delete m_pWallsObject;

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[int(wParam - '1')];
			pExplosiveObject->m_bBlowingUp = true;
			break;
		}
		case 'A':
			for (int i = 0; i < m_nObjects; i++)
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject* pNearestObject = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = m_ppObjects[i];
		}
	}
	return(pNearestObject);
}

void CScene::CheckObjectByObjectCollisions()
{
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->m_pObjectCollided = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}

void CScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case CONTAINS:
			break;
		}
	}
}

void CScene::CheckPlayerByWallCollision()
{
	BoundingOrientedBox xmOOBBPlayerMoveCheck;
	m_pWallsObject->m_xmOOBBPlayerMoveCheck.Transform(xmOOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->m_xmf4x4World));
	XMStoreFloat4(&xmOOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOOBBPlayerMoveCheck.Orientation)));

	if (!xmOOBBPlayerMoveCheck.Intersects(m_pPlayer->m_xmOOBB)) m_pWallsObject->SetPosition(m_pPlayer->m_xmf3Position);
}

void CScene::CheckObjectByBulletCollisions()
{
	CBulletObject** ppBullets = ((CAirplanePlayer*)m_pPlayer)->m_ppBullets;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_ppObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
				ppBullets[j]->Reset();
				score += 10;
					
				CAirplanePlayer* pPlayer = ((CAirplanePlayer*)m_pPlayer);
				pPlayer->SetColor(RGB(0, score, 255 - score));
			}
		}
	}
}

void CScene::CheckStartObjectByBulletCollisions()//+스타트 오브젝트랑 총알 충돌
{
	CBulletObject** ppBullets = ((CAirplanePlayer*)m_pPlayer)->m_ppBullets;
	for (int i = 0; i < m_sObjects; i++)
	{
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_StartObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_StartObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
				ppBullets[j]->Reset();
				isMainGameTimer = true;
			}
		}
	}
}

void CScene::CheckShieldByBulletCollisions() {//+적총알 쉴드 충돌
	
	CAirplanePlayer* pPlayer = ((CAirplanePlayer*)m_pPlayer);
	CGameObject* pShield = pPlayer->m_Shields;

	for (int i = 0; i < m_nObjects; i++)
	{
		CBulletObject** ppBullets = ((CExplosiveObject*)m_ppObjects[i])->m_ppBullets;
		for (int j = 0; j < ENEMYBULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && pShield->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				ppBullets[j]->Reset();
				std::cout << "쉴드와 충돌함" << std::endl;
			}
		}
	}
}

void CScene::CheckPlayerByBulletCollisions()
{
	CAirplanePlayer* pPlayer = ((CAirplanePlayer*)m_pPlayer);
	for (int i = 0; i < m_nObjects; i++)
	{
		CBulletObject** ppBullets = ((CExplosiveObject*)m_ppObjects[i])->m_ppBullets;
		for (int j = 0; j < ENEMYBULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && pPlayer->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				ppBullets[j]->Reset();
				std::cout << "플레이어가 맞음" << std::endl;
				isMainGame = false;
				MainGameTimer = 0.f;
				isMainGameTimer = false;
			}
		}
	}
}

void CScene::Animate(float fElapsedTime)
{
	if (isMainGame) {//+메인게임 시작시
		m_pWallsObject->Animate(fElapsedTime);

		for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);

		CheckPlayerByWallCollision();

		CheckObjectByWallCollisions();

		CheckObjectByObjectCollisions();

		CheckObjectByBulletCollisions();
		
		if(((CAirplanePlayer*)m_pPlayer)->isShield)//+ 쉴드가 작동중일 때
			CheckShieldByBulletCollisions();//+ 적총알 아군쉴드 충돌처리

		CheckPlayerByBulletCollisions();

		for (int i = 0; i < m_nObjects; ++i)
			((CExplosiveObject*)m_ppObjects[i])->FireBullet();///+Scene애님 업데이트
	}
	else {
		CheckStartObjectByBulletCollisions();
		for (int i = 0; i < m_sObjects; i++) 
			m_StartObjects[i]->Animate(fElapsedTime);
		if (isMainGameTimer)
			MainGameTimer += fElapsedTime;//+ 쉴드 쿨타임 계산
		std::cout << MainGameTimer << std::endl;
		if (MainGameTimer > 1.f)
			isMainGame = true;
	}
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);

	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);
	if (isMainGame) {//+메인게임 시작시
		m_pWallsObject->Render(hDCFrameBuffer, pCamera);
		for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	}else {
		for (int i = 0; i < m_sObjects; i++) 
			m_StartObjects[i]->Render(hDCFrameBuffer, pCamera);
	}
	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);

	//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}
