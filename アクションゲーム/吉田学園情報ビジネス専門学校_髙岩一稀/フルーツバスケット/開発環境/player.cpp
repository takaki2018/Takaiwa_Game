//-----------------------------------------------------------------
//
// �v���C���[ (polygon.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "player.h"
#include "selectmode.h"
#include "block.h"
#include "fruits.h"
#include "enemy.h"
#include "bear.h"
#include "point.h"
#include "score.h"
#include "gauge.h"
#include "input.h"
#include "gamepad.h"
#include "setparticle.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)															// �v���C���[�̍ő吔
#define TEXTURE_X			(4)															// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y			(2)															// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_aPlayer[nCntPlayer].nPatternAnim)	// �e�N�X�`���̓���
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)								// U�̒l
#define UV_V				(1.0f / TEXTURE_Y)											// V�̒l

#define PLAYER_VERTEX_X		(365.0f)													// �v���C���[�̒��S���W(X)
#define PLAYER_VERTEX_Y		(650.0f)													// �v���C���[�̒��S���W(Y)
#define PlAYER_WIDTH		(180.0f)													// �v���C���[��z�u����Ԋu

#define MOVE_PLAYER			(1.5f)														// �v���C���[�̈ړ���
#define PI_QUARTER			(D3DX_PI / 4)												// �~������1/4(�΂߈ړ��Ŏg��)

#define JAMP_MOVE			(18.0f)														// �W�����v�����Ƃ��̈ړ���
#define GRAVITY				(0.8f)														// �d��

#define WOOD_WIDTH			(80.0f)														// �؂̕�

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		// �v���C���[�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayer[MAX_PLAYER] = {};	// �v���C���[�̃e�N�X�`���̃|�C���^
PLAYER g_aPlayer[MAX_PLAYER];							// �v���C���[�̏��
int g_nCntPlayer;										// �v���C���[��

//-----------------------------------------------------------------
// �v���C���[�̏���������
//-----------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// �V���O�����[�h�̎�����
	case PLAYMODE_SINGLE:
		g_nCntPlayer = 1;
		break;
		// �}���`���[�h�̎�����
	case PLAYMODE_MULTI:
		g_nCntPlayer = GetnCntController();
		break;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_1P.png", &g_pTexturePlayer[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_2P.png", &g_pTexturePlayer[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_3P.png", &g_pTexturePlayer[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_4P.png", &g_pTexturePlayer[3]);

	// �v���C���[���̏�����
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(PLAYER_VERTEX_X + PlAYER_WIDTH * nCntPlayer, PLAYER_VERTEX_Y, 0.0f);
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = D3DXVECTOR3(NULL, NULL, NULL);
		g_aPlayer[nCntPlayer].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aPlayer[nCntPlayer].fWidth = PLAYER_SIZE_WIDTH / 2.0f;
		g_aPlayer[nCntPlayer].fHeight = PLAYER_SIZE_HEIGHT / 2.0f;
		g_aPlayer[nCntPlayer].nCntAnim = 0;
		g_aPlayer[nCntPlayer].nPatternAnim = 0;
		g_aPlayer[nCntPlayer].fV = 0.0f;
		g_aPlayer[nCntPlayer].bMove = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;
		g_aPlayer[nCntPlayer].nCounterState = NULL;
		g_aPlayer[nCntPlayer].nCounterDisp = NULL;
		g_aPlayer[nCntPlayer].nCounterSize = NULL;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PLAYER, // �m�ۂ���o�b�t�@�T�C�Y
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

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// ���_���W�̐ݒ�
		SetVertexPlayer(nCntPlayer);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// �X�V
		pVtx += 4;
	}

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
	for (int nCntTexture = 0; nCntTexture < MAX_PLAYER; nCntTexture++)
	{
		if (g_pTexturePlayer[nCntTexture] != NULL)
		{
			g_pTexturePlayer[nCntTexture]->Release();
			g_pTexturePlayer[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �v���C���[�̍X�V����
//-----------------------------------------------------------------
void UpdatePlayer(void)
{
	// �ϐ��錾
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// �V���O�����[�h�̂Ƃ�����
	case PLAYMODE_SINGLE:
		SingleModePlayer();
		break;
	// �}���`���[�h�̂Ƃ�����
	case PLAYMODE_MULTI:
		MultiModePlayer();
		break;
	}
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

	for (int nCntPlayer = 0; nCntPlayer < g_nCntPlayer; nCntPlayer++)
	{// �v���C���[�̕`��
	 // �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer[nCntPlayer]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
	}
}

//-----------------------------------------------------------------
// �v���C���[�̒��_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexPlayer(int nIdx)
{
	// �ϐ��錾
	float ShaikingMove = GetShaking();

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x - g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y + g_aPlayer[nIdx].fHeight + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x - g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y - g_aPlayer[nIdx].fHeight+ ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x + g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y + g_aPlayer[nIdx].fHeight+ ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x + g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y - g_aPlayer[nIdx].fHeight+ ShaikingMove, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_aPlayer[nIdx].col;
	pVtx[1].col = g_aPlayer[nIdx].col;
	pVtx[2].col = g_aPlayer[nIdx].col;
	pVtx[3].col = g_aPlayer[nIdx].col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// �V���O�����[�h�̃v���C���[����
//-----------------------------------------------------------------
void SingleModePlayer(void)
{
	// �ϐ��錾
	MODE mode = GetMode();

	switch (mode)
	{// �Q�[����ʂ̂Ƃ�
	case MODE_GAME:
		// ���ʂ̏���
		PlayerNormalProcessing(PLAYER_1);

		// �v���C���[��ԂŎ���
		switch (g_aPlayer[PLAYER_1].state)
		{// �v���C���[��Ԃ��_�ł̂Ƃ�
		case PLAYERSTATE_FLASHING:
			PlayerStateFlashing(PLAYER_1);
			break;
		}
		break;

		// ���U���g��ʂ̂Ƃ�
	case MODE_RESULT:
		ResultPlayer(PLAYER_1);
		break;
	}
}

//-----------------------------------------------------------------
// �}���`���[�h�̃v���C���[����
//-----------------------------------------------------------------
void MultiModePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < g_nCntPlayer; nCntPlayer++)
	{
		// �ϐ��錾
		MODE mode = GetMode();

		switch (mode)
		{// �Q�[����ʂ̂Ƃ�
		case MODE_GAME:
			// ���ʂ̏���
			PlayerNormalProcessing(nCntPlayer);

			if (g_aPlayer[nCntPlayer].fHeight != PLAYER_SIZE_HEIGHT / 2.0f)
			{// �������k��ł���Ƃ�
				PlayerStateShrink(nCntPlayer);
			}
			// �v���C���[��ԂŎ���
			switch (g_aPlayer[nCntPlayer].state)
			{// �v���C���[��Ԃ��_�ł̂Ƃ�
			case PLAYERSTATE_FLASHING:
				PlayerStateFlashing(nCntPlayer);
				break;
			}
			break;

			// ���U���g��ʂ̂Ƃ�
		case MODE_RESULT:
			ResultPlayer(nCntPlayer);
			break;
		}

	}
}

//-----------------------------------------------------------------
// �V���O�����[�h�̃v���C���[�ړ�����
//-----------------------------------------------------------------
void SingleMovePlayer(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// �ϐ��錾
	bool *pStickPress = GetStickPress(PLAYER_1);
	bool *pCrossKeyPress = GetCrossKeyPress(PLAYER_1);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// �v���C���[�̈ړ�
	if (pStickPress[STICKPRESS_RIGHT] == true || 
		pCrossKeyPress[CROSSKEYPRESS_RIGHT] == true ||
		GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// �E�ړ�
		g_aPlayer[PLAYER_1].move.x += MOVE_PLAYER;
		g_aPlayer[PLAYER_1].nCntAnim++;
		g_aPlayer[PLAYER_1].fV = 0.0f;
		g_aPlayer[PLAYER_1].bMove = true;

		// ����
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[PLAYER_1].pos.x, g_aPlayer[PLAYER_1].pos.y + g_aPlayer[PLAYER_1].fHeight, 0.0f),
			g_aPlayer[PLAYER_1].move, SETPARTICLE_MOVE_PLAYER);
	}
	else if (pStickPress[STICKPRESS_LEFT] == true || 
		pCrossKeyPress[CROSSKEYPRESS_LEFT] == true ||
		GetKeyboardPress(KEYINFO_LEFT) == true)
	{// ���ړ�
		g_aPlayer[PLAYER_1].move.x -= MOVE_PLAYER;
		g_aPlayer[PLAYER_1].nCntAnim++;
		g_aPlayer[PLAYER_1].fV = 0.5f;
		g_aPlayer[PLAYER_1].bMove = true;

		// ����
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[PLAYER_1].pos.x, g_aPlayer[PLAYER_1].pos.y + g_aPlayer[PLAYER_1].fHeight, 0.0f),
			g_aPlayer[PLAYER_1].move, SETPARTICLE_MOVE_PLAYER);
	}

	// �ړ�����߂��Ƃ�
	if (pStickPress[STICKPRESS_RIGHT] == false && pStickPress[STICKPRESS_LEFT] == false &&
		pCrossKeyPress[CROSSKEYPRESS_RIGHT] == false && pCrossKeyPress[CROSSKEYPRESS_LEFT] == false &&
		GetKeyboardPress(KEYINFO_RIGHT) == false && GetKeyboardPress(KEYINFO_LEFT) == false)
	{// �����Ă��Ȃ���Ԃɂ���
		g_aPlayer[PLAYER_1].bMove = false;

		if (g_aPlayer[PLAYER_1].bJump == false)
		{// �������Ă��Ȃ��Ƃ��A�j���[�V����NO.0�ɂ���
			g_aPlayer[PLAYER_1].nPatternAnim = 0;
		}
	}
	// �W�����v�����Ƃ��܂��͋󒆂ɂ���Ƃ�
	if (g_aPlayer[PLAYER_1].bJump == false)
	{
		if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true ||
			GetKeyboardTrigger(KEYINFO_JAMP) == true)
		{
			// �T�E���h
			PlaySound(SOUND_LABEL_SE_JUMP);

			// �W�����v����
			g_aPlayer[PLAYER_1].move.y -= JAMP_MOVE;
			g_aPlayer[PLAYER_1].nPatternAnim = 1;
			g_aPlayer[PLAYER_1].bJump = true;
		}
		if (g_aPlayer[PLAYER_1].move.y != 0.0f)
		{// �W�����v���̏���
			g_aPlayer[PLAYER_1].bJump = true;
			g_aPlayer[PLAYER_1].nPatternAnim = 1;
		}
	}
	// �d�͂̐ݒ�
	g_aPlayer[PLAYER_1].move.y += GRAVITY;

	// �ʒu�X�V
	g_aPlayer[PLAYER_1].pos.x += g_aPlayer[PLAYER_1].move.x;
	g_aPlayer[PLAYER_1].pos.y += g_aPlayer[PLAYER_1].move.y;

	// �u���b�N�Ƃ̓����蔻��
	HitBlock(PLAYER_1);

	// ��ʊO�ɏo�Ȃ��悤�ɂ���
	if (g_aPlayer[PLAYER_1].pos.y - g_aPlayer[PLAYER_1].fHeight >= 580.0f)
	{// ���̂�����W�̍����ɂ���Ƃ�
		if (g_aPlayer[PLAYER_1].pos.x - g_aPlayer[PLAYER_1].fWidth * 0.7f <= 50.0f)
		{// �����̌��ɂ���Ƃ������̖؂̏ォ��o��
			g_aPlayer[PLAYER_1].pos.x = g_aPlayer[PLAYER_1].fWidth * 0.7f + 50.0f;
			g_aPlayer[PLAYER_1].pos.y = 270.0f;
		}
		else if (g_aPlayer[PLAYER_1].pos.x - g_aPlayer[PLAYER_1].fWidth * 0.7f >= SCREEN_WIDTH - 50.0f)
		{// �E���̌��ɂ���Ƃ��E���̖؂̏ォ��o��
			g_aPlayer[PLAYER_1].pos.x = SCREEN_WIDTH - 50.0f - g_aPlayer[PLAYER_1].fWidth * 0.7f;
			g_aPlayer[PLAYER_1].pos.y = 270.0f;
		}
	}
	else if (g_aPlayer[PLAYER_1].pos.x - g_aPlayer[PLAYER_1].fWidth * 0.7f <= WOOD_WIDTH)
	{// ��ԍ��ɓ��B�����Ƃ�����
		g_aPlayer[PLAYER_1].pos.x = g_aPlayer[PLAYER_1].fWidth * 0.7f + WOOD_WIDTH;
	}
	else if (g_aPlayer[PLAYER_1].pos.x + g_aPlayer[PLAYER_1].fWidth * 0.7f >= SCREEN_WIDTH - WOOD_WIDTH)
	{// ��ԉE�ɓ��B�����Ƃ�����
		g_aPlayer[PLAYER_1].pos.x = SCREEN_WIDTH - WOOD_WIDTH - g_aPlayer[PLAYER_1].fWidth * 0.7f;
	}

	// �ړ��ʂ̌���(����)
	g_aPlayer[PLAYER_1].move.x += (0.0f - g_aPlayer[PLAYER_1].move.x) * 0.3f;

	// ���_���W�̍X�V
	SetVertexPlayer(PLAYER_1);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// �}���`���[�h�̃v���C���[�ړ�����
