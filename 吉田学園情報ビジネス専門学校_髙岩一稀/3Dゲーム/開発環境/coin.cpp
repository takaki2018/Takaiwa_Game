//-----------------------------------------------------------------
//
// �R�C�� (Coin.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "Coin.h"
#include "wall.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "score.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_COIN			(8)
#define POLYGON_SIZE		(5.0f)		// �|���S���̃T�C�Y
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
Coin g_aCoin[MAX_COIN];										// ���f�����
LPDIRECT3DTEXTURE9 g_apTextureCoin[MAX_COIN] = {};			// �e�N�X�`���ւ̃|�C���^

//-----------------------------------------------------------------
// �R�C���̏���������
//-----------------------------------------------------------------
HRESULT InitCoin(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_COIN; nCnt++)
	{
		// ���̏�����
		g_aCoin[nCnt].pMeshModel = NULL;
		g_aCoin[nCnt].pBuffMatModel = NULL;
		g_aCoin[nCnt].nNumMatModel = NULL;
		g_aCoin[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].rotDest = g_aCoin[nCnt].rot;
		g_aCoin[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].nIdx = -1;
		g_aCoin[nCnt].bUse = false;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/coin.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aCoin[nCnt].pBuffMatModel,
			NULL,
			&g_aCoin[nCnt].nNumMatModel,
			&g_aCoin[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}

		// �R�C���̃e�N�X�`��
		TexCoin(nCnt);

		// �R�C���̒��_
		VecCoin(nCnt);
	}

	// �R�C���̐ݒu
	SetCoin(D3DXVECTOR3(600.0f, 0.0f, 0.0f));
	SetCoin(D3DXVECTOR3(-200.0f, 0.0f, 400.0f));
	SetCoin(D3DXVECTOR3(0.0f, 0.0f, 600.0f));
	SetCoin(D3DXVECTOR3(700.0f, 0.0f, -200.0f));
	SetCoin(D3DXVECTOR3(100.0f, 0.0f, 100.0f));
	SetCoin(D3DXVECTOR3(-500.0f, 0.0f, -400.0f));
	SetCoin(D3DXVECTOR3(200.0f, 0.0f, -100.0f));
	SetCoin(D3DXVECTOR3(0.0f, 0.0f, -500.0f));
	
	return S_OK;
}

//-----------------------------------------------------------------
// �R�C���̏I������
//-----------------------------------------------------------------
void UninitCoin(void)
{
	for (int nCnt = 0; nCnt < MAX_COIN; nCnt++)
	{
		// ���b�V�����̊J��
		if (g_aCoin[nCnt].pBuffMatModel != NULL)
		{
			g_aCoin[nCnt].pBuffMatModel->Release();
			g_aCoin[nCnt].pBuffMatModel = NULL;
		}

		// �}�e���A�����̊J��
		if (g_aCoin[nCnt].pMeshModel != NULL)
		{
			g_aCoin[nCnt].pMeshModel->Release();
			g_aCoin[nCnt].pMeshModel = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �R�C���̍X�V����
//-----------------------------------------------------------------
void UpdateCoin(void)
{
	// �\���̂̃|�C���^�ϐ�
	Player *pPlayer = GetPlayer();
	Coin *pCoin = &g_aCoin[0];

	// �ϐ��錾
	int nCntTrue = 0;

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++,pCoin++)
	{
		if (pCoin->bUse == true)
		{// �g���Ă���̂ŃJ�E���g����
			nCntTrue++;

			// ��]������
			pCoin->rot.y += 0.05f;

			// 3.14f�𒴂�����6.28f������
			if (pCoin->rot.y > D3DX_PI)
			{
				pCoin->rot.y -= D3DX_PI * 2.0f;
			}
		}
	}
	if (nCntTrue == 0)
	{// �R�C���̐���0���̎��t�F�[�h�A�E�g
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------
//�R�C���̕`�揈��
//-----------------------------------------------------------------
void DrawCoin(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aCoin[nCntCoin].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCoin[nCntCoin].rot.y, g_aCoin[nCntCoin].rot.x, g_aCoin[nCntCoin].rot.z);
			D3DXMatrixMultiply(&g_aCoin[nCntCoin].mtxWorld, &g_aCoin[nCntCoin].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aCoin[nCntCoin].pos.x, g_aCoin[nCntCoin].pos.y, g_aCoin[nCntCoin].pos.z);
			D3DXMatrixMultiply(&g_aCoin[nCntCoin].mtxWorld, &g_aCoin[nCntCoin].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCoin[nCntCoin].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aCoin[nCntCoin].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aCoin[nCntCoin].nNumMatModel; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureCoin[nCntMat]);

				// ���f��(�p�[�c)�̕`��
				g_aCoin[nCntCoin].pMeshModel->DrawSubset(nCntMat);
			}
			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------
// �R�C���̐ݒu
//-----------------------------------------------------------------
void SetCoin(D3DXVECTOR3 pos)
{
	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == false)
		{
			// ���W�̐ݒ�
			g_aCoin[nCntCoin].pos = pos;

			// �O�ς�p���������蔻��
			CrossProductCoin(nCntCoin);

			// �g�p��Ԃ�true�ɂ���
			g_aCoin[nCntCoin].bUse = true;

			// �e�̔z�u
			g_aCoin[nCntCoin].nIdx = SetShadow(D3DXVECTOR3(g_aCoin[nCntCoin].pos.x, 0.0f, g_aCoin[nCntCoin].pos.z), SHADOW_SIZE, SHADOW_SIZE);

			break;
		}
	}
}

//-----------------------------------------------------------------
// �R�C���̃e�N�X�`��
//-----------------------------------------------------------------
void TexCoin(int nCntCoin)
{
	// �ϐ��錾
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aCoin[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aCoin[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureCoin[nCntMat]);
		}
	}
}

