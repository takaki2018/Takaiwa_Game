//-----------------------------------------------------------------
//
// ���[�V���� (motion.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "player.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
MOTION_INFO g_MotionInfo;					// ���[�V�������

//-----------------------------------------------------------------
// ���[�V�����̌���
//-----------------------------------------------------------------
void SetMotion(void)
{
	NeutralMotion();
}

//-----------------------------------------------------------------
// �j���[�g�������[�V����
//-----------------------------------------------------------------
void NeutralMotion(void)
{
	// �j���[�g�������[�V�����̐ݒ�
	// �\���̂̃|�C���^�ϐ�
	Player *pPlayer = GetPlayer();

	// ���[�v���邩�ǂ����ݒ�
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].bLoop = true;			// 0�����[�v���Ȃ��A1�����[�v����

	// �L�[���̐ݒ�
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey = 2;

	// �e���f���̃t���[�����̐ݒ�
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame = 40;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].nFrame = 40;

	// �e���f���̃L�[�v�f�̒l��ݒ�
	// �p�[�c0(1��)�̃L�[0
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotZ = 0.0f;

	// �p�[�c0(1��)�̃L�[1
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotZ = 0.0f;

	// �p�[�c0(1��)�̃L�[2
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotZ = 0.79f;

	// �p�[�c0(1��)�̃L�[3
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotX = -0.03f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotY = -0.16f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotZ = 0.09f;

	// �p�[�c0(1��)�̃L�[4
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotZ = -0.79f;

	// �p�[�c0(1��)�̃L�[5
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotX = 0.03f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotY = 0.16f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotZ = -0.09f;

	// �p�[�c0(1��)�̃L�[6
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotZ = -0.09f;

	// �p�[�c0(1��)�̃L�[7
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotZ = 0.0f;

	// �p�[�c0(1��)�̃L�[8
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotZ = 0.09f;

	// �p�[�c0(1��)�̃L�[9
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotZ = 0.0f;

	// �p�[�c1(2��)�̃L�[0
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosY = -1.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotX = -0.03f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotZ = 0.0f;

	// �p�[�c1(2��)�̃L�[1
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotX = -0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotZ = 0.0f;

	// �p�[�c1(2��)�̃L�[2
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotY = -0.35f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotZ = 1.26f;

	// �p�[�c1(2��)�̃L�[3
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotX = -0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotZ = 0.0f;

	// �p�[�c1(2��)�̃L�[4
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotY = 0.35f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotZ = -1.26f;

	// �p�[�c1(2��)�̃L�[5
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotX = 0.6f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotY = 0.22f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotZ = 0.0f;

	// �p�[�c1(2��)�̃L�[6
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosY = 0.5f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotX = 0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotZ = -0.09f;

	// �p�[�c1(2��)�̃L�[7
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotY = -0.18f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotZ = 0.0f;

	// �p�[�c1(2��)�̃L�[8
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosY = 0.5f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotX = 0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotZ = 0.09f;

	// �p�[�c1(2��)�̃L�[9
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotY = 0.18f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotZ = 0.0f;
}

