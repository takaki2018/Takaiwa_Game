//==============================================================================================================
//
// �`���[�g���A���pPressOK (tutorial_press_ok.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_press_ok.h"
#include "tutorial_image.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define PRESSOK_X		(1050.0f)					// �v���XOK�̒��S���W(X)
#define PRESSOK_Y		(660.0f)					// �v���XOK�̒��S���W(Y)
#define PRESSOK_WIDTH 	(300.0f)					// �v���XOK�̕�
#define PRESSOK_HEIGHT	(50.0f)						// �v���XOK�̍���

#define MOVE_ALPHA		(0.01f)						// ���l�̈ړ���
#define FADE_INTERBAL	(60)						// �t�F�[�h�̃C���^�[�o��
#define ALUFA_INTERBAL	(10)						// �_�ł̃C���^�[�o���C���^�[�o��

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialPressOK = NULL;		// �`���[�g���A���pPressOK�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTutorialPressOK = NULL;			// �`���[�g���A���pPressOK�̃e�N�X�`���̃|�C���^
TutorialPressOK g_TutorialPressOK;								// 

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialPressOK(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �`���[�g���A���pPressOK���̏�����
	g_TutorialPressOK.pos = D3DXVECTOR3(PRESSOK_X, PRESSOK_Y,0.0f);
	g_TutorialPressOK.col = D3DXCOLOR(1.0f, 1.0f, 1.0f,0.0f);
	g_TutorialPressOK.fWidth = PRESSOK_WIDTH / 2.0f;
	g_TutorialPressOK.fHeight = PRESSOK_HEIGHT / 2.0f;
	g_TutorialPressOK.fMoveAlpha = MOVE_ALPHA;
	g_TutorialPressOK.nFlashCnt = 0;
	g_TutorialPressOK.bDisPlay = false;
	g_TutorialPressOK.bPressOK = false;
	g_TutorialPressOK.bGameTrasition = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_A.png", &g_pTextureTutorialPressOK);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX , // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialPressOK,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialPressOK->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x - g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y + g_TutorialPressOK.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x - g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y - g_TutorialPressOK.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x + g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y + g_TutorialPressOK.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x + g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y - g_TutorialPressOK.fHeight, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	SetColTutorialPressOK();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialPressOK->Unlock();

	// �T�E���h
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialPressOK(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorialPressOK != NULL)
	{
		g_pVtxBuffTutorialPressOK->Release();
		g_pVtxBuffTutorialPressOK = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureTutorialPressOK != NULL)
	{
		g_pTextureTutorialPressOK->Release();
		g_pTextureTutorialPressOK = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialPressOK(void)
{
	switch (g_TutorialPressOK.bPressOK)
	{
	case true:
		// PressOK����������
		AfterPreesingOK();
		break;

	case false:
		// PressOK�������O
		BeforePressingOK();
		break;
	}

	// �F�̍X�V
	SetColTutorialPressOK();
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialPressOK(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialPressOK, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	if (g_TutorialPressOK.bDisPlay == true)
	{
		pDevice->SetTexture(0, g_pTextureTutorialPressOK);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK�������O�̏���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void BeforePressingOK(void)
{
	// �ϐ��錾
	int nPointImage = GetPointImage();

	if (nPointImage == MAX_TUTORIAL_IMAGE - 1)
	{// �Ō�̃y�[�W�ɂ������Ƃ�
	 // ���������Ƃ�`����
		g_TutorialPressOK.bDisPlay = true;

		// ���l�̌��Z
		g_TutorialPressOK.col.a -= g_TutorialPressOK.fMoveAlpha;

		if (g_TutorialPressOK.col.a < 0.0f || g_TutorialPressOK.col.a >= 1.0f)
		{
			// 0.0f��菬�����܂���1.0f���傫���Ȃ����Ƃ����l�̈ړ��ʂ�-1��������
			g_TutorialPressOK.fMoveAlpha *= -1;
		}
		// A�{�^���܂��̓G���^�[�L�[�őJ��
		if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true ||
			GetKeyboardTrigger(KEYINFO_OK) == true)
		{
			// ���������Ƃ�`����
			g_TutorialPressOK.bPressOK = true;

			// �T�E���h
			PlaySound(SOUND_LABEL_SE_OK);
		}
	}
	else if (nPointImage != MAX_TUTORIAL_IMAGE - 1)
	{// �Ō�̃y�[�W�ɂ����Ă��Ȃ��Ƃ��J�E���g�������ƃe�N�X�`���𓧖��ɂ���
		g_TutorialPressOK.nFlashCnt = 0;
		g_TutorialPressOK.col.a = 0.0f;

		// �`�悵�Ȃ��悤�ɂ���
		g_TutorialPressOK.bDisPlay = false;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK����������̏���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void AfterPreesingOK(void)
{
	// �_�ŃJ�E���g��i�߂�
	g_TutorialPressOK.nFlashCnt++;

	// �_�ŏ���
	if (0 == g_TutorialPressOK.nFlashCnt % ALUFA_INTERBAL)
	{
		// �s����
		g_TutorialPressOK.col.a = 1.0f;
	}
	else if ((ALUFA_INTERBAL / 2) == g_TutorialPressOK.nFlashCnt % ALUFA_INTERBAL)
	{
		// �኱����
		g_TutorialPressOK.col.a = 0.2f;
	}

	if (g_TutorialPressOK.nFlashCnt >= FADE_INTERBAL)
	{// �Q�[���ɑJ�ڂ��鋖��^����
		g_TutorialPressOK.bGameTrasition = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK�̃v���XOK
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetColTutorialPressOK(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialPressOK->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_TutorialPressOK.col;
	pVtx[1].col = g_TutorialPressOK.col;
	pVtx[2].col = g_TutorialPressOK.col;
	pVtx[3].col = g_TutorialPressOK.col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialPressOK->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���pPressOK���̎擾
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fbool	- �`���[�g���A���pPressOK����n��
//--------------------------------------------------------------------------------------------------------------
TutorialPressOK GetPressOKInfo(void)
{
	return g_TutorialPressOK;
}