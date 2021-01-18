//-----------------------------------------------------------------
//
// �G (enemy.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "object.h"
#include "shockwave.h"
#include "item.h"
#include "setparticle.h"
#include "score.h"
#include "score_enemy.h"
#include "sound.h"
#include "selectmode.h"
#include "timer.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MOVE_ENEMY			(0.5f)				// �G�̈ړ���
#define SET_ENEMY			(g_nSetCounter)			// �G�̔z�u����Ԋu

#define MAX_TYPE_ENEMY		(3)					// �G�̎��
#define BULLET_COUNT		(200)				// �e�����J�E���g

#define PARTICLE_ANGLE		(10)				// �p�[�e�B�N���̔�U�p�x
#define PARTICLE_MOVE		(0.1f)				// �p�[�e�B�N���̈ړ���

#define ENEMY_SCORE_WHITE	(50)				// �G(��)�̃X�R�A
#define ENEMY_SCORE_RED		(100)				// �G(��)�̃X�R�A
#define ENEMY_SCORE_BLUE	(200)				// �G(��)�̃X�R�A

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// �G�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEnemy[MAX_TYPE_ENEMY] = {};	// �G�̃e�N�X�`���̃|�C���^
ENEMY g_aEnemy[MAX_ENEMY];									// �G�̏��
D3DXVECTOR3 g_posEnemy;										// �G�̒��_���W
int g_nCnt = 0;												// �J�E���g
D3DXCOLOR g_EnemyCol;										// �G�̐F
D3DXVECTOR3 g_posobject;									// �I�u�W�F�N�g�̍��W
bool g_HitObject;											// �I�u�W�F�N�g�ɓ����������ǂ���
bool g_bItemEffect;											// �A�C�e�����g���Ă��邩
int g_nCntEnemy;											// ���Ԗڂ��ۑ�
int g_nCntColor;											// �_�ł̂Ƃ��Ɏg�p
float g_NB;													// �m�b�N�o�b�N�l
int g_nCombo;												// �R���{���Z�l
int g_nSetCounter;											// �G��z�u����J�E���g
int g_nInCounter;											// �G���T�[�N���̒��ɓ����Ă�����

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
	g_EnemyCol = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	g_HitObject = false;
	g_nCnt = 0;
	g_nCntEnemy = NULL;
	g_nCntColor = 0;
	g_NB = 0.0f;
	g_nCombo = 10;
	g_nSetCounter = 105;
	g_nInCounter = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat_white.png", &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat_red.png", &g_pTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat_blue.png", &g_pTextureEnemy[2]);

	// �G�̏��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = ENEMYTYPE_WHITE;
		g_aEnemy[nCntEnemy].col = g_EnemyCol;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].fAngle = 0.0f;
		g_aEnemy[nCntEnemy].nCounterState = NULL;
		g_aEnemy[nCntEnemy].nLife = 1;
		g_aEnemy[nCntEnemy].nCntFlash = 0;
		g_aEnemy[nCntEnemy].nFlashCounter = 0;
		g_aEnemy[nCntEnemy].bUse = false;
	}

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

	// ���_���W�̐ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ���_���W�̐ݒ�
		SetVertexEnemy(nCntEnemy);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_EnemyCol;
		pVtx[1].col = g_EnemyCol;
		pVtx[2].col = g_EnemyCol;
		pVtx[3].col = g_EnemyCol;

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// �e�̏������ɂ���
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
	for (int nCntTexture = 0; nCntTexture < MAX_TYPE_ENEMY; nCntTexture++)
	{
		if (g_pTextureEnemy[nCntTexture] != NULL)
		{
			g_pTextureEnemy[nCntTexture]->Release();
			g_pTextureEnemy[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �G�̍X�V����
//-----------------------------------------------------------------
void UpdateEnemy(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	PLAYER *pPlayer;

	// �ϐ��̎󂯓n��
	pEnemy = &g_aEnemy[0];
	pPlayer = GetPlayer();
	g_posobject = GetposObject();
	g_bItemEffect = GetbUseItem();
	g_NB = ShockwaveMove();

	// ���@�̏��
	if (pPlayer->state == PLAYERSTATE_HIT)
	{// �U�����󂯂��Ƃ��R���{�̏�����
		g_nCombo = 10;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// �G�̍X�V
	if (g_HitObject == false)
	{// �I�u�W�F�N�g�ɓ������Ă��Ȃ��Ƃ�
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{// �G���g�p���Ă���Ƃ�
				// �ϐ��錾
				// �G�ƃI�u�W�F�N�g�̋���
				float fLength = sqrtf((pEnemy->pos.x - g_posobject.x) * (pEnemy->pos.x - g_posobject.x)
					+ (pEnemy->pos.y - g_posobject.y) * (pEnemy->pos.y - g_posobject.y));

				if (g_bItemEffect == true && pEnemy->state == ENEMYSTATE_NORMAL )
				{// �A�C�e�����g���Ă���Ƃ�
					pEnemy->state = ENEMYSTATE_ITEMEFFECT;
					pEnemy->nCounterState = 60;
				}
				// �^�C�v���ƂɍX�V
				switch (pEnemy->nType)
				{
				case ENEMYTYPE_WHITE:			// ��
					EnemyInfoWhite(nCntEnemy);
					break;

				case ENEMYTYPE_RED:				// ��
					EnemyInfoRed(nCntEnemy);
					break;

				case ENEMYTYPE_BLUE:			// ��
					EnemyInfoBlue(nCntEnemy);
					break;
				}
				// �G��Ԃ̔���
				EnemyState(nCntEnemy);

				// �G�ƃI�u�W�F�N�g�����蔻��
				if (pEnemy->pos.y - 10.0f <= g_posobject.y + 10.0f &&
					pEnemy->pos.y + 10.0f >= g_posobject.y - 10.0f &&
					pEnemy->pos.x - 10.0f <= g_posobject.x + 10.0f &&
					pEnemy->pos.x + 10.0f >= g_posobject.x - 10.0f)
				{// ���������Ƃ�true�ɕϊ�	
					g_HitObject = true;

					// ���������G�����Ԗڂ��ۑ�
					g_nCntEnemy = nCntEnemy;
					break;
				}
				// ���a250.0�ɗ����Ƃ��x�����o��
				if (fLength <= 250.0f && pEnemy->nCntFlash != 0)
				{// �x���p�Ռ��j���o����
					if (pEnemy->nFlashCounter == 0)
					{
						// �x���p�̏Ռ��g��z�u
						SetShockwave(g_posobject, D3DXVECTOR3(280.0f, 280.0f, 0.0f), D3DXCOLOR(0.8f, 0.01f, 0.01f, 0.5f),
							2.0f, 0.0f, 1.0f, 0.01f);

						// �J�E���g�����炷
						pEnemy->nCntFlash--;
						
						// �J�E���g��߂�
						pEnemy->nFlashCounter = 30;

						// �{�[�i�X�̑���
						g_nInCounter++;

						// �T�E���h
						PlaySound(SOUND_LABEL_SE_ALARM);
					}
					// �J�E���g�����炷
					pEnemy->nFlashCounter--;
				}
				else if (fLength > 250.0f && pEnemy->nCntFlash == 0)
				{// �J�E���g��߂�
					pEnemy->nCntFlash = 2;
				}
			}
		}
		// �G�������_���ɔz�u
		SetEnemyRandam();
	}
	else if (g_HitObject == true)
	{
		// �J�E���^�[�A�b�v
		g_nCntColor++;
		pEnemy += g_nCntEnemy;

		// �_��
		if (g_nCntColor == 10)
		{
			pEnemy->col.a = 0.0f;
		}
		else if (g_nCntColor == 20)
		{
			pEnemy->col.a = 1.0f;
			g_nCntColor = 0;
		}
		// ���_���W�̐ݒ�
		SetVertexEnemy(g_nCntEnemy);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
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

	// �G�̕`��
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �G(��)�̏��
//-----------------------------------------------------------------
void EnemyInfoWhite(int nCntEnemy)
{
	// �ړ��ʂ̍X�V
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x - g_aEnemy[nCntEnemy].move.x * g_NB;
	g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y - g_aEnemy[nCntEnemy].move.y * g_NB;

	// ���_���W�̐ݒ�
	SetVertexEnemy(nCntEnemy);

	// �G�̋O��
	SetMoveOrbit(g_aEnemy[nCntEnemy].pos,
		D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			cosf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			0.0f),
		D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.5f),
		SETPARTICLE_MOVE_ENEMY);
}

//-----------------------------------------------------------------
// �G(��)�̏��
//-----------------------------------------------------------------
void EnemyInfoRed(int nCntEnemy)
{
	// �ړ��ʂ̍X�V
	g_aEnemy[nCntEnemy].pos.x += (g_aEnemy[nCntEnemy].move.x - g_aEnemy[nCntEnemy].move.x * g_NB) * 2.0f;
	g_aEnemy[nCntEnemy].pos.y += (g_aEnemy[nCntEnemy].move.y - g_aEnemy[nCntEnemy].move.y * g_NB) * 2.0f;

	// ���_���W�̐ݒ�
	SetVertexEnemy(nCntEnemy);

	// �G�̋O��
	SetMoveOrbit(g_aEnemy[nCntEnemy].pos,
		D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			cosf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			0.0f),
		D3DXCOLOR(0.9f, 0.3f, 0.1f, 0.5f),
		SETPARTICLE_MOVE_ENEMY);
}

//-----------------------------------------------------------------
// �G(��)�̏��
//-----------------------------------------------------------------
void EnemyInfoBlue(int nCntEnemy)
{
	// �\���̂̃|�C���^�ϐ�
	PLAYER *pPlayer;

	// �ϐ��錾
	float fAngleEnemy;	// �G�ƃv���C���[�܂ł̊p�x
	int nBulletColor = BULLETCOLOR_RED;

	// �ϐ��̎󂯓n��
	pPlayer = GetPlayer();

	// �ړ��ʂ̍X�V
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x - g_aEnemy[nCntEnemy].move.x * g_NB;
	g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y - g_aEnemy[nCntEnemy].move.y * g_NB;

	// ���_���W�̐ݒ�
	SetVertexEnemy(nCntEnemy);

	// �G�̋O��
	SetMoveOrbit(g_aEnemy[nCntEnemy].pos,
		D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			cosf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			0.0f),
		D3DXCOLOR(0.1f, 0.3f, 0.9f, 0.5f),
		SETPARTICLE_MOVE_ENEMY);

	// �J�E���g�̌���
	g_aEnemy[nCntEnemy].nCntBullet--;

	// �e�̔���
	if (g_aEnemy[nCntEnemy].nCntBullet == 0)
	{// �p�x�̊m��
		fAngleEnemy = atan2f(g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x, g_aEnemy[nCntEnemy].pos.y - pPlayer->pos.y);

		// �e�̐ݒ�
		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * 5.0f, cosf(D3DX_PI - fAngleEnemy) * 5.0f, 0.0f),
			nBulletColor, 100, BULLETTYPE_ENEMY);

		// �J�E���g�̏�����
		g_aEnemy[nCntEnemy].nCntBullet = BULLET_COUNT;
	}
}

