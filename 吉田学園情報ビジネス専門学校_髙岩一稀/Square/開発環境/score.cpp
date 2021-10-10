//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "score.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// CScore�̃R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntNumber = 0; nCntNumber < MAX_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}
	m_nScore = 0;
	m_nDigit = 1;
}

//=============================================================================
// CScore�̃f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ��������
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement)
{
	// �|�C���^�ϐ��̐���
	CScore *pScore;
	pScore = new CScore(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pScore != NULL)
	{
		// ����������
		pScore->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pScore->SetObjType(OBJTYPE_SCORE);

		// �X�R�A�z�u�̎�ނ�ۑ�
		pScore->m_scorePlacement = scorePlacement;

		// �X�R�A�z�u�Ԋu��ۑ�
		pScore->m_fSpace = fSpace;
	}

	return pScore;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{	
	// CScene2D�̏���������
	m_apNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x,pos.y,0.0f), size);
	m_nScore = 0;
	m_nDigit = 1;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// CScene2D�̏I������
		m_apNumber[nCntNumber]->Uninit();
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// CScene2D�̏I������
		m_apNumber[nCntNumber]->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// CScene2D�̕`�揈��
		m_apNumber[nCntNumber]->Draw();
	}
}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void CScore::AddScore(int nValue)
{
	// �X�R�A��ۑ�
	m_nScore += nValue;
	
	// �X�R�A�̕`��
	Placementtype();
}

//=============================================================================
// �X�R�A�̐ݒ�
//=============================================================================
void CScore::SetScore(int nScore)
{
	// �X�R�A��ۑ�
	m_nScore = nScore;

	// �X�R�A�̕`��
	Placementtype();
}

//=============================================================================
// �X�R�A�z�u�̎�ނ��Ƃɏ���
//=============================================================================
void CScore::Placementtype(void)
{
	// �X�R�A�z�u�̎�ނ��Ƃɏ���
	switch (m_scorePlacement)
	{
		// �X�R�A�̉E�񂹔z�u
	case SCOREPLACEMENT_RIGHT:
		RightJustified();
		break;
		// �X�R�A�̍��񂹔z�u
	case SCOREPLACEMENT_LEFT:
		LeftJustified();
		break;
	}
}

//=============================================================================
// �X�R�A�̉E�񂹔z�u
//=============================================================================
void CScore::RightJustified(void)
{
	// ������������
	m_nDigit = 1;

	// �������v�Z
	for (int nCntDigit = 1; nCntDigit < MAX_DIGIT; nCntDigit++)
	{
		int nNumber = (m_nScore / (int)powf(10.0f, (float)nCntDigit));

		if (nNumber <= 0)
		{
			// 0�ȉ��̎��I���
			break;
		}
		else
		{
			// �����̑���
			m_nDigit++;

			if (m_apNumber[m_nDigit - 1] == NULL)
			{
				// ���NULL�Ȃ琶��
				D3DXVECTOR3 pos = m_apNumber[nCntDigit - 1]->GetPosition();
				D3DXVECTOR2 size = m_apNumber[nCntDigit - 1]->GetSize();
				m_apNumber[m_nDigit - 1] = CNumber::Create(D3DXVECTOR3(pos.x - m_fSpace, pos.y, pos.z), size);
			}
		}
	}

	// �X�R�A���i�[
	for (int nCntNumber = m_nDigit - 1; nCntNumber > 1; nCntNumber--)
	{
		// ��������i�[
		int nNumber = (m_nScore / (int)powf(10.0f, (float)((nCntNumber))));

		// �i���o�[�Z�b�g
		m_apNumber[nCntNumber]->SetNumber(nNumber);
	}
}

//=============================================================================
// �X�R�A�̍��񂹔z�u
//=============================================================================
void CScore::LeftJustified(void)
{
	// ������������
	m_nDigit = 1;

	// �������v�Z
	for (int nCntDigit = 1; nCntDigit < MAX_DIGIT; nCntDigit++)
	{
		// �����̕ۑ�
		int nNumber = (m_nScore / (int)powf(10.0f, (float)nCntDigit));

		if (nNumber <= 0)
		{
			// 0�ȉ��̎��I���
			break;
		}
		else
		{
			// �����̑���
			m_nDigit++;

			if (m_apNumber[m_nDigit - 1] == NULL)
			{
				// ���NULL�Ȃ琶��
				D3DXVECTOR3 pos = m_apNumber[nCntDigit - 1]->GetPosition();
				D3DXVECTOR2 size = m_apNumber[nCntDigit - 1]->GetSize();
				m_apNumber[m_nDigit - 1] = CNumber::Create(D3DXVECTOR3(pos.x + m_fSpace, pos.y, 0.0f), size);
			}
		}
	}

	// �X�R�A���i�[
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// ��������i�[
		int nNumber = (m_nScore / (int)powf(10.0f, (float)(m_nDigit - (nCntNumber + 1))));

		// �i���o�[�Z�b�g
		m_apNumber[nCntNumber]->SetNumber(nNumber);
	}
}