//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : 
//
//=============================================================================
#include "scene.h"
#include "manager.h"
#include "count_down.h"
#include "camera.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CScene *CScene::m_pTop[PRIORITY_MAX] = {};
CScene *CScene::m_pCur[PRIORITY_MAX] = {};

//=============================================================================
// CScene�̃f�t�H���g�R���X�g���N�^
//=============================================================================
CScene::CScene()
{
	// �擪��NULL����Ȃ��Ƃ�
	if (m_pTop[PRIORITY_NONE] == NULL)
	{
		// ���X�g�ɒǉ�����
		this->m_pNext = NULL;
		this->m_pPrev = NULL;

		// �擪�ɏ���ۑ�����
		m_pTop[PRIORITY_NONE] = this;
	}
	else
	{
		// ���X�g�ɒǉ�����
		m_pCur[PRIORITY_NONE]->m_pNext = this;
		this->m_pPrev = m_pCur[PRIORITY_NONE];
		this->m_pNext = NULL;
	}

	// �Ō���ɏ���ۑ�����
	m_pCur[PRIORITY_NONE] = this;

	// ���S�t���O�̏�����
	m_bDeath = false;
}

//=============================================================================
// CScene�̃R���X�g���N�^(�I�[�o�[���[�h)
//=============================================================================
CScene::CScene(int nPriority)
{
	// �擪��NULL����Ȃ��Ƃ�
	if (m_pTop[nPriority] == NULL)
	{
		// ���X�g�ɒǉ�����
		this->m_pNext = NULL;
		this->m_pPrev = NULL;

		// �擪�ɏ���ۑ�����
		m_pTop[nPriority] = this;
	}
	else 
	{
		// ���X�g�ɒǉ�����
		m_pCur[nPriority]->m_pNext = this;
		this->m_pPrev = m_pCur[nPriority];
		this->m_pNext = NULL;
	}
	// �Ō���ɏ���ۑ�����
	m_pCur[nPriority] = this;

	// ���S�t���O�̏�����
	m_bDeath = false;
}

//=============================================================================
// CScene�̃f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// �I������
//=============================================================================
void CScene::ReleaseAll(void)
{
	// �I������
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g���̎擾
		CScene *pScene = m_pTop[nCntPriority];

		// NULL�ɂȂ�܂Ń��[�v
		while (pScene)
		{
			// ���̏����擾
			CScene *pSceneNext = pScene->m_pNext;

			// �X�V����
			pScene->Uninit();

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
	// ���S�t���O���`�F�b�N���ăI�u�W�F�N�g��j��
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g���̎擾
		CScene *pScene = m_pTop[nCntPriority];

		// NULL�ɂȂ�܂Ń��[�v
		while (pScene)
		{
			// ���̏����擾
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_bDeath)
			{
				// ���X�g����폜
				if (pScene->m_pPrev != NULL)
				{
					pScene->m_pPrev->m_pNext = pScene->m_pNext;
				}
				if (pScene->m_pNext != NULL)
				{
					pScene->m_pNext->m_pPrev = pScene->m_pPrev;
				}

				if (pScene == m_pCur[nCntPriority])
				{
					m_pCur[nCntPriority] = pScene->m_pPrev;
				}
				if (pScene == m_pTop[nCntPriority])
				{
					m_pTop[nCntPriority] = pScene->m_pNext;
				}

				delete pScene;
				pScene = NULL;
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene::UpdateAll(void)
{
	// ���[�h��Ԃ��擾
	CManager::MODE mode = CManager::GetMode();

	switch (mode)
	{
	case CManager::MODE_TITLE:
		UpdateTitle();
		break;

	case CManager::MODE_GAME:
		UpdateGame();
		break;

	case CManager::MODE_RESULT:
		UpdateResult();
		break;
	}

	// �j�����邩�ǂ���
	ReleaseScene();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene::DrawAll(void)
{
	// �J�����̐ݒ�
	CManager::GetCamera()->SetCamera();

	// �|�[�Y�����擾
	bool bPause = CManager::GetPause();

	// �`�揈��
	if (bPause == true)
	{
		for (int nCntPriority = 1; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			// �擪�I�u�W�F�N�g���̎擾
			CScene *pScene = m_pTop[nCntPriority];

			// NULL�ɂȂ�܂Ń��[�v
			while (pScene)
			{
				// ���̏����擾
				CScene *pSceneNext = pScene->m_pNext;

				// �X�V����
				pScene->Draw();

				// ���̏��ɂ���
				pScene = pSceneNext;
			}
		}
	}
	else
	{
		for (int nCntPriority = 1; nCntPriority < PRIORITY_PAUSE; nCntPriority++)
		{
			// �擪�I�u�W�F�N�g���̎擾
			CScene *pScene = m_pTop[nCntPriority];

			// NULL�ɂȂ�܂Ń��[�v
			while (pScene)
			{
				// ���̏����擾
				CScene *pSceneNext = pScene->m_pNext;

				// �X�V����
				pScene->Draw();

				// ���̏��ɂ���
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// �ʏI������
//=============================================================================
void CScene::Release(void)
{
	// ���S�t���O�𗧂Ă�
	this->m_bDeath = true;
}

//=============================================================================
// ���Ԗڂ̃I�u�W�F�N�g���擾���邩
//=============================================================================
CScene *CScene::GetScene(int nPriority, int nCnt)
{
	// ���Ԗڂ��v�Z�p
	int nCounter = 0;

	// �擪�I�u�W�F�N�g���̎擾
	CScene *pScene = m_pTop[nPriority];

	// NULL�ɂȂ�܂Ń��[�v
	while (pScene)
	{
		// ���̏����擾
		CScene *pSceneNext = pScene->m_pNext;

		// ���̏��ɂ���
		pScene = pSceneNext;

		// ���Ԗڂ�
		nCounter++;

		// �w��񐔂܂ŗ�����I����
		if (nCounter == nCnt)
		{
			break;
		}
	}
	return pScene;
}

//=============================================================================
// �^�C�g���̍X�V����
//=============================================================================
void CScene::UpdateTitle(void)
{
	// �`���[�g���A����ʂ��J���Ă��邩
	bool bTutorial = CManager::GetTutorial();

	if (bTutorial == true)
	{
		// �擪�I�u�W�F�N�g���̎擾
		CScene *pScene = m_pTop[PRIORITY_TUTORIAL];

		// NULL�ɂȂ�܂Ń��[�v
		while (pScene)
		{
			// ���̏����擾
			CScene *pSceneNext = pScene->m_pNext;

			// �X�V����
			pScene->Update();

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
	else
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_TUTORIAL; nCntPriority++)
		{
			// �擪�I�u�W�F�N�g���̎擾
			CScene *pScene = m_pTop[nCntPriority];

			// NULL�ɂȂ�܂Ń��[�v
			while (pScene)
			{
				// ���̏����擾
				CScene *pSceneNext = pScene->m_pNext;

				if (pScene->m_bDeath == false)
				{
					// �X�V����
					pScene->Update();
				}

				// ���̏��ɂ���
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// �Q�[���̍X�V����
//=============================================================================
void CScene::UpdateGame(void)
{
	// �|�[�Y���̎擾
	bool bPause = CManager::GetPause();

	// �J�E���g�_�E�������擾����
	bool bFinishCountDown = CCountDown::GetFinishCountDown();

	if (bFinishCountDown == false)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_PAUSE; nCntPriority++)
		{
			if (nCntPriority == PRIORITY_TILE || nCntPriority == PRIORITY_COUNTDOWN || nCntPriority == PRIORITY_TILEREVIVAL)
			{
				// �擪�I�u�W�F�N�g���̎擾
				CScene *pScene = m_pTop[nCntPriority];

				// NULL�ɂȂ�܂Ń��[�v
				while (pScene)
				{
					// ���̏����擾
					CScene *pSceneNext = pScene->m_pNext;

					// �X�V����
					pScene->Update();

					// ���̏��ɂ���
					pScene = pSceneNext;
				}
			}
		}
	}
	else
	{
		if (bPause == true)
		{
			// �擪�I�u�W�F�N�g���̎擾
			CScene *pScene = m_pTop[PRIORITY_PAUSE];

			// NULL�ɂȂ�܂Ń��[�v
			while (pScene)
			{
				// ���̏����擾
				CScene *pSceneNext = pScene->m_pNext;

				// �X�V����
				pScene->Update();

				// ���̏��ɂ���
				pScene = pSceneNext;
			}
		}
		else
		{
			for (int nCntPriority = 0; nCntPriority < PRIORITY_PAUSE; nCntPriority++)
			{
				// �擪�I�u�W�F�N�g���̎擾
				CScene *pScene = m_pTop[nCntPriority];

				// NULL�ɂȂ�܂Ń��[�v
				while (pScene)
				{
					// ���̏����擾
					CScene *pSceneNext = pScene->m_pNext;

					if (pScene->m_bDeath == false)
					{
						// �X�V����
						pScene->Update();
					}

					// ���̏��ɂ���
					pScene = pSceneNext;
				}
			}
		}
	}
}

//=============================================================================
// ���U���g�̍X�V����
//=============================================================================
void CScene::UpdateResult(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g���̎擾
		CScene *pScene = m_pTop[nCntPriority];

		// NULL�ɂȂ�܂Ń��[�v
		while (pScene)
		{
			// ���̏����擾
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_bDeath == false)
			{
				// �X�V����
				pScene->Update();
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �V�[���̔j������
//=============================================================================
void CScene::ReleaseScene(void)
{
	// ���S�t���O���`�F�b�N���ăI�u�W�F�N�g��j��
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g���̎擾
		CScene *pScene = m_pTop[nCntPriority];

		// true�̂Ƃ��폜
		while (pScene)
		{
			// ���̏����擾
			CScene *pSceneNext = pScene->m_pNext;

			// ���S�t���O�������Ă���Ƃ�
			if (pScene->m_bDeath)
			{
				// ���X�g����폜
				if (pScene->m_pPrev != NULL)
				{
					pScene->m_pPrev->m_pNext = pScene->m_pNext;
				}
				if (pScene->m_pNext != NULL)
				{
					pScene->m_pNext->m_pPrev = pScene->m_pPrev;
				}

				if (pScene == m_pCur[nCntPriority])
				{
					m_pCur[nCntPriority] = pScene->m_pPrev;
				}
				if (pScene == m_pTop[nCntPriority])
				{
					m_pTop[nCntPriority] = pScene->m_pNext;
				}

				delete pScene;
				pScene = NULL;
			}
			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}