//-----------------------------------------------------------------
// �R�C���̒��_���W
//-----------------------------------------------------------------
void VecCoin(int nCntCoin)
{
	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	// ���_���̎擾
	nNumVtx = g_aCoin[nCntCoin].pMeshModel->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_aCoin[nCntCoin].pMeshModel->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	g_aCoin[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_aCoin[nCntCoin].vtxMinModel.x > vtx.x)
		{// X���W�̍ŏ��l
			g_aCoin[nCntCoin].vtxMinModel.x = vtx.x;
		}
		else if (g_aCoin[nCntCoin].vtxMaxModel.x < vtx.x)
		{// X���W�̍ő�l
			g_aCoin[nCntCoin].vtxMaxModel.x = vtx.x;
		}

		if (g_aCoin[nCntCoin].vtxMinModel.y > vtx.y)
		{// Y���W�̍ŏ��l
			g_aCoin[nCntCoin].vtxMinModel.y = vtx.y;
		}
		else if (g_aCoin[nCntCoin].vtxMaxModel.y < vtx.y)
		{// Y���W�̍ő�l
			g_aCoin[nCntCoin].vtxMaxModel.y = vtx.y;
		}

		if (g_aCoin[nCntCoin].vtxMinModel.z > vtx.z)
		{// Z���W�̍ŏ��l
			g_aCoin[nCntCoin].vtxMinModel.z = vtx.z;
		}
		else if (g_aCoin[nCntCoin].vtxMaxModel.z < vtx.z)
		{// Z���W�̍ő�l
			g_aCoin[nCntCoin].vtxMaxModel.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_aCoin[0].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// �O�ς�p�����R�C���̓����蔻��
//-----------------------------------------------------------------
void CrossProductCoin(int nCntCoin)
{
	// �ϐ��̏�����
	g_aCoin[nCntCoin].aPos[0] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMinModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMinModel.z);
	g_aCoin[nCntCoin].aPos[1] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMinModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMaxModel.z);
	g_aCoin[nCntCoin].aPos[2] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMaxModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMaxModel.z);
	g_aCoin[nCntCoin].aPos[3] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMaxModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMinModel.z);

	// �ϐ��̏�����
	g_aCoin[nCntCoin].aVecA[0] = g_aCoin[nCntCoin].aPos[1] - g_aCoin[nCntCoin].aPos[0];
	g_aCoin[nCntCoin].aVecA[1] = g_aCoin[nCntCoin].aPos[2] - g_aCoin[nCntCoin].aPos[1];
	g_aCoin[nCntCoin].aVecA[2] = g_aCoin[nCntCoin].aPos[3] - g_aCoin[nCntCoin].aPos[2];
	g_aCoin[nCntCoin].aVecA[3] = g_aCoin[nCntCoin].aPos[0] - g_aCoin[nCntCoin].aPos[3];
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̓����蔻��
//-----------------------------------------------------------------
bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// �\���̂̃|�C���^�ϐ�
	Coin *pCoin = &g_aCoin[0];

	// �ϐ��錾
	bool bGetCoin = false;		// �R�C�����l���������ǂ���

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++, pCoin++)
	{
		if (pCoin->bUse == true)
		{
			// �O�ς�p���������蔻��
			pCoin->aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pCoin->aPos[0].x, 0.0f, pPos->z - pCoin->aPos[0].z);
			pCoin->aVecB[1] = D3DXVECTOR3(pPos->x - pCoin->aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pCoin->aPos[1].z);
			pCoin->aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pCoin->aPos[2].x, 0.0f, pPos->z - pCoin->aPos[2].z);
			pCoin->aVecB[3] = D3DXVECTOR3(pPos->x - pCoin->aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pCoin->aPos[3].z);

			pCoin->fVec[0] = (pCoin->aVecA[0].z * pCoin->aVecB[0].x) - (pCoin->aVecA[0].x * pCoin->aVecB[0].z);
			pCoin->fVec[1] = (pCoin->aVecA[1].z * pCoin->aVecB[1].x) - (pCoin->aVecA[1].x * pCoin->aVecB[1].z);
			pCoin->fVec[2] = (pCoin->aVecA[2].z * pCoin->aVecB[2].x) - (pCoin->aVecA[2].x * pCoin->aVecB[2].z);
			pCoin->fVec[3] = (pCoin->aVecA[3].z * pCoin->aVecB[3].x) - (pCoin->aVecA[3].x * pCoin->aVecB[3].z);

			if (pPos->y <= pCoin->pos.y + pCoin->vtxMaxModel.y - pvtxMin->y &&
				pPos->y >= pCoin->pos.y + pCoin->vtxMinModel.y - pvtxMax->y)
			{
				if (pCoin->fVec[0] > 0.0f && pCoin->fVec[1] > 0.0f &&
					pCoin->fVec[2] > 0.0f && pCoin->fVec[3] > 0.0f)
				{
					// �g�p��Ԃ�false�ɂ���
					pCoin->bUse = false;

					// �X�R�A�����Z����
					AddScore(1000);

					// �e������
					DeleteShadow(pCoin->nIdx);
				}
			}
		}
	}

	return bGetCoin;
}