//-----------------------------------------------------------------
// �G�̏��
//-----------------------------------------------------------------
void EnemyState(int nCntEnemy)
{
	// �G�̏��
	switch (g_aEnemy[nCntEnemy].state)
	{// �_���[�W���󂯂��Ƃ�
	case ENEMYSTATE_ANYHIT:
		// ��ԃJ�E���^�̌��Z
		g_aEnemy[nCntEnemy].nCounterState--;

		if (g_aEnemy[nCntEnemy].nCounterState <= 0)
		{// ��ԃJ�E���^��0�ȉ��ɂȂ����Ƃ���Ԃ�ʏ�ɕύX
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

			// �F�����ɖ߂�
			g_aEnemy[nCntEnemy].col = g_EnemyCol;
		}
		break;
		// �A�C�e���̌��ʂ��󂯂Ă���Ƃ�
	case ENEMYSTATE_ITEMEFFECT:
		// ��ԃJ�E���^�̌��Z
		g_aEnemy[nCntEnemy].nCounterState--;

		if (g_aEnemy[nCntEnemy].nCounterState <= 0)
		{// ��ԃJ�E���^��0�ȉ��ɂȂ����Ƃ���Ԃ�ʏ�ɕύX
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

			// �A�C�e�����g���Ă��Ȃ���Ԃɂ���
			SetItem(false);
		}
		break;
	}
}

