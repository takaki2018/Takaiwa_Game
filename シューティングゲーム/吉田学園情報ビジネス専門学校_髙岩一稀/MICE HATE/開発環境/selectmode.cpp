//-----------------------------------------------------------------
//
// �Z���N�g���[�h (selectmode.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "selectmode.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN			(3)					// �A�j���[�V�����p�^�[��No.�̍ő吔
#define MARKAR_NUMBER		(2)

#define SELECTMODE_X		(400.0f)			// �Z���N�g���[�h�̕�
#define SELECTMODE_Y		(40.0f)				// �Z���N�g���[�h�̍���

#define MARKAR_X			(40.0f)				// �}�[�J�[�̕�
#define MARKAR_Y			(40.0f)				// �}�[�J�[�̍���

#define FLASH_INTERBAL		(10)				// �_�ŃC���^�[�o��
#define CLEAR_NUMBER		(5)					// ����
#define LESS_CLEAR_NUMBER	(0)					// �s����

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectmode = NULL;			// �Z���N�g���[�h�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureSelectmode[MAX_PATTERN] = {};		// �Z���N�g���[�h�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posSelectMode;									// �v���X�G���^�[�̒��S���W
D3DXVECTOR3 g_posMarcar;										// �}�[�J�[�̒��S���W
float g_posY;													// Y���W�̈ړ���
int g_SelectMode;												// ���[�h�J�E���g
D3DXCOLOR g_LogoCol;											// �F		
bool g_PressOK;													// �L�[����������
int g_nCntFlash;												// �J�E���g

//-----------------------------------------------------------------
// �Z���N�g���[�h�̏���������
//-----------------------------------------------------------------
HRESULT InitSelectmode(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posSelectMode = D3DXVECTOR3(SCREEN_WIDTH / 2, 450.0f, 0.0f);
	g_posMarcar = D3DXVECTOR3(SCREEN_WIDTH / 3, 450.0f, 0.0f);
	g_posY = 0.0f;
	g_SelectMode = SELECTMODE_NORMAL;
	g_LogoCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_PressOK = false;
	g_nCntFlash = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Normalmode_logo.png", &g_pTextureSelectmode[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Endlessmode_logo.png", &g_pTextureSelectmode[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/markar.png", &g_pTextureSelectmode[2]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectmode,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLogo = 0; nCntLogo < MAX_PATTERN; nCntLogo++)
	{
		// ���_���W�̐ݒ�
		SetVertexLogo(nCntLogo);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_LogoCol;
		pVtx[1].col = g_LogoCol;
		pVtx[2].col = g_LogoCol;
		pVtx[3].col = g_LogoCol;

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelectmode->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �Z���N�g���[�h�̏I������
//-----------------------------------------------------------------
void UninitSelectmode(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffSelectmode != NULL)
	{
		g_pVtxBuffSelectmode->Release();
		g_pVtxBuffSelectmode = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		if (g_pTextureSelectmode[nCntTexture] != NULL)
		{
			g_pTextureSelectmode[nCntTexture]->Release();
			g_pTextureSelectmode[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �Z���N�g���[�h�̍X�V����
//-----------------------------------------------------------------
void UpdateSelectmode(void)
{
	// �ϐ��錾
	bool *pStick = GetControllerTrigger();

	if (g_PressOK == false)
	{
		if (GetKeyboardTrigger(KEYINFO_UP) == true)
		{// �}�[�J�[����ɓ��������Ƃ�
			// ���[�h�̊m��
			g_SelectMode--;
			g_posY -= 100.0f;

			if (g_SelectMode < SELECTMODE_NORMAL)
			{// ������
				g_SelectMode = SELECTMODE_ENDLESS;
				g_posY = 100.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		else if (GetKeyboardTrigger(KEYINFO_DOWN) == true)
		{// �}�[�J�[�����ɓ��������Ƃ�
			// ���[�h�̊m��
			g_SelectMode++;
			g_posY += 100.0f;

			if (g_SelectMode > SELECTMODE_ENDLESS)
			{// ������
				g_SelectMode = SELECTMODE_NORMAL;
				g_posY = 0.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		// JOYPAD �̏���
		if (pStick[0] == true)
		{// �}�[�J�[����ɓ��������Ƃ�
			// ���[�h�̊m��
			g_SelectMode--;
			g_posY -= 100.0f;

			if (g_SelectMode < SELECTMODE_NORMAL)
			{// ������
				g_SelectMode = SELECTMODE_ENDLESS;
				g_posY = 100.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		else if (pStick[1] == true)
		{// �}�[�J�[����ɓ��������Ƃ�
		 // ���[�h�̊m��
			g_SelectMode++;
			g_posY += 100.0f;

			if (g_SelectMode > SELECTMODE_ENDLESS)
			{// ������
				g_SelectMode = SELECTMODE_NORMAL;
				g_posY = 0.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		// �e���[�h�ɑJ��
		if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
		{// �������̂�true�ɂ���
			g_PressOK = true;

			// �T�E���h
			PlaySound(SOUND_LABEL_SE_OK);
		}
	}
	else if (g_PressOK == true)
	{// �_�ł����邽�߂ɃJ�E���g��i�߂�
		g_nCntFlash++;

		if (CLEAR_NUMBER == g_nCntFlash % FLASH_INTERBAL)
		{// ����
			g_LogoCol.a = 0.0f;
		}
		else if (LESS_CLEAR_NUMBER == g_nCntFlash % FLASH_INTERBAL)
		{// �s����
			g_LogoCol.a = 1.0f;
		}
		// �J�E���g��������x�i�񂾂�J��
		if (g_nCntFlash >= 50)
		{// ���ɐi�񂾂Ƃ�����
			if (g_SelectMode == SELECTMODE_NORMAL)
			{// �Q�[���ɑJ��
				SetFade(FADE_OUT, MODE_LOADSCREEN);
			}
			else if (g_SelectMode == SELECTMODE_ENDLESS)
			{// �Q�[���ɑJ��
				SetFade(FADE_OUT, MODE_LOADSCREEN);
			}
		}
	}
	// ���_���W�̍X�V
	SetVertexLogo(g_SelectMode);
	SetVertexLogo(MARKAR_NUMBER);
}

//-----------------------------------------------------------------
// �Z���N�g���[�h�̕`�揈��
//-----------------------------------------------------------------
void DrawSelectmode(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSelectmode, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureSelectmode[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// �^�C�g����ʂɓ\��e�N�X�`���̎w��
//-----------------------------------------------------------------
void SetVertexLogo(int nCntLogo)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	//	���Ԗڂ�
	pVtx += nCntLogo * 4;

	if (nCntLogo == SELECTMODE_NORMAL)
	{// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2), 0.0f);
	}
	else if (nCntLogo == SELECTMODE_ENDLESS)
	{// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2) + 100, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2) + 100, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2) + 100, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2) + 100, 0.0f);
	}
	else if (nCntLogo == MARKAR_NUMBER)
	{// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(g_posMarcar.x - (MARKAR_X / 2), g_posMarcar.y + (MARKAR_Y / 2) + g_posY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posMarcar.x - (MARKAR_X / 2), g_posMarcar.y - (MARKAR_Y / 2) + g_posY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posMarcar.x + (MARKAR_X / 2), g_posMarcar.y + (MARKAR_Y / 2) + g_posY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posMarcar.x + (MARKAR_X / 2), g_posMarcar.y - (MARKAR_Y / 2) + g_posY, 0.0f);
	}
	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_LogoCol;
	pVtx[1].col = g_LogoCol;
	pVtx[2].col = g_LogoCol;
	pVtx[3].col = g_LogoCol;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelectmode->Unlock();
}

//-----------------------------------------------------------------
// �ǂ̃��[�h��I�񂾂�
//-----------------------------------------------------------------
int GetSelectMode(void)
{
	return g_SelectMode;
}