//-----------------------------------------------------------------
//
// �A�C�e�� (item.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "item.h"
#include "shockwave.h"
#include "object.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN	(3)						// �A�j���[�V�����p�^�[��No.�̍ő吔

#define ITEM_SIZE_X	(20)					// �A�C�e���̕�
#define ITEM_SIZE_Y	(20)					// �A�C�e���̍���
#define ITEM_WIDTH	(50)					// �A�C�e�����u����镝

#define MAX_ITEM	(MAX_PATTERN)			// �A�C�e���̍ő吔
//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// �A�C�e���̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureItem = NULL;		// �A�C�e���̃e�N�X�`���Ƀ|�C���^
D3DXVECTOR3 g_posItem;							// �I�u�W�F�N�g�̒��S���W
bool g_bUseItem;								// �A�C�e�����g�p�����ǂ���
int g_nCntItem;									// �A�C�e����

//-----------------------------------------------------------------
// �A�C�e���̏���������
//-----------------------------------------------------------------
HRESULT InitItem(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posItem = D3DXVECTOR3(ITEM_SIZE_X + 10, ITEM_SIZE_Y + 10, 0.0f);
	g_bUseItem = false;
	g_nCntItem = MAX_ITEM;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cheese.png", &g_pTextureItem);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_PATTERN; nCntItem++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y + ITEM_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y - ITEM_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y + ITEM_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y - ITEM_SIZE_Y, 0.0f);

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
	g_pVtxBuffItem->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �A�C�e���̏I������
//-----------------------------------------------------------------
void UninitItem(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}
}

//-----------------------------------------------------------------
// �A�C�e���̍X�V����
//-----------------------------------------------------------------
void UpdateItem(void)
{
	// �ϐ��錾
	D3DXVECTOR3 posObject = GetposObject();

	// �A�C�e���̎g�p�ł��邩
	if (g_nCntItem != 0 && g_bUseItem != true)
	{
		if (GetKeyboardTrigger(KEYINFO_ITEM) == true || GetJoypadTrigger(JPINFO_ITEM) == true)
		{// �T�E���h
			PlaySound(SOUND_LABEL_SE_ITEM);
			
			// �A�C�e���̎g�p
			g_nCntItem = (g_nCntItem - 1) % MAX_ITEM;	// �A�C�e�����̃J�E���g
			g_bUseItem = true;							// �A�C�e���̎g�p

			// �Ռ��g�̔z�u
			SetShockwave(posObject,D3DXVECTOR3(10.0f,10.0f,10.0f) ,
				D3DXCOLOR(0.5f, 0.5f, 0.1f, 1.0f), 8.0f, 50.0f,0.9f, 0.02f);

			SetShockwave(posObject, D3DXVECTOR3(10.0f, 10.0f, 10.0f),
				D3DXCOLOR(0.5f, 0.5f, 0.1f, 1.0f), 5.0f, 0.0f,0.0f, 0.05f);
		}
	}
}

//-----------------------------------------------------------------
// �A�C�e���̕`�揈��
//-----------------------------------------------------------------
void DrawItem(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntItem = 0; nCntItem < g_nCntItem; nCntItem++)
	{
		pDevice->SetTexture(0, g_pTextureItem);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
	}
}

//-----------------------------------------------------------------
// �A�C�e���̎g�p����
//-----------------------------------------------------------------
bool GetbUseItem(void)
{
	return g_bUseItem;
}

//-----------------------------------------------------------------
// �A�C�e���̎g�p����
//-----------------------------------------------------------------
void SetItem(bool bUseItem)
{
	g_bUseItem = bUseItem;
}
