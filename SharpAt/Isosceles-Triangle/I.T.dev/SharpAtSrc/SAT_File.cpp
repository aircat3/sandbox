// SAT_File.cpp
// #@用拡張関数：F：ファイル用
#include	"SharpAt.h"

/*

・/dt/file??.dat へのファイル操作が可能。
・ファイル番号は 0～99 まで。
・1度に開けるファイルは1つまで。
※メモリ範囲を超えないようにしてある為、不正操作等はできないようにしてあります。

＜使用例＞
　F0;　…　0番ファイルを開く：0～99まで
　FC;　…　開いているファイルを閉じる
　FW50;　…　@ポインタから50個分のメモリを開いてるファイルに書き込む
　FR50;　…　開いてるファイルから50個分の値を、@ポインタに読み込む
　FS-10;　…　ファイル位置を10個分戻す
　FS10;　…　ファイル位置を10個分進める

 ※1個分＝4バイト

 */

// ファイル用
static FILE* fID = nullptr;

// ファイルを開く
static FILE* FileOpen(int n)
{
	if (fID)
	{
		fclose(fID);
		fID = nullptr;
	}
	char fname[320];
	sprintf(fname, "%sdt/file%d.dat", szExecDir, n);
	fID = fopen(fname, "wb+");
	return fID;
}

//=======================================
// 拡張関数 F：ファイル用
//=======================================
int CSharpAt::ExPr_File(CSharpAt *ps,char *s)
{
	// 数値ならファイルを開く
	int n;
	if (ps->GetNumExStr(s, n))
	{
		// 0-99まで
		if (n >= 0 && n <= 99)
		{
			if (FileOpen(n) == nullptr)
			{
				MES_PUT("Error: File Open = %d", n);
				ps->err = 1;
			}
		}
		else
		{
			MES_PUT("Error: File No Over = %d", n);
			ps->err = 1;
		}
		return 0;
	}
	// その他を調べる
	char c = *s ++;
	if (ps->GetNumExStr(s, n)==0)
	{
		if (c == 'C')	// Close
		{
			if (fID)
				fclose(fID);
			fID = nullptr;
			return 0;
		}
		ps->ERR_PUT(ps->pos - 4);
		return 0;
	}
	switch (c)
	{
	case 'W':	// 書き込み
		{
			// 範囲超え？
			if (IsAtOver(ps->at + n))
			{
				MES_PUT("Error: File Write Over = %d", n);
				ps->err = 1;
				return 0;
			}
			int* p = ps->GetAt();
			fwrite(p, sizeof(int), n, fID);
		}
		break;
	case 'R':	// 読み込み
		{
			// 範囲超え？
			if (IsAtOver(ps->at + n))
			{
				MES_PUT("Error: File Read Over = %d", n);
				ps->err = 1;
				return 0;
			}
			int* p = ps->GetAt();
			fread(p, sizeof(int), n, fID);
		}
		break;
	case 'S':	// シーク
		fseek(fID, n * sizeof(int), SEEK_CUR);
		break;
	default:
		// 文字列のまま出力
		CSharpAt::MES_PUT(s);
		break;
	}
	return 0;
}
