// SAT_Text.cpp
// #@用拡張関数：T：テキスト用
#include "SharpAt.h"
#include <vector>
#include <string>

/*

・/dt/t??.dat からのテキストデータ読み込みが可能。
・ファイル番号は 0～99 まで。
・1度に使えるファイルは1つまで。
・通常のメモリではなく、専用のstringへ保持。

＜使用例＞
　TL0;　…　t0番ファイルを開く：0～99まで
　TG0,x,y;　…　0行目をx,yの位置に表示
  TB0;　…　作業バッファにstring専用領域の0行目をコピー
  TBSa;　…　作業バッファ内の最初の $ 位置にstring専用領域のa行目を挿入
  TBIa;　…　作業バッファ内の最初の $ 位置に変数aの値を挿入
  TBGx,y;　…　作業バッファの文字列を、x,yの位置に表示
 */

// 専用string
static std::vector<std::string> stringList;	// 読み込んだ全文字列
static std::string stringBuf; // 作業バッファ用

// ファイルから文字列を読み込む
static int LoadText(int n)
{
	// ファイルを開く
	char fname[320];
	sprintf(fname, "%sdt/t%d.dat", szExecDir, n);
	FILE* fp = fopen(fname, "rb");
	if (fp == nullptr)
		return -1;
	fpos_t fsize = 0;
	long fsizeb = fseek(fp, 0, SEEK_END);
	fgetpos(fp, &fsize);
	fseek(fp, fsizeb, SEEK_SET);
	// 全読み込み
	fsize++;
	std::string buf((size_t)fsize, 0);
	char* str_all = (char*)buf.c_str();
	fread((void*)str_all, 1, (size_t)fsize, fp);
	// 分割して保持
	stringList.clear();
	{
		size_t n = 0, max = (size_t)fsize;
		while (n < max)
		{
			size_t len = CSharpAt::Util_GetStrLen(str_all + n, '\n');
			if (len == 0) break;
			str_all[n + len] = 0;
			if (str_all[n + len-1] == '\r')
				str_all[n + len-1] = 0;
			std::string str(len,0);
			str = str_all + n;
			stringList.push_back(str);
			n += len+1;
		}
	}
	// 終了
	fclose(fp);
	return 0;	// OK
}

// List内の文字列を取得
static const char* GetString(int n)
{
	if (stringList.size() > (size_t)n)
		return stringList[n].c_str();
	return " ";
}

//=======================================
// 拡張関数 T：テキスト用
//=======================================
int CSharpAt::ExPr_Text(CSharpAt *ps,char *s)
{
	char* get = 0;
	int p[8];	// 連続した値を取得用
	int n;
	char c = *s ++;
	if (ps->GetNumExStr(s, n)==0)
	{
		// バッファ内制御？
		if (c == 'B')
		{
			c = *s++;
			// 表示？
			if (c == 'G')
			{
				if (ps->GetCommaNum(s, p, 2, &get) != 2)	// カンマ区切りの数値を取得
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				DrawString(p[0], p[1], stringBuf.c_str());
				return 0;
			}
			if (ps->GetNumExStr(s, n)) // OK?
			{
				switch (c)
				{
				case 'S': // 文字列
					{
						auto pos = Util_GetStrLen(stringBuf.c_str(), '$');
						if (pos >= stringBuf.length())
							return 0;
						stringBuf.erase(pos, 1);
						stringBuf.insert(pos, GetString(n));
					}
					return 0;
				case 'I': // 整数
					{
						auto pos = Util_GetStrLen(stringBuf.c_str(), '$');
						if (pos >= stringBuf.length())
							return 0;
						stringBuf.erase(pos, 1);
						stringBuf.insert(pos, std::to_string(n));
					}
					return 0;
				}
			}
		}
		ps->ERR_PUT(ps->pos - 4);
		return 0;
	}
	switch (c)
	{
	case 'L':	// 読み込み
		{
			// 0-99まで
			if (n >= 0 && n <= 99)
			{
				if (LoadText(n) == -1)
				{
					MES_PUT("Error: Text Open = %d", n);
					ps->err = 1;
				}
			}
			else
			{
				MES_PUT("Error: Text File No Over = %d", n);
				ps->err = 1;
			}
		}
		return 0;
	case 'G':	// 表示
		{
			// 座標取得
			s+=2;
			if (ps->GetCommaNum(s, p, 2, &get) != 2)	// カンマ区切りの数値を取得
			{
				ps->ERR_PUT(ps->pos - 4);	// ERR
				return 0;
			}
			// 位置を移動		
			//s = get;
			// 作成したフォントで描画
			DrawString(p[0], p[1], GetString(n));
		}
		return 0;
	case 'B':	// バッファへベース文字列を設定
		stringBuf = GetString(n);
		return 0;
	default:
		// 文字列のまま出力
		CSharpAt::MES_PUT(s);
		break;
	}
	return 0;
}
