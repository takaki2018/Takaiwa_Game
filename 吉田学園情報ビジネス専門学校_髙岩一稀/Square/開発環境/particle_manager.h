//-------------------------------------------------------------------
//
// �p�[�e�B�N���}�l�[�W���[�w�b�_�[ (particle_manager.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "main.h"

//-------------------------------------------------------------------
// �p�[�e�B�N���}�l�[�W���[�N���X�i��{�N���X�j
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