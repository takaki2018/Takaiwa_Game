//-----------------------------------------------------------------
//
// ���f���Z�b�g (model_set.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "model_set.h"
#include "shadow.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
Model g_Model;												// ���f�����

//-----------------------------------------------------------------
// ���f���Z�b�g�̏���������
//-----------------------------------------------------------------
HRESULT InitModelSet(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	// �e�L�X�g�t�@�C���̓ǂݍ���
	LoadModelSet();

	for (int nCnt = 0; nCnt < g_Model.nNumModel; nCnt++)
	{
		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(&g_Model.modelInfo[nCnt].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Model.modelInfo[nCnt].pBuffMatModel,
			NULL,
			&g_Model.modelInfo[nCnt].nNumMatModel,
			&g_Model.modelInfo[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}

		// ���f���̃e�N�X�`��
		TexModel(nCnt);

		// ���f���̒��_
		VecModel(nCnt);
	}
	// ���f���̔z�u
	for (int nCntModel = 0; nCntModel < g_Model.nCntModel; nCntModel++)
	{
		// �g�p��Ԃ�true�ɂ���
		g_Model.modelSet[nCntModel].bUse = true;
	}

	return S_OK;
}

//-----------------------------------------------------------------
// ���f���Z�b�g�̏I������
//-----------------------------------------------------------------
void UninitModelSet(void)
{
	for (int nCnt = 0; nCnt < g_Model.nNumModel; nCnt++)
	{
		// ���b�V�����̊J��
		if (g_Model.modelInfo[nCnt].pBuffMatModel != NULL)
		{
			g_Model.modelInfo[nCnt].pBuffMatModel->Release();
			g_Model.modelInfo[nCnt].pBuffMatModel = NULL;
		}

		// �}�e���A�����̊J��
		if (g_Model.modelInfo[nCnt].pMeshModel != NULL)
		{
			g_Model.modelInfo[nCnt].pMeshModel->Release();
			g_Model.modelInfo[nCnt].pMeshModel = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ���f���Z�b�g�̍X�V����
//-----------------------------------------------------------------
void UpdateModelSet(void)
{

}

//-----------------------------------------------------------------
// ���f���Z�b�g�̕`�揈��
//-----------------------------------------------------------------
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
		if (g_Model.modelSet[nModelSet].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Model.modelSet[nModelSet].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.modelSet[nModelSet].rot.y, g_Model.modelSet[nModelSet].rot.x, g_Model.modelSet[nModelSet].rot.z);
			D3DXMatrixMultiply(&g_Model.modelSet[nModelSet].mtxWorld, &g_Model.modelSet[nModelSet].mtxWorld, &mtxRot);

			// ���_���W�̕␳
			D3DXVec3TransformCoord(&g_Model.modelSet[nModelSet].vtxMaxModel, &g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].vtxMaxModel, &g_Model.modelSet[nModelSet].mtxWorld);
			D3DXVec3TransformCoord(&g_Model.modelSet[nModelSet].vtxMinModel, &g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].vtxMinModel, &g_Model.modelSet[nModelSet].mtxWorld);

			// �ő�l�ŏ��l�̕␳
			VecModelSet(nModelSet);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Model.modelSet[nModelSet].pos.x, g_Model.modelSet[nModelSet].pos.y, g_Model.modelSet[nModelSet].pos.z);
			D3DXMatrixMultiply(&g_Model.modelSet[nModelSet].mtxWorld, &g_Model.modelSet[nModelSet].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Model.modelSet[nModelSet].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].nNumMatModel; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].apTextureModelSet[nCntMat]);

				// ���f��(�p�[�c)�̕`��
				g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].pMeshModel->DrawSubset(nCntMat);
			}
			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------
// ���f���̃e�N�X�`��
//-----------------------------------------------------------------
void TexModel(int nModelSet)
{
	// �ϐ��錾
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_Model.modelInfo[nModelSet].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Model.modelInfo[nModelSet].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_Model.modelInfo[nModelSet].apTextureModelSet[nCntMat]);
		}
	}
}

