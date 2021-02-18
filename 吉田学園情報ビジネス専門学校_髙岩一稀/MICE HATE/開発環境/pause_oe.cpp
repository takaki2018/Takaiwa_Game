//-----------------------------------------------------------------
//
// �|�[�YOE (pause_oe.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "pause_oe.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN     (4)			// �g���e�N�X�`���̍ő吔
#define MAX_TEXTURE		(4)
#define MAX_SELECT		(4)			// �}�[�J�[�̍ő�l

#define IMAGE_X			(850.0f)	// �摜�̒��S���W(X)
#define IMAGE_Y			(390.0f)	// �摜�̒��S���W(Y)
#define IMAGE_WIDTH 	(768.0f)	// �摜�̕�
#define IMAGE_HEIGHT	(432.0f)	// �摜�̍���

#define SELECT_X		(800.0f)	// �}�[�J�[�̒��S���W(X)
#define SELECT_Y		(660.0f)	// �}�[�J�[�̒��S���W(Y)
#define SELECT_SIZE 	(20.0f)		// �}�[�J�[�̃T�C�Y
#define SELECT_WIDTH	(50.0f)		// �}�[�J�[�̊Ԋu

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseOe = NULL;			// ���[�h��ʂ̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePauseOe[MAX_TEXTURE] = {};		// ���[�h��ʂ̃e�N�X�`���̃|�C���^
int g_nCountSelect;											// ���ڂ�����
int g_nCountImage;											// ���Ԗڂ̉摜���o����

//-----------------------------------------------------------------
// �|�[�YOE�̏���������
//-----------------------------------------------------------------
HRESULT InitPauseOe(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_nCountSelect = 0;
	g_nCountImage = 1;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTexturePauseOe[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe001.png", &g_pTexturePauseOe[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe002.png", &g_pTexturePauseOe[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_se.png", &g_pTexturePauseOe[3]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseOe,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPauseOe->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// ���_���W�̐ݒ�
		SetVertexPauseOe(nCnt);

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
	g_pVtxBuffPauseOe->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �|�[�YOE�̏I������
//-----------------------------------------------------------------
void UninitPauseOe(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPauseOe != NULL)
	{
		g_pVtxBuffPauseOe->Release();
		g_pVtxBuffPauseOe = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTexturePauseOe[nCnt] != NULL)
		{
			g_pTexturePauseOe[nCnt]->Release();
			g_pTexturePauseOe[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �|�[�YOE�̍X�V����
//-----------------------------------------------------------------
void UpdatePauseOe(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	bool *pStick = GetControllerTrigger();

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	// �|�[�Y���j���[���J�����Ƃ�����
	if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(JPINFO_PAUSE) == true)
	{
		g_nCountSelect = 0;
		g_nCountImage = 1;
	}
	// �}�[�J�[�̍X�V
	if (GetKeyboardTrigger(KEYINFO_RIGHT) == true)
	{
		if (g_nCountSelect != 2)
		{
			g_nCountSelect++;		// �}�[�J�[�̈ړ��l�̑���
			g_nCountImage++;		// �摜�����̂��̂Ɉڂ�

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	else if (GetKeyboardTrigger(KEYINFO_LEFT) == true)
	{// ���Ɉړ�����Ƃ�
		if (g_nCountSelect != 0)
		{
			g_nCountSelect--;		// �}�[�J�[�̈ړ��l�̌���
			g_nCountImage--;		// �摜��O�̂��̂Ɉڂ�

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	// JOYPAD �̑���
	if (pStick[2] == true)
	{
		if (g_nCountSelect != 2)
		{
			g_nCountSelect++;		// �}�[�J�[�̈ړ��l�̑���
			g_nCountImage++;		// �摜�����̂��̂Ɉڂ�

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	else if (pStick[3] == true)
	{// ���Ɉړ�����Ƃ�
		if (g_nCountSelect != 0)
		{
			g_nCountSelect--;		// �}�[�J�[�̈ړ��l�̌���
			g_nCountImage--;		// �摜��O�̂��̂Ɉڂ�

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	// ���_���W�̍X�V
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		SetVertexPauseOe(nCnt);
	}
}

//-----------------------------------------------------------------
// �|�[�YOE�̕`�揈��
//-----------------------------------------------------------------
void DrawPauseOe(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPauseOe, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (nCnt <= 2)
		{// �}�[�J�[
			pDevice->SetTexture(0, g_pTexturePauseOe[0]);
		}
		else if (nCnt == 3)
		{// �摜
			pDevice->SetTexture(0, g_pTexturePauseOe[g_nCountImage]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// �|�[�YOE�̒��_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexPauseOe(int nCnt)
{
	if (nCnt <= 2)
	{// �}�[�J�[�̐ݒ�
		PauseOeSelect(nCnt);
	}
	else if (nCnt == 3)
	{// �摜�̐ݒ�
		PauseOeImage(nCnt);
	}
}

//-----------------------------------------------------------------
// �|�[�YOE�̉摜
//-----------------------------------------------------------------
void PauseOeImage(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPauseOe->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ��m��
	pVtx += nCnt * 4;

	// �w�i�̍X�V
	pVtx[0].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPauseOe->Unlock();
}

//-----------------------------------------------------------------
// �|�[�YOE�̃}�[�J�[
//-----------------------------------------------------------------
void PauseOeSelect(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPauseOe->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ��m��
	pVtx += nCnt * 4;

	// �Ԋu�̊m��
	float nWidth = (nCnt % MAX_SELECT) * SELECT_WIDTH;

	// �w�i�̍X�V
	pVtx[0].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);

	// ���_�J���[�̐ݒ�
	if (nCnt == g_nCountSelect)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPauseOe->Unlock();
}