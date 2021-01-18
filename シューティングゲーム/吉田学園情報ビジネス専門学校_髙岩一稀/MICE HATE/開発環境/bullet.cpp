//-----------------------------------------------------------------
//
// �e (bullet.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bullet.h"
#include "input.h"
#include "gamepad.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "gauge.h"
#include "setparticle.h"
#include "score.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define BULLET_SIZE		(5)			// �e�̑傫��

#define MAX_BULLET		(256)		// �e�̍ő�l

#define MOVE_BULLET		(8.0f)		// �e�̈ړ���
#define BULLET_SPASE	(10.0f)		// �e�̊Ԋu

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// �e�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			// �e�̒��_�o�b�t�@�̃|�C���^
BULLET g_aBullet[MAX_BULLET];						// �e�̏��
int g_nColorCnt;

//-----------------------------------------------------------------
// �e�̏���������
//-----------------------------------------------------------------
HRESULT InitBullet(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_nColorCnt = BULLETCOLOR_WHITE;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);

	// �e���̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aBullet[nCntBullet].nColType = NULL;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].bUse = false;	
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_BULLET, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ���_���W�̐ݒ�
		SetVertexBullet(nCntBullet);

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

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		
		// �e�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �e�̏I������
//-----------------------------------------------------------------
void UninitBullet(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//-----------------------------------------------------------------
// �e�̍X�V����
//-----------------------------------------------------------------
void UpdateBullet(void)
{
	// �ϐ��錾
	BULLET *pBullet;	// �e���̍\���̂̃|�C���^
	ENEMY *pEnemy;		// �G���̍\���̂̃|�C���^
	PLAYER *pPlayer;	// �v���C���[���̍\���̂̃|�C���^
	bool bRemove;		// �Q�[�W�̌����ʂ̏��

	// �ϐ��̎󂯓n��
	pBullet = &g_aBullet[0];
	pEnemy = GetEnemy();
	pPlayer = GetPlayer();
	bRemove = GetRemove();

	// �e�̐F�̐؂�ւ�
	if (GetKeyboardTrigger(KEYINFO_COLOR_CHANGE) == true || GetJoypadTrigger(JPINFO_COLOR_CHANGE) == true)
	{
		g_nColorCnt = (g_nColorCnt + 1) % BULLETCOLOR_MAX;
	}

	// �e�̔���
	if (pPlayer->state != PLAYERSTATE_HIT)
	{// ���@�̏�Ԃ��G�ɍU�����󂯂Ă��Ȃ��Ƃ�
		if (bRemove == false)
		{// �Q�[�W�����S�Ɍ������Ă��Ȃ��Ƃ�
			if (GetKeyboardTrigger(KEYINFO_SHOOT) == true || GetJoypadTrigger(JPINFO_SHOOT) == true)
			{// �e�̐ݒ�
			 // ���ʉ�
				PlaySound(SOUND_LABEL_SE_SHOOT);

				// �ꔭ��
				SetBullet(D3DXVECTOR3(pPlayer->pos.x + (BULLET_SPASE * sinf(D3DX_PI / 2 - pPlayer->rot.z)), pPlayer->pos.y - (BULLET_SPASE * cosf(D3DX_PI / 2 - pPlayer->rot.z)), 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI - pPlayer->rot.z) * -MOVE_BULLET, cosf(D3DX_PI - pPlayer->rot.z) * MOVE_BULLET, 0.0f),
					g_nColorCnt,
					60,
					BULLETTYPE_PLAYER);

				// �񔭖�
				SetBullet(D3DXVECTOR3(pPlayer->pos.x - (BULLET_SPASE * sinf(D3DX_PI / 2 - pPlayer->rot.z)), pPlayer->pos.y + (BULLET_SPASE * cosf(D3DX_PI / 2 - pPlayer->rot.z)), 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI - pPlayer->rot.z) * -MOVE_BULLET, cosf(D3DX_PI - pPlayer->rot.z) * MOVE_BULLET, 0.0f),
					g_nColorCnt,
					60,
					BULLETTYPE_PLAYER);
			}
		}
	}
	// �e�̍X�V
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{// �ʒu�X�V
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			// �e�̋O��
			if (pBullet->type == BULLETTYPE_PLAYER)
			{// ���@�̒e�̎�
				SetBulletOrbit(pBullet->pos, pBullet->col, SETPARTICLE_PLAYER_BULLET);
			}
			else if (pBullet->type == BULLETTYPE_ENEMY)
			{// �G�̒e�̎�
				SetBulletOrbit(pBullet->pos, pBullet->col, SETPARTICLE_ENEMY_BULLET);
			}

			// �G�Ƃ̓����蔻��
			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
			{
				// �g�p���Ă邩����
				if (pEnemy->bUse == true)
				{
					if (pBullet->type == BULLETTYPE_PLAYER)
					{
						if (pEnemy->pos.y - ENEMY_Y <= pBullet->pos.y &&
							pEnemy->pos.y + ENEMY_Y >= pBullet->pos.y &&
							pEnemy->pos.x - ENEMY_X <= pBullet->pos.x &&
							pEnemy->pos.x + ENEMY_X >= pBullet->pos.x)
						{// �G�ɓ��������Ƃ�
							HitEnemy(nCntEnemy, nCntBullet);

							// �e�𖢎g�p�̏�Ԃɂ���
							pBullet->bUse = false;
						}
					}
					else if (pBullet->type == BULLETTYPE_ENEMY)
					{// �G�̒e�̂Ƃ�
						if (pPlayer->state == PLAYERSTATE_NORMAL)
						{// ���@�ɓ��������Ƃ�
							if (pPlayer->pos.y - ENEMY_Y <= pBullet->pos.y &&
								pPlayer->pos.y + ENEMY_Y >= pBullet->pos.y &&
								pPlayer->pos.x - ENEMY_X <= pBullet->pos.x &&
								pPlayer->pos.x + ENEMY_X >= pBullet->pos.x)
							{// ���@�ɓ��������Ƃ�
								HitPlayer(nCntBullet);

								// �e�𖢎g�p�̏�Ԃɂ���
								pBullet->bUse = false;
							}
						}
						if (pEnemy->nType != ENEMYTYPE_BLUE)
						{// �ȊO�̂Ƃ�
							if (pEnemy->pos.y - ENEMY_Y <= pBullet->pos.y &&
								pEnemy->pos.y + ENEMY_Y >= pBullet->pos.y &&
								pEnemy->pos.x - ENEMY_X <= pBullet->pos.x &&
								pEnemy->pos.x + ENEMY_X >= pBullet->pos.x)
							{// �G�ɓ��������Ƃ�
								HitEnemy(nCntEnemy, nCntBullet);

								// �e�𖢎g�p�̏�Ԃɂ���
								pBullet->bUse = false;
							}
						}
					}
				}
			}
			// �G���̃��Z�b�g
			pEnemy = GetEnemy();

			// ���_���W�̐ݒ�
			SetVertexBullet(nCntBullet);

			// ��ʊO�`�F�b�N
			if (pBullet->pos.y <= 0)
			{// �e�̏�Ԃ�false�ɂ���
				pBullet->bUse = false;
			}
			if (pBullet->pos.y >= SCREEN_HEIGHT)
			{// �e�̏�Ԃ�false�ɂ���
				pBullet->bUse = false;
			}
			if (pBullet->pos.x <= 0)
			{// �e�̏�Ԃ�false�ɂ���
				pBullet->bUse = false;
			}
			if (pBullet->pos.x >= SCREEN_WIDTH)
			{// �e�̏�Ԃ�false�ɂ���
				pBullet->bUse = false;
			}
		}
	}
}

