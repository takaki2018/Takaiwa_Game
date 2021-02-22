//-----------------------------------------------------------------
//
// �G (enemy.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "enemy.h"
#include "wall.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "score.h"
#include "model_set.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_ENEMY			(8)
#define PI_QUARTER			(D3DX_PI / 4.0f)

#define MOVE_ENEMY			(1.0f)							// �G�̈ړ���
#define RENGE				(300.0f)						// �G���v���C���[��������͈�

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
Enemy g_aEnemy[MAX_ENEMY];										// ���f�����
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_ENEMY] = {};			// �e�N�X�`���ւ̃|�C���^

//-----------------------------------------------------------------
// �G�̏���������
//-----------------------------------------------------------------
HRESULT InitEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// ���̏�����
		g_aEnemy[nCnt].pMeshModel = NULL;
		g_aEnemy[nCnt].pBuffMatModel = NULL;
		g_aEnemy[nCnt].nNumMatModel = NULL;
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rotDest = g_aEnemy[nCnt].rot;
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].nIdx = -1;
		g_aEnemy[nCnt].bUse = false;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/rat.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCnt].pBuffMatModel,
			NULL,
			&g_aEnemy[nCnt].nNumMatModel,
			&g_aEnemy[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}
		// �G�̒��_
		VecEnemy(nCnt);
	}

	// �G�̐ݒu
	SetEnemy(D3DXVECTOR3(100.0f, 0.0f, 200.0f));
	SetEnemy(D3DXVECTOR3(600.0f, 0.0f, -200.0f));
	SetEnemy(D3DXVECTOR3(-200.0f, 0.0f, 600.0f));

	return S_OK;
}

//-----------------------------------------------------------------
// �G�̏I������
//-----------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// ���b�V�����̊J��
		if (g_aEnemy[nCnt].pBuffMatModel != NULL)
		{
			g_aEnemy[nCnt].pBuffMatModel->Release();
			g_aEnemy[nCnt].pBuffMatModel = NULL;
		}

		// �}�e���A�����̊J��
		if (g_aEnemy[nCnt].pMeshModel != NULL)
		{
			g_aEnemy[nCnt].pMeshModel->Release();
			g_aEnemy[nCnt].pMeshModel = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �G�̍X�V����
//-----------------------------------------------------------------
void UpdateEnemy(void)
{
	// �ϐ��錾
	int nCntTrue = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// ���W�̕ۑ�
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			// �G�̈ړ�����
			MoveEnemy(nCntEnemy);

			// ���f���̓����蔻��
			CollisionModelSet(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move,&g_aEnemy[nCntEnemy].vtxMinModel, &g_aEnemy[nCntEnemy].vtxMaxModel,g_aEnemy[nCntEnemy].nIdx);
		}
	}
	if (nCntTrue == 0)
	{// �R�C���̐���0���̎��t�F�[�h�A�E�g
	 //SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------
// �G�̕`�揈��
//-----------------------------------------------------------------
void DrawEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatModel; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				// ���f��(�p�[�c)�̕`��
				g_aEnemy[nCntEnemy].pMeshModel->DrawSubset(nCntMat);
			}
			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------
// �G�̈ړ�����
//-----------------------------------------------------------------
void MoveEnemy(int nCntEnemy)
{
	// �\���̂̃|�C���^�ϐ�
	Player *pPlayer = GetPlayer();

	// �ϐ��錾
	D3DXVECTOR3 EnemyVec;		// �v���C���[�̕����Ɍ������邽�߂Ɏg�p

	// �ړI�̉�]�p�𐧌�
	if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
	}
	// �����̍X�V
	g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * 0.08f;

	// ���݂̉�]�p�𐧌�
	if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	// �G���v���C���[������悤�ɂ���
	// �G�ƃv���C���[�̕����x�N�g��
	EnemyVec = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

	// �p�x�����߂�
	float fAngle = atan2f(EnemyVec.x, EnemyVec.z);

	// ���@�ƒ��S�_�̑Ίp���̒���
	float fLength = sqrtf((EnemyVec.x * EnemyVec.x) + (EnemyVec.z * EnemyVec.z));

	if (fLength < RENGE)
	{
		// �v���C���[��ǔ�����
		g_aEnemy[nCntEnemy].move.x = MOVE_ENEMY * sinf(fAngle);
		g_aEnemy[nCntEnemy].move.z = MOVE_ENEMY * cosf(fAngle);

		// �����̍X�V
		g_aEnemy[nCntEnemy].rotDest.y = fAngle;

		// �ʒu�̍X�V
		g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
		g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;
	}

	// �O�ς�p���������蔻��Ɏg���x�N�g���̌v�Z
	CrossProductEnemy(nCntEnemy);

	// �e�̈ړ�
	//SetPositionShadow(g_aEnemy[nCntEnemy].nIdx, g_aEnemy[nCntEnemy].pos);
}

