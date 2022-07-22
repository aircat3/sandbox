// SAT_Grp.cpp
// #@用拡張関数：G：画像描画用
#include	"SharpAt.h"

/*

＜使用例＞（数値のところは、全て変数やポインタを使える）
※起動時に、G0.png～G999.pngを自動的に読み込む。（画像が無ければパスしていく）
GS640,480;	// 画面サイズを設定：デフォルトは640x480

G0,100,200;	// G0.pngを100,200の位置に描画：x,y座標は省略可
GP;			// r,g,bの色で、x,yの位置に点を描画（アルファ値対応）
GB;			// r,g,bの色で、x,yの位置から、x1,y1のサイズの四角を描画（アルファ値対応）
GM4;		// 描画時の座標に、指定の数値を掛ける。GM4なら4倍なので、G0,10,10;の場合、x=40,y=40に描画することになる
GF;			// Flip命令等の実際に画面に表示する処理（上記は全て裏バッファに描画している為、Flipしないと画面に表示されない）
GA40;		// 以降のアルファ値を設定：40なら40％、0なら表示されない

GC;			// 画面を黒で塗りつぶす（GAによるアルファ値も有効）
GCr,g,b;	// 画面を指定のRGB値で塗りつぶす（GAによるアルファ値も有効）

GW50;		// Flip前に、ウエイトを入れるようにする：値はms（1000で1秒）※ウエイト無しの場合、約60fpsになっている
※Shiftキーを押している間、ウエイトが無い状態になる

GT10,20"・・・";	// ""内の文字列を10,20の位置に描画（文字サイズはデフォルト）
GT10,20,a;			// a0の値を10,20の位置に描画（文字サイズはデフォルト）

*/

//=======================================
// 定義系
//=======================================
#define SAT_GRP_MAX	1000	// 読み込める枚数（0～SAT_GRP_MAX-1.png）
// 画像タイプ
enum
{
	GRPDT_NM=0,		// 通常描画
	GRPDT_V,		// 上下反転
	GRPDT_H,		// 左右反転
};

//=======================================
// Grp用クラス
//=======================================
class CGrp
{
public:
	int	bai;	// 座標倍率
	int gmx, gmy;	// 画面サイズ
	int wait;	// ミリ秒単位
	char dtype;	// 描画タイプ：GRPDT_???
	float alpha;	// アルファ率：0-1.0f：0なら描画しない

	CGrp();
	~CGrp(){;}	// 必要ならデストラクタで解放処理を行う
};
// コンストラクタで画像を読み込む
CGrp::CGrp()
{
	// DATA初期化
	bai = 1;
	dtype = GRPDT_NM;
	alpha = 100;
	gmx = GMX;
	gmy = GMY;
	wait = 0;
	// G0.png～G999.pngを読み込む　※ゲームでは読み込み済みなので何もしない
	;
}
static CGrp cgrp;	// Grp用画像クラス

#ifdef _DXLIB
// 画像を全て読み込む
void LoadGrpAll()
{
	char buf[256];
	for(int i=0; i<SAT_GRP_MAX; i++)
	{
		sprintf(buf,"dt/G%d.png",i);
		int sf = FileRead_open(buf);	// ファイルがあるか調べる
		if(sf==0)
		{
			grp[i] = -1;
			continue;
		}
		FileRead_close(sf);
		grp[i] = LoadGraph(buf);
	}
}
#endif

// （汎用）文字列表示
void CSharpAt::DrawString(int x,int y,const char* str)
{
	if (!str) return;
	int sx = x * cgrp.bai;
	int sy = y * cgrp.bai;
	int incy = 18 * cgrp.bai;
	auto str_en = str + strlen(str);
	while (str < str_en)
	{
		// 改行コードで分ける
		auto next = Util_GetStrLen(str, "\\n");
		// 作成したフォントで描画
		DrawNStringToHandle(sx, sy, str, next, GetColor(255, 255, 255), fontH);
		if (str[next] == 0)
			break;
		str += next+2;
		sy += incy;
	}
}

