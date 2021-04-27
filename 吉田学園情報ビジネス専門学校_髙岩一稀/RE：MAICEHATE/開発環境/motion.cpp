//============================================================================================================
//
// モーション (motion.cpp)
// Author:Itsuki Takaiwa
//
//============================================================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "player.h"

//------------------------------------------------------------------------------------------------------------
// 構造体の定義
//------------------------------------------------------------------------------------------------------------
typedef struct
{
	bool bGetScript;		// SCRIPTを見つけたかどうか
	bool bPlayerSet;		// PLAYERSETを見つけたかどうか
	bool bPartsSet;			// PRATSSETを見つけたかどうか
	bool bMotionSet;		// MOTIONSETを見つけたかどうか
	bool bKeySet;			// KEYSETを見つけたかどうか
	bool bKey;				// KEYを見つけたかどうか
}WordToFindOut;				// 単語を見つけたかどうか

typedef struct
{
	int nCntModel;			// モデル数
	int nCntParts;			// パーツ数
	int nCntMotion;			// モーション数
	int nCntKey;			// キー情報数
	int nCntKeyParts;		// キー数
}NumberOfEachInfomation;	// 各情報管理用

//------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//------------------------------------------------------------------------------------------------------------
void LoadPlayerModelFileName(FILE *pFile,char *pText, NumberOfEachInfomation *Noei, Player *pPlayer);							// プレイヤーモデルのファイル情報を読み込む
void LoadPlayerModelPartsSet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer);		// プレイヤーモデルのパーツセット情報を読み込む
void LoadPlayerMotionData(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer);		// プレイヤーのモーション情報を読み込む
void LoadPlayerMotionBasicsInfo(FILE *pFile, char *pText, NumberOfEachInfomation *Noei, Player *pPlayer);						// プレイヤーのモーションの基礎情報を読み込む
void LoadPlayerMotionKeySet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer);		// プレイヤーのモーションキーセット情報を読み込む
void LoadPlayerMotionKey(FILE *pFile, char *pText, NumberOfEachInfomation *Noei, Player *pPlayer);								// プレイヤーのモーションの各キー情報を読み込む

