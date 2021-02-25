//-----------------------------------------------------------------
//
// �R�C�� (Coin.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "Coin.h"
#include "coin_sheets.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "setparticle.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
CoinInfo g_CoinInfo;								// ���f�����
bool g_bNoCoin;										// �R�C�������邩�Ȃ���

//-----------------------------------------------------------------
// �R�C���̏���������
//-----------------------------------------------------------------
HRESULT InitCoin(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_bNoCoin = false;

	// �R�C���̔z�u���̓ǂݍ���
	LoadSetCoin();

	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nNumModel; nCntCoin++)
	{
		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(&g_CoinInfo.coin[nCntCoin].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_CoinInfo.coin[nCntCoin].pBuffMatModel,
			NULL,		
			&g_CoinInfo.coin[nCntCoin].nNumMatModel,
			&g_CoinInfo.coin[nCntCoin].pMeshModel)))
		{
			return E_FAIL;
		}

		// �R�C���̃e�N�X�`��
		TexCoin(nCntCoin);

		// �R�C���̒��_
		VecCoin(nCntCoin);
	}
	// ���f���̔z�u
	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nCntCoin; nCntCoin++)
	{
		// �g�p��Ԃ�true�ɂ���
		g_CoinInfo.coinSet[nCntCoin].bUse = true;

		// �e�̔z�u
		g_CoinInfo.coinSet[nCntCoin].nIdxShadow = SetShadow(D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x, g_CoinInfo.coinSet[nCntCoin].pos.y, g_CoinInfo.coinSet[nCntCoin].pos.z), SHADOW_SIZE, SHADOW_SIZE);
	}

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
		if (g_CoinInfo.coin[nCnt].pBuffMatModel != NULL)
		{
			g_CoinInfo.coin[nCnt].pBuffMatModel->Release();
			g_CoinInfo.coin[nCnt].pBuffMatModel = NULL;
		}

		// �}�e���A�����̊J��
		if (g_CoinInfo.coin[nCnt].pMeshModel != NULL)
		{
			g_CoinInfo.coin[nCnt].pMeshModel->Release();
			g_CoinInfo.coin[nCnt].pMeshModel = NULL;
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
	CoinInfo *pCoinInfo = &g_CoinInfo;

	// �ϐ��錾
	int nCntTrue = 0;

	for (int nCntCoin = 0; nCntCoin < pCoinInfo->nCntCoin; nCntCoin++)
	{
		if (pCoinInfo->coinSet[nCntCoin].bUse == true)
		{// �g���Ă���̂ŃJ�E���g����
			nCntTrue++;

			// ��]������
			pCoinInfo->coinSet[nCntCoin].rot.y += 0.05f;

			// 3.14f�𒴂�����6.28f������
			if (pCoinInfo->coinSet[nCntCoin].rot.y > D3DX_PI)
			{
				pCoinInfo->coinSet[nCntCoin].rot.y -= D3DX_PI * 2.0f;
			}
		}
	}
	if (nCntTrue == 0)
	{// �R�C���̐���0���̎��Ȃ��Ƃ���
		g_bNoCoin = true;
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

	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nCntCoin; nCntCoin++)
	{
		if (g_CoinInfo.coinSet[nCntCoin].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_CoinInfo.coinSet[nCntCoin].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_CoinInfo.coinSet[nCntCoin].rot.y, g_CoinInfo.coinSet[nCntCoin].rot.x, g_CoinInfo.coinSet[nCntCoin].rot.z);
			D3DXMatrixMultiply(&g_CoinInfo.coinSet[nCntCoin].mtxWorld, &g_CoinInfo.coinSet[nCntCoin].mtxWorld, &mtxRot);

			// ���_���W�̕␳
			D3DXVec3TransformCoord(&g_CoinInfo.coinSet[nCntCoin].vtxMaxModel, &g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].vtxMaxModel, &g_CoinInfo.coinSet[nCntCoin].mtxWorld);
			D3DXVec3TransformCoord(&g_CoinInfo.coinSet[nCntCoin].vtxMinModel, &g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].vtxMinModel, &g_CoinInfo.coinSet[nCntCoin].mtxWorld);

			// ���_���W�̕␳
			VecCoinSet(nCntCoin);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_CoinInfo.coinSet[nCntCoin].pos.x, g_CoinInfo.coinSet[nCntCoin].pos.y, g_CoinInfo.coinSet[nCntCoin].pos.z);
			D3DXMatrixMultiply(&g_CoinInfo.coinSet[nCntCoin].mtxWorld, &g_CoinInfo.coinSet[nCntCoin].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_CoinInfo.coinSet[nCntCoin].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].nNumMatModel; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].apTextureModelSet[nCntMat]);

				// ���f��(�p�[�c)�̕`��
				g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].pMeshModel->DrawSubset(nCntMat);
			}
			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
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
	pMat = (D3DXMATERIAL*)g_CoinInfo.coin[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_CoinInfo.coin[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_CoinInfo.coin[nCntCoin].apTextureModelSet[nCntMat]);
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
	nNumVtx = g_CoinInfo.coin[nCntCoin].pMeshModel->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_CoinInfo.coin[nCntCoin].pMeshModel->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	g_CoinInfo.coin[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_CoinInfo.coin[nCntCoin].vtxMinModel.x > vtx.x)
		{// X���W�̍ŏ��l
			g_CoinInfo.coin[nCntCoin].vtxMinModel.x = vtx.x;
		}
		else if (g_CoinInfo.coin[nCntCoin].vtxMaxModel.x < vtx.x)
		{// X���W�̍ő�l
			g_CoinInfo.coin[nCntCoin].vtxMaxModel.x = vtx.x;
		}

		if (g_CoinInfo.coin[nCntCoin].vtxMinModel.y > vtx.y)
		{// Y���W�̍ŏ��l
			g_CoinInfo.coin[nCntCoin].vtxMinModel.y = vtx.y;
		}
		else if (g_CoinInfo.coin[nCntCoin].vtxMaxModel.y < vtx.y)
		{// Y���W�̍ő�l
			g_CoinInfo.coin[nCntCoin].vtxMaxModel.y = vtx.y;
		}

		if (g_CoinInfo.coin[nCntCoin].vtxMinModel.z > vtx.z)
		{// Z���W�̍ŏ��l
			g_CoinInfo.coin[nCntCoin].vtxMinModel.z = vtx.z;
		}
		else if (g_CoinInfo.coin[nCntCoin].vtxMaxModel.z < vtx.z)
		{// Z���W�̍ő�l
			g_CoinInfo.coin[nCntCoin].vtxMaxModel.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_CoinInfo.coin[nCntCoin].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// ���f���̒��_���W�̕␳
//-----------------------------------------------------------------
void VecCoinSet(int nCntCoin)
{
	// �ő�l�ƍŏ��l�̓���ւ�
	if (g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x < g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x)
	{
		// �ϐ��錾
		float fVecX;		// �ۑ��p

		// �ő�l�ƍŏ��l�����ւ���
		fVecX = g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x;
		g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x = g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x;
		g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x = fVecX;
	}
	if (g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z < g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z)
	{
		// �ϐ��錾
		float fVecZ;		// �ۑ��p

		// �ő�l�ƍŏ��l�����ւ���
		fVecZ = g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z;
		g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z = g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z;
		g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z = fVecZ;
	}
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̓����蔻��
//-----------------------------------------------------------------
bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// �\���̂̃|�C���^�ϐ�
	CoinInfo *pCoinInfo = &g_CoinInfo;

	// �ϐ��錾
	bool bGetCoin = false;		// �R�C�����l���������ǂ���

	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nCntCoin; nCntCoin++)
	{
		if (pCoinInfo->coinSet[nCntCoin].bUse == true)
		{
			// �R�C���̊e���_���W
			g_CoinInfo.coinSet[nCntCoin].aPos[0] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z);
			g_CoinInfo.coinSet[nCntCoin].aPos[1] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z);
			g_CoinInfo.coinSet[nCntCoin].aPos[2] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z);
			g_CoinInfo.coinSet[nCntCoin].aPos[3] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z);

			// �e���_�Ɍ����Ẵx�N�g��
			g_CoinInfo.coinSet[nCntCoin].aVecA[0] = g_CoinInfo.coinSet[nCntCoin].aPos[1] - g_CoinInfo.coinSet[nCntCoin].aPos[0];
			g_CoinInfo.coinSet[nCntCoin].aVecA[1] = g_CoinInfo.coinSet[nCntCoin].aPos[2] - g_CoinInfo.coinSet[nCntCoin].aPos[1];
			g_CoinInfo.coinSet[nCntCoin].aVecA[2] = g_CoinInfo.coinSet[nCntCoin].aPos[3] - g_CoinInfo.coinSet[nCntCoin].aPos[2];
			g_CoinInfo.coinSet[nCntCoin].aVecA[3] = g_CoinInfo.coinSet[nCntCoin].aPos[0] - g_CoinInfo.coinSet[nCntCoin].aPos[3];

			// �R�C���̊e���_����Ώۂ܂ł̃x�N�g��
			pCoinInfo->coinSet[nCntCoin].aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pCoinInfo->coinSet[nCntCoin].aPos[0].x, 0.0f, pPos->z - pCoinInfo->coinSet[nCntCoin].aPos[0].z);
			pCoinInfo->coinSet[nCntCoin].aVecB[1] = D3DXVECTOR3(pPos->x - pCoinInfo->coinSet[nCntCoin].aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pCoinInfo->coinSet[nCntCoin].aPos[1].z);
			pCoinInfo->coinSet[nCntCoin].aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pCoinInfo->coinSet[nCntCoin].aPos[2].x, 0.0f, pPos->z - pCoinInfo->coinSet[nCntCoin].aPos[2].z);
			pCoinInfo->coinSet[nCntCoin].aVecB[3] = D3DXVECTOR3(pPos->x - pCoinInfo->coinSet[nCntCoin].aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pCoinInfo->coinSet[nCntCoin].aPos[3].z);

			// �O�ς�p���������蔻��
			pCoinInfo->coinSet[nCntCoin].fVec[0] = (pCoinInfo->coinSet[nCntCoin].aVecA[0].z * pCoinInfo->coinSet[nCntCoin].aVecB[0].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[0].x * pCoinInfo->coinSet[nCntCoin].aVecB[0].z);
			pCoinInfo->coinSet[nCntCoin].fVec[1] = (pCoinInfo->coinSet[nCntCoin].aVecA[1].z * pCoinInfo->coinSet[nCntCoin].aVecB[1].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[1].x * pCoinInfo->coinSet[nCntCoin].aVecB[1].z);
			pCoinInfo->coinSet[nCntCoin].fVec[2] = (pCoinInfo->coinSet[nCntCoin].aVecA[2].z * pCoinInfo->coinSet[nCntCoin].aVecB[2].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[2].x * pCoinInfo->coinSet[nCntCoin].aVecB[2].z);
			pCoinInfo->coinSet[nCntCoin].fVec[3] = (pCoinInfo->coinSet[nCntCoin].aVecA[3].z * pCoinInfo->coinSet[nCntCoin].aVecB[3].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[3].x * pCoinInfo->coinSet[nCntCoin].aVecB[3].z);

			if (pPos->y <= pCoinInfo->coinSet[nCntCoin].pos.y + pCoinInfo->coinSet[nCntCoin].vtxMaxModel.y - pvtxMin->y &&
				pPos->y >= pCoinInfo->coinSet[nCntCoin].pos.y + pCoinInfo->coinSet[nCntCoin].vtxMinModel.y - pvtxMax->y)
			{
				if (pCoinInfo->coinSet[nCntCoin].fVec[0] > 0.0f && pCoinInfo->coinSet[nCntCoin].fVec[1] > 0.0f &&
					pCoinInfo->coinSet[nCntCoin].fVec[2] > 0.0f && pCoinInfo->coinSet[nCntCoin].fVec[3] > 0.0f)
				{
					// �g�p��Ԃ�false�ɂ���
					pCoinInfo->coinSet[nCntCoin].bUse = false;

					// �X�R�A�����Z����
					AddScore(1000);

					// �R�C���l���󋵂̉��Z
					AddCoinSheets(1);

					// �e�X�g
					CoinParticle(pCoinInfo->coinSet[nCntCoin].pos);

					// �����o��
					PlaySound(SOUND_LABEL_SE_GETCOIN);

					// �e������
					DeleteShadow(pCoinInfo->coinSet[nCntCoin].nIdxShadow);
				}
			}
		}
	}

	return bGetCoin;
}

