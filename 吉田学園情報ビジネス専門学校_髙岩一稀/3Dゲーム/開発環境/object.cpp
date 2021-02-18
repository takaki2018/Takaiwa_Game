//-----------------------------------------------------------------
//
// �I�u�W�F�N�g (object.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "object.h"
#include "wall.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_OBJECT			(1)
#define POLYGON_SIZE		(5.0f)		// �|���S���̃T�C�Y
#define MOVE_MODEL			(0.8f)
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
Object g_aCoin[MAX_OBJECT];											// ���f�����
D3DXVECTOR3 g_vtxMinObject, g_vtxMaxObject;						// �e���_���W�̍ŏ��l�A�ő�l
LPDIRECT3DTEXTURE9 g_apTextureObject[8] = {};					// �e�N�X�`���ւ̃|�C���^

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̏���������
//-----------------------------------------------------------------
HRESULT InitObject(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_vtxMinObject = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_vtxMaxObject = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		// ���̏�����
		g_aCoin[nCnt].pMeshModel = NULL;
		g_aCoin[nCnt].pBuffMatModel = NULL;
		g_aCoin[nCnt].nNumMatModel = NULL;
		g_aCoin[nCnt].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aCoin[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].rotDest = g_aCoin[nCnt].rot;
		g_aCoin[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].nIdx = -1;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/test.x",
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
	}
	// �ϐ��錾
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aCoin[0].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aCoin[0].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureObject[nCntMat]);
		}
	}

	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	// ���_���̎擾
	nNumVtx = g_aCoin[0].pMeshModel->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_aCoin[0].pMeshModel->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	g_aCoin[0].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_vtxMinObject.x > vtx.x)
		{// X���W�̍ŏ��l
			g_vtxMinObject.x = vtx.x;
		}
		else if (g_vtxMaxObject.x < vtx.x)
		{// X���W�̍ő�l
			g_vtxMaxObject.x = vtx.x;
		}

		if (g_vtxMinObject.y > vtx.y)
		{// Y���W�̍ŏ��l
			g_vtxMinObject.y = vtx.y;
		}
		else if (g_vtxMaxObject.y < vtx.y)
		{// Y���W�̍ő�l
			g_vtxMaxObject.y = vtx.y;
		}

		if (g_vtxMinObject.z > vtx.z)
		{// Z���W�̍ŏ��l
			g_vtxMinObject.z = vtx.z;
		}
		else if (g_vtxMaxObject.z < vtx.z)
		{// Z���W�̍ő�l
			g_vtxMaxObject.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_aCoin[0].pMeshModel->UnlockVertexBuffer();

	// �e�̔z�u
	g_aCoin[0].nIdx = SetShadow(D3DXVECTOR3(g_aCoin[0].pos.x, 0.0f, g_aCoin[0].pos.z), SHADOW_SIZE, SHADOW_SIZE);

	return S_OK;
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̏I������
//-----------------------------------------------------------------
void UninitObject(void)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
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
// �I�u�W�F�N�g�̍X�V����
//-----------------------------------------------------------------
void UpdateObject(void)
{

}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̕`�揈��
//-----------------------------------------------------------------
void DrawObject(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

									// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCoin[0].mtxWorld);

	// �X�P�[���𔽉f
	//g_model[0].mtxWorld._11 = 0.8f;
	//g_model[0].mtxWorld._22 = 0.8f;
	//g_model[0].mtxWorld._33 = 0.8f;

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCoin[0].rot.y, g_aCoin[0].rot.x, g_aCoin[0].rot.z);
	D3DXMatrixMultiply(&g_aCoin[0].mtxWorld, &g_aCoin[0].mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aCoin[0].pos.x, g_aCoin[0].pos.y, g_aCoin[0].pos.z);
	D3DXMatrixMultiply(&g_aCoin[0].mtxWorld, &g_aCoin[0].mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aCoin[0].mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aCoin[0].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aCoin[0].nNumMatModel; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureObject[nCntMat]);

		// ���f��(�p�[�c)�̕`��
		g_aCoin[0].pMeshModel->DrawSubset(nCntMat);
	}
	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̓����蔻��
//-----------------------------------------------------------------
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// �\���̂̃|�C���^�ϐ�
	Object *pObject = &g_aCoin[0];

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++,pObject++)
	{
		if ((pPos->x - pvtxMin->x) < (pObject->pos.x + g_vtxMaxObject.x) &&
			(pPos->x + pvtxMax->x) > (pObject->pos.x - g_vtxMinObject.x))
		{// �v���C���[���u���b�N�̒��ɂ���Ƃ�
			if (pPosOld->z + pvtxMax->z <= (pObject->pos.z - g_vtxMinObject.z))
			{// �v���C���[���u���b�N�̏�ɂ���Ƃ�
				if (pPos->z + pvtxMax->z >(pObject->pos.z - g_vtxMinObject.z))
				{// �߂荞�񂾂珈��
					pPos->z = pObject->pos.z - g_vtxMinObject.z - pvtxMax->z;
				}
			}
			else if ((pPosOld->z - pvtxMin->z) >= (pObject->pos.z + g_vtxMaxObject.z))
			{// �v���C���[���u���b�N�̉��ɂ���Ƃ�
				if ((pPos->z - pvtxMin->z) < (pObject->pos.z + g_vtxMaxObject.z))
				{// �߂荞�񂾂�u���b�N�̉��̍��W�ɂ���
					pPos->y = (pObject->pos.z + g_vtxMinObject.z + pvtxMin->z);
				}
			}
		}
		if ((pPos->z + pvtxMax->z) > pObject->pos.z - g_vtxMinObject.z && 
			(pPos->z - pvtxMin->z) < (pObject->pos.z + g_vtxMaxObject.z))
		{// �v���C���[���u���b�N��Y���ɂ���Ƃ�
			if ((pPosOld->x + pvtxMax->x) <= pObject->pos.x - g_vtxMinObject.x)
			{// �v���C���[���u���b�N�̍����ɂ���Ƃ�
				if ((pPos->x + pvtxMax->x) > pObject->pos.x - g_vtxMinObject.x)
				{// �߂荞�񂾂�u���b�N�̍��̍��W�ɂ���
					pPos->x = pObject->pos.x - g_vtxMinObject.x - pvtxMax->x;
				}
			}
			else if ((pPosOld->x - pvtxMin->x) >= (pObject->pos.x + g_vtxMaxObject.x))
			{// �v���C���[���u���b�N�̉E���ɂ���Ƃ�
				if ((pPos->x - pvtxMin->x) < (pObject->pos.x + g_vtxMaxObject.x))
				{// �߂荞�񂾂�u���b�N�̉E�̍��W�ɂ���
					pPos->x = pObject->pos.x - g_vtxMinObject.x + pvtxMin->x;
				}
			}
		}
	}
}