//------------------------------------------------------------------------------------------------------------
// プレイヤー情報の読み込み
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void LoadPlayerData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/motion.txt", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// 構造体の変数宣言
		WordToFindOut Wtfo;				// 単語を見つけたかどうか
		NumberOfEachInfomation Noei;	// 各情報管理用
		Player *pPlayer = GetPlayer();	// プレイヤー情報

		// 変数宣言
		char aLine[128];			
		char aText[128];

		// 構造体変数の初期化
		Wtfo.bGetScript = false;	// SCRIPTを見つけたかどうか
		Wtfo.bPlayerSet = false;	// PLAYERSETを見つけたかどうか
		Wtfo.bPartsSet = false;		// PRATSSETを見つけたかどうか
		Wtfo.bMotionSet = false;	// MOTIONSETを見つけたかどうか
		Wtfo.bKeySet = false;		// KEYSETを見つけたかどうか
		Wtfo.bKey = false;			// KEYを見つけたかどうか

		Noei.nCntModel = 0;			// モデル数
		Noei.nCntParts = 0;			// パーツ数
		Noei.nCntMotion = 0;		// ループ管理用
		Noei.nCntKey = 0;			// キー情報数
		Noei.nCntKeyParts = 0;		// キー数

		// SCRIPTを探す
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// 毎行空白を見つけるまで読み込む
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPTを見つけたらそれ以降を読み込む許可を与える
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				Wtfo.bGetScript = true;
			}

			if (Wtfo.bGetScript == true)
			{
				// プレイヤーモデルのファイル情報の読み込み
				LoadPlayerModelFileName(pFile, &aText[0], &Noei, pPlayer);

				// PLAYERSETを見つけたらプレイヤー情報を読み込む許可を与える
				if (strcmp(&aText[0], "PLAYERSET") == 0)
				{
					Wtfo.bPlayerSet = true;
				}
				// プレイヤーセット情報の読み込み許可がある場合
				if (Wtfo.bPlayerSet == true)
				{
					/// プレイヤーモデルのパーツセット情報を読み込む
					LoadPlayerModelPartsSet(pFile, &aText[0], &Wtfo, &Noei, pPlayer);

					// END_PLAYERSETを見つけたらプレイヤー情報を読み込むを終える
					if (strcmp(&aText[0], "END_PLAYERSET") == 0)
					{
						Wtfo.bPlayerSet = false;
					}
				}
				// モーション情報の読み込み
				// MOTIONSETを見つけたらモーション情報を読み込む許可を与える
				if (strcmp(&aText[0], "MOTIONSET") == 0)
				{
					Wtfo.bMotionSet = true;
				}
				// モーション情報を読み込む許可があるとき処理
				if (Wtfo.bMotionSet == true)
				{
					// プレイヤーのモーション情報を読み込む
					LoadPlayerMotionData(pFile, &aText[0], &Wtfo, &Noei, pPlayer);

					// END_MOTIONSETを見つけたらモーション情報の読み込みを終える
					if (strcmp(&aText[0], "END_MOTIONSET") == 0)
					{
						Wtfo.bMotionSet = false;

						// モーション数の加算
						Noei.nCntMotion++;

						// キーセット数の初期化
						Noei.nCntKey = 0;
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
	else
	{
	}
}

//------------------------------------------------------------------------------------------------------------
// プレイヤーモデルのファイル情報を読み込む
// 引数		：*pFile	- ファイルのポインタ
//			：*pText	- 詠み込んだ文字列情報のポインタ
//			：*Noei		- NumberOfEachInfomation(各情報管理用)構造体のポインタ
//			：*pPlayer	- プレイヤー情報のポインタ
// 返り値	：void		- 何も返さない
//------------------------------------------------------------------------------------------------------------
void LoadPlayerModelFileName(FILE *pFile, char *pText,NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// NUM_MODELを見つけたらモデルの総数を格納
	if (strcmp(pText, "NUM_MODEL") == 0)
	{
		fscanf(pFile, "%s %d", pText, &pPlayer->nNumModel);
	}
	// MODEL_FILENAMEを見つけたらXファイル名を格納
	if (strcmp(pText, "MODEL_FILENAME") == 0 && Noei->nCntModel < pPlayer->nNumModel)
	{
		fscanf(pFile, "%s %s", pText, &pPlayer->aModel[Noei->nCntModel].aFileName[0]);

		// モデルの総数だけ格納するのでカウントを増やす
		Noei->nCntModel++;
	}
}

//------------------------------------------------------------------------------------------------------------
// プレイヤーモデルのパーツセット情報を読み込む
// 引数		：*pFile	- ファイルのポインタ
//			：*pText	- 詠み込んだ文字列情報のポインタ
//			：*Wtfo		- WordToFindOut(単語を見つけたかどうか)構造体のポインタ
//			：*Noei		- NumberOfEachInfomation(各情報管理用)構造体のポインタ
//			：*pPlayer	- プレイヤー情報のポインタ
// 返り値	：void		- 何も返さない
//------------------------------------------------------------------------------------------------------------
void LoadPlayerModelPartsSet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// PARTSSETを見つけたら各パーツの情報を格納する許可を与える
	if (strcmp(pText, "PARTSSET") == 0)
	{
		Wtfo->bPartsSet = true;
	}
	// 各パーツの情報を格納する許可があるとき処理
	if (Wtfo->bPartsSet == true)
	{
		// PARENTを見つけたらXファイル名を格納
		if (strcmp(pText, "PARENT") == 0)
		{
			fscanf(pFile, "%s %d", pText, &pPlayer->aModel[Noei->nCntParts].nIdxModelParent);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(pText, "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", pText,
				&pPlayer->aModel[Noei->nCntParts].pos.x,
				&pPlayer->aModel[Noei->nCntParts].pos.y,
				&pPlayer->aModel[Noei->nCntParts].pos.z);
		}
		// ROTを見つけたら角度情報を格納
		if (strcmp(pText, "ROT") == 0)
		{
			fscanf(pFile, "%s %f %f %f", pText,
				&pPlayer->aModel[Noei->nCntParts].rot.x,
				&pPlayer->aModel[Noei->nCntParts].rot.y,
				&pPlayer->aModel[Noei->nCntParts].rot.z);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(pText, "END_PARTSSET") == 0)
		{
			Wtfo->bPartsSet = false;

			if (Noei->nCntParts < pPlayer->nNumModel)
			{
				// パーツの総数だけ格納するのでカウントを増やす
				Noei->nCntParts++;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// プレイヤーのモーション情報を読み込む
// 引数		：*pFile	- ファイルのポインタ
//			：*pText	- 詠み込んだ文字列情報のポインタ
//			：*Wtfo		- WordToFindOut(単語を見つけたかどうか)構造体のポインタ
//			：*Noei		- NumberOfEachInfomation(各情報管理用)構造体のポインタ
//			：*pPlayer	- プレイヤー情報のポインタ
// 返り値	：void		- 何も返さない
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionData(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// プレイヤーのモーションループ情報を読み込む
	LoadPlayerMotionBasicsInfo(pFile, pText, Noei, pPlayer);

	// KEYSETを見つけたら各キー情報を読み込む許可を与える
	if (strcmp(pText, "KEYSET") == 0)
	{
		Wtfo->bKeySet = true;
	}
	// 各キー情報を読み込む許可があるとき処理
	if (Wtfo->bKeySet == true)
	{
		// プレイヤーのモーションキーセット情報を読み込む
		LoadPlayerMotionKeySet(pFile, pText, Wtfo, Noei, pPlayer);

		// END_KEYSETを見つけたら各キー情報の読み込みを終える
		if (strcmp(pText, "END_KEYSET") == 0)
		{
			Wtfo->bKeySet = false;

			// キーセット数の加算
			Noei->nCntKey++;

			// キー数の初期化
			Noei->nCntKeyParts = 0;
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// プレイヤーのモーションの基礎情報を読み込む
// 引数		：*pFile	- ファイルのポインタ
//			：*pText	- 詠み込んだ文字列情報のポインタ
//			：*Noei		- NumberOfEachInfomation(各情報管理用)構造体のポインタ
//			：*pPlayer	- プレイヤー情報のポインタ
// 返り値	：void		- 何も返さない
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionBasicsInfo(FILE *pFile, char *pText,NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// 変数宣言
	int nLoopNum = 0;			// ループ番号格納用

	// LOOPを見つけたらモーションをループさせるかどうか決める
	if (strcmp(pText, "LOOP") == 0)
	{
		fscanf(pFile, "%s %d", pText, &nLoopNum);

		// 数字でループするかどうか判断
		if (nLoopNum == 1)
		{
			pPlayer->aMotionInfo[Noei->nCntMotion].bLoop = true;
		}
		else
		{
			pPlayer->aMotionInfo[Noei->nCntMotion].bLoop = false;
		}
	}
	// NUM_KEYを見つけたらキー数を格納
	if (strcmp(pText, "NUM_KEY") == 0)
	{
		fscanf(pFile, "%s %d", pText, &pPlayer->aMotionInfo[Noei->nCntMotion].nNumKey);
	}
}

//------------------------------------------------------------------------------------------------------------
// プレイヤーのモーションキーセット情報を読み込む
// 引数		：*pFile	- ファイルのポインタ
//			：*pText	- 詠み込んだ文字列情報のポインタ
//			：*Wtfo		- WordToFindOut(単語を見つけたかどうか)構造体のポインタ
//			：*Noei		- NumberOfEachInfomation(各情報管理用)構造体のポインタ
//			：*pPlayer	- プレイヤー情報のポインタ
// 返り値	：void		- 何も返さない
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionKeySet(FILE *pFile, char *pText, WordToFindOut *Wtfo, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// FRAMEを見つけたらフレーム数を格納
	if (strcmp(pText, "FRAME") == 0)
	{
		fscanf(pFile, "%s %d", pText, &pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].nFrame);
	}
	// KEYを見つけたら各パーツに情報を読み込む許可を与える
	if (strcmp(pText, "KEY") == 0)
	{
		Wtfo->bKey = true;
	}
	// 各パーツに情報を読み込む許可があるとき処理
	if (Wtfo->bKey == true)
	{
		// プレイヤーのモーションの各キー情報を読み込む
		LoadPlayerMotionKey(pFile, pText, Noei, pPlayer);

		// END_KEYを見つけたら各パーツに情報の読み込みを終える
		if (strcmp(pText, "END_KEY") == 0)
		{
			Wtfo->bKey = false;

			// キー数を加算
			Noei->nCntKeyParts++;
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// プレイヤーのモーションの各キー情報を読み込む
// 引数		：*pFile	- ファイルのポインタ
//			：*pText	- 詠み込んだ文字列情報のポインタ
//			：*Noei		- NumberOfEachInfomation(各情報管理用)構造体のポインタ
//			：*pPlayer	- プレイヤー情報のポインタ
// 返り値	：void		- 何も返さない
//------------------------------------------------------------------------------------------------------------
void LoadPlayerMotionKey(FILE *pFile, char *pText, NumberOfEachInfomation *Noei, Player *pPlayer)
{
	// POSを見つけたら座標情報を格納
	if (strcmp(pText, "POS") == 0)
	{
		fscanf(pFile, "%s %f %f %f", pText,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosX,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosY,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosZ);

		// 各パーツのモーション位置情報に各パーツの座標を加算し一を補正する
		pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosX += pPlayer->aModel[Noei->nCntKeyParts].pos.x;
		pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosY += pPlayer->aModel[Noei->nCntKeyParts].pos.y;
		pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fPosZ += pPlayer->aModel[Noei->nCntKeyParts].pos.z;
	}
	// ROTを見つけたら角度情報を格納
	if (strcmp(pText, "ROT") == 0)
	{
		fscanf(pFile, "%s %f %f %f", pText,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fRotX,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fRotY,
			&pPlayer->aMotionInfo[Noei->nCntMotion].aKeyInfo[Noei->nCntKey].aKey[Noei->nCntKeyParts].fRotZ);
	}
}