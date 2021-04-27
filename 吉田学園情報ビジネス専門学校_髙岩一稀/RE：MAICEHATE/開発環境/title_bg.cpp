//==============================================================================================================
//
// �^�C�g����ʂ̔w�i (title_bg.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "title_bg.h"
#include "player.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_TITLEBG			(3)					// �^�C�g���w�i�̎��

#define MOVE_UV				(0.001f)			// �^�C�g���w�i��UV�l�ړ���

#define TITLEBG_X			(SCREEN_WIDTH / 2)	// �^�C�g���w�i��X���W
#define TITLEBG_Y			(SCREEN_HEIGHT / 2)	// �^�C�g���w�i��Y���W
#define TITLEBG_WIDTH		(SCREEN_WIDTH)		// �^�C�g���w�i�̕�
#define TITLEBG_HEIGHT		(SCREEN_HEIGHT)		// �^�C�g���w�i�̍���

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	float fWidth;			// ��
	float fHieght;			// ����
}TitleBg;

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleBg = NULL;						// �^�C�g���w�i�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitleBg[MAX_TITLEBG] = {};					// �^�C�g���w�i�̃e�N�X�`���̃|�C���^
TitleBg g_TitleBg[MAX_TITLEBG];											// �^�C�g���w�i���

float g_fMoveUV[MAX_TITLEBG] = { MOVE_UV ,MOVE_UV * 2 ,MOVE_UV * 3 };	// �^�C�g���w�i��UV�l�̌�����
int g_nMoveBg[MAX_TITLEBG] = { 6000,4500,3000 };						// �^�C�g���w�i�̈ړ���

//--------------------------------------------------------------------------------------------------------------
// �^�C�g���w�i�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTitleBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �^�C�g���w�i���̏�����
	for (int nCntTitleBg = 0; nCntTitleBg < MAX_TITLEBG; nCntTitleBg++)
	{
		g_TitleBg[nCntTitleBg].pos = D3DXVECTOR3(TITLEBG_X, TITLEBG_Y, 0.0f);
		g_TitleBg[nCntTitleBg].fWidth = TITLEBG_WIDTH / 2.0f;
		g_TitleBg[nCntTitleBg].fHieght = TITLEBG_HEIGHT / 2.0f;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureTitleBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureTitleBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureTitleBg[2]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TITLEBG,	// �m�ۂ���o�b�t�@�T�C�Y
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

	for (int nCntTitleBg = 0; nCntTitleBg < MAX_TITLEBG; nCntTitleBg++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x - g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y + g_TitleBg[nCntTitleBg].fHieght, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x - g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y - g_TitleBg[nCntTitleBg].fHieght, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x + g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y + g_TitleBg[nCntTitleBg].fHieght, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x + g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y - g_TitleBg[nCntTitleBg].fHieght, 0.0f);

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

		// �e�N�X�`�����W�̐ݒ�
		SetTexUVTitleBg(nCntTitleBg);

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleBg->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g���w�i�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitTitleBg(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitleBg != NULL)
	{
		g_pVtxBuffTitleBg->Release();
		g_pVtxBuffTitleBg = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntBg = 0; nCntBg < MAX_TITLEBG; nCntBg++)
	{
		if (g_pTextureTitleBg[nCntBg] != NULL)
		{
			g_pTextureTitleBg[nCntBg]->Release();
			g_pTextureTitleBg[nCntBg] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g���w�i�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateTitleBg(void)
{
	for (int nCntBg = 0; nCntBg < MAX_TITLEBG; nCntBg++)
	{
		// �ϐ������Z
		g_fMoveUV[nCntBg] -= MOVE_UV * (nCntBg + 1);

		// UV�̒l��1.0f�𒴂����Ƃ�0.0f�ɖ߂�
		if (g_fMoveUV[nCntBg] >= 1.0f)
		{
			g_fMoveUV[nCntBg] = 0.0f;
		}
		// �e�N�X�`�����W�̍X�V
		SetTexUVTitleBg(nCntBg);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g���w�i�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
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
	for (int nCntBg = 0; nCntBg < MAX_TITLEBG; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureTitleBg[nCntBg]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// ���_���W�̐ݒ�
// ����		�FnCntTitleBg	- ���Ԗڂ̃^�C�g���w�i���J�E���g����
// �Ԃ�l	�Fvoid			- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetTexUVTitleBg(int nCntTitleBg)
{
	// TitleBg�^�\���̂̃|�C���^�ϐ�
	TitleBg *pTitleBg = &g_TitleBg[0];

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCntTitleBg * 4;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + g_nMoveBg[nCntTitleBg], 1.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);
	pVtx[1].tex = D3DXVECTOR2(0.0f + g_nMoveBg[nCntTitleBg], 0.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);
	pVtx[2].tex = D3DXVECTOR2(1.0f + g_nMoveBg[nCntTitleBg], 1.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);
	pVtx[3].tex = D3DXVECTOR2(1.0f + g_nMoveBg[nCntTitleBg], 0.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleBg->Unlock();
}