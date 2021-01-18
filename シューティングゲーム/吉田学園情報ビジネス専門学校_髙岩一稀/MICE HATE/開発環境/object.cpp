//-----------------------------------------------------------------
//
// �I�u�W�F�N�g (object.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "object.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN     (1)										// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(1)										// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)										// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define PLAY_ANIM       ((1.0f / TEXTURE_X) * g_nPatternAnim)   // �e�N�X�`���̓���
#define UV_U			(1.0f / TEXTURE_X + PLAY_ANIM)			// U�̒l
#define UV_V			(1.0f / TEXTURE_Y)						// V�̒l

#define OBJECT_SIZE_X	(10)									// �I�u�W�F�N�g�̕�
#define OBJECT_SIZE_Y	(10)									// �I�u�W�F�N�g�̍���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObject = NULL;	// �I�u�W�F�N�g�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureObject = NULL;			// �I�u�W�F�N�g�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posObject;							// �I�u�W�F�N�g�̒��S���W

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̏���������
//-----------------------------------------------------------------
HRESULT InitObject(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posObject = D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 50.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cheese.png", &g_pTextureObject);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffObject,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffObject->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posObject.x - OBJECT_SIZE_X, g_posObject.y + OBJECT_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posObject.x - OBJECT_SIZE_X, g_posObject.y - OBJECT_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posObject.x + OBJECT_SIZE_X, g_posObject.y + OBJECT_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posObject.x + OBJECT_SIZE_X, g_posObject.y - OBJECT_SIZE_Y, 0.0f);

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
	g_pVtxBuffObject->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̏I������
//-----------------------------------------------------------------
void UninitObject(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffObject != NULL)
	{
		g_pVtxBuffObject->Release();
		g_pVtxBuffObject = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureObject != NULL)
	{
		g_pTextureObject->Release();
		g_pTextureObject = NULL;
	}
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̍X�V����
//-----------------------------------------------------------------
void UpdateObject(void)
{

}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̕`�揈��
//-----------------------------------------------------------------
void DrawObject(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffObject, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureObject);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̈ʒu���̎擾
//-----------------------------------------------------------------
D3DXVECTOR3 GetposObject(void)
{
	return g_posObject;
}