//-----------------------------------------------------------------
// �G�������_���ɔz�u
//-----------------------------------------------------------------
void SetEnemyRandam(void)
{
	// �ϐ��錾
	D3DXVECTOR3 posObject;
	float fAngleEnemy;	// �G�ƃI�u�W�F�N�g�܂ł̊p�x

	// �ϐ��̏�����
	posObject = GetposObject();

	// �G���h���X���[�h�̂Ƃ��G��z�u����Ԋu�����炷
	if (GetSelectMode() == SELECTMODE_ENDLESS)
	{
		if (GetCntSetEnemy() < g_nSetCounter)
		{// �z�u����Ԋu�����ꂽ�Ƃ��J�E���g��������
			g_nCnt = 0;
		}
		// �G�̏o��Ԋu
		g_nSetCounter = GetCntSetEnemy();
	}

	// �J�E���^�[�A�b�v
	g_nCnt++;				// �G�̈ړ��ʎw��

	// �G�̔z�u
	if (g_nCnt == SET_ENEMY)
	{
		g_posEnemy = D3DXVECTOR3((float)(rand() % SCREEN_WIDTH), (float)(-ENEMY_Y), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y );
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY,0.0f) ,fAngleEnemy, ENEMYTYPE_WHITE);
	}
	else if (g_nCnt == SET_ENEMY * 2)
	{
		g_posEnemy = D3DXVECTOR3((float)(ENEMY_X + SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y);
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY, 0.0f), fAngleEnemy, (ENEMYTYPE)(rand()%3));
	}
	else if (g_nCnt == SET_ENEMY * 3)
	{
		g_posEnemy = D3DXVECTOR3((float)(rand() % SCREEN_WIDTH), (float)(ENEMY_Y + SCREEN_HEIGHT), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y);
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY, 0.0f), fAngleEnemy, ENEMYTYPE_WHITE);
	}
	else if (g_nCnt == SET_ENEMY * 4)
	{
		g_posEnemy = D3DXVECTOR3((float)-ENEMY_X, (float)(rand() % SCREEN_HEIGHT), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y);
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY, 0.0f), fAngleEnemy, (ENEMYTYPE)(rand()%3));
		g_nCnt = 0;
	}
}

