//-----------------------------------------------------------------
//
// �e�̐F�m�F (bulletcol.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bulletcol.h"
#include "bullet.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define BULLETCOL_X			(300.0f)				// �e�̐F�m�F�p�̃e�N�X�`���̒��S���W(X)
#define BULLETCOL_Y			(75.0f)					// �e�̐F�m�F�p�̃e�N�X�`���̒��S���W(Y)

#define BULLETCOL_SIZE		(15.0f)					// �e�̐F�m�F�p�̃e�N�X�`���̃T�C�Y

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletcol = NULL;	// ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBulletcol = NULL;		// �e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posBulletcol;							// ���S���W
int g_nCntCol;										// �e�̐F�J�E���g
D3DXCOLOR g_Bulletcol;								// �F�̐ݒ�

//-----------------------------------------------------------------
// �e�̐F�m�F�̏���������
//-----------------------------------------------------------------
HRESULT InitBulletcol(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posBulletcol = D3DXVECTOR3(BULLETCOL_X, BULLETCOL_Y,0.0f );
	g_nCntCol = 0;
	g_Bulletcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBulletcol);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletcol,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletcol->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posBulletcol.x - BULLETCOL_SIZE, g_posBulletcol.y + BULLETCOL_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posBulletcol.x - BULLETCOL_SIZE, g_posBulletcol.y - BULLETCOL_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posBulletcol.x + BULLETCOL_SIZE, g_posBulletcol.y + BULLETCOL_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posBulletcol.x + BULLETCOL_SIZE, g_posBulletcol.y - BULLETCOL_SIZE, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_Bulletcol;
	pVtx[1].col = g_Bulletcol;
	pVtx[2].col = g_Bulletcol;
	pVtx[3].col = g_Bulletcol;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletcol->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �e�̐F�m�F�̏I������
//-----------------------------------------------------------------
void UninitBulletcol(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBulletcol != NULL)
	{
		g_pVtxBuffBulletcol->Release();
		g_pVtxBuffBulletcol = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureBulletcol != NULL)
	{
		g_pTextureBulletcol->Release();
		g_pTextureBulletcol = NULL;
	}
}

//-----------------------------------------------------------------
// �e�̐F�m�F�̍X�V����
//-----------------------------------------------------------------
void UpdateBulletcol(void)
{
	// �e�̐F�̐؂�ւ�
	if (GetKeyboardTrigger(KEYINFO_COLOR_CHANGE) == true || GetJoypadTrigger(JPINFO_COLOR_CHANGE) == true)
	{
		g_nCntCol = (g_nCntCol + 1) % BULLETCOLOR_MAX;
	}

	// �F�̊m��
	if (g_nCntCol == BULLETCOLOR_WHITE)
	{// ���ɐݒ�
		g_Bulletcol = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.9f);
	}
	else if (g_nCntCol == BULLETCOLOR_RED)
	{// �Ԃɐݒ�
		g_Bulletcol = D3DXCOLOR(0.9f, 0.3f, 0.1f, 0.9f);
	}
	else if (g_nCntCol == BULLETCOLOR_BLUE)
	{// �ɐݒ�
		g_Bulletcol = D3DXCOLOR(0.1f, 0.3f, 0.9f, 0.9f);
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletcol->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_Bulletcol;
	pVtx[1].col = g_Bulletcol;
	pVtx[2].col = g_Bulletcol;
	pVtx[3].col = g_Bulletcol;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletcol->Unlock();
}

//-----------------------------------------------------------------
// �e�̐F�m�F�̕`�揈��
//-----------------------------------------------------------------
void DrawBulletcol(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBulletcol, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTextureBulletcol);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}