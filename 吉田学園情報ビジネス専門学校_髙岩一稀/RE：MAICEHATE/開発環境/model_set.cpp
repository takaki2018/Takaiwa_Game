//==============================================================================================================
//
// ���f���Z�b�g (model_set.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "model_set.h"
#include "shadow.h"
#include "player.h"
#include "camera.h"

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
ModelSetInfo g_ModelSetInfo;												// ���f�����

//--------------------------------------------------------------------------------------------------------------
// ���f���Z�b�g�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitModelSet(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	// �e�L�X�g�t�@�C���̓ǂݍ���
	LoadSetModel();

	for (int nCnt = 0; nCnt < g_ModelSetInfo.nNumModel; nCnt++)
	{
		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(&g_ModelSetInfo.modelInfo[nCnt].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_ModelSetInfo.modelInfo[nCnt].pBuffMatModel,
			NULL,
			&g_ModelSetInfo.modelInfo[nCnt].nNumMatModel,
			&g_ModelSetInfo.modelInfo[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}

		// ���f���̃e�N�X�`��
		TexModel(nCnt);

		// ���f���̒��_
		VecModel(nCnt);
	}
	// ���f���̔z�u
	for (int nCntModel = 0; nCntModel < g_ModelSetInfo.nCntModel; nCntModel++)
	{
		// �g�p��Ԃ�true�ɂ���
		g_ModelSetInfo.modelSet[nCntModel].bUse = true;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ���f���Z�b�g�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitModelSet(void)
{
	for (int nCnt = 0; nCnt < g_ModelSetInfo.nNumModel; nCnt++)
	{
		// ���b�V�����̊J��
		if (g_ModelSetInfo.modelInfo[nCnt].pBuffMatModel != NULL)
		{
			g_ModelSetInfo.modelInfo[nCnt].pBuffMatModel->Release();
			g_ModelSetInfo.modelInfo[nCnt].pBuffMatModel = NULL;
		}

		// �}�e���A�����̊J��
		if (g_ModelSetInfo.modelInfo[nCnt].pMeshModel != NULL)
		{
			g_ModelSetInfo.modelInfo[nCnt].pMeshModel->Release();
			g_ModelSetInfo.modelInfo[nCnt].pMeshModel = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ���f���Z�b�g�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateModelSet(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// ���f���Z�b�g�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawModelSet(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	for (int nModelSet = 0; nModelSet < MAX_MODEL; nModelSet++)
	{
		if (g_ModelSetInfo.modelSet[nModelSet].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_ModelSetInfo.modelSet[nModelSet].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ModelSetInfo.modelSet[nModelSet].rot.y, g_ModelSetInfo.modelSet[nModelSet].rot.x, g_ModelSetInfo.modelSet[nModelSet].rot.z);
			D3DXMatrixMultiply(&g_ModelSetInfo.modelSet[nModelSet].mtxWorld, &g_ModelSetInfo.modelSet[nModelSet].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_ModelSetInfo.modelSet[nModelSet].pos.x, g_ModelSetInfo.modelSet[nModelSet].pos.y, g_ModelSetInfo.modelSet[nModelSet].pos.z);
			D3DXMatrixMultiply(&g_ModelSetInfo.modelSet[nModelSet].mtxWorld, &g_ModelSetInfo.modelSet[nModelSet].mtxWorld, &mtxTrans);

			// �ő�l�ŏ��l�̕␳
			VecModelSet(nModelSet);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_ModelSetInfo.modelSet[nModelSet].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_ModelSetInfo.modelInfo[g_ModelSetInfo.modelSet[nModelSet].nIdx].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_ModelSetInfo.modelInfo[g_ModelSetInfo.modelSet[nModelSet].nIdx].nNumMatModel; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_ModelSetInfo.modelInfo[g_ModelSetInfo.modelSet[nModelSet].nIdx].apTextureModelSet[nCntMat]);

				// ���f��(�p�[�c)�̕`��
				g_ModelSetInfo.modelInfo[g_ModelSetInfo.modelSet[nModelSet].nIdx].pMeshModel->DrawSubset(nCntMat);
			}
			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ���f���̃e�N�X�`��
// ����		�FnCntModelSet	- ���Ԗڂ̃��f���z�u���
// �Ԃ�l	�Fvoid			- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void TexModel(int nCntModelSet)
{
	// �ϐ��錾
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_ModelSetInfo.modelInfo[nCntModelSet].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_ModelSetInfo.modelInfo[nCntModelSet].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_ModelSetInfo.modelInfo[nCntModelSet].apTextureModelSet[nCntMat]);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ���f���̒��_���W
// ����		�FnCntModelSet	- ���Ԗڂ̃��f���z�u���
// �Ԃ�l	�Fvoid			- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void VecModel(int nCntModelSet)
{
	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	// ���_���̎擾
	nNumVtx = g_ModelSetInfo.modelInfo[nCntModelSet].pMeshModel->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_ModelSetInfo.modelInfo[nCntModelSet].pMeshModel->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	g_ModelSetInfo.modelInfo[nCntModelSet].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_ModelSetInfo.modelInfo[nCntModelSet].vtxMinModel.x > vtx.x)
		{// X���W�̍ŏ��l
			g_ModelSetInfo.modelInfo[nCntModelSet].vtxMinModel.x = vtx.x;
		}
		else if (g_ModelSetInfo.modelInfo[nCntModelSet].vtxMaxModel.x < vtx.x)
		{// X���W�̍ő�l
			g_ModelSetInfo.modelInfo[nCntModelSet].vtxMaxModel.x = vtx.x;
		}

		if (g_ModelSetInfo.modelInfo[nCntModelSet].vtxMinModel.y > vtx.y)
		{// Y���W�̍ŏ��l
			g_ModelSetInfo.modelInfo[nCntModelSet].vtxMinModel.y = vtx.y;
		}
		else if (g_ModelSetInfo.modelInfo[nCntModelSet].vtxMaxModel.y < vtx.y)
		{// Y���W�̍ő�l
			g_ModelSetInfo.modelInfo[nCntModelSet].vtxMaxModel.y = vtx.y;
		}

		if (g_ModelSetInfo.modelInfo[nCntModelSet].vtxMinModel.z > vtx.z)
		{// Z���W�̍ŏ��l
			g_ModelSetInfo.modelInfo[nCntModelSet].vtxMinModel.z = vtx.z;
		}
		else if (g_ModelSetInfo.modelInfo[nCntModelSet].vtxMaxModel.z < vtx.z)
		{// Z���W�̍ő�l
			g_ModelSetInfo.modelInfo[nCntModelSet].vtxMaxModel.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_ModelSetInfo.modelInfo[nCntModelSet].pMeshModel->UnlockVertexBuffer();
}

//--------------------------------------------------------------------------------------------------------------
// ���f���̒��_���W�̕␳
// ����		�FnCntModelSet	- ���Ԗڂ̃��f���z�u���
// �Ԃ�l	�Fvoid			- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void VecModelSet(int nCntModelSet)
{
	// �ϐ��錾
	D3DXVECTOR3 pVecMax = g_ModelSetInfo.modelInfo[g_ModelSetInfo.modelSet[nCntModelSet].nIdx].vtxMaxModel;	// ���f���̍ő咸�_
	D3DXVECTOR3 pVecMin = g_ModelSetInfo.modelInfo[g_ModelSetInfo.modelSet[nCntModelSet].nIdx].vtxMinModel;	// ���f���̍ŏ����_
	D3DXVECTOR3 pRot = g_ModelSetInfo.modelSet[nCntModelSet].rot;											// ����
	D3DXVECTOR3 aPosVec[4];		// �e���_���W

	// ���_�̉�]
	aPosVec[0].x = pVecMin.x * cosf(pRot.y) + pVecMin.z * sinf(pRot.y);
	aPosVec[0].y = pVecMin.y;
	aPosVec[0].z = pVecMin.z * cosf(pRot.y) + pVecMin.x * -sinf(pRot.y);

	aPosVec[1].x = pVecMin.x * cosf(pRot.y) + pVecMax.z * sinf(pRot.y);
	aPosVec[1].y = pVecMin.y;
	aPosVec[1].z = pVecMax.z * cosf(pRot.y) + pVecMin.x * -sinf(pRot.y);
	
	aPosVec[2].x = pVecMax.x * cosf(pRot.y) + pVecMax.z * sinf(pRot.y);
	aPosVec[2].y = pVecMin.y;
	aPosVec[2].z = pVecMax.z * cosf(pRot.y) + pVecMax.x * -sinf(pRot.y);
	
	aPosVec[3].x = pVecMax.x * cosf(pRot.y) + pVecMin.z * sinf(pRot.y);
	aPosVec[3].y = pVecMin.y;
	aPosVec[3].z = pVecMin.z * cosf(pRot.y) + pVecMax.x * -sinf(pRot.y);

	// ���f���̊e���_���W
	g_ModelSetInfo.modelSet[nCntModelSet].aPos[0] = D3DXVECTOR3(g_ModelSetInfo.modelSet[nCntModelSet].pos.x + aPosVec[0].x, g_ModelSetInfo.modelSet[nCntModelSet].pos.y + aPosVec[0].y , g_ModelSetInfo.modelSet[nCntModelSet].pos.z + aPosVec[0].z);
	g_ModelSetInfo.modelSet[nCntModelSet].aPos[1] = D3DXVECTOR3(g_ModelSetInfo.modelSet[nCntModelSet].pos.x + aPosVec[1].x, g_ModelSetInfo.modelSet[nCntModelSet].pos.y + aPosVec[1].y , g_ModelSetInfo.modelSet[nCntModelSet].pos.z + aPosVec[1].z);
	g_ModelSetInfo.modelSet[nCntModelSet].aPos[2] = D3DXVECTOR3(g_ModelSetInfo.modelSet[nCntModelSet].pos.x + aPosVec[2].x, g_ModelSetInfo.modelSet[nCntModelSet].pos.y + aPosVec[2].y , g_ModelSetInfo.modelSet[nCntModelSet].pos.z + aPosVec[2].z);
	g_ModelSetInfo.modelSet[nCntModelSet].aPos[3] = D3DXVECTOR3(g_ModelSetInfo.modelSet[nCntModelSet].pos.x + aPosVec[3].x, g_ModelSetInfo.modelSet[nCntModelSet].pos.y + aPosVec[3].y , g_ModelSetInfo.modelSet[nCntModelSet].pos.z + aPosVec[3].z);

	// ���f���̊e���_���W�Ɍ����Ẵx�N�g��
	g_ModelSetInfo.modelSet[nCntModelSet].aVecA[0] = g_ModelSetInfo.modelSet[nCntModelSet].aPos[0] - g_ModelSetInfo.modelSet[nCntModelSet].aPos[3];
	g_ModelSetInfo.modelSet[nCntModelSet].aVecA[1] = g_ModelSetInfo.modelSet[nCntModelSet].aPos[1] - g_ModelSetInfo.modelSet[nCntModelSet].aPos[0];
	g_ModelSetInfo.modelSet[nCntModelSet].aVecA[2] = g_ModelSetInfo.modelSet[nCntModelSet].aPos[2] - g_ModelSetInfo.modelSet[nCntModelSet].aPos[1];
	g_ModelSetInfo.modelSet[nCntModelSet].aVecA[3] = g_ModelSetInfo.modelSet[nCntModelSet].aPos[3] - g_ModelSetInfo.modelSet[nCntModelSet].aPos[2];
}

//--------------------------------------------------------------------------------------------------------------
// ���f���̓����蔻��
// ����		�F*pPos			- �Q�Ƃ��錻�݂̈ʒu���̃|�C���^
//			�F*pPosOld		- �Q�Ƃ���O��̈ʒu���̃|�C���^
//			�F*pMove		- �Q�Ƃ���ړ���
//			�F*pvtxMin		- �Q�Ƃ���ŏ����W
//			�F*pvtxMax		- �Q�Ƃ���ő���W
//			�FnIdxShadow	- �e�ԍ�
// �Ԃ�l	�Fvoid			- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
bool CollisionModelSet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax,int nIdxShadow)
{
	// �\���̂̃|�C���^�ϐ�
	ModelSetInfo *pModel = &g_ModelSetInfo;
	Camera pCamera = GetPlayerCamera();

	// �ϐ��錾
	bool bLandModel = false;		// �R�C�����l���������ǂ���
	bool bCollisionModel = false;		// �R�C�����l���������ǂ���
	bool bCollisionSide = false;

 	for (int nModelSet = 0; nModelSet < MAX_MODEL; nModelSet++)
	{
		if (pModel->modelSet[nModelSet].bUse == true)
		{
			// ���f���̊e���_����Ώۂ܂ł̃x�N�g��
			pModel->modelSet[nModelSet].aVecB[0] = D3DXVECTOR3((pPos->x + pvtxMax->x) - pModel->modelSet[nModelSet].aPos[0].x, 0.0f, (pPos->z + pvtxMax->z) - pModel->modelSet[nModelSet].aPos[0].z);
			pModel->modelSet[nModelSet].aVecB[1] = D3DXVECTOR3((pPos->x + pvtxMax->x) - pModel->modelSet[nModelSet].aPos[1].x, 0.0f, (pPos->z + pvtxMin->z) - pModel->modelSet[nModelSet].aPos[1].z);
			pModel->modelSet[nModelSet].aVecB[2] = D3DXVECTOR3((pPos->x + pvtxMin->x) - pModel->modelSet[nModelSet].aPos[2].x, 0.0f, (pPos->z + pvtxMin->z) - pModel->modelSet[nModelSet].aPos[2].z);
			pModel->modelSet[nModelSet].aVecB[3] = D3DXVECTOR3((pPos->x + pvtxMin->x) - pModel->modelSet[nModelSet].aPos[3].x, 0.0f, (pPos->z + pvtxMax->z) - pModel->modelSet[nModelSet].aPos[3].z);

			// �O�ς�p���������蔻�� 
			pModel->modelSet[nModelSet].fVec[0] = (pModel->modelSet[nModelSet].aVecA[0].z * pModel->modelSet[nModelSet].aVecB[0].x) - (pModel->modelSet[nModelSet].aVecA[0].x * pModel->modelSet[nModelSet].aVecB[0].z);
			pModel->modelSet[nModelSet].fVec[1] = (pModel->modelSet[nModelSet].aVecA[1].z * pModel->modelSet[nModelSet].aVecB[1].x) - (pModel->modelSet[nModelSet].aVecA[1].x * pModel->modelSet[nModelSet].aVecB[1].z);
			pModel->modelSet[nModelSet].fVec[2] = (pModel->modelSet[nModelSet].aVecA[2].z * pModel->modelSet[nModelSet].aVecB[2].x) - (pModel->modelSet[nModelSet].aVecA[2].x * pModel->modelSet[nModelSet].aVecB[2].z);
			pModel->modelSet[nModelSet].fVec[3] = (pModel->modelSet[nModelSet].aVecA[3].z * pModel->modelSet[nModelSet].aVecB[3].x) - (pModel->modelSet[nModelSet].aVecA[3].x * pModel->modelSet[nModelSet].aVecB[3].z);

			// ���f���͈̔͂ɂ���ꍇ
			if (pModel->modelSet[nModelSet].fVec[0] > 0.0f && pModel->modelSet[nModelSet].fVec[1] > 0.0f &&
				pModel->modelSet[nModelSet].fVec[2] > 0.0f && pModel->modelSet[nModelSet].fVec[3] > 0.0f)
			{

				if ((pPosOld->y >= pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMaxModel.y) &&
					(pPos->y < pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMaxModel.y))
				{
					// ���f���̏ォ����荞�񂾏ꍇ�A���f���̏�ɗ���
					pPos->y = pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMaxModel.y;

					bLandModel = true;
				}
				else if ((pPosOld->y + pvtxMax->y + 25.0f <= pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMinModel.y) &&
					(pPos->y + pvtxMax->y + 25.0f > pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMinModel.y))
				{
					// ���f���̉�������荞�񂾏ꍇ�A���荞�܂Ȃ��悤�Ƀ��f���̉����W�ɂ��A�ړ��ʂ�������
					pPos->y = pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMinModel.y - pvtxMax->y - 25.0f;
					pMove->y = 0.0f;
				}
				else if (pPos->y < pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMaxModel.y &&
					pPos->y + pvtxMax->y + 25.0f > pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMinModel.y)
				{
					// ���ʂɓ��������ꍇ�Q�Ƃ������W��O�̍��W�ɖ߂�
					pPos->z = pPosOld->z;
					pMove->z = 0.0f;
					pPos->x = pPosOld->x;
					pMove->x = 0.0f;
				}	
				if (pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMaxModel.y <= pPos->y)
				{
					// ���f���̏�ɑΏە�������ꍇ�A�e�̈ʒu�����f���̏�ɂ���
					SetPositionShadow(nIdxShadow, D3DXVECTOR3(pPos->x, pModel->modelSet[nModelSet].pos.y + pModel->modelInfo[pModel->modelSet[nModelSet].nIdx].vtxMaxModel.y, pPos->z));
				}
			}
		}
	}

	return bLandModel;
}

//--------------------------------------------------------------------------------------------------------------
// ���f���z�u���̓ǂݍ���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void LoadSetModel(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/model.txt", "r");
	
	// �ϐ��̏�����
	g_ModelSetInfo.nCntModel = 0;

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
				fscanf(pFile, "%s %d", &aText[0], &g_ModelSetInfo.nNumModel);
			}
			// MODEL_FILENAME����������X�t�@�C�������i�[
			if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < g_ModelSetInfo.nNumModel)
			{
				fscanf(pFile, "%s %s", &aText[0], &g_ModelSetInfo.modelInfo[nCntModel].aFileName[0]);

				// ���f���̑��������i�[����̂ŃJ�E���g�𑝂₷
				nCntModel++;
			}
			// �e�p�[�c�ɏ���ǂݍ��ދ�������Ƃ�����
			if (strcmp(&aText[0], "MODELSET") == 0)
			{
				// �ϐ������Z
				nCntModelSet++;

				// �z�u���郂�f���������Z
				g_ModelSetInfo.nCntModel++;

				bModelSet = true;
			}
			if (bModelSet == true)
			{
				// TYPE����������ԍ����i�[
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &g_ModelSetInfo.modelSet[nCntModelSet].nIdx);
				}
				// POS������������W�����i�[
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&g_ModelSetInfo.modelSet[nCntModelSet].pos.x,
						&g_ModelSetInfo.modelSet[nCntModelSet].pos.y,
						&g_ModelSetInfo.modelSet[nCntModelSet].pos.z);
				}
				// ROT����������p�x�����i�[
				if (strcmp(&aText[0], "ROT") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&rot.x,
						&rot.y,
						&rot.z);

					g_ModelSetInfo.modelSet[nCntModelSet].rot.x = (rot.x / 180.0f) * D3DX_PI;
					g_ModelSetInfo.modelSet[nCntModelSet].rot.y = (rot.y / 180.0f) * D3DX_PI;
					g_ModelSetInfo.modelSet[nCntModelSet].rot.z = (rot.z / 180.0f) * D3DX_PI;
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

//--------------------------------------------------------------------------------------------------------------
// ���f���z�u���̎擾
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FModelSet	- ���f���͔z�u����n��
//--------------------------------------------------------------------------------------------------------------
ModelSet GetmodelSet(void)
{
	return g_ModelSetInfo.modelSet[0];
}