//-----------------------------------------------------------------
//
// ���[�h��� (loadscreen.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "loadscreen.h"
#include "input.h"
#include "gamepad.h"
#include "selectmode.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN     (9)							// �g���e�N�X�`���̍ő吔
#define MAX_TEXTURE		(9)
#define MAX_BG			(3)							// �w�i�̍ő吔
#define MAX_SELECT		(4)							// �}�[�J�[�̍ő�l
#define MAX_IMAGE		(4)							// �摜�̍ő吔

#define MOVE_ALUFA		(0.02f)						// ���l�̈ړ���
#define MOVE_BG			(0.002f)					// ���[�h��ʂ̈ړ���

#define IMAGE_X			(SCREEN_WIDTH / 2)			// �摜�̒��S���W(X)
#define IMAGE_Y			(340.0f)					// �摜�̒��S���W(Y)
#define IMAGE_WIDTH 	(SCREEN_WIDTH / 4 * 3)		// �摜�̕�
#define IMAGE_HEIGHT	(SCREEN_HEIGHT / 4 * 3)		// �摜�̍���

#define SELECT_X		(560.0f)					// �}�[�J�[�̒��S���W(X)
#define SELECT_Y		(660.0f)					// �}�[�J�[�̒��S���W(Y)
#define SELECT_SIZE 	(20.0f)						// �}�[�J�[�̃T�C�Y
#define SELECT_WIDTH	(50.0f)						// �}�[�J�[�̊Ԋu

#define PRESSOK_X		(1050.0f)					// �v���XOK�̒��S���W(X)
#define PRESSOK_Y		(660.0f)					// �v���XOK�̒��S���W(Y)
#define PRESSOK_WIDTH 	(300.0f)					// �v���XOK�̕�
#define PRESSOK_HEIGHT	(50.0f)						// �v���XOK�̍���

#define FADE_INTERBAL	(60)						// �t�F�[�h�̃C���^�[�o��
#define ALUFA_INTERBAL	(10)						// �_�ł̃C���^�[�o���C���^�[�o��

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLoadScreen = NULL;			// ���[�h��ʂ̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureLoadScreen[MAX_TEXTURE] = {};		// ���[�h��ʂ̃e�N�X�`���̃|�C���^
float g_colPressOK;												// �v���XOK�̓_�ŗp		
float g_fmoveAlufa;												// ���l�̈ړ�
int g_nCntSelect;												// ���ڂ�����
int g_nCntImage;												// ���Ԗڂ̉摜���o����
bool bPressOK;													// ���肵�����ǂ���
int g_fadeCnt;													// �t�F�[�h�J�E���g
int g_PressOKFlashCnt;											// �v���XOK�̓_�ŃJ�E���g

float g_fMove[MAX_BG] = { MOVE_BG ,MOVE_BG * 2 ,MOVE_BG * 3 };	// ���[�h��ʂ�UV�l�̌�����

