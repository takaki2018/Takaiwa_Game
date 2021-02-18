//-----------------------------------------------------------------
//
// �v���C���[ (player.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "player.h"
#include "motion.h"
#include "wall.h"
#include "coin.h"
#include "enemy.h"
#include "meshfield.h"
#include "wall.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "motion.h"
#include "model_set.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define POLYGON_SIZE		(5.0f)		// �|���S���̃T�C�Y
#define MOVE_MODEL			(2.0f)
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
Player g_player;						// �v���C���[���
int g_nCntMotion;

//-----------------------------------------------------------------
// �v���C���[�̏���������
//-----------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �v���C���[���̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.bJump = false;
	g_player.bFall = false;
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.MotionType = MOTIONTYPE_NEUTRAL;
	g_player.MotionTypeOld = g_player.MotionType;
	g_player.nStateCounter = 0;
	g_player.nCounterMotion = 0;
	g_player.nKey = 0;

	// �v���C���[���̓ǂݍ���
	LoadPlayerdata();

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
	g_player.nIdx = SetShadow(D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z), 20.0f, 20.0f);

	return S_OK;
}

//-----------------------------------------------------------------
// �v���C���[�̏I������
//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
// �v���C���[�̍X�V����
//-----------------------------------------------------------------
void UpdatePlayer(void)
{
	// �v���C���[���[�V����
	PlayerMotion();

	// �v���C���[�̏�Ԃ𔻕�
	StateManagerPlayer();

	// �ړ��ʂ̌���(����)
	g_player.move.x += (0.0f - g_player.move.x) * 0.2f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.2f;

	// �d�͐ݒ�
	g_player.move.y -= 0.6f;

	// �O�̍��W��ۑ�
	g_player.posOld = g_player.pos;

	// �ʒu�̍X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	// �R�C���Ƃ̓����蔻��
	CollisionCoin(&g_player.pos, &g_player.posOld, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// ���f���Ƃ̓����蔻��
	bool bCollisionModel = CollisionModelSet(&g_player.pos, &g_player.posOld, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	if (bCollisionModel == true)
	{
		// �ړ��ʂ�0.0f�ɂ���
		g_player.move.y = 0.0f;

		if (g_player.state == PLAYERSTATE_FLOATING)
		{// �W�����v���̂Ƃ��W�����v���Ă��Ȃ���Ԃɂ���
			g_player.state = PLAYERSTATE_NORMAL;
		}
	}
	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	bool bLand = CollisionMeshField(&g_player.pos, &g_player.posOld);

	if (bLand == true)
	{
		// �ړ��ʂ�0.0f�ɂ���
		g_player.move.y = 0.0f;

		if (g_player.state == PLAYERSTATE_FLOATING)
		{// �W�����v���̂Ƃ��W�����v���Ă��Ȃ���Ԃɂ���
			g_player.state = PLAYERSTATE_NORMAL;
		}
	}

	// �e�̈ړ�
	SetPositionShadow(g_player.nIdx, D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z));

	// ��������t�F�[�h
	if (g_player.pos.y < -100.0f)
	{
		g_player.bFall = true;
	}
}

//-----------------------------------------------------------------
// �v���C���[�̕`�揈��
//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
// �v���C���[��Ԃ̔���
//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
// �v���C���[��Ԃ��ʏ�̎�
//-----------------------------------------------------------------
void PlayerStateNormal(void)
{
	// �v���C���[�̈ړ�����
	MovePlayer();

	// �G�Ƃ̓����蔻��
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// �G�ɓ���������_���[�W��Ԃɂ���
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;
	}
}

//-----------------------------------------------------------------
// �v���C���[��Ԃ����V�̎�
//-----------------------------------------------------------------
void PlayerStateFloating(void)
{
	// �v���C���[�̈ړ�����
	MovePlayer();

	// �G�Ƃ̓����蔻��
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// �G�ɓ���������_���[�W��Ԃɂ���
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;
	}

	// ���[�V�������W�����v���[�V�����ɂ���
	g_player.MotionType = MOTIONTYPE_JUMP;
}

//-----------------------------------------------------------------
// �v���C���[���_���[�W��Ԃ̂Ƃ�
//-----------------------------------------------------------------
void PlayerStateDamage(void)
{
	// ��ԃJ�E���^��i�߂�
	g_player.nStateCounter++;

	// �̂����胂�[�V�����ɕς���
	g_player.MotionType = MOTIONTYPE_DAMAGE;

	// ��ԃJ�E���^�[��20�ɂȂ����Ƃ�����
	if (g_player.nStateCounter >= 20)
	{
		// ���[�V������߂�
		g_player.MotionType = MOTIONTYPE_NEUTRAL;

		// ��Ԃ��m�[�}���ɖ߂�
		g_player.state = PLAYERSTATE_NORMAL;

		// ��ԃJ�E���^�[��0�ɏ�����
		g_player.nStateCounter = 0;
	}
}

//-----------------------------------------------------------------
// �v���C���[�̈ړ�����
//-----------------------------------------------------------------
void MovePlayer(void)
{
	// �\���̂̃|�C���^�ϐ�
	Camera pCamera = GetCamera();

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

	// �L�[�{�[�h
	MovePlayerKeyboard();

	// �Q�[���p�b�h
	MovePlayerGamepad();
}

//-----------------------------------------------------------------
// �v���C���[�̈ړ�����(�L�[�{�[�h)
//-----------------------------------------------------------------
void MovePlayerKeyboard(void)
{
	// ���̎擾
	Camera camera = GetCamera();				// �J�������
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
		g_player.MotionType = MOTIONTYPE_MOVE;
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
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// ���f���E�ړ�
		g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x += MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y - D3DX_PI / 2.0f;

		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// ���f�����ړ�
		g_player.move.z += MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y + D3DX_PI / 2.0f;

		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	if (GetKeyboardPress(KEYINFO_MODEL_RIGHT_TURN) == true)
	{// �����_�̍���]
		g_player.rotDest.y += 0.1f;						// ��]��
	}
	if (GetKeyboardPress(KEYINFO_MODEL_LEFT_TURN) == true)
	{// �����_�̉E��]
		g_player.rotDest.y -= 0.1f;						// ��]��
	}
	if(GetKeyboardTrigger(KEYINFO_SHOOT) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += 14.0f;
		g_player.state = PLAYERSTATE_FLOATING;
	}
	if (GetKeyboardPress(KEYINFO_UP) == false && GetKeyboardPress(KEYINFO_DOWN) == false &&
		GetKeyboardPress(KEYINFO_LEFT) == false && GetKeyboardPress(KEYINFO_RIGHT) == false &&
		g_player.bJump == false && g_player.MotionType != MOTIONTYPE_LAND)
	{
		// �������Ȃ��Ƃ��j���[�g�������[�V�����ɐݒ�
		g_player.MotionType = MOTIONTYPE_NEUTRAL;
	}
#endif
}

//-----------------------------------------------------------------
// �v���C���[�̈ړ�����(�Q�[���p�b�h)
//-----------------------------------------------------------------
void MovePlayerGamepad(void)
{
	// ���̎擾
	Camera camera = GetCamera();				// �J�������

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
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	// �����_��]
	if (GetJoypadPress(PLAYER_1, JPINFO_RIGHT_TURN) == true)
	{// �����_�̍���]
		g_player.rotDest.y += 0.1f;						// ��]��
	}
	if (GetJoypadPress(PLAYER_1, JPINFO_LEFT_TURN) == true)
	{// �����_�̉E��]
		g_player.rotDest.y -= 0.1f;						// ��]��
	}
	// �W�����v����
	if (GetJoypadTrigger(PLAYER_1, JPINFO_JUMP) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += 14.0f;
		g_player.state = PLAYERSTATE_FLOATING;
	}
}

//-----------------------------------------------------------------
// �v���C���[���̎擾
//-----------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_player;
}

//-----------------------------------------------------------------
// �v���C��-���[�V�����̐ݒ�
//-----------------------------------------------------------------
void PlayerMotion(void)
{
	// �O�̃��[�V�����ƌ��݂̃��[�V�������r���ĈقȂ�Ƃ�����
	if (g_player.MotionTypeOld != g_player.MotionType)
	{
		// ���[�V�����J�E���^�̏�����
		g_player.nCounterMotion = 0;

		// ���݂̃L�[��������
		g_player.nKey = 0;

		// �ʒu�̕␳
		g_player.aModel[0].pos.x = g_player.posParent.x;
		g_player.aModel[0].pos.y = g_player.posParent.y;
		g_player.aModel[0].pos.z = g_player.posParent.z;
	}
	// ���[�v���邩�ǂ���
	g_player.bLoopMotion = g_player.aMotionInfo[g_player.MotionType].bLoop;

	// �L�[���̊m��
	g_player.nNumKey = g_player.aMotionInfo[g_player.MotionType].nNumKey;

	// ���[�V�����̋L��
	g_player.MotionTypeOld = g_player.MotionType;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// �v���C���[�̃��[�V�����̐ݒ������
		if (g_player.nKey == (g_player.nNumKey - 1) && g_player.bLoopMotion == true)
		{
			//g_player.aModel[nCntModel].pos.x += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.y += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.z += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			g_player.aModel[nCntModel].rot.x = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.y = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.z = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
		}
		else
		{
			//g_player.aModel[nCntModel].pos.x += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.y += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.z += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			g_player.aModel[nCntModel].rot.x = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.y = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.z = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
		
			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false)
			{
				break;
			}
		}
		if (g_player.nCounterMotion == g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame)
		{
			g_player.nCounterMotion = 0;

			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
			{
				g_player.nKey = 0;
			}
			else
			{
				g_player.nKey++;
			}
		}
	}
	if (g_player.nCounterMotion != g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nNumKey - 1].nFrame)
	{
		// ���[�V�����J�E���^�̍X�V
		g_player.nCounterMotion++;
	}
}