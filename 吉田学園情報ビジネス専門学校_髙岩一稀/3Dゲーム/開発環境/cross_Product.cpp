//-----------------------------------------------------------------
//
// �O�ς�p���������蔻�� (cross_product.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "cross_product.h"
#include "player.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define POS_0_X			(50.0f)		// Pos[0]��X���W
#define POS_0_Y			(0.0f)			// Pos[0]��Y���W
#define POS_0_Z			(50.0f)			// Pos[0]��Z���W

#define POS_1_X			(-50.0f)		// Pos[1]��X���W
#define POS_1_Y			(0.0f)			// Pos[1]��Y���W
#define POS_1_Z			(0.0f)		// Pos[1]��Z���W

#define POS_X			(120.0f)			// pos��X���W
#define POS_Y			(0.0f)			// pos��Y���W
#define POS_Z			(100.0f)			// pos��Z���W

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
CrossProduct g_CrossProduct;			// �e�X�g�p

//-----------------------------------------------------------------
// �O�ς�p���������蔻��e�X�g
//-----------------------------------------------------------------
void CrossProductTest(void)
{
	// �\���̂̃|�C���^�ϐ�
	Player *pPlayer = GetPlayer();

	// �ϐ��̏�����
	g_CrossProduct.aPos[0] = D3DXVECTOR3(POS_0_X, POS_0_Y, POS_0_Z);
	g_CrossProduct.aPos[1] = D3DXVECTOR3(POS_1_X, POS_1_Y, POS_1_Z);
	g_CrossProduct.pos = pPlayer->pos;
	g_CrossProduct.vecA = D3DXVECTOR3(POS_0_X - POS_1_X, POS_0_Y - POS_1_Y, POS_0_Z - POS_1_Z);
	g_CrossProduct.vecB = D3DXVECTOR3(POS_0_X - pPlayer->pos.x, POS_0_Y - pPlayer->pos.y, POS_0_Z - pPlayer->pos.z);
	g_CrossProduct.fVec = (g_CrossProduct.vecA.z * g_CrossProduct.vecB.x) - (g_CrossProduct.vecA.x * g_CrossProduct.vecB.z);
}

//-----------------------------------------------------------------
// ���̎擾
//-----------------------------------------------------------------
CrossProduct *GetCrossProduct(void)
{
	return &g_CrossProduct;
}