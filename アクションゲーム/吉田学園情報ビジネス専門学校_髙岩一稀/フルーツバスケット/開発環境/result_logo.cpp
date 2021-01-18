//-----------------------------------------------------------------
//
// ���U���g���S (count_down.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_logo.h"
#include "gauge.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_TEXTURE			(4)				// �X�R�A�e�N�X�`���̍ő�l

#define RESULT_LOGO_WIDTH	(90.0f)			// ���U���g���S�̕�
#define RESULT_LOGO_HEIGHT	(55.0f)			// ���U���g���S�̍���

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultLogo = NULL;			// ���U���g���S�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultLogo[MAX_TEXTURE] = {};		// ���U���g���S�̒��_�o�b�t�@�̃|�C���^
RESULTLOGO g_aResultLogo[MAX_TEXTURE];							// ���U���g���S�̏��

//-----------------------------------------------------------------
// ���U���g���S�̏���������
//-----------------------------------------------------------------
HRESULT InitResultLogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureResultLogo[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureResultLogo[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureResultLogo[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureResultLogo[3]);

	// �ϐ��̏�����
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		g_aResultLogo[nCntResultLogo].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aResultLogo[nCntResultLogo].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aResultLogo[nCntResultLogo].nTexType = RANK_FOURTH;
		g_aResultLogo[nCntResultLogo].fHeight = 0.0f;
		g_aResultLogo[nCntResultLogo].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		// ���_���W�̍X�V
		SetVErtexResultLogo(nCntResultLogo);

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

		// ���Ԗڂ�
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultLogo->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ���U���g���S�̏I������
//-----------------------------------------------------------------
void UninitResultLogo(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffResultLogo != NULL)
	{
		g_pVtxBuffResultLogo->Release();
		g_pVtxBuffResultLogo = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureResultLogo[nCnt] != NULL)
		{
			g_pTextureResultLogo[nCnt]->Release();
			g_pTextureResultLogo[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ���U���g���S�̍X�V����
//-----------------------------------------------------------------
void UpdateResultLogo(void)
{
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aResultLogo[nCntResultLogo].bUse == true)
		{
			// ���_���W�̍X�V
			SetVErtexResultLogo(nCntResultLogo);
		}
	}
}

//-----------------------------------------------------------------
// ���U���g���S�̕`�揈��
//-----------------------------------------------------------------
void DrawResultLogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aResultLogo[nCntResultLogo].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureResultLogo[(int)(g_aResultLogo[nCntResultLogo].nTexType)]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultLogo * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVErtexResultLogo(int nCntResultLogo)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	RESULTLOGO *pResultLogo = &g_aResultLogo[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCntResultLogo * 4;
	pResultLogo += nCntResultLogo;

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(pResultLogo->pos.x - (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y + (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pResultLogo->pos.x - (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y - (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pResultLogo->pos.x + (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y + (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pResultLogo->pos.x + (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y - (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);

	// ���_�J���[�̍X�V
	pVtx[0].col = g_aResultLogo[nCntResultLogo].col;
	pVtx[1].col = g_aResultLogo[nCntResultLogo].col;
	pVtx[2].col = g_aResultLogo[nCntResultLogo].col;
	pVtx[3].col = g_aResultLogo[nCntResultLogo].col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultLogo->Unlock();
}

//-----------------------------------------------------------------
// ���S�̐ݒu
//-----------------------------------------------------------------
void SetResultLogo(D3DXVECTOR3 pos,float fHeight, RANK nTexType)
{
	// �\���̂̃|�C���^�ϐ�
	RESULTLOGO *pResultLogo;

	// �ϐ��̎󂯓n��
	pResultLogo = &g_aResultLogo[0];

	// �e�̐ݒ�
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++, pResultLogo++)
	{
		if (pResultLogo->bUse == false)
		{// �ʒu�ݒ�
			pResultLogo->pos = pos;

			// �Q�[�W�̍���
			pResultLogo->fHeight = fHeight;

			// ���̃e�N�X�`�����g����
			pResultLogo->nTexType = nTexType;

			// true�ŕԂ�
			pResultLogo->bUse = true;

			break;
		}
	}
}