//=============================================================================
//
// �n�C�X�R�A���� [hiscore.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "hiscore.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// CHiScore�̃R���X�g���N�^
//=============================================================================
CHiScore::CHiScore(int nPriority) : CScore(nPriority)
{
	// �ϐ��̏�����
	m_nHiScore = 0;
}

//=============================================================================
// CHiScore�̃f�X�g���N�^
//=============================================================================
CHiScore::~CHiScore()
{

}

//=============================================================================
// ��������
//=============================================================================
CHiScore *CHiScore::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement)
{
	// �|�C���^�ϐ��̐���
	CHiScore *pHiScore;
	pHiScore = new CHiScore(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pHiScore != NULL)
	{
		// ����������
		pHiScore->Init(pos, size,fSpace,scorePlacement);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pHiScore->SetObjType(OBJTYPE_SCORE);
	}

	return pHiScore;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CHiScore::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement)
{
	// �X�R�A�z�u�̎�ނ�ۑ�
	CScore::SetScorePlacement(scorePlacement);

	// �X�y�[�X�̕ۑ�
	CScore::SetSpace(fSpace);

	// CScore�̏���������
	CScore::Init(pos, size);

	// �n�C�X�R�A�̓ǂݍ���
	LoadHiScore();

	// �X�R�A�̐ݒ�
	CScore::SetScore(m_nHiScore);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CHiScore::Uninit(void)
{
	// CScore�̏I������
	CScore::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CHiScore::Update(void)
{
	// CScore�̍X�V����
	CScore::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CHiScore::Draw(void)
{  
	// CScore�̕`�揈��
	CScore::Draw();
}

//=============================================================================
// �n�C�X�R�A�̃��[�h����
//=============================================================================
void CHiScore::LoadHiScore(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXTFILE/HiScore.txt", "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_nHiScore);

		fclose(pFile);
	}
	else
	{

	}
}