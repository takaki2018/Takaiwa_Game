//-----------------------------------------------------------------
//
// �� (wood.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "wood.h"
#include "bear.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define WOOD_X				(SCREEN_WIDTH / 2)				// �؂̒��S���W(X)
#define WOOD_Y				(SCREEN_HEIGHT / 2)				// �؂̒��S���W(Y)
#define WOOD_WIDTH			(SCREEN_WIDTH)					// �؂̕�
#define WOOD_HEIGHT			(SCREEN_HEIGHT)					// �؂̍���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWood = NULL;				// �؂̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWood = NULL;					// �؂̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posWood;										// �؂̒��S���W

//-----------------------------------------------------------------
// �؂̏���������
//-----------------------------------------------------------------
HRESULT InitWood(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_posWood = D3DXVECTOR3(WOOD_X, WOOD_Y, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wood.png", &g_pTextureWood);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWood,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWood->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetVertexWood(0);

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
	g_pVtxBuffWood->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �؂̏I������
//-----------------------------------------------------------------
void UninitWood(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffWood != NULL)
	{
		g_pVtxBuffWood->Release();
		g_pVtxBuffWood = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureWood != NULL)
	{
		g_pTextureWood->Release();
		g_pTextureWood = NULL;
	}
}

//-----------------------------------------------------------------
// �؂̍X�V����
//-----------------------------------------------------------------
void UpdateWood(void)
{
	// ���_���W�̍X�V
	SetVertexWood(0);
}

//-----------------------------------------------------------------
// �؂̕`�揈��
//-----------------------------------------------------------------
void DrawWood(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWood, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTextureWood);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 , 2);
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexWood(int nCnt)
{
	// �ϐ��錾
	float ShaikingMove = GetShaking();

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWood->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posWood.x - (WOOD_WIDTH / 2), g_posWood.y + (WOOD_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posWood.x - (WOOD_WIDTH / 2), g_posWood.y - (WOOD_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posWood.x + (WOOD_WIDTH / 2), g_posWood.y + (WOOD_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posWood.x + (WOOD_WIDTH / 2), g_posWood.y - (WOOD_HEIGHT / 2) + ShaikingMove, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWood->Unlock();
}