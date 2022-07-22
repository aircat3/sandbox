// SAT_Calc.cpp
// #@用拡張関数：C：乱数や計算用
#include "SharpAt.h"
#include <time.h>

/*

＜使用例＞
a=CR;	// int型の乱数を返す
a=CC-10/a+b*((5+c)%8);	// 通常の計算処理を行い、結果を返す
a=CC(a>=10)&&((b+5)==0));	// 論理演算も可能
a=CHx,y,x2,y2,s1,s2:	// HitTest用：x,yの値が、矩形（x2,y2,s1,s2）内にあれば1を返し、そうでなければ0を返す。s1,s2はxとyの矩形サイズ
h=CLx,y,x2,y2,n;	// LINE位置を計算する処理：x,yからx2,y2までの線上で、nピクセル進んだ座標を、変数xとyに設定する。戻り値は、進む方向を8方向に分割した際の方向値
※cos、sin、sqrt等の計算は、必要に応じて追加していく
※Cの後に変数や数字が来たら、通常演算を行う：例えば、Ca=b*50+3; 等

*/

// 計算用
#ifdef _DXLIB
const float PI = 3.14159265358979323846f;
const float PI_45 = 0.78539816339744827900f;
const float PI_90 = 1.57079632679489655800f;
const float PI_180 = PI;
const float PI_270 = PI+PI_90;
const float PI_360 = 6.28318530717958623200f;
#endif

// 識別用
enum
{
	CC_NUM,		// 数値

	CC_KL,		// (
	CC_KR,		// )

	CC_KAKE,	// *
	CC_WARU,	// /
	CC_AMARI,	// %

	CC_TASU,	// +
	CC_HIKU,	// -

	CC_RB,		// <
	CC_LB,		// >
	CC_RBEQ,	// <=
	CC_LBEQ,	// >=

	CC_EQ,		// ==
	CC_NEQ,		// !=

	CC_AND,		// &&
	CC_OR,		// ||

	CC_END,		// データ終了
};
// 優先順位テーブル：大きい方を優先
static char cct[]=
{
	0,	//CC_NUM,	// 数値

	6,	//CC_KL,	// (
	6,	//CC_KR,	// )

	5,	//CC_KAKE,	// *
	5,	//CC_WARU,	// /
	5,	//CC_AMARI,	// %

	4,	//CC_TASU,	// +
	4,	//CC_HIKU,	// -

	3,	//CC_RB,	// <
	3,	//CC_LB,	// >
	3,	//CC_RBEQ,	// <=
	3,	//CC_LBEQ,	// >=

	2,	//CC_EQ,	// ==
	2,	//CC_NEQ,	// !=

	1,	//CC_AND,	// &&
	1,	//CC_OR,	// ||

	0,	//CC_END
};

// 計算式の分割保存用データ
typedef struct
{
	int type;	// データの種類：CC_???
	int pt;		// 値
} CC_DATA;
// 計算用データ
#define CCMAX 256
static CC_DATA dt[CCMAX+1];	// CC_TBL構造体で保持

// dtを１つ詰める
//IN : dtの位置、詰める数
static void DelDt(int n,int n2)
{
	for(int i=n; i<CCMAX; i++)
	{
		dt[i].type = dt[i+n2].type;
		dt[i].pt = dt[i+n2].pt;
		if( dt[i].type==CC_END ) break;
	}
}

