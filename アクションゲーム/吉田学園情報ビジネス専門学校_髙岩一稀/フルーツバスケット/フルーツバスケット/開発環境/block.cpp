//-------------------------------------------------------------------
//
// �u���b�N (block.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "block.h"
#include "player.h"
#include "bear.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_BLOCKTYPE		(2)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;			// �u���b�N�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBlock[MAX_BLOCKTYPE] = {};	// �u���b�N�̒��_�o�b�t�@�̃|�C���^
BLOCK g_aBlock[MAX_BLOCK];								// �u���b�N�̏��

//-----------------------------------------------------------------
// �u���b�N�̏���������
//-----------------------------------------------------------------
HRESULT InitBlock(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block001.jpg", &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block002.png", &g_pTextureBlock[1]);

	// �e���̏�����
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].nTimer = 0;
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].fTexU = 1.0f;
		g_aBlock[nCntBlock].fTexV = 1.0f;
		g_aBlock[nCntBlock].state = BLOCKSTATE_NORMAL;
		g_aBlock[nCntBlock].type = BLOCKTYPE_COLLISION;
		g_aBlock[nCntBlock].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_BLOCK, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntBullet = 0; nCntBullet < MAX_BLOCK; nCntBullet++)
	{
		// ���_���W�̐ݒ�
		SetVertexBlock(nCntBullet);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();	

	return S_OK;
}

