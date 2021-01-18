//-----------------------------------------------------------------
//
// �^�C�g�����S (titlelogo.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "titlelogo.h"
#include "selectmode.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN     (2)							// �A�j���[�V�����p�^�[��No.�̍ő吔

#define TITLELOGO_X		(800.0f)					// �^�C�g�����S��
#define TITLELOGO_Y		(200.0f)					// �^�C�g�����S�̍���
#define HEIGHT_Y		(100.0f * -1.0f)			// �^�C�g�����S�̏������SY���W

#define MOVE_TITLELOGO	(5.0f)						// �^�C�g�����S�̈ړ���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;		// �^�C�g�����S�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;			// �^�C�g�����S�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posTitleLogo;								// �^�C�g�����S�̒��S���W
D3DXCOLOR g_color;										// �F
bool g_bFall;											// �^�C�g�����S�����S�ɗ�����������

//-----------------------------------------------------------------
// �^�C�g�����S�̏���������
//-----------------------------------------------------------------
HRESULT InitTitlelogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posTitleLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_bFall = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo.png", &g_pTextureTitleLogo);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2),g_posTitleLogo.y + (TITLELOGO_Y / 2),0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2),g_posTitleLogo.y - (TITLELOGO_Y / 2),0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2),g_posTitleLogo.y + (TITLELOGO_Y / 2),0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2),g_posTitleLogo.y - (TITLELOGO_Y / 2),0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_color;
	pVtx[1].col = g_color;
	pVtx[2].col = g_color;
	pVtx[3].col = g_color;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �^�C�g�����S�̏I������
//-----------------------------------------------------------------
void UninitTitlelogo(void)
{
	// �Z���N�g���[�h�̏I������
	UninitSelectmode();

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//-----------------------------------------------------------------
// �^�C�g�����S�̍X�V����
//-----------------------------------------------------------------
void UpdateTitlelogo(void)
{
	// �t�F�[�h���̎擾
	int nFade = GetFade();

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
	{// �G���^�[�L�[���������Ƃ�
		if (nFade == FADE_IN)
		{// �t�F�[�h�C���̎�
			SetFade(FADE_NONE, MODE_TITLE);
			g_posTitleLogo.y = SCREEN_HEIGHT / 4;
			g_bFall = true;
		}
		else if (g_posTitleLogo.y <= (SCREEN_HEIGHT / 4))
		{// �^�C�g�����S�������Ă��Ă���Ƃ�
			g_posTitleLogo.y = SCREEN_HEIGHT / 4;
			g_bFall = true;
		}
	}
	// �X�V����
	if (nFade == FADE_NONE && g_posTitleLogo.y != (SCREEN_HEIGHT / 4))
	{// �^�C�g�����S�̈ړ��ʂ̍X�V
		g_posTitleLogo.y += MOVE_TITLELOGO;
	}
	else if (g_posTitleLogo.y >= (SCREEN_HEIGHT / 4))
	{// �^�C�g�����S�������Ă��Ă���Ƃ�
		g_posTitleLogo.y = SCREEN_HEIGHT / 4;
		g_bFall = true;
	}

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();
}

//-----------------------------------------------------------------
// �^�C�g�����S�̕`�揈��
//-----------------------------------------------------------------
void DrawTitlelogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTextureTitleLogo);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// �^�C�g�����S���̎擾
//-----------------------------------------------------------------
bool GetFall(void)
{
	return g_bFall;
}