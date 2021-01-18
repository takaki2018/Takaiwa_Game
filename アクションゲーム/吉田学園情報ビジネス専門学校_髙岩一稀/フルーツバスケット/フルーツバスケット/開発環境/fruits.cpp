//-------------------------------------------------------------------
//
// �t���[�c (fruits.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "fruits.h"
#include "player.h"
#include "block.h"
#include "bear.h"
#include "crow.h"
#include "score.h"
#include "point.h"
#include "gamepad.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_FRUITS				(256)					// �t���[�c�̍ő�l

#define FRUITS_SIZE_X_GAME		(20.0f)					// �t���[�c�̃T�C�Y(��)�Q�[��
#define FRUITS_SIZE_Y_GAME		(20.0f)					// �t���[�c�̃T�C�Y(����)�Q�[��

#define FRUITS_SIZE_X_TITLE		(80.0f)				// �t���[�c�̃T�C�Y(��)�^�C�g��
#define FRUITS_SIZE_Y_TITLE		(80.0f)				// �t���[�c�̃T�C�Y(����)�^�C�g��

#define ADD_GRAVITY				(0.3f)					// �d��
#define ADD_ROT					(0.1f)					// ��]��
#define BOUND					(1.70f)					// �o�E���h�l

#define SETFRUITS_INTERVAL		(60)					// �t���[�c��z�u����Ԋu
#define SETFRUITS_ONE			(0)						// ���
#define SETFRUITS_TWO			(30)					// ���

#define SETFEVERFRUITS_INTERVAL	(10)					// �t�B�[�o�[�^�C���̃t���[�c��z�u����Ԋu
#define SETFEVERFRUITS_ONE		(0)						// ���

#define MAX_GETFRUITS			(5)						// �l�������t���[�c�̗����̍ő吔

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFruits = NULL	;				// �t���[�c�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureFruits[FRUITSTYPE_MAX] = {};		// �t���[�c�̒��_�o�b�t�@�̃|�C���^
FRUITS g_aFruits[MAX_FRUITS];									// �t���[�c�̏��
float g_fLength;												// �Ίp��
float g_fAngle;													// �p�x
int g_nSetFruitsCnt;											// �t���[�c��z�u����J�E���g
int g_nSetFeverFruitsCnt;										// �t�B�[�o�[�^�C���̃t���[�c��z�u����J�E���g
int g_nSetGoldFruitsCnt;										// ���F�̃t���[�c��z�u����J�E���g
int g_nCntTitleFruits;											// �^�C�g����ʂŗ��Ƃ�����
bool g_bTitleFruits;											// �^�C�g����ʂŃt���[�c�𗎂Ƃ����ǂ���
																
