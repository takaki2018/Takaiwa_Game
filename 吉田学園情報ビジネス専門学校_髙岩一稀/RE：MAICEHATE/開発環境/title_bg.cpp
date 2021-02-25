//-----------------------------------------------------------------
//
// �^�C�g����ʂ̔w�i (title_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "title_bg.h"
#include "player.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN			(3)									// �w�i�̎��

#define MOVE_BG				(0.001f)							// �w�i�̈ړ���

#define SKY_X				(SCREEN_WIDTH / 2)
#define SKY_Y				(SCREEN_HEIGHT / 2)
#define SKY_WIDTH			(SCREEN_WIDTH)
#define SKY_HEIGHT			(SCREEN_HEIGHT)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleBg = NULL;				// �w�i�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitleBg[MAX_PATTERN] = {};			// �w�i�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posTitleBg;

float g_fMoveUV[MAX_PATTERN] = { MOVE_BG ,MOVE_BG * 2 ,MOVE_BG * 3 };	// �w�i��UV�l�̌�����
int g_nMoveBg[MAX_PATTERN] = { 6000,4500,3000 };						// �w�i�̈ړ���

//-----------------------------------------------------------------
// �w�i�̏���������
//-----------------------------------------------------------------
HRESULT InitTitleBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_posTitleBg = D3DXVECTOR3(SKY_X, SKY_Y, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureTitleBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureTitleBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureTitleBg[2]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleBg,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		// ���_���W�̐ݒ�
		SetVertexTitleBg(nCntBg);

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

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleBg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �w�i�̏I������
//-----------------------------------------------------------------
void UninitTitleBg(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitleBg != NULL)
	{
		g_pVtxBuffTitleBg->Release();
		g_pVtxBuffTitleBg = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		if (g_pTextureTitleBg[nCntBg] != NULL)
		{
			g_pTextureTitleBg[nCntBg]->Release();
			g_pTextureTitleBg[nCntBg] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �w�i�̍X�V����
//-----------------------------------------------------------------
void UpdateTitleBg(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		// �w�i�̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_nMoveBg[nCntBg], 1.0f + g_fMoveUV[nCntBg] + g_nMoveBg[nCntBg]);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_nMoveBg[nCntBg], 0.0f + g_fMoveUV[nCntBg] + g_nMoveBg[nCntBg]);
		pVtx[2].tex = D3DXVECTOR2(1.0f + g_nMoveBg[nCntBg], 1.0f + g_fMoveUV[nCntBg] + g_nMoveBg[nCntBg]);
		pVtx[3].tex = D3DXVECTOR2(1.0f + g_nMoveBg[nCntBg], 0.0f + g_fMoveUV[nCntBg] + g_nMoveBg[nCntBg]);

		// ���̍X�V
		pVtx += 4;

		// �ϐ������Z
		g_fMoveUV[nCntBg] -= MOVE_BG * (nCntBg + 1);

		// 
		if (g_fMoveUV[nCntBg] >= 1.0f)
		{
			g_fMoveUV[nCntBg] = 0.0f;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleBg->Unlock();
}

//-----------------------------------------------------------------
// �w�i�̕`�揈��
//-----------------------------------------------------------------
void DrawTitleBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureTitleBg[nCntBg]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexTitleBg(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posTitleBg.x - (SKY_WIDTH / 2), g_posTitleBg.y + (SKY_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posTitleBg.x - (SKY_WIDTH / 2), g_posTitleBg.y - (SKY_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posTitleBg.x + (SKY_WIDTH / 2), g_posTitleBg.y + (SKY_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posTitleBg.x + (SKY_WIDTH / 2), g_posTitleBg.y - (SKY_HEIGHT / 2), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleBg->Unlock();
}