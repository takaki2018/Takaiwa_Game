//==============================================================================================================
//
// �`���[�g���A���摜 (tutorial_image.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_image.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define IMAGE_X					(SCREEN_WIDTH / 2)		// �摜�̒��S���W(X)
#define IMAGE_Y					(340.0f)				// �摜�̒��S���W(Y)
#define IMAGE_WIDTH 			(960.0f)				// �摜�̕�
#define IMAGE_HEIGHT			(540.0f)				// �摜�̍���

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialImage = NULL;					// �`���[�g���A���摜�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTutorialImage[MAX_TUTORIAL_IMAGE] = {};	// �`���[�g���A���摜�̃e�N�X�`���̃|�C���^
int g_nPointImage;														// �`���[�g���A���摜�̃y�[�W�ԍ�

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���摜�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialImage(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_nPointImage = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe001.png", &g_pTextureTutorialImage[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe002.png", &g_pTextureTutorialImage[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_se.png", &g_pTextureTutorialImage[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_ge.png", &g_pTextureTutorialImage[3]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TUTORIAL_IMAGE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,										// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialImage,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialImage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TUTORIAL_IMAGE; nCnt++)
	{
		// �w�i�̍X�V
		pVtx[0].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);

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

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialImage->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���摜�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialImage(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorialImage != NULL)
	{
		g_pVtxBuffTutorialImage->Release();
		g_pVtxBuffTutorialImage = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TUTORIAL_IMAGE; nCntTexture++)
	{
		if (g_pTextureTutorialImage[nCntTexture] != NULL)
		{
			g_pTextureTutorialImage[nCntTexture]->Release();
			g_pTextureTutorialImage[nCntTexture] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���摜�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialImage(void)
{
	// �ϐ��錾
	bool *pStick = GetStickTrigger(PLAYER_1);
	bool *pCrossKey = GetCrossKeyTrigger(PLAYER_1);

	// �}�[�J�[�̍X�V
	// JOYPAD �̏���
	if (pStick[STICKTRIGGER_RIGHT] == true ||
		pCrossKey[CROSSKEYPRESS_RIGHT] == true ||
		GetKeyboardTrigger(KEYINFO_RIGHT) == true ||
		GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{// �E�Ɉړ�����Ƃ�
		if (g_nPointImage != MAX_TUTORIAL_IMAGE - 1)
		{
			g_nPointImage++;		// // �摜�ԍ��𑝂₷

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	if (pStick[STICKTRIGGER_LEFT] == true ||
		pCrossKey[CROSSKEYPRESS_LEFT] == true ||
		GetKeyboardTrigger(KEYINFO_LEFT) == true ||
		GetJoypadTrigger(PLAYER_1, JPINFO_CANSEL) == true)
	{// ���Ɉړ�����Ƃ�
		if (g_nPointImage != 0)
		{
			g_nPointImage--;		// �摜�ԍ������炷

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���摜�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialImage(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialImage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTextureTutorialImage[g_nPointImage]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���摜�����Ԗڂ��擾
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fint	- ���Ԗڂ̉摜���I������Ă��邩�Ԃ�
//--------------------------------------------------------------------------------------------------------------
int GetPointImage(void)
{
	return g_nPointImage;
}