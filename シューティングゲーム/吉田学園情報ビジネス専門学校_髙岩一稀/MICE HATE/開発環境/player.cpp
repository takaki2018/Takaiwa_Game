//-----------------------------------------------------------------
//
// �v���C���[ (polygon.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "player.h"
#include "bg.h"
#include "input.h"
#include "gamepad.h"
#include "bullet.h"
#include "enemy.h"
#include "setparticle.h"
#include "object.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define PLAYER_VERTEX_X			(SCREEN_WIDTH / 2)			// �v���C���[�̒��S���W(X)
#define PLAYER_VERTEX_Y			(SCREEN_HEIGHT / 2)			// �v���C���[�̒��S���W(Y)

#define PLAYER_SIZE_WIDTH		(15)						// �v���C���[�̃T�C�Y(��)
#define PLAYER_SIZE_HEIGHT		(15)						// �v���C���[�̃T�C�Y(����)

#define MOVE_PLAYER				(0.5f)						// �v���C���[�̈ړ���
#define PI_QUARTER				(D3DX_PI / 4)				// �~������1/4(�΂߈ړ��Ŏg��)

#define HIT_DATA				(15)						// �����蔻��
#define RENGE					(250)						// �~�̔��a

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// �v���C���[�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			// �v���C���[�̃e�N�X�`���̃|�C���^
PLAYER g_Player;									// �v���C���[�̏��
D3DXVECTOR3 g_rotPlayer;							// �v���C���[�̉�]�p	
float g_fLengthPlayer;								// �v���C���[�̑Ίp���̒���
float g_fAnglePlayer;								// �v���C���[�̑Ίp���̊p�x
D3DXCOLOR g_Playercol;								// �v���C���[�̐F
D3DXCOLOR g_PlyaerOrbit;							// �v���C���[�̋O�Ղ̐F

//-----------------------------------------------------------------
// �v���C���[�̏���������
//-----------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_Playercol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_PlyaerOrbit = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player100.png", &g_pTexturePlayer);

	// �v���C���[���̏�����
	g_Player.pos = D3DXVECTOR3(PLAYER_VERTEX_X, PLAYER_VERTEX_Y, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(NULL, NULL, NULL);
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.nCounterState = NULL;
	g_Player.nCounterDisp = NULL;
	g_Player.bDisp = false;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// �Ίp���̒���
	g_fLengthPlayer = sqrtf(PLAYER_SIZE_WIDTH * PLAYER_SIZE_WIDTH + PLAYER_SIZE_HEIGHT * PLAYER_SIZE_HEIGHT);

	// �Ίp���̊p�x
	g_fAnglePlayer = atan2f(PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE_WIDTH, g_Player.pos.y + PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE_WIDTH, g_Player.pos.y - PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE_WIDTH, g_Player.pos.y + PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE_WIDTH, g_Player.pos.y - PLAYER_SIZE_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_Playercol;
	pVtx[1].col = g_Playercol;
	pVtx[2].col = g_Playercol;
	pVtx[3].col = g_Playercol;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �v���G���[�̏I������
//-----------------------------------------------------------------
void UninitPlayer(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//-----------------------------------------------------------------
// �v���C���[�̍X�V����
//-----------------------------------------------------------------
void UpdatePlayer(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// �v���C���[�̏��
	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		// �ʏ�̎�
		PlayerStateNormal();
		break;

	case PLAYERSTATE_HIT:
		// �G�ɓ��������܂��͍U�����󂯂��Ƃ�
		PlayerStateHit();
		break;

	case PLAYERSTATE_FLASHING:
		// �_�ł̂Ƃ�
		PlayerStateFlashing();
		break;

	default:
		break;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// �v���C���[�̐F�X�V
	pVtx[0].col = g_Playercol;
	pVtx[1].col = g_Playercol;
	pVtx[2].col = g_Playercol;
	pVtx[3].col = g_Playercol;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// �v���C���[�̕`�揈��
//-----------------------------------------------------------------
void DrawPlayer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	// �v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// �v���C���[�̈ړ�����
//-----------------------------------------------------------------
void MovePlayer(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	DIJOYSTATE2 Controller = GetControllerPless();

	// �ϐ��錾
	D3DXVECTOR3 posObject = GetposObject();
	int nCntBulletcol = GetBulletcol();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// �v���C���[�̈ʒu���X�V
	if (GetKeyboardPress(KEYINFO_UP) == true )
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// �E�΂ߏ�ړ�
			g_Player.move.x += MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y -= MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// ���΂ߏ�ړ�
			g_Player.move.x -= MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y -= MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else
		{// ��ړ�
			g_Player.move.y -=  MOVE_PLAYER;
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// �E�΂߉��ړ�
			g_Player.move.x += MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y += MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// ���΂߉��ړ�
			g_Player.move.x -= MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y += MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else 
		{// ���ړ�
			g_Player.move.y += MOVE_PLAYER;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// �E�ړ�
		g_Player.move.x += MOVE_PLAYER;
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// ���ړ�
		g_Player.move.x -= MOVE_PLAYER;
	}
	// JOYPAD�̃X�e�B�b�N����
	if (Controller.lY != 0 || Controller.lX != 0)
	{// �X�e�B�b�N�̓��͊p�x
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY * -1);

		g_Player.move.x += MOVE_PLAYER * sinf(D3DX_PI - fAngle);
		g_Player.move.y += MOVE_PLAYER * cosf(D3DX_PI - fAngle);
	}
	// ��]
	if (GetKeyboardPress(KEYINFO_LEFT_TURN) == true || GetJoypadPress(JPINFO_LEFT_TURN) == true)
	{// ����]
		g_Player.rot.z += 0.07f;
		if (g_Player.rot.z > D3DX_PI)
		{
			g_Player.rot.z -= D3DX_PI * 2.0f;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT_TURN) == true || GetJoypadPress(JPINFO_RIGHT_TURN) == true)
	{// �E��]
		g_Player.rot.z -= 0.07f;
		if (g_Player.rot.z > -D3DX_PI)
		{
			g_Player.rot.z += D3DX_PI * 2.0f;
		}
	}
	// �ʒu�X�V
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	// ���@�ƒ��S�_�̑Ίp���̒���
	float fLength = sqrtf((g_Player.pos.x - posObject.x) * (g_Player.pos.x - posObject.x) +
		(g_Player.pos.y - posObject.y) * (g_Player.pos.y - posObject.y));

	if (fLength > RENGE)
	{// ���݂̊p�x
		float fAngle = atan2f(g_Player.pos.x - posObject.x, g_Player.pos.y - posObject.y);
		
		// ���W�̐ݒ�
		g_Player.pos.x = RENGE * sinf(fAngle) + posObject.x;
		g_Player.pos.y = RENGE * cosf(fAngle) + posObject.y;
	}

	// ��ʊO�ɏo�Ȃ��悤�ɂ���
	if (g_Player.pos.y - PLAYER_SIZE_HEIGHT < 0)
	{// ��ԏ�ɓ��B�����Ƃ�����
		g_Player.pos.y = PLAYER_SIZE_HEIGHT;
	}
	if (g_Player.pos.y + PLAYER_SIZE_HEIGHT > SCREEN_HEIGHT)
	{// ��ԉ��ɓ��B�����Ƃ�����
		g_Player.pos.y = SCREEN_HEIGHT - PLAYER_SIZE_HEIGHT;
	}
	if (g_Player.pos.x - PLAYER_SIZE_WIDTH <= 0)
	{// ��ԍ��ɓ��B�����Ƃ�����
		g_Player.pos.x = PLAYER_SIZE_WIDTH;
	}
	if (g_Player.pos.x + PLAYER_SIZE_WIDTH >= SCREEN_WIDTH)
	{// ��ԉE�ɓ��B�����Ƃ�����
		g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZE_WIDTH;
	}

	// �ړ��ʂ̌���(����)
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
	g_Player.move.y += (0.0f - g_Player.move.y) * 0.1f;

	// ���_���W�̍X�V
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI * 0.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI * 0.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 1.00f)) * g_fLengthPlayer;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 1.00f)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 0.00f)) * g_fLengthPlayer;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 0.00f)) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI * 1.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI * 1.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// �O���F�̐ݒ�
	if (nCntBulletcol == BULLETCOLOR_WHITE)
	{// ���ɐݒ�
		g_PlyaerOrbit = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.9f);
	}
	else if (nCntBulletcol == BULLETCOLOR_RED)
	{// �Ԃɐݒ�
		g_PlyaerOrbit = D3DXCOLOR(0.9f, 0.3f, 0.1f, 0.9f);
	}
	else if (nCntBulletcol == BULLETCOLOR_BLUE)
	{// �ɐݒ�
		g_PlyaerOrbit = D3DXCOLOR(0.1f, 0.3f, 0.9f, 0.9f);
	}
	// �v���C���[�̋O��
	SetMoveOrbit(g_Player.pos,
		D3DXVECTOR3(sinf(g_Player.rot.z) * (float)(rand() % 10 + 5), cosf(g_Player.rot.z) * (float)(rand() % 10 + 5), 0.0f),
		g_PlyaerOrbit,
		SETPARTICLE_MOVE_PLAYER);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// �v���C���[�̓����蔻��
//-----------------------------------------------------------------
bool HitPlayer(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	ENEMY *pEnemy;
	BULLET *pBullet;

	// �ϐ��̎󂯓n��
	pEnemy = GetEnemy();
	pBullet = GetBullet();

	// ���Ԗڂ�
	pEnemy += nIdx;
	pBullet += nIdx;

	if (pEnemy->bUse == true || pBullet->bUse == true)
	{// �G�܂��͓G�̒e���g���Ă���Ƃ�
		// ���l��0.0f�ɐݒ�
		g_Playercol.a = 0.0f;

		// �v���C���[�̏�Ԃ𓖂������Ƃ��ɕύX
		g_Player.state = PLAYERSTATE_HIT;

		// ��ԃJ�E���^��60�ɐݒ�
		g_Player.nCounterState = 60;

		// �����p�[�e�B�N��
		SetBom(g_Player.pos, SETPARTICLE_HIT_PLAYER);
		
		// ���ʉ�(����)
		PlaySound(SOUND_LABEL_SE_PLAYER_BRAEK);

		return true;
	}
	else
	{
		return false;
	}
}

//-----------------------------------------------------------------
// �v���C���[��Ԃ��ʏ�̎�
//-----------------------------------------------------------------
void PlayerStateNormal(void)
{
	// �\���̂̃|�C���^�ϐ�
	ENEMY *pEnemy;

	// �ϐ��̎󂯓n��
	pEnemy = GetEnemy();

	// �\���J�E���^�̏�����
	g_Player.nCounterDisp = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// ���@�ƓG�̓����蔻��
			if (pEnemy->pos.y - ENEMY_Y <= g_Player.pos.y + HIT_DATA &&
				pEnemy->pos.y + ENEMY_Y >= g_Player.pos.y - HIT_DATA &&
				pEnemy->pos.x - ENEMY_X + pEnemy->move.x <= g_Player.pos.x + HIT_DATA &&
				pEnemy->pos.x + ENEMY_X + pEnemy->move.x >= g_Player.pos.x - HIT_DATA)
			{// �G�ɓ��������珈��
				HitPlayer(nCntEnemy);
			}
		}
	}
	// �ړ�����
	MovePlayer();
}

//-----------------------------------------------------------------
// �v���C���[��Ԃ��U�����󂯂��Ƃ�
//-----------------------------------------------------------------
void PlayerStateHit(void)
{
	// ��ԃJ�E���^�̌���
	g_Player.nCounterState--;

	if (g_Player.nCounterState <= 0)
	{// ��ԃJ�E���^��0�ȉ��ɂȂ����Ƃ�
		g_Player.nCounterState = 100;

		// �v���C���[��Ԃ�_�łɕύX
		g_Player.state = PLAYERSTATE_FLASHING;
	}
}

//-----------------------------------------------------------------
// �v���C���[��Ԃ��_�ł̎�
//-----------------------------------------------------------------
void PlayerStateFlashing(void)
{
	// ��ԃJ�E���^�̌���
	g_Player.nCounterState--;

	// �\���J�E���^�̉��Z
	g_Player.nCounterDisp++;

	if (g_Player.nCounterState <= 0)
	{// ��ԃJ�E���^��0�ȉ��̂Ƃ���Ԃ�ʏ�ɕύX
		g_Player.state = PLAYERSTATE_NORMAL;
	}
	else
	{// ��ԃJ�E���^��0���傫���Ƃ�
		if (g_Player.nCounterDisp == 5)
		{// �\���J�E���^��5�̂Ƃ����l��1.0f�ɐݒ�
			g_Playercol.a = 1.0f;
		}
		else if (g_Player.nCounterDisp == 10)
		{// �\���J�E���^��10�̂Ƃ����l��0.2f�ɐݒ�
			g_Playercol.a = 0.2f;

			// �\���J�E���g�̏�����
			g_Player.nCounterDisp = 0;
		}
	}

	// �ړ�����
	MovePlayer();
}

//-----------------------------------------------------------------
// �v���C���[���̎擾
//-----------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_Player;
}