//-----------------------------------------------------------------
// ���f���̒��_���W
//-----------------------------------------------------------------
void VecModel(int nModelSet)
{
	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	// ���_���̎擾
	nNumVtx = g_Model.modelInfo[nModelSet].pMeshModel->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_Model.modelInfo[nModelSet].pMeshModel->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	g_Model.modelInfo[nModelSet].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_Model.modelInfo[nModelSet].vtxMinModel.x > vtx.x)
		{// X���W�̍ŏ��l
			g_Model.modelInfo[nModelSet].vtxMinModel.x = vtx.x;
		}
		else if (g_Model.modelInfo[nModelSet].vtxMaxModel.x < vtx.x)
		{// X���W�̍ő�l
			g_Model.modelInfo[nModelSet].vtxMaxModel.x = vtx.x;
		}

		if (g_Model.modelInfo[nModelSet].vtxMinModel.y > vtx.y)
		{// Y���W�̍ŏ��l
			g_Model.modelInfo[nModelSet].vtxMinModel.y = vtx.y;
		}
		else if (g_Model.modelInfo[nModelSet].vtxMaxModel.y < vtx.y)
		{// Y���W�̍ő�l
			g_Model.modelInfo[nModelSet].vtxMaxModel.y = vtx.y;
		}

		if (g_Model.modelInfo[nModelSet].vtxMinModel.z > vtx.z)
		{// Z���W�̍ŏ��l
			g_Model.modelInfo[nModelSet].vtxMinModel.z = vtx.z;
		}
		else if (g_Model.modelInfo[nModelSet].vtxMaxModel.z < vtx.z)
		{// Z���W�̍ő�l
			g_Model.modelInfo[nModelSet].vtxMaxModel.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_Model.modelInfo[nModelSet].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// ���f���̒��_���W�̕␳
//-----------------------------------------------------------------
void VecModelSet(int nNumModelSet)
{
	// �ő�l�ƍŏ��l�̓���ւ�
	if (g_Model.modelSet[nNumModelSet].vtxMaxModel.x < g_Model.modelSet[nNumModelSet].vtxMinModel.x)
	{
		// �ϐ��錾
		float fVecX;		// �ۑ��p

		// �ő�l�ƍŏ��l�����ւ���
		fVecX = g_Model.modelSet[nNumModelSet].vtxMaxModel.x;
		g_Model.modelSet[nNumModelSet].vtxMaxModel.x = g_Model.modelSet[nNumModelSet].vtxMinModel.x;
		g_Model.modelSet[nNumModelSet].vtxMinModel.x = fVecX;
	}
	if (g_Model.modelSet[nNumModelSet].vtxMaxModel.z < g_Model.modelSet[nNumModelSet].vtxMinModel.z)
	{
		// �ϐ��錾
		float fVecZ;		// �ۑ��p

		// �ő�l�ƍŏ��l�����ւ���
		fVecZ = g_Model.modelSet[nNumModelSet].vtxMaxModel.z;
		g_Model.modelSet[nNumModelSet].vtxMaxModel.z = g_Model.modelSet[nNumModelSet].vtxMinModel.z;
		g_Model.modelSet[nNumModelSet].vtxMinModel.z = fVecZ;
	}
}

//-----------------------------------------------------------------
// �O�ς�p�������f���̓����蔻��
//-----------------------------------------------------------------
void CrossProductModelSet(int nModelSet)
{
	// �ϐ��̏�����
	g_Model.modelSet[nModelSet].aPos[0] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMinModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMinModel.z);
	g_Model.modelSet[nModelSet].aPos[1] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMinModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMaxModel.z);
	g_Model.modelSet[nModelSet].aPos[2] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMaxModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMaxModel.z);
	g_Model.modelSet[nModelSet].aPos[3] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMaxModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMinModel.z);

	// �ϐ��̏�����
	g_Model.modelSet[nModelSet].aVecA[0] = g_Model.modelSet[nModelSet].aPos[1] - g_Model.modelSet[nModelSet].aPos[0];
	g_Model.modelSet[nModelSet].aVecA[1] = g_Model.modelSet[nModelSet].aPos[2] - g_Model.modelSet[nModelSet].aPos[1];
	g_Model.modelSet[nModelSet].aVecA[2] = g_Model.modelSet[nModelSet].aPos[3] - g_Model.modelSet[nModelSet].aPos[2];
	g_Model.modelSet[nModelSet].aVecA[3] = g_Model.modelSet[nModelSet].aPos[0] - g_Model.modelSet[nModelSet].aPos[3];
}