//-----------------------------------------------------------------
// �t���[�c�̏���������
//-----------------------------------------------------------------
HRESULT InitFruits(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	MODE mode = GetMode();

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_FRUITS; nCnt++)
	{
		g_aFruits[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFruits[nCnt].posOld = g_aFruits[nCnt].pos;
		g_aFruits[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFruits[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFruits[nCnt].nCntBound = 0;

		switch (mode)
		{// �^�C�g���̂Ƃ�
		case MODE_TITLE:
			g_aFruits[nCnt].fWidth = FRUITS_SIZE_X_TITLE;
			g_aFruits[nCnt].fHeight = FRUITS_SIZE_Y_TITLE;
			break;

			// �Q�[���̂Ƃ�
		case MODE_GAME:
			g_aFruits[nCnt].fWidth = FRUITS_SIZE_X_GAME;
			g_aFruits[nCnt].fHeight = FRUITS_SIZE_Y_GAME;
			break;
		}
		
		g_aFruits[nCnt].nType = FRUITSTYPE_APPLE;
		g_aFruits[nCnt].bLand = false;
		g_aFruits[nCnt].bUse = false;
	}

	// �ϐ��̏�����
	g_fLength = sqrtf(g_aFruits[0].fWidth * g_aFruits[0].fWidth + g_aFruits[0].fHeight * g_aFruits[0].fHeight);
	g_fAngle = atan2f(g_aFruits[0].fWidth, g_aFruits[0].fHeight);
	g_nSetFruitsCnt = 0;
	g_nCntTitleFruits = 100;
	g_bTitleFruits = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/apple.png", &g_pTextureFruits[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/blue_apple.png", &g_pTextureFruits[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/peach.png", &g_pTextureFruits[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/orange.png", &g_pTextureFruits[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/apple_gold.png", &g_pTextureFruits[4]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_FRUITS, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffFruits,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFruits->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_FRUITS; nCnt++)
	{
		// ���_���W�̐ݒ�
		SetVertexFruits(nCnt);

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
	g_pVtxBuffFruits->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �t���[�c�̏I������
//-----------------------------------------------------------------
void UninitFruits(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffFruits != NULL)
	{
		g_pVtxBuffFruits->Release();
		g_pVtxBuffFruits = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < FRUITSTYPE_MAX; nCnt++)
	{
		if (g_pTextureFruits[nCnt] != NULL)
		{
			g_pTextureFruits[nCnt]->Release();
			g_pTextureFruits[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �t���[�c�̍X�V����
//-----------------------------------------------------------------
void UpdateFruits(void)
{
	// �ϐ��錾
	MODE mode = GetMode();

	switch (mode)
	{// �^�C�g����ʂ̂Ƃ�
	case MODE_TITLE:
		TitleFruits();
		break;

		// �Q�[����ʂ̂Ƃ�
	case MODE_GAME:
		GameFruits();
		break;
	}
}

//-----------------------------------------------------------------
// �t���[�c�̕`�揈��
//-----------------------------------------------------------------
void DrawFruits(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFruits, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �o�E���h�̕`��
	for (int nCnt = 0; nCnt < MAX_FRUITS; nCnt++)
	{
		if (g_aFruits[nCnt].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureFruits[g_aFruits[nCnt].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �^�C�g����ʂ̃t���[�c
//-----------------------------------------------------------------
void TitleFruits(void)
{
	// �\���̂̃|�C���^�ϐ�
	FRUITS *pFruits = &g_aFruits[0];

	// �t���[�c�̔z�u
	SetFruitsTitle();

	for (int nCntFruits = 0; nCntFruits < MAX_FRUITS; nCntFruits++, pFruits++)
	{
		if (pFruits->bUse == true)
		{
			// �ʒu�̕ۑ�
			pFruits->posOld = pFruits->pos;

			// �ړ��ʂ̍X�V
			pFruits->move.y += ADD_GRAVITY;

			// �ʒu�̍X�V
			pFruits->pos += pFruits->move;

			// ��ʊO�ɂ������Ƃ�false
			if (g_aFruits[nCntFruits].pos.y > SCREEN_HEIGHT + g_aFruits[nCntFruits].fHeight / 2.0f)
			{
				g_aFruits[nCntFruits].bUse = false;
			}

			// ���_���W�̍X�V
			SetVertexFruits(nCntFruits);
		}
	}
}

//-----------------------------------------------------------------
// �Q�[����ʂ̃t���[�c
//-----------------------------------------------------------------
void GameFruits(void)
{
	// �\���̂̃|�C���^�ϐ�
	FRUITS *pFruits = &g_aFruits[0];

	// �t���[�c�̔z�u
	SetFruitsGame();

	for (int nCntFruits = 0; nCntFruits < MAX_FRUITS; nCntFruits++, pFruits++)
	{
		if (pFruits->bUse == true)
		{
			// �ʒu�̕ۑ�
			pFruits->posOld = pFruits->pos;

			// �ړ��ʂ̍X�V
			pFruits->move.y += ADD_GRAVITY;

			// �ʒu�̍X�V
			pFruits->pos += pFruits->move;

			// ��]
			if (pFruits->posOld.x < pFruits->pos.x)
			{// �E�ɐi��ł�Ƃ��E��]����
				pFruits->rot.z -= ADD_ROT;
				if (pFruits->rot.z < D3DX_PI)
				{
					pFruits->rot.z -= D3DX_PI * 2.0f;
				}
			}
			else if (pFruits->posOld.x > pFruits->pos.x)
			{// ���ɐi��ł�Ƃ�����]����
				pFruits->rot.z += ADD_ROT;
				if (pFruits->rot.z > D3DX_PI)
				{
					pFruits->rot.z -= D3DX_PI * 2.0f;
				}
			}
			// �t���[�c�̓����蔻��
			HitFruits(nCntFruits);

			if (pFruits->pos.x + pFruits->fWidth <= 120.0f)
			{// ��ԍ��ɓ��B�����Ƃ�����
			 //pFruits->bUse = false;
				pFruits->move.x *= -1;
			}
			if (pFruits->pos.x - pFruits->fWidth >= SCREEN_WIDTH - 120.0f)
			{// ��ԉE�ɓ��B�����Ƃ�����
				pFruits->move.x *= -1;
			}
			// ���ɍs�����Ƃ�
			if (pFruits->pos.y - pFruits->fHeight >= 580.0f)
			{
				if (pFruits->pos.x + pFruits->fWidth < 180.0f)
				{// �����ɓ������Ƃ�
					pFruits->bUse = false;
				}
				else if (pFruits->pos.x - pFruits->fWidth > 1070.0f)
				{// �E���ɓ������Ƃ�
					pFruits->bUse = false;
				}
			}
			// �u���b�N�ɏ�������ǂ���
			pFruits->bLand = CollisionBlock(&pFruits->pos, &pFruits->posOld, &pFruits->move,
				pFruits->fWidth, pFruits->fHeight, COLLISIONINFO_FRUITS);

			if (pFruits->bLand == true)
			{// �u���b�N�̏�����Ƃ��o�E���h����
				pFruits->move.y -= pFruits->move.y * BOUND;
			}
			// ���_���W�̍X�V
			SetVertexFruits(nCntFruits);
		}
	}
}

//-----------------------------------------------------------------
// �^�C�g����ʂ̔z�u���
//-----------------------------------------------------------------
void SetFruitsTitle(void)
{
	// �ϐ��ɏ���n��
	float fShaikingMove = GetShaking();

	if (fShaikingMove != 0.0f)
	{// ��ʂ��h��Ă���Ƃ�
		g_bTitleFruits = true;
	}

	if (g_bTitleFruits == true && g_nCntTitleFruits >= 0)
	{// �t�B�[�o�[�^�C���̃t���[�c�̔z�u�J�E���g�𑝉�
		g_nCntTitleFruits--;

		// �t���[�c�̔z�u
		SetFruits(D3DXVECTOR3((float)(rand() % 430), -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));

		// �t���[�c�̔z�u
		SetFruits(D3DXVECTOR3((float)(rand() % 430 + 430), -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));

		// �t���[�c�̔z�u
		SetFruits(D3DXVECTOR3((float)(rand() % 430 + 860), -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));
	}
}

//-----------------------------------------------------------------
// �t���[�c�̔z�u���
//-----------------------------------------------------------------
void SetFruitsGame(void)
{
	// �ϐ��ɏ���n��
	CROW *pCrow = GetCrow();
	float fShaikingMove = GetShaking();

	// �ϐ��錾
	bool bSetGoldFruits = false;
	bool bSetFruits = false;

	if (fShaikingMove != 0.0f)
	{// ��ʂ��h��Ă���Ƃ�
		bSetFruits = true;
	}
	for (int nCntCrow = 0; nCntCrow < 4; nCntCrow++)
	{// �g�p���Ă���J���X��T��
		if (pCrow->bUse == true)
		{
			bSetGoldFruits = true;
			g_nSetGoldFruitsCnt++;
		}
	}
	// �t���[�c�̔z�u�J�E���g�𑝉�
	g_nSetFruitsCnt++;

	if (bSetFruits == true)
	{// �t�B�[�o�[�^�C���̃t���[�c�̔z�u�J�E���g�𑝉�
		g_nSetFeverFruitsCnt++;

		if (SETFEVERFRUITS_ONE == g_nSetFeverFruitsCnt % SETFEVERFRUITS_INTERVAL)
		{// �t���[�c�̔z�u
			SetFruits(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
				(FRUITSTYPE)(rand() % 4));

			// �t���[�c�̔z�u
			SetFruits(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 - 5), 0.0f, 0.0f),
				(FRUITSTYPE)(rand() % 4));
		}
	}
	if (bSetGoldFruits == true)
	{// ���F�̉ʕ��̔z�u
		if (0 == g_nSetGoldFruitsCnt % 800)
		{
			SetFruits(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
				(FRUITSTYPE)(FRUITSTYPE_GOLDAPPLE));
		}
	}

	if (SETFRUITS_ONE == g_nSetFruitsCnt % SETFRUITS_INTERVAL)
	{// �t���[�c�̔z�u
		SetFruits(D3DXVECTOR3((float)(rand() % 950 + 150), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));
	}
	else if (SETFRUITS_TWO == g_nSetFruitsCnt % SETFRUITS_INTERVAL)
	{// �t���[�c�̔z�u
		SetFruits(D3DXVECTOR3((float)(rand() % 950 + 150), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 - 5), 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));
	}
}

//-----------------------------------------------------------------
// �t���[�c�̓����蔻��
//-----------------------------------------------------------------
void HitFruits(int nCntFruits)
{
	// �\���̂̃|�C���^�ϐ�
	FRUITS *pFruits = GetFruits();
	PLAYER *pPlayer = GetPlayer();

	// �ϐ��錾
	int nPlayer = GetPlayerCnt();

	// ���Ԗڂ�
	pFruits += nCntFruits;

	// �t���[�c�̓����蔻��
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if ((pPlayer->pos.y + pPlayer->fHeight) >(pFruits->pos.y - pFruits->fHeight) &&
			(pPlayer->pos.y - pPlayer->fHeight) < (pFruits->pos.y + pFruits->fHeight) &&
			(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (pFruits->pos.x - pFruits->fWidth) &&
			(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (pFruits->pos.x + pFruits->fWidth))
		{
			if (pFruits->nType != FRUITSTYPE_GOLDAPPLE)
			{
				// �l���|�C���g��\������
				SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_PLUS_ONE);

				// �X�R�A�̉��Z
				AddScore(nCntPlayer, 1);

				// �T�E���h
				PlaySound(SOUND_LABEL_SE_GET);
			}
			else if (pFruits->nType == FRUITSTYPE_GOLDAPPLE)
			{
				// �l���|�C���g��\������
				SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_PLUS_FIVE);

				// �X�R�A�̉��Z
				AddScore(nCntPlayer, 5);

				// �T�E���h
				PlaySound(SOUND_LABEL_SE_GET_GOLD);
			}
			// �g�p��Ԃ�false�ɂ���
			pFruits->bUse = false;
		}
		
	}
}

//-----------------------------------------------------------------
// �t���[�c�̐ݒ�
//-----------------------------------------------------------------
void SetFruits(D3DXVECTOR3 pos, D3DXVECTOR3 move, FRUITSTYPE type)
{
	// �\���̂̃|�C���^�ϐ�
	FRUITS *pFruits = &g_aFruits[0];

	// �e�̐ݒ�
	for (int nCntFruits = 0; nCntFruits < MAX_FRUITS; nCntFruits++, pFruits++)
	{
		if (pFruits->bUse == false)
		{// �ʒu�ݒ�
			pFruits->pos = pos;

			// �ړ��ʂ̐ݒ�
			pFruits->move = move;

			// �^�C�v�̊m��
			pFruits->nType = type;

			// true�ŕԂ�
			pFruits->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �t���[�c�̒��_���W
//-----------------------------------------------------------------
void SetVertexFruits(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFruits->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̍X�V
	pVtx[0].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (D3DX_PI * 0.00f - g_fAngle)) * g_fLength;
	pVtx[0].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (D3DX_PI * 0.00f - g_fAngle)) * g_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 1.00f)) * g_fLength;
	pVtx[1].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 1.00f)) * g_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 0.00f)) * g_fLength;
	pVtx[2].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 0.00f)) * g_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (D3DX_PI * 1.00f - g_fAngle)) * g_fLength;
	pVtx[3].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (D3DX_PI * 1.00f - g_fAngle)) * g_fLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFruits->Unlock();
}

//-----------------------------------------------------------------
// �t���[�c���̎擾
//-----------------------------------------------------------------
FRUITS *GetFruits(void)
{
	return &g_aFruits[0];
}

//-----------------------------------------------------------------
// �^�C�g����ʂ̃t���[�c�J�E���g�̎擾
//-----------------------------------------------------------------
bool GetbTitleFruits(void)
{
	return g_bTitleFruits;
}

//-----------------------------------------------------------------
// �^�C�g����ʂ̃t���[�c�J�E���g�̎擾
//-----------------------------------------------------------------
int GetTitleFruitsCnt(void)
{
	return g_nCntTitleFruits;
}