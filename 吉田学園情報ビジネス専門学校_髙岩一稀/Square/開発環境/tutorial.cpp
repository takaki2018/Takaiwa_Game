//-----------------------------------------------------------------
//
// �`���[�g���A����� (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "tutorial.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
#include "logo.h"
#include "input_joypad.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN				(5)					// �A�j���[�V�����p�^�[��No.�̍ő吔

#define PAUSE_MENU_BG_X			(SCREEN_WIDTH / 2)	// �|�[�Y���j���[�̒��S���WX
#define PAUSE_MENU_BG_Y			(370.0f)			// �|�[�Y���j���[�̒��S���WY
#define PAUSE_MENU_BG_WIDTH		(400.0f)			// �|�[�Y���j���[�̕�
#define PAUSE_MENU_BG_HEIGHT	(400.0f)			// �|�[�Y���j���[�̍���

#define LOGO_X					(SCREEN_WIDTH / 2)	// ���S�̒��S���WX(���ׂċ���)
#define LOGO_Y					(310.0f)			// ���S�̒��S���WY(���ׂċ���)
#define LOGO_WIDTH				(280.0f)			// ���S�̕�(���ׂċ���)
#define LOGO_HEIGHT				(70.0f)				// ���S�̍���(���ׂċ���)

#define ALUFA_INTERBAL			(10)				// �_�ł̃C���^�[�o��

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[CTutorial::TEXTURE_MAX] = {};

//=============================================================================
// CTutorial�̃R���X�g���N�^
//=============================================================================
CTutorial::CTutorial(int nPriority) :CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntTutorialUI = 0; nCntTutorialUI < TUTORIALUI_MAX; nCntTutorialUI++)
	{
		m_apTutorial[nCntTutorialUI] = NULL;
	}
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		m_apCursor[nCntCursor] = NULL;
	}
	m_pLogo = NULL;
	m_nCursor = (int)TUTORIALUI_KEYBOARD;
}

