// SAT_Sound.cpp
// #@用拡張関数：S：サウンド用
#include	"SharpAt.h"

/*

＜使用例＞
※起動時に、S0.ogg～S99.oggの音データを読み込む。（ファイルが無ければパスしていく）
S0;		// S0.oggを1回再生
SS0;	// S0.oggの再生を停止
SL0;	// S0.oggをループ再生
SM0;	// （曲用）M0.oggをループ再生。曲用のファイルは実行された時に読み込む
SMS;	// 曲の再生を停止

*/

//=======================================
// 拡張関数 S：サウンド用
//=======================================
int CSharpAt::ExPr_Sound(CSharpAt *ps,char *s)
{
	// 数値なら効果音
	int n;
	if (ps->GetNumExStr(s, n) )
	{
		// 効果音を再生
#ifdef _DXLIB
		if( (n>=0) && (n<SAT_SE_MAX) )
		{
			if(se[n]>=0)
				PlaySoundMem(se[n],DX_PLAYTYPE_BACK);
		}
#else
		SE::Play(n);
#endif
		return 0;
	}
	// その他を調べる
	char c = *s ++;
	if (ps->GetNumExStr(s, n)==0)
	{
		ps->ERR_PUT(ps->pos - 4);
		ps->err = 1;	// 1=通常エラー
		return 0;
	}
	switch (c)
	{
	case 'M':	// BGM系
#ifdef _DXLIB
		if(n<=-1)	// 停止？
			StopMusic();
		else
		{
			char buf[256];
			sprintf(buf,"dt/M%d.mp3",n);
			PlayMusic(buf,DX_PLAYTYPE_LOOP);
		}
#else
		if(n<=-1)	// 停止？
			BGM::Stop();
		else
			BGM::Play(n);
#endif
		break;
	case 'S':	// サウンド停止
#ifdef _DXLIB
		if((n>=0) && (n<SAT_SE_MAX))
			StopSoundMem(se[n]);
#else
		SE::Stop(n);
#endif
		break;
	case 'L':	// 効果音をループ再生
#ifdef _DXLIB
		if((n>=0) && (n<SAT_SE_MAX))
			PlaySoundMem(se[n],DX_PLAYTYPE_LOOP);
#else
		SE::Play(n,1);
#endif
		break;
	default:
		// 文字列のまま出力
		CSharpAt::MES_PUT(s);
		break;
	}
	return 0;
}
