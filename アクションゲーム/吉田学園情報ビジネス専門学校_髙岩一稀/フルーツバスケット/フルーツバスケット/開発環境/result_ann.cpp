//-----------------------------------------------------------------
//
// ���U���g���S (count_down.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_ann.h"
#include "gauge.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_TEXTURE			(1)						// �X�R�A�e�N�X�`���̍ő�l

#define RESULT_ANN_X		(0.0f)					// ���ʔ��\�̒��S���W(X)
#define RESULT_ANN_Y		(SCREEN_HEIGHT / 2.0f)	// ���ʔ��\�̒��S���W(Y)

#define RESULT_ANN_WIDTH	(500.0f)					// ���U���g���S�̕�
#define RESULT_ANN_HEIGHT	(125.0f)					// ���U���g���S�̍���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultAnn = NULL;		// ���ʔ��\�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultAnn = NULL;			// ���ʔ��\�̒��_�o�b�t�@�̃|�C���^
RESULTANN g_aResultAnn;									// ���ʔ��\�̏��

//-----------------------------------------------------------------
// ���ʔ��\�̏���������
//-----------------------------------------------------------------
HRESULT InitResultAnn(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result_ann.png", &g_pTextureResultAnn);

	// �ϐ��̏�����
	g_aResultAnn.pos = D3DXVECTOR3(RESULT_ANN_X, RESULT_ANN_Y, 0.0f);
	g_aResultAnn.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResultAnn.fWidth = RESULT_ANN_WIDTH;
	g_aResultAnn.fHeight = RESULT_ANN_HEIGHT;
	g_aResultAnn.nCounterState = 100;
	g_aResultAnn.bSound = false;
	g_aResultAnn.bUse = true;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultAnn,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultAnn->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	SetVErtexResultAnn();

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e�N�X�`���̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultAnn->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ���ʔ��\�̏I������
//-----------------------------------------------------------------
void UninitResultAnn(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultAnn != NULL)
	{
		g_pVtxBuffResultAnn->Release();
		g_pVtxBuffResultAnn = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureResultAnn != NULL)
	{
		g_pTextureResultAnn->Release();
		g_pTextureResultAnn = NULL;
	}
}

//-----------------------------------------------------------------
// ���ʔ��\�̍X�V����
//-----------------------------------------------------------------
void UpdateResultAnn(void)
{
	if (g_aResultAnn.bUse == true)
	{// �g���Ă���Ƃ�
		if (g_aResultAnn.pos.x < SCREEN_WIDTH / 2.0f  && g_aResultAnn.nCounterState > 0)
		{// �܂��͍����璆���Ɉړ�
			g_aResultAnn.pos.x += 15.0f;

			if (g_aResultAnn.bSound == false)
			{// �T�E���h
				PlaySound(SOUND_LABEL_SE_THROW);

				g_aResultAnn.bSound = true;
			}
		}
		else if (g_aResultAnn.pos.x >= SCREEN_WIDTH / 2.0f && g_aResultAnn.nCounterState > 0)
		{// �����ɗ����班���~�܂�
			g_aResultAnn.pos.x = SCREEN_WIDTH / 2.0f;

			g_aResultAnn.nCounterState--;

			g_aResultAnn.bSound = false;
		}
		else if (g_aResultAnn.nCounterState <= 0)
		{// ������x�~�܂�����E�Ɍ������ē���
			g_aResultAnn.pos.x += 15.0f;

			if (g_aResultAnn.bSound == false)
			{// �T�E���h
				PlaySound(SOUND_LABEL_SE_DORAM);

				g_aResultAnn.bSound = true;
			}
		}

		if (g_aResultAnn.pos.x - g_aResultAnn.fWidth / 2.0f > 2000.0f)
		{// ������x�܂ōs���������
			g_aResultAnn.bUse = false;
		}

		// ���_���W�̍X�V
		SetVErtexResultAnn();
	}
}

//-----------------------------------------------------------------
// ���ʔ��\�̕`�揈��
//-----------------------------------------------------------------
void DrawResultAnn(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultAnn, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	if (g_aResultAnn.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureResultAnn);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVErtexResultAnn(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	RESULTANN *pResultAnn = &g_aResultAnn;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultAnn->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_aResultAnn.pos.x - (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y + (g_aResultAnn.fHeight / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResultAnn.pos.x - (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y - (g_aResultAnn.fHeight / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResultAnn.pos.x + (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y + (g_aResultAnn.fHeight / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResultAnn.pos.x + (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y - (g_aResultAnn.fHeight / 2), 0.0f);

	// ���_�J���[�̍X�V
	pVtx[0].col = g_aResultAnn.col;
	pVtx[1].col = g_aResultAnn.col;
	pVtx[2].col = g_aResultAnn.col;
	pVtx[3].col = g_aResultAnn.col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultAnn->Unlock();
}

//-----------------------------------------------------------------
// ���ʔ��\�̏��̎擾
//-----------------------------------------------------------------
bool GetResultAnn(void)
{
	return g_aResultAnn.bUse;
}