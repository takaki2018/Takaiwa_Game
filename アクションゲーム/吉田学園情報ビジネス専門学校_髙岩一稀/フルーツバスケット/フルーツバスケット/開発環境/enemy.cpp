//-------------------------------------------------------------------
//
// �G (enemy.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "enemy.h"
#include "player.h"
#include "block.h"
#include "score.h"
#include "point.h"
#include "gamepad.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_ENEMY			(256)					// �t���[�c�̍ő�l
#define MAX_ENEMY_TYPE		(2)						// �t���[�c�̎��

#define ENEMY_SIZE_X		(30.0f)					// �t���[�c�̃T�C�Y(��)
#define ENEMY_SIZE_Y		(30.0f)					// �t���[�c�̃T�C�Y(����)

#define ADD_GRAVITY			(0.1f)					// �d��
#define ADD_ROT				(0.1f)					// ��]��
#define BOUND				(1.80f)					// �o�E���h�l

#define SETENEMY_INTERVAL	(200)					// �G���o��������Ԋu

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// �G�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEnemy[MAX_ENEMY_TYPE] = {};	// �G�̒��_�o�b�t�@�̃|�C���^
ENEMY g_aEnemy[MAX_ENEMY];									// �G�̏��
float g_fEnemyLength;										// �G�̑Ίp��
float g_fEnemyAngle;										// �G�̊p�x
int g_SetCntEnemy;											// �G�̏o���J�E���g

//-----------------------------------------------------------------
// �G�̏���������
//-----------------------------------------------------------------
HRESULT InitEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].posOld = g_aEnemy[nCnt].pos;
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].nCntBound = 0;
		g_aEnemy[nCnt].fWidth = ENEMY_SIZE_X;
		g_aEnemy[nCnt].fHeight = ENEMY_SIZE_Y;
		g_aEnemy[nCnt].nType = 0;
		g_aEnemy[nCnt].bUse = false;
	}

	// �ϐ��̏�����
	g_fEnemyLength = sqrtf(ENEMY_SIZE_X * ENEMY_SIZE_X + ENEMY_SIZE_Y * ENEMY_SIZE_Y);
	g_fEnemyAngle = atan2f(ENEMY_SIZE_X, ENEMY_SIZE_Y);
	g_SetCntEnemy = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/caterpillar001.png", &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/caterpillar002.png", &g_pTextureEnemy[1]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_ENEMY, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// ���_���W�̐ݒ�
		SetVertexEnemy(nCnt);

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

		// ���̏��ɍX�V
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �G�̏I������
//-----------------------------------------------------------------
void UninitEnemy(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_ENEMY_TYPE; nCnt++)
	{
		if (g_pTextureEnemy[nCnt] != NULL)
		{
			g_pTextureEnemy[nCnt]->Release();
			g_pTextureEnemy[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �G�̍X�V����
//-----------------------------------------------------------------
void UpdateEnemy(void)
{
	// �\���̂̃|�C���^�ϐ�
	ENEMY *pEnemy = &g_aEnemy[0];
	BLOCK *pBlock = GetBlock();

	g_SetCntEnemy++;
	if (0 == g_SetCntEnemy % SETENEMY_INTERVAL)
	{// ���̔z�u
		SetEnemy(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
			rand() % MAX_ENEMY_TYPE);
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// �ʒu�̕ۑ�
			pEnemy->posOld = pEnemy->pos;

			// �ړ��ʂ̍X�V
			pEnemy->move.y += ADD_GRAVITY;

			// �ʒu�̍X�V
			pEnemy->pos += pEnemy->move;

			// �u���b�N�Ƃ̓����蔻��
			bool bLand = CollisionBlock(&pEnemy->pos, &pEnemy->posOld, &pEnemy->move, 
				pEnemy->fWidth * 0.9f, pEnemy->fHeight * 0.9f,COLLISIONINFO_ENEMY);

			// �u���b�N�̏�ɏ�����Ƃ�
			if (bLand == true)
			{
				pEnemy->move.y -= pEnemy->move.y * BOUND;
			}

			// ��]
			if (pEnemy->posOld.x < pEnemy->pos.x)
			{// ����]
				pEnemy->rot.z -= ADD_ROT;
				if (pEnemy->rot.z < D3DX_PI)
				{
					pEnemy->rot.z -= D3DX_PI * 2.0f;
				}
			}
			else if (pEnemy->posOld.x > pEnemy->pos.x)
			{// �E��]
				pEnemy->rot.z += ADD_ROT;
				if (pEnemy->rot.z > D3DX_PI)
				{
					pEnemy->rot.z -= D3DX_PI * 2.0f;
				}
			}
			// �G�̓����蔻��
			HitEnemy(nCntEnemy);

			// ��ʊO�ɏo�Ȃ��悤�ɂ���
			if (pEnemy->pos.y + pEnemy->fHeight > SCREEN_HEIGHT)
			{// ��ԉ��ɓ��B�����Ƃ�����
				pEnemy->pos.y = SCREEN_HEIGHT - pEnemy->fHeight;
				pEnemy->move.y -= pEnemy->move.y * BOUND;
			}
			if (pEnemy->pos.x + pEnemy->fWidth <= 130.0f)
			{// ��ԍ��ɓ��B�����Ƃ�����
				pEnemy->move.x *= -1;
			}
			if (pEnemy->pos.x - pEnemy->fWidth >= SCREEN_WIDTH - 130.0f)
			{// ��ԉE�ɓ��B�����Ƃ�����
				pEnemy->move.x *= -1;
			}
			// ���ɍs�����Ƃ�
			if (pEnemy->pos.y - pEnemy->fHeight >= 580.0f)
			{
				if (pEnemy->pos.x + pEnemy->fWidth < 180.0f)
				{// �����ɓ������Ƃ�
					pEnemy->bUse = false;
				}
				else if (pEnemy->pos.x - pEnemy->fWidth > 1070.0f)
				{// �E���ɓ������Ƃ�
					pEnemy->bUse = false;
				}
			}

			// ���_���W�̍X�V
			SetVertexEnemy(nCntEnemy);
		}
	}
}

//-----------------------------------------------------------------
// �G�̕`�揈��
//-----------------------------------------------------------------
void DrawEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �o�E���h�̕`��
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCnt].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �G�̓����蔻��
//-----------------------------------------------------------------
void HitEnemy(int nCntEnemy)
{
	// �\���̂̃|�C���^�ϐ�
	ENEMY *pEnemy = GetEnemy();
	PLAYER *pPlayer = GetPlayer();

	// �ϐ��錾
	int nPlayer = GetnCntController();

	// ���Ԗڂ�
	pEnemy += nCntEnemy;

	// �G�Ƃ̓����蔻��
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if ((pPlayer->pos.y + pPlayer->fHeight) >(pEnemy->pos.y - pEnemy->fHeight * 0.7f) &&
			(pPlayer->pos.y - pPlayer->fHeight) < (pEnemy->pos.y + pEnemy->fHeight * 0.7f) &&
			(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (pEnemy->pos.x - pEnemy->fWidth * 0.7f) &&
			(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (pEnemy->pos.x + pEnemy->fWidth * 0.7f))
		{// �l���|�C���g��\������
			SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_DECREASE_FIVE);

			// �X�R�A�̉��Z
			AddScore(nCntPlayer, -5);

			// �T�E���h
			PlaySound(SOUND_LABEL_SE_ENEMY);

			// �g�p��Ԃ�false�ɂ���
			pEnemy->bUse = false;
		}
		
	}
}

//-----------------------------------------------------------------
// �G�̐ݒ�
//-----------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int type)
{
	// �\���̂̃|�C���^�ϐ�
	ENEMY *pEnemy = &g_aEnemy[0];

	// �e�̐ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{// �ʒu�ݒ�
			pEnemy->pos = pos;

			// �ړ��ʂ̐ݒ�
			pEnemy->move = move;

			// �^�C�v�̊m��
			pEnemy->nType = type;

			// true�ŕԂ�
			pEnemy->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �G�̒��_���W
//-----------------------------------------------------------------
void SetVertexEnemy(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̍X�V
	pVtx[0].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 0.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[0].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 0.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 1.00f)) * g_fEnemyLength;
	pVtx[1].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 1.00f)) * g_fEnemyLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 0.00f)) * g_fEnemyLength;
	pVtx[2].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 0.00f)) * g_fEnemyLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 1.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[3].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 1.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------
// �G���̎擾
//-----------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}