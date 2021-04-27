//============================================================================================================
//
// ���[�V���� (motion.cpp)
// Author:Itsuki Takaiwa
//
//============================================================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "player.h"

//------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//------------------------------------------------------------------------------------------------------------
typedef struct
{
	bool bGetScript;		// SCRIPT�����������ǂ���
	bool bPlayerSet;		// PLAYERSET�����������ǂ���
	bool bPartsSet;			// PRATSSET�����������ǂ���
	bool bMotionSet;		// MOTIONSET�����������ǂ���
	bool bKeySet;			// KEYSET�����������ǂ���
	bool bKey;				// KEY�����������ǂ���
}WordToFindOut;				// �P������������ǂ���

typedef struct
{
	int nCntModel;			// ���f����
	int nCntParts;			// �p�[�c��
	int nCntMotion;			// ���[�V������
	int nCntKey;			// �L�[���
	int nCntKeyParts;		// �L�[��
}NumberOfEachInfomation;	// �e���Ǘ��p

//------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//------------------------------------------------------------------------------------------------------------
void LoadPlayerModelFileName(FILE *pFile,char *pText, NumberOfEachInfomation *Noei, Player *pPlayer);							// �v���C���[���f���̃t�@�C������ǂݍ���
void LoadPlayerModelPartsSet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer);		// �v���C���[���f���̃p�[�c�Z�b�g����ǂݍ���
void LoadPlayerMotionData(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer);		// �v���C���[�̃��[�V��������ǂݍ���
void LoadPlayerMotionBasicsInfo(FILE *pFile, char *pText, NumberOfEachInfomation *Noei, Player *pPlayer);						// �v���C���[�̃��[�V�����̊�b����ǂݍ���
void LoadPlayerMotionKeySet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer);		// �v���C���[�̃��[�V�����L�[�Z�b�g����ǂݍ���
void LoadPlayerMotionKey(FILE *pFile, char *pText, NumberOfEachInfomation *Noei, Player *pPlayer);								// �v���C���[�̃��[�V�����̊e�L�[����ǂݍ���

//------------------------------------------------------------------------------------------------------------
// �v���C���[���̓ǂݍ���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//------------------------------------------------------------------------------------------------------------
void LoadPlayerData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/motion.txt", "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �\���̂̕ϐ��錾
		WordToFindOut Wtfo;				// �P������������ǂ���
		NumberOfEachInfomation Noei;	// �e���Ǘ��p
		Player *pPlayer = GetPlayer();	// �v���C���[���

		// �ϐ��錾
		char aLine[128];			
		char aText[128];

		// �\���̕ϐ��̏�����
		Wtfo.bGetScript = false;	// SCRIPT�����������ǂ���
		Wtfo.bPlayerSet = false;	// PLAYERSET�����������ǂ���
		Wtfo.bPartsSet = false;		// PRATSSET�����������ǂ���
		Wtfo.bMotionSet = false;	// MOTIONSET�����������ǂ���
		Wtfo.bKeySet = false;		// KEYSET�����������ǂ���
		Wtfo.bKey = false;			// KEY�����������ǂ���

		Noei.nCntModel = 0;			// ���f����
		Noei.nCntParts = 0;			// �p�[�c��
		Noei.nCntMotion = 0;		// ���[�v�Ǘ��p
		Noei.nCntKey = 0;			// �L�[���
		Noei.nCntKeyParts = 0;		// �L�[��

		// SCRIPT��T��
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// ���s�󔒂�������܂œǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPT���������炻��ȍ~��ǂݍ��ދ���^����
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				Wtfo.bGetScript = true;
			}

			if (Wtfo.bGetScript == true)
			{
				// �v���C���[���f���̃t�@�C�����̓ǂݍ���
				LoadPlayerModelFileName(pFile, &aText[0], &Noei, pPlayer);

				// PLAYERSET����������v���C���[����ǂݍ��ދ���^����
				if (strcmp(&aText[0], "PLAYERSET") == 0)
				{
					Wtfo.bPlayerSet = true;
				}
				// �v���C���[�Z�b�g���̓ǂݍ��݋�������ꍇ
				if (Wtfo.bPlayerSet == true)
				{
					/// �v���C���[���f���̃p�[�c�Z�b�g����ǂݍ���
					LoadPlayerModelPartsSet(pFile, &aText[0], &Wtfo, &Noei, pPlayer);

					// END_PLAYERSET����������v���C���[����ǂݍ��ނ��I����
					if (strcmp(&aText[0], "END_PLAYERSET") == 0)
					{
						Wtfo.bPlayerSet = false;
					}
				}
				// ���[�V�������̓ǂݍ���
				// MOTIONSET���������烂�[�V��������ǂݍ��ދ���^����
				if (strcmp(&aText[0], "MOTIONSET") == 0)
				{
					Wtfo.bMotionSet = true;
				}
				// ���[�V��������ǂݍ��ދ�������Ƃ�����
				if (Wtfo.bMotionSet == true)
				{
					// �v���C���[�̃��[�V��������ǂݍ���
					LoadPlayerMotionData(pFile, &aText[0], &Wtfo, &Noei, pPlayer);

					// END_MOTIONSET���������烂�[�V�������̓ǂݍ��݂��I����
					if (strcmp(&aText[0], "END_MOTIONSET") == 0)
					{
						Wtfo.bMotionSet = false;

						// ���[�V�������̉��Z
						Noei.nCntMotion++;

						// �L�[�Z�b�g���̏�����
						Noei.nCntKey = 0;
					}
				}

				// END_SCRIPT����������ǂݍ��݂��I����
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					break;
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
	}
}

