//==============================================================================================================
//
// �`���[�g���A���}�[�J�[ (tutorial_marker.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_marker.h"
#include "tutorial_image.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_MARKER		(4)		// �}�[�J�[�̍ő�l

#define MARKER_X		(565.0f)					// �}�[�J�[�̒��S���W(X)
#define MARKER_Y		(660.0f)					// �}�[�J�[�̒��S���W(Y)
#define MARKER_SIZE 	(20.0f)						// �}�[�J�[�̃T�C�Y
#define MARKER_SPACE	(50.0f)						// �}�[�J�[��z�u����Ԋu

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	float fSize;		// �傫��
}TutorialMarker;

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMarker = NULL;		// �`���[�g���A���}�[�J�[�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTutorialMarker[MAX_MARKER] = {};	// �`���[�g���A���}�[�J�[�̃e�N�X�`���̃|�C���^
TutorialMarker g_TutorialMarker[MAX_MARKER];					// �`���[�g���A���}�[�J�[���
int g_nPointMarker;												// ���ڂ̃}�[�J�[��I��ł��邩

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���}�[�J�[�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialMarker(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �}�[�J�[���̏�����
	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		g_TutorialMarker[nCntMarker].pos = D3DXVECTOR3(MARKER_X + MARKER_SPACE * nCntMarker, MARKER_Y,0.0f);
		g_TutorialMarker[nCntMarker].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		g_TutorialMarker[nCntMarker].fSize = MARKER_SIZE / 2.0f;
	}
	g_nPointMarker = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/marker.png", &g_pTextureTutorialMarker[0]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_MARKER, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialMarker,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialMarker->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x - g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y + g_TutorialMarker[nCntMarker].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x - g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y - g_TutorialMarker[nCntMarker].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x + g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y + g_TutorialMarker[nCntMarker].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x + g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y - g_TutorialMarker[nCntMarker].fSize, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �F�̐ݒ�
		SetColTutorialMarker(nCntMarker);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialMarker->Unlock();

	// �T�E���h
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���}�[�J�[�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialMarker(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorialMarker != NULL)
	{
		g_pVtxBuffTutorialMarker->Release();
		g_pVtxBuffTutorialMarker = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		if (g_pTextureTutorialMarker[nCntMarker] != NULL)
		{
			g_pTextureTutorialMarker[nCntMarker]->Release();
			g_pTextureTutorialMarker[nCntMarker] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���}�[�J�[�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialMarker(void)
{
	// �摜�ԍ��̎擾
	int nPointImage = GetPointImage();

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		if (nCntMarker == nPointImage)
		{
			// �摜�ԍ��ƈ�v���Ă���Ƃ��F����Ԗ��邭����
			g_TutorialMarker[nCntMarker].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// ����ȊO�͈Â�����
			g_TutorialMarker[nCntMarker].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		// �F�̍X�V
		SetColTutorialMarker(nCntMarker);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���}�[�J�[�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialMarker(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialMarker, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_MARKER; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureTutorialMarker[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���}�[�J�[�̐F�ݒ�
// ����		�FnCntMarker	- ���Ԗڂ̃}�[�J�[���擾
// �Ԃ�l	�Fvoid			- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetColTutorialMarker(int nCntMarker)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialMarker->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ̃}�[�J�[��
	pVtx += 4 * nCntMarker;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_TutorialMarker[nCntMarker].col;
	pVtx[1].col = g_TutorialMarker[nCntMarker].col;
	pVtx[2].col = g_TutorialMarker[nCntMarker].col;
	pVtx[3].col = g_TutorialMarker[nCntMarker].col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialMarker->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// �}�[�J�[�̈ʒu���擾
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fint	- ���Ԗڂ̃}�[�J�[���I������Ă邩�Ԃ�
//--------------------------------------------------------------------------------------------------------------
int GetPointMarker(void)
{
	return g_nPointMarker;
}