//-----------------------------------------------------------------
// ���[�h��ʂ̏���������
//-----------------------------------------------------------------
HRESULT InitLoadScreen(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_colPressOK = 0.0f;
	g_fmoveAlufa = MOVE_ALUFA;
	g_nCntSelect = 0;
	g_nCntImage = 4;
	bPressOK = false;
	g_fadeCnt = 0;
	g_PressOKFlashCnt = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureLoadScreen[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureLoadScreen[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureLoadScreen[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureLoadScreen[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe001.png", &g_pTextureLoadScreen[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe002.png", &g_pTextureLoadScreen[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_se.png", &g_pTextureLoadScreen[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_ge.png", &g_pTextureLoadScreen[7]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_OK.png", &g_pTextureLoadScreen[8]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffLoadScreen,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// ���_���W�̐ݒ�
		SetVertexLoadScreen(nCnt);

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
	g_pVtxBuffLoadScreen->Unlock();

	// �T�E���h
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//-----------------------------------------------------------------
// ���[�h��ʂ̏I������
//-----------------------------------------------------------------
void UninitLoadScreen(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffLoadScreen != NULL)
	{
		g_pVtxBuffLoadScreen->Release();
		g_pVtxBuffLoadScreen = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (g_pTextureLoadScreen[nCnt] != NULL)
		{
			g_pTextureLoadScreen[nCnt]->Release();
			g_pTextureLoadScreen[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ���[�h��ʂ̍X�V����
//-----------------------------------------------------------------
void UpdateLoadScreen(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	bool *pStick = GetControllerTrigger();
	DIJOYSTATE2 Controller = GetControllerPless();

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	// �v���XOK���������������ĂȂ���
	if (bPressOK == false)
	{// �v���XOK�������ĂȂ��Ƃ�
		// �}�[�J�[�̍X�V
		if (GetKeyboardTrigger(KEYINFO_RIGHT) == true)
		{// �E�Ɉړ�����Ƃ�
			if (g_nCntSelect != 3)
			{
				g_nCntSelect++;		// �}�[�J�[�̈ړ��l�̑���
				g_nCntImage++;		// �摜�����̂��̂Ɉڂ�

				// SE
				PlaySound(SOUND_LABEL_SE_DOWN);
			}
		}
		else if (GetKeyboardTrigger(KEYINFO_LEFT) == true)
		{// ���Ɉړ�����Ƃ�
			if (g_nCntSelect != 0)
			{
				g_nCntSelect--;		// �}�[�J�[�̈ړ��l�̌���
				g_nCntImage--;		// �摜��O�̂��̂Ɉڂ�

				// SE
				PlaySound(SOUND_LABEL_SE_DOWN);
			}
		}
		// JOYPAD �̏���
		if (pStick[2] == true)
		{// �E�Ɉړ�����Ƃ�
			if (g_nCntSelect != 3)
			{
				g_nCntSelect++;		// �}�[�J�[�̈ړ��l�̑���
				g_nCntImage++;		// �摜�����̂��̂Ɉڂ�
				
				// SE
				PlaySound(SOUND_LABEL_SE_DOWN);
			}
		}
		if (pStick[3] == true)
		{// ���Ɉړ�����Ƃ�
			if (g_nCntSelect != 0)
			{
				g_nCntSelect--;		// �}�[�J�[�̈ړ��l�̌���
				g_nCntImage--;		// �摜��O�̂��̂Ɉڂ�

				// SE
				PlaySound(SOUND_LABEL_SE_DOWN);
			}
		}
		// ��ʑJ��
		if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
		{
			if (g_nCntSelect == 3)
			{// �v���XOK��true�ɂ���
				bPressOK = true;

				// �T�E���h
				PlaySound(SOUND_LABEL_SE_OK);
			}
		}
	}
	else if (bPressOK == true)
	{// �v���XOK���������Ƃ�
		// �J�E���^�[�̍X�V
		g_fadeCnt++;

		if (g_fadeCnt >= FADE_INTERBAL)
		{// �Q�[���Ɉړ�
			SetFade(FADE_OUT, MODE_GAME);
		}
	}
		// ���_���W�̍X�V
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		SetVertexLoadScreen(nCnt);
	}
}

//-----------------------------------------------------------------
// ���[�h��ʂ̕`�揈��
//-----------------------------------------------------------------
void DrawLoadScreen(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLoadScreen, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (nCnt < MAX_BG)
		{// �w�i
			pDevice->SetTexture(0, g_pTextureLoadScreen[nCnt]);
		}
		else if (nCnt >= 3 && nCnt < 7)
		{// �}�[�J�[
			pDevice->SetTexture(0, g_pTextureLoadScreen[3]);
		}
		else if (nCnt == 7)
		{// �摜
			pDevice->SetTexture(0, g_pTextureLoadScreen[g_nCntImage]);
		}
		else if (nCnt == 8)
		{// �摜
			pDevice->SetTexture(0, g_pTextureLoadScreen[8]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
} 

//-----------------------------------------------------------------
// ���[�h��ʂ̒��_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexLoadScreen(int nCnt)
{
	if (nCnt < MAX_BG)
	{// ���[�h��ʂ̔w�i�̐ݒ�
		LoadScreenBg(nCnt);
	}
	else if (nCnt >= 3 && nCnt < 7)
	{// �}�[�J�[�̐ݒ�
		LoadScreenSelect(nCnt);
	}
	else if (nCnt == 7)
	{// �摜�̐ݒ�
		LoadScreenImage(nCnt);
	}
	else if (nCnt == 8)
	{// �摜�̐ݒ�
		LoadScreenPressOK(nCnt);
	}
}

//-----------------------------------------------------------------
// ���[�h��ʂ̔w�i
//-----------------------------------------------------------------
void LoadScreenBg(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ��m��
	pVtx += nCnt * 4;

	// �w�i�̍X�V
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	// �w�i�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f + g_fMove[nCnt]);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f + g_fMove[nCnt]);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f + g_fMove[nCnt]);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f + g_fMove[nCnt]);

	// �ϐ������Z
	g_fMove[nCnt] -= MOVE_BG * (nCnt + 1);

	// 1.0f�𒴂����Ƃ�����
	if (g_fMove[nCnt] >= 1.0f)
	{
		g_fMove[nCnt] = 0.0f;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLoadScreen->Unlock();
}

//-----------------------------------------------------------------
// ���[�h��ʂ̉摜
//-----------------------------------------------------------------
void LoadScreenImage(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ��m��
	pVtx += nCnt * 4;

	// �w�i�̍X�V
	pVtx[0].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2,0.0f);
	pVtx[1].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2,0.0f);
	pVtx[3].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2,0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLoadScreen->Unlock();
}

//-----------------------------------------------------------------
// ���[�h��ʂ̃}�[�J�[
//-----------------------------------------------------------------
void LoadScreenSelect(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ��m��
	pVtx += nCnt * 4;

	// �Ԋu�̊m��
	float nWidth = ((nCnt - 3) % MAX_SELECT) * SELECT_WIDTH;
	
	// �w�i�̍X�V
	pVtx[0].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);

	// ���_�J���[�̐ݒ�
	if ((nCnt - 3) == g_nCntSelect)
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
	g_pVtxBuffLoadScreen->Unlock();
}

//-----------------------------------------------------------------
// ���[�h��ʂ̃v���XOK
//-----------------------------------------------------------------
void LoadScreenPressOK(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ��m��
	pVtx += nCnt * 4;

	// �w�i�̍X�V
	pVtx[0].pos = D3DXVECTOR3(PRESSOK_X - PRESSOK_WIDTH / 2 , PRESSOK_Y + PRESSOK_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PRESSOK_X - PRESSOK_WIDTH / 2 , PRESSOK_Y - PRESSOK_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PRESSOK_X + PRESSOK_WIDTH / 2 , PRESSOK_Y + PRESSOK_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PRESSOK_X + PRESSOK_WIDTH / 2 , PRESSOK_Y - PRESSOK_HEIGHT / 2, 0.0f);

	// �_��
	if (bPressOK == false)
	{
		if (3 == g_nCntSelect)
		{// �Ō�̃y�[�W�ɂ������Ƃ�
			g_colPressOK += g_fmoveAlufa;

			if (g_colPressOK <= 0.0f || g_colPressOK >= 1.0f)
			{// 0.0f�ȉ��܂���1.0f�ȏ�ɂȂ����Ƃ�-1��������
				g_fmoveAlufa *= -1;
			}
		}
		else if (3 != g_nCntSelect)
		{// �Ō�̃y�[�W�ɂ����Ă��Ȃ��Ƃ�
			g_colPressOK = 0.0f;
		}
	}
	else if (bPressOK == true)
	{// �v���XOK���������Ƃ��_��
		g_PressOKFlashCnt++;

		if (0 == g_PressOKFlashCnt % ALUFA_INTERBAL)
		{
			g_colPressOK = 1.0f;
		}
		else if ((ALUFA_INTERBAL / 2) == g_PressOKFlashCnt % ALUFA_INTERBAL)
		{
			g_colPressOK = 0.2f;
		}
	}
	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLoadScreen->Unlock();
}