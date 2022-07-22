#pragma once
/*=============================================================

 SharpAt.h
 #@（SharpAt）インタプリタ用の共通ヘッダ

 Version : 1.05

 ＜バージョン履歴＞
 1.01 : 派生classで拡張関数を追加し易いように、p_ExProc等をクラス内に入れ、SetExPr() で新しい拡張関数を設定できるように変更
 1.02 : GP;やGB;を追加。また、Gn,x,y; は Gn; と省略可能に変更
 1.03 : 描画時、cgrp.baiを使っていなかったものを修正、cosやsin計算を追加
 1.04 : 最新状態でのリンクエラー修正。GTのテキスト表示を修正
 1.05 : GTの[];などの表示に対応
 1.06 : Fでのファイル操作に対応
 1.07 : Fでのファイル位置を変更、T系のテキスト関数群を追加

//=============================================================*/

#ifndef GIWLIB
#define _DXLIB	// 有効にすると、DXライブラリで描画する
#endif

#define WIN32_LEAN_AND_MEAN
#include	<windows.h>
#include	<stdarg.h>		// ... を使う為のヘッダ

#ifdef _DXLIB
//--------------------------------------
#pragma warning( disable: 4996 )      // sprintf等を使えるようにする
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<time.h>
#include	"DxLib.h"	// DXライブラリ

// エラーチェック用
#define ERET(p) {if(p==-1) return -1;}

// 絶対値
inline int _PM(int n)
{
	if(n<0) n *= -1;
	return n;
}
// dtを範囲内の値にし、戻り値で返す
inline int MinMax(int dt,int min,int max)
{
	if(dt<min)	return min;
	if(dt>max)	return max;
	return dt;
}
// DXLibでは何もしない
inline void InitKeyFlag(){;}
// SYSTEMに返す処理
inline int SysTask(){if(ProcessMessage()==0) return 0; return -1;}

#else
//--------------------------------------
#include	"main.h"

#endif


//=======================================
// 定数
//=======================================
class CSharpAt;
typedef int(*SHARP_AT_PROC)(CSharpAt*,char*);	// 拡張関数を呼び出す関数のフォーマット

#define SAT_DEF_ALF_MAX 26		// a～z
#define SAT_MEM1_MAX 1000		// 1メモリの最大数
#define SAT_DEF_MEM_MAX (SAT_DEF_ALF_MAX*SAT_MEM1_MAX)	// 基本メモリのサイズ
#define SAT_SRC_SIZE_MAX (1024*1024)	// ソースファイルの最大サイズ
#define SAT_STACK_MAX (1024*1024/4)		// スタックサイズ
#define SAT_PROC_MAX 1000		// 関数の位置保持用
#define SAT_STR_MAX 1024		// 関数内の文字列取得用サイズ
#define SAT_GRP_MAX 1000	// 最大画像数
#define SAT_SE_MAX 1000		// 最大効果音数

// @ポインタの不正範囲？
inline bool IsAtOver(int at) {
	return ((at < 0) || (at >= SAT_DEF_MEM_MAX));
}

//=======================================
// #@制御クラス：定義
//=======================================
class CSharpAt
{
public:
	// DATA
	int		pos;					// 現在のソースの位置
	int		at;						// @ポインタの現在地
	int		fsize;					// 読み込んだソースのファイルサイズ
	int		st_pos;					// スタック位置
	char	err;					// 0以外ならエラーが発生した：1ならプログラムのみ終了。-1なら呼び出し側のプログラムも終了させる必要がある
	char	*src;					// ソース制御用
	char	_src[SAT_SRC_SIZE_MAX + 16];	// ここにソースを読み込む：1MBくらに制限する（少し余分に確保）
	int		mem[SAT_DEF_MEM_MAX + 16];	// 通常メモリ[a～z(26) * 0～999(1000)]（少し余分に確保）
	int		stack[SAT_STACK_MAX];	// 関数や繰り返し用のスタック[SAT_STACK_MAX]
	int		proc[SAT_PROC_MAX];		// 関数の位置を保持[SAT_PROC_MAX]：proc[0]には、#0関数の開始位置、{の次の位置が入る
	SHARP_AT_PROC p_ExProc[26];		// 拡張関数用
	static char *sat_def_fname;		// デフォルトのファイル名を設定しておき
	static char	*box_title;			// MES_PUT時のタイトルバーの文字列を設定しておく

	// コンストラクタ
	CSharpAt();
	// デストラクタ
	~CSharpAt() { Free(); }