//=============================================================================
// CTutorial�̃f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CTutorial::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_keyboard.png", &m_pTexture[TEXTURE_KEYBOARD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_gamescreen.png", &m_pTexture[TEXTURE_GAMESCREEN]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_game.png", &m_pTexture[TEXTURE_GAME]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/marker.png", &m_pTexture[TEXTURE_CURSOR]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CTutorial::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < TEXTURE_MAX; nCntTexture++)
	{
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CTutorial *CTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CTutorial *pTutorial;
	pTutorial = new CTutorial(CScene::PRIORITY_TUTORIAL);

	// NULL�`�F�b�N
	if (pTutorial != NULL)
	{
		// ����������
		pTutorial->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pTutorial->SetObjType(OBJTYPE_TUTORIAL);
	}

	return pTutorial;
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ϐ��錾
	// �|�[�Y��ʂɎg�p����UI�̈ʒu
	D3DXVECTOR3 aPos[TUTORIALUI_MAX] = { D3DXVECTOR3(pos.x,pos.y,pos.z),
		D3DXVECTOR3(pos.x,340.0f,pos.z),
		D3DXVECTOR3(pos.x,340.0f,pos.z),
		D3DXVECTOR3(pos.x,340.0f,pos.z) };

	// �|�[�Y��ʂɎg�p����UI�̈ʒu
	D3DXVECTOR2 aSize[TUTORIALUI_MAX] = { D3DXVECTOR2(size.x,size.y),
		D3DXVECTOR2(960.0f,540.0f),
		D3DXVECTOR2(960.0f,540.0f),
		D3DXVECTOR2(960.0f,540.0f) };

	for (int nCntTutorialUI = 0; nCntTutorialUI < TUTORIALUI_MAX; nCntTutorialUI++)
	{
		// �������̊m��
		m_apTutorial[nCntTutorialUI] = new CScene2D(CScene::PRIORITY_NONE);

		// CScene2D�̏���������
		m_apTutorial[nCntTutorialUI]->Init(aPos[nCntTutorialUI], aSize[nCntTutorialUI]);

		// �e�N�X�`���̊��蓖��
		if (m_pTexture[nCntTutorialUI] != NULL)
		{
			m_apTutorial[nCntTutorialUI]->BindTexture(m_pTexture[nCntTutorialUI]);
		}
	}
	// �`���[�g���A����ʂ̌��𔖂�����
	m_apTutorial[TUTORIALUI_NONE]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));

	// �I������Ă���UI�̕ۑ�
	m_nCursor = (int)TUTORIALUI_KEYBOARD;

	// �eUI�̐F��ݒ�
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		// �������̊m��
		m_apCursor[nCntCursor] = new CScene2D(CScene::PRIORITY_NONE);

		// �J�[�\���̏���������
		m_apCursor[nCntCursor]->Init(D3DXVECTOR3(580.0f + 60.0f * nCntCursor, 650.0f, 0.0f), D3DXVECTOR2(20.0f, 20.0f));

		// �e�N�X�`���̊��蓖��
		if (m_pTexture[TEXTURE_CURSOR] != NULL)
		{
			m_apCursor[nCntCursor]->BindTexture(m_pTexture[TEXTURE_CURSOR]);
		}

		if (m_nCursor == nCntCursor)
		{
			// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ����ȊO�̓��l0.3f�ɐݒ�
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}
	// ���S�̏�����
	m_pLogo = CLogo::Create(D3DXVECTOR3(1100.0f,650.0f,0.0f),D3DXVECTOR2(200.0f,50.0f),CLogo::TEXTURETYPE_PRESSENTER);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CTutorial::Uninit(void)
{
	for (int nCntTutorialUI = 0; nCntTutorialUI < TUTORIALUI_MAX; nCntTutorialUI++)
	{
		// CScene2D�̏I������
		if (m_apTutorial[nCntTutorialUI] != NULL)
		{
			m_apTutorial[nCntTutorialUI]->Uninit();
		}
	}
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		if (m_apCursor[nCntCursor] != NULL)
		{
			m_apCursor[nCntCursor]->Uninit();
		}
	}
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CTutorial::Update(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *pTrigger = CManager::GetInputJoypad()->GetStickTrigger(PLAYER_1);

	// �t�F�[�h���̎擾
	CFade *pFade = CManager::GetFade();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_LEFT) || pTrigger[CInputJoypad::CROSSKEYTRIGGER_LEFT])
	{
		// ��ɂ����Ƃ��ԍ������炷
		m_nCursor--;

		if (m_nCursor < TUTORIALUI_KEYBOARD)
		{
			// �R���e�B�j���[��菬�����Ȃ����Ƃ�
			m_nCursor = (TUTORIALUI_MAX - 1);
		}
		// ���ʉ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
	}
	else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_RIGHT) || pTrigger[CInputJoypad::CROSSKEYTRIGGER_RIGHT])
	{
		// ���ɂ����Ƃ��ԍ��𑝂₷
		m_nCursor++;

		if (m_nCursor > (TUTORIALUI_MAX - 1))
		{
			// �N�C�b�g���傫���Ȃ����Ƃ�
			m_nCursor = TUTORIALUI_KEYBOARD;
		}
		// ���ʉ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
	}

	// �eUI�̐F��ݒ�
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		if ((m_nCursor - 1) == nCntCursor)
		{
			// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ����ȊO�̓��l0.3f�ɐݒ�
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}
	if (m_nCursor == TUTORIALUI_MAX - 1)
	{
		if (m_pLogo != NULL)
		{
			m_pLogo->Update();

			m_pLogo->StartBlinking(20);
		}
	}
	// �J�[�\�����Ōォ�G���^�[�L�[���������Ƃ��`���[�g���A����ʂ����
	if (m_nCursor == TUTORIALUI_GAME && (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN)))
	{
		// false�ɂ���
		CManager::SetTutorial(false);

		// �I������
		Uninit();

		return;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CTutorial::Draw(void)
{
	// �`���[�g���A���̕`�揈��
	m_apTutorial[TUTORIALUI_NONE]->Draw();
	m_apTutorial[m_nCursor]->Draw();
		
	// �J�[�\���̕`��
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		m_apCursor[nCntCursor]->Draw();
	}
	if (m_nCursor == TUTORIALUI_MAX - 1)
	{
		m_pLogo->Draw();
	}
}