//=======================================
// 拡張関数 G：画像描画
//=======================================
int CSharpAt::ExPr_Grp(CSharpAt *ps,char *s)	// *sには、最初のGと最後の;が無い状態の文字列が入っている
{
	char *get=0;
	int n, x,y,x1,y1,r,g,b;
	int p[8];	// 連続した値を取得用
	switch( *s )
	{
	case 'P':	// r,g,bの色で、x,yの位置に点を描画（アルファ値対応）
		x = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX];
		y = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX];
		r = ps->mem[(int)('r' - 'a')*SAT_MEM1_MAX];
		g = ps->mem[(int)('g' - 'a')*SAT_MEM1_MAX];
		b = ps->mem[(int)('b' - 'a')*SAT_MEM1_MAX];
#ifdef _DXLIB
		DrawPixel(x,y,GetColor(r,g,b));
#else
		if( HitTest_fast((DWORD)x,(DWORD)y,vram) )
		{
			n = (x * cgrp.bai)+(y * cgrp.bai)*GMX;
			WORD *mp = &((WORD*)vram.map)[n];
			if(cgrp.alpha >= 1.0f)
				*mp = RGB555(r,g,b);
			else
				*mp = SpCol(*mp,(WORD)r,(WORD)g,(WORD)b,cgrp.alpha);
		}
#endif
		return 0;
	case 'B':	// r,g,bの色で、x,yの位置から、x1,y1のサイズの四角を描画（アルファ値対応）
		x = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX];
		y = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX];
		x1 = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX +1];
		y1 = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX +1];
		r = ps->mem[(int)('r' - 'a')*SAT_MEM1_MAX];
		g = ps->mem[(int)('g' - 'a')*SAT_MEM1_MAX];
		b = ps->mem[(int)('b' - 'a')*SAT_MEM1_MAX];
#ifdef _DXLIB
		DrawBox(x,y,x+x1-1,y+y1-1,GetColor(r,g,b),TRUE);
#else
		if(cgrp.alpha >= 1.0f)
			GRP_DrawBox16(vram,vram,x * cgrp.bai,y * cgrp.bai,x1,y1,RGB555(r,g,b));
		else
			GRP_DrawBox16SpCol(vram,vram,x * cgrp.bai,y * cgrp.bai,x1,y1,RGB555(r,g,b),cgrp.alpha);
#endif
		return 0;
	case 'F':	// Flip（画面を更新）
#ifdef _DXLIB
		if((cgrp.wait>0) && ((CheckHitKey(KEY_INPUT_LSHIFT))==0))
			Sleep(cgrp.wait);
		// 画面をフリップ
		ScreenFlip();
#else
		if((cgrp.wait>0) && ((wm.c_key&KEY_SFT)==0))
			Sleep(cgrp.wait);
		ERET( SysTask() );
		// 操作を取得
		GetGameKey();
		// ウエイト：シフトキーで高速化する
		ERET(Eff::EffWait(wm.c_key));
		if( PaintMain(1)==-1 )
		{
			ps->err = -1;	// -1=強制終了させる
			return -1;
		}
		// F8キーで画面をBMPで保存
		if (wm.f_key == KEY_F8)
		{
			wm.cGdi.SaveBmp16(SetPath("scr_shot.bmp", 1));	//スクリーンショット
			InitKeyFlag();
		}
#endif
		break;
	case 'S':	// 画面サイズ変更
		s++;
#ifdef _DXLIB
		if(ps->GetCommaNum(s,p,2)!=2)	// カンマ区切りの数値を取得
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		// 画面サイズ変更
		if( SetGraphMode(p[0],p[1],32)==DX_CHANGESCREEN_OK )	// 成功？
		{
			cgrp.gmx = GMX = p[0];
			cgrp.gmy = GMY = p[1];
			SetDrawScreen(DX_SCREEN_BACK);
			// 画像を全て読み込む
			LoadGrpAll();
			// フォント作成
			void CreFontDef();
			CreFontDef();
		}
