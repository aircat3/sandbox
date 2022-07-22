// SAT_Key.cpp
// #@用拡張関数：K：キー操作取得用
#include	"SharpAt.h"

/*

＜使用例＞
a=KA;		// A～Z、0～9等のキー判定用、指定のキーが押されていれば1、そうでなければ0を返す
a=KUP;		// UP DW LF RT で上下左右キーも判定可能
a=KEn;		// En=Enter, Es=Esc, Ta=Tab, Ct=Ctrl, Sp=Space, Sh=Shiftキーも判定可能
a=KML;		// ML=マウスの左クリック, MR=マウスの右クリック
a=KMX;		// MX=マウスのX座標, MY=マウスのY座標
（上記以外は、必要に応じて追加していく）

＜注意＞
※現在は、画面Flip時にキー操作判定が入る

*/

//=======================================
// 定義系
//=======================================

//=======================================
// 拡張関数 K：キー操作取得
//=======================================
#ifdef _DXLIB
int CSharpAt::ExPr_Key(CSharpAt *ps,char *s)
{
	// マウス系？
	if(s[0] == 'M')
	{
		int x,y;
		switch(s[1])
		{
		case 'L': return ((GetMouseInput() & MOUSE_INPUT_LEFT)!=0);
		case 'R': return ((GetMouseInput() & MOUSE_INPUT_RIGHT)!=0);
		case 'X': GetMousePoint(&x,&y); return x;
		case 'Y': GetMousePoint(&x,&y); return y;
		}
		return 0;	// ERR
	}
	// 上下左右キー
	if((s[0] == 'U') && (s[1] == 'P'))
		return ((CheckHitKey(KEY_INPUT_UP))!=0);
	if((s[0] == 'D') && (s[1] == 'W'))
		return ((CheckHitKey(KEY_INPUT_DOWN))!=0);
	if((s[0] == 'R') && (s[1] == 'T'))
		return ((CheckHitKey(KEY_INPUT_RIGHT))!=0);
	if((s[0] == 'L') && (s[1] == 'F'))
		return ((CheckHitKey(KEY_INPUT_LEFT))!=0);
	// スペース等
	if((s[0] == 'S') && (s[1] == 'p'))
		return ((CheckHitKey(KEY_INPUT_SPACE))!=0);
	if((s[0] == 'E') && (s[1] == 'n'))
		return ((CheckHitKey(KEY_INPUT_RETURN))!=0);
	if((s[0] == 'E') && (s[1] == 's'))
		return ((CheckHitKey(KEY_INPUT_ESCAPE))!=0);
	if((s[0] == 'T') && (s[1] == 'a'))
		return ((CheckHitKey(KEY_INPUT_TAB))!=0);
	if((s[0] == 'C') && (s[1] == 't'))
		return ((CheckHitKey(KEY_INPUT_LCONTROL)||CheckHitKey(KEY_INPUT_RCONTROL))!=0);
	if((s[0] == 'S') && (s[1] == 'h'))
		return ((CheckHitKey(KEY_INPUT_LSHIFT)||CheckHitKey(KEY_INPUT_RSHIFT))!=0);
	return 0;
}
#else
int CSharpAt::ExPr_Key(CSharpAt *ps,char *s)
{
	// マウス系？
	if(s[0] == 'M')
	{
		switch(s[1])
		{
		case 'L': return ((c_key&MUS_L)!=0);
		case 'R': return ((c_key&MUS_R)!=0);
		case 'X': return musPX;
		case 'Y': return musPY;
		}
		return 0;	// ERR
	}
	// 上下左右キー
	if((s[0] == 'U') && (s[1] == 'P'))
		return ((t_key&KEY_UP)!=0);
	if((s[0] == 'D') && (s[1] == 'W'))
		return ((t_key&KEY_DW)!=0);
	if((s[0] == 'R') && (s[1] == 'T'))
		return ((t_key&KEY_RT) != 0);
	if((s[0] == 'L') && (s[1] == 'F'))
		return ((t_key&KEY_LF) != 0);
	// スペース等
	if((s[0] == 'S') && (s[1] == 'p'))
		return ((c_key&KEY_SPC) != 0);
	if((s[0] == 'E') && (s[1] == 'n'))
		return ((c_key&KEY_RET) != 0);
	if((s[0] == 'E') && (s[1] == 's'))
		return ((c_key&KEY_ESC) != 0);
	if((s[0] == 'T') && (s[1] == 'a'))
		return ((c_key&KEY_TAB) != 0);
	if((s[0] == 'C') && (s[1] == 't'))
		return ((c_key&KEY_CTL) != 0);
	if((s[0] == 'S') && (s[1] == 'h'))
		return ((c_key&KEY_SFT) != 0);
	return 0;
}
#endif
