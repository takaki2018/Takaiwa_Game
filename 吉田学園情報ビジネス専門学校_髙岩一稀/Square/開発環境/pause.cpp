//==============================================================================================================
//
// �|�[�Y (pause.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "pause.h"
#include "game.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
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
LPDIRECT3DTEXTURE9 CPause::m_pTexture[CPause::PAUSEUI_MAX] = {};

//=============================================================================
// CBullet�̃R���X�g���N�^
//=============================================================================
CPause::CPause(int nPriority):CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		m_apScene2D[nCntPauseUI] = NULL;
	}
	m_nPauseSelect = (int)PAUSEUI_CONTINUE;
}

//=============================================================================
// CPlayer�̃f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CPause::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_menu_bg.png", &m_pTexture[PAUSEUI_MENUBG]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/continue_logo.png", &m_pTexture[PAUSEUI_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/retry_logo.png", &m_pTexture[PAUSEUI_RETRY]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/quit_logo.png", &m_pTexture[PAUSEUI_QUIT]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPause::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < PAUSEUI_MAX; nCntTexture++)
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
CPause *CPause::Create(D3DXVECTOR3 pos,D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CPause *pPause;
	pPause = new CPause(CScene::PRIORITY_PAUSE);

	// NULL�`�F�b�N
	if (pPause != NULL)
	{
		// ����������
		pPause->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pPause->SetObjType(OBJTYPE_PAUSE);
	}

	return pPause;
}

//--------------------------------------------------------------------------------------------------------------
// �|�[�Y�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ϐ��錾
	// �|�[�Y��ʂɎg�p����UI�̈ʒu
	D3DXVECTOR3 aPos[PAUSEUI_MAX] = { D3DXVECTOR3(pos.x,pos.y,pos.z),
		D3DXVECTOR3(pos.x,PAUSE_MENU_BG_Y,pos.z),
		D3DXVECTOR3(pos.x,LOGO_Y,pos.z),
		D3DXVECTOR3(pos.x,LOGO_Y + 70.0f,pos.z),
		D3DXVECTOR3(pos.x,LOGO_Y + 140.0f,pos.z)};

	// �|�[�Y��ʂɎg�p����UI�̈ʒu
	D3DXVECTOR2 aSize[PAUSEUI_MAX] = { D3DXVECTOR2(size.x,size.y),
		D3DXVECTOR2(PAUSE_MENU_BG_WIDTH,PAUSE_MENU_BG_HEIGHT),
		D3DXVECTOR2(LOGO_WIDTH,LOGO_HEIGHT),
		D3DXVECTOR2(LOGO_WIDTH,LOGO_HEIGHT),
		D3DXVECTOR2(LOGO_WIDTH,LOGO_HEIGHT) };

	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		// �������̊m��
		m_apScene2D[nCntPauseUI] = new CScene2D(CScene::PRIORITY_NONE);

		// CScene2D�̏���������
		m_apScene2D[nCntPauseUI]->Init(aPos[nCntPauseUI], aSize[nCntPauseUI]);

		// �e�N�X�`���̊��蓖��
		if (m_pTexture[nCntPauseUI] != NULL)
		{
			m_apScene2D[nCntPauseUI]->BindTexture(m_pTexture[nCntPauseUI]);
		}
	}
	// �|�[�Y��ʂ̔w�i�̂ݐF��ς���
	m_apScene2D[PAUSEUI_NONE]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));

	// �I������Ă���UI�̕ۑ�
	m_nPauseSelect = (int)PAUSEUI_CONTINUE;

	// �eUI�̐F��ݒ�
	for (int nCntPauseUI = PAUSEUI_CONTINUE; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		if (m_nPauseSelect == nCntPauseUI)
		{
			// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
			m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ����ȊO�̓��l0.3f�ɐݒ�
			m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �|�[�Y�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CPause::Uninit(void)
{
	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		// CScene2D�̏I������
		m_apScene2D[nCntPauseUI]->Uninit();
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//--------------------------------------------------------------------------------------------------------------
// �|�[�Y�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CPause::Update(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *pTrigger = CManager::GetInputJoypad()->GetStickTrigger(PLAYER_1);

	// �t�F�[�h���̎擾
	CFade *pFade = CManager::GetFade();

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) || pTrigger[CInputJoypad::CROSSKEYTRIGGER_UP])
		{
			// ��ɂ����Ƃ��ԍ������炷
			m_nPauseSelect--;

			if (m_nPauseSelect < PAUSEUI_CONTINUE)
			{
				// �R���e�B�j���[��菬�����Ȃ����Ƃ�
				m_nPauseSelect = PAUSEUI_QUIT;
			}
			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || pTrigger[CInputJoypad::CROSSKEYTRIGGER_DOWN])
		{
			// ���ɂ����Ƃ��ԍ��𑝂₷
			m_nPauseSelect++;

			if (m_nPauseSelect > PAUSEUI_QUIT)
			{
				// �N�C�b�g���傫���Ȃ����Ƃ�
				m_nPauseSelect = PAUSEUI_CONTINUE;
			}
			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}

		// �eUI�̐F��ݒ�
		for (int nCntPauseUI = PAUSEUI_CONTINUE; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
		{
			if (m_nPauseSelect == nCntPauseUI)
			{
				// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
				m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// ����ȊO�̓��l0.3f�ɐݒ�
				m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			}
		}

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN))
		{
			// �I������Ă���UI���Q�Ƃ��A�ǂ̏��������邩����
			switch (m_nPauseSelect)
			{
				// �R���e�B�j���[�{�^��
			case PAUSEUI_CONTINUE:
				CManager::SetPause(false);										// �|�[�Y�����
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_PAUSE_OUT);	// ���ʉ�
				break;
				// ���g���C�{�^��
			case PAUSEUI_RETRY:
				pFade->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_START);	// ���ʉ�
				break;
				// �I���{�^��
			case PAUSEUI_QUIT:
				pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_START);	// ���ʉ�
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �|�[�Y�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CPause::Draw(void)
{
	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		// CScene2D�̕`�揈��
		m_apScene2D[nCntPauseUI]->Draw();
	}
}