// （入れ子）dt[]を()内を優先して計算していく
//IN : dtの位置
static int CalcDt(int n)
{
	// 優先順位の高い順に処理する。'('があればその中に入る
	int yuu = cct[CC_KL];	// 現在計算する優先順位
	while(1)
	{
		for(int i=n; i<CCMAX; i++)
		{
_JMP_ST:
			if( dt[i].type==CC_END )	// 終了
			{
				// 計算完了？
				if( (i-1)==n )
					return dt[n].pt;
				break;
			}
			if(dt[i].type==CC_NUM )		// 数値はパス
				continue;
			if( cct[dt[i].type]<yuu )	// 実行しない？
				continue;
			if( dt[i].type==CC_KL )	// (？
			{
				DelDt(i,1);	// dtを詰める
				// )まで計算
				CalcDt(i);
				continue;
			}
			if(dt[i].type==CC_KR)	// )？
			{
				// もう計算式が無い？
				if( (i-1)==n )
				{
					DelDt(i,1);	// dtを詰める
					return dt[n].pt;
				}
				break;
			}
			// 計算を行う
			switch( dt[i].type )
			{
			case CC_KAKE:	// *
				dt[i-1].pt *= dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_WARU:	// /
				if(dt[i+1].pt==0 )
					dt[i-1].pt = 0;	// 0にしておく
				else
					dt[i-1].pt /= dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_AMARI:	// %
				dt[i-1].pt = dt[i-1].pt % dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_TASU:	// +
				dt[i-1].pt += dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_HIKU:	// -
				dt[i-1].pt -= dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_RB:		// <
				dt[i-1].pt = dt[i-1].pt < dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_LB:		// >
				dt[i-1].pt = dt[i-1].pt > dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_RBEQ:	// <=
				dt[i-1].pt = dt[i-1].pt <= dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_LBEQ:	// >=
				dt[i-1].pt = dt[i-1].pt >= dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_EQ:		// ==
				dt[i-1].pt = dt[i-1].pt == dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_NEQ:	// !=
				dt[i-1].pt = dt[i-1].pt != dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_AND:	// &&
				dt[i-1].pt = dt[i-1].pt && dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			case CC_OR:		// ||
				dt[i-1].pt = dt[i-1].pt || dt[i+1].pt;
				DelDt(i,2);	// dtを詰める
				goto _JMP_ST;
			}
		}
		yuu --;	// 優先度を下げる
		if( yuu<=0 ) break;
	}
	return dt[n].pt;
}

// 文字列から、通常の計算を行う
int CalcStr(CSharpAt *psat, char *s)
{
	int n,wk;
	memset(dt,0,sizeof(dt));	// INIT
	//-----------------------------------------------
	// 文字列から、数字や演算子を分けて取得する
	int i;
	for(i=0; i<CCMAX; i++)
	{
		if( *s==0 ) break;
		switch(*s)
		{
		case '(': dt[i].type = CC_KL; s++; break;
		case ')': dt[i].type = CC_KR; s++; break;
		case '*': dt[i].type = CC_KAKE; s++; break;
		case '/': dt[i].type = CC_WARU; s++; break;
		case '%': dt[i].type = CC_AMARI; s++; break;
		case '+':
			// 最初なら、数字へ
			if(i==0) goto _JMP_NUM;
			dt[i].type = CC_TASU; s++; break;
		case '-':
			// 最初なら、数字へ
			if( i==0 ) goto _JMP_NUM;
			dt[i].type = CC_HIKU; s++; break;
		case '<':
			s++;
			if( *s=='=' )
			{
				dt[i].type = CC_RBEQ;
				s++;
			}else
				dt[i].type = CC_RB;
			break;
		case '>':
			s++;
			if(*s=='=')
			{
				dt[i].type = CC_LBEQ;
				s++;
			}else
				dt[i].type = CC_LB;
			break;
		case '=':
			s++;
			if(*s=='=')
				dt[i].type = CC_EQ;
			else
			{
				psat->ERR_PUT(psat->pos-4);	// ERR
				return 0;
			}
			s++;
			break;
		case '!':
			s++;
			if(*s=='=')
				dt[i].type = CC_NEQ;
			else
			{
				psat->ERR_PUT(psat->pos-4);	// ERR
				return 0;
			}
			s++;
			break;
		case '&':
			s++;
			if(*s=='&')
				dt[i].type = CC_AND;
			else
			{
				psat->ERR_PUT(psat->pos-4);	// ERR
				return 0;
			}
			s++;
			break;
		case '|':
			s++;
			if(*s=='|')
				dt[i].type = CC_OR;
			else
			{
				psat->ERR_PUT(psat->pos-4);	// ERR
				return 0;
			}
			s++;
			break;
		case ' ':	s++; break;	// PASS
		default:	// 数値や変数
_JMP_NUM:
			wk = psat->GetNumExStr(s,n);
			if( wk==0 )
			{
				psat->ERR_PUT(psat->pos-4);	// ERR
				return 0;
			}
			dt[i].type = CC_NUM;
			dt[i].pt = n;
			s += wk;
			break;
		}
	}
	// END設定
	dt[i].type = CC_END; i++;
	//-----------------------------------------------
	// ()内を優先して処理していく：入れ子で
	return CalcDt(0);	// 計算結果を返す
}

