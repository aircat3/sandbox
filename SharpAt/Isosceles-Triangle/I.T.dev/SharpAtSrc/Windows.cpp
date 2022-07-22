// Windows.cpp

#include	"SharpAt.h"

// DATA
int fontH=0;			// フォント用
int GMX = 640;	// 現在の画面サイズX
int GMY = 480;	// 現在の画面サイズY
int grp[SAT_GRP_MAX];	// 画像用
int se[SAT_SE_MAX];		// サウンド用

char szExecDir[260];	// ファイルパス用
int szExecDirLen = 0;

// #@の実行関数
int StartSharpAt(char *p_fName);
// 画像を全て読み込む
void LoadGrpAll();

// フォント解放＆作成
void FreeFontDef()
{
	if(fontH)
		DeleteFontToHandle(fontH);
	fontH = 0;
}
void CreFontDef()
{
	FreeFontDef();
	fontH = CreateFontToHandle(NULL,16,8);
}

// ファイルが存在する？
bool IsFile(const char* fname)
{
	char fn[320];
	sprintf(fn, "%s%s", szExecDir, fname);
	auto fp = fopen(fn, "r");
	if (fp == nullptr) return false;
	fclose(fp);
	return true;
}

// プログラム開始
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	SetUseDirectInputFlag(FALSE);	// デバッグモードで重くなるのを回避する用
	SetOutApplicationLogValidFlag(FALSE);	// LOGファイルを出力しない
	// TITLE変更
	char title[] = "#@(SharpAt) Language";
	SetMainWindowText(title);
	// パス取得
	{
		int len = GetModuleFileName(0, szExecDir, sizeof(szExecDir));
		szExecDirLen = GetBackDir(szExecDir);
	}
	//-----------------------------------------------------------------------
	// デバッグファイルがあるなら、フルスクリーン確認をパス
	if(!IsFile("debug.ini"))
	{
		// フルスクリーン？
		char *cmd = lpCmdLine;
		if( cmd[0]==0 )
			cmd = CSharpAt::sat_def_fname;
		sprintf(CSharpAt::str_buf, "Program file : %s\n\nFull screen mode?",cmd);
		if( MessageBox(GetMainWindowHandle(),CSharpAt::str_buf,title,MB_YESNO)!=IDYES)
		{
			ChangeWindowMode(TRUE);			// TRUEにするとウインドウモードで起動
		}
	}else
		ChangeWindowMode(TRUE);			// TRUEにするとウインドウモードで起動
	//-----------------------------------------------------------------------
	if(DxLib_Init() == -1)			// DXライブラリを開始する処理（ゲーム用の画面が作られる）
		return -1;
	// マウス表示OFF
//	SetMouseDispFlag(false);

	// フォント作成
	CreFontDef();

	// 画像を全て読み込む
	LoadGrpAll();
	// 効果音を読み込む
	char buf[260];
	for(int i=0; i<SAT_SE_MAX; i++)
	{
		sprintf(buf,"%sdt/S%d.ogg", szExecDir, i);
		int sf = FileRead_open(buf);	// ファイルがあるか調べる
		if(sf==0)
		{
			se[i] = -1;
			continue;
		}
		FileRead_close(sf);
		se[i] = LoadSoundMem(buf);
	}

	// 描画先を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// #@の実行関数
	StartSharpAt(lpCmdLine);

	// 作成したフォントデータを削除する
	FreeFontDef();

	DxLib_End();		// DXライブラリを終了する（ゲーム画面を閉じる）
	return 0;			// プログラムを終了する
}
