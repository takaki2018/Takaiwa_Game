//-----------------------------------------------------------------
//
// �^�C�}�[ (timer.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "timer.h"
#include "selectmode.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define SCORE_X			(565.0f)										// ��X���W
#define RIGHT_X			(615.0f)										// �EX���W
#define SCORE_Y			(10.0f)											// ��Y���W
#define LOW_Y			(90.0f)											// ��Y���W
#define WIDTH_X			(45.0f)											// �X�R�A�̕�

#define MAX_PATTERN     (10)											// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(10)											// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)												// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define MAX_TIMER		(3)												// �X�R�A�e�N�X�`���̍ő�l
#define TIME			(50)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;	// �^�C�}�[�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;		// �^�C�}�[�̒��_�o�b�t�@�̃|�C���^
int g_nTimer;									// �^�C�}�[
int g_nTimerCnt;								// �^�C�}�[�J�E���g
int g_aNumber[3];								// �c�莞�Ԃ��i�[
D3DXCOLOR g_TimerCol;							// �^�C�}�[�̐F
int g_nSelectMode;
int g_nCntSetEnemy;

//-----------------------------------------------------------------
// �^�C�}�[�̏���������
//-----------------------------------------------------------------
HRESULT InitTimer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	g_nSelectMode = GetSelectMode();
	g_nCntSetEnemy = 105;

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTimer);

	// �ϐ��̏�����
	switch (g_nSelectMode)
	{// �^�C�}�[
	case SELECTMODE_NORMAL:
		g_nTimer = TIME;
		break;
		// �^�C�}�[
	case SELECTMODE_ENDLESS:
		g_nTimer = 0;
		break;
	}								
	g_nTimerCnt = 60;								// �^�C�}�[�J�E���g
	g_TimerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �^�C�}�[�̐F

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TIMER, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(SCORE_X + WIDTH_X * nCntTimer, LOW_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_X + WIDTH_X * nCntTimer, SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTimer, LOW_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTimer, SCORE_Y, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		// �c�莞�Ԃ̌v�Z
		g_aNumber[nCntTimer] = (g_nTimer / (int)powf(10.0f, (float)(MAX_TIMER - (nCntTimer + 1))));

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 0.0f);

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �^�C�}�[�̏I������
//-----------------------------------------------------------------
void UninitTimer(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
}

//-----------------------------------------------------------------
// �^�C�}�[�̍X�V����
//-----------------------------------------------------------------
void UpdateTimer(void)
{
	// ���[�h�Ŏ���
	switch (g_nSelectMode)
	{// �m�[�}�����[�h
	case SELECTMODE_NORMAL:
		// �^�C�}�[�J�E���g�̌���
		g_nTimerCnt--;

		if (g_nTimerCnt == 0 && g_nTimer != 0)
		{
			// �c�莞�Ԃ����炷
			g_nTimer -= 1;

			// ������
			g_nTimerCnt = 60;

			if (g_nTimer == 0)
			{
				g_nTimerCnt = 10;
			}
		}
		if (g_nTimerCnt == 0 && g_nTimer == 0)
		{// �^�C�}�[��_�ł�����
			g_TimerCol.a = 0.0f;
			g_nTimerCnt = 20;
		}
		else if (g_nTimerCnt == 10 && g_nTimer == 0)
		{
			g_TimerCol.a = 1.0f;
		}
		break;
		// �G���h���X���[�h
	case SELECTMODE_ENDLESS:
		// �^�C�}�[�J�E���g�̌���
		g_nTimerCnt--;

		if (g_nTimerCnt == 0)
		{
			// ���Ԃ̃J�E���g
			g_nTimer++;

			// ������
			g_nTimerCnt = 60;

			// �G��z�u����Ԋu��ύX
			if (g_nTimer <= 300)
			{
				if (0 == g_nTimer % 50)
				{// �T�E���h
					PlaySound(SOUND_LABEL_SE_OK);
					g_nCntSetEnemy -= 10;
				}
			}
		}
		break;

	default:
		break;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// �c�莞�Ԃ��i�[
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{// �c�莞�Ԃ̌v�Z
		g_aNumber[nCntTimer] = (g_nTimer / (int)powf(10.0f, (float)(MAX_TIMER - (nCntTimer + 1))));

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 0.0f);

		// ���_�J���[�̍X�V
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//-----------------------------------------------------------------
// �^�C�}�[�̕`�揈��
//-----------------------------------------------------------------
void DrawTimer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{// �X�R�A�̕\��
		pDevice->SetTexture(0, g_pTextureTimer);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
	}
}

//-----------------------------------------------------------------
// �^�C�}�[���̎擾
//-----------------------------------------------------------------
int GetTimer(void)
{
	return g_nTimer;
}

//-----------------------------------------------------------------
// �G��z�u����Ԋu
//-----------------------------------------------------------------
int GetCntSetEnemy(void)
{
	return g_nCntSetEnemy;
}