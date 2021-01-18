//-----------------------------------------------------------------
//
// �w�i (bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bg.h"
#include "player.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN     (4)				// �w�i�̎��
#define MAX_COUNT_BG	(3)				// �w�i�̐�

#define MOVE_BG			(0.002f)		// �w�i�̈ړ���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;							// �w�i�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_PATTERN] = {};						// �w�i�̃e�N�X�`���̃|�C���^

float g_fMoveUV[MAX_COUNT_BG] = { MOVE_BG ,MOVE_BG * 2 ,MOVE_BG * 3 };	// �w�i��UV�l�̌�����
int g_nMoveBg[MAX_COUNT_BG] = {6000,4500,3000 };						// �w�i�̈ړ���

int g_nCntPattern;														// �ǂݍ��ރe�N�X�`���̐�
MODE g_nmode;															// ���[�h�̎���

//-----------------------------------------------------------------
// �w�i�̏���������
//-----------------------------------------------------------------
HRESULT InitBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_nCntPattern = MAX_PATTERN;
	g_nmode = GetMode();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureBg[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg200.png", &g_pTextureBg[3]);

	// ���[�h���^�C�g���̂Ƃ�����
	if (g_nmode == MODE_TITLE)
	{
		g_nCntPattern = MAX_COUNT_BG;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * g_nCntPattern, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
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

	for (int nCntBg = 0; nCntBg < g_nCntPattern; nCntBg++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		if (nCntBg == 3)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT + 50.0f , 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,50.0f, 0.0f);
		}

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
	for (int nCntBg = 0; nCntBg < g_nCntPattern; nCntBg++)
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

	for (int nCntBg = 0; nCntBg < MAX_COUNT_BG; nCntBg++)
	{
		// �w�i�̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 1.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);
		pVtx[1].tex = D3DXVECTOR2(0.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 0.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);
		pVtx[2].tex = D3DXVECTOR2(1.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 1.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);
		pVtx[3].tex = D3DXVECTOR2(1.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 0.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);

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
	for (int nCntBg = 0; nCntBg < g_nCntPattern; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureBg[nCntBg]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}