//------------------------------------------------------------------------------------------------------------
// �v���C���[���f���̃t�@�C������ǂݍ���
// ����		�F*pFile	- �t�@�C���̃|�C���^
//			�F*pText	- �r�ݍ��񂾕�������̃|�C���^
//			�F*Noei		- NumberOfEachInfomation(�e���Ǘ��p)�\���̂̃|�C���^
//			�F*pPlayer	- �v���C���[���̃|�C���^
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//------------------------------------------------------------------------------------------------------------
void LoadPlayerModelFileName(FILE *pFile, char *pText,NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// NUM_MODEL���������烂�f���̑������i�[
	if (strcmp(pText, "NUM_MODEL") == 0)
	{
		fscanf(pFile, "%s %d", pText, &pPlayer->nNumModel);
	}
	// MODEL_FILENAME����������X�t�@�C�������i�[
	if (strcmp(pText, "MODEL_FILENAME") == 0 && Noei->nCntModel < pPlayer->nNumModel)
	{
		fscanf(pFile, "%s %s", pText, &pPlayer->aModel[Noei->nCntModel].aFileName[0]);

		// ���f���̑��������i�[����̂ŃJ�E���g�𑝂₷
		Noei->nCntModel++;
	}
}

//------------------------------------------------------------------------------------------------------------
// �v���C���[���f���̃p�[�c�Z�b�g����ǂݍ���
// ����		�F*pFile	- �t�@�C���̃|�C���^
//			�F*pText	- �r�ݍ��񂾕�������̃|�C���^
//			�F*Wtfo		- WordToFindOut(�P������������ǂ���)�\���̂̃|�C���^
//			�F*Noei		- NumberOfEachInfomation(�e���Ǘ��p)�\���̂̃|�C���^
//			�F*pPlayer	- �v���C���[���̃|�C���^
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//------------------------------------------------------------------------------------------------------------
void LoadPlayerModelPartsSet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// PARTSSET����������e�p�[�c�̏����i�[���鋖��^����
	if (strcmp(pText, "PARTSSET") == 0)
	{
		Wtfo->bPartsSet = true;
	}
	// �e�p�[�c�̏����i�[���鋖������Ƃ�����
	if (Wtfo->bPartsSet == true)
	{
		// PARENT����������X�t�@�C�������i�[
		if (strcmp(pText, "PARENT") == 0)
		{
			fscanf(pFile, "%s %d", pText, &pPlayer->aModel[Noei->nCntParts].nIdxModelParent);
		}
		// POS������������W�����i�[
		if (strcmp(pText, "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", pText,
				&pPlayer->aModel[Noei->nCntParts].pos.x,
				&pPlayer->aModel[Noei->nCntParts].pos.y,
				&pPlayer->aModel[Noei->nCntParts].pos.z);
		}
		// ROT����������p�x�����i�[
		if (strcmp(pText, "ROT") == 0)
		{
			fscanf(pFile, "%s %f %f %f", pText,
				&pPlayer->aModel[Noei->nCntParts].rot.x,
				&pPlayer->aModel[Noei->nCntParts].rot.y,
				&pPlayer->aModel[Noei->nCntParts].rot.z);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(pText, "END_PARTSSET") == 0)
		{
			Wtfo->bPartsSet = false;

			if (Noei->nCntParts < pPlayer->nNumModel)
			{
				// �p�[�c�̑��������i�[����̂ŃJ�E���g�𑝂₷
				Noei->nCntParts++;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// �v���C���[�̃��[�V��������ǂݍ���
// ����		�F*pFile	- �t�@�C���̃|�C���^
//			�F*pText	- �r�ݍ��񂾕�������̃|�C���^
//			�F*Wtfo		- WordToFindOut(�P������������ǂ���)�\���̂̃|�C���^
//			�F*Noei		- NumberOfEachInfomation(�e���Ǘ��p)�\���̂̃|�C���^
//			�F*pPlayer	- �v���C���[���̃|�C���^
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionData(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// �v���C���[�̃��[�V�������[�v����ǂݍ���
	LoadPlayerMotionBasicsInfo(pFile, pText, Noei, pPlayer);

	// KEYSET����������e�L�[����ǂݍ��ދ���^����
	if (strcmp(pText, "KEYSET") == 0)
	{
		Wtfo->bKeySet = true;
	}
	// �e�L�[����ǂݍ��ދ�������Ƃ�����
	if (Wtfo->bKeySet == true)
	{
		// �v���C���[�̃��[�V�����L�[�Z�b�g����ǂݍ���
		LoadPlayerMotionKeySet(pFile, pText, Wtfo, Noei, pPlayer);

		// END_KEYSET����������e�L�[���̓ǂݍ��݂��I����
		if (strcmp(pText, "END_KEYSET") == 0)
		{
			Wtfo->bKeySet = false;

			// �L�[�Z�b�g���̉��Z
			Noei->nCntKey++;

			// �L�[���̏�����
			Noei->nCntKeyParts = 0;
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// �v���C���[�̃��[�V�����̊�b����ǂݍ���
// ����		�F*pFile	- �t�@�C���̃|�C���^
//			�F*pText	- �r�ݍ��񂾕�������̃|�C���^
//			�F*Noei		- NumberOfEachInfomation(�e���Ǘ��p)�\���̂̃|�C���^
//			�F*pPlayer	- �v���C���[���̃|�C���^
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionBasicsInfo(FILE *pFile, char *pText,NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// �ϐ��錾
	int nLoopNum = 0;			// ���[�v�ԍ��i�[�p

	// LOOP���������烂�[�V���������[�v�����邩�ǂ������߂�
	if (strcmp(pText, "LOOP") == 0)
	{
		fscanf(pFile, "%s %d", pText, &nLoopNum);

		// �����Ń��[�v���邩�ǂ������f
		if (nLoopNum == 1)
		{
			pPlayer->aMotionInfo[Noei->nCntMotion].bLoop = true;
		}
		else
		{
			pPlayer->aMotionInfo[Noei->nCntMotion].bLoop = false;
		}
	}
	// NUM_KEY����������L�[�����i�[
	if (strcmp(pText, "NUM_KEY") == 0)
	{
		fscanf(pFile, "%s %d", pText, &pPlayer->aMotionInfo[Noei->nCntMotion].nNumKey);
	}
}

//------------------------------------------------------------------------------------------------------------
// �v���C���[�̃��[�V�����L�[�Z�b�g����ǂݍ���
// ����		�F*pFile	- �t�@�C���̃|�C���^
//			�F*pText	- �r�ݍ��񂾕�������̃|�C���^
//			�F*Wtfo		- WordToFindOut(�P������������ǂ���)�\���̂̃|�C���^
//			�F*Noei		- NumberOfEachInfomation(�e���Ǘ��p)�\���̂̃|�C���^
//			�F*pPlayer	- �v���C���[���̃|�C���^
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionKeySet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// FRAME����������t���[�������i�[
	if (strcmp(pText, "FRAME") == 0)
	{
		fscanf(pFile, "%s %d", pText, &pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].nFrame);
	}
	// KEY����������e�p�[�c�ɏ���ǂݍ��ދ���^����
	if (strcmp(pText, "KEY") == 0)
	{
		Wtfo->bKey = true;
	}
	// �e�p�[�c�ɏ���ǂݍ��ދ�������Ƃ�����
	if (Wtfo->bKey == true)
	{
		// �v���C���[�̃��[�V�����̊e�L�[����ǂݍ���
		LoadPlayerMotionKey(pFile, pText, Noei, pPlayer);

		// END_KEY����������e�p�[�c�ɏ��̓ǂݍ��݂��I����
		if (strcmp(pText, "END_KEY") == 0)
		{
			Wtfo->bKey = false;

			// �L�[�������Z
			Noei->nCntKeyParts++;
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// �v���C���[�̃��[�V�����̊e�L�[����ǂݍ���
// ����		�F*pFile	- �t�@�C���̃|�C���^
//			�F*pText	- �r�ݍ��񂾕�������̃|�C���^
//			�F*Noei		- NumberOfEachInfomation(�e���Ǘ��p)�\���̂̃|�C���^
//			�F*pPlayer	- �v���C���[���̃|�C���^
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionKey(FILE *pFile, char *pText, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// POS������������W�����i�[
	if (strcmp(pText, "POS") == 0)
	{
		fscanf(pFile, "%s %f %f %f", pText,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosX,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosY,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosZ);

		// �e�p�[�c�̃��[�V�����ʒu���Ɋe�p�[�c�̍��W�����Z�����␳����
		pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosX += pPlayer->aModel[Noei->nCntKeyParts].pos.x;
		pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosY += pPlayer->aModel[Noei->nCntKeyParts].pos.y;
		pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosZ += pPlayer->aModel[Noei->nCntKeyParts].pos.z;
	}
	// ROT����������p�x�����i�[
	if (strcmp(pText, "ROT") == 0)
	{
		fscanf(pFile, "%s %f %f %f", pText,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fRotX,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fRotY,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fRotZ);
	}
}