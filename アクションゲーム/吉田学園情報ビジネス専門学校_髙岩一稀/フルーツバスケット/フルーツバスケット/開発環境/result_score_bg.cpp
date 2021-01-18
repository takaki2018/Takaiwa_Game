//-----------------------------------------------------------------
//
// ���U���g�X�R�A�w�i (result_score_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_score_bg.h"
#include "selectmode.h"
#include "gauge.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PLAYER				(4)						// �v���C���[�̍ő吔
#define RESULTSCOREBG_X			(370.0f)				// 1�Ԗڂ̃X�R�A�̒��S���W(X)
#define RESULTSCOREBG_Y			(500.0f)					// 1�Ԗڂ̃X�R�A�̒��S���W(Y)

#define RESULTSCOREBG_WIDTH		(150.0f)				// �X�R�A�̕�
#define RESULTSCOREBG_HEIGHT	(75.0f)					// �X�R�A�̍���
#define WIDTH_X					(40.0f)					// �X�R�A��z�u����Ԋu
#define PlAYER_WIDTH			(180.0f)				// �v���C���[��z�u����Ԋu

#define MAX_PATTERN				(1)						// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X				(1)						// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y				(1)						// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScoreBg = NULL;			// ���U���g�X�R�A�w�i�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultScoreBg[MAX_PLAYER] = {};	// ���U���g�X�R�A�w�i�̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posResultScoreBg[MAX_PLAYER];						// ���U���g�X�R�A�w�i�̒��S���W
int g_nDisplayResultScoreBg;									// �\�����鐔��ۑ�

//-----------------------------------------------------------------
// ���U���g�X�R�A�w�i�̏���������
//-----------------------------------------------------------------
HRESULT InitResultScoreBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// �V���O�����[�h�̎�����
	case PLAYMODE_SINGLE:
		g_nDisplayResultScoreBg = 1;
		break;
		// �}���`���[�h�̎�����
	case PLAYMODE_MULTI:
		g_nDisplayResultScoreBg = MAX_PLAYER;
		break;
	}

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_1P.png", &g_pTextureResultScoreBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_2P.png", &g_pTextureResultScoreBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_3P.png", &g_pTextureResultScoreBg[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_4P.png", &g_pTextureResultScoreBg[3]);

	// �ϐ��̏�����
	for (int nCntResultScoreBg = 0; nCntResultScoreBg < MAX_PLAYER; nCntResultScoreBg++)
	{
		g_posResultScoreBg[nCntResultScoreBg] = D3DXVECTOR3(RESULTSCOREBG_X + PlAYER_WIDTH * nCntResultScoreBg, RESULTSCOREBG_Y, 0.0f);
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PLAYER, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScoreBg,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScoreBg->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nDisplayScore = 0; nDisplayScore < MAX_PLAYER; nDisplayScore++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2), 0.0f);

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

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScoreBg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ���U���g�X�R�A�w�i�̏I������
//-----------------------------------------------------------------
void UninitResultScoreBg(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultScoreBg != NULL)
	{
		g_pVtxBuffResultScoreBg->Release();
		g_pVtxBuffResultScoreBg = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < g_nDisplayResultScoreBg; nCnt++)
	{
		if (g_pTextureResultScoreBg[nCnt] != NULL)
		{
			g_pTextureResultScoreBg[nCnt]->Release();
			g_pTextureResultScoreBg[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ���U���g�X�R�A�w�i�̍X�V����
//-----------------------------------------------------------------
void UpdateResultScoreBg(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScoreBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDisplayScore = 0;nDisplayScore < g_nDisplayResultScoreBg;nDisplayScore++)
	{
		// �ϐ��錾
		float fmove = GetMoveGauge(nDisplayScore);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
	
		// ���̏��
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScoreBg->Unlock();
}

//-----------------------------------------------------------------
// ���U���g�X�R�A�w�i�̕`�揈��
//-----------------------------------------------------------------
void DrawResultScoreBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScoreBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntScore = 0; nCntScore < g_nDisplayResultScoreBg; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureResultScoreBg[nCntScore]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}