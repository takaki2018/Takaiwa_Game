//-----------------------------------------------------------------
//
// �X�R�A (score.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "score.h"
#include "bear.h"
#include "selectmode.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PLAYER		(4)						// �v���C���[�̍ő吔
#define MAX_TEXTURE		(2)						// �e�N�X�`���̍ő吔
#define MAX_SCORE		(2)						// UI�ɕ\������X�R�A�̌���

#define SCORE_X_SINGLE	(410.0f)				// 1�Ԗڂ̃X�R�A�̒��S���W(X)�V���O�����[�h
#define SCORE_X_MULTI	(160.0f)				// 1�Ԗڂ̃X�R�A�̒��S���W(X)�}���`���[�h

#define SCORE_TWO_X		(410.0f)				// 2�Ԗڂ̃X�R�A�̒��S���W(X)
#define SCORE_TUIRD_X	(920.0f)				// 3�Ԗڂ̃X�R�A�̒��S���W(X)
#define SCORE_FOURTH_X	(1170.0f)				// 4�Ԗڂ̃X�R�A�̒��S���W(X)
#define SCORE_Y			(70.0f)					// �X�R�A�̒��S���W(Y)

#define SCORE_WIDTH		(40.0f)					// �X�R�A�̕�
#define SCOREHEIGHT		(75.0f)					// �X�R�A�̍���
#define WIDTH_X			(35.0f)					// �X�R�A��z�u����Ԋu

#define MAX_PATTERN     (10)					// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(10)					// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)						// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;				// �X�R�A�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureScore[MAX_TEXTURE] = {};		// �X�R�A�̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posScore[MAX_PLAYER] ;						// �X�R�A�̒��S���W
D3DXCOLOR g_colScore;										// �X�R�A�̐F
bool g_bHiddenScore;										// �X�R�A���B��邩�ǂ���
int g_nScore[MAX_PLAYER];									// �X�R�A��ۑ�����
int g_nDisplayScore;										// �\�����鐔��ۑ�

//-----------------------------------------------------------------
// �X�R�A�̏���������
//-----------------------------------------------------------------
HRESULT InitScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	PLAYMODE playmode = GetPlayMode();

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore[0]);

	// �ϐ��̏�����
	switch (playmode)
	{// �V���O�����[�h�̂Ƃ�����
	case PLAYMODE_SINGLE:
		g_nDisplayScore = 1;											// �\������X�R�A�̐�
		g_posScore[0] = D3DXVECTOR3(SCORE_X_SINGLE, SCORE_Y, 0.0f);		// �z�u������W(1P)
		break;
		// �}���`���[�h�̂Ƃ�����
	case PLAYMODE_MULTI:
		g_nDisplayScore = MAX_PLAYER;									// �\������X�R�A�̐�
		g_posScore[0] = D3DXVECTOR3(SCORE_X_MULTI, SCORE_Y, 0.0f);		// �z�u������W(1P)
		break;
	}
	g_posScore[1] = D3DXVECTOR3(SCORE_TWO_X, SCORE_Y, 0.0f);			// �z�u������W(2P)
	g_posScore[2] = D3DXVECTOR3(SCORE_TUIRD_X, SCORE_Y, 0.0f);			// �z�u������W(3P)
	g_posScore[3] = D3DXVECTOR3(SCORE_FOURTH_X, SCORE_Y, 0.0f);			// �z�u������W(4P)

	g_colScore = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						// �X�R�A�̐F
	g_bHiddenScore = false;												// �X�R�A�������邩�ǂ���

	for (int nCntScore = 0; nCntScore < MAX_PLAYER; nCntScore++)
	{// �X�R�A�̏�����
		g_nScore[nCntScore] = 0;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE * g_nDisplayScore, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x - (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y + (SCOREHEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x - (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y - (SCOREHEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x + (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y + (SCOREHEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x + (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y - (SCOREHEIGHT / 2), 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_colScore;
			pVtx[1].col = g_colScore;
			pVtx[2].col = g_colScore;
			pVtx[3].col = g_colScore;

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
			pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

			// �X�R�A�̏������ɂ���
			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �X�R�A�̏I������
//-----------------------------------------------------------------
void UninitScore(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureScore[nCntTexture] != NULL)
		{
			g_pTextureScore[nCntTexture]->Release();
			g_pTextureScore[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �X�R�A�̍X�V����
//-----------------------------------------------------------------
void UpdateScore(void)
{
	// �ϐ��錾
	PLAYMODE playmode = GetPlayMode();

	// ���[�h�ɂ���ď���
	switch (playmode)
	{// �}���`���[�h�̂Ƃ�����
	case PLAYMODE_MULTI:
		HiddenScore();
		break;
	}
}

//-----------------------------------------------------------------
// �X�R�A�̕`�揈��
//-----------------------------------------------------------------
void DrawScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntScore = 0; nCntScore < MAX_SCORE * g_nDisplayScore; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureScore[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//-----------------------------------------------------------------
// �X�R�A���B������
//-----------------------------------------------------------------
void HiddenScore(void)
{
	// �ϐ��錾
	float fShaking = GetShaking();

	if (fShaking != 0.0f)
	{// ��ʂ��h�ꂽ�Ƃ�
		g_bHiddenScore = true;
	}

	if (g_bHiddenScore == true)
	{// �X�R�A���B���Ƃ�
		g_colScore.a -= 0.003f;

		if (g_colScore <= 0.0f)
		{// ���S�ɏ������Ƃ�
			g_bHiddenScore = false;
		}
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// ���_�J���[�̍X�V
			pVtx[0].col = g_colScore;
			pVtx[1].col = g_colScore;
			pVtx[2].col = g_colScore;
			pVtx[3].col = g_colScore;

			// �X�R�A�̏������ɂ���
			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//-----------------------------------------------------------------
// �X�R�A�̉��Z����
//-----------------------------------------------------------------
void AddScore(int nCntPlayer,int nValue)
{
	// �ϐ��錾
	int aNumber[MAX_SCORE];			// �X�R�A���i�[
	g_nScore[nCntPlayer] += nValue;

	// 0��菬�����Ȃ����Ƃ�0�ɂ���
	if (g_nScore[nCntPlayer] < 0)
	{
		g_nScore[nCntPlayer] = 0;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ̃X�R�A��
	pVtx += nCntPlayer * 4 * MAX_SCORE;
	
	// �X�R�A���i�[
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{// ��������i�[
		aNumber[nCntScore] = (g_nScore[nCntPlayer] / (int)powf(10.0f, (float)(MAX_SCORE - (nCntScore + 1))));

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 0.0f);

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//-----------------------------------------------------------------
// �X�R�A�̎擾
//-----------------------------------------------------------------
int GetScore(int nCntScore)
{
	return g_nScore[nCntScore];
}