#ifdef _DXLIB
// 線上(x,y,x2,y2)にある指定(len)の位置(get_x,get_y)を求める
void GetLinePos(int x,int y,int x2,int y2,int len,int &get_x,int &get_y)
{
	int wx2 = x2-x;
	int wy2 = y2-y;
	if(_PM(wx2) > _PM(wy2))	//Ｘが大きい
	{
		if(wx2 < 0)	len *= -1;	//方向修正
		get_x = len;
		if(wx2)
			get_y = (wy2*len)/wx2;
		else
			get_y = 0;
	}
	else					//Ｙが大きい
	{
		if(wy2 < 0)	len *= -1;	//方向修正
		get_y = len;
		if(wy2)
			get_x = (wx2*len)/wy2;
		else
			get_x = 0;
	}
	get_x += x;
	get_y += y;
}

// ラジアンを求める
float GetRadian(float x,float y,float x1,float y1)
{
	//--0時の方向から左回りでラジアン角を求める
	float tei= x1-x;		//底辺
	float naname= y1-y;		//斜め
	int ch=0;
	//--Ｘが０度のとき
	if(tei==0)
	{
		if(naname<0)
			return PI;
		else
			return 0;
	}
	//--Ｙが０度のとき
	if(naname==0)
	{
		if(tei<0)
			return PI/2;
		else
			return PI+PI/2;
	}
	//-------------------------------------------------
	if(naname<0)
	{
		naname = naname * -1;	//マイナス補正
		ch=1;
	}
	naname=(float)hypot(tei,naname);	//長さをＧＥＴ
	naname=tei/naname;
	naname=acosf(naname);
	//	kakudo=(naname*180)/PI;
	if(ch==1)	//Ｙがマイナスの場合は反転
				//	kakudo = 360-kakudo;
		naname = PI*2-naname;
	naname -= PI/2;	//補正
	if(naname<0)
		naname += PI*2;
	return naname;
}
#endif

// ラジアンから８方向を取得
//RET: 0-8方向
static int _GetAngle2Hou(float a)
{
	int hou;
	a += PI_45+0.001f;
	if(a < PI_90) hou=4;	//DW
	else if(a < (PI_90+PI_45)) hou=5;	//LD
	else if(a < (PI_180)) hou=6;	//LF
	else if(a < (PI_180+PI_45)) hou=7;	//UL
	else if(a < (PI_270)) hou=0;	//UP
	else if(a < (PI_270+PI_45)) hou=1;	//UR
	else if(a < (PI_360)) hou=2;	//RT
	else hou=3;	//RD
	return hou;
}

//=======================================
// 拡張関数 C：乱数や計算用
//=======================================
int CSharpAt::ExPr_Calc(CSharpAt *ps,char *s)
{
	int p[8];	// 連続した値を取得用
	switch( *s )
	{
	case 'R':	// 乱数？
		{
			static char flg=0;
			if( flg==0 )
			{
				flg = 1;
				srand((unsigned)time(0));	// 最初だけ乱数を初期化
			}
			return rand();
		}
		break;
	case 'C':	// 通常の計算
		{
			s++;
			return CalcStr(ps,s);
		}
		break;
	case 'H':	// HitTest：矩形内か調べる
		{
			s++;
			if (ps->GetCommaNum(s, p, 6) != 6)	// カンマ区切りの数値を取得
			{
				ps->ERR_PUT(ps->pos - 4);
				return 0;
			}
			int px=p[0], py=p[1];
			if ((px >= p[2]) && (px < p[2] + p[4]))
				if ((py >= p[3]) && (py < p[3] + p[5]))
					return 1;
			return 0;
		}
	case 'L':	// Line位置計算
		{
			s++;
			if(ps->GetCommaNum(s,p,5) != 5)	// カンマ区切りの数値を取得
			{
				ps->ERR_PUT(ps->pos - 4);
				return 0;
			}
			int x=p[0],y=p[1],x2=p[2],y2=p[3];
			GetLinePos(x,y,x2,y2,p[4],ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX],ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX]);
			return _GetAngle2Hou(GetRadian((float)x,(float)y,(float)x2,(float)y2));	// 角度を求め、方向を求める
		}
		break;
	default:
		// 未対応の命令
		ps->NON_CODE_PUT(ps->pos - 4);
		break;
	}
	return 0;
}

