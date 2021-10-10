//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "tile.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[EFFECT_TYPE_MAX] = {};		// �e�N�X�`�����̃|�C���^

//=============================================================================
// CEffect�̃R���X�g���N�^
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{
	// �ϐ��̏�����
	m_effect.move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_effect.col = D3DXCOLOR(0.220f,0.0f,0.0f,0.0f);
	m_effect.fRemove = 0.0f;
	m_effect.nCntAlpha = 0;
	m_effect.nLife = 0;
	m_effect.nInterval = 0;
}

//=============================================================================
// CEffect�̃f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CEffect::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_explosion.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_tile.jpg", &m_pTexture[2]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CEffect::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < EFFECT_TYPE_MAX;nCntTexture++)
	{
		// �e�N�X�`���̔j��
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
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, EFFECT_TYPE type, int nInterval)
{
	// �|�C���^�ϐ��̐���
	CEffect *pEffect;
	pEffect = new CEffect(CScene::PRIORITY_EFFECT);

	// NULL�`�F�b�N
	if (pEffect != NULL)
	{
		// ����������
		pEffect->Init(pos, move, col, fRemove,size,fCntAlpha,nLife,nInterval);

		// �e�N�X�`���ݒ�
		pEffect->BindTexture(m_pTexture[type]);
	}

	return pEffect;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, int nInterval)
{
	// �����o�ϐ��̏�����
	m_effect.move = move;
	m_effect.col = col;
	m_effect.fRemove = fRemove;
	m_effect.nCntAlpha = fCntAlpha;
	m_effect.nLife = nLife;
	m_effect.nInterval = nInterval;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_EFFECT);

	// ���_���W�̐ݒ�
	SetPosition(pos);

	// �F�̍X�V
	SetCol(m_effect.col);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	if (m_effect.nInterval <= 0)
	{
		// ���S���W�̍X�V
		pos.x += m_effect.move.x;
		pos.y += m_effect.move.y;

		// �ړ��ʂ̍X�V
		m_effect.move.x *= m_effect.fRemove;
		m_effect.move.y *= m_effect.fRemove;

		// ���l�̉��Z
		m_effect.col.a -= m_effect.nCntAlpha;

		// �����̍X�V
		m_effect.nLife--;

		// �F�̍X�V
		SetCol(m_effect.col);

		// �I�u�W�F�N�g���̎擾
		CScene *pScene = GetScene(CScene::PRIORITY_TILE);

		// �G�Ƃ̓����蔻��
		while (pScene)
		{
			// ���̏����擾
			CScene *pSceneNext = pScene->GetNext();

			// �I�u�W�F�N�g�̎�ނ̎擾
			CScene::OBJTYPE objType = pScene->GetObjType();

			// �I�u�W�F�N�g�̎�ނ�Enemy�������珈��
			if (objType == CScene::OBJTYPE_TILE)
			{
				// �G�C���X�^���X���̎擾
				CTile *pTile = (CTile*)pScene;

				switch (pTile->GetTileType())
				{
				case CTile::TILETYPE_INVISIBLE:
					// �^�C���Ƃ̓����蔻��
					pTile->EffectReflection(&pos, &m_effect.move, size);
					break;
				}
			}
			// ���̏���
			pScene = pSceneNext;
		}

		// ���_���W�̐ݒ�
		SetPosition(pos);

		// �G�t�F�N�g�̍폜
		if (m_effect.nLife <= 0 || m_effect.col.a < 0.0f)
		{
			Uninit();

			return;
		}
	}
	else
	{
		// �J�E���g�����炷
		m_effect.nInterval--;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}