//-----------------------------------------------------------------
// �G�̐ݒ�
//-----------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move,float angle ,ENEMYTYPE nType)
{
	// �\���̂̃|�C���^�ϐ�
	ENEMY *pEnemy;

	// �ϐ��̎󂯓n��
	pEnemy = &g_aEnemy[0];

	// �G�̐ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			// �ʒu�ݒ�
			pEnemy->pos = pos;

			// �ړ���
			pEnemy->move = move;

			// ���_���W�̐ݒ�
			SetVertexEnemy(nCntEnemy);

			// �I�u�W�F�N�g�܂ł̊p�x
			pEnemy->fAngle = angle;

			// �^�C�v�̐ݒ�
			pEnemy->nType = nType;

			// �e�����J�E���g�̐ݒ�
			if (nType == ENEMYTYPE_BLUE)
			{
				pEnemy->nCntBullet = BULLET_COUNT;
			}

			// �x���̉�
			pEnemy->nCntFlash = 2;

			// �x���̃C���^�[�o��
			pEnemy->nFlashCounter = 0;

			// true�ŕԂ�
			pEnemy->bUse = true;

			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
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

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_X, g_aEnemy[nIdx].pos.y + ENEMY_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_X, g_aEnemy[nIdx].pos.y - ENEMY_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_X, g_aEnemy[nIdx].pos.y + ENEMY_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_X, g_aEnemy[nIdx].pos.y - ENEMY_Y, 0.0f);

	// �F�̍X�V
	pVtx[0].col = g_aEnemy[nIdx].col;
	pVtx[1].col = g_aEnemy[nIdx].col;
	pVtx[2].col = g_aEnemy[nIdx].col;
	pVtx[3].col = g_aEnemy[nIdx].col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------