//-----------------------------------------------------------------
// �e�̕`�揈��
//-----------------------------------------------------------------
void DrawBullet(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�̕`��
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{// �e���g���Ă���Ƃ�����
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �e�̐ݒ�
//-----------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nBulletColor,int nLife,BULLETTYPE type)
{
	// �\���̂̃|�C���^�ϐ�
	BULLET *pBullet;

	// �ϐ��̎󂯓n��
	pBullet = &g_aBullet[0];

	// �e�̐ݒ�
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{// �ʒu�ݒ�
			pBullet->pos = pos;

			// �e�̃^�C�v
			pBullet->nColType = nBulletColor;

			// �F�̐ݒ�
			if (pBullet->nColType == BULLETCOLOR_WHITE)
			{// ���ɐݒ�
				pBullet->col = D3DXCOLOR(0.9f, 0.9f, 0.9f,0.9f);
			}
			else if (pBullet->nColType == BULLETCOLOR_RED)
			{// �Ԃɐݒ�
				pBullet->col = D3DXCOLOR(0.9f,0.3f,0.1f,0.9f);
			}
			else if (pBullet->nColType == BULLETCOLOR_BLUE)
			{// �ɐݒ�
				pBullet->col = D3DXCOLOR(0.1f, 0.3f,0.9f, 0.9f);
			}
			// ���_���W�̐ݒ�
			SetVertexBullet(nCntBullet);

			// �ړ��ʂ�ݒ�
			pBullet->move = move;

			// �v���C���[�ƓG�ǂ��炪�g���Ă��邩
			pBullet->type = type;

			// true�ŕԂ�
			pBullet->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �e�̒��_���W
//-----------------------------------------------------------------
void SetVertexBullet(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0.0f);

	// �F�̐ݒ�
	pVtx[0].col = g_aBullet[nIdx].col;
	pVtx[1].col = g_aBullet[nIdx].col;
	pVtx[2].col = g_aBullet[nIdx].col;
	pVtx[3].col = g_aBullet[nIdx].col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------------------------------------------
// �e�̏��̎擾
//-----------------------------------------------------------------
BULLET *GetBullet(void)
{
	return &g_aBullet[0];
}

//-----------------------------------------------------------------
// ���݂̒e�̐F
//-----------------------------------------------------------------
int GetBulletcol(void)
{
	return g_nColorCnt;
}