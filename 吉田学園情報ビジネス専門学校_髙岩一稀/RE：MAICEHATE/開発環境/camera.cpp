//==============================================================================================================
//
// �J���� (camera.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "camera.h"
#include "player.h"
#include "input.h"
#include "gamepad.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MOVE_CAMERA			(3.0f)
#define LENGTH				(350.0f)
#define ROT					(0.03f)

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
Camera g_camera[CAMERATYPE_MAX];			// �J�������

//--------------------------------------------------------------------------------------------------------------
// �J�����̏���������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void InitCamera(void)
{
	// �ϐ��̏�����
	// �v���C���[�����낵�J����
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest = g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest = g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveV = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.X = 950;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.Y = 400;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.Width = 300;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.Height = 300;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.MinZ = 0.0f;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.MaxZ = 1.0f;

	// �v���C���[�Ǐ]�J����
	g_camera[CAMERATYPE_PLAYER].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].posVDest = g_camera[CAMERATYPE_PLAYER].posV;
	g_camera[CAMERATYPE_PLAYER].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].posRDest = g_camera[CAMERATYPE_PLAYER].posR;
	g_camera[CAMERATYPE_PLAYER].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].moveV = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].viewport.X = 0;
	g_camera[CAMERATYPE_PLAYER].viewport.Y = 0;
	g_camera[CAMERATYPE_PLAYER].viewport.Width = SCREEN_WIDTH;
	g_camera[CAMERATYPE_PLAYER].viewport.Height = SCREEN_HEIGHT;
	g_camera[CAMERATYPE_PLAYER].viewport.MinZ = 0.0f;
	g_camera[CAMERATYPE_PLAYER].viewport.MaxZ = 1.0f;
}

//--------------------------------------------------------------------------------------------------------------
// �J�����̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitCamera(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// �J�����̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateCamera(void)
{
	// �Ǐ]
	KeyboradFollow();

	// �����낵�J����
	LookDownPlayer();
}

//--------------------------------------------------------------------------------------------------------------
// �J�����̐ݒu
// ����		�FnIdx	- ���Ԗڂ̃J������
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetCamera(int nIdx)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&g_camera[nIdx].viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nIdx].mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera[nIdx].mtxProjection,
		D3DXToRadian(45.0f),
		(float)g_camera[nIdx].viewport.Width / (float)g_camera[nIdx].viewport.Height,
		10.0f,
		3000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nIdx].mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nIdx].mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera[nIdx].mtxView,
		&g_camera[nIdx].posV,
		&g_camera[nIdx].posR,
		&g_camera[nIdx].vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nIdx].mtxView);
}

//--------------------------------------------------------------------------------------------------------------
// �J�����̃L�[�{�[�h����(�Ǐ])
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void KeyboradFollow(void)
{
	// �v���C���[���̎擾
	Player *pPlayer = GetPlayer();

	if (GetKeyboardPress(KEYINFO_ROT_LEFT) == true)
	{// ���_�̍���]
		g_camera[CAMERATYPE_PLAYER].rot.y -= ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].rot.y < -D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER].rot.y += D3DX_PI * 2.0f;
		}
	}
	else if (GetKeyboardPress(KEYINFO_ROT_RIGHT) == true)
	{// ���_�̉E��]
		g_camera[CAMERATYPE_PLAYER].rot.y += ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].rot.y > D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER].rot.y -= D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(KEYINFO_POS_DOWN) == true)
	{// ���_�̉��ړ�
		g_camera[CAMERATYPE_PLAYER].moveV.y -= 5.0f;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].moveV.y <= 50.0f)
		{// ���_�̈ʒu��50.0f��菬�����Ȃ����Ƃ�����
			g_camera[CAMERATYPE_PLAYER].moveV.y = 50.0f;
		}
	}
	else if (GetKeyboardPress(KEYINFO_POS_UP) == true)
	{// ���_�̏�ړ�
		g_camera[CAMERATYPE_PLAYER].moveV.y += 5.0f;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].moveV.y >= 300.0f)
		{// ���_�̈ʒu��500.0f���傫���Ȃ����Ƃ�����
			g_camera[CAMERATYPE_PLAYER].moveV.y = 300.0f;
		}
	}

	// �Q�[���p�b�h���̎擾
	bool *pRstckPress = GetRStickPress(PLAYER_1); 

	if (pRstckPress[RSTICKPRESS_LEFT] == true)
	{// ���_�̉E��]
		g_camera[CAMERATYPE_PLAYER].rot.y -= ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].rot.y < -D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER].rot.y += D3DX_PI * 2.0f;
		}
	}
	else if (pRstckPress[RSTICKPRESS_RIGHT] == true)
	{// ���_�̍���]
		g_camera[CAMERATYPE_PLAYER].rot.y += ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].rot.y > D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER].rot.y -= D3DX_PI * 2.0f;
		}
	}

	if (pRstckPress[RSTICKPRESS_UP] == true)
	{// ���_�̉��ړ�
		g_camera[CAMERATYPE_PLAYER].moveV.y -= 5.0f;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].moveV.y <= 50.0f)
		{// ���_�̈ʒu��50.0f��菬�����Ȃ����Ƃ�����
			g_camera[CAMERATYPE_PLAYER].moveV.y = 50.0f;
		}
	}
	else if (pRstckPress[RSTICKPRESS_DOWN] == true)
	{// ���_�̏�ړ�
		g_camera[CAMERATYPE_PLAYER].moveV.y += 5.0f;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER].moveV.y >= 300.0f)
		{// ���_�̈ʒu��500.0f���傫���Ȃ����Ƃ�����
			g_camera[CAMERATYPE_PLAYER].moveV.y = 300.0f;
		}
	}

	// �ړI�̒����_
	g_camera[CAMERATYPE_PLAYER].posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 10.0f + g_camera[CAMERATYPE_PLAYER].moveR.x;		// �ړI�̒����_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 10.0f + g_camera[CAMERATYPE_PLAYER].moveR.y;		// �ړI�̒����_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posRDest.y = pPlayer->pos.y + g_camera[CAMERATYPE_PLAYER].moveR.y;

	// �ړI�̎��_
	g_camera[CAMERATYPE_PLAYER].posVDest.x = pPlayer->pos.x - sinf(g_camera[CAMERATYPE_PLAYER].rot.y) * LENGTH + g_camera[CAMERATYPE_PLAYER].moveV.x;	// �ړI�̎��_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posVDest.z = pPlayer->pos.z - cosf(g_camera[CAMERATYPE_PLAYER].rot.y) * LENGTH + g_camera[CAMERATYPE_PLAYER].moveV.z;	// �ړI�̎��_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posVDest.y = pPlayer->pos.y + g_camera[CAMERATYPE_PLAYER].moveV.y;

	// �����_�̍X�V
	g_camera[CAMERATYPE_PLAYER].posR.x += (g_camera[CAMERATYPE_PLAYER].posRDest.x - g_camera[CAMERATYPE_PLAYER].posR.x) * 0.4f;	// �����_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posR.z += (g_camera[CAMERATYPE_PLAYER].posRDest.z - g_camera[CAMERATYPE_PLAYER].posR.z) * 0.4f;	// �����_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posR.y += (g_camera[CAMERATYPE_PLAYER].posRDest.y - g_camera[CAMERATYPE_PLAYER].posR.y) * 0.4f;	// �����_��Y�����W�̌���

	// ���_�̍X�V
	g_camera[CAMERATYPE_PLAYER].posV.x += (g_camera[CAMERATYPE_PLAYER].posVDest.x - g_camera[CAMERATYPE_PLAYER].posV.x) * 1.0f;	// ���_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posV.z += (g_camera[CAMERATYPE_PLAYER].posVDest.z - g_camera[CAMERATYPE_PLAYER].posV.z) * 1.0f;	// ���_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER].posV.y += (g_camera[CAMERATYPE_PLAYER].posVDest.y - g_camera[CAMERATYPE_PLAYER].posV.y) * 1.0f;
}

