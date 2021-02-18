//-------------------------------------------------------------------
//
// �����L���O (ranking.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ranking.h"
#include "score.h"
#include "selectmode.h"
#include "rank_logo.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_SCORE				(2)			// �\������X�R�A�̌���
#define MAX_PATTERN				(2)			// �w�i�̎��
#define MAX_TEXTURE				(11)		// �\��e�N�X�`���̍ő吔
#define TEXTURE_X				(10)		// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y				(1)			// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔
#define MAX_RANKING_RANK		(5)			// ���ʂ̐�

#define RANKING_LOGO_X			(800.0f)	// �����L���O���S�̒��_���W(X)
#define RANKING_LOGO_Y			(200.0f)	// �����L���O���S�̒��_���W(Y)
#define RANKING_LOGO_WIDTH		(440.0f)	// �����L���O���S�̕�
#define RANKING_LOGO_HEIGHT		(64.0f)		// �����L���O���S�̍���

#define RANKING_RANK_X			(650.0f)	// �����L���O�����N�̒��_���W(X)
#define RANKING_RANK_Y			(300.0f)	// �����L���O�����N�̒��_���W(Y)
#define RANKING_RANK_WIDTH		(110.0f)	// �����L���O�����N�̕�
#define RANKING_RANK_HEIGHT		(70.0f)		// �����L���O�����N�̍���
#define RANKING_RANK_SPACE		(70.0f)		// �����L���O�����N��z�u����Ԋu

#define RANKING_SCORE_X			(900.0f)	// �����L���O�X�R�A�̒��_���W(X)
#define RANKING_SCORE_Y			(300.0f)	// �����L���O�X�R�A�̒��_���W(Y)
#define RANKING_SCORE_WIDTH		(50.0f)		// �����L���O�X�R�A�̕�
#define RANKING_SCORE_HEIGHT	(70.0f)		// �����L���O�X�R�A�̍���
#define RANKING_SCORE_SPACE		(60.0f)		// �����L���O�X�R�A��z�u����Ԋu

#define FLASH_SCORE_INTERVAL	(10)			// �_�ł̃C���^�[�o��

//-----------------------------------------------------------------
// �����L���O�Ŕz�u�������(�񋓌^)
//-----------------------------------------------------------------
typedef enum
{
	RANKINGSET_LOGO = 0,	// �����L���O���S
	RANKINGSET_SCORE,		// �����L���O�ɔz�u����X�R�A
	RANKINGSET_MAX,
}RANKINGSET;

//-----------------------------------------------------------------
// �����L���O�̃X�R�A���
//-----------------------------------------------------------------
typedef struct
{
	D3DXCOLOR col;		// �F
	int nScore;			// �X�R�A
	int nRank;			// ����
	bool bChangeRank;	// ���ʂ��ς�邩�ǂ���
}RANKINGSCORE;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;			// �����L���O�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRanking[RANKINGSET_MAX] = {};	// �����L���O�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posRankingLogo;								// �����L���O���S�̒��_���W
D3DXVECTOR3 g_posRankingRank;								// �����L���O�����N�̒��_���W
D3DXVECTOR3 g_posRankingScore;								// �����L���O�X�R�A�̒��_���W
RANKINGSCORE g_aRankingScore[RANK_MAX];						// �����L���O�̃X�R�A���
bool g_bChangeRank;											// ���ʂ��ς�邩�ǂ���
bool g_bFlash;												// �_�ł����邩
int g_nFlashScore;											// �_�ł�����X�R�A
int g_nFlashScoreTimer;										// �_�Ń^�C�}�[

