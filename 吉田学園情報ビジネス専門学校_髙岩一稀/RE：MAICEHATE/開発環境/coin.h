//==============================================================================================================
//
// �R�C���w�b�_�[ (coin.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _COIN_H_
#define _COIN_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_COIN			(8)

#define MAX_TEXTURE_NAME	(128)		// �e�N�X�`����
#define MAX_TEXTURE			(128)		// �e�N�X�`���̐�

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshModel;									// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMatModel;								// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMatModel = 0;									// �}�e���A���̐�
	LPDIRECT3DTEXTURE9 apTextureModelSet[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
	char aFileName[MAX_TEXTURE_NAME];						// �e�N�X�`����
	D3DXVECTOR3 vtxMinModel;								// ���W�̍ŏ��l
	D3DXVECTOR3 vtxMaxModel;								// ���W�̍ő�l
}Coin;

typedef struct
{
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;				// ���S���W
	D3DXVECTOR3 rot;				// ��]�p
	D3DXVECTOR3 vtxMinModel;		// ���W�̍ŏ��l
	D3DXVECTOR3 vtxMaxModel;		// ���W�̍ő�l
	D3DXVECTOR3 aPos[4];			// ������4�_
	D3DXVECTOR3 aVecA[4];			// �����̃x�N�g��
	D3DXVECTOR3 aVecB[4];			// �����̃x�N�g��
	float fVec[4];					// �O�ς̒l
	int nIdx;						// ���f�����ʔԍ�
	int nIdxShadow;					// ���Ԗڂ̉e��
	bool bUse;						// �g�p���Ă邩�ǂ���
}CoinSet;

typedef struct
{
	int nNumModel;					// ���f���̑���
	CoinSet coinSet[MAX_COIN];		// ���f���ݒu
	Coin coin[MAX_COIN];			// ���f�����
	int nCntCoin;					// �z�u����R�C���̐�
}CoinInfo;

//--------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------------------------------------------------------
HRESULT InitCoin(void);										// �R�C���̏���������
void UninitCoin(void);										// �R�C���̏I������
void UpdateCoin(void);										// �R�C���̍X�V����
void DrawCoin(void);										// �R�C���̕`�揈��

void TexCoin(int nCntCoin);									// �R�C���̃e�N�X�`��
void VecCoin(int nCntCoin);									// �R�C���̒��_���W
void VecCoinSet(int nCntCoin);								// ���f���̒��_���W�̕␳

bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);			// �����蔻��

void LoadCoinSetData(void);										// �R�C���z�u���̓ǂݍ���
bool GetbNoCoin(void);											// �R�C�������邩�Ȃ���

#endif