#endif
		break;
	case 'M':	// 描画座標の倍率設定
		s++;
		if(ps->GetNumExStr(s, n)==0 )	// 数値や変数から値を取得
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		cgrp.bai = n;
		break;
	case 'D':	// 描画タイプを設定
		s++;
		if (ps->GetNumExStr(s, n) == 0)	// 数値や変数から値を取得
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		cgrp.dtype = n;
		break;
	case 'A':	// アルファ値を設定
		s++;
		if (ps->GetNumExStr(s, n) == 0)	// 数値や変数から値を取得
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		n = MinMax(n,0,100);
#ifdef _DXLIB
		if( n>=100 )
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		else
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,(n*255)/100);
#endif
		cgrp.alpha = (float)n / 100.0f;
		break;
	case 'W':	// ウエイト設定
		s++;
		if (ps->GetNumExStr(s, n) == 0)	// 数値や変数から値を取得
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		cgrp.wait = n;
		break;
	case 'C':	// 画面を黒で塗りつぶす（GAによるアルファ値も有効）
		{
			s++;
#ifdef _DXLIB
			DWORD cl = GetColor(0,0,0);
			if(*s!=0)
			{
				if(ps->GetCommaNum(s,p,3)!=3)	// カンマ区切りの数値を取得
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				cl = GetColor(p[0],p[1],p[2]);
			}
			DrawBox(0,0,cgrp.gmx,cgrp.gmy,cl,TRUE);
#else
			// 色値がある？
			WORD cl = 0;
			if( *s!=0 )
			{
				if(ps->GetCommaNum(s, p, 3)!=3 )	// カンマ区切りの数値を取得
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				cl = RGB555(p[0], p[1], p[2]);
			}
			// アルファ無し？
			if( cgrp.alpha>=1.0f )
				GRP_DrawBox16(vram,vram,0,0,cgrp.gmx,cgrp.gmy, cl);
			else
				GRP_DrawBox16SpCol(vram,0,0,cgrp.gmx,cgrp.gmy, cl, cgrp.alpha);
#endif
		}
		break;
	case 'T':	// テキスト
		s++;
		if (ps->GetCommaNum(s, p, 2, &get) != 2)	// カンマ区切りの数値を取得
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		// 位置を移動		
		s = get;
		{
#ifdef _DXLIB
			// 文字列？
			if(*s=='\"')
			{
				s++;
				// 文字を描画
				CSharpAt::GetStrEx(CSharpAt::str_buf2,s,'\"');
				// 作成したフォントで描画
				DrawString(p[0], p[1], CSharpAt::str_buf2);
			}
			else
			{
				// 数値を描画
				if(ps->GetNumExStr(s,n)==0)
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				itoa(n,CSharpAt::str_buf2,10);
				DrawString(p[0], p[1], CSharpAt::str_buf2);
			}
#else
			// 文字列？
			if( *s=='\"' )
			{
				s++;
				// 文字を描画
				CSharpAt::GetStrEx(CSharpAt::str_buf2,s,'\"');
				GM::DrKanji(vram,p[0] * cgrp.bai,p[1] * cgrp.bai, CSharpAt::str_buf2);
			}else
			{
				// 数値を描画
				if(ps->GetNumExStr(s,n)==0 )
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				DrawNum(vram,p[0] * cgrp.bai,p[1] * cgrp.bai,n);//,0,0,0,0, (float)grp.alpha / 100.0f);
			}
#endif
		}
		break;
	default:	// 画像表示
		n = ps->GetCommaNum(s,p,3);
		if( n==1 )	// 座標を省略した？
		{
			p[1] = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX];
			p[2] = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX];
		}
		else if (n != 3)	// カンマ区切りの数値を取得
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		// 範囲外？
		if( (p[0]<0) || (p[0]>=SAT_GRP_MAX) )
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		{
#ifdef _DXLIB
			// 画像番号をゲーム用に変換
			int gno = p[0];
			if(grp[gno]<=-1)	// これ以上は絵が無い
				break;
			// 描画
			DrawGraph(p[1]*cgrp.bai,p[2]*cgrp.bai,grp[gno],TRUE);
#else
			// 画像番号をゲーム用に変換
			int gno = p[0];
			if( gno>=Grp::sat.MaxHed() )	// これ以上は絵が無い
				break;
			// アルファ無し？
			if (cgrp.alpha >= 1.0f)
				Grp::sat.HDraw(vram,vram,p[1]*cgrp.bai,p[2] * cgrp.bai, gno);
			else
				Grp::sat.HDrawSpCol(vram, vram, p[1] * cgrp.bai, p[2] * cgrp.bai, gno, cgrp.alpha);
#endif
		}
		break;
	}

	// $$ 文字列のまま出力
//	CSharpAt::MES_PUT(s);
	return 0;
}