//-----------------------------------------------------------------
// �R�C���z�u���̓ǂݍ���
//-----------------------------------------------------------------
void LoadSetCoin(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/coin.txt", "r");

	// �ϐ��̏�����
	g_CoinInfo.nCntCoin = 0;

	// �ϐ��錾
	bool bGetScript = false;
	bool bModelSet = false;

	char aLine[128];
	char aText[128];
	int nCntModel = 0;
	int nCntModelSet = -1;
	int nCntMotion = -1;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// SCRIPT��T��
	while (fgets(aLine, 128, pFile) != NULL)
	{
		fscanf(pFile, "%s", &aText[0]);

		// SCRIPT���������炻��ȍ~��ǂݍ���
		if (strcmp(&aText[0], "SCRIPT") == 0)
		{
			bGetScript = true;
		}

		if (bGetScript == true)
		{
			// ���f�����̓ǂݍ���
			// NUM_MODEL���������烂�f���̑������i�[
			if (strcmp(&aText[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s %d", &aText[0], &g_CoinInfo.nNumModel);
			}
			// MODEL_FILENAME����������X�t�@�C�������i�[
			if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < g_CoinInfo.nNumModel)
			{
				fscanf(pFile, "%s %s", &aText[0], &g_CoinInfo.coin[nCntModel].aFileName[0]);

				// ���f���̑��������i�[����̂ŃJ�E���g�𑝂₷
				nCntModel++;
			}
			// �e�p�[�c�ɏ���ǂݍ��ދ�������Ƃ�����
			if (strcmp(&aText[0], "MODELSET") == 0)
			{
				// �ϐ������Z
				nCntModelSet++;

				// �z�u���郂�f���������Z
				g_CoinInfo.nCntCoin++;

				bModelSet = true;
			}
			if (bModelSet == true)
			{
				// TYPE����������ԍ����i�[
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &g_CoinInfo.coinSet[nCntModelSet].nIdx);
				}
				// POS������������W�����i�[
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&g_CoinInfo.coinSet[nCntModelSet].pos.x,
						&g_CoinInfo.coinSet[nCntModelSet].pos.y,
						&g_CoinInfo.coinSet[nCntModelSet].pos.z);
				}
				// ROT����������p�x�����i�[
				if (strcmp(&aText[0], "ROT") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&rot.x,
						&rot.y,
						&rot.z);

					rot.x = rot.x / 180.0f;
					rot.y = rot.y / 180.0f;
					rot.z = rot.z / 180.0f;

					g_CoinInfo.coinSet[nCntModelSet].rot.x = rot.x * D3DX_PI;
					g_CoinInfo.coinSet[nCntModelSet].rot.y = rot.y * D3DX_PI;
					g_CoinInfo.coinSet[nCntModelSet].rot.z = rot.z * D3DX_PI;
				}
				// END_MODELSETKEY���������烂�f���z�u������
				if (strcmp(&aText[0], "END_MODELSET") == 0)
				{
					bModelSet = false;
				}
			}
			// END_SCRIPT����������ǂݍ��݂��I����
			if (strcmp(&aText[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	// �t�@�C�������
	fclose(pFile);
}

//-----------------------------------------------------------------
// �R�C�������邩�Ȃ���
//-----------------------------------------------------------------
bool GetbNoCoin(void)
{
	return g_bNoCoin;
}