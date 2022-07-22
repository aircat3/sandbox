// SAT_Debug.cpp
// #@用拡張関数：D：デバッグ用のメッセージ表示
#include	"SharpAt.h"

/*

＜使用例＞
Da;	// a0の値をメッセージボックス等で表示
D@;	// @ポインタ内の値を表示
Dあいうえお;	// 文字列をそのまま表示

＜#@への登録方法＞
SharpAt.cpp の p_ExProc に、先頭の関数（ExPr_?）を追加する

*/

//=======================================
// 拡張関数 D：デバッグ用のメッセージ出力
//=======================================
int CSharpAt::ExPr_DMes(CSharpAt *ps, char *s)
{
	// 変数等なら、その値を表示
	int n;
	if (ps->GetNumExStr(s, n))
	{
		CSharpAt::MES_PUT("%d", n);
	}
	else
	{
		// 文字列のまま出力
		CSharpAt::MES_PUT(s);
	}
	return 0;
}

// 未対応コードであることを表示
int CSharpAt::ExPr_D(CSharpAt *ps,char *s)
{
	ps->NON_CODE_PUT(ps->pos-3);
	return 0;
}
