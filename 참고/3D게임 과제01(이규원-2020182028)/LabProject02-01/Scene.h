#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Player.h"

class CScene
{
public:
	CScene(CPlayer *pPlayer);
	virtual ~CScene();

	bool						isMainGame = false;
	float						MainGameTimer = 0.f;
	bool						isMainGameTimer = false;
	int							score = 0;
private:
	int							m_nObjects = 0;
	int							m_sObjects = 0;//+ Startmenu 오브젝트
	const int					startObjectsNum = 48;

	CGameObject					**m_ppObjects = NULL;
	CGameObject					**m_StartObjects = NULL;//+ Startmenu 오브젝트

	CWallsObject*				m_pWallsObject = NULL;

	CPlayer*					m_pPlayer = NULL;

#ifdef _WITH_DRAW_AXIS
	CGameObject*				m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByObjectCollisions();
	void CheckObjectByWallCollisions();
	void CheckPlayerByWallCollision();
	void CheckObjectByBulletCollisions();
	void CheckStartObjectByBulletCollisions();//+총알 스타트 메뉴 충돌
	void CheckShieldByBulletCollisions();//+총알 쉴드 충돌
	void CheckPlayerByBulletCollisions();//+적총알 아군기체 충돌

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);
};

