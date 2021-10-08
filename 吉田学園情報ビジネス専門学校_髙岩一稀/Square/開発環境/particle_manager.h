//-------------------------------------------------------------------
//
// パーティクルマネージャーヘッダー (particle_manager.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "main.h"

//-------------------------------------------------------------------
// パーティクルマネージャークラス（基本クラス）
//-------------------------------------------------------------------
class CParticlManager
{
public:
	CParticlManager();
	~CParticlManager();

	static CParticlManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif