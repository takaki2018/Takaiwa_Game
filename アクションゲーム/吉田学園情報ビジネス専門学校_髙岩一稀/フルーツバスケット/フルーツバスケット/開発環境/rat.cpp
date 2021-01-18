//-----------------------------------------------------------------
//
// �J���X (crow.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "rat.h"
#include "player.h"
#include "score.h"
#include "point.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_RAT				(16)										// �J���X�̍ő吔
#define TEXTURE_X			(4)											// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y			(2)											// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_Rat.nPatternAnim)	// �e�N�X�`���̓���
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)				// U�̒l
#define UV_V				(1.0f / TEXTURE_Y)							// V�̒l

#define RAT_WIDTH			(50.0f)										// �l�Y�~�̕�
#define RAT_HEIGHT			(30.0f)										// �l�Y�~�̍���

#define ANIMATION_CNT		(10)										// �A�j���[�V�������X�V����J�E���g
#define MOVE_RAT			(5.0f)										// �l�Y�~�̈ړ���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRat = NULL;	// �l�Y�~�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRat = NULL;		// �l�Y�~�̃e�N�X�`���̃|�C���^
RAT g_Rat;										// �l�Y�~�̏��
int g_nRandSetRatCnt;							// �����_���Ŕz�u����J�E���g
int g_nSetRatCnt;								// �l�Y�~�̐ݒu�J�E���g

//-----------------------------------------------------------------
// �l�Y�~�̏���������
//-----------------------------------------------------------------
HRESULT InitRat(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_nRandSetRatCnt = rand() % 300 + 1000;
	g_nSetRatCnt = 0;

	// ���̏���������
	g_Rat.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Rat.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Rat.fWidth = RAT_WIDTH / 2.0f;
	g_Rat.fHeight = RAT_HEIGHT / 2.0f;
	g_Rat.nCntAnim = 0;
	g_Rat.nPatternAnim = 0;
	g_Rat.bUse = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat.png", &g_pTextureRat);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_RAT,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffRat,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRat->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetVertexRat(0);

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

	// ���̍X�V
	pVtx += 4;
	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRat->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �l�Y�~�̏I������
//-----------------------------------------------------------------
void UninitRat(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRat != NULL)
	{
		g_pVtxBuffRat->Release();
		g_pVtxBuffRat = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureRat != NULL)
	{
		g_pTextureRat->Release();
		g_pTextureRat = NULL;
	}
}

//-----------------------------------------------------------------
// �l�Y�~�̍X�V����
//-----------------------------------------------------------------
void UpdateRat(void)
{
	// �\���̂̃|�C���^�ϐ�
	RAT *pRat = &g_Rat;

	// �ݒu�J�E���g�̃J�E���g
	g_nSetRatCnt++;

	// ����؂��Ƃ��ݒu
	if (g_nRandSetRatCnt == g_nSetRatCnt)
	{// �J���X�̐ݒu
		SetRat(D3DXVECTOR3(SCREEN_WIDTH, 680.0f, 0.0f),
			D3DXVECTOR3(-MOVE_RAT, 0.0f, 0.0f));

		// �ϐ��̏�����
		g_nRandSetRatCnt = rand() % 300 + 1000;
		g_nSetRatCnt = 0;
	}
	// �l�Y�~���g�p���Ă邩
	if (pRat->bUse == true)
	{
		// �ʒu�̍X�V
		pRat->pos += pRat->move;

		// �l�Y�~�̓����蔻��
		HitRat();

		// ���_���W�̍X�V
		SetVertexRat(0);

		// ��ʊO�ɍs�����Ƃ�
		if (pRat->pos.x < 0.0f)
		{
			pRat->bUse = false;
		}
	}
}

//-----------------------------------------------------------------
// �l�Y�~�̕`�揈��
//-----------------------------------------------------------------
void DrawRat(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRat, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	if (g_Rat.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureRat);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------
// �l�Y�~�̓����蔻��
//-----------------------------------------------------------------
void HitRat(void)
{
	// �\���̂̃|�C���^�ϐ�
	PLAYER *pPlayer = GetPlayer();

	// �ϐ��錾
	int nPlayer = GetnCntController();

	// �N�}�Ƃ̓����蔻��
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->state != PLAYERSTATE_FLASHING)
		{
			if ((pPlayer->pos.y + pPlayer->fHeight) >(g_Rat.pos.y - g_Rat.fHeight) &&
				(pPlayer->pos.y - pPlayer->fHeight) < (g_Rat.pos.y + g_Rat.fHeight) &&
				(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (g_Rat.pos.x - g_Rat.fWidth) &&
				(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (g_Rat.pos.x + g_Rat.fWidth))
			{// �ϐ��錾
				float fAngle = atan2f(pPlayer->pos.x - g_Rat.pos.x, pPlayer->pos.y - g_Rat.pos.y);

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
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexRat(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRat->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Rat.pos.x - g_Rat.fWidth, g_Rat.pos.y + g_Rat.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Rat.pos.x - g_Rat.fWidth, g_Rat.pos.y - g_Rat.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Rat.pos.x + g_Rat.fWidth, g_Rat.pos.y + g_Rat.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Rat.pos.x + g_Rat.fWidth, g_Rat.pos.y - g_Rat.fHeight, 0.0f);

	// �A�j���[�V�����J�E���^�[�̃J�E���g
	g_Rat.nCntAnim++;

	if (0 == g_Rat.nCntAnim % ANIMATION_CNT)
	{// ����؂��Ƃ�����
		g_Rat.nPatternAnim = (g_Rat.nPatternAnim + 1) % TEXTURE_X;
	}
	// �A�j���[�V�����̍X�V
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
	pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRat->Unlock();
}

//-----------------------------------------------------------------
// �l�Y�~�̐ݒu
//-----------------------------------------------------------------
void SetRat(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	// �\���̂̃|�C���^�ϐ�
	RAT *pRat = &g_Rat;

	// �l�Y�~�̔z�u
	if (pRat->bUse == false)
	{
		// �ʒu�̐ݒ�
		pRat->pos = pos;

		// �ړ��ʂ̐ݒ�
		pRat->move = move;

		// �g�p��Ԃ�true�ɂ���
		pRat->bUse = true;
	}
}