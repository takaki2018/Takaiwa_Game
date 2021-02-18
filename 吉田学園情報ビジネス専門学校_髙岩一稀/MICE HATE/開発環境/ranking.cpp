//-------------------------------------------------------------------
//
// �����L���O (ranking.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "ranking.h"
#include "score.h"
#include "selectmode.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN				(12)				// �w�i�̎��
#define MAX_TEXTURE				(47)				// �\��e�N�X�`���̍ő吔
#define TEXTURE_X				(10)				// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y				(1)					// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔
#define MAX_RANKING_RANK		(5)					// ���ʂ̐�

#define RANKING_LOGO_X			(SCREEN_WIDTH / 2)	// �����L���O���S�̒��_���W(X)
#define RANKING_LOGO_Y			(90.0f)				// �����L���O���S�̒��_���W(Y)
#define RANKING_LOGO_WIDTH		(600.0f)			// �����L���O���S�̕�
#define RANKING_LOGO_HEIGHT		(70.0f)				// �����L���O���S�̍���

#define RANKING_RANK_X			(350.0f)			// �����L���O�����N�̒��_���W(X)
#define RANKING_RANK_Y			(220.0f)			// �����L���O�����N�̒��_���W(Y)
#define RANKING_RANK_WIDTH		(140.0f)			// �����L���O�����N�̕�
#define RANKING_RANK_HEIGHT		(70.0f)				// �����L���O�����N�̍���
#define RANKING_RANK_SPACE		(100.0f)			// �����L���O�����N��z�u����Ԋu

#define RANKING_SCORE_X			(600.0f)			// �����L���O�X�R�A�̒��_���W(X)
#define RANKING_SCORE_Y			(220.0f)			// �����L���O�X�R�A�̒��_���W(Y)
#define RANKING_SCORE_WIDTH		(60.0f)				// �����L���O�X�R�A�̕�
#define RANKING_SCORE_HEIGHT	(90.0f)				// �����L���O�X�R�A�̍���
#define RANKING_SCORE_SPACE		(60.0f)				// �����L���O�X�R�A��z�u����Ԋu

#define FLASH_SCORE_INTERVAL	(20)				// �_�ł̃C���^�[�o��

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;							// �����L���O�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRanking[MAX_PATTERN] = {};						// �����L���O�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posRankingLogo;												// �����L���O���S�̒��_���W
D3DXVECTOR3 g_posRankingRank;												// �����L���O�����N�̒��_���W
D3DXVECTOR3 g_posRankingScore;												// �����L���O�X�R�A�̒��_���W
D3DXCOLOR g_ScoreCol[MAX_RANKING_RANK];										// �X�R�A�̐F
int g_aScoreNormal[MAX_RANKING_RANK] = { 50000,40000,30000,20000,10000 };	// �����L���O�̏����X�R�A
int g_aScoreEndless[MAX_RANKING_RANK] = { 50000,40000,30000,20000,10000 };	// �����L���O�̏����X�R�A
int g_nCntScore;															// �\������X�R�A�̐�
int g_nSubScore;															// �X�R�A�̕ۑ���
bool g_bFlash;																// �_�ł����邩
int g_nFlashScore;															// �_�ł�����X�R�A
int g_nFlashScoreTimer;														// �_�Ń^�C�}�[
int g_SelectMode2;															// �ǂ̃��[�h��I��������

//-----------------------------------------------------------------
// �����L���O�̏���������
//-----------------------------------------------------------------
HRESULT InitRanking(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	g_SelectMode2 = GetSelectMode();

	// �ϐ��̏�����
	g_posRankingLogo = D3DXVECTOR3(RANKING_LOGO_X, RANKING_LOGO_Y, 0.0f);
	g_posRankingRank = D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y, 0.0f);
	g_posRankingScore = D3DXVECTOR3(RANKING_SCORE_X, RANKING_SCORE_Y, 0.0f);
	g_nCntScore = 0;
	g_nSubScore = 0;
	g_bFlash = false;
	g_nFlashScore = 0;
	g_nFlashScoreTimer = MAX_RANKING_RANK;

	// �X�R�A�̐F�̏�����
	for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
	{
		g_ScoreCol[nCnt] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_logo.png", &g_pTextureRanking[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureRanking[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureRanking[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureRanking[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureRanking[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_5th.png", &g_pTextureRanking[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureRanking[7]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		// ���_���W�̐ݒ�
		SetVertexRanking(nCntRanking);

		// ���̏��Ɉڍs
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

	// �ϐ��錾
	int nScore = GetScore();
	int nSubScore = 0;

	switch (g_SelectMode2)
	{
	case SELECTMODE_NORMAL:
		if (g_aScoreNormal[4] <= nScore)
		{//	5�ʂ̃X�R�A���傫���Ƃ�
			g_aScoreNormal[4] = nScore;
		}

		// �����L���O�̍X�V
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK - 1; nCnt++)
		{
			for (int nCnt2 = nCnt + 1; nCnt2 < MAX_RANKING_RANK; nCnt2++)
			{
				if (g_aScoreNormal[nCnt] <= g_aScoreNormal[nCnt2])
				{// �����L���O�X�R�A�X�V�����Ƃ�
					nSubScore = g_aScoreNormal[nCnt];
					g_aScoreNormal[nCnt] = g_aScoreNormal[nCnt2];
					g_aScoreNormal[nCnt2] = nSubScore;
				}
			}
		}
		// �ǂ��ɓ�������
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
		{
			if (g_aScoreNormal[nCnt] == nScore)
			{
				g_bFlash = true;
				g_nFlashScore = nCnt;
				break;
			}
		}
		break;
		// �G���h���X���[�h�̂Ƃ�
	case SELECTMODE_ENDLESS:
		if (g_aScoreEndless[4] <= nScore)
		{
			g_aScoreEndless[4] = nScore;
		}

		// �����L���O�̍X�V
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK - 1; nCnt++)
		{
			for (int nCnt2 = nCnt + 1; nCnt2 < MAX_RANKING_RANK; nCnt2++)
			{
				if (g_aScoreEndless[nCnt] <= g_aScoreEndless[nCnt2])
				{// �����L���O�X�R�A�X�V�����Ƃ�
					nSubScore = g_aScoreEndless[nCnt];
					g_aScoreEndless[nCnt] = g_aScoreEndless[nCnt2];
					g_aScoreEndless[nCnt2] = nSubScore;
				}
			}
		}
		// �ǂ��ɓ�������
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
		{
			if (g_aScoreEndless[nCnt] == nScore)
			{
				g_bFlash = true;
				g_nFlashScore = nCnt;
				break;
			}
		}
	}
	// �T�E���h
	PlaySound(SOUND_LABEL_RANKING);

	return S_OK;
}

