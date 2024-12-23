#pragma once
#include <tchar.h>

// 카메라 모드 설정
// 카메라 클래스 멤버 함수 Update(float FT)에서 실행할 때 사용하는 열거형 클래스이다. 카메라 클래스에서 자세히 설명하도록 한다.
enum class CamMode{ TRACK_MODE, SPECTOR_MODE };

// 프로그램 시작 시 사용할 윈도우 사이즈를 설정한다.
// START_WITH_FULL_SCREEN 옵션 활성화 시 모니터 해상도로 실행된다.
constexpr int START_UP_WIDTH = 1300;
constexpr int START_UP_HEIGHT = 800;

// 풀스크린 모드로 시작할 지를 설정한다.
constexpr bool START_WITH_FULL_SCREEN = false;

// 최대 프레임 제한을 설정한다. 0으로 설정 시 최대 프레임으로 동작한다.
constexpr float FRAME_LIMITS = 144;

// 바운드박스 표시 여부를 설정한다.
#define SHOW_BOUND_BOX

// 게임 월드에 사용할 레이어를 설정한다. 끝 부분에는 반드시 END표기를 해야한다.
// UI의 경우 가장 최상단 레이어에 추가하는 것을 권장한다.
enum LayerEnum
{ LAYER_1, LAYER_2, LAYER_3, END };

// Scene 컨테이너 리저브 사이즈 지정
constexpr int OBJECT_LIST_RESERVE = 500;

// 제목표시줄 문자열을 설정한다. 반드시 아래와 같은 형식으로 저정해야 한다.
constexpr TCHAR TitleName[] = _T("Objective-D(");