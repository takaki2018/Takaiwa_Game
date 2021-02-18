//-----------------------------------------------------------------
//
// ���U���g (result.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result.h"
#include "enemy.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"
#include "score.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN			(11)									// �A�j���[�V�����p�^�[��No.�̍ő吔
#define MAX_TEXTURE			(4)										// ���U���g��ʂɎg���e�N�X�`���̐�
#define MAX_SCORE			(8)										// �\������X�R�A�̌�
#define TEXTURE_X			(10)									// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y			(1)										// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_nPatternAnim)   // �e�N�X�`���̓���
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)			// U�̒l
#define UV_V				(1.0f / TEXTURE_Y)						// V�̒l

#define RESULT_LOGO_X		(SCREEN_WIDTH / 2)						// ���U���g���S�̒��S���W(X)
#define RESULT_LOGO_Y		(150.0f)								// ���U���g���S�̒��S���W(Y)
#define RESULT_LOGO_WIDTH	(800.0f)								// ���U���g���S�̕�
#define RESULT_LOGO_HEIGHT	(100.0f)								// ���U���g���S�̍���

#define YOUR_SCORE_X		(SCREEN_WIDTH / 2)						// YOUR SCORE���S�̒��S���W(X)
#define YOUR_SCORE_Y		(300.0f)								// YOUR SCORE���S�̒��S���W(Y)
#define YOUR_SCORE_WIDTH	(300.0f)								// YOUR SCORE���S�̕�
#define YOUR_SCORE_HEIGHT	(40.0f)									// YOUR SCORE���S�̍���

#define SCORE_X				(395.0f)								// �X�R�A�̒��S���W(X)
#define SCORE_Y				(420.0f)								// �X�R�A�̒��S���W(Y)
#define SCORE_WIDTH			(80.0f)									// �X�R�A�̕�
#define SCORE_HEIGHT		(110.0f)								// �X�R�A�̍���
#define SCORE_SPACE			(70.0f)									// �X�R�A��z�u����Ԋu

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// ���U���g��ʂ̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_TEXTURE] = {};	// ���U���g��ʂ̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posLogo;									// �Q�[���N���A�[���S�̒��S���W
D3DXVECTOR3 g_posYourScore;								// ���S
D3DXVECTOR3 g_posScoreResult;							// �X�R�A
int g_nScoreResult;										// �X�R�A
bool g_bHitObject;

//-----------------------------------------------------------------
// ���U���g�̏���������
//-----------------------------------------------------------------
HRESULT InitResult(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	g_bHitObject = GetHitObject();
	g_nScoreResult = GetScore();

	// �ϐ��̏�����
	g_posLogo = D3DXVECTOR3(RESULT_LOGO_X, RESULT_LOGO_Y, 0.0f);
	g_posYourScore = D3DXVECTOR3(YOUR_SCORE_X, YOUR_SCORE_Y, 0.0f);
	g_posScoreResult = D3DXVECTOR3(SCORE_X, SCORE_Y, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureResult[0]);
	switch (g_bHitObject)
	{// �G���I�u�W�F�N�g�ɓ������Ă��Ȃ��Ƃ�
	case false:
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear_logo.png", &g_pTextureResult[1]);

		// �T�E���h
		PlaySound(SOUND_LABEL_RESULT_CLEAR);
		break;
	// �G���I�u�W�F�N�g�ɓ��������Ƃ�
	case true:
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover_logo.png", &g_pTextureResult[1]);

		// �T�E���h
		PlaySound(SOUND_LABEL_RESULT_CLEAR);
		break;
	}
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/your_score.png", &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureResult[3]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCntResult = 0; nCntResult < MAX_TEXTURE; nCntResult++)
	{
		// ���_���W�̐ݒ�
		SetVertexResult(nCntResult);
	}

	return S_OK;
}

//-----------------------------------------------------------------
// ���U���g�̏I������
//-----------------------------------------------------------------
void UninitResult(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureResult[nCntTexture] != NULL)
		{
			g_pTextureResult[nCntTexture]->Release();
			g_pTextureResult[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ���U���g�̍X�V����
//-----------------------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
	{// ��ʃ��[�h�̏���������
		SetFade(FADE_OUT,MODE_RANKING);
	}
}

//-----------------------------------------------------------------
// ���U���g�̕`�揈��
//-----------------------------------------------------------------
void DrawResult(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		if (nCntTexture < 3)
		{
			pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
		}
		else if (nCntTexture >= 3 && nCntTexture < 11)
		{
			pDevice->SetTexture(0, g_pTextureResult[3]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// ���U���g��ʂɓ\��e�N�X�`���̎w��
//-----------------------------------------------------------------
void SetVertexResult(int nCntResult)
{
	if (nCntResult == 0)
	{
		// �\���̂̃|�C���^�ϐ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// ���Ԗڂ�
		pVtx += nCntResult * 4;

		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffResult->Unlock();
	}
	else if (nCntResult == 1)
	{
		// �\���̂̃|�C���^�ϐ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// ���Ԗڂ�
		pVtx += nCntResult * 4;

		pVtx[0].pos = D3DXVECTOR3(g_posLogo.x - (RESULT_LOGO_WIDTH / 2), g_posLogo.y + (RESULT_LOGO_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posLogo.x - (RESULT_LOGO_WIDTH / 2), g_posLogo.y - (RESULT_LOGO_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posLogo.x + (RESULT_LOGO_WIDTH / 2), g_posLogo.y + (RESULT_LOGO_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posLogo.x + (RESULT_LOGO_WIDTH / 2), g_posLogo.y - (RESULT_LOGO_HEIGHT / 2), 0.0f);

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

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffResult->Unlock();
	}
	else if (nCntResult == 2)
	{
		// �\���̂̃|�C���^�ϐ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// ���Ԗڂ�
		pVtx += nCntResult * 4;

		pVtx[0].pos = D3DXVECTOR3(g_posYourScore.x - (YOUR_SCORE_WIDTH / 2), g_posYourScore.y + (YOUR_SCORE_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posYourScore.x - (YOUR_SCORE_WIDTH / 2), g_posYourScore.y - (YOUR_SCORE_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posYourScore.x + (YOUR_SCORE_WIDTH / 2), g_posYourScore.y + (YOUR_SCORE_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posYourScore.x + (YOUR_SCORE_WIDTH / 2), g_posYourScore.y - (YOUR_SCORE_HEIGHT / 2), 0.0f);

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

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffResult->Unlock();
	}
	else if (nCntResult == 3)
	{
		// �\���̂̃|�C���^�ϐ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// ���Ԗڂ�
		pVtx += nCntResult * 4;

		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			// �ϐ��錾
			int aNumber[8];

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posScoreResult.x - (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y + (SCORE_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posScoreResult.x - (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y - (SCORE_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posScoreResult.x + (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y + (SCORE_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posScoreResult.x + (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y - (SCORE_HEIGHT / 2), 0.0f);

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

			// �X�R�A���i�[
			aNumber[nCnt] = (g_nScoreResult / (int)powf(10.0f, (float)(MAX_SCORE - (nCnt + 1))));

			// �e�N�X�`���̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCnt] * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCnt] * 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCnt] * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCnt] * 0.1f, 0.0f);

			// �X�V
			pVtx += 4;
		}

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffResult->Unlock();
	}
}