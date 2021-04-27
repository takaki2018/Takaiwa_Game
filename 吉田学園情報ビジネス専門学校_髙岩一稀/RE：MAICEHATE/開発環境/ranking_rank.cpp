//==============================================================================================================
//
// �����L���O���� (ranking_rank.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "ranking_rank.h"
#include "ranking_score.h"
#include "setparticle.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN				(5)					// �w�i�̎��

#define RANKING_RANK_X			(350.0f)			// �����L���O�����N�̒��_���W(X)
#define RANKING_RANK_Y			(220.0f)			// �����L���O�����N�̒��_���W(Y)
#define RANKING_RANK_WIDTH		(140.0f)			// �����L���O�����N�̕�
#define RANKING_RANK_HEIGHT		(70.0f)				// �����L���O�����N�̍���
#define RANKING_RANK_SPACE		(100.0f)			// �����L���O�����N��z�u����Ԋu

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingRank = NULL;				// �����L���O�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRankingRank[MAX_PATTERN] = {};			// �����L���O�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posRankingRank;										// �����L���O�����N�̒��_���W

//--------------------------------------------------------------------------------------------------------------
// �����L���O���ʂ̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingRank(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posRankingRank = D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureRankingRank[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureRankingRank[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureRankingRank[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureRankingRank[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_5th.png", &g_pTextureRankingRank[4]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingRank,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingRank->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);

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

		// ���̏��Ɉڍs
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingRank->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O���ʂ̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitRankingRank(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRankingRank != NULL)
	{
		g_pVtxBuffRankingRank->Release();
		g_pVtxBuffRankingRank = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRankingRank[nCntRanking] != NULL)
		{
			g_pTextureRankingRank[nCntRanking]->Release();
			g_pTextureRankingRank[nCntRanking] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O���ʂ̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateRankingRank(void)
{
	// �ϐ��錾
	int nFalshRank = GetFlashNumber();

	if (nFalshRank != -1)
	{
		SetRankingEffect(D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, 
			(g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nFalshRank, 0.0f),
			false);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O���ʂ̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawRankingRank(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankingRank, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRankingRank[nCntRanking]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}

}