//-----------------------------------------------------------------
// ���f���̓����蔻��
//-----------------------------------------------------------------
bool CollisionModelSet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// �\���̂̃|�C���^�ϐ�
	Model *pModel = &g_Model;

	// �ϐ��錾
	bool bCollisionModel = false;		// �R�C�����l���������ǂ���

	for (int nModelSet = 0; nModelSet < MAX_MODEL; nModelSet++)
	{
		if (pModel->modelSet[nModelSet].bUse == true)
		{
			// �O�ς�p���������蔻��
			CrossProductModelSet(nModelSet);

			// �O�ς�p���������蔻��
			pModel->modelSet[nModelSet].aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pModel->modelSet[nModelSet].aPos[0].x, 0.0f, pPos->z - pModel->modelSet[nModelSet].aPos[0].z);
			pModel->modelSet[nModelSet].aVecB[1] = D3DXVECTOR3(pPos->x - pModel->modelSet[nModelSet].aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pModel->modelSet[nModelSet].aPos[1].z);
			pModel->modelSet[nModelSet].aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pModel->modelSet[nModelSet].aPos[2].x, 0.0f, pPos->z - pModel->modelSet[nModelSet].aPos[2].z);
			pModel->modelSet[nModelSet].aVecB[3] = D3DXVECTOR3(pPos->x - pModel->modelSet[nModelSet].aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pModel->modelSet[nModelSet].aPos[3].z);

			// �O�ς̒l
			pModel->modelSet[nModelSet].fVec[0] = (pModel->modelSet[nModelSet].aVecA[0].z * pModel->modelSet[nModelSet].aVecB[0].x) - (pModel->modelSet[nModelSet].aVecA[0].x * pModel->modelSet[nModelSet].aVecB[0].z);
			pModel->modelSet[nModelSet].fVec[1] = (pModel->modelSet[nModelSet].aVecA[1].z * pModel->modelSet[nModelSet].aVecB[1].x) - (pModel->modelSet[nModelSet].aVecA[1].x * pModel->modelSet[nModelSet].aVecB[1].z);
			pModel->modelSet[nModelSet].fVec[2] = (pModel->modelSet[nModelSet].aVecA[2].z * pModel->modelSet[nModelSet].aVecB[2].x) - (pModel->modelSet[nModelSet].aVecA[2].x * pModel->modelSet[nModelSet].aVecB[2].z);
			pModel->modelSet[nModelSet].fVec[3] = (pModel->modelSet[nModelSet].aVecA[3].z * pModel->modelSet[nModelSet].aVecB[3].x) - (pModel->modelSet[nModelSet].aVecA[3].x * pModel->modelSet[nModelSet].aVecB[3].z);

			if (pModel->modelSet[nModelSet].fVec[0] > 0.0f && pModel->modelSet[nModelSet].fVec[1] > 0.0f &&
				pModel->modelSet[nModelSet].fVec[2] > 0.0f && pModel->modelSet[nModelSet].fVec[3] > 0.0f)
			{
				if ((pPosOld->y >= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y) && 
					(pPos->y < pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y))
				{// ���f���̏�ɗ���
					pPos->y = pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y;

					bCollisionModel = true;
				}
				else if ((pPosOld->y + pvtxMax->y <= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y) &&
					(pPos->y + pvtxMax->y > pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y))
				{
					pPos->y = pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y - pvtxMax->y;
				}
				else if (pPos->y <= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y - pvtxMin->y &&
					pPos->y >= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y - pvtxMax->y)
				{
					if (pPos->x > pPosOld->x && (pPosOld->x < pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMinModel.x))
					{// �v���C���[�������瓖�������Ƃ�
						pPos->x = pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMinModel.x - pvtxMax->x;
					}
					else if (pPos->x < pPosOld->x && (pPosOld->x > pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMaxModel.x))
					{// �v���C���[���E���瓖�������Ƃ�
						pPos->x = pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMaxModel.x - pvtxMin->x;
					}
					else if (pPos->z > pPosOld->z && pPosOld->z < pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMinModel.z)
					{// �v���C���[����O���瓖�������Ƃ�
						pPos->z = pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMinModel.z - pvtxMax->z;
					}
					else if (pPos->z < pPosOld->z && pPosOld->z > pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMaxModel.z)
					{// �v���C���[�������瓖�������Ƃ�
						pPos->z = pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMaxModel.z - pvtxMin->z;
					}

				}
			}
		}
	}

	return bCollisionModel;
}

//-----------------------------------------------------------------
// ���f���z�u���̓ǂݍ���
//-----------------------------------------------------------------
void LoadModelSet(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/model.txt", "r");
	
	// �ϐ��̏�����
	g_Model.nCntModel = 0;

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
				fscanf(pFile, "%s %d", &aText[0], &g_Model.nNumModel);
			}
			// MODEL_FILENAME����������X�t�@�C�������i�[
			if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < g_Model.nNumModel)
			{
				fscanf(pFile, "%s %s", &aText[0], &g_Model.modelInfo[nCntModel].aFileName[0]);

				// ���f���̑��������i�[����̂ŃJ�E���g�𑝂₷
				nCntModel++;
			}
			// �e�p�[�c�ɏ���ǂݍ��ދ�������Ƃ�����
			if (strcmp(&aText[0], "MODELSET") == 0)
			{
				// �ϐ������Z
				nCntModelSet++;

				// �z�u���郂�f���������Z
				g_Model.nCntModel++;

				bModelSet = true;
			}
			if (bModelSet == true)
			{
				// TYPE����������ԍ����i�[
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &g_Model.modelSet[nCntModelSet].nIdx);
				}
				// POS������������W�����i�[
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&g_Model.modelSet[nCntModelSet].pos.x,
						&g_Model.modelSet[nCntModelSet].pos.y,
						&g_Model.modelSet[nCntModelSet].pos.z);
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

					g_Model.modelSet[nCntModelSet].rot.x = rot.x * D3DX_PI;
					g_Model.modelSet[nCntModelSet].rot.y = rot.y * D3DX_PI;
					g_Model.modelSet[nCntModelSet].rot.z = rot.z * D3DX_PI;
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