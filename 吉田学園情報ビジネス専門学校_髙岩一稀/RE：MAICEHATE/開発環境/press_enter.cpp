//==============================================================================================================
//
// �Z���N�g���[�h (selectmode.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "press_enter.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define PRESSENTER_X					(SCREEN_WIDTH / 2)	// �v���X�G���^�[��X���W
#define PREESENTER_Y					(500.0f)			// �v���X�G���^�[��Y���W
#define PRESSENTER_WIDTH				(400.0f)			// �v���X�G���^�[�̕�
#define PREESENTER_HIEGHT				(40.0f)				// �v���X�G���^�[�̍���

#define PREESENTER_FLASH_INTERBAL		(50)				// �v���X�G���^�[�̓_�ŃC���^�[�o��
#define PREESENTER_CLEAR_NUMBER			(25)				// �v���X�G���^�[�̓���
#define PREESENTER_LESS_CLEAR_NUMBER	(0)					// �v���X�G���^�[�̕s����

#define PREESENTER_FLASH_INTERBAL_HI	(10)				// �v���X�G���^�[�̓_�ŃC���^�[�o��
#define PREESENTER_CLEAR_NUMBER_HI		(5)					// �v���X�G���^�[�̓���

#define TITLE_TRANSITION_COUNT			(50)				// �^�C�g������J�ڂ���J�E���g

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXCOLOR col;				// �F
	float fWidth;				// ��
	float fHieght;				// ����
	int nCntFlash;				// �_�ŗp�J�E���^
	bool bPressingEnter;		// ���������ǂ���
	bool bTutorialTransition;	// �`���[�g���A����ʂɑJ��
}PressEnter;

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPressEnter = NULL;	// �v���X�G���^�[�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePressEnter = NULL;			// �v���X�G���^�[�̃e�N�X�`���̃|�C���^
PressEnter g_PressEnter;								// �v���X�G���^�[���

//--------------------------------------------------------------------------------------------------------------
// �Z���N�g���[�h�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitPressEnter(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �Z���N�g���[�h���̏�����
	g_PressEnter.pos = D3DXVECTOR3(PRESSENTER_X, PREESENTER_Y,0.0f);
	g_PressEnter.col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	g_PressEnter.fWidth = PRESSENTER_WIDTH / 2.0f;
	g_PressEnter.fHieght = PREESENTER_HIEGHT / 2.0f;
	g_PressEnter.nCntFlash = 0;
	g_PressEnter.bPressingEnter = false;
	g_PressEnter.bTutorialTransition = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTexturePressEnter);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPressEnter,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetVertexPressEnter();

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

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPressEnter->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �v���X�G���^�[�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitPressEnter(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPressEnter != NULL)
	{
		g_pVtxBuffPressEnter->Release();
		g_pVtxBuffPressEnter = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTexturePressEnter != NULL)
	{
		g_pTexturePressEnter->Release();
		g_pTexturePressEnter = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �v���X�G���^�[�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdatePressEnter(void)
{
	// �_�ł����邽�߂ɃJ�E���g��i�߂�
	g_PressEnter.nCntFlash++;

	switch (g_PressEnter.bPressingEnter)
	{
	case true:
		// �G���^�[�L�[����������̏���
		AfterPreesingEnter();
		break;

	case false:
		// �G���^�[�L�[�������O�̏���
		BeforePressingEnter();
		break;
	}
	// ���_���W�̍X�V
	SetVertexPressEnter();
}

//--------------------------------------------------------------------------------------------------------------
// �v���X�G���^�[�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawPressEnter(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTexturePressEnter);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//--------------------------------------------------------------------------------------------------------------
// �G���^�[�L�[�������O�̏���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void BeforePressingEnter(void)
{
	if (PREESENTER_CLEAR_NUMBER == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL)
	{// ����
		g_PressEnter.col.a = 0.0f;
	}
	else if (PREESENTER_LESS_CLEAR_NUMBER == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL)
	{// �s����
		g_PressEnter.col.a = 1.0f;
	}

	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{// �������̂�true�ɂ���
		g_PressEnter.bPressingEnter = true;

		// �T�E���h
		PlaySound(SOUND_LABEL_SE_OK);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �G���^�[�L�[����������̏���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void AfterPreesingEnter(void)
{
	if (PREESENTER_CLEAR_NUMBER_HI == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL_HI)
	{// ����
		g_PressEnter.col.a = 0.0f;
	}
	else if (PREESENTER_LESS_CLEAR_NUMBER == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL_HI)
	{// �s����
		g_PressEnter.col.a = 1.0f;
	}
	// �J�E���g��������x�i�񂾂�J��
	if (g_PressEnter.nCntFlash >= TITLE_TRANSITION_COUNT)
	{// ���ɐi�񂾂Ƃ�����
	 // �Q�[���ɑJ�ڂ��鋖��^����
		g_PressEnter.bTutorialTransition = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g����ʂɓ\��v���X�G���^�[�̒��_���W�̐ݒ�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetVertexPressEnter(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_PressEnter.pos.x - g_PressEnter.fWidth, g_PressEnter.pos.y + g_PressEnter.fHieght, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_PressEnter.pos.x - g_PressEnter.fWidth, g_PressEnter.pos.y - g_PressEnter.fHieght, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_PressEnter.pos.x + g_PressEnter.fWidth, g_PressEnter.pos.y + g_PressEnter.fHieght, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_PressEnter.pos.x + g_PressEnter.fWidth, g_PressEnter.pos.y - g_PressEnter.fHieght, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_PressEnter.col;
	pVtx[1].col = g_PressEnter.col;
	pVtx[2].col = g_PressEnter.col;
	pVtx[3].col = g_PressEnter.col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPressEnter->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A����ʂɑJ�ڂ��邩�ǂ����n��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fbool	- �`���[�g���A����ʂɑJ�ڂ��邩�ǂ����n��
//--------------------------------------------------------------------------------------------------------------
bool bGetTutorialTransition(void)
{
	return g_PressEnter.bTutorialTransition;
}