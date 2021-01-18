//-------------------------------------------------------------------
//
// �v���C���[����o��|�C���g (point.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "point.h"
#include "player.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_POINT			(256)					// �t���[�c�̍ő�l

#define POINT_SIZE_X		(20.0f)					// �t���[�c�̃T�C�Y(��)
#define POINT_SIZE_Y		(20.0f)					// �t���[�c�̃T�C�Y(����)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPoint = NULL;				// �G�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePoint[POINTTYPE_MAX] = {};	// �G�̒��_�o�b�t�@�̃|�C���^
POINTLOGO g_aPoint[MAX_POINT];									// �G�̏��

//-----------------------------------------------------------------
// �G�̏���������
//-----------------------------------------------------------------
HRESULT InitPoint(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_POINT; nCnt++)
	{
		g_aPoint[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPoint[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPoint[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aPoint[nCnt].fWidth = POINT_SIZE_X;
		g_aPoint[nCnt].fHeight = POINT_SIZE_Y;
		g_aPoint[nCnt].type = POINTTYPE_PLUS_ONE;
		g_aPoint[nCnt].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/plus_1.png", &g_pTexturePoint[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/plus_5.png", &g_pTexturePoint[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/decease_1.png", &g_pTexturePoint[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/decease_5.png", &g_pTexturePoint[3]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_POINT, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPoint,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_POINT; nCnt++)
	{
		// ���_���W�̐ݒ�
		SetVertexPoint(nCnt);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���̏��ɍX�V
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPoint->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �G�̏I������
//-----------------------------------------------------------------
void UninitPoint(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPoint != NULL)
	{
		g_pVtxBuffPoint->Release();
		g_pVtxBuffPoint = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < POINTTYPE_MAX; nCnt++)
	{
		if (g_pTexturePoint[nCnt] != NULL)
		{
			g_pTexturePoint[nCnt]->Release();
			g_pTexturePoint[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �G�̍X�V����
//-----------------------------------------------------------------
void UpdatePoint(void)
{
	// �\���̂̃|�C���^�ϐ�
	POINTLOGO *pPoint = &g_aPoint[0];

	for (int nCntPoint = 0; nCntPoint < MAX_POINT; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse == true)
		{
			// �傫���̍X�V
			pPoint->fWidth += 0.1f;
			pPoint->fHeight += 0.1f;

			// �ړ��ʂ̌���
			pPoint->move.y *= 0.95f;

			// �ʒu�̍X�V
			pPoint->pos += pPoint->move;

			// ���l�̌���
			pPoint->col.a -= 0.02f;

			// ���_���W�̍X�V
			SetVertexPoint(nCntPoint);

			// ���l��0.0f��菬�����Ȃ����Ƃ�
			if (pPoint->col.a <= 0.0f)
			{
				pPoint->col.a = 1.0f;				// ���l��������
				pPoint->fWidth = POINT_SIZE_X;		// ����������
				pPoint->fHeight = POINT_SIZE_Y;		// ������������
				pPoint->bUse = false;				// �g�p��Ԃ�false�ɂ���
			}
		}
	}
}

//-----------------------------------------------------------------
// �G�̕`�揈��
//-----------------------------------------------------------------
void DrawPoint(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPoint, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �o�E���h�̕`��
	for (int nCnt = 0; nCnt < MAX_POINT; nCnt++)
	{
		if (g_aPoint[nCnt].bUse == true)
		{
			pDevice->SetTexture(0, g_pTexturePoint[(int)(g_aPoint[nCnt].type)]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �G�̐ݒ�
//-----------------------------------------------------------------
void SetPoint(D3DXVECTOR3 pos, D3DXVECTOR3 move, POINTTYPE type)
{
	// �\���̂̃|�C���^�ϐ�
	POINTLOGO *pPoint = &g_aPoint[0];

	// �e�̐ݒ�
	for (int nCntPoint = 0; nCntPoint < MAX_POINT; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse == false)
		{// �ʒu�ݒ�
			pPoint->pos = pos;

			// �ړ��ʂ̐ݒ�
			pPoint->move = move;

			// �^�C�v�̊m��
			pPoint->type = type;

			// true�ŕԂ�
			pPoint->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �G�̒��_���W
//-----------------------------------------------------------------
void SetVertexPoint(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x - g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y + g_aPoint[nIdx].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x - g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y - g_aPoint[nIdx].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x + g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y + g_aPoint[nIdx].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x + g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y - g_aPoint[nIdx].fHeight, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_aPoint[nIdx].col;
	pVtx[1].col = g_aPoint[nIdx].col;
	pVtx[2].col = g_aPoint[nIdx].col;
	pVtx[3].col = g_aPoint[nIdx].col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPoint->Unlock();
}