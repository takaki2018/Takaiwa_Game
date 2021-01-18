//-----------------------------------------------------------------
//
// �Q�[����ʂ̔w�i (bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bg.h"
#include "player.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN			(2)									// �w�i�̎��

#define MOVE_BG				(0.0001f)							// �w�i�̈ړ���

#define SKY_X				(SCREEN_WIDTH / 2)
#define SKY_Y				(SCREEN_HEIGHT / 2)
#define SKY_WIDTH			(SCREEN_WIDTH)
#define SKY_HEIGHT			(SCREEN_HEIGHT)

#define MOUNTAIN_X			(SCREEN_WIDTH / 2)
#define MOUNTAIN_Y			(SCREEN_HEIGHT / 2)
#define MOUNTAIN_WIDTH		(SCREEN_WIDTH)
#define MOUNTAIN_HEIGHT		(SCREEN_HEIGHT)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;				// �w�i�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_PATTERN] = {};			// �w�i�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posSky;
D3DXVECTOR3 g_posMountain;

float g_fMoveUV = MOVE_BG;									// �w�i��UV�l�̌�����
int g_nMoveBg= 60000;										// �w�i�̈ړ���

//-----------------------------------------------------------------
// �w�i�̏���������
//-----------------------------------------------------------------
HRESULT InitBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_posSky = D3DXVECTOR3(SKY_X, SKY_Y, 0.0f);
	g_posMountain = D3DXVECTOR3(MOUNTAIN_X, MOUNTAIN_Y ,0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cloud.png", &g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/reaf.png", &g_pTextureBg[1]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		// ���_���W�̐ݒ�
		SetVertexBg(nCntBg);

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
	g_pVtxBuffBg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �w�i�̏I������
//-----------------------------------------------------------------
void UninitBg(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		if (g_pTextureBg[nCntBg] != NULL)
		{
			g_pTextureBg[nCntBg]->Release();
			g_pTextureBg[nCntBg] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �w�i�̍X�V����
//-----------------------------------------------------------------
void UpdateBg(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	// �ϐ��̎󂯓n��
	pPlayer = GetPlayer();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// �w�i�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f - g_fMoveUV, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f - g_fMoveUV, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f - g_fMoveUV, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f - g_fMoveUV, 0.0f);

	// �ϐ������Z
	g_fMoveUV -= MOVE_BG ;

	// 
	if (g_fMoveUV >= 1.0f)
	{
		g_fMoveUV = 0.0f;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//-----------------------------------------------------------------
// �w�i�̕`�揈��
//-----------------------------------------------------------------
void DrawBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureBg[nCntBg]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexBg(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	if (nCnt == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(g_posSky.x - (SKY_WIDTH / 2), g_posSky.y + (SKY_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posSky.x - (SKY_WIDTH / 2), g_posSky.y - (SKY_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posSky.x + (SKY_WIDTH / 2), g_posSky.y + (SKY_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posSky.x + (SKY_WIDTH / 2), g_posSky.y - (SKY_HEIGHT / 2), 0.0f);
	}
	else if (nCnt == 1)
	{
		pVtx[0].pos = D3DXVECTOR3(g_posMountain.x - (MOUNTAIN_WIDTH / 2), g_posMountain.y + (MOUNTAIN_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posMountain.x - (MOUNTAIN_WIDTH / 2), g_posMountain.y - (MOUNTAIN_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posMountain.x + (MOUNTAIN_WIDTH / 2), g_posMountain.y + (MOUNTAIN_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posMountain.x + (MOUNTAIN_WIDTH / 2), g_posMountain.y - (MOUNTAIN_HEIGHT / 2), 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}