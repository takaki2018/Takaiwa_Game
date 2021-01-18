//-----------------------------------------------------------------
//
// �؂̗�(wood.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "wood_back.h"
#include "bear.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define WOOD_BACK_X				(SCREEN_WIDTH / 2)				// �؂̒��S���W(X)
#define WOOD_BACK_Y				(SCREEN_HEIGHT / 2)				// �؂̒��S���W(Y)
#define WOOD_BACK_WIDTH			(SCREEN_WIDTH)					// �؂̕�
#define WOOD_BACK_HEIGHT		(SCREEN_HEIGHT)					// �؂̍���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWoodBack = NULL;				// �؂̗��̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWoodBack = NULL;					// �؂̗��̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posWoodBack;										// �؂̗��̒��S���W

//-----------------------------------------------------------------
// �؂̗��̏���������
//-----------------------------------------------------------------
HRESULT InitWoodBack(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_posWoodBack = D3DXVECTOR3(WOOD_BACK_X, WOOD_BACK_Y, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wood_back.png", &g_pTextureWoodBack);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,			// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWoodBack,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWoodBack->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetVertexWoodBack(0);

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

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWoodBack->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �؂̗��̏I������
//-----------------------------------------------------------------
void UninitWoodBack(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffWoodBack != NULL)
	{
		g_pVtxBuffWoodBack->Release();
		g_pVtxBuffWoodBack = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureWoodBack != NULL)
	{
		g_pTextureWoodBack->Release();
		g_pTextureWoodBack = NULL;
	}
}

//-----------------------------------------------------------------
// �؂̗��̍X�V����
//-----------------------------------------------------------------
void UpdateWoodBack(void)
{
	// ���_���W�̍X�V
	SetVertexWoodBack(0);
}

//-----------------------------------------------------------------
// �؂̗��̕`�揈��
//-----------------------------------------------------------------
void DrawWoodBack(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWoodBack, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTextureWoodBack);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexWoodBack(int nCnt)
{
	// �ϐ��錾
	float ShaikingMove = GetShaking();

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWoodBack->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posWoodBack.x - (WOOD_BACK_WIDTH / 2), g_posWoodBack.y + (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posWoodBack.x - (WOOD_BACK_WIDTH / 2), g_posWoodBack.y - (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posWoodBack.x + (WOOD_BACK_WIDTH / 2), g_posWoodBack.y + (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posWoodBack.x + (WOOD_BACK_WIDTH / 2), g_posWoodBack.y - (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWoodBack->Unlock();
}