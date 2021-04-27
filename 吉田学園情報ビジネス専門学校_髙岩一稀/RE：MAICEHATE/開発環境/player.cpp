//==============================================================================================================
//
// �v���C���[ (player.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "player.h"
#include "motion.h"
#include "coin.h"
#include "enemy.h"
#include "meshfield.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "motion.h"
#include "model_set.h"
#include "count_down.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MOVE_MODEL			(2.0f)
#define JUMP_PLAYER			(11.0f)
#define PI_QUARTER			(D3DX_PI / 4.0f)
#define GRAVITY				(0.5f)		// �d��

#define LENGTH				(8.0f)

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
Player g_player;						// �v���C���[���
int g_nCntMotion;

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �v���C���[���̓ǂݍ���
	LoadPlayerData();

	// �v���C���[���̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.bJump = false;
	g_player.bFall = false;
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.MotionType = MOTIONTYPE_NEUTRAL;
	g_player.MotionTypeOld = g_player.MotionType;
	g_player.nStateCounter = 0;
	g_player.nCounterMotion = 1;
	g_player.nKey = 0;
	g_player.bLoopMotion = g_player.aMotionInfo[g_player.MotionType].bLoop;
	g_player.nNumKey = g_player.aMotionInfo[g_player.MotionType].nNumKey;

	// �v���C���[�̃p�[�c�ݒ�
	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(g_player.aModel[nCntModel].aFileName, 
			D3DXMESH_SYSTEMMEM, 
			pDevice,
			NULL,
			&g_player.aModel[nCntModel].pBuffMat,
			NULL, 
			&g_player.aModel[nCntModel].nNumMat,
			&g_player.aModel[nCntModel].pMesh)))
		{
			return E_FAIL;
		}
	}
		
	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// ���_���̎擾
		nNumVtx = g_player.aModel[nCntModel].pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[nCntModel].pMesh->GetFVF());

		// ���_�o�b�t�@�̃��b�N
		g_player.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// ���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
			if (g_player.minVecPlayer.x > vtx.x)
			{// X���W�̍ŏ��l
				g_player.minVecPlayer.x = vtx.x;
			}
			else if (g_player.maxVecPlayer.x < vtx.x)
			{// X���W�̍ő�l
				g_player.maxVecPlayer.x = vtx.x;
			}

			if (g_player.minVecPlayer.y > vtx.y)
			{// Y���W�̍ŏ��l
				g_player.minVecPlayer.y = vtx.y;
			}
			else if (g_player.maxVecPlayer.y < vtx.y)
			{// Y���W�̍ő�l
				g_player.maxVecPlayer.y = vtx.y;
			}

			if (g_player.minVecPlayer.z > vtx.z)
			{// Z���W�̍ŏ��l
				g_player.minVecPlayer.z = vtx.z;
			}
			else if (g_player.maxVecPlayer.z < vtx.z)
			{// Z���W�̍ő�l
				g_player.maxVecPlayer.z = vtx.z;
			}
			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
		// ���_�o�b�t�@�̃A�����b�N
		g_player.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
	// �e�̔z�u
	g_player.nIdx = SetShadow(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z), 20.0f, 20.0f);
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		// �}�e���A�����̊J��
		if (g_player.aModel[nCnt].pBuffMat != NULL)
		{
			g_player.aModel[nCnt].pBuffMat->Release();
			g_player.aModel[nCnt].pBuffMat = NULL;
		}

		// ���b�V�����̊J��
		if (g_player.aModel[nCnt].pMesh != NULL)
		{
			g_player.aModel[nCnt].pMesh->Release();
			g_player.aModel[nCnt].pMesh = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	// �v���C���[���[�V����
	PlayerMotion();

	// �v���C���[�̏�Ԃ𔻕�
	StateManagerPlayer();

	// �O�̍��W��ۑ�
	g_player.posOld = g_player.pos;

	// �ړ��ʂ̌���(����)
	g_player.move.x += (0.0f - g_player.move.x) * 0.3f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.3f;

	// �d�͐ݒ�
	g_player.move.y -= GRAVITY;

	// �ʒu�̍X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	// �R�C���Ƃ̓����蔻��
	CollisionCoin(&g_player.pos, &g_player.posOld, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	bool bLand = CollisionMeshField(&g_player.pos, &g_player.posOld, g_player.nIdx);

	// ���f���Ƃ̓����蔻��
	bool bLandModel = CollisionModelSet(&g_player.pos, &g_player.posOld, &g_player.move,&g_player.minVecPlayer, &g_player.maxVecPlayer,g_player.nIdx);

	if (bLand == true || bLandModel == true)
	{
		if (g_player.state == PLAYERSTATE_FLOATING)
		{// �W�����v���̂Ƃ��W�����v���Ă��Ȃ���Ԃɂ���
			g_player.state = PLAYERSTATE_NORMAL;
		}

		// �ړ��ʂ�0.0f�ɂ���
		g_player.move.y = 0.0f;
	}
	else if (bLand == false && bLandModel == false)
	{
		// ��������Ă��Ȃ��Ƃ������Ă����Ԃɂ���
		g_player.state = PLAYERSTATE_FLOATING;
	}

	// ��������t�F�[�h
	if (g_player.pos.y < -100.0f)
	{
		g_player.bFall = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawPlayer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// �ϐ��錾
		D3DXMATRIX mtxRotModel, mtxTransModel;		// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxParent;						// �e�̃}�g���b�N�X

		// �e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		// �e�p�[�c�̌���
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// �e�p�[�c�̈ʒu
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		// �e�p�[�c�̐e�̃}�g���b�N�X��ݒ�
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;
		}
		// �Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld,
			&g_player.aModel[nCntModel].mtxWorld,
			&mtxParent);

		// �e�p�[�c�̃}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// ���f��(�p�[�c)�̕`��
			g_player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}
	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[��Ԃ̔���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void StateManagerPlayer(void)
{
	// �v���C���[�̏�Ԃ𒲂ׂ�
	switch (g_player.state)
	{// ��Ԃ��ʏ�̂Ƃ�
	case PLAYERSTATE_NORMAL:
		PlayerStateNormal();
		break;
		// ��Ԃ����V�̂Ƃ�
	case PLAYERSTATE_FLOATING:
		PlayerStateFloating();
		break;
		// ��Ԃ��_���[�W�̂Ƃ�
	case PLAYERSTATE_DAMAGE:
		PlayerStateDamage();
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[��Ԃ��ʏ�̎�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void PlayerStateNormal(void)
{
	// �Q�[���p�b�h���̎擾
	DIJOYSTATE2 Controller = GetControllerPless(PLAYER_1);

	// �v���C���[�̈ړ�����
	MovePlayer();

	// �G�Ƃ̓����蔻��
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// �G�ɓ���������_���[�W��Ԃɂ���
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;

		// �����o��
		PlaySound(SOUND_LABEL_SE_DAMAGE);
	}

	if (GetKeyboardPress(KEYINFO_UP) == false && GetKeyboardPress(KEYINFO_DOWN) == false &&
		GetKeyboardPress(KEYINFO_LEFT) == false && GetKeyboardPress(KEYINFO_RIGHT) == false &&
		g_player.bJump == false && g_player.MotionType != MOTIONTYPE_LAND &&
		Controller.lX == 0 && Controller.lY == 0)
	{
		// �������Ȃ��Ƃ��j���[�g�������[�V�����ɐݒ�
		SetMotion(MOTIONTYPE_NEUTRAL, true, 10);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[��Ԃ����V�̎�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void PlayerStateFloating(void)
{
	// �v���C���[�̈ړ�����
	MovePlayer();

	// ���[�V�������W�����v���[�V�����ɂ���
	SetMotion(MOTIONTYPE_JUMP, true, 10);

	// �G�Ƃ̓����蔻��
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// �G�ɓ���������_���[�W��Ԃɂ���
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;

		// �����o��
		PlaySound(SOUND_LABEL_SE_DAMAGE);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[���_���[�W��Ԃ̂Ƃ�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void PlayerStateDamage(void)
{
	// ��ԃJ�E���^��i�߂�
	g_player.nStateCounter++;

	// �̂����胂�[�V�����ɕς���
	SetMotion(MOTIONTYPE_DAMAGE, true, 5);

	// ��ԃJ�E���^�[��20�ɂȂ����Ƃ�����
	if (g_player.nStateCounter >= 20)
	{
		// ���[�V������߂�
		SetMotion(MOTIONTYPE_NEUTRAL, true, 10);

		// ��Ԃ��m�[�}���ɖ߂�
		g_player.state = PLAYERSTATE_NORMAL;

		// ��ԃJ�E���^�[��0�ɏ�����
		g_player.nStateCounter = 0;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�̈ړ�����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void MovePlayer(void)
{
	// �J�E���g�_�E�����I��������ǂ����擾
	bool bFinishCountDown = GetFinishCountDown();

	// �\���̂̃|�C���^�ϐ�
	Camera pCamera = GetPlayerCamera();

	// ���݂̉�]�p�𐧌�
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}
	// �����̍X�V
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.15f;

	// ���݂̉�]�p�𐧌�
	if (g_player.rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	if (bFinishCountDown == true)
	{
		// �L�[�{�[�h
		MovePlayerKeyboard();

		// �Q�[���p�b�h
		MovePlayerGamepad();
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�̈ړ�����(�L�[�{�[�h)
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void MovePlayerKeyboard(void)
{
	// ���̎擾
	Camera camera = GetPlayerCamera();				// �J�������
#if 1
	if (GetKeyboardPress(KEYINFO_UP) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// ���f���΂߉E��ړ�
			g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_player.move.x += MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_player.rotDest.y = camera.rot.y - PI_QUARTER * 3.0f;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// ���f���΂߉E��ړ�
			g_player.move.z += MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_player.rotDest.y = camera.rot.y + PI_QUARTER * 3.0f;
		}
		else
		{// ���f����ړ�
			g_player.move.z += MOVE_MODEL * cosf(camera.rot.y);
			g_player.move.x += MOVE_MODEL * sinf(camera.rot.y);
			g_player.rotDest.y = camera.rot.y - D3DX_PI;
		}
		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// ���f���΂߉E��ړ�
			g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_player.move.x += MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_player.rotDest.y = camera.rot.y - PI_QUARTER;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// ���f���΂߉E��ړ�
			g_player.move.z += MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_player.rotDest.y = camera.rot.y + PI_QUARTER;
		}
		else
		{// ���f�����ړ�
			g_player.move.z -= MOVE_MODEL * cosf(camera.rot.y);
			g_player.move.x -= MOVE_MODEL * sinf(camera.rot.y);
			g_player.rotDest.y = camera.rot.y;
		}
		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// ���f���E�ړ�
		g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x += MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y - D3DX_PI / 2.0f;

		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// ���f�����ړ�
		g_player.move.z += MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y + D3DX_PI / 2.0f;

		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	if(GetKeyboardTrigger(KEYINFO_SHOOT) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += JUMP_PLAYER;
		g_player.state = PLAYERSTATE_FLOATING;

		// �����o��
		PlaySound(SOUND_LABEL_SE_JUMP);
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�̈ړ�����(�Q�[���p�b�h)
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void MovePlayerGamepad(void)
{
	// ���̎擾
	Camera camera = GetPlayerCamera();				// �J�������

	// �Q�[���p�b�h���̎擾
	DIJOYSTATE2 Controller = GetControllerPless(PLAYER_1);

	// JOYPAD�̃X�e�B�b�N����
	// �ړ�����
	if (Controller.lX != 0 || Controller.lY != 0)
	{// �X�e�B�b�N�̓��͊p�x
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY);

		g_player.rotDest.y = camera.rot.y - fAngle;								// �����̐ݒ�
		g_player.move.x -= MOVE_MODEL * sinf(g_player.rotDest.y);			// X���ړ��ʂ̌���
		g_player.move.z -= MOVE_MODEL * cosf(g_player.rotDest.y);			// Z���ړ��ʂ̌���

		// ���݂̉�]�p�𐧌�
		if (g_player.rotDest.y > D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			g_player.rotDest.y -= D3DX_PI * 2.0f;
		}
		else if (g_player.rotDest.y < -D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			g_player.rotDest.y += D3DX_PI * 2.0f;
		}
		// ���[�V�������ړ����[�V�����ɂ���
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	// �W�����v����
	if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += JUMP_PLAYER;
		g_player.state = PLAYERSTATE_FLOATING;

		// �����o��
		PlaySound(SOUND_LABEL_SE_JUMP);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[���̎擾
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FPlayer	- Player�^�\���̂̃|�C���^��Ԃ�
//--------------------------------------------------------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_player;
}

//--------------------------------------------------------------------------------------------------------------
// �v���C��-���[�V�����̐ݒ�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void PlayerMotion(void)
{
	// �ϐ��錾
	Player *pPlayer = GetPlayer();
	KEY *pKey;				// �L�[���
	KEY *pKeyNext;			// ���̃L�[���
	KEY *pKeyBlend;			// �u�����h�L�[���
	KEY *pKeyNextBlend;		// ���̃u�����h�L�[���
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false && pPlayer->bBlendMotion == false)
		{
			break;
		}

		// �L�[�̊m��
		pKey = &pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].aKey[nCntModel];
		pKeyNext = &pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[(pPlayer->nKey + 1) % pPlayer->nNumKey].aKey[nCntModel];

		// �����̊m��
		float fDiffMoitonPosX = pKeyNext->fPosX - pKey->fPosX;
		float fDiffMoitonPosY = pKeyNext->fPosY - pKey->fPosY;
		float fDiffMoitonPosZ = pKeyNext->fPosZ - pKey->fPosZ;
		float fDiffMoitonRotX = pKeyNext->fRotX - pKey->fRotX;
		float fDiffMoitonRotY = pKeyNext->fRotY - pKey->fRotY;
		float fDiffMoitonRotZ = pKeyNext->fRotZ - pKey->fRotZ;

		// ���Βl�̊m��
		float fRateMotion = (float)pPlayer->nCounterMotion / (float)pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].nFrame;

		if (g_player.bBlendMotion == true)
		{
			// �L�[�̊m��
			pKeyBlend = &pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[pPlayer->nKeyBlend].aKey[nCntModel];
			pKeyNextBlend = &pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[(pPlayer->nKeyBlend + 1) % pPlayer->nNumKeyBlend].aKey[nCntModel];

			// �����̊m��
			float fDiffMoitonBlendPosX = pKeyNextBlend->fPosX - pKeyBlend->fPosX;
			float fDiffMoitonBlendPosY = pKeyNextBlend->fPosY - pKeyBlend->fPosY;
			float fDiffMoitonBlendPosZ = pKeyNextBlend->fPosZ - pKeyBlend->fPosZ;
			float fDiffMoitonBlendRotX = pKeyNextBlend->fRotX - pKeyBlend->fRotX;
			float fDiffMoitonBlendRotY = pKeyNextBlend->fRotY - pKeyBlend->fRotY;
			float fDiffMoitonBlendRotZ = pKeyNextBlend->fRotZ - pKeyBlend->fRotZ;

			// ���Βl�̊m��
			float fRateMotionBlend = (float)pPlayer->nCounterMotionBlend / (float)pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[pPlayer->nKeyBlend].nFrame;

			float fRateBlend = (float)pPlayer->nCounterBlend / (float)pPlayer->nFrameBlend;

			// ���݂̃��[�V�����̈ʒu�ƌ����̊m��
			float fPosXCurrent = pKey->fPosX + (fDiffMoitonPosX * fRateMotion);
			float fPosYCurrent = pKey->fPosY + (fDiffMoitonPosY * fRateMotion);
			float fPosZCurrent = pKey->fPosZ + (fDiffMoitonPosZ * fRateMotion);
			float fRotXCurrent = pKey->fRotX + (fDiffMoitonRotX * fRateMotion);
			float fRotYCurrent = pKey->fRotY + (fDiffMoitonRotY * fRateMotion);
			float fRotZCurrent = pKey->fRotZ + (fDiffMoitonRotZ * fRateMotion);

			// �p�x�̐���
			D3DXVECTOR3(fRotXCurrent, fRotYCurrent, fRotZCurrent) = AngleCorrection(fRotXCurrent, fRotYCurrent, fRotZCurrent);

			// �u�����h���[�V�����̈ʒu�ƌ����̊m��
			float fPosXBlend = pKeyBlend->fPosX + (fDiffMoitonBlendPosX * fRateMotionBlend);
			float fPosYBlend = pKeyBlend->fPosY + (fDiffMoitonBlendPosY * fRateMotionBlend);
			float fPosZBlend = pKeyBlend->fPosZ + (fDiffMoitonBlendPosZ * fRateMotionBlend);
			float fRotXBlend = pKeyBlend->fRotX + (fDiffMoitonBlendRotX * fRateMotionBlend);
			float fRotYBlend = pKeyBlend->fRotY + (fDiffMoitonBlendRotY * fRateMotionBlend);
			float fRotZBlend = pKeyBlend->fRotZ + (fDiffMoitonBlendRotZ * fRateMotionBlend);

			// �p�x�̐���
			D3DXVECTOR3(fRotXBlend, fRotYBlend, fRotZBlend) = AngleCorrection(fRotXBlend, fRotYBlend, fRotZBlend);

			// ���݂̃��[�V�����ƃu�����h���[�V�����̍���
			float fDiffBlendPosX = fPosXBlend - fPosXCurrent;
			float fDiffBlendPosY = fPosYBlend - fPosYCurrent;
			float fDiffBlendPosZ = fPosZBlend - fPosZCurrent;
			float fDiffBlendRotX = fRotXBlend - fRotXCurrent;
			float fDiffBlendRotY = fRotYBlend - fRotYCurrent;
			float fDiffBlendRotZ = fRotZBlend - fRotZCurrent;

			// �p�x�̐���
			D3DXVECTOR3(fDiffBlendRotX, fDiffBlendRotY, fDiffBlendRotZ) = AngleCorrection(fDiffBlendRotX, fDiffBlendRotY, fDiffBlendRotZ);

			// �u�����h���[�V�����̈ʒu�ƌ����̊m��
			fPosX = fPosXCurrent + (fDiffBlendPosX * fRateBlend);
			fPosY = fPosYCurrent + (fDiffBlendPosY * fRateBlend);
			fPosZ = fPosZCurrent + (fDiffBlendPosZ * fRateBlend);
			fRotX = fRotXCurrent + (fDiffBlendRotX * fRateBlend);
			fRotY = fRotYCurrent + (fDiffBlendRotY * fRateBlend);
			fRotZ = fRotZCurrent + (fDiffBlendRotZ * fRateBlend);
		}
		else
		{
			// ���[�V�����̈ʒu�ƌ����̊m��
			fPosX = pKey->fPosX + (fDiffMoitonPosX * fRateMotion);
			fPosY = pKey->fPosY + (fDiffMoitonPosY * fRateMotion);
			fPosZ = pKey->fPosZ + (fDiffMoitonPosZ * fRateMotion);
			fRotX = pKey->fRotX + (fDiffMoitonRotX * fRateMotion);
			fRotY = pKey->fRotY + (fDiffMoitonRotY * fRateMotion);
			fRotZ = pKey->fRotZ + (fDiffMoitonRotZ * fRateMotion);
		}
		// �p�x�̐���
		D3DXVECTOR3(fRotX, fRotY, fRotZ) = AngleCorrection(fRotX, fRotY, fRotZ);

		// �ʒu�ƌ����̐ݒ�
		pPlayer->aModel[nCntModel].pos.x = fPosX;
		pPlayer->aModel[nCntModel].pos.y = fPosY;
		pPlayer->aModel[nCntModel].pos.z = fPosZ;
		pPlayer->aModel[nCntModel].rot.x = fRotX;
		pPlayer->aModel[nCntModel].rot.y = fRotY;
		pPlayer->aModel[nCntModel].rot.z = fRotZ;
	}

	if (g_player.bBlendMotion == true)
	{
		if (pPlayer->nCounterMotionBlend > pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[pPlayer->nKeyBlend].nFrame)
		{
			// �u�����h���[�V�����J�E���^�̏�����
			pPlayer->nCounterMotionBlend = 0;

			// �L�[�̍X�V
			pPlayer->nKeyBlend++;

			// �L�[�̊m��
			pPlayer->nKeyBlend = pPlayer->nKeyBlend % pPlayer->nNumKeyBlend;
		}
		else
		{
			if (pPlayer->nCounterBlend >= pPlayer->nFrameBlend)
			{
				// ���[�V�����̊m��
				pPlayer->MotionType = pPlayer->MotionTypeBlend;

				// ���[�V�����J�E���^��������
				pPlayer->nCounterMotion = pPlayer->nCounterMotionBlend;

				// ���݂̃L�[��������
				pPlayer->nKey = pPlayer->nKeyBlend;

				// ���[�v���邩�ǂ���
				pPlayer->bLoopMotion = pPlayer->bLoopMotionBlend;

				// �L�[���̊m��
				pPlayer->nNumKey = pPlayer->nNumKeyBlend;

				// ���[�V�����u�����h���I����
				pPlayer->bBlendMotion = false;
			}
			else
			{
				// �u�����h���[�V�����J�E���^�̍X�V
				pPlayer->nCounterMotionBlend++;

				// �u�����h�J�E���^�̍X�V
				pPlayer->nCounterBlend++;
			}
		}
	}
	else
	{
		// ���[�V�����J�E���^���e�L�[�̍ŏI�t���[���ɓ��B�����Ƃ�
  		if (pPlayer->nCounterMotion >= pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].nFrame)
		{
			if (g_player.bLoopMotion == false && pPlayer->nKey == pPlayer->nNumKey - 1)
			{
				// ���[�V�����J�E���^�̍X�V
  				pPlayer->nCounterMotion = pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].nFrame;
			}
			else
			{
				// ���[�V�����J�E���^�̏�����
				pPlayer->nCounterMotion = 0;

				// �L�[�̍X�V
				pPlayer->nKey++;

				// �L�[�̊m��
				pPlayer->nKey = pPlayer->nKey % pPlayer->nNumKey;
			}
		}
		else
		{
			// ���[�V�����J�E���^�̍X�V
			pPlayer->nCounterMotion++;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ���[�V�����̐ݒ�
// ����		�Fmotiontype	- ���[�V�����^�C�v
//			�FbBlend		- �u�����h���邩�ǂ���
//			�FnFrameBlend	- �u�����h�t���[����
// �Ԃ�l	�Fvoid			- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetMotion(MOTIONTYPE motiontype, bool bBlend, int nFrameBlend)
{
	// �ϐ��錾
	Player *pPlayer = GetPlayer();

	// ���[�V�����^�C�v�ɂ���ď�����ς���
	switch (motiontype)
	{
	case MOTIONTYPE_NEUTRAL:
		if (g_player.state == PLAYERSTATE_FLOATING)
		{
			motiontype = MOTIONTYPE_JUMP;
		}
		break;

	case MOTIONTYPE_MOVE:
		if (g_player.state == PLAYERSTATE_FLOATING)
		{
			motiontype = MOTIONTYPE_JUMP;
		}
		break;

	case MOTIONTYPE_JUMP:
		break;

	case MOTIONTYPE_LAND:
		break;

	case MOTIONTYPE_DAMAGE:
		break;

	default:
		break;
	}
	//if (g_player.bFinishMotion == true)
	{
		if (bBlend == true && g_player.MotionTypeBlend != motiontype)
		{
			// ���[�V�����̊m��
			g_player.MotionTypeBlend = motiontype;

			// ���[�V�����J�E���^��������
			g_player.nCounterMotionBlend = 0;

			// ���݂̃L�[��������
			g_player.nKeyBlend = 0;

			// ���[�v���邩�ǂ���
			g_player.bLoopMotionBlend = g_player.aMotionInfo[g_player.MotionTypeBlend].bLoop;

			// �L�[���̊m��
			g_player.nNumKeyBlend = g_player.aMotionInfo[g_player.MotionTypeBlend].nNumKey;

			// �u�����h�t���[���̐ݒ�
			g_player.nFrameBlend = nFrameBlend;

			// �u�����h���邩�ǂ���
			g_player.bBlendMotion = bBlend;

			// �u�����h�J�E���^�̏�����
			g_player.nCounterBlend = 0;
		}
		else if (bBlend == false && g_player.MotionType != motiontype)
		{
			// ���[�V�����̊m��
			g_player.MotionType = motiontype;

			// ���[�V�����J�E���^��������
			g_player.nCounterMotion = 0;

			// ���݂̃L�[��������
			g_player.nKey = 0;

			// ���[�v���邩�ǂ���
			g_player.bLoopMotion = g_player.aMotionInfo[g_player.MotionType].bLoop;

			// �L�[���̊m��
			g_player.nNumKey = g_player.aMotionInfo[g_player.MotionType].nNumKey;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �p�x�̐���
// ����		�FrotX			- X���̊p�x���
//			�FrotY			- Y���̊p�x���
//			�FrotZ			- Z���̊p�x���
// �Ԃ�l	�FD3DXVECTOR3	- ��]�p�𐧌�������̊p�x����n��
//--------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 AngleCorrection(float rotX, float rotY, float rotZ)
{
	// ���݂̉�]�p�𐧌�
	if (rotX > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		rotX -= D3DX_PI * 2.0f;
	}
	else if (rotX < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		rotX += D3DX_PI * 2.0f;
	}
	// ���݂̉�]�p�𐧌�
	if (rotY  > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		rotY -= D3DX_PI * 2.0f;
	}
	else if (rotY < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		rotY += D3DX_PI * 2.0f;
	}
	// ���݂̉�]�p�𐧌�
	if (rotZ > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		rotZ -= D3DX_PI * 2.0f;
	}
	else if (rotZ < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		rotZ += D3DX_PI * 2.0f;
	}

	return D3DXVECTOR3(rotX, rotY, rotZ);
}