// �G�ɓ���������
//-----------------------------------------------------------------
bool HitEnemy(int nIdx, int nCntBullet)
{
	// �\���̂̃|�C���^�ϐ�
	ENEMY *pEnemy;
	BULLET *pBullet;

	// �ϐ��̎󂯓n��
	pEnemy = &g_aEnemy[0];
	pBullet = GetBullet();

	// ���Ԗڂ̏��
	pEnemy += nIdx;
	pBullet += nCntBullet;

	// �^�C�v�̎���
	if (pEnemy->nType == pBullet->nColType && pBullet->type == BULLETTYPE_PLAYER)
	{// �G�̐F�̔���
		if (pEnemy->nType == ENEMYTYPE_WHITE)
		{// ��
			// �X�R�A
			AddScore(ENEMY_SCORE_WHITE * g_nCombo);
		}
		else if (pEnemy->nType == ENEMYTYPE_RED)
		{// ��
			// �X�R�A
			AddScore(ENEMY_SCORE_RED * g_nCombo);
		}
		else if (pEnemy->nType == ENEMYTYPE_BLUE)
		{// ��
			// �X�R�A
			AddScore(ENEMY_SCORE_BLUE * g_nCombo);
		}
		// �G�𖢎g�p�ɂ���
		pEnemy->bUse = false;

		// �����p�[�e�B�N��
		SetBom(pEnemy->pos,SETPARTICLE_HIT_ENEMY);

		// ���ʉ�(����)
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		// �R���{���Z
		g_nCombo += 1;

		// true�ŕԂ�
		return true;
	}
	else
	{// �G�ɈႤ�e�����������Ƃ���Ԃ�ύX�@
		pEnemy->state = ENEMYSTATE_ANYHIT;

		// ��ԃJ�E���^��5�ɐݒ�
		pEnemy->nCounterState = 5;

		// �F�̕ύX
		pEnemy->col = D3DCOLOR_RGBA(100, 100, 100, 255);

		// �����p�[�e�B�N��
		SetBom(pEnemy->pos, SETPARTICLE_ANYHIT_ENEMY);

		if (pBullet->type == BULLETTYPE_PLAYER)
		{// �R���{�l�̏�����
			g_nCombo = 10;
		}
		// false�ŕԂ�
		return false;
	}
}

//-----------------------------------------------------------------
// �G���̎擾
//-----------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̓����蔻��̎擾
//-----------------------------------------------------------------
bool GetHitObject(void)
{
	return g_HitObject;
}

//-----------------------------------------------------------------
// �͈͓��ɗ�����
//-----------------------------------------------------------------
int GetEnemyIn(void)
{
	return g_nInCounter;
}