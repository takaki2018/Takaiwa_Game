//==============================================================================================================
//
// �J�����w�i (camera_bg.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "camera_bg.h"
#include "camera.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define POS_ADJUST (10.0f)			// ���W�����p

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCameraBg = NULL;		// �J�����w�i�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCameraBg = NULL;			// �J�����w�i�̃e�N�X�`���̃|�C���^

//--------------------------------------------------------------------------------------------------------------
// �J�����w�i�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitCameraBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	Camera pCamera = GetLookDownCamera();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/camera_bg.png", &g_pTextureCameraBg);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffCameraBg,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCameraBg->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3((float)pCamera.viewport.X, (float)pCamera.viewport.Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(pCamera.viewport.X + pCamera.viewport.Width), (float)pCamera.viewport.Y , 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)pCamera.viewport.X, (float)(pCamera.viewport.Y + pCamera.viewport.Height), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(pCamera.viewport.X + pCamera.viewport.Width), (float)(pCamera.viewport.Y + pCamera.viewport.Height), 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCameraBg->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �J�����w�i�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitCameraBg(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffCameraBg != NULL)
	{
		g_pVtxBuffCameraBg->Release();
		g_pVtxBuffCameraBg = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureCameraBg != NULL)
	{
		g_pTextureCameraBg->Release();
		g_pTextureCameraBg = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �J�����w�i�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateCameraBg(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// �J�����w�i�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawCameraBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCameraBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTextureCameraBg);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}