//-----------------------------------------------------------------
// �����L���O�̏I������
//-----------------------------------------------------------------
void UninitRanking(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRanking[nCntRanking] != NULL)
		{
			g_pTextureRanking[nCntRanking]->Release();
			g_pTextureRanking[nCntRanking] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �����L���O�̍X�V����
//-----------------------------------------------------------------
void UpdateRanking(void)
{
	// �ϐ��̏�����
	g_nCntScore = 0;

	// ��ʑJ��
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	// �X�R�A�̃e�N�X�`���X�V
	for (int nCntScore = 7; nCntScore < MAX_PATTERN; nCntScore++)
	{// ���_���W�̍X�V
		SetVertexRanking(nCntScore);
	}
}

//-----------------------------------------------------------------
// �����L���O�̕`�揈��
//-----------------------------------------------------------------
void DrawRanking(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_TEXTURE; nCntRanking++)
	{
		// �|���S���̕`��
		if (nCntRanking < 7)
		{
			pDevice->SetTexture(0, g_pTextureRanking[nCntRanking]);
		}
		else if (nCntRanking >= 7)
		{
			pDevice->SetTexture(0, g_pTextureRanking[7]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}

//-----------------------------------------------------------------
// �����L���O�̒��_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexRanking(int nCnt)
{
	// ���_���W�̐ݒ�
	if (nCnt == 0)
	{// �����L���O�̔w�i�̐ݒu
		RankingBg(nCnt);
	}
	else if (nCnt == 1)
	{// �����L���O���S�̔z�u
		RankingLogo(nCnt);
	}
	else if (nCnt >= 2 && nCnt < 7)
	{// �����L���O���ʂ̔z�u
		RankingRank(nCnt);
	}
	else if (nCnt >= 7 && nCnt < MAX_PATTERN)
	{// �_��
		if (g_bFlash == true)
		{
			if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL / 2)
			{// �F�̊m��
				g_ScoreCol[g_nFlashScore] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			}
			else if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL)
			{// �F�̊m��
				g_ScoreCol[g_nFlashScore] = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);

				// �^�C�}�[�̏�����
				g_nFlashScoreTimer = 0;
			}
			g_nFlashScoreTimer++;
		}
		// �X�R�A�̍X�V
		RankingScore(nCnt);
	}
}

//-----------------------------------------------------------------
// �����L���O�̔w�i
//-----------------------------------------------------------------
void RankingBg(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//-----------------------------------------------------------------
// �����L���O�̃��S
//-----------------------------------------------------------------
void RankingLogo(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	pVtx[0].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//-----------------------------------------------------------------
// �����L���O�̏���
//-----------------------------------------------------------------
void RankingRank(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	// ���ʂ�z�u����Ԋu
	int nCntWidth = (nCnt - 2) % MAX_RANKING_RANK;

	pVtx[0].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//-----------------------------------------------------------------
// �����L���O�̃X�R�A
//-----------------------------------------------------------------
void RankingScore(int nCnt)
{
	// �ϐ��錾
	int nCntWidth = 0;
	int nNumber;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += (nCnt - g_nCntScore + MAX_SCORE * g_nCntScore) * 4;

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{// �X�R�A��z�u����Ԋu
		int nWidth = nCntScore % MAX_SCORE;			// ��

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_ScoreCol[g_nCntScore];
		pVtx[1].col = g_ScoreCol[g_nCntScore];
		pVtx[2].col = g_ScoreCol[g_nCntScore];
		pVtx[3].col = g_ScoreCol[g_nCntScore];

		// �X�R�A���i�[
		if (g_SelectMode2 == SELECTMODE_NORMAL)
		{
			nNumber = (g_aScoreNormal[g_nCntScore] / (int)powf(10.0f, (float)(MAX_SCORE - (nWidth + 1))));
		}
		else if (g_SelectMode2 == SELECTMODE_ENDLESS)
		{
			nNumber = (g_aScoreEndless[g_nCntScore] / (int)powf(10.0f, (float)(MAX_SCORE - (nWidth + 1))));
		}
		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 0.0f);

		// ���̏��Ɉڂ�
		pVtx += 4;
	}
	// �X�R�A�\���̍X�V
	g_nCntScore++;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}