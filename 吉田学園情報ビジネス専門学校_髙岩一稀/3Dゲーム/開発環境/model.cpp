//-----------------------------------------------------------------
//
// ���f�� (model.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "model.h"
#include "wall.h"
#include "object.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_MODEL			(1)
#define POLYGON_SIZE		(5.0f)		// �|���S���̃T�C�Y
#define MOVE_MODEL			(0.8f)
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
TestModel g_model[MAX_MODEL];												// ���f�����
D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;						// �e���_���W�̍ŏ��l�A�ő�l
LPDIRECT3DTEXTURE9 g_apTextureModel[8] = {};					// �e�N�X�`���ւ̃|�C���^

//-----------------------------------------------------------------
// ���f���̏���������
//-----------------------------------------------------------------
HRESULT InitModel(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_vtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_vtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		// ���̏�����
		g_model[nCnt].pMesh = NULL;
		g_model[nCnt].pBuffMat = NULL;
		g_model[nCnt].nNumMat = NULL;
		g_model[nCnt].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_model[nCnt].posOld = g_model[nCnt].pos;
		g_model[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[nCnt].rotDest = g_model[nCnt].rot;
		g_model[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[nCnt].nIdx = -1;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/test.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_model[nCnt].pBuffMat,
			NULL,
			&g_model[nCnt].nNumMat,
			&g_model[nCnt].pMesh)))
		{
			return E_FAIL;
		}
	}
	// �ϐ��錾
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_model[0].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_model[0].nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureModel[nCntMat]);
		}
	}

	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	// ���_���̎擾
	nNumVtx = g_model[0].pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_model[0].pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	g_model[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY,(void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_vtxMinModel.x > vtx.x)
		{// X���W�̍ŏ��l
			g_vtxMinModel.x = vtx.x;
		}
		else if (g_vtxMaxModel.x < vtx.x)
		{// X���W�̍ő�l
			g_vtxMaxModel.x = vtx.x;
		}

		if (g_vtxMinModel.y > vtx.y)
		{// Y���W�̍ŏ��l
			g_vtxMinModel.y = vtx.y;
		}
		else if (g_vtxMaxModel.y < vtx.y)
		{// Y���W�̍ő�l
			g_vtxMaxModel.y = vtx.y;
		}

		if (g_vtxMinModel.z > vtx.z)
		{// Z���W�̍ŏ��l
			g_vtxMinModel.z = vtx.z;
		}
		else if (g_vtxMaxModel.z < vtx.z)
		{// Z���W�̍ő�l
			g_vtxMaxModel.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_model[0].pMesh->UnlockVertexBuffer();

	// �e�̔z�u
	g_model[0].nIdx = SetShadow(D3DXVECTOR3(g_model[0].pos.x,0.0f, g_model[0].pos.z), SHADOW_SIZE, SHADOW_SIZE);

	return S_OK;
}

//-----------------------------------------------------------------
// ���f���̏I������
//-----------------------------------------------------------------
void UninitModel(void)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		// ���b�V�����̊J��
		if (g_model[nCnt].pBuffMat != NULL)
		{
			g_model[nCnt].pBuffMat->Release();
			g_model[nCnt].pBuffMat = NULL;
		}

		// �}�e���A�����̊J��
		if (g_model[nCnt].pMesh != NULL)
		{
			g_model[nCnt].pMesh->Release();
			g_model[nCnt].pMesh = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ���f���̍X�V����
//-----------------------------------------------------------------
void UpdateModel(void)
{
	// ���̎擾
	Camera camera = GetCamera();		// �J�������
	Wall *pWall = GetWall();			// �Ǐ��

	// ���f���̈ʒu��ۑ�
	g_model[0].posOld = g_model[0].pos;

	// �ړI�̉�]�p�𐧌�
	if (g_model[0].rotDest.y - g_model[0].rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		g_model[0].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_model[0].rotDest.y - g_model[0].rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		g_model[0].rotDest.y -= D3DX_PI * 2.0f;
	}
	// �����̍X�V
	g_model[0].rot.y += (g_model[0].rotDest.y - g_model[0].rot.y) * 0.08f;

	// ���݂̉�]�p�𐧌�
	if (g_model[0].rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		g_model[0].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_model[0].rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		g_model[0].rot.y -= D3DX_PI * 2.0f;
	}

#if 1
	if (GetKeyboardPress(KEYINFO_UP) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// ���f���΂߉E��ړ�
			g_model[0].move.z -= MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_model[0].move.x += MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y - PI_QUARTER * 3.0f;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// ���f���΂߉E��ړ�
			g_model[0].move.z += MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_model[0].move.x -= MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y + PI_QUARTER * 3.0f;
		}
		else
		{// ���f����ړ�
			g_model[0].move.z += MOVE_MODEL * cosf(camera.rot.y);
			g_model[0].move.x += MOVE_MODEL * sinf(camera.rot.y);
			g_model[0].rotDest.y = camera.rot.y - D3DX_PI;
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// ���f���΂߉E��ړ�
			g_model[0].move.z -= MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_model[0].move.x += MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y - PI_QUARTER;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// ���f���΂߉E��ړ�
			g_model[0].move.z += MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_model[0].move.x -= MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y + PI_QUARTER;
		}
		else
		{// ���f�����ړ�
			g_model[0].move.z -= MOVE_MODEL * cosf(camera.rot.y);
			g_model[0].move.x -= MOVE_MODEL * sinf(camera.rot.y);
			g_model[0].rotDest.y = camera.rot.y;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// ���f���E�ړ�
		g_model[0].move.z -= MOVE_MODEL * sinf(camera.rot.y);
		g_model[0].move.x += MOVE_MODEL * cosf(camera.rot.y);
		g_model[0].rotDest.y = camera.rot.y - D3DX_PI / 2.0f;
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// ���f�����ړ�
		g_model[0].move.z += MOVE_MODEL * sinf(camera.rot.y);
		g_model[0].move.x -= MOVE_MODEL * cosf(camera.rot.y);
		g_model[0].rotDest.y = camera.rot.y + D3DX_PI / 2.0f;
	}
	if (GetKeyboardPress(KEYINFO_MODEL_RIGHT_TURN) == true)
	{// �����_�̍���]
		g_model[0].rotDest.y += 0.1f;						// ��]��
	}
	if (GetKeyboardPress(KEYINFO_MODEL_LEFT_TURN) == true)
	{// �����_�̉E��]
		g_model[0].rotDest.y -= 0.1f;						// ��]��
	}
#else
	// �Q�[���p�b�h���̎擾
	DIJOYSTATE2 Controller = GetControllerPless(PLAYER_1);

	// JOYPAD�̃X�e�B�b�N����
	if (Controller.lX != 0 || Controller.lY != 0)
	{// �X�e�B�b�N�̓��͊p�x
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY * -1);

		g_model[0].rot.y = camera.rot.y + (D3DX_PI + fAngle);					// �����̐ݒ�
		g_model[0].move.x += MOVE_MODEL * sinf(camera.rot.y + fAngle);			// X���ړ��ʂ̌���
		g_model[0].move.z += MOVE_MODEL * cosf(camera.rot.y + fAngle);			// Z���ړ��ʂ̌���
	}
#endif
	// �ړ��ʂ̌���(����)
	g_model[0].move.x += (0.0f - g_model[0].move.x) * 0.1f;
	g_model[0].move.z += (0.0f - g_model[0].move.z) * 0.1f;

	// �ʒu�̍X�V
	g_model[0].pos.x += g_model[0].move.x;
	g_model[0].pos.z += g_model[0].move.z;

	// �I�u�W�F�N�g�Ƃ̓����蔻��
	//CollisionObject(&g_model[0].pos, &g_model[0].posOld, &g_vtxMinModel,&g_vtxMaxModel);

	// �e�̔z�u
	SetPositionShadow(g_model[0].nIdx, D3DXVECTOR3(g_model[0].pos.x, 0.0f, g_model[0].pos.z));

	// ���Z�b�g
	if (GetKeyboardTrigger(KEYINFO_OK) == true)
	{
		g_model[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_model[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[0].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//-----------------------------------------------------------------
// ���f���̕`�揈��
//-----------------------------------------------------------------
void DrawModel(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_model[0].mtxWorld);

	// �X�P�[���𔽉f
	//g_model[0].mtxWorld._11 = 0.8f;
	//g_model[0].mtxWorld._22 = 0.8f;
	//g_model[0].mtxWorld._33 = 0.8f;

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[0].rot.y, g_model[0].rot.x, g_model[0].rot.z);
	D3DXMatrixMultiply(&g_model[0].mtxWorld, &g_model[0].mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_model[0].pos.x, g_model[0].pos.y, g_model[0].pos.z);
	D3DXMatrixMultiply(&g_model[0].mtxWorld, &g_model[0].mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_model[0].mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_model[0].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_model[0].nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		// ���f��(�p�[�c)�̕`��
		g_model[0].pMesh->DrawSubset(nCntMat);
	}
	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
#if 0
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_model[1].mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[0].rot.y, g_model[0].rot.x, g_model[0].rot.z);
	D3DXMatrixMultiply(&g_model[1].mtxWorld, &g_model[1].mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_model[0].pos.x, g_model[0].pos.y + 15.0f, g_model[0].pos.z);
	D3DXMatrixMultiply(&g_model[1].mtxWorld, &g_model[1].mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_model[1].mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_model[0].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_model[1].nNumMatModel; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// ���f��(�p�[�c)�̕`��
		g_model[1].pMeshModel->DrawSubset(nCntMat);
	}
	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
#endif
}

//-----------------------------------------------------------------
// ���f���̈ʒu���̎擾
//-----------------------------------------------------------------
TestModel *GetModel(void)
{
	return &g_model[0];
}