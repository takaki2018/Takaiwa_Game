//-----------------------------------------------------------------
//
// �Q�[�W (gauge.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "gauge.h"
#include "selectmode.h"
#include "player.h"
#include "score.h"
#include "result_score.h"
#include "rank_logo.h"
#include "sound.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN		(4)				// ���

#define GAUGE_CENTER_X	(365.0f)		// �Q�[�W�̒��S���W(X)
#define GAUGE_CENTER_Y	(690.0f)		// �Q�[�W�̒��S���W(Y)

#define GAUGE_X			(100.0f)		// �Q�[�W�̕�

#define PlAYER_WIDTH	(180.0f)		// �v���C���[��z�u����Ԋu

#define RANK_LOGO_WIDTH		(90.0f)			// �����N���S�̕�
#define RANK_LOGO_HEIGHT	(55.0f)			// �����N���S�̍���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;			// �Q�[�W�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureGauge[MAX_PATTERN] = {};	// �Q�[�W�̃e�N�X�`���̃|�C���^
GAUGE g_aGauge[MAX_PATTERN];							// �Q�[�W���
int g_nDisplayGauge;									// �����邩
int g_nCntGauge;										// �Q�[�W��L�΂��Ԋu�̃J�E���g									 
bool g_bSoundResult;									// ���U���g��BGM���Ȃ������ǂ���
int g_nCntGaugeTimer;									// ���U���g��BGM�𗬂��J�E���g
bool g_bSkipResult;										// ���U���g���΂������ǂ���

//-----------------------------------------------------------------
// �Q�[�W�̏���������
//-----------------------------------------------------------------
HRESULT InitGauge(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	g_nDisplayGauge = GetPlayerCnt();
	g_bSoundResult = false;
	g_nCntGaugeTimer = 0;
	g_bSkipResult = false;

	// �ϐ��̏�����
	for (int nCntBar = 0; nCntBar < MAX_PATTERN; nCntBar++)
	{
		g_aGauge[nCntBar].pos = D3DXVECTOR3(GAUGE_CENTER_X + PlAYER_WIDTH * nCntBar, GAUGE_CENTER_Y, 0.0f);
		g_aGauge[nCntBar].fMoveHeight = 0.0f;
		g_aGauge[nCntBar].nRank = RANK_1;
		g_aGauge[nCntBar].nCntScore = 0;
		g_aGauge[nCntBar].bReach = false;
	}
	// ���ʂ̊m��
	RankingGauge();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_1.png", &g_pTextureGauge[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_2.png", &g_pTextureGauge[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_3.png", &g_pTextureGauge[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_4.png", &g_pTextureGauge[3]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �Q�[�W�̏I������
//-----------------------------------------------------------------
void UninitGauge(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (g_pTextureGauge[nCnt] != NULL)
		{
			g_pTextureGauge[nCnt]->Release();
			g_pTextureGauge[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �Q�[�W�̍X�V����
//-----------------------------------------------------------------
void UpdateGauge(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	PLAYMODE playmode = GetPlayMode();

	// ��ʑJ��
	if (GetKeyboardTrigger(KEYINFO_OK) == true || 
		GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true)
	{
		g_bSkipResult = true;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCntGaugeTimer != 0)
	{// �Q�[�W���L�т�J�E���g
		g_nCntGaugeTimer--;

		if (g_bSoundResult == false)
		{// ���ׂẴv���C���[�̃Q�[�W���L�т������Ƃ�
			//�T�E���h�̈ꎞ��~
			StopSound(SOUND_LABEL_SE_DORAM);

			// �T�E���h
			PlaySound(SOUND_LABEL_RESULT);
			g_bSoundResult = true;
		}
	}

	// �J�E���g�̉��Z
	g_nCntGauge++;

	for (int nCnt = 0; nCnt < g_nDisplayGauge; nCnt++)
	{
		// �ϐ��錾
		int nScore = GetScore(nCnt);			// �e�v���C���[�̃X�R�A

		if (nScore > g_aGauge[nCnt].nCntScore)
		{
			// �����̍X�V
			g_aGauge[nCnt].fMoveHeight += 1.0f;

			// �X�R�A�̍X�V
			if (0 == g_nCntGauge % 5)
			{// �X�R�A�̉��Z
				g_aGauge[nCnt].nCntScore++;		
				AddResultScore(nCnt, 1);
			}
			if (g_bSkipResult == true)
			{// �ϐ��錾
				int nScoreRemaining;			// �K�v�Ȏc��X�R�A��ۑ����邽�߂̕ϐ�

				nScoreRemaining = nScore - g_aGauge[nCnt].nCntScore;			// �c��X�R�A�̌v�Z
				g_aGauge[nCnt].nCntScore = nScore;								// �v���C���[�̃X�R�A��ۑ�
				g_aGauge[nCnt].fMoveHeight += 5.0f * (float)nScoreRemaining;	// �c��X�R�A�̕������Q�[�W��L�΂�
				AddResultScore(nCnt, nScoreRemaining);							// �c��X�R�A������
			}
			if (g_aGauge[nCnt].fMoveHeight > 300.0f)
			{// �Q�[�W�̍������
				g_aGauge[nCnt].fMoveHeight = 300.0f;
			}
		}
		else if (nScore == g_aGauge[nCnt].nCntScore && g_aGauge[nCnt].bReach == false)
		{// �����̃X�R�A�ɓ��B�����珈��
			g_aGauge[nCnt].bReach = true;							// �X�R�A�ɓ��B

			if (playmode == PLAYMODE_MULTI)
			{
				SetRankLogo(D3DXVECTOR3(g_aGauge[nCnt].pos.x, g_aGauge[nCnt].pos.y, g_aGauge[nCnt].pos.z),
					RANK_LOGO_WIDTH, RANK_LOGO_HEIGHT, (RANK)g_aGauge[nCnt].nRank);	// ���ʂ��ݒu
			}

			switch (g_aGauge[nCnt].nRank)
			{// ���
			case RANK_1:
				// �T�E���h
				PlaySound(SOUND_LABEL_SE_RANK_1);

				// �ϐ��̏�����
				g_nCntGaugeTimer = 30;
				break;
			// ���
			case RANK_2:
				// �T�E���h
				PlaySound(SOUND_LABEL_SE_RANK_2);
				break;
			// �O��
			case RANK_3:
				// �T�E���h
				PlaySound(SOUND_LABEL_SE_RANK_3);
				break;
			// �l��
			case RANK_4:
				// �T�E���h
				PlaySound(SOUND_LABEL_SE_RANK_4);
				break;
			}
		}

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight , 0.0f);

		// ���̏��
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//-----------------------------------------------------------------
// �Q�[�W�̕`�揈��
//-----------------------------------------------------------------
void DrawGauge(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureGauge[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// �����L���O�̊m��
//-----------------------------------------------------------------
void RankingGauge(void)
{
	// �X�R�A�̊m��
	for (int nCnt = 0; nCnt < g_nDisplayGauge; nCnt++)
	{
		// �ϐ��錾
		int nScore = GetScore(nCnt);

		// �X�R�A��ۑ�
		g_aGauge[nCnt].nScore = nScore;
	}
	
	// ���ʂ̊m��
	for (int nCnt = 0; nCnt < g_nDisplayGauge; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < g_nDisplayGauge; nCnt2++)
		{
			if (nCnt != nCnt2)
			{
				if (g_aGauge[nCnt].nScore < g_aGauge[nCnt2].nScore)
				{// �����L���O�X�R�A�X�V�����Ƃ�
					g_aGauge[nCnt].nRank++;
				}
			}
		}
	}
}


//-----------------------------------------------------------------
// �Q�[�W�̕`�揈��
//-----------------------------------------------------------------
float GetMoveGauge(int nCnt)
{
	return g_aGauge[nCnt].fMoveHeight;
}

//-----------------------------------------------------------------
// �Q�[�W���オ�肫�������ǂ���
//-----------------------------------------------------------------
bool GetbGauge(void)
{
	return g_bSoundResult;
}