//-----------------------------------------------------------------
// �u���b�N�̏I������
//-----------------------------------------------------------------
void UninitBlock(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_BLOCKTYPE; nCntTexture++)
	{
		if (g_pTextureBlock[nCntTexture] != NULL)
		{
			g_pTextureBlock[nCntTexture]->Release();
			g_pTextureBlock[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �u���b�N�̍X�V����
//-----------------------------------------------------------------
void UpdateBlock(void)
{
	// �\���̂̃|�C���^�ϐ�
	BLOCK *pBlock = &g_aBlock[0];

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			// �ʒu�̍X�V
			pBlock->pos += pBlock->move;

			// ���Ԃ̍X�V
			pBlock->nTimer++;

			if (pBlock->nTimer == 200)
			{// �J�E���g��100�ɂȂ����Ƃ�����
				pBlock->move *= -1;
				pBlock->nTimer = 0;
			}

			// ���_���W�̍X�V
			SetVertexBlock(nCntBlock);
		}
	}
}

//-----------------------------------------------------------------
// �u���b�N�̕`�揈��
//-----------------------------------------------------------------
void DrawBlock(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�̕`��
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{// �e���g���Ă���Ƃ�����
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �u���b�N�̐ݒ�
//-----------------------------------------------------------------
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, float fTexU, float fTexV,BLOCKSTATE state, BLOCKTYPE type)
{
	// �\���̂̃|�C���^�ϐ�
	BLOCK *pBlock;

	// �ϐ��̎󂯓n��
	pBlock = &g_aBlock[0];

	// �e�̐ݒ�
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == false)
		{// �ʒu�ݒ�
			pBlock->pos = pos;

			// �ړ��ʂ̐ݒ�
			pBlock->move = move;

			// ���̐ݒ�
			pBlock->fWidth = fWidth;

			// �����̐ݒ�
			pBlock->fHeight = fHeight;

			// �e�N�X�`�����WU�̐ݒ�
			pBlock->fTexU = fTexU;

			// �e�N�X�`�����WV�̐ݒ�
			pBlock->fTexV = fTexV;

			// �u���b�N�̏�Ԃ̐ݒ�
			pBlock->state = state;

			// �u���b�N�̏�Ԃ̐ݒ�
			pBlock->type = type;

			// true�ŕԂ�
			pBlock->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �u���b�N���̎擾
//-----------------------------------------------------------------
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//-----------------------------------------------------------------
// �u���b�N�̒��_���W
//-----------------------------------------------------------------
void SetVertexBlock(int nIdx)
{
	// �ϐ��錾
	float ShaikingMove = GetShaking();

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x , g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x , g_aBlock[nIdx].pos.y + ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight + ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y + ShaikingMove, 0.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_aBlock[nIdx].fTexV);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_aBlock[nIdx].fTexU, g_aBlock[nIdx].fTexV);
	pVtx[3].tex = D3DXVECTOR2(g_aBlock[nIdx].fTexU, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//-----------------------------------------------------------------
// �z�u����u���b�N�̏��
//-----------------------------------------------------------------
void SetBlockInfo(MODESTATE modestate)
{
	switch (modestate)
	{// �^�C�g���̃u���b�N�ݒu
	case MODESTATE_TITLE:
		TitleBlock();
		break;

		// �Q�[���̐X�X�e�[�W�̃u���b�N�ݒu
	case MODESTATE_GAME_FOREST:
		GameForestBlock();
		break;

		// ���U���g�̃u���b�N�ݒu
	case MODESTATE_RESULT:
		ResultBlock();
		break;
	}
}

//-----------------------------------------------------------------
// �^�C�g����ʂ̃u���b�N�z�u
//-----------------------------------------------------------------
void TitleBlock(void)
{
	// ��
	SetBlock(D3DXVECTOR3(0, 690.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, BLOCK_SIZE,
		NORMAL_BLOCK, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_COLLISION);
}

//-----------------------------------------------------------------
// �Q�[���X�X�e�[�W�̃u���b�N�z�u
//-----------------------------------------------------------------
void GameForestBlock(void)
{
	// ��
	SetBlock(D3DXVECTOR3(0, 690.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, BLOCK_SIZE,
		NORMAL_BLOCK, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_COLLISION);

	// ��
	SetBlock(D3DXVECTOR3(969.0f, 600.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 310.0f, 120.0f,
		1.0f, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_NOT_COLLISION);

	// ��
	SetBlock(D3DXVECTOR3(-10.0f, 600.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 310.0f, 120.0f,
		-1.0f, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_NOT_COLLISION);

	// �����Ă鏰
	// ��
	SetBlock(D3DXVECTOR3(0, 580.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * FLOAT_BLOCK, BLOCK_SIZE,
		FLOAT_BLOCK, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// �E
	SetBlock(D3DXVECTOR3(970.0f, 580.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * FLOAT_BLOCK, BLOCK_SIZE,
		FLOAT_BLOCK, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// ����
	SetBlock(D3DXVECTOR3(415.0f, 450.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * 15.0f, BLOCK_SIZE,
		15.0f, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// ����
	SetBlock(D3DXVECTOR3(70.0f, 330.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * 5.0f, BLOCK_SIZE,
		5.0f, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// �E��
	SetBlock(D3DXVECTOR3(1050.0f, 330.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * 5.0f, BLOCK_SIZE,
		5.0f, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);
}

//-----------------------------------------------------------------
// ���U���g��ʂ̃u���b�N�z�u
//-----------------------------------------------------------------
void ResultBlock(void)
{
	// ��
	SetBlock(D3DXVECTOR3(0, 690.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, BLOCK_SIZE,
		NORMAL_BLOCK, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_COLLISION);
}

//-----------------------------------------------------------------
// �u���b�N�̓����蔻��
//-----------------------------------------------------------------
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, COLLISIONINFO info)
{
	// �\���̂̃|�C���^�ϐ�
	BLOCK *pBlock = &g_aBlock[0];
	bool bLand = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == true)
		{// �u���b�N���g���Ă���Ƃ�
			if (pBlock->type == BLOCKTYPE_COLLISION)
			{// �����蔻�肪����u���b�N�̎�����
				if ((pPos->x - fWidth) < (pBlock->pos.x + pBlock->fWidth) &&
					(pPos->x + fWidth) > pBlock->pos.x)
				{// �v���C���[���u���b�N�̒��ɂ���Ƃ�
					if (pPosOld->y + fHeight <= pBlock->pos.y)
					{// �v���C���[���u���b�N�̏�ɂ���Ƃ�
						if (pPos->y + fHeight > (pBlock->pos.y))
						{// �߂荞�񂾂珈��
							LandBlock(nCntBlock, pPos, pMove, fHeight);
							bLand = true;
						}
					}
					else if ((pPosOld->y - fHeight) >= (pBlock->pos.y + pBlock->fHeight))
					{// �v���C���[���u���b�N�̉��ɂ���Ƃ�
						if ((pPos->y - fHeight) < (pBlock->pos.y + pBlock->fHeight))
						{// �߂荞�񂾂�u���b�N�̉��̍��W�ɂ���
							pPos->y = (pBlock->pos.y + pBlock->fHeight + fHeight);
							pMove->y = 1.0f;
						}
					}
				}
				if ((pPos->y + fHeight) > pBlock->pos.y && (pPos->y - fHeight) < (pBlock->pos.y + pBlock->fHeight))
				{// �v���C���[���u���b�N��Y���ɂ���Ƃ�
					if ((pPosOld->x + fWidth) <= pBlock->pos.x)
					{// �v���C���[���u���b�N�̍����ɂ���Ƃ�
						if ((pPos->x + fWidth) > pBlock->pos.x)
						{// �߂荞�񂾂�u���b�N�̍��̍��W�ɂ���
							pPos->x = pBlock->pos.x - fWidth + pBlock->move.x;
							CollisionBlockSide(pMove, info);
						}
					}
					else if ((pPosOld->x - fWidth) >= (pBlock->pos.x + pBlock->fWidth))
					{// �v���C���[���u���b�N�̉E���ɂ���Ƃ�
						if ((pPos->x - fWidth) < (pBlock->pos.x + pBlock->fWidth))
						{// �߂荞�񂾂�u���b�N�̉E�̍��W�ɂ���
							pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth + pBlock->move.x;
							CollisionBlockSide(pMove, info);
						}
					}
				}
			}
		}
	}
	// ����Ă��邩�Ԃ�
	return bLand;
}

//-----------------------------------------------------------------
// ���̃u���b�N�ɏ���Ă��邩
//-----------------------------------------------------------------
void LandBlock(int nCntBlock, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, float fHeight)
{
	// �\���̂̃|�C���^�ϐ�
	BLOCK *pBlock = &g_aBlock[0];
	
	// ���Ԗڂ�
	pBlock += nCntBlock;

	if (pBlock->state == BLOCKSTATE_NORMAL || pBlock->state == BLOCKSTATE_FLOAT)
	{// �u���b�N�̏�Ԃ����������Ă��鏰�̂Ƃ�
		pPos->y = pBlock->pos.y - fHeight;
	}
	else if (pBlock->state == BLOCKSTATE_RIFT)
	{// �u���b�N�̏�Ԃ��㉺�ɓ������̂Ƃ�
		if (pBlock->move.y >= 0)
		{// �ォ�牺�̎�
			pPos->y = pBlock->pos.y - fHeight;
		}
		else if (pBlock->move.y < 0)
		{// �������̎�
			pPos->y = pBlock->pos.y + pBlock->move.y - fHeight;
		}
	}
}

//-----------------------------------------------------------------
// �u���b�N�̉��ɓ��������Ƃ�
//-----------------------------------------------------------------
void CollisionBlockSide(D3DXVECTOR3 *pMove, COLLISIONINFO info)
{
	switch (info)
	{// �v���C���[�̂Ƃ��ړ��ʂ�0�ɂ���
	case COLLISIONINFO_PLAYER:
		pMove->x = 0.0f;
		break;
		// �t���[�c�̂Ƃ����˂���
	case COLLISIONINFO_FRUITS:
		pMove->x *= -1;
		break;
		// �G�̂Ƃ����˂���
	case COLLISIONINFO_ENEMY:
		pMove->x *= -1;
		break;
	}
}