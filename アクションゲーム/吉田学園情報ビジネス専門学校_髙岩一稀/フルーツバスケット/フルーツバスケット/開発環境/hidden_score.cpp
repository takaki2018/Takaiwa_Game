//-----------------------------------------------------------------
//
// �͂Ăȃ}�[�N (hidden_score.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "hidden_score.h"
#include "bear.h"
#include "selectmode.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)					// �v���C���[�̍ő吔
#define MAX_TEXTURE			(1)					// �e�N�X�`���̍ő吔
#define MAX_SCORE			(2)					// UI�ɕ\������͂Ăȃ}�[�N�̌���

#define QUESTION_X			(160.0f)			// 1�Ԗڂ̂͂Ăȃ}�[�N�̒��S���W(X)
#define QUESTION_TWO_X		(410.0f)			// 2�Ԗڂ̂͂Ăȃ}�[�N�̒��S���W(X)
#define QUESTION_TUIRD_X	(920.0f)			// 3�Ԗڂ̂͂Ăȃ}�[�N�̒��S���W(X)
#define QUESTION_FOURTH_X	(1170.0f)			// 4�Ԗڂ̂͂Ăȃ}�[�N�̒��S���W(X)
#define QUESTION_Y			(70.0f)				// �͂Ăȃ}�[�N�̒��S���W(Y)

#define QUESTION_WIDTH		(40.0f)				// �͂Ăȃ}�[�N�̕�
#define QUESTION_HEIGHT		(75.0f)				// �͂Ăȃ}�[�N�̍���
#define WIDTH_X				(35.0f)				// �͂Ăȃ}�[�N��z�u����Ԋu

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHiddenScore = NULL;	// �X�R�A�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureHiddenScore = NULL;		// �X�R�A�̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posHiddenScore[MAX_PLAYER];				// �X�R�A�̒��S���W
D3DXCOLOR g_colHiddenScore;								// �͂Ăȃ}�[�N�̐F
bool g_bAppearScore;									// �X�R�A������邩�ǂ���
int g_nDisplayHiddenScore;								// �\�����鐔��ۑ�

//-----------------------------------------------------------------
// �͂Ăȃ}�[�N�̏���������
//-----------------------------------------------------------------
HRESULT InitHiddenScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	g_nDisplayHiddenScore = MAX_PLAYER;

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_question.png", &g_pTextureHiddenScore);

	// �ϐ��̏�����
	g_posHiddenScore[0] = D3DXVECTOR3(QUESTION_X, QUESTION_Y, 0.0f);
	g_posHiddenScore[1] = D3DXVECTOR3(QUESTION_TWO_X, QUESTION_Y, 0.0f);
	g_posHiddenScore[2] = D3DXVECTOR3(QUESTION_TUIRD_X, QUESTION_Y, 0.0f);
	g_posHiddenScore[3] = D3DXVECTOR3(QUESTION_FOURTH_X, QUESTION_Y, 0.0f);

	g_colHiddenScore = D3DXCOLOR(1.0f, 1.0f, 1.0f,0.0f);

	g_bAppearScore = false;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE * g_nDisplayHiddenScore, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffHiddenScore,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHiddenScore->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayHiddenScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x - (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y + (QUESTION_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x - (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y - (QUESTION_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x + (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y + (QUESTION_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x + (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y - (QUESTION_HEIGHT / 2), 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_colHiddenScore;
			pVtx[1].col = g_colHiddenScore;
			pVtx[2].col = g_colHiddenScore;
			pVtx[3].col = g_colHiddenScore;

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

			// �X�R�A�̏������ɂ���
			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHiddenScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �͂Ăȃ}�[�N�̏I������
//-----------------------------------------------------------------
void UninitHiddenScore(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffHiddenScore != NULL)
	{
		g_pVtxBuffHiddenScore->Release();
		g_pVtxBuffHiddenScore = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureHiddenScore != NULL)
	{
		g_pTextureHiddenScore->Release();
		g_pTextureHiddenScore = NULL;
	}
}

//-----------------------------------------------------------------
// �͂Ăȃ}�[�N�̍X�V����
//-----------------------------------------------------------------
void UpdateHiddenScore(void)
{
	// �ϐ��錾
	PLAYMODE playmode = GetPlayMode();

	// ���[�h�ɂ���ď���
	switch (playmode)
	{// �}���`���[�h�̂Ƃ�����
	case PLAYMODE_MULTI:
		DisplayQuestionMark();
		break;
	}
}

//-----------------------------------------------------------------
// �͂Ăȃ}�[�N�̕`�揈��
//-----------------------------------------------------------------
void DrawHiddenScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHiddenScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntHiddenScore = 0; nCntHiddenScore < MAX_SCORE * g_nDisplayHiddenScore; nCntHiddenScore++)
	{
		pDevice->SetTexture(0, g_pTextureHiddenScore);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHiddenScore * 4, 2);
	}
}

//-----------------------------------------------------------------
// �͂Ăȃ}�[�N��\�����鏈��
//-----------------------------------------------------------------
void DisplayQuestionMark(void)
{
	// �ϐ��錾
	float fShaking = GetShaking();

	if (fShaking != 0.0f)
	{// ��ʂ��h�ꂽ�Ƃ�
		g_bAppearScore = true;
	}

	// �X�R�A������킠���Ƃ�
	if (g_bAppearScore == true)
	{
		g_colHiddenScore.a += 0.003f;

		if (g_colHiddenScore <= 0.0f)
		{// ���S�Ɍ��ꂽ�Ƃ�
			g_bAppearScore = false;
		}
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHiddenScore->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayHiddenScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// ���_�J���[�̍X�V
			pVtx[0].col = g_colHiddenScore;
			pVtx[1].col = g_colHiddenScore;
			pVtx[2].col = g_colHiddenScore;
			pVtx[3].col = g_colHiddenScore;

			// �X�R�A�̏������ɂ���
			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHiddenScore->Unlock();
}