//-----------------------------------------------------------------
// �����L���O�̏���������
//-----------------------------------------------------------------
HRESULT InitRanking(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posRankingLogo = D3DXVECTOR3(RANKING_LOGO_X, RANKING_LOGO_Y, 0.0f);
	g_posRankingRank = D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y, 0.0f);
	g_posRankingScore = D3DXVECTOR3(RANKING_SCORE_X, RANKING_SCORE_Y, 0.0f);
	g_bChangeRank = false;
	g_bFlash = false;
	g_nFlashScore = 0;
	g_nFlashScoreTimer = MAX_RANKING_RANK;

	// �����L���O�̃X�R�A���̍X�V
	for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
	{
		g_aRankingScore[nCntRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRankingScore[nCntRank].nScore = 0;
		g_aRankingScore[nCntRank].nRank = nCntRank;
		g_aRankingScore[nCntRank].bChangeRank = false;
	}

	// �X�R�A�̃��[�h
	LoadRankingScore();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_logo.png", &g_pTextureRanking[RANKINGSET_LOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureRanking[RANKINGSET_SCORE]);

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

	// �����L���O���ʂ̔z�u
	for (int nCntRank = RANK_1; nCntRank < MAX_RANKING_RANK; nCntRank++)
	{
		if (g_aRankingScore[nCntRank - 1].nScore == g_aRankingScore[nCntRank].nScore && nCntRank != RANK_1)
		{
			// �O�̏��ʂƓ������ʂ�ۑ�
			g_aRankingScore[nCntRank].nRank = g_aRankingScore[nCntRank - 1].nRank;
		}
		// ���ʂ��ݒu
		SetRankLogo(D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y + RANKING_RANK_SPACE * nCntRank, 0.0f),
			RANKING_RANK_WIDTH, RANKING_RANK_HEIGHT, (RANK)g_aRankingScore[nCntRank].nRank);
	}

	// ������
	for (int nCntRanking = 0; nCntRanking < RANKINGSET_MAX; nCntRanking++)
	{
		// ���_���W�̐ݒ�
		SetVertexRanking(nCntRanking);
	}

	// �����L���O�̓���ւ�
	Ranking();

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
	// ���ʂ��ς��Ƃ��낪����Ƃ�����
	if (g_bChangeRank == true)
	{
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{// ���Ԗڂ̏��ʂ��ς�邩
			if (g_aRankingScore[nCntRank].bChangeRank == true)
			{
				if (nCntRank == RANK_1)
				{// ���ʂ̈�ʂ�艺���ς��Ƃ�
					for (int nCntRankLower = RANK_2; nCntRankLower < RANK_MAX; nCntRankLower++)
					{
						if (g_aRankingScore[nCntRankLower].nScore == g_aRankingScore[nCntRankLower - 1].nScore && nCntRankLower != RANK_2)
						{// �����X�R�A�œ�ʂ���Ȃ��Ƃ����ʂ����낦��
							g_aRankingScore[nCntRankLower].nRank = g_aRankingScore[nCntRankLower - 1].nRank;
						}
						else
						{
							// ��ʂ���Ȃ��A�܂��͓����X�R�A���Ȃ��Ƃ����ʂ�ύX
							g_aRankingScore[nCntRankLower].nRank = nCntRankLower;
						}

						// �����N���S�̕ύX
						CangeRankLogo(nCntRankLower, (RANK)g_aRankingScore[nCntRankLower].nRank);
					}
				}
				else
				{// ���̑��̏���
					// ���ʂ�ύX
					g_aRankingScore[nCntRank].nRank = g_aRankingScore[nCntRank - 1].nRank;

					// �����N���S�̕ύX
					CangeRankLogo(nCntRank, (RANK)g_aRankingScore[nCntRank].nRank);
				}
				// �t���O�𗎂Ƃ�
				g_bChangeRank = false;
			}
		}
	}
	// �X�R�A���X�V�����Ƃ��_�ł�����
	if (g_bFlash == true)
	{
		// �_�ŗp�J�E���g�𑝂₷
		g_nFlashScoreTimer++;

		if (0 == g_nFlashScoreTimer % FLASH_SCORE_INTERVAL)
		{
			g_aRankingScore[g_nFlashScore].col.b = 0.3f;
			g_aRankingScore[g_nFlashScore].col.g = 0.3f;
		}
		else if (FLASH_SCORE_INTERVAL / 2 == g_nFlashScoreTimer % FLASH_SCORE_INTERVAL)
		{
			g_aRankingScore[g_nFlashScore].col.b = 1.0f;
			g_aRankingScore[g_nFlashScore].col.g = 1.0f;
		}
	}
	// �X�R�A�̍X�V
	SetVertexRanking(RANKINGSET_SCORE);
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
		if (nCntRanking == RANKINGSET_LOGO)
		{
			pDevice->SetTexture(0, g_pTextureRanking[RANKINGSET_LOGO]);
		}
		else if (nCntRanking >= RANKINGSET_SCORE)
		{
			pDevice->SetTexture(0, g_pTextureRanking[RANKINGSET_SCORE]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}

//-----------------------------------------------------------------
// �����L���O�X�R�A�̃��[�h
//-----------------------------------------------------------------
void LoadRankingScore(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data\\ranking.txt", "r");
	if (pFile != NULL)
	{ //�t�@�C���W�J�\
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			fscanf(pFile, "%d", &g_aRankingScore[nCntRank].nScore);
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}

//-----------------------------------------------------------------
// �����L���O�X�R�A�̃Z�[�u
//-----------------------------------------------------------------
void SaveRankingScore(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;

	// �t�@�C���̓W�J
	pFile = fopen("data\\ranking.txt", "w");
	if (pFile != NULL)
	{ // �t�@�C���W�J�\
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			fprintf(pFile, "%d\n", g_aRankingScore[nCntRank].nScore);
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}

//-----------------------------------------------------------------
// �����L���O�̓���ւ�
//-----------------------------------------------------------------
void Ranking(void)
{
	// �ϐ��錾
	int nScore = GetScore(PLAYER_1);
	int nSubScore = 0;
	bool bChangeRanking = false;	// �����L���O�̏������s�����ǂ���

	if (g_aRankingScore[RANK_5].nScore <= nScore)
	{//	5�ʂ̃X�R�A���傫���Ƃ�
		g_aRankingScore[RANK_5].nScore = nScore;
		bChangeRanking = true;
	}
	// �����L���O�ɕϓ�������Ƃ�
	if (bChangeRanking == true)
	{
		// �X�R�A�������Ƃ��O�̏��ʂƓ����ɂ���
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			if (g_aRankingScore[nCntRank].nScore == g_aRankingScore[RANK_5].nScore)
			{
				// �ς��̂Ńt���O�𗧂Ă�
				g_aRankingScore[nCntRank + 1].bChangeRank = true;
				g_bChangeRank = true;
			}
		}

		// �����L���O�̍X�V
		for (int nCntRankTop = 0; nCntRankTop < RANK_MAX - 1; nCntRankTop++)
		{
			for (int nCntRankLower = nCntRankTop + 1; nCntRankLower < RANK_MAX; nCntRankLower++)
			{
				if (g_aRankingScore[nCntRankTop].nScore <= g_aRankingScore[nCntRankLower].nScore)
				{// �����L���O�X�R�A�X�V�����Ƃ�
					nSubScore = g_aRankingScore[nCntRankTop].nScore;
					g_aRankingScore[nCntRankTop].nScore = g_aRankingScore[nCntRankLower].nScore;
					g_aRankingScore[nCntRankLower].nScore = nSubScore;
				}
			}
		}

		// �V�����������X�R�A����ʂ̂Ƃ�
		if (g_aRankingScore[RANK_1].nScore == nScore)
		{// ��ʂ�艺�̏��ʂ��X�V����
			g_aRankingScore[RANK_1].bChangeRank = true;
		}

		// �ǂ��ɓ�������
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			if (g_aRankingScore[nCntRank].nScore == nScore)
			{
				g_bFlash = true;
				g_nFlashScore = nCntRank;
				break;
			}
		}
		// �X�R�A�̃Z�[�u
		SaveRankingScore();
	}
}

//-----------------------------------------------------------------
// �����L���O�̒��_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexRanking(int nCnt)
{
	// ���_���W�̐ݒ�
	if (nCnt == RANKINGSET_LOGO)
	{// �����L���O���S�̔z�u
		RankingLogo(nCnt);
	}
	else if (nCnt == RANKINGSET_SCORE)
	{
		// �X�R�A�̔z�u
		RankingScore(nCnt);
	}
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
// �����L���O�̃X�R�A
//-----------------------------------------------------------------
void RankingScore(int nCnt)
{
	// �ϐ��錾
	int nNumber;												// �X�R�A��ۑ�����ϐ�

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE; nCntDigit++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aRankingScore[nCntRank].col;
			pVtx[1].col = g_aRankingScore[nCntRank].col;
			pVtx[2].col = g_aRankingScore[nCntRank].col;
			pVtx[3].col = g_aRankingScore[nCntRank].col;

			// �X�R�A���i�[
			nNumber = (g_aRankingScore[nCntRank].nScore / (int)powf(10.0f, (float)(MAX_SCORE - (nCntDigit + 1))));

			// �e�N�X�`���̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[1].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 0.0f);

			// ���̏��Ɉڂ�
			pVtx += 4;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}