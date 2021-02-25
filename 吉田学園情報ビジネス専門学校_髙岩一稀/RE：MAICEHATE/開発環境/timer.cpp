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
#define TIMER_POS_X				(595.0f)		// �}���`���[�h�̃^�C�}�[�̒��S���W(X)
#define TIMER_POS_Y				(50.0f)			// �}���`���[�h�̃^�C�}�[�̒��S���W(Y)

#define TIMER_WIDTH				(50.0f)			// �^�C�}�[�̕�
#define TIMER_HEIGHT			(80.0f)			// �^�C�}�[�̍���
#define TIMER_SPACE				(45.0f)			// �^�C�}�[�̐�����z�u����Ԋu

#define MAX_PATTERN				(10)			// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X				(10)			// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y				(1)				// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define MAX_TIMER				(3)				// �X�R�A�e�N�X�`���̍ő�l
#define TIME					(180)			// �^�C���̐ݒ�
#define TIMECOUNT_INTERBAL		(60)			// �^�C���J�E���g�̃C���^�[�o��

#define FLASH_INTERBAL_TIMER	(20)			// �^�C�}�[�̓_�ŃC���^�[�o��
#define CLEAR_NUMBER_TIMER		(10)			// �^�C�}�[�̓����ԍ�
#define LESS_CLEAR_NUMBER_TIMER	(0)				// �^�C�}�[�̕s�����ԍ�

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;	// �^�C�}�[�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;		// �^�C�}�[�̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posTimer;							// �^�C�}�[�̒��S���W
int g_nTimer;									// �^�C�}�[
int g_nTimerCnt;								// �^�C�}�[�J�E���g
int g_nCntFlashTimer;							// �^�C�}�[�̓_�ŃJ�E���^
int g_aNumber[MAX_TIMER];						// �c�莞�Ԃ��i�[
D3DXCOLOR g_TimerCol;							// �^�C�}�[�̐F

//-----------------------------------------------------------------
// �^�C�}�[�̏���������
//-----------------------------------------------------------------
HRESULT InitTimer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTimer);

	// �ϐ��̏�����
	g_posTimer = D3DXVECTOR3(TIMER_POS_X, TIMER_POS_Y, 0.0f);
	g_nTimer = TIME;								
	g_nTimerCnt = TIMECOUNT_INTERBAL;				
	g_nCntFlashTimer = 0;
	g_TimerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	

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
		pVtx[0].pos = D3DXVECTOR3(g_posTimer.x - (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y + (TIMER_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTimer.x - (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y - (TIMER_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTimer.x + (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y + (TIMER_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTimer.x + (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y - (TIMER_HEIGHT / 2), 0.0f);

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
	if (g_nTimer != 0)
	{// �^�C�}�[��0���傫���Ƃ�
		// �^�C�}�[�J�E���g����
		CountTimer();
	}
	else if (g_nTimer == 0)
	{// �^�C�}�[��0�̂Ƃ�
		// �^�C���A�b�v���̃^�C�}�[�_�ŏ���
		FlashTmer();
	}
	
	// �e�N�X�`��UV�̐ݒ�
	SetTexUVTimer();

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
// �^�C�}�[�̃J�E���g����
//-----------------------------------------------------------------
void CountTimer(void)
{
	// �^�C�}�[�J�E���g�̌���
	g_nTimerCnt--;

	if (g_nTimerCnt == 0)
	{
		// �c�莞�Ԃ����炷
		g_nTimer -= 1;

		// ������
		g_nTimerCnt = TIMECOUNT_INTERBAL;
	}
}

//-----------------------------------------------------------------
// �^�C���A�b�v���̃^�C�}�[�_�ŏ���
//-----------------------------------------------------------------
void FlashTmer(void)
{
	if (CLEAR_NUMBER_TIMER == g_nCntFlashTimer % FLASH_INTERBAL_TIMER)
	{// �^�C�}�[��_�ł�����
		g_TimerCol.a = 0.0f;
	}
	else if (LESS_CLEAR_NUMBER_TIMER == g_nCntFlashTimer % FLASH_INTERBAL_TIMER)
	{
		g_TimerCol.a = 1.0f;
	}
	// �_�ŃJ�E���^�̃J�E���g
	g_nCntFlashTimer++;
}

//-----------------------------------------------------------------
// �e�N�X�`��UV�̐ݒ�
//-----------------------------------------------------------------
void SetTexUVTimer(void)
{
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
// �^�C�}�[���̎擾
//-----------------------------------------------------------------
int GetTimer(void)
{
	return g_nTimer;
}