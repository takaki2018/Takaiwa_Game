//-----------------------------------------------------------------
//
// �J�E���g�_�E�� (count_down.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "count_down.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define COUNT_DOWN_X		(SCREEN_WIDTH / 2.0f)		// �J�E���g�_�E���̒��S���W(X)
#define COUNT_DOWN_Y		(SCREEN_HEIGHT / 2.0f)		// �J�E���g�_�E���̒��S���W(Y)
#define COUNT_DOWN_WIDTH	(100.0f)						// �J�E���g�_�E���̕�
#define COUNT_DOWN_HEIGHT	(160.0f)						// �J�E���g�_�E���̍���

#define START_WIDTH			(320.0f)
#define START_HEIGHT		(90.0f)

#define MAX_PATTERN			(10)						// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X			(10)						// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y			(1)							// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define MAX_TEXTURE			(3)							// �X�R�A�e�N�X�`���̍ő�l
#define COUNT_DOWN			(3)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountDown = NULL;				// �J�E���g�_�E���̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCountDown[MAX_TEXTURE] = {};		// �J�E���g�_�E���̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posCountDown;										// �J�E���g�_�E���̒��S���W
int g_nCountDown;												// �J�E���g�_�E��
int g_nCountDownCnt;											// �J�E���g�_�E���J�E���g
D3DXCOLOR g_colCountDown;										// �J�E���g�_�E���̐F
float g_fNumber;												// �J�E���g�_�E���̐���
COUNTDOWN g_CountDown;											// �J�E���g�_�E���̏��
bool g_bSound;

//-----------------------------------------------------------------
// �J�E���g�_�E���̏���������
//-----------------------------------------------------------------
HRESULT InitCountDown(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureCountDown[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/start_logo.png", &g_pTextureCountDown[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/timeup_logo.png", &g_pTextureCountDown[2]);

	// �ϐ��̏�����
	g_CountDown.pos = D3DXVECTOR3(COUNT_DOWN_X, COUNT_DOWN_Y, 0.0f);
	g_CountDown.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			
	g_CountDown.nCntAlufa = 50;
	g_CountDown.fCntSize = 1.0f;
	g_CountDown.nTExType = 0;
	g_CountDown.bUse = true;

	// �O���[�o���ϐ��̏�����
	g_nCountDown = COUNT_DOWN;										// �J�E���g�_�E��			
	g_nCountDownCnt = 60;											// �J�E���g�_�E���J�E���g
	g_fNumber = 3.0f;
	g_bSound = false;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffCountDown,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	SetVertexCountDown();

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCountDown->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �J�E���g�_�E���̏I������
//-----------------------------------------------------------------
void UninitCountDown(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffCountDown != NULL)
	{
		g_pVtxBuffCountDown->Release();
		g_pVtxBuffCountDown = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureCountDown[nCnt] != NULL)
		{
			g_pTextureCountDown[nCnt]->Release();
			g_pTextureCountDown[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �J�E���g�_�E���̍X�V����
//-----------------------------------------------------------------
void UpdateCountDown(void)
{
	// �ϐ��錾
	int nTimer = GetTimer();

	if (g_nCountDownCnt != 0)
	{// �T�C�Y�J�E���g�̌���
		g_CountDown.fCntSize -= 0.005f;
	}
	if (g_nCountDownCnt == 60)
	{
		// �T�E���h
		PlaySound(SOUND_LABEL_SE_COUNTDOWN);
	}

	// �^�C�}�[�J�E���g�̌���
	g_nCountDownCnt--;

	// ���l�̈ړ�
	g_CountDown.col.a -= 0.02f;

	if (g_nCountDownCnt == 0 && g_nCountDown != 0)
	{
		// �T�E���h
		PlaySound(SOUND_LABEL_SE_COUNTDOWN);

		// �c�莞�Ԃ����炷
		g_nCountDown -= 1;

		// ������
		g_nCountDownCnt = 60;

		g_fNumber -= 1.0f;

		if (g_nCountDown == 0)
		{// �J�E���g�_�E���̃J�E���g�̏�����
			g_nCountDownCnt = 0;

			// �J�E���g�_�E���̃e�N�X�`���̍X�V
			g_CountDown.nTExType = 1;

			// �T�E���h
			PlaySound(SOUND_LABEL_SE_START);

			//	�T�E���h�̐ݒ�
			PlaySound(SOUND_LABEL_BATTLE);
		}
		// ���l�̏�����
		g_CountDown.col.a = 1.0f;

		// �T�C�Y�J�E���g�̏�����
		g_CountDown.fCntSize = 1.0f;
	}
	if (nTimer == 0)
	{
		// �T�E���h�̈ꎞ��~
		StopSound(SOUND_LABEL_BATTLE);

		if (g_bSound == false)
		{
			// �T�E���h
			PlaySound(SOUND_LABEL_SE_TIMEUP);

			g_bSound = true;
		}

		// �J�E���g�_�E���̃e�N�X�`���̍X�V
		g_CountDown.nTExType = 2;

		// ���l�̏�����
		g_CountDown.col.a = 1.0f;

	}

	// ���_���W�̍X�V
	SetVertexCountDown();
}

//-----------------------------------------------------------------
// �J�E���g�_�E���̕`�揈��
//-----------------------------------------------------------------
void DrawCountDown(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCountDown, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	if (g_CountDown.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureCountDown[g_CountDown.nTExType]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexCountDown(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	if (g_CountDown.nTExType == 0)
	{// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x - (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y + (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x - (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y - (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x + (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y + (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y - (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_fNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_fNumber * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_fNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_fNumber * 0.1f, 0.0f);
	}
	else if (g_CountDown.nTExType == 1)
	{
		// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x - (START_WIDTH / 2), g_CountDown.pos.y + (START_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x - (START_WIDTH / 2), g_CountDown.pos.y - (START_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x + (START_WIDTH / 2), g_CountDown.pos.y + (START_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + (START_WIDTH / 2), g_CountDown.pos.y - (START_HEIGHT / 2) , 0.0f);

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// ���_�J���[�̍X�V
	pVtx[0].col = g_CountDown.col;
	pVtx[1].col = g_CountDown.col;
	pVtx[2].col = g_CountDown.col;
	pVtx[3].col = g_CountDown.col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCountDown->Unlock();
}


//-----------------------------------------------------------------
// �J�E���g�_�E�����̎擾
//-----------------------------------------------------------------
int GetCountDown(void)
{
	return g_nCountDown;
}