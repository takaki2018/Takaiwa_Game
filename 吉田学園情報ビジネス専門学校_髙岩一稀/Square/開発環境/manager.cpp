//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"

#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "sound.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "fade.h"
#include "pause.h"
#include "logo.h"
#include "bg.h"
#include "number.h"
#include "tile.h"
#include "gamedata_keep_object.h"
#include "count_down.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CSound *CManager::m_pSound = NULL;
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CManager::MODE CManager::m_mode = MODE_TITLE;
CFade *CManager::m_pFade = NULL;
bool CManager::m_bPause = false;
bool CManager::m_bTutorial = false;
CGameDataKeepObject *CManager::m_pGemeData = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;

//=============================================================================
// CManager�̃R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// CManager�̃f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	// ���ݎ����ɂ���ă����_���ɐݒ肷��
	srand((unsigned)time(NULL));

	// �����_���[�̏���������
	m_pRenderer = new CRenderer;

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	// �L�[�{�[�h�̏���������
	m_pInputKeyboard = new CInputKeyboard;

	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// �W���C�p�b�h���̏���������
	m_pInputJoypad = new CInputJoypad;

	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Init(hInstance, hWnd);
	}

	// �T�E���h�̏���������
	m_pSound = new CSound;

	if (m_pSound != NULL)
	{
		m_pSound->Init(hWnd);
	}

	// �t�F�[�h�̏���������
	m_pFade = new CFade;

	if (m_pFade != NULL)
	{
		m_pFade->Init(m_mode);
	}

	// �Q�[���f�[�^��ۑ�����I�u�W�F�N�g�̍쐬
	m_pGemeData = new CGameDataKeepObject;

	if (m_pGemeData != NULL)
	{
		m_pGemeData->Init();
	}

	// �J�����̏���������
	m_pCamera = new CCamera;

	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	// ���C�g�̏���������
	m_pLight = new CLight;

	if (m_pLight != NULL)
	{
		m_pLight->Init();
	}

#ifdef _DEBUG
	CDebugProc *pDebug = new CDebugProc;
	pDebug->Init();
#endif

	// �e�N�X�`���̃��[�h
	LoadTexture();

	// ���[�h�̌Ăяo��
	SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �e�N�X�`���̃A�����[�h
	UnloadTexture();

	// �T�E���h�̏I������
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// �W���C�p�b�h�̏I������
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// �L�[�{�[�h�̏I������
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �t�F�[�h�̏I������
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// �����_���̏I������
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �I�u�W�F�N�g�̏I������
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		break;

	case MODE_GAME:
		// �|�[�Y����
		if ((m_pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) || 
			m_pInputJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_PAUSE)) && 
			CCountDown::GetFinishCountDown())
		{
			SetPause();
		}
		break;
	}

	// �L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �W���C�p�b�h�̍X�V����
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	// �����_���̍X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// �I�u�W�F�N�g�̍X�V����
	CScene::UpdateAll();

	// �t�F�[�h�̍X�V����
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	m_pCamera->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �����_���̕`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{// �^�C�g����ʂ̏I������
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
		}
		// �`���[�g���A�����̏I������
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
		}
		break;

		// �Q�[����ʂ̏I������
	case MODE_GAME:
		if (m_pGame!= NULL)
		{
			m_pGame->Uninit();
		}
		break;

		// ���U���g��ʂ̏I������
	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
		}
		break;
	}
	// �I�u�W�F�N�g�̍폜
	CScene::ReleaseAll();

	// �|�[�Y��Ԃ�false�ɂ���
	SetPause(false);

	// ���[�h�̈ڍs
	m_mode = mode;

	switch (mode)
	{// �^�C�g����ʂ̏���������
	case MODE_TITLE:
		m_pTitle = new CTitle;
		if (m_pTitle != NULL)
		{
			m_pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		break;

		// �Q�[����ʂ̏���������
	case MODE_GAME:
		m_pGame = new CGame;
		if (m_pGame != NULL)
		{
			m_pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		break;

		// ���U���g��ʂ̏���������
	case MODE_RESULT:
		m_pResult = new CResult;
		if (m_pResult != NULL)
		{
			m_pResult->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		break;
	}
}

//=============================================================================
// �e�N�X�`���̃��[�h
//=============================================================================
void CManager::LoadTexture(void)
{
	// ���S�̃e�N�X�`�����[�h
	CLogo::Load();

	// �w�i�̃e�N�X�`�����[�h
	CBg::Load();

	// �����̃e�N�X�`�����[�h
	CNumber::Load();

	// �^�C���̃e�N�X�`�����[�h
	CTile::Load();

	// �`���[�g���A���̃e�N�X�`�����[�h
	CTutorial::Load();
}

//=============================================================================
// �e�N�X�`���̃A�����[�h
//=============================================================================
void CManager::UnloadTexture(void)
{
	// ���S�̃e�N�X�`���A�����[�h
	CLogo::Unload();

	// �w�i�̃e�N�X�`���A�����[�h
	CBg::Unload();

	// �����̃e�N�X�`���A�����[�h
	CNumber::Unload();

	// �^�C���̃e�N�X�`���A�����[�h
	CTile::Unload();

	// �`���[�g���A���̃e�N�X�`���A�����[�h
	CTutorial::Unload();
}

//=============================================================================
// �����_���[���̎擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// �L�[�{�[�h���̎擾
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// �W���C�p�b�h���̎擾
//=============================================================================
CInputJoypad *CManager::GetInputJoypad(void)
{
	return m_pInputJoypad;
}

//=============================================================================
// �|�[�Y�̏���
//=============================================================================
void CManager::SetPause(void)
{
	if (m_pFade->GetFade() == CFade::FADE_NONE)
	{
		m_bPause = m_bPause ? false : true;

		if (m_bPause == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_PAUSE_IN);
		}
		else if (m_bPause == false)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_PAUSE_OUT);
		}
	}
}