//-----------------------------------------------------------------
// �G�̐ݒu
//-----------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			// ���W�̐ݒ�
			g_aEnemy[nCntEnemy].pos = pos;

			// �G�̃e�N�X�`��
			TexEnemy(nCntEnemy);

			// �g�p��Ԃ�true�ɂ���
			g_aEnemy[nCntEnemy].bUse = true;

			// �e�̔z�u
			//g_aEnemy[nCntEnemy].nIdx = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), SHADOW_SIZE, SHADOW_SIZE);

			break;
		}
	}
}

//-----------------------------------------------------------------
// �G�̃e�N�X�`��
//-----------------------------------------------------------------
void TexEnemy(int nCntCoin)
{
	// �ϐ��錾
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureEnemy[nCntMat]);
		}
	}
}

//-----------------------------------------------------------------
// �G�̒��_���W
//-----------------------------------------------------------------
void VecEnemy(int nCntCoin)
{
	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

							// ���_���̎擾
	nNumVtx = g_aEnemy[nCntCoin].pMeshModel->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntCoin].pMeshModel->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	g_aEnemy[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_aEnemy[nCntCoin].vtxMinModel.x > vtx.x)
		{// X���W�̍ŏ��l
			g_aEnemy[nCntCoin].vtxMinModel.x = vtx.x;
		}
		else if (g_aEnemy[nCntCoin].vtxMaxModel.x < vtx.x)
		{// X���W�̍ő�l
			g_aEnemy[nCntCoin].vtxMaxModel.x = vtx.x;
		}

		if (g_aEnemy[nCntCoin].vtxMinModel.y > vtx.y)
		{// Y���W�̍ŏ��l
			g_aEnemy[nCntCoin].vtxMinModel.y = vtx.y;
		}
		else if (g_aEnemy[nCntCoin].vtxMaxModel.y < vtx.y)
		{// Y���W�̍ő�l
			g_aEnemy[nCntCoin].vtxMaxModel.y = vtx.y;
		}

		if (g_aEnemy[nCntCoin].vtxMinModel.z > vtx.z)
		{// Z���W�̍ŏ��l
			g_aEnemy[nCntCoin].vtxMinModel.z = vtx.z;
		}
		else if (g_aEnemy[nCntCoin].vtxMaxModel.z < vtx.z)
		{// Z���W�̍ő�l
			g_aEnemy[nCntCoin].vtxMaxModel.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_aEnemy[0].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// �O�ςɎg���x�N�g���̌v�Z
//-----------------------------------------------------------------
void CrossProductEnemy(int nCntCoin)
{
	// �ϐ��̏�����
	g_aEnemy[nCntCoin].aPos[0] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMinModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMinModel.z);
	g_aEnemy[nCntCoin].aPos[1] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMinModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMaxModel.z);
	g_aEnemy[nCntCoin].aPos[2] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMaxModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMaxModel.z);
	g_aEnemy[nCntCoin].aPos[3] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMaxModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMinModel.z);

	// �ϐ��̏�����
	g_aEnemy[nCntCoin].aVecA[0] = g_aEnemy[nCntCoin].aPos[1] - g_aEnemy[nCntCoin].aPos[0];
	g_aEnemy[nCntCoin].aVecA[1] = g_aEnemy[nCntCoin].aPos[2] - g_aEnemy[nCntCoin].aPos[1];
	g_aEnemy[nCntCoin].aVecA[2] = g_aEnemy[nCntCoin].aPos[3] - g_aEnemy[nCntCoin].aPos[2];
	g_aEnemy[nCntCoin].aVecA[3] = g_aEnemy[nCntCoin].aPos[0] - g_aEnemy[nCntCoin].aPos[3];
}