//-----------------------------------------------------------------
void MultiMovePlayer(int nCntPlayer)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// �ϐ��錾
	bool *pStickPress = GetStickPress(nCntPlayer);
	bool *pCrossKeyPress = GetCrossKeyPress(nCntPlayer);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// �v���C���[�̈ړ�
	if (pStickPress[STICKPRESS_RIGHT] == true || pCrossKeyPress[CROSSKEYPRESS_RIGHT] == true)
	{// �E�ړ�
		g_aPlayer[nCntPlayer].move.x += MOVE_PLAYER;
		g_aPlayer[nCntPlayer].nCntAnim++;
		g_aPlayer[nCntPlayer].fV = 0.0f;
		g_aPlayer[nCntPlayer].bMove = true;

		// ����
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight, 0.0f),
			g_aPlayer[nCntPlayer].move, SETPARTICLE_MOVE_PLAYER);
	}
	else if (pStickPress[STICKPRESS_LEFT] == true || pCrossKeyPress[CROSSKEYPRESS_LEFT] == true)
	{// ���ړ�
		g_aPlayer[nCntPlayer].move.x -= MOVE_PLAYER;
		g_aPlayer[nCntPlayer].nCntAnim++;
		g_aPlayer[nCntPlayer].fV = 0.5f;
		g_aPlayer[nCntPlayer].bMove = true;

		// ����
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight, 0.0f),
			g_aPlayer[nCntPlayer].move, SETPARTICLE_MOVE_PLAYER);
	}

	// �ړ�����߂��Ƃ�
	if (pStickPress[STICKPRESS_RIGHT] == false && pStickPress[STICKPRESS_LEFT] == false && 
		pCrossKeyPress[CROSSKEYPRESS_RIGHT] == false && pCrossKeyPress[CROSSKEYPRESS_LEFT] == false)
	{// �����Ă��Ȃ���Ԃɂ���
		g_aPlayer[nCntPlayer].bMove = false;
		if (g_aPlayer[nCntPlayer].bJump == false)
		{// �������Ă��Ȃ��Ƃ��A�j���[�V����NO.0�ɂ���
			g_aPlayer[nCntPlayer].nPatternAnim = 0;
		}
	}
	// �W�����v�����Ƃ��܂��͋󒆂ɂ���Ƃ�
	if (g_aPlayer[nCntPlayer].bJump == false)
	{
		if (GetJoypadTrigger(nCntPlayer,JPINFO_OKorJAMP) == true)
		{
			// �T�E���h
			PlaySound(SOUND_LABEL_SE_JUMP);

			g_aPlayer[nCntPlayer].move.y -= JAMP_MOVE;
			g_aPlayer[nCntPlayer].nPatternAnim = 1;
			g_aPlayer[nCntPlayer].bJump = true;
		}
		if (g_aPlayer[nCntPlayer].move.y != 0.0f)
		{
			g_aPlayer[nCntPlayer].bJump = true;
			g_aPlayer[nCntPlayer].nPatternAnim = 1;
		}
	}
	// �d�͂̐ݒ�
	g_aPlayer[nCntPlayer].move.y += GRAVITY;

	// �ʒu�X�V
	g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
	g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y;

	// �u���b�N�Ƃ̓����蔻��
	HitBlock(nCntPlayer);

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(nCntPlayer);

	// ��ʊO�ɏo�Ȃ��悤�ɂ���
	if (g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight >= 580.0f)
	{// ���̂�����W�̍����ɂ���Ƃ�
		if (g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f <= 50.0f)
		{// �����̌��ɂ���Ƃ������̖؂̏ォ��o��
			g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntPlayer].fWidth * 0.7f + 50.0f;
			g_aPlayer[nCntPlayer].pos.y = 270.0f;
		}
		else if (g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f >= SCREEN_WIDTH - 50.0f)
		{// �E���̌��ɂ���Ƃ��E���̖؂̏ォ��o��
			g_aPlayer[nCntPlayer].pos.x = SCREEN_WIDTH - 50.0f - g_aPlayer[nCntPlayer].fWidth * 0.7f ;
			g_aPlayer[nCntPlayer].pos.y = 270.0f;
		}
	}
	else if (g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f <= WOOD_WIDTH)
	{// ��ԍ��ɓ��B�����Ƃ�����
		g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntPlayer].fWidth * 0.7f + WOOD_WIDTH;
	}
	else if (g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth * 0.7f >= SCREEN_WIDTH - WOOD_WIDTH)
	{// ��ԉE�ɓ��B�����Ƃ�����
		g_aPlayer[nCntPlayer].pos.x = SCREEN_WIDTH - WOOD_WIDTH - g_aPlayer[nCntPlayer].fWidth * 0.7f;
	}

	// �ړ��ʂ̌���(����)
	g_aPlayer[nCntPlayer].move.x += (0.0f - g_aPlayer[nCntPlayer].move.x) * 0.3f;

	// ���_���W�̍X�V
	SetVertexPlayer(nCntPlayer);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// �v���C���[�̃A�j���[�V����
//-----------------------------------------------------------------
void AnimationPlayer(int nCntPlayer)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCntPlayer * 4;

	if (0 == g_aPlayer[nCntPlayer].nCntAnim % 5 && g_aPlayer[nCntPlayer].bJump == false && g_aPlayer[nCntPlayer].bMove == true)
	{
		g_aPlayer[nCntPlayer].nPatternAnim = (g_aPlayer[nCntPlayer].nPatternAnim + 1) % TEXTURE_X;
	}
	// �A�j���[�V�����̍X�V
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V + g_aPlayer[nCntPlayer].fV);
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f + g_aPlayer[nCntPlayer].fV);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V + g_aPlayer[nCntPlayer].fV);
	pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f + g_aPlayer[nCntPlayer].fV);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// �u���b�N�̓����蔻��
//-----------------------------------------------------------------
void HitBlock(int nCntPlayer)
{
	// �ϐ��錾
	bool bLand;			// �u���b�N�ɏ���Ă��邩

	// �u���b�N�Ƃ̓����蔻��
	bLand = CollisionBlock(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move,
		g_aPlayer[nCntPlayer].fWidth * 0.7f, g_aPlayer[nCntPlayer].fHeight,COLLISIONINFO_PLAYER);

	// �u���b�N�ɏ���Ă���Ƃ��W�����v�ł���悤�ɂ���
	if (bLand == true)
	{
		g_aPlayer[nCntPlayer].move.y = 0.0f;
		g_aPlayer[nCntPlayer].bJump = false;
	}
}

