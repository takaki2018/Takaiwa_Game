//-----------------------------------------------------------------
//
// �p�[�e�B�N�� (particle.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "particle.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PATTERN     (8)													// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(1)													// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)													// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define MAX_PARTICLE	(1024)												// �e�̍ő�l

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fRandius;			// ���a
	float fCntAlpha;		// ���l�̃J�E���g
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
} PARTICLE;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	// �p�[�e�B�N���̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;		// �p�[�e�B�N���̒��_�o�b�t�@�̃|�C���^
PARTICLE g_aParticle[MAX_PARTICLE];					// �p�[�e�B�N���̏��
D3DXVECTOR3 g_posBase;

//-----------------------------------------------------------------
// �����̏���������
//-----------------------------------------------------------------
HRESULT InitParticle(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureParticle);

	// �ϐ��̏�����
	g_posBase = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	// �����̏��̏�����
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRandius = NULL;
		g_aParticle[nCntParticle].fCntAlpha = NULL;
		g_aParticle[nCntParticle].nLife = NULL;
		g_aParticle[nCntParticle].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PARTICLE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// �e�̏������ɂ���
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffParticle->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �����̏I������
//-----------------------------------------------------------------
void UninitParticle(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}
}

//-----------------------------------------------------------------
// �����̍X�V����
//-----------------------------------------------------------------
void UpdateParticle(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	PARTICLE *pParticle;
	

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	// �����̍X�V
	pParticle = &g_aParticle[0];
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == true)
		{// ���S���W�̍X�V
			pParticle->pos.x += pParticle->move.x;
			pParticle->pos.y += pParticle->move.y;
			
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pParticle->pos.x - pParticle->fRandius, pParticle->pos.y + pParticle->fRandius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pParticle->pos.x - pParticle->fRandius, pParticle->pos.y - pParticle->fRandius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pParticle->pos.x + pParticle->fRandius, pParticle->pos.y + pParticle->fRandius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pParticle->pos.x + pParticle->fRandius, pParticle->pos.y - pParticle->fRandius, 0.0f);

			// ���_�J���[�̍X�V
			pVtx[0].col = pParticle->col;
			pVtx[1].col = pParticle->col;
			pVtx[2].col = pParticle->col;
			pVtx[3].col = pParticle->col;

			// ���l�̌v�Z
			pParticle->col.a -= pParticle->fCntAlpha;

			// �����̌v�Z
			pParticle->nLife--;

			// �����G�t�F�N�g�̍폜
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
				pParticle->nLife = 0;
			}
		}
		// ���̍X�V
		pVtx += 4;
	}

	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{// �A�h���X�̏�����
		pParticle = &g_aParticle[0];
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++, pParticle++)
		{
			if (pParticle->bUse == false)
			{// ���S���W�̐ݒ�
				pParticle->pos = g_posBase;
				
				// ���a�̐ݒ�
				pParticle->fRandius = (float)(rand() % 15 + 5);

				// ���l�̌����l
				pParticle->fCntAlpha = 0.01f;
				
				// �F�̐ݒ�
				pParticle->col = D3DXCOLOR(0.7f, 0.4f, 0.1f,1.0f);

				// �����̐ݒ�
				pParticle->nLife = 100;

				float fAngle; // ���ˊp�x
				// �p�x�̊m��
				fAngle = (float)(rand() % 628 - 314) / 100.0f;

				// �ړ��ʂ̐ݒ�
				pParticle->move.x = sinf(fAngle) * (float)((rand() % 15 + 1) * 0.1f);
				pParticle->move.y = cosf(fAngle) * (float)((rand() % 15 + 1) * 0.1f);

				// true�ŕԂ�
				pParticle->bUse = true;

				break;
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffParticle->Unlock();
}

//-----------------------------------------------------------------
// �����̕`�揈��
//-----------------------------------------------------------------
void DrawParticle(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �\�[�X�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// �f�X�e�B�l�[�V�����̍������@�̐���

															// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �����̕`��
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// �����G�t�F�N�g���g���Ă���Ƃ�����
		if (g_aParticle[nCntParticle].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureParticle);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �\�[�X�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V�����̍������@�̐���
}

