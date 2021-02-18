//-----------------------------------------------------------------
//
// モーション (motion.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "player.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
MOTION_INFO g_MotionInfo;					// モーション情報

//-----------------------------------------------------------------
// モーションの決定
//-----------------------------------------------------------------
void SetMotion(void)
{
	NeutralMotion();
}

//-----------------------------------------------------------------
// ニュートラルモーション
//-----------------------------------------------------------------
void NeutralMotion(void)
{
	// ニュートラルモーションの設定
	// 構造体のポインタ変数
	Player *pPlayer = GetPlayer();

	// ループするかどうか設定
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].bLoop = true;			// 0がループしない、1がループする

	// キー数の設定
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey = 2;

	// 各モデルのフレーム数の設定
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame = 40;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].nFrame = 40;

	// 各モデルのキー要素の値を設定
	// パーツ0(1個目)のキー0
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotZ = 0.0f;

	// パーツ0(1個目)のキー1
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotZ = 0.0f;

	// パーツ0(1個目)のキー2
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotZ = 0.79f;

	// パーツ0(1個目)のキー3
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotX = -0.03f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotY = -0.16f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotZ = 0.09f;

	// パーツ0(1個目)のキー4
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotZ = -0.79f;

	// パーツ0(1個目)のキー5
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotX = 0.03f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotY = 0.16f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotZ = -0.09f;

	// パーツ0(1個目)のキー6
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotZ = -0.09f;

	// パーツ0(1個目)のキー7
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotZ = 0.0f;

	// パーツ0(1個目)のキー8
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotZ = 0.09f;

	// パーツ0(1個目)のキー9
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotZ = 0.0f;

	// パーツ1(2個目)のキー0
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosY = -1.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotX = -0.03f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotZ = 0.0f;

	// パーツ1(2個目)のキー1
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotX = -0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotZ = 0.0f;

	// パーツ1(2個目)のキー2
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotY = -0.35f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotZ = 1.26f;

	// パーツ1(2個目)のキー3
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotX = -0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotZ = 0.0f;

	// パーツ1(2個目)のキー4
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotY = 0.35f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotZ = -1.26f;

	// パーツ1(2個目)のキー5
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotX = 0.6f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotY = 0.22f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotZ = 0.0f;

	// パーツ1(2個目)のキー6
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosY = 0.5f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotX = 0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotZ = -0.09f;

	// パーツ1(2個目)のキー7
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotY = -0.18f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotZ = 0.0f;

	// パーツ1(2個目)のキー8
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosY = 0.5f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotX = 0.06f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotZ = 0.09f;

	// パーツ1(2個目)のキー9
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosY = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosZ = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotX = 0.0f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotY = 0.18f;
	pPlayer->aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotZ = 0.0f;
}

