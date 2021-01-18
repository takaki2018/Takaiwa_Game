//-----------------------------------------------------------------
//
// ���U���g�X�R�A (score.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_score.h"
#include "selectmode.h"
#include "bear.h"
#include "gauge.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)						// �v���C���[�̍ő吔
#define MAX_TEXTURE			(2)						// �e�N�X�`���̍ő吔
#define MAX_SCORE			(2)						// UI�ɕ\������X�R�A�̌���
#define RESULTSCORE_X		(400.0f)				// 1�Ԗڂ̃X�R�A�̒��S���W(X)
#define RESULTSCORE_Y		(500.0f)				// 1�Ԗڂ̃X�R�A�̒��S���W(Y)

#define RESULTSCORE_WIDTH	(30.0f)					// �X�R�A�̕�
#define RESULTSCORE_HEIGHT	(60.0f)					// �X�R�A�̍���
#define WIDTH_X				(25.0f)					// �X�R�A��z�u����Ԋu
#define PlAYER_WIDTH		(180.0f)				// �v���C���[��z�u����Ԋu

#define MAX_PATTERN			(10)					// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X			(10)					// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y			(1)						// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;	// �X�R�A�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;		// �X�R�A�̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posResultScore[MAX_PLAYER];				// �X�R�A�̒��S���W
int g_nResultScore[MAX_PLAYER];							// �X�R�A��ۑ�����
int g_nDisplayResultScore;								// �\�����鐔��ۑ�

//-----------------------------------------------------------------
// ���U���g�X�R�A�̏���������
//-----------------------------------------------------------------
HRESULT InitResultScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// �V���O�����[�h�̎�����
	case PLAYMODE_SINGLE:
		g_nDisplayResultScore = 1;
		break;
	// �}���`���[�h�̎�����
	case PLAYMODE_MULTI:
		g_nDisplayResultScore = MAX_PLAYER;
		break;
	}	

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureResultScore);

	// �ϐ��̏�����
	for (int nCntScore = 0; nCntScore < MAX_PLAYER; nCntScore++)
	{
		g_posResultScore[nCntScore] = D3DXVECTOR3(RESULTSCORE_X + PlAYER_WIDTH * nCntScore, RESULTSCORE_Y, 0.0f);
		g_nResultScore[nCntScore] = 0;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE * MAX_PLAYER, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nDisplayScore = 0; nDisplayScore < MAX_PLAYER; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2), 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
	g_pVtxBuffResultScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ���U���g�X�R�A�̏I������
//-----------------------------------------------------------------
void UninitResultScore(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

}

//-----------------------------------------------------------------
// ���U���g�X�R�A�̍X�V����
//-----------------------------------------------------------------
void UpdateResultScore(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayResultScore; nDisplayScore++)
	{
		// �ϐ��錾
		float fmove = GetMoveGauge(nDisplayScore);

		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);

			// ���̏��
			pVtx += 4;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();

}

//-----------------------------------------------------------------
// ���U���g�X�R�A�̕`�揈��
//-----------------------------------------------------------------
void DrawResultScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntResultScore = 0; nCntResultScore < MAX_SCORE * g_nDisplayResultScore; nCntResultScore++)
	{
		pDevice->SetTexture(0, g_pTextureResultScore);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultScore * 4, 2);
	}
}

//-----------------------------------------------------------------
// ���U���g�X�R�A�̉��Z����
//-----------------------------------------------------------------
void AddResultScore(int nCntPlayer, int nValue)
{
	// �ϐ��錾
	int aNumber[MAX_SCORE];			// �X�R�A���i�[
	g_nResultScore[nCntPlayer] += nValue;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ̃X�R�A��
	pVtx += nCntPlayer * 4 * MAX_SCORE;

	// �X�R�A���i�[
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{// ��������i�[
		aNumber[nCntScore] = (g_nResultScore[nCntPlayer] / (int)powf(10.0f, (float)(MAX_SCORE - (nCntScore + 1))));

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 0.0f);

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}