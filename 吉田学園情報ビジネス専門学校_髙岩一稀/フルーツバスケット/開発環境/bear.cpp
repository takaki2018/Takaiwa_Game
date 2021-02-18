//-------------------------------------------------------------------
//
// �N�} (bear.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "bear.h"
#include "player.h"
#include "block.h"
#include "timer.h"
#include "score.h"
#include "point.h"
#include "title.h"
#include "input.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define BEAR_SIZE_X			(120.0f)				// �N�}�̃T�C�Y(��)
#define BEAR_SIZE_Y			(85.0f)					// �N�}�̃T�C�Y(����)

#define ADD_GRAVITY			(1.0f)					// �d��
#define JAMP_COUNT			(20)					// �W�����v�܂ł̃J�E���g
#define TRUN_COUNT			(150)					// ������ς���܂ł̃J�E���g
#define JUMP_BEAR			(18.0f)					// �N�}�̃W�����v��
#define SHAKE_SCREEN		(10.0f)					// ��ʗh��̗�
#define SHAKE_COUNTER		(5)						// ��ʗh��̃J�E���^
#define SHAKE_MULTIPLAY		(0.95f)					// ��ʗh��̗ʂɊ|�����킹�鐔

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBear = NULL;	// �N�}�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBear = NULL;		// �N�}�̒��_�o�b�t�@�̃|�C���^
BEAR g_Bear;									// �N�}�̏��
float g_fmove;								

//-----------------------------------------------------------------
// �N�}�̏���������
//-----------------------------------------------------------------
HRESULT InitBear(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̏�����
	pDevice = GetDevice();
	g_fmove = 0.0f;

	// �ϐ��̏�����
	g_Bear.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Bear.posOld = g_Bear.pos;
	g_Bear.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Bear.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Bear.fWidth = BEAR_SIZE_X / 2.0f;
	g_Bear.fHeight = BEAR_SIZE_Y / 2.0f;
	g_Bear.fTexU = 1.0f;
	g_Bear.fTexV = 1.0f;
	g_Bear.nAppTime = rand()%10 + 10;
	g_Bear.nJampCnt = 0;
	g_Bear.nTurnCnt = 0;
	g_Bear.bJump = false;
	g_Bear.bLand = false;
	g_Bear.bEvent = false;
	g_Bear.bAppear = false;
	g_Bear.bUse = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bear.png", &g_pTextureBear);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBear,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBear->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetVertexBear();

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBear->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �N�}�̏I������
//-----------------------------------------------------------------
void UninitBear(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBear != NULL)
	{
		g_pVtxBuffBear->Release();
		g_pVtxBuffBear = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureBear != NULL)
	{
		g_pTextureBear->Release();
		g_pTextureBear = NULL;
	}
}

//-----------------------------------------------------------------
// �N�}�̍X�V����
//-----------------------------------------------------------------
void UpdateBear(void)
{
	// �ϐ��錾
	MODE mode = GetMode();

	switch (mode)
	{// �^�C�g����ʂ̂Ƃ�
	case MODE_TITLE:
		TitleBear();
		break;

		// �Q�[����ʂ̂Ƃ�
	case MODE_GAME:
		GameBear();
		break;
	}
}

//-----------------------------------------------------------------
// �N�}�̕`�揈��
//-----------------------------------------------------------------
void DrawBear(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBear, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �o�E���h�̕`��
	if (g_Bear.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureBear);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------
// �^�C�g����ʂ̃N�}
//-----------------------------------------------------------------
void TitleBear(void)
{
	// �ϐ��錾
	bool bSkipTitle = GetSkipTitle();

	// �N�}��ݒu
	if (g_Bear.bUse == false && g_Bear.bAppear == false)
	{
		SetBear(D3DXVECTOR3(1280.0f, 640.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));

		// �o�ꂵ�����Ƃ�`����
		g_Bear.bAppear = true;
	}

	if (g_Bear.bUse == true && g_Bear.bAppear == true && bSkipTitle == false)
	{
		// �N�}�̓���
		ActionBear();
	}
	else if (bSkipTitle == true)
	{// ��ʗh�������
		g_fmove = 0.0f;
	}

	// ���_���W�̍X�V
	SetVertexBear();
}

