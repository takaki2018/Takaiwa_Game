//-----------------------------------------------------------------
//
// ���b�V���t�B�[���h (meshfield.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "meshfield.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define WIDTH_NUM			(10)										// ��
#define DEPTH_NUM			(10)										// �s
#define WIDTH				(200.0f)									// ��
#define DEPTH				(200.0f)									// ���s
#define WIDTH_VEC			(WIDTH_NUM + 1)								// ��̒��_��
#define DEPTH_VEC			(DEPTH_NUM + 1)								// �s�̒��_��

#define MAX_MF_VERTEX		(DEPTH_NUM * 2 * (WIDTH_NUM + 2) - 2)		// �����_��
#define MAX_INDEX			(WIDTH_NUM * DEPTH_NUM * 2 + (4 * (DEPTH_NUM - 1)))	// ���C���f�b�N�X��
#define MAX_MESHFIELD		((WIDTH_NUM + 1) * (DEPTH_NUM + 1))					// 

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;				// ���b�V���t�B�[���h�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;				// ���b�V���t�B�[���h�̃C���f�b�N�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;					// ���b�V���t�B�[���h�̃e�N�X�`���̃|�C���^
MeshField g_aMeshField;											// ���b�V���t�B�[���h���

//-----------------------------------------------------------------
// ���b�V���t�B�[���h�̏���������
//-----------------------------------------------------------------
HRESULT InitMeshField(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	
	// ���̏�����
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nCntBlockX = WIDTH_NUM;
	g_aMeshField.nCntBlockZ = DEPTH_NUM;
	g_aMeshField.fWidth = WIDTH;
	g_aMeshField.fDepth = DEPTH;
	g_aMeshField.fsizeWidth = g_aMeshField.nCntBlockX * g_aMeshField.fWidth;
	g_aMeshField.fsizeDepth = g_aMeshField.nCntBlockZ * g_aMeshField.fDepth;
	g_aMeshField.bUse = false;
	
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTextureMeshField);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_MESHFIELD,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_VEC; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC; nCntX++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aMeshField.fsizeWidth / 2.0f + (float)nCntX * g_aMeshField.fWidth,
				g_aMeshField.pos.y, 
				g_aMeshField.fsizeDepth / 2.0f - (float)nCntZ * g_aMeshField.fDepth);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

			// ���̏��
			pVtx++;
		}
	}
	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * MAX_MF_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,						// �t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL)))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_NUM; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC + 1; nCntX++)
		{
			if (nCntX != 0 && nCntX == WIDTH_VEC && nCntZ != DEPTH_NUM - 1)
			{// �E�[����܂�Ԃ���
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntZ == DEPTH_NUM - 1 && nCntX == WIDTH_VEC)
			{// �I�����ɖ�������
				break;
			}
			else
			{// �ʏ�z�u
				pIdx[0] = WIDTH_VEC + (WIDTH_VEC * nCntZ) + nCntX;
				pIdx[1] = pIdx[0] - WIDTH_VEC;
			}
			// ���̏��Ɉړ�
			pIdx += 2;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshField->Unlock();

	// ���̔z�u
	SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f, 0.0f);

	return S_OK;
}

//-----------------------------------------------------------------
// ���b�V���t�B�[���h�̏I������
//-----------------------------------------------------------------
void UninitMeshField(void)
{
	// �C���f�b�N�X�̊J��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}
}

//-----------------------------------------------------------------
// ���b�V���t�B�[���h�̍X�V����
//-----------------------------------------------------------------
void UpdateMeshField(void)
{

}

//-----------------------------------------------------------------
// ���b�V���t�B�[���h�̕`�揈��
//-----------------------------------------------------------------
void DrawMeshField(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	if (g_aMeshField.bUse == true)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMeshField.mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
		D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
		D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshField);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �|���S���̕`��
		pDevice->SetTexture(0, g_pTextureMeshField);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0, MAX_MESHFIELD,0, MAX_INDEX);
	}
	
}

//-----------------------------------------------------------------
// ���b�V���t�B�[���h�̐ݒu
//-----------------------------------------------------------------
void SetMeshField(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	// �\���̂̃|�C���^�ϐ�
	MeshField *pMeshField = &g_aMeshField;

	if (pMeshField->bUse == false)
	{
		// �ʒu�̐ݒ�
		pMeshField->pos = pos;

		// �g�p���Ă����Ԃɂ���
		pMeshField->bUse = true;
	}
}

//-----------------------------------------------------------------
// ���b�V���t�B�[���h�Ƃ̓����蔻��
//-----------------------------------------------------------------
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	// �ϐ��錾
	bool bLand = false;

	if (pPosOld->x > g_aMeshField.pos.x - g_aMeshField.fsizeWidth / 2.0f && pPosOld->x < g_aMeshField.pos.x + g_aMeshField.fsizeWidth / 2.0f &&
		pPosOld->z > g_aMeshField.pos.z - g_aMeshField.fsizeDepth / 2.0f && pPosOld->z < g_aMeshField.pos.z + g_aMeshField.fsizeDepth / 2.0f)
	{
		if (pPosOld->y >= g_aMeshField.pos.y)
		{
			if (pPos->y < g_aMeshField.pos.y)
			{
				pPos->y = g_aMeshField.pos.y;
				bLand = true;
			}
		}
	}
	return bLand;
}