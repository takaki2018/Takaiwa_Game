//=============================================================================
//
// ���C�g (light.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// CLight�̃R���X�g���N�^
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
// CLight�̃f�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void CLight::Init(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�ւ̃|�C���^
	D3DXVECTOR3 vecDir;		// �ݒ�p�����x�N�g��

	// �ϐ��̎󂯓n��
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�g���N���A����
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̕�����ݒ�
		switch (nCntLight)
		{
		case 0:
			// ���C�g�̊g�U���̐ݒ�
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// ���K������
			m_light[nCntLight].Direction = vecDir;
			break;

		case 1:
			// ���C�g�̊g�U���̐ݒ�
			m_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			vecDir = D3DXVECTOR3(-0.2f, 0.8f, -0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// ���K������
			m_light[nCntLight].Direction = vecDir;
			break;

		case 2:
			// ���C�g�̊g�U���̐ݒ�
			m_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			vecDir = D3DXVECTOR3(0.9f, -0.1f, 0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// ���K������
			m_light[nCntLight].Direction = vecDir;
			break;
		}
		// ���C�g��ݒ�
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::Uninit(void)
{

}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::Update(void)
{

}