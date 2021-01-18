//-----------------------------------------------------------------
//
// �Q�[�W (gauge.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "gauge.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_PATTERN		(2)				// ���

#define GAUGE_CENTER_X	(135.0f)		// �Q�[�W�̒��S���W(X)
#define GAUGE_CENTER_y	(75.0f)			// �Q�[�W�̒��S���W(Y)

#define GAUGE_X			(250.0f)		// �Q�[�W�̕�
#define GAUGE_Y			(30.0f)			// �Q�[�W�̍���

#define GAUGE_GREEN_X	(242.0f)		// �΃Q�[�W�̕�
#define GAUGE_GREEN_Y	(24.0f)			// �΃Q�[�W�̍���

#define REMOVE			(40.0f)			// ������

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;			// �Q�[�W�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureGauge[MAX_PATTERN] = {};	// �Q�[�W�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posGauge;									// �Q�[�W�̒��S���W
float g_fRemove;										// �Q�[�W�̌�����
bool g_bRemove;											// �Q�[�W�����������������ǂ���

//-----------------------------------------------------------------
// �Q�[�W�̏���������
//-----------------------------------------------------------------
HRESULT InitGauge(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posGauge = D3DXVECTOR3(GAUGE_CENTER_X, GAUGE_CENTER_y, 0.0f);
	g_fRemove = 0.0f;
	g_bRemove = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge.png", &g_pTextureGauge[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge100.png", &g_pTextureGauge[1]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// ���_���W�̐ݒ�
		switch (nCnt)
		{// �Q�[�W�g
		case 0:
			pVtx[0].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_X / 2, g_posGauge.y + GAUGE_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_X / 2, g_posGauge.y - GAUGE_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_X / 2, g_posGauge.y + GAUGE_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_X / 2, g_posGauge.y - GAUGE_Y / 2, 0.0f);
			break;
			// �Q�[�W
		case 1:
			pVtx[0].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);
			break;

		default:
			break;
		}

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

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �Q�[�W�̏I������
//-----------------------------------------------------------------
void UninitGauge(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (g_pTextureGauge[nCnt] != NULL)
		{
			g_pTextureGauge[nCnt]->Release();
			g_pTextureGauge[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �Q�[�W�̍X�V����
//-----------------------------------------------------------------
void UpdateGauge(void)
{
	// �e����������
	if (g_bRemove == false)
	{
		if (GetKeyboardTrigger(KEYINFO_SHOOT) == true || GetJoypadTrigger(JPINFO_SHOOT) == true)
		{// �����l�𑝂₷
			g_fRemove += REMOVE;

			if (g_fRemove >= GAUGE_GREEN_X)
			{// �[�܂ŗ����Ƃ�
				g_fRemove = GAUGE_GREEN_X;
				g_bRemove = true;
			}
		}
	}
	// �����l�����炷
	g_fRemove -= 0.8f;

	if (g_fRemove <= 0.0f)
	{// �������Ȃ�����
		g_fRemove = 0.0f;
		g_bRemove = false;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[4].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2 - g_fRemove, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2 - g_fRemove, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//-----------------------------------------------------------------
// �Q�[�W�̕`�揈��
//-----------------------------------------------------------------
void DrawGauge(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureGauge[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// �����l�̎擾
//-----------------------------------------------------------------
bool GetRemove(void)
{
	return g_bRemove;
}