//-----------------------------------------------------------------
// ���[�V�����̓ǂݍ���
//-----------------------------------------------------------------
void LoadPlayerdata(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �\���̂̃|�C���^�ϐ�
	Player *pPlayer = GetPlayer();

	// �t�@�C�����J��
	pFile = fopen("data/motion.txt", "r");

	// �ϐ��錾
	bool bGetScript = false;
	bool bPlayerSet = false;
	bool bPartsSet = false;
	bool bMotionSet = false;
	bool bKeySet = false;
	bool bKey = false;

	char aLine[128];
	char aText[128];
	int nCntModel = 0;
	int nCntParts = 0;
	int nCntMotion = -1;
	int nNumLoop = 0;
	int nCntKey = -1;
	int nCntKeyParts = -1;

	// SCRIPT��T��
	while (fgets(aLine, 128, pFile) != NULL)
	{
		fscanf(pFile, "%s", &aText[0]);

		// SCRIPT���������炻��ȍ~��ǂݍ���
		if (strcmp(&aText[0], "SCRIPT") == 0)
		{
			bGetScript = true;
		}

		if (bGetScript == true)
		{
			// �v���C���[�̃��f�����̓ǂݍ���
			// NUM_MODEL���������烂�f���̑������i�[
			if (strcmp(&aText[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s %d", &aText[0], &pPlayer->nNumModel);
			}
			// MODEL_FILENAME����������X�t�@�C�������i�[
			if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < pPlayer->nNumModel)
			{
				fscanf(pFile, "%s %s", &aText[0], &pPlayer->aModel[nCntModel].aFileName[0]);

				// ���f���̑��������i�[����̂ŃJ�E���g�𑝂₷
				nCntModel++;
			}
			// PLAYERSET����������v���C���[����ǂݍ��ދ���^����
			if (strcmp(&aText[0], "PLAYERSET") == 0)
			{
				bPlayerSet = true;
			}
			// PARTSSET����������e�p�[�c�̏����i�[���鋖��^����
			if (strcmp(&aText[0], "PARTSSET") == 0 && bPlayerSet == true)
			{
				bPartsSet = true;
			}
			// �e�p�[�c�̏����i�[���鋖������Ƃ�����
			if (bPartsSet == true)
			{
				// PARENT����������X�t�@�C�������i�[
				if (strcmp(&aText[0], "PARENT") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &pPlayer->aModel[nCntParts].nIdxModelParent);
				}
				// POS������������W�����i�[
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0], 
						&pPlayer->aModel[nCntParts].pos.x,
						&pPlayer->aModel[nCntParts].pos.y,
						&pPlayer->aModel[nCntParts].pos.z);

					if (nCntParts == 0)
					{
						// �e�̃p�[�c�̂Ƃ����W��ۑ�
						pPlayer->posParent.x = pPlayer->aModel[nCntParts].pos.x;
						pPlayer->posParent.y = pPlayer->aModel[nCntParts].pos.y;
						pPlayer->posParent.z = pPlayer->aModel[nCntParts].pos.z;
					}
				}
				// ROT����������p�x�����i�[
				if (strcmp(&aText[0], "ROT") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&pPlayer->aModel[nCntParts].rot.x,
						&pPlayer->aModel[nCntParts].rot.y,
						&pPlayer->aModel[nCntParts].rot.z);
				}
				// END_PARTSSET����������e�p�[�c�̏����i�[���I����
				if (strcmp(&aText[0], "END_PARTSSET") == 0 && bPlayerSet == true)
				{
					bPartsSet = false;

					if (nCntParts < pPlayer->nNumModel)
					{
						// �p�[�c�̑��������i�[����̂ŃJ�E���g�𑝂₷
						nCntParts++;
					}
				}
			}
			// END_PLAYERSET����������v���C���[����ǂݍ��ނ��I����
			if (strcmp(&aText[0], "END_PLAYERSET") == 0)
			{
				bPlayerSet = false;
			}

			// ���[�V�������̓ǂݍ���
			// MOTIONSET���������烂�[�V��������ǂݍ��ދ���^����
			if (strcmp(&aText[0], "MOTIONSET") == 0)
			{
				bMotionSet = true;

				// ���[�V�������������邩������Ȃ��̂ŃJ�E���g
				nCntMotion++;
			}
			// ���[�V��������ǂݍ��ދ�������Ƃ�����
			if (bMotionSet == true)
			{
				// LOOP���������烂�[�V���������[�v�����邩�ǂ������߂�
				if (strcmp(&aText[0], "LOOP") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumLoop);

					// �����Ń��[�v���邩�ǂ������f
					if (nNumLoop == 1)
					{
						pPlayer->aMotionInfo[nCntMotion].bLoop = true;
					}
					else
					{
						pPlayer->aMotionInfo[nCntMotion].bLoop = false;
					}
				}
				// NUM_KEY����������L�[�����i�[
				if (strcmp(&aText[0], "NUM_KEY") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &pPlayer->aMotionInfo[nCntMotion].nNumKey);
				}
				// KEYSET����������e�L�[����ǂݍ��ދ���^����
				if (strcmp(&aText[0], "KEYSET") == 0)
				{
					bKeySet = true;

					// �L�[�͕�������̂ŃJ�E���g
					nCntKey++;
				}
				// �e�L�[����ǂݍ��ދ�������Ƃ�����
				if (bKeySet == true)
				{
					// FRAME����������t���[�������i�[
					if (strcmp(&aText[0], "FRAME") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					// KEY����������e�p�[�c�ɏ���ǂݍ��ދ���^����
					if (strcmp(&aText[0], "KEY") == 0)
					{
						bKey = true;

						// �p�[�c����������̂ŃJ�E���g
						nCntKeyParts++;
					}
					// �e�p�[�c�ɏ���ǂݍ��ދ�������Ƃ�����
					if (bKey == true)
					{
						// POS������������W�����i�[
						if (strcmp(&aText[0], "POS") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fPosX,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fPosY,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fPosZ);
						}
						// ROT����������p�x�����i�[
						if (strcmp(&aText[0], "ROT") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fRotX,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fRotY,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fRotZ);
						}
						// END_KEY����������e�p�[�c�ɏ��̓ǂݍ��݂��I����
						if (strcmp(&aText[0], "END_KEY") == 0)
						{
							bKey = false;
						}
					}
					// END_KEYSET����������e�L�[���̓ǂݍ��݂��I����
					if (strcmp(&aText[0], "END_KEYSET") == 0)
					{
						bKeySet = false;

						// �p�[�c�J�E���g�p�̕ϐ���������
						nCntKeyParts = -1;
					}
				}
				// END_MOTIONSET���������烂�[�V�������̓ǂݍ��݂��I����
				if (strcmp(&aText[0], "END_MOTIONSET") == 0)
				{
					bMotionSet = false;

					// �L�[�J�E���g�p�̕ϐ���������
					nCntKey = -1;
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