//-----------------------------------------------------------------
// �v���C���[�̓����蔻��
//-----------------------------------------------------------------
void HitPlayer(int nCntPlayer, float fAngle)
{
	// �m�b�N�o�b�N�̐ݒ�
	g_aPlayer[nCntPlayer].move.x += 10.0f * sinf(fAngle);
	g_aPlayer[nCntPlayer].move.y -= 15.0f;

	// �v���C���[��Ԃ�_�łɂ���
	g_aPlayer[nCntPlayer].state = PLAYERSTATE_FLASHING;

	// ��ԃJ�E���^�̐ݒ�
	g_aPlayer[nCntPlayer].nCounterState = 150;

	// �T�E���h
	PlaySound(SOUND_LABEL_SE_DAMAGE);
}

//-----------------------------------------------------------------
// �v���C���[���m�̓����蔻��
//-----------------------------------------------------------------
void CollisionPlayer(int nCntPlayer)
{
	// �v���C���[���m�̓����蔻��
	for (int nCntCollisionPlayer = 0; nCntCollisionPlayer < g_nCntPlayer; nCntCollisionPlayer++)
	{
		if (nCntPlayer != nCntCollisionPlayer)
		{// �����ȊO�̃v���C���[�̂Ƃ�
			if ((g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f) < (g_aPlayer[nCntCollisionPlayer].pos.x + g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f) &&
				(g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth * 0.7f) > (g_aPlayer[nCntCollisionPlayer].pos.x - g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
			{// �v���C���[�����̃v���C���[�̏�ɂ���Ƃ�
				if (g_aPlayer[nCntPlayer].posOld.y + g_aPlayer[nCntPlayer].fHeight <= g_aPlayer[nCntCollisionPlayer].posOld.y - g_aPlayer[nCntCollisionPlayer].fHeight)
				{// �v���C���[�����̃v���C���[�̏�ɂ���Ƃ�
					if (g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight > g_aPlayer[nCntCollisionPlayer].pos.y - g_aPlayer[nCntCollisionPlayer].fHeight)
					{// �߂荞�񂾂珈��
						g_aPlayer[nCntPlayer].pos.y = g_aPlayer[nCntCollisionPlayer].pos.y - g_aPlayer[nCntCollisionPlayer].fHeight - g_aPlayer[nCntPlayer].fHeight;
						g_aPlayer[nCntCollisionPlayer].move.y = 0.0f;
						g_aPlayer[nCntPlayer].move.y -= 10.0f;

						// �T�E���h
						PlaySound(SOUND_LABEL_SE_STOMP);

						if (g_aPlayer[nCntCollisionPlayer].fHeight == PLAYER_SIZE_HEIGHT / 2.0f)
						{// ���܂ꂽ�v���C���[�̃T�C�Y������������
							g_aPlayer[nCntCollisionPlayer].fHeight /= 1.25f;
							g_aPlayer[nCntCollisionPlayer].nCounterSize = 300;
						}
					}
				}
			}
			if ((g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight) > (g_aPlayer[nCntCollisionPlayer].pos.y - g_aPlayer[nCntCollisionPlayer].fHeight) &&
				(g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight) < (g_aPlayer[nCntCollisionPlayer].pos.y + g_aPlayer[nCntCollisionPlayer].fHeight))
			{// �v���C���[�����̃v���C���[�̉��ɂ���Ƃ�
				if ((g_aPlayer[nCntPlayer].posOld.x + g_aPlayer[nCntPlayer].fWidth * 0.7f) <= (g_aPlayer[nCntCollisionPlayer].posOld.x - g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
				{// �v���C���[�����̃v���C���[�̍����ɂ���Ƃ�
					if ((g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth * 0.7f) > (g_aPlayer[nCntCollisionPlayer].pos.x - g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
					{// �߂荞�񂾂瑼�̃v���C���[�̍��̍��W�ɂ���
						g_aPlayer[nCntCollisionPlayer].pos.x += (g_aPlayer[nCntPlayer].move.x / 2.0f) - (g_aPlayer[nCntCollisionPlayer].move.x / 2.0f);
						g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntCollisionPlayer].pos.x - (g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f) - (g_aPlayer[nCntPlayer].fWidth * 0.7f);
					}
				}
				else if ((g_aPlayer[nCntPlayer].posOld.x - g_aPlayer[nCntPlayer].fWidth * 0.7f) >= (g_aPlayer[nCntCollisionPlayer].posOld.x + g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
				{// �v���C���[�����̃v���C���[�̉E���ɂ���Ƃ�
					if ((g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f) < (g_aPlayer[nCntCollisionPlayer].pos.x + g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
					{// �߂荞�񂾂瑼�̃v���C���[�̉E�̍��W�ɂ���
						g_aPlayer[nCntCollisionPlayer].pos.x += (g_aPlayer[nCntPlayer].move.x / 2.0f) - (g_aPlayer[nCntCollisionPlayer].move.x / 2.0f);
						g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntCollisionPlayer].pos.x + (g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f) + (g_aPlayer[nCntPlayer].fWidth * 0.7f);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------
// �v���C���[��Ԃ��ʏ�̎�
//-----------------------------------------------------------------
void PlayerNormalProcessing(int nCntPlayer)
{
	// �ϐ��錾
	MODE mode = GetMode();					// �Q�[�����[�h�̎擾
	PLAYMODE playmode = GetPlayMode();		// �v���C���[�h�̎擾

	// �A�j���[�V�����̍X�V
	AnimationPlayer(nCntPlayer);

	// ���݂̈ʒu����ۑ�
	g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

	if (mode == MODE_GAME)
	{// �ړ�����
		switch (playmode)
		{// �V���O�����[�h�̂Ƃ�����
		case PLAYMODE_SINGLE:
			SingleMovePlayer();
			break;
		// �}���`���[�h�̂Ƃ�����
		case PLAYMODE_MULTI:
			MultiMovePlayer(nCntPlayer);
			break;
		}		
	}
}

//-----------------------------------------------------------------
// �v���C���[��Ԃ��_�ł̎�
//-----------------------------------------------------------------
void PlayerStateFlashing(int nCntPlayer)
{
	// ��ԃJ�E���^�̌���
	g_aPlayer[nCntPlayer].nCounterState--;

	// �\���J�E���^�̉��Z
	g_aPlayer[nCntPlayer].nCounterDisp++;

	if (g_aPlayer[nCntPlayer].nCounterState <= 0)
	{// ��ԃJ�E���^��0�ȉ��̂Ƃ���Ԃ�ʏ�ɕύX
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;

		// �\���J�E���g�̏�����
		g_aPlayer[nCntPlayer].nCounterDisp = 0;
	}
	else
	{// ��ԃJ�E���^��0���傫���Ƃ�
		if (5 == g_aPlayer[nCntPlayer].nCounterDisp % 10)
		{// �\���J�E���^��5�̂Ƃ����l��1.0f�ɐݒ�
			g_aPlayer[nCntPlayer].col.a = 1.0f;
		}
		else if (0 == g_aPlayer[nCntPlayer].nCounterDisp % 10)
		{// �\���J�E���^��10�̂Ƃ����l��0.2f�ɐݒ�
			g_aPlayer[nCntPlayer].col.a = 0.2f;
		}
	}
}

//-----------------------------------------------------------------
// �v���C���[��Ԃ��k��ł���Ƃ�
//-----------------------------------------------------------------
void PlayerStateShrink(int nCntPlayer)
{
	// ��ԃJ�E���^�̌���
	g_aPlayer[nCntPlayer].nCounterSize--;

	if (g_aPlayer[nCntPlayer].nCounterSize <= 0)
	{// �\���J�E���g�̏�����
		g_aPlayer[nCntPlayer].nCounterSize = 0;

		// ������������
		g_aPlayer[nCntPlayer].move.y -= 5.0f;

		// ����������������
		g_aPlayer[nCntPlayer].fHeight = PLAYER_SIZE_HEIGHT / 2.0f;
	}
}

//-----------------------------------------------------------------
// ���U���g��ʂ̃v���C���[
//-----------------------------------------------------------------
void ResultPlayer(int nCntPlayer)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// �ϐ��錾
	float fmove = GetMoveGauge(nCntPlayer);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCntPlayer * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// �v���C���[���̎擾
//-----------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_aPlayer[0];
}

//-----------------------------------------------------------------
// �v���C���[���̊l��
//-----------------------------------------------------------------
int GetPlayerCnt(void)
{
	return g_nCntPlayer;
}