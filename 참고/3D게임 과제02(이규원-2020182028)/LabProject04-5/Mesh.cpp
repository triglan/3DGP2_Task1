//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "Scene.h"

CMesh::CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, char *pstrFileName, bool bTextFile)
{
	if (pstrFileName) LoadMeshFromFile(pd3dDevice, pd3dCommandList, pstrFileName, bTextFile);
}

CMesh::~CMesh()
{
	if (m_pxmf3Positions) delete[] m_pxmf3Positions;
	if (m_pxmf3Normals) delete[] m_pxmf3Normals;
	if (m_pxmf2TextureCoords) delete[] m_pxmf2TextureCoords;

	if (m_pnIndices) delete[] m_pnIndices;

	if (m_pd3dVertexBufferViews) delete[] m_pd3dVertexBufferViews;

	if (m_pd3dPositionBuffer) m_pd3dPositionBuffer->Release();
	if (m_pd3dNormalBuffer) m_pd3dNormalBuffer->Release();
	if (m_pd3dTextureCoordBuffer) m_pd3dTextureCoordBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
}

void CMesh::ReleaseUploadBuffers() 
{
	if (m_pd3dPositionUploadBuffer) m_pd3dPositionUploadBuffer->Release();
	if (m_pd3dNormalUploadBuffer) m_pd3dNormalUploadBuffer->Release();
	if (m_pd3dTextureCoordUploadBuffer) m_pd3dTextureCoordUploadBuffer->Release();
	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();

	m_pd3dPositionUploadBuffer = NULL;
	m_pd3dNormalUploadBuffer = NULL;
	m_pd3dTextureCoordUploadBuffer = NULL;
	m_pd3dIndexUploadBuffer = NULL;
};

void CMesh::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, m_nVertexBufferViews, m_pd3dVertexBufferViews);
	if (m_pd3dIndexBuffer)
	{
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CMesh::LoadMeshFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, char *pstrFileName, bool bTextFile)
{
	char pstrToken[64] = { '\0' };

	if (bTextFile)
	{
		ifstream InFile(pstrFileName);

		for (; ; )
		{
			InFile >> pstrToken;
			if (!InFile) break;

			if (!strcmp(pstrToken, "<Vertices>:"))
			{
				InFile >> m_nVertices;
				m_pxmf3Positions = new XMFLOAT3[m_nVertices];
				for (UINT i = 0; i < m_nVertices; i++) InFile >> m_pxmf3Positions[i].x >> m_pxmf3Positions[i].y >> m_pxmf3Positions[i].z;
			}
			else if (!strcmp(pstrToken, "<Normals>:"))
			{
				InFile >> pstrToken;
				m_pxmf3Normals = new XMFLOAT3[m_nVertices];
				for (UINT i = 0; i < m_nVertices; i++) InFile >> m_pxmf3Normals[i].x >> m_pxmf3Normals[i].y >> m_pxmf3Normals[i].z;
			}
			else if (!strcmp(pstrToken, "<TextureCoords>:"))
			{
				InFile >> pstrToken;
				m_pxmf2TextureCoords = new XMFLOAT2[m_nVertices];
				for (UINT i = 0; i < m_nVertices; i++) InFile >> m_pxmf2TextureCoords[i].x >> m_pxmf2TextureCoords[i].y;
			}
			else if (!strcmp(pstrToken, "<Indices>:"))
			{
				InFile >> m_nIndices;
				m_pnIndices = new UINT[m_nIndices];
				for (UINT i = 0; i < m_nIndices; i++) InFile >> m_pnIndices[i];
			}
		}
	}
	else
	{
		FILE* pFile = NULL;
		::fopen_s(&pFile, pstrFileName, "rb");
		::rewind(pFile);

		char pstrToken[64] = { '\0' };

		BYTE nStrLength = 0;
		UINT nReads = 0;
		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 14, pFile); //"<BoundingBox>:"
		nReads = (UINT)::fread(&m_OBB.Center, sizeof(float), 3, pFile);
		nReads = (UINT)::fread(&m_OBB.Extents, sizeof(float), 3, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 11, pFile); //"<Vertices>:"
		nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pFile);
		m_pxmf3Positions = new XMFLOAT3[m_nVertices];
		nReads = (UINT)::fread(m_pxmf3Positions, sizeof(float), 3 * m_nVertices, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 10, pFile); //"<Normals>:"
		nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pFile);
		m_pxmf3Normals = new XMFLOAT3[m_nVertices];
		nReads = (UINT)::fread(m_pxmf3Normals, sizeof(float), 3 * m_nVertices, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 16, pFile); //"<TextureCoords>:"
		nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pFile);
		m_pxmf2TextureCoords = new XMFLOAT2[m_nVertices];
		nReads = (UINT)::fread(m_pxmf2TextureCoords, sizeof(float), 2 * m_nVertices, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 10, pFile); //"<Indices>:"
		nReads = (UINT)::fread(&m_nIndices, sizeof(int), 1, pFile);
		m_pnIndices = new UINT[m_nIndices];
		nReads = (UINT)::fread(m_pnIndices, sizeof(UINT), m_nIndices, pFile);

		::fclose(pFile);
	}

	m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Positions, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
	m_pd3dNormalBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Normals, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dNormalUploadBuffer);
	m_pd3dTextureCoordBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf2TextureCoords, sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTextureCoordUploadBuffer);

	m_nVertexBufferViews = 3;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[1].BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[2].BufferLocation = m_pd3dTextureCoordBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[2].StrideInBytes = sizeof(XMFLOAT2);
	m_pd3dVertexBufferViews[2].SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;

	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
}

//BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken)
//{
//	BYTE nStrLength = 0;
//	UINT nReads = 0;
//	nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
//	nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
//	pstrToken[nStrLength] = '\0';
//
//	return(nStrLength);
//}
//
//void CMesh::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, FILE* pInFile)
//{
//	char pstrToken[64] = { '\0' };
//	UINT nReads = 0;
//
//	int nFrame = 0;
//
//	CGameObject* pGameObject = NULL;
//
//	for (; ; )
//	{
//		::ReadStringFromFile(pInFile, pstrToken);
//		if (!strcmp(pstrToken, "<Frame>:"))
//		{
//			pGameObject = new CGameObject();
//
//			nFrame = ::ReadIntegerFromFile(pInFile);
//			::ReadStringFromFile(pInFile, pGameObject->m_pstrFrameName);
//		}
//		else if (!strcmp(pstrToken, "<Transform>:"))
//		{
//			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
//			XMFLOAT4 xmf4Rotation;
//			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
//			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
//			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
//			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
//		}
//		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
//		{
//			nReads = (UINT)::fread(&pGameObject->m_xmf4x4Transform, sizeof(float), 16, pInFile);
//		}
//		else if (!strcmp(pstrToken, "<Mesh>:"))
//		{
//			CMeshLoadInfo* pMeshInfo = pGameObject->LoadMeshInfoFromFile(pInFile);
//			if (pMeshInfo)
//			{
//				CMesh* pMesh = NULL;
//				if (pMeshInfo->m_nType & VERTEXT_NORMAL)
//				{
//					pMesh = new CMeshIlluminatedFromFile(pd3dDevice, pd3dCommandList, pMeshInfo);
//				}
//				if (pMesh) pGameObject->SetMesh(pMesh);
//				delete pMeshInfo;
//			}
//		}
//		else if (!strcmp(pstrToken, "<Materials>:"))
//		{
//			MATERIALSLOADINFO* pMaterialsInfo = pGameObject->LoadMaterialsInfoFromFile(pd3dDevice, pd3dCommandList, pInFile);
//			if (pMaterialsInfo && (pMaterialsInfo->m_nMaterials > 0))
//			{
//				pGameObject->m_nMaterials = pMaterialsInfo->m_nMaterials;
//				pGameObject->m_ppMaterials = new CMaterial * [pMaterialsInfo->m_nMaterials];
//
//				for (int i = 0; i < pMaterialsInfo->m_nMaterials; i++)
//				{
//					pGameObject->m_ppMaterials[i] = NULL;
//
//					CMaterial* pMaterial = new CMaterial();
//
//					CMaterialColors* pMaterialColors = new CMaterialColors(&pMaterialsInfo->m_pMaterials[i]);
//					pMaterial->SetMaterialColors(pMaterialColors);
//
//					if (pGameObject->GetMeshType() & VERTEXT_NORMAL) pMaterial->SetIlluminatedShader();
//
//					pGameObject->SetMaterial(i, pMaterial);
//				}
//			}
//		}
//		else if (!strcmp(pstrToken, "<Children>:"))
//		{
//			int nChilds = ::ReadIntegerFromFile(pInFile);
//			if (nChilds > 0)
//			{
//				for (int i = 0; i < nChilds; i++)
//				{
//					CGameObject* pChild = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pInFile);
//					if (pChild) pGameObject->SetChild(pChild);
//#ifdef _WITH_DEBUG_RUNTIME_FRAME_HIERARCHY
//					TCHAR pstrDebug[256] = { 0 };
//					_stprintf_s(pstrDebug, 256, _T("(Child Frame: %p) (Parent Frame: %p)\n"), pChild, pGameObject);
//					OutputDebugString(pstrDebug);
//#endif
//				}
//			}
//		}
//		else if (!strcmp(pstrToken, "</Frame>"))
//		{
//			break;
//		}
//	}
//	return(pGameObject);
//}
//
//
//void CMesh::LoadGeometryFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName)
//{
//	FILE* pInFile = NULL;
//	::fopen_s(&pInFile, pstrFileName, "rb");
//	::rewind(pInFile);
//
//	CScene* pGameObject = NULL;
//	char pstrToken[64] = { '\0' };
//
//	for (; ; )
//	{
//		::ReadStringFromFile(pInFile, pstrToken);
//
//		if (!strcmp(pstrToken, "<Hierarchy>:"))
//		{
//			pGameObject = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pInFile);
//		}
//		else if (!strcmp(pstrToken, "</Hierarchy>"))
//		{
//			break;
//		}
//	}
//
//#ifdef _WITH_DEBUG_FRAME_HIERARCHY
//	TCHAR pstrDebug[256] = { 0 };
//	_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
//	OutputDebugString(pstrDebug);
//
//	CGameObject::PrintFrameInfo(pGameObject, NULL);
//#endif
//
//	return(pGameObject);
//}