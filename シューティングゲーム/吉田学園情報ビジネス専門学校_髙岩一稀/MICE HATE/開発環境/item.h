//-------------------------------------------------------------------
//
// �A�C�e���w�b�_�[ (item.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitItem(void);				// �A�C�e���̏���������
void UninitItem(void);				// �A�C�e���̏I������
void UpdateItem(void);				// �A�C�e���̍X�V����
void DrawItem(void);				// �A�C�e���̕`�揈��
bool GetbUseItem(void);				// �A�C�e���̎g�p����
void SetItem(bool bUseItem);		// �g�p����

#endif