	// 全て初期化
	void Free();
	// ソースファイルを読み込む
	//RET: -1=ERR
	int LoadSrcFile(char *fName);
	// 指定の値をスタックする
	void CtPush(int n);
	// スタックから取り出す
	int CtPop();
	// コメントや改行、Tabをスペースで埋める：ソース読み込み時に実行する関数
	void SetCommentSpcAll();
	// スペースを全て詰める：但し、拡張関数に渡す用の文字列はチェックしない
	//※コメントや改行を無くした状態で実行すること
	void CtSpaceCutSrcAll();
	// 関係ない文字をパス
	//RET: 0=終了
	int CtNextSrc();
	// 文字列部分をパス
	//RET: 0=終了
	int CtSrcStrPass();
	// 指定の文字まで次までパスする
	//RET: 0=終了
	int CtSrcPass(char c);
	// 指定1の文字まで次までパスする。但し、指定2の文字があった場合は、指定1の検出数を増やす
	//※例えば、[・・[・・]・・]で、対応する]まで進みたい時用
	//RET: 0=終了
	int CtSrcPassEx(char c1=']', char c2='[');
	// 変数かどうか調べ、変数なら変数番号とその中身を取得
	//IN&OUT : 変数番号取得用, 変数内の値を取得用
	//RET: 0以外なら取得成功
	int GetMem(int &pn, int &n);
	// 次の数値を取得（数値でない場合は、0を返す）
	int GetNum(int &n);
	// ポインタならその位置と中身の値を取得（ポインタでなければ0を返す）
	//OUT: pn=ポインタ位置を取得、n=中身の値を取得
	int GetPtNum(int &pn,int &n);
	// 変数か数字かポインタなら、その値をnに設定
	//RET: 0以外なら取得成功
	int GetNumEx(int &n);
	// 指定の文字列から、GetNumEx を行う
	//RET: 取得した文字数（0=エラー）
	int GetNumExStr(char *str, int &n);
	// カンマ区切りの数値（変数やポインタもOK）を指定数取得する
	//IN : 取得する文字列, 取得したint型を入れる配列, 取得数, 取得完了位置を取得（0なら取得しない）
	//RET: 取得した数（0=失敗）
	int GetCommaNum(char *str, int *p_n, int num, char **s_end=0);
	// 文字列を指定文字まで取得（SAT_STR_MAXまで取得可能）
	//RET: 取得した文字数
	int GetStr(char *str, char c = ';');
	// （汎用）指定文字か終わりまでの文字数を取得
	static size_t Util_GetStrLen(const char* str, char chk_c);
	// （汎用）指定文字列か終わりまでの文字数を取得
	static size_t Util_GetStrLen(const char* str, const char* chk);

	// 拡張機能なら、拡張関数を実行する（0以外なら拡張機能を実行した）
	//OUT: retに拡張関数からの戻り値が入る
	int PrEx(int &ret);

	// （汎用）文字列表示
	static void DrawString(int x, int y, const char* str);

	// @ポインタを踏まえたメモリ位置を取得
	int* GetAt() {
		if (IsAtOver(at)) return mem;
		return &(mem[at]);
	}

	// 未対応の文字列を表示用
	void NON_CODE_PUT(int _pos);
	// エラー表示用：エラーの箇所から数文字分のプログラムを表示
	//IN : エラー位置
	void ERR_PUT(int _pos);

	//=======================================
	// #@のメインループ内の処理：AI対戦等で、1ターン毎実行する時にも使える
	//RET: 0なら終了
	int PrSharpAtMain();
	// #@のメインループ処理
	//RET: -1=ERR
	int GoSharpAt();

	//=======================================
	// 拡張関数設定用
	//=======================================
	// 拡張関数を設定する
	//IN : 'S'等の識別子, 設定する拡張関数
	void SetExPr(char c,SHARP_AT_PROC p){p_ExProc[c-'A'] = p;}

	//=======================================
	// 基本の拡張関数群
	//=======================================
	static int ExPr_D(CSharpAt *ps,char *s);	// 未対応コードであることを表示
	static int ExPr_Calc(CSharpAt *ps,char *s);	// C：乱数や計算用
	static int ExPr_DMes(CSharpAt *ps,char *s);	// D：デバッグ表示用
	static int ExPr_Grp(CSharpAt *ps,char *s);	// G：画像描画
	static int ExPr_Key(CSharpAt *ps,char *s);	// K：キー操作取得
	static int ExPr_Sound(CSharpAt *ps,char *s);// S：サウンド用
	static int ExPr_File(CSharpAt* ps, char* s);// F：ファイル用
	static int ExPr_Text(CSharpAt* ps, char* s);// T：テキスト用

	//=======================================
	// static関数＆DATA
	//=======================================
	// DATA
	static char str_buf[SAT_STR_MAX + 16];	// 文字列取得用のバッファ
	static char str_buf2[SAT_STR_MAX + 16];	// 文字列取得用のバッファ2
	// メッセージ表示用：主にデバッグやエラー用
	static void MES_PUT(char *mes, ...);
	// 1バイト目が全角文字なら1を返す
	static int IsZenkaku(char c);
	// 文字列を指定文字まで取得（SAT_STR_MAXまで取得可能）
	//IN : 格納バッファ、取り出す側、チェックする文字
	//RET: 取得した文字数
	static int GetStrEx(char *buf, char *moto, char chk_c);
	// 文字列内に、変数用の数字（0～999）があれば取得し、無ければ0を設定
	//RET: 進んだ数
	static int GetMemNum(char *s,int &n);
	// 文字列から、数値を取得（数値がない場合は、0を返す）
	//RET: 進んだ数 
	static int GetNumStr(char *s,int &n);


};	// class SharpAt


#ifdef _DXLIB
// 外部変数
extern int fontH;				// フォント用
extern int GMX;// = 640;	// 現在の画面サイズX
extern int GMY;// = 480;	// 現在の画面サイズY
extern int grp[SAT_GRP_MAX];	// 画像用
extern int se[SAT_SE_MAX];		// サウンド用
#endif

extern char szExecDir[];	// ファイルパス用
extern int szExecDirLen;

// ディレクトリを１つ戻す（'\\'の後をNULLにする）
//RET:戻した後の文字列の長さ
int GetBackDir(char* dir);