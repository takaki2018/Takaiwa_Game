//=============================================================================
//
// �^�C�g�����S���� [title_logo.cpp]
// Author : 
//
//=============================================================================
#include "title_logo.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "input_keyboard.h"
#include "ui_bg.h"
#include "title.h"
#include "sound.h"
#include "tutorial.h"
#include "fade.h"
#include "input_joypad.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CTitleLogo::bTransition = false;

//=============================================================================
// CTitleLogo�̃R���X�g���N�^
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntTitleLogo = 0; nCntTitleLogo < LOGOTYPE_MAX; nCntTitleLogo++)
	{
		m_apLogo[nCntTitleLogo] = NULL;
	}
	m_nSelectLogo = LOGOTYPE_PLAY;
	bTransition = false;
}

//=============================================================================
// CTitleLogo�̃f�X�g���N�^
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
// ��������
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CTitleLogo *pTitleLogo;
	pTitleLogo = new CTitleLogo(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pTitleLogo != NULL)
	{
		// ����������
		pTitleLogo->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pTitleLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pTitleLogo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ���S�w�i�����擾
	CUIBg **apLogoBg = CTitle::GetLogoBg();

	// ���S�̏���������
	m_apLogo[LOGOTYPE_TITLE] = CLogo::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR2(300.0f, 80.0f), CLogo::TEXTURETYPE_TITLELOGO);
	m_apLogo[LOGOTYPE_PLAY] = CLogo::Create(apLogoBg[LOGOTYPE_PLAY - 1]->GetPosition(), D3DXVECTOR2(95.0f,30.0f), CLogo::TEXTURETYPE_PLAY);
	m_apLogo[LOGOTYPE_TUTORIAL] = CLogo::Create(apLogoBg[LOGOTYPE_TUTORIAL - 1]->GetPosition(), D3DXVECTOR2(200.0f, 30.0f), CLogo::TEXTURETYPE_TUTORIAL);
	m_apLogo[LOGOTYPE_RANKING] = CLogo::Create(apLogoBg[LOGOTYPE_RANKING - 1]->GetPosition(), D3DXVECTOR2(150.0f, 30.0f), CLogo::TEXTURETYPE_RANKING);
	m_apLogo[LOGOTYPE_OPTION] = CLogo::Create(apLogoBg[LOGOTYPE_OPTION - 1]->GetPosition(), D3DXVECTOR2(150.0f, 30.0f), CLogo::TEXTURETYPE_OPTION);

	m_nSelectLogo = LOGOTYPE_PLAY;

	// �e���S�̐F��ݒ�
	for (int nCntLogo = LOGOTYPE_PLAY; nCntLogo <= LOGOTYPE_OPTION; nCntLogo++)
	{
		if (m_nSelectLogo == nCntLogo)
		{
			// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
			m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ����ȊO�̓��l0.3f�ɐݒ�
			m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitleLogo::Uninit(void)
{
	// ���S�̏I������
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Uninit();
		}
	}

	// �j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitleLogo::Update(void)
{
	// ���S�w�i�����擾
	CUIBg **apLogoBg = CTitle::GetLogoBg();

	// ���S�̍X�V����
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Update();
		}
	}

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
			m_nSelectLogo--;

			if (m_nSelectLogo < LOGOTYPE_PLAY)
			{
				// �R���e�B�j���[��菬�����Ȃ����Ƃ�
				m_nSelectLogo = LOGOTYPE_OPTION;
			}
			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || pTrigger[CInputJoypad::CROSSKEYTRIGGER_DOWN])
		{
			// ���ɂ����Ƃ��ԍ��𑝂₷
			m_nSelectLogo++;

			if (m_nSelectLogo > LOGOTYPE_OPTION)
			{
				// �N�C�b�g���傫���Ȃ����Ƃ�
				m_nSelectLogo = LOGOTYPE_PLAY;
			}
			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}

		// �e���S�̐F��ݒ�
		for (int nCntLogo = LOGOTYPE_PLAY; nCntLogo <= LOGOTYPE_OPTION; nCntLogo++)
		{
			if (m_nSelectLogo == nCntLogo)
			{
				// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
				m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// ����ȊO�̓��l0.3f�ɐݒ�
				m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
				apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			}
		}

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN))
		{
			// �I������Ă���UI���Q�Ƃ��A�ǂ̏��������邩����
			switch (m_nSelectLogo)
			{
				// Play���S
			case LOGOTYPE_PLAY:
				bTransition = true;
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_TRANSITION);		// ���ʉ�
				break;
				// Tutorial���S
			case LOGOTYPE_TUTORIAL:
				// �`���[�g���A����ʂ̐�������
				CTutorial::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
				CManager::SetTutorial(true);
				break;
				// Ranking���S
			case LOGOTYPE_RANKING:
				break;
				// Option���S
			case LOGOTYPE_OPTION:
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitleLogo::Draw(void)
{
	// ���S�̕`�揈��
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Draw();
		}
	}
}