//-----------------------------------------------------------------
// �Q�[����ʂ̃N�}
//-----------------------------------------------------------------
void GameBear(void)
{
	// �ϐ��錾
	int nTimer = GetTimer();

	if (nTimer == g_Bear.nAppTime)
	{
		SetBear(D3DXVECTOR3(1280.0f, 450.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	}

	if (g_Bear.bUse == true)
	{
		// �N�}�̓���
		ActionBear();

		// �N�}�̓����蔻��
		HitBear();

		// ���_���W�̍X�V
		SetVertexBear();
	}
}

//-----------------------------------------------------------------
// �N�}�̓���
//-----------------------------------------------------------------
void ActionBear(void)
{
	// �ʒu��ۑ�
	g_Bear.posOld = g_Bear.pos;

	// �ʒu�̍X�V
	g_Bear.pos += g_Bear.move;

	// �d��
	g_Bear.move.y += ADD_GRAVITY;

	// �u���b�N�Ƃ̓����蔻��
	g_Bear.bLand = CollisionBlock(&g_Bear.pos, &g_Bear.posOld, &g_Bear.move, g_Bear.fWidth, g_Bear.fHeight, COLLISIONINFO_ENEMY);

	if (g_Bear.pos.x <= 1130.0f && g_Bear.bEvent == false && g_Bear.bJump == false)
	{// �N�}������̈ʒu�܂ŗ����Ƃ��������~�߂ăW�����v�܂ł̃J�E���g���X�V
		g_Bear.nJampCnt++;
		g_Bear.move.x = 0.0f;

		if (g_Bear.nJampCnt == JAMP_COUNT)
		{// �W�����v
			g_Bear.move.y = -JUMP_BEAR;
			g_Bear.bJump = true;
		}
	}
	else if (g_Bear.bJump == true && g_Bear.bLand == true)
	{// �u���b�N�ɏ���Ă��ăW�����v���Ă���Ƃ�����
		g_Bear.bJump = false;
		g_Bear.bEvent = true;

		// ��ʗh��̒l��ݒ�
		g_fmove = SHAKE_SCREEN;
	}
	else if (g_Bear.bEvent == true)
	{// �C�x���g�������Ƃ��J�E���g���X�V
		g_Bear.nTurnCnt++;

		if (2 == g_Bear.nTurnCnt % 5)
		{
			g_fmove *= -SHAKE_MULTIPLAY;
		}
		else if (4 == g_Bear.nTurnCnt % 5)
		{
			g_fmove *= -SHAKE_MULTIPLAY;
		}

		if (g_Bear.nTurnCnt == TRUN_COUNT)
		{
			g_Bear.move.x = 1.0f;			// �ړ��ʂ̍X�V
			g_Bear.fTexU = -1.0f;			// �e�N�X�`�����WU�̍X�V
			g_Bear.bEvent = false;			// �C�x���g���I����

											// ��ʗh��̒l��ݒ�
			g_fmove = 0.0f;
		}
	}
}

//-----------------------------------------------------------------
// �N�}�̓����蔻��
//-----------------------------------------------------------------
void HitBear(void)
{
	// �\���̂̃|�C���^�ϐ�
	PLAYER *pPlayer = GetPlayer();

	// �ϐ��錾
	int nPlayer = GetPlayerCnt();

	// �N�}�Ƃ̓����蔻��
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->state != PLAYERSTATE_FLASHING)
		{
			if ((pPlayer->pos.y + pPlayer->fHeight) > (g_Bear.pos.y - g_Bear.fHeight) &&
				(pPlayer->pos.y - pPlayer->fHeight) < (g_Bear.pos.y + g_Bear.fHeight) &&
				(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (g_Bear.pos.x - g_Bear.fWidth) &&
				(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (g_Bear.pos.x + g_Bear.fWidth))
			{// �ϐ��錾
				float fAngle = atan2f(pPlayer->pos.x - g_Bear.pos.x, pPlayer->pos.y - g_Bear.pos.y);

				// �l���|�C���g��\������
				SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_DECREASE_ONE);

				// �X�R�A�̉��Z
				AddScore(nCntPlayer, -1);

				// �v���C���[�̓����蔻��
				HitPlayer(nCntPlayer, fAngle);
			}
		}
	}
}

//-----------------------------------------------------------------
// �N�}�̐ݒ�
//-----------------------------------------------------------------
void SetBear(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	// �\���̂̃|�C���^�ϐ�
	BEAR *pBear = &g_Bear;

	// �e�̐ݒ�
	if (pBear->bUse == false)
	{// �ʒu�ݒ�
		pBear->pos = pos;

		// �ړ��ʂ̐ݒ�
		pBear->move = move;

		// true�ŕԂ�
		pBear->bUse = true;
	}
}

//-----------------------------------------------------------------
// �G�̒��_���W
//-----------------------------------------------------------------
void SetVertexBear(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBear->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_Bear.pos.x - g_Bear.fWidth, g_Bear.pos.y + g_Bear.fHeight + g_fmove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Bear.pos.x - g_Bear.fWidth, g_Bear.pos.y - g_Bear.fHeight + g_fmove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Bear.pos.x + g_Bear.fWidth, g_Bear.pos.y + g_Bear.fHeight + g_fmove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Bear.pos.x + g_Bear.fWidth, g_Bear.pos.y - g_Bear.fHeight + g_fmove, 0.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_Bear.fTexV);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_Bear.fTexU, g_Bear.fTexV);
	pVtx[3].tex = D3DXVECTOR2(g_Bear.fTexU, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBear->Unlock();
}

//-----------------------------------------------------------------
// ��ʗh����̎擾
//-----------------------------------------------------------------
float GetShaking(void)
{
	return g_fmove;
}