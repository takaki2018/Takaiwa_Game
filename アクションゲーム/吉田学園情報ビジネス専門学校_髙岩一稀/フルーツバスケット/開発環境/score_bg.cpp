//-----------------------------------------------------------------
//
// �X�R�A�w�i (score_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "score_bg.h"
#include "selectmode.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)						// �v���C���[�̍ő吔

#define SCOREBG_X_SINGLE	(380.0f)				// 1�Ԗڂ̃X�R�A�w�i�̒��S���W(X)�V���O�����[�h
#define SCOREBG_X_MULTI		(130.0f)				// 1�Ԗڂ̃X�R�A�w�i�̒��S���W(X)�}���`���[�h

#define SCOREBG_TWO_X		(380.0f)				// 2�Ԗڂ̃X�R�A�w�i�̒��S���W(X)
#define SCOREBG_TUIRD_X		(890.0f)				// 3�Ԗڂ̃X�R�A�w�i�̒��S���W(X)
#define SCOREBG_FOURTH_X	(1140.0f)				// 4�Ԗڂ̃X�R�A�w�i�̒��S���W(X)
#define SCOREBG_Y			(70.0f)					// 1�Ԗڂ̃X�R�A�w�i�̒��S���W(Y)

#define SCOREBG_WIDTH		(195.0f)				// �X�R�A�w�i�̕�
#define SCOREBG_HEIGHT		(95.0f)					// �X�R�A�w�i�̍���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreBg = NULL;			// �X�R�A�w�i�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureScoreBg[MAX_PLAYER] = {};		// �X�R�A�w�i�̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posScoreBg[MAX_PLAYER];						// �X�R�A�w�i�̒��S���W
int g_nDisplayScoreBg;										// �\�����鐔��ۑ�

//-----------------------------------------------------------------
// �X�R�A�w�i�̏���������
//-----------------------------------------------------------------
HRESULT InitScoreBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	PLAYMODE playmode = GetPlayMode();

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_1P.png", &g_pTextureScoreBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_2P.png", &g_pTextureScoreBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_3P.png", &g_pTextureScoreBg[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_4P.png", &g_pTextureScoreBg[3]);

	// �ϐ��̏�����
	switch (playmode)
	{// �V���O�����[�h�̂Ƃ�����
	case PLAYMODE_SINGLE:
		g_nDisplayScoreBg = 1;										// �\������X�R�A�w�i�̐�
		g_posScoreBg[0] = D3DXVECTOR3(SCOREBG_X_SINGLE, SCOREBG_Y, 0.0f);		// �z�u������W(1P)
		break;
	// �}���`���[�h�̂Ƃ�����
	case PLAYMODE_MULTI:
		g_nDisplayScoreBg = MAX_PLAYER;								// �\������X�R�A�w�i�̐�
		g_posScoreBg[0] = D3DXVECTOR3(SCOREBG_X_MULTI, SCOREBG_Y, 0.0f);		// �z�u������W(1P)
		break;
	}	
	g_posScoreBg[1] = D3DXVECTOR3(SCOREBG_TWO_X, SCOREBG_Y, 0.0f);		// �z�u������W(2P)
	g_posScoreBg[2] = D3DXVECTOR3(SCOREBG_TUIRD_X, SCOREBG_Y, 0.0f);	// �z�u������W(3P)
	g_posScoreBg[3] = D3DXVECTOR3(SCOREBG_FOURTH_X, SCOREBG_Y, 0.0f);	// �z�u������W(4P)

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * g_nDisplayScoreBg, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreBg,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreBg->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayScoreBg; nDisplayScore++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x - (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y + (SCOREBG_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x - (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y - (SCOREBG_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x + (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y + (SCOREBG_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x + (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y - (SCOREBG_HEIGHT / 2), 0.0f);

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
	g_pVtxBuffScoreBg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �X�R�A�w�i�̏I������
//-----------------------------------------------------------------
void UninitScoreBg(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScoreBg != NULL)
	{
		g_pVtxBuffScoreBg->Release();
		g_pVtxBuffScoreBg = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < g_nDisplayScoreBg; nCnt++)
	{
		if (g_pTextureScoreBg[nCnt] != NULL)
		{
			g_pTextureScoreBg[nCnt]->Release();
			g_pTextureScoreBg[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �X�R�A�w�i�̍X�V����
//-----------------------------------------------------------------
void UpdateScoreBg(void)
{

}

//-----------------------------------------------------------------
// �X�R�A�w�i�̕`�揈��
//-----------------------------------------------------------------
void DrawScoreBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScoreBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntScore = 0; nCntScore < g_nDisplayScoreBg; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureScoreBg[nCntScore]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}