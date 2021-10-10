//=============================================================================
//
// ライトヘッダー (light.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT		(3)				// ライトの最大数

//*****************************************************************************
// ライトクラス(基本クラス)
//*****************************************************************************
class CLight
{
public:
	CLight();
	~CLight();

	void Init(void);			
	void Uninit(void);			
	void Update(void);			

private:
	D3DLIGHT9 m_light[MAX_LIGHT];			// ライト情報
};

#endif