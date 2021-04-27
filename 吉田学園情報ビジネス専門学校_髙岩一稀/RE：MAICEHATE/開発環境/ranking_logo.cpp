//==============================================================================================================
//
// �����L���O (ranking.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "ranking_logo.h"
#include "score.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN				(1)					// �e�N�X�`���̐�

#define RANKING_LOGO_X			(SCREEN_WIDTH / 2)	// �����L���O���S�̒��_���W(X)
#define RANKING_LOGO_Y			(90.0f)				// �����L���O���S�̒��_���W(Y)
#define RANKING_LOGO_WIDTH		(600.0f)			// �����L���O���S�̕�
#define RANKING_LOGO_HEIGHT		(70.0f)				// �����L���O���S�̍���

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingLogo = NULL;							// �����L���O�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRankingLogo[MAX_PATTERN] = {};						// �����L���O�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posRankingLogo;												// �����L���O���S�̒��_���W

//--------------------------------------------------------------------------------------------------------------
// �����L���O���S�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingLogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posRankingLogo = D3DXVECTOR3(RANKING_LOGO_X, RANKING_LOGO_Y, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_logo.png", &g_pTextureRankingLogo[0]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingLogo->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		// ���_���W�̐ݒ�
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingLogo->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O���S�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitRankingLogo(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRankingLogo != NULL)
	{
		g_pVtxBuffRankingLogo->Release();
		g_pVtxBuffRankingLogo = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRankingLogo[nCntRanking] != NULL)
		{
			g_pTextureRankingLogo[nCntRanking]->Release();
			g_pTextureRankingLogo[nCntRanking] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O���S�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateRankingLogo(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// �����L���O���S�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawRankingLogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankingLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRankingLogo[nCntRanking]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}