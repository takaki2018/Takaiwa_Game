//-----------------------------------------------------------------
//
// �|�[�Y (pause.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "pause.h"
#include "loadscreen.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN				(6)					// �A�j���[�V�����p�^�[��No.�̍ő吔

#define	PAUSE_MENU_LOGO_X		(SCREEN_WIDTH / 2)	// �|�[�Y���j���[���S�̒��S���WX
#define PAUSE_MENU_LOGO_Y		(80.0f)				// �|�[�Y���j���[���S�̒��S���WY
#define PAUSE_MENU_LOGO_WIDTH	(280.0f)			// �|�[�Y���j���[���S�̕�
#define PAUSE_MENU_LOGO_HEIGHT	(50.0f)				// �|�[�Y���j���[���S�̍���

#define PAUSE_MENU_BG_X			(250.0f)			// �|�[�Y���j���[�̒��S���WX
#define PAUSE_MENU_BG_Y			(390.0f)			// �|�[�Y���j���[�̒��S���WY
#define PAUSE_MENU_BG_WIDTH		(330.0f)			// �|�[�Y���j���[�̕�
#define PAUSE_MENU_BG_HEIGHT	(330.0f)			// �|�[�Y���j���[�̍���

#define LOGO_X					(250.0f)			// ���S�̒��S���WX(���ׂċ���)
#define LOGO_Y					(310.0f)			// ���S�̒��S���WY(���ׂċ���)
#define LOGO_WIDTH				(250.0f)			// ���S�̕�(���ׂċ���)
#define LOGO_HEIGHT				(50.0f)				// ���S�̍���(���ׂċ���)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			// �|�[�Y��ʂ̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_PATTERN] = {};	// �|�[�Y��ʂ̃e�N�X�`���̃|�C���^
D3DXCOLOR g_Pausecol;									// �|�[�Y��ʂ̐F
int g_nPauseCnt;										// �ǂ̃��[�h��I��ł��邩

//-----------------------------------------------------------------
// �|�[�Y�̏���������
//-----------------------------------------------------------------
HRESULT InitPause(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_nPauseCnt = PAUSE_MENU_CONTINUE;
	g_Pausecol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_menu_logo.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_menu_bg.png", &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/continue_logo.png", &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/retry_logo.png", &g_pTexturePause[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/quit_logo.png", &g_pTexturePause[5]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < MAX_PATTERN; nCntPause++)
	{
		// ���_���W�̐ݒ�
		SetVertexPause(nCntPause);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �|�[�Y�̏I������
//-----------------------------------------------------------------
void UninitPause(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntPause = 0; nCntPause < MAX_PATTERN; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �|�[�Y�̍X�V����
//-----------------------------------------------------------------
void UpdatePause(void)
{
	// �ϐ��錾
	bool *pStick = GetControllerTrigger();

	if (GetKeyboardTrigger(KEYINFO_UP) == true)
	{// ���̏��̊m��
		g_nPauseCnt--;

		if (g_nPauseCnt < PAUSE_MENU_CONTINUE)
		{// 0��菬�����Ȃ����Ƃ��X�V
			g_nPauseCnt = PAUSE_MENU_QUIT;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}
	if (GetKeyboardTrigger(KEYINFO_DOWN) == true)
	{// ���̏��̊m��
		g_nPauseCnt++;

		if (g_nPauseCnt > PAUSE_MENU_QUIT)
		{// 2���傫���Ȃ����Ƃ��X�V
			g_nPauseCnt = PAUSE_MENU_CONTINUE;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}
	// JOYPAD �̏���
	if (pStick[0] == true)
	{// ���̏��̊m��
		g_nPauseCnt--;

		if (g_nPauseCnt < PAUSE_MENU_CONTINUE)
		{// 0��菬�����Ȃ����Ƃ��X�V
			g_nPauseCnt = PAUSE_MENU_QUIT;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}
	if (pStick[1] == true)
	{// ���̏��̊m��
		g_nPauseCnt++;

		if (g_nPauseCnt > PAUSE_MENU_QUIT)
		{// 2���傫���Ȃ����Ƃ��X�V
			g_nPauseCnt = PAUSE_MENU_CONTINUE;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}
	// �ǂ̏��ŉ�������
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
	{
		if (g_nPauseCnt == PAUSE_MENU_CONTINUE)
		{// �R���e�B�j���[
			SetPause(false);
			// SE
			PlaySound(SOUND_LABEL_SE_PAUSE_OUT);
		}
		else if (g_nPauseCnt == PAUSE_MENU_RETRY)
		{// ���g���C
			SetFade(FADE_OUT, MODE_GAME);
			// SE
			PlaySound(SOUND_LABEL_SE_OK);
		}
		else if (g_nPauseCnt == PAUSE_MENU_QUIT)
		{// �N�C�b�g
			SetFade(FADE_OUT, MODE_TITLE);
			// SE
			PlaySound(SOUND_LABEL_SE_OK);
		}
	}
	else if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(JPINFO_PAUSE) == true)
	{// �R���e�B�j���[�ɏ�����
		g_nPauseCnt = PAUSE_MENU_CONTINUE;
	}
	// ���_���W�̍X�V
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		SetVertexPause(nCnt);
	}
}

//-----------------------------------------------------------------
// �|�[�Y�̕`�揈��
//-----------------------------------------------------------------
void DrawPause(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntPause = 0; nCntPause < MAX_PATTERN; nCntPause++)
	{
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//-----------------------------------------------------------------
// �|�[�Y�̒��_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexPause(int nCntPause)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCntPause * 4;
	if (nCntPause == 0)
	{// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	}
	else if (nCntPause == 1)
	{// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(PAUSE_MENU_LOGO_X - PAUSE_MENU_LOGO_WIDTH / 2, PAUSE_MENU_LOGO_Y + PAUSE_MENU_LOGO_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_MENU_LOGO_X - PAUSE_MENU_LOGO_WIDTH / 2, PAUSE_MENU_LOGO_Y - PAUSE_MENU_LOGO_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_MENU_LOGO_X + PAUSE_MENU_LOGO_WIDTH / 2, PAUSE_MENU_LOGO_Y + PAUSE_MENU_LOGO_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_MENU_LOGO_X + PAUSE_MENU_LOGO_WIDTH / 2, PAUSE_MENU_LOGO_Y - PAUSE_MENU_LOGO_HEIGHT / 2, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	}
	else if (nCntPause == 2)
	{// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(PAUSE_MENU_BG_X - PAUSE_MENU_BG_WIDTH / 2, PAUSE_MENU_BG_Y + PAUSE_MENU_BG_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_MENU_BG_X - PAUSE_MENU_BG_WIDTH / 2, PAUSE_MENU_BG_Y - PAUSE_MENU_BG_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_MENU_BG_X + PAUSE_MENU_BG_WIDTH / 2, PAUSE_MENU_BG_Y + PAUSE_MENU_BG_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_MENU_BG_X + PAUSE_MENU_BG_WIDTH / 2, PAUSE_MENU_BG_Y - PAUSE_MENU_BG_HEIGHT / 2, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntPause >= 3 && nCntPause < 6)
	{
		PauseMenu(nCntPause);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//-----------------------------------------------------------------
// �|�[�Y���j���[
//-----------------------------------------------------------------
void PauseMenu(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// �ϐ��錾
	float fPosY = 80;

	pVtx += 4 * nCnt;
	fPosY *= nCnt - 3;

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(LOGO_X - (LOGO_WIDTH / 2), LOGO_Y + fPosY + (LOGO_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(LOGO_X - (LOGO_WIDTH / 2), LOGO_Y + fPosY - (LOGO_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(LOGO_X + (LOGO_WIDTH / 2), LOGO_Y + fPosY + (LOGO_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(LOGO_X + (LOGO_WIDTH / 2), LOGO_Y + fPosY - (LOGO_HEIGHT / 2), 0.0f);

	if (g_nPauseCnt == (nCnt - 3) % PAUSE_MENU_MAX)
	{// �I�΂�Ă���Ƃ��s�����ɂ���
		g_Pausecol.a = 1.0f;
	}
	else 
	{// �I�΂�Ă��Ȃ��Ƃ��������ɂ���
		g_Pausecol.a = 0.5f;
	}
	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_Pausecol;
	pVtx[1].col = g_Pausecol;
	pVtx[2].col = g_Pausecol;
	pVtx[3].col = g_Pausecol;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}