//-----------------------------------------------------------------
// モーションの読み込み
//-----------------------------------------------------------------
void LoadPlayerdata(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// 構造体のポインタ変数
	Player *pPlayer = GetPlayer();

	// ファイルを開く
	pFile = fopen("data/motion.txt", "r");

	// 変数宣言
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

	// SCRIPTを探す
	while (fgets(aLine, 128, pFile) != NULL)
	{
		fscanf(pFile, "%s", &aText[0]);

		// SCRIPTを見つけたらそれ以降を読み込む
		if (strcmp(&aText[0], "SCRIPT") == 0)
		{
			bGetScript = true;
		}

		if (bGetScript == true)
		{
			// プレイヤーのモデル情報の読み込み
			// NUM_MODELを見つけたらモデルの総数を格納
			if (strcmp(&aText[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s %d", &aText[0], &pPlayer->nNumModel);
			}
			// MODEL_FILENAMEを見つけたらXファイル名を格納
			if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < pPlayer->nNumModel)
			{
				fscanf(pFile, "%s %s", &aText[0], &pPlayer->aModel[nCntModel].aFileName[0]);

				// モデルの総数だけ格納するのでカウントを増やす
				nCntModel++;
			}
			// PLAYERSETを見つけたらプレイヤー情報を読み込む許可を与える
			if (strcmp(&aText[0], "PLAYERSET") == 0)
			{
				bPlayerSet = true;
			}
			// PARTSSETを見つけたら各パーツの情報を格納する許可を与える
			if (strcmp(&aText[0], "PARTSSET") == 0 && bPlayerSet == true)
			{
				bPartsSet = true;
			}
			// 各パーツの情報を格納する許可があるとき処理
			if (bPartsSet == true)
			{
				// PARENTを見つけたらXファイル名を格納
				if (strcmp(&aText[0], "PARENT") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &pPlayer->aModel[nCntParts].nIdxModelParent);
				}
				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0], 
						&pPlayer->aModel[nCntParts].pos.x,
						&pPlayer->aModel[nCntParts].pos.y,
						&pPlayer->aModel[nCntParts].pos.z);

					if (nCntParts == 0)
					{
						// 親のパーツのとき座標を保存
						pPlayer->posParent.x = pPlayer->aModel[nCntParts].pos.x;
						pPlayer->posParent.y = pPlayer->aModel[nCntParts].pos.y;
						pPlayer->posParent.z = pPlayer->aModel[nCntParts].pos.z;
					}
				}
				// ROTを見つけたら角度情報を格納
				if (strcmp(&aText[0], "ROT") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&pPlayer->aModel[nCntParts].rot.x,
						&pPlayer->aModel[nCntParts].rot.y,
						&pPlayer->aModel[nCntParts].rot.z);
				}
				// END_PARTSSETを見つけたら各パーツの情報を格納を終える
				if (strcmp(&aText[0], "END_PARTSSET") == 0 && bPlayerSet == true)
				{
					bPartsSet = false;

					if (nCntParts < pPlayer->nNumModel)
					{
						// パーツの総数だけ格納するのでカウントを増やす
						nCntParts++;
					}
				}
			}
			// END_PLAYERSETを見つけたらプレイヤー情報を読み込むを終える
			if (strcmp(&aText[0], "END_PLAYERSET") == 0)
			{
				bPlayerSet = false;
			}

			// モーション情報の読み込み
			// MOTIONSETを見つけたらモーション情報を読み込む許可を与える
			if (strcmp(&aText[0], "MOTIONSET") == 0)
			{
				bMotionSet = true;

				// モーションが複数あるかもしれないのでカウント
				nCntMotion++;
			}
			// モーション情報を読み込む許可があるとき処理
			if (bMotionSet == true)
			{
				// LOOPを見つけたらモーションをループさせるかどうか決める
				if (strcmp(&aText[0], "LOOP") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumLoop);

					// 数字でループするかどうか判断
					if (nNumLoop == 1)
					{
						pPlayer->aMotionInfo[nCntMotion].bLoop = true;
					}
					else
					{
						pPlayer->aMotionInfo[nCntMotion].bLoop = false;
					}
				}
				// NUM_KEYを見つけたらキー数を格納
				if (strcmp(&aText[0], "NUM_KEY") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &pPlayer->aMotionInfo[nCntMotion].nNumKey);
				}
				// KEYSETを見つけたら各キー情報を読み込む許可を与える
				if (strcmp(&aText[0], "KEYSET") == 0)
				{
					bKeySet = true;

					// キーは複数あるのでカウント
					nCntKey++;
				}
				// 各キー情報を読み込む許可があるとき処理
				if (bKeySet == true)
				{
					// FRAMEを見つけたらフレーム数を格納
					if (strcmp(&aText[0], "FRAME") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					// KEYを見つけたら各パーツに情報を読み込む許可を与える
					if (strcmp(&aText[0], "KEY") == 0)
					{
						bKey = true;

						// パーツも複数あるのでカウント
						nCntKeyParts++;
					}
					// 各パーツに情報を読み込む許可があるとき処理
					if (bKey == true)
					{
						// POSを見つけたら座標情報を格納
						if (strcmp(&aText[0], "POS") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fPosX,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fPosY,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fPosZ);
						}
						// ROTを見つけたら角度情報を格納
						if (strcmp(&aText[0], "ROT") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fRotX,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fRotY,
								&pPlayer->aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKeyParts].fRotZ);
						}
						// END_KEYを見つけたら各パーツに情報の読み込みを終える
						if (strcmp(&aText[0], "END_KEY") == 0)
						{
							bKey = false;
						}
					}
					// END_KEYSETを見つけたら各キー情報の読み込みを終える
					if (strcmp(&aText[0], "END_KEYSET") == 0)
					{
						bKeySet = false;

						// パーツカウント用の変数を初期化
						nCntKeyParts = -1;
					}
				}
				// END_MOTIONSETを見つけたらモーション情報の読み込みを終える
				if (strcmp(&aText[0], "END_MOTIONSET") == 0)
				{
					bMotionSet = false;

					// キーカウント用の変数を初期化
					nCntKey = -1;
				}
			}

			// END_SCRIPTを見つけたら読み込みを終える
			if (strcmp(&aText[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	// ファイルを閉じる
	fclose(pFile);
}