//--------------------------------------------------------------------------------------------------------------
// �v���C���[�������낷�J����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void LookDownPlayer(void)
{
	// �v���C���[���̎擾
	Player *pPlayer = GetPlayer();

	if (GetKeyboardPress(KEYINFO_ROT_LEFT) == true)
	{// ���_�̍���]
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y < -D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(KEYINFO_ROT_RIGHT) == true)
	{// ���_�̉E��]
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y > D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= D3DX_PI * 2.0f;
		}
	}

	// �Q�[���p�b�h���̎擾
	bool *pRstckPress = GetRStickPress(PLAYER_1);

	if (pRstckPress[RSTICKPRESS_LEFT] == true)
	{// ���_�̉E��]
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y < -D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += D3DX_PI * 2.0f;
		}
	}
	if (pRstckPress[RSTICKPRESS_RIGHT] == true)
	{// ���_�̍���]
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += ROT;						// ��]��

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y > D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= D3DX_PI * 2.0f;
		}
	}

	// �ړI�̒����_
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.x = pPlayer->pos.x;		// �ړI�̒����_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.z = pPlayer->pos.z;		// �ړI�̒����_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.y = g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveR.y;

	// �ړI�̎��_
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.x = pPlayer->pos.x - sinf(g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y);	// �ړI�̎��_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.z = pPlayer->pos.z - cosf(g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y);	// �ړI�̎��_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.y = pPlayer->pos.y + g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveV.y;

	// �����_�̍X�V
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.x += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.x - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.x) * 1.0f;	// �����_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.z += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.z - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.z) * 1.0f;	// �����_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.y += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.y - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.y) * 1.0f;	// �����_��Y�����W�̌���

	// ���_�̍X�V
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.x += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.x - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.x) * 1.0f;	// ���_��X�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.z += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.z - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.z) * 1.0f;	// ���_��Z�����W�̌���
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.y += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.y - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.y) * 1.0f;
}

//--------------------------------------------------------------------------------------------------------------
// �Q�[����ʂ̃J����������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void InitGameCamera(void)
{
	InitCamera();
}


//--------------------------------------------------------------------------------------------------------------
// �v���C���[�Ǐ]�J�������̎擾
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FCamera	- �Ǐ]�J��������n��
//--------------------------------------------------------------------------------------------------------------
Camera GetPlayerCamera(void)
{
	return g_camera[CAMERATYPE_PLAYER];
}

//--------------------------------------------------------------------------------------------------------------
// �����낵�J�������̎擾
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FCamera	- �����낵�J��������n��
//--------------------------------------------------------------------------------------------------------------
Camera GetLookDownCamera(void)
{
	return g_camera[CAMERATYPE_PLAYER_LOOKDOWN];
}