#include "GameResource.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Scene::Init()에서 실행된다.

////////////////////////////////
BasicObjectShader* ObjectShader;
BasicObjectShader* BoundboxShader;
Texture* LineTex, * GuideTex;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;
////////////////////////////////

Mesh* HelicopterBodyMesh, * HelicopterHeadMesh;
Mesh* TerrainMesh;
Texture* HelicopterTex, * WoodTex, * SkyboxTex, * TerrainTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 파이프라인 생성이 곧 쉐이더 설정의 마무리이다.
	// 일반 렌더링 쉐이더 생성
	ObjectShader = new BasicObjectShader();
	ObjectShader->CreateDefaultPipeline(Device, RootSignature);

	// 깊이 검사 미포함 파이프라인 생성
	ObjectShader->CreateImageDepthPipelineState(Device, RootSignature);

	// 바운드박스 쉐이더 생성
	BoundboxShader = new BasicObjectShader();
	BoundboxShader->CreateBoundboxPipeline(Device, RootSignature);
	////////////////////////////////
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 이미지 출력용 패널 생성
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);

	// 스카이박스 출력용 매쉬 생성
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	// 바운드박스 출력용 매쉬 생성
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);

	// 바운드스페어 출력용 매쉬 생성
	BoundingSphereMesh = new Mesh(Device, CmdList, "Resources//SystemResources//Models//BoundingSphereMesh.txt", MESH_TYPE_TEXT);
	////////////////////////////////

	// 헬리콥터
	HelicopterBodyMesh = new Mesh(Device, CmdList, "Resources//Models//GunShip.bin", MESH_TYPE_BIN);
	HelicopterHeadMesh = new Mesh(Device, CmdList, "Resources//Models//Rotor.bin", MESH_TYPE_BIN);

	// 지형
	TerrainMesh = new Mesh(Device, CmdList, "Resources//Models//terrain.bin", MESH_TYPE_BIN);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// guide
	LineTex = new Texture(Device, CmdList, L"Resources//SystemResources//Textures//line_tex.png", TEXTURE_TYPE_WIC);
	GuideTex = new Texture(Device, CmdList, L"Resources//Image//guide.png", TEXTURE_TYPE_WIC);
	////////////////////////////////

	TerrainTex = new Texture(Device, CmdList, L"Resources//Image//grass.jpg", TEXTURE_TYPE_WIC);
	HelicopterTex = new Texture(Device, CmdList, L"Resources//Image//GunShip.png", TEXTURE_TYPE_WIC);
	WoodTex = new Texture(Device, CmdList, L"Resources//Image//Wood.jpg", TEXTURE_TYPE_WIC);
	SkyboxTex = new Texture(Device, CmdList, L"Resources//Image//skytex.png", TEXTURE_TYPE_WIC);
}