//-----------------------------------------------------------------
// �G�̓����蔻��
//-----------------------------------------------------------------
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// �\���̂̃|�C���^�ϐ�
	Enemy *pEnemy = &g_aEnemy[0];

	// �ϐ��錾
	bool bCollision = false;		// �����������ǂ���

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// �x�N�g���v�Z
			pEnemy->aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pEnemy->aPos[0].x, 0.0f, pPos->z - pEnemy->aPos[0].z);
			pEnemy->aVecB[1] = D3DXVECTOR3(pPos->x - pEnemy->aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pEnemy->aPos[1].z);
			pEnemy->aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pEnemy->aPos[2].x, 0.0f, pPos->z - pEnemy->aPos[2].z);
			pEnemy->aVecB[3] = D3DXVECTOR3(pPos->x - pEnemy->aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pEnemy->aPos[3].z);

			// �O�ς�p���������蔻��
			pEnemy->fVec[0] = (pEnemy->aVecA[0].z * pEnemy->aVecB[0].x) - (pEnemy->aVecA[0].x * pEnemy->aVecB[0].z);
			pEnemy->fVec[1] = (pEnemy->aVecA[1].z * pEnemy->aVecB[1].x) - (pEnemy->aVecA[1].x * pEnemy->aVecB[1].z);
			pEnemy->fVec[2] = (pEnemy->aVecA[2].z * pEnemy->aVecB[2].x) - (pEnemy->aVecA[2].x * pEnemy->aVecB[2].z);
			pEnemy->fVec[3] = (pEnemy->aVecA[3].z * pEnemy->aVecB[3].x) - (pEnemy->aVecA[3].x * pEnemy->aVecB[3].z);

			if (pEnemy->fVec[0] > 0.0f && pEnemy->fVec[1] > 0.0f &&
				pEnemy->fVec[2] > 0.0f && pEnemy->fVec[3] > 0.0f)
			{
				if ((pPosOld->y >= pEnemy->pos.y + pEnemy->vtxMaxModel.y) && (pPos->y < pEnemy->pos.y + pEnemy->vtxMaxModel.y))
				{// �G�𓥂񂾂����
					pPos->y = pEnemy->pos.y + pEnemy->vtxMaxModel.y;
					pMove->y *= -0.5f;
					
					// �G����������
					DeleteEnemy(nCntEnemy);
				}
				else if (pPos->y <= pEnemy->pos.y + pEnemy->vtxMaxModel.y&&
					pPos->y >= pEnemy->pos.y + pEnemy->vtxMinModel.y - pvtxMax->y)
				{
					if (pPos->x > pPosOld->x && (pPosOld->x < pEnemy->pos.x + pEnemy->vtxMinModel.x))
					{// �v���C���[�������瓖�������Ƃ�
						pPos->x = pEnemy->pos.x + pEnemy->vtxMinModel.x - pvtxMax->x;
					}
					else if (pPos->x < pPosOld->x && (pPosOld->x > pEnemy->pos.x + pEnemy->vtxMaxModel.x))
					{// �v���C���[���E���瓖�������Ƃ�
						pPos->x = pEnemy->pos.x + pEnemy->vtxMaxModel.x - pvtxMin->x;
					}
					else if (pPos->z > pPosOld->z && pPosOld->z < pEnemy->pos.z + pEnemy->vtxMinModel.z)
					{// �v���C���[����O���瓖�������Ƃ�
						pPos->z = pEnemy->pos.z + pEnemy->vtxMinModel.z - pvtxMax->z;
					}
					else if (pPos->z < pPosOld->z && pPosOld->z > pEnemy->pos.z + pEnemy->vtxMaxModel.z)
					{// �v���C���[�������瓖�������Ƃ�
						pPos->z = pEnemy->pos.z + pEnemy->vtxMaxModel.z - pvtxMin->z;
					}
					pMove->x = pEnemy->move.x * 10.0f;
					pMove->z = pEnemy->move.z * 10.0f;

					bCollision = true;
				}
			}
		}
	}

	return bCollision;
}

//-----------------------------------------------------------------
// �G���������鏈��
//-----------------------------------------------------------------
void DeleteEnemy(int nCntEnemy)
{
	// �e������
	//DeleteShadow(g_aEnemy[nCntEnemy].nIdx);

	// �g�p��Ԃ�false�ɂ���
	g_aEnemy[nCntEnemy].bUse = false;
}