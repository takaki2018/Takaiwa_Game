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
#define MAX_MODE					(2)					// �A�j���[�V�����p�^�[��No.�̍ő吔

#define SELECTMODE_X				(SCREEN_WIDTH / 2)	// �Z���N�g���[�h�̒��S���W(X)
#define SELECTMODE_Y				(410.0f)			// �Z���N�g���[�h�̒��S���W(Y)
#define SELECTMODE_WIDTH			(400.0f)			// �Z���N�g���[�h�̕�
#define SELECTMODE_HEIGHT			(70.0f)				// �Z���N�g���[�h�̍���

#define SELECTMODE_SCALE_X			(0.6f)				// �Z���N�g���[�h�̊g��k���l(X)
#define SELECTMODE_SCALE_Y			(0.14f)				// �Z���N�g���[�h�̊g��k���l(Y)
#define SELECTMODE_CHANGE_COLOR_B	(0.01f)				// �Z���N�g���[�h�̐F�ω��l(B)

#define FLASH_INTERBAL_MODELOGO		(10)				// �_�ŃC���^�[�o��
#define CLEAR_NUMBER_MODELOGO		(5)					// ����
#define LESS_CLEAR_NUMBER_MODELOGO	(0)					// �s����

#define TURN_TEXT_RIGHT				(70)				// �e�L�X�g�̉E�X���J�E���g
#define TURN_TEXT_LEFT				(140)				// �e�L�X�g�̍��X���J�E���g
#define FLASH_COUNT					(50)				// �t���b�V���J�E���g

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectmode = NULL;			// �Z���N�g���[�h�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureSelectmode[MAX_MODE] = {};			// �Z���N�g���[�h�̃e�N�X�`���̃|�C���^
SELECTMODE g_aSelectMode[MAX_MODE];								// �Z���N�g���[�h�̏��
D3DXCOLOR g_LogoCol;											// �F
PLAYMODE g_playmode;											// �v���C���[�h
int g_nCntMode;													// ���[�h�J�E���g
bool g_bPressOK;												// �L�[����������
int g_nCntScale;												// �g��k���J�E���g
int g_nCntFlash;												// �_�ŃJ�E���g

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
	for (int nCntSM = 0; nCntSM < MAX_MODE; nCntSM++)
	{
		g_aSelectMode[nCntSM].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SELECTMODE_Y + 100.0f * nCntSM, 0.0f);
		g_aSelectMode[nCntSM].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSelectMode[nCntSM].fWidth = SELECTMODE_WIDTH;
		g_aSelectMode[nCntSM].fHeight = SELECTMODE_HEIGHT;
	}
	g_playmode = PLAYMODE_SINGLE;
	g_nCntMode = (int)g_playmode;
	g_bPressOK = false;
	g_nCntScale = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/play_single.png", &g_pTextureSelectmode[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/play_multi.png", &g_pTextureSelectmode[1]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_MODE, // �m�ۂ���o�b�t�@�T�C�Y
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

	for (int nCntLogo = 0; nCntLogo < MAX_MODE; nCntLogo++)
	{
		// ���_���W�̐ݒ�
		SetVertexLogo(nCntLogo);

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
	for (int nCntTexture = 0; nCntTexture < MAX_MODE; nCntTexture++)
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
	// �ǂ̃��[�h��I�����Ă��邩
	g_playmode = (PLAYMODE)g_nCntMode;

	switch (g_bPressOK)
	{// ���[�h�I��
	case false:
		SelectMode();
		break;
		// ���[�h�I������
	case true:
		DecideMode();
		break;
	}
	// ���_���W�̍X�V
	for (int nCntMode = 0; nCntMode < MAX_MODE; nCntMode++)
	{
		SetVertexLogo(nCntMode);
	}
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
	for (int nCntTexture = 0; nCntTexture < MAX_MODE; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureSelectmode[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// ���[�h�I��
//-----------------------------------------------------------------
void SelectMode(void)
{
	// �ϐ��錾
	bool *pStickTrigger = GetStickTrigger(PLAYER_1);
	bool *pCrossKeyTrigger = GetCrossKeyTrigger(PLAYER_1);

	// �g��k�����J�E���g
	g_nCntScale++;

	// JOYPAD �̏���
	if (pStickTrigger[STICKTRIGGER_UP] == true || 
		pCrossKeyTrigger[CROSSKEYPRESS_UP] == true ||
		GetKeyboardTrigger(KEYINFO_UP) == true)
	{// �X�e�B�b�N����ɓ|�����Ƃ�
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;		// ���̏�����
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;		// �����̏�����
		g_aSelectMode[g_nCntMode].col.b = 1.0f;						// �����̏�����
		g_nCntScale = 0;											// �J�E���g�̏�����
		g_nCntMode--;												// ���[�h�̊m��

		if (g_nCntMode < (int)PLAYMODE_SINGLE)
		{// ������
			g_nCntMode = (int)PLAYMODE_MULTI;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}
	else if (pStickTrigger[STICKTRIGGER_DOWN] == true || 
		pCrossKeyTrigger[CROSSKEYPRESS_DOWN] == true ||
		GetKeyboardTrigger(KEYINFO_DOWN) == true)
	{// �X�e�B�b�N�����ɓ|�����Ƃ�
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;		// ���̏�����
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;		// �����̏�����
		g_aSelectMode[g_nCntMode].col.b = 1.0f;						// �����̏�����
		g_nCntScale = 0;											// �J�E���g�̏�����
		g_nCntMode++;												// ���[�h�̊m��

		if (g_nCntMode > (int)PLAYMODE_MULTI)
		{// ������
			g_nCntMode = (int)PLAYMODE_SINGLE;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}

	if (0 <= g_nCntScale && TURN_TEXT_RIGHT > g_nCntScale)
	{// �傫���ƐF�̕ύX
		g_aSelectMode[g_nCntMode].fWidth += SELECTMODE_SCALE_X;
		g_aSelectMode[g_nCntMode].fHeight += SELECTMODE_SCALE_Y;
		g_aSelectMode[g_nCntMode].col.b -= SELECTMODE_CHANGE_COLOR_B;
	}
	else if (TURN_TEXT_RIGHT <= g_nCntScale && TURN_TEXT_LEFT > g_nCntScale)
	{// �傫���ƐF�̕ύX
		g_aSelectMode[g_nCntMode].fWidth -= SELECTMODE_SCALE_X;
		g_aSelectMode[g_nCntMode].fHeight -= SELECTMODE_SCALE_Y;
		g_aSelectMode[g_nCntMode].col.b += SELECTMODE_CHANGE_COLOR_B;
	}
	else if (g_nCntScale == TURN_TEXT_LEFT)
	{// �J�E���g��140�ɂȂ����Ƃ����ɖ߂�
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;
		g_aSelectMode[g_nCntMode].col.b = 1.0f;
		g_nCntScale = 0;
	}
	// �e���[�h�ɑJ��
	if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true || 
		GetKeyboardTrigger(KEYINFO_OK) == true)
	{// �������̂�true�ɂ���
		g_bPressOK = true;

		// �I���������[�h�̃��S�̏�Ԃ�������
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;
		g_aSelectMode[g_nCntMode].col.b = 1.0f;

		// �T�E���h
		PlaySound(SOUND_LABEL_SE_OK);
	}
}

//-----------------------------------------------------------------
// ���[�h�I������
//-----------------------------------------------------------------
void DecideMode(void)
{
	// �_�ł����邽�߂ɃJ�E���g��i�߂�
	g_nCntFlash++;

	if (CLEAR_NUMBER_MODELOGO == g_nCntFlash % FLASH_INTERBAL_MODELOGO)
	{// ����
		g_aSelectMode[g_nCntMode].col.a = 0.0f;
	}
	else if (LESS_CLEAR_NUMBER_MODELOGO == g_nCntFlash % FLASH_INTERBAL_MODELOGO)
	{// �s����
		g_aSelectMode[g_nCntMode].col.a = 1.0f;
	}
	// �J�E���g��������x�i�񂾂�J��
	if (g_nCntFlash >= FLASH_COUNT)
	{// ���ɐi�񂾂Ƃ�����
	 // �Q�[���ɑJ��
		SetFade(FADE_OUT, MODE_LOADSCREEN);
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexLogo(int nCntLogo)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	SELECTMODE *pSelectMode = &g_aSelectMode[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	//	���Ԗڂ�
	pVtx += nCntLogo * 4;
	pSelectMode += nCntLogo;

	if (nCntLogo == (int)PLAYMODE_SINGLE)
	{// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
	}
	else if (nCntLogo == (int)PLAYMODE_MULTI)
	{// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
	}

	// ���_�J���[�̐ݒ�
	pVtx[0].col = pSelectMode->col;
	pVtx[1].col = pSelectMode->col;
	pVtx[2].col = pSelectMode->col;
	pVtx[3].col = pSelectMode->col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelectmode->Unlock();
}

//-----------------------------------------------------------------
// �ǂ̃��[�h��I�񂾂�
//-----------------------------------------------------------------
PLAYMODE GetPlayMode(void)
{
	return g_playmode;
}