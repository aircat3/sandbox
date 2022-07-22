// SharpAt.cpp
// #@インタプリタ用の基礎プログラム
#include	"SharpAt.h"

//=======================================
// ゲーム用
//=======================================
extern char *str_sharpat_help;	// 開始時の説明用

//=======================================
// DATA
//=======================================
// コマンドラインが無い場合はこのファイルを実行する
#ifdef _DEBUG
static char _sat_def_fname[] = "SharpAt_deb.txt";	// デフォルトのファイル名
#else
static char _sat_def_fname[] = "SharpAt.txt";	// デフォルトのファイル名
#endif
static char _box_title[] = "#@ Language";	// MES_PUTのタイトルバーの文字列（基本）

//=======================================
// CLASS DATA
//=======================================
char CSharpAt::str_buf[SAT_STR_MAX + 16];	// 文字列取得用のバッファ：基本側で使う用
char CSharpAt::str_buf2[SAT_STR_MAX + 16];	// 文字列取得用のバッファ2：主に拡張側で使う用
char *CSharpAt::sat_def_fname = _sat_def_fname;	// デフォルトのファイル名を設定しておき
char *CSharpAt::box_title = _box_title;		// MES_PUT時のタイトルバーの文字列を設定しておく

//=======================================
// #@制御クラス：本体
//=======================================
// コンストラクタ
CSharpAt::CSharpAt()
{
	src = _src; Free();
	// 初期拡張関数を設定
	for(int i=0; i<26; i++)
	{
		p_ExProc[i] = ExPr_D;
	}
	SetExPr('C', ExPr_Calc);	// C：乱数や計算用
	SetExPr('D', ExPr_DMes);	// D：デバッグ用の拡張関数
	SetExPr('G', ExPr_Grp);		// G：画像描画
	SetExPr('K', ExPr_Key);		// K：キー操作取得
	SetExPr('S', ExPr_Sound);	// S：サウンド用
	SetExPr('F', ExPr_File);	// F：ファイル用
	SetExPr('T', ExPr_Text);	// T：テキスト用
	// その他
	sat_def_fname = _sat_def_fname;
	box_title = _box_title;
}
// 全て初期化
void CSharpAt::Free()
{
	// INIT
	pos = at = st_pos = 0;
	fsize = 0;
	err = 0;
	memset(src, 0, sizeof(src));
	memset(mem, 0, sizeof(mem));
	memset(stack, 0, sizeof(stack));
	memset(proc, 0, sizeof(proc));
}

#ifdef _DXLIB
// ディレクトリを１つ戻す（'\\'の後をNULLにする）
//RET:戻した後の文字列の長さ
int GetBackDir(char *dir)
{
	int i;
	for(i=strlen(dir)-1; i>=1; i--)
	{
		// 一つ前は全角の後ろ？
		if(_mbsbtype((BYTE*)dir,i) == 2)
			i --;
		// セパレータ？
		if((dir[i-1]=='\\') || (dir[i-1]=='/'))
			break;
	}
	dir[i] = 0;
	return i;
}
#endif

// ソースファイルを読み込む
//RET: -1=ERR
int CSharpAt::LoadSrcFile(char *fName)
{
	Free();	// 初期化
#ifdef _DXLIB
	int sf = FileRead_open(fName);
	if( sf==0 )
	{
		MES_PUT("Error: Not file [ %s ]",fName);
		return -1;
	}
	// サイズオーバー？
	LONGLONG size = FileRead_size(fName);
	if(size > SAT_SRC_SIZE_MAX)
	{
		MES_PUT("Error: File size (%d / %d)",size,SAT_SRC_SIZE_MAX);
		return -1;
	}
	// 読み込む
	ERET( FileRead_read(src, (int)size, sf) );
	fsize = (int)size;
	// コメントや改行、Tabをスペースで埋める：ソース読み込み時に実行する関数
	SetCommentSpcAll();
	// スペースを全て詰める：但し、拡張関数に渡す用の文字列はチェックしない
	CtSpaceCutSrcAll();
	// フォルダ名とファイル名を抽出
	char dir[2000],fn[256];
	strcpy(dir,fName);
	int n = GetBackDir(dir);
	strcpy(fn,fName+n);
	// 現状を保存
	sprintf(dir+strlen(dir),"tmp_%s.txt",fn);
	{
		// ファイルを作成し書き込む
	//	ERET(sf.Open(dir,FNEW));
		void *m_fh = CreateFile(dir,GENERIC_WRITE,0,NULL,TRUNCATE_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
		if(m_fh == INVALID_HANDLE_VALUE)
			m_fh = CreateFile(dir,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_ARCHIVE,NULL);
		if(m_fh == INVALID_HANDLE_VALUE)	//ERR
			return -1;
	//	sf.Write(src,fsize);
		DWORD	siz;
		WriteFile(m_fh,src,fsize,&siz,NULL);
		CloseHandle(m_fh);
	}
#else
	// ファイルを開く
	CStdFile sf;
	if( sf.Open(fName, LOAD)==-1 )
	{
		MES_PUT("Error: Not file [ %s ]", fName);
		return -1;
	}
	// サイズオーバー？
	int size = sf.GetSize();
	if (size > SAT_SRC_SIZE_MAX)
	{
		MES_PUT("Error: File size (%d / %d)", size, SAT_SRC_SIZE_MAX);
		return -1;
	}
	// 読み込む
	ERET( sf.Read(src,size) );
	fsize = size;
	// コメントや改行、Tabをスペースで埋める：ソース読み込み時に実行する関数
	SetCommentSpcAll();
	// スペースを全て詰める：但し、拡張関数に渡す用の文字列はチェックしない
	CtSpaceCutSrcAll();
	// フォルダ名とファイル名を抽出
	char dir[2000], fn[256];
	strcpy(dir,fName);
	int n = GetBackDir(dir);
	strcpy(fn, fName+n);
	// 現状を保存
	sprintf(dir+strlen(dir),"tmp_%s.txt",fn);
	ERET( sf.Open(dir,FNEW) );
	sf.Write(src,fsize);
#endif
	return 0;
}

// 指定の値をスタックする
void CSharpAt::CtPush(int n)
{
	// これ以上はスタックできない？
	if( st_pos >= SAT_STACK_MAX )
	{
		MES_PUT("Error: Stack over");
		ERR_PUT(pos-1);
		return;
	}
	stack[st_pos++] = n;
}

// スタックから取り出す
int CSharpAt::CtPop()
{
	if(st_pos<=0)
	{
		MES_PUT("Error: Stack zero");
		ERR_PUT(pos - 1);
		return 0;
	}
	st_pos --;
	return stack[st_pos];	// ポップ後の値はクリアしないように
}

// コメントや改行、Tabをスペースで埋める：ソース読み込み時に実行する関数
void CSharpAt::SetCommentSpcAll()
{
	pos = 0;
	while (pos < fsize)
	{
		switch (src[pos])
		{
		case ' ': pos++; break;	// Space
		case 0x9: src[pos++] = ' '; break;	// TAB
		case 0x0D: src[pos++] = ' '; 	// 改行
		case 0x0A: src[pos++] = ' ';  break;	// 改行
		case '/':
			if (src[pos + 1] == '/')	// 以降コメント行？
			{
				src[pos++] = ' '; src[pos++] = ' ';
				// 改行までパスする
				while (1)
				{
					if (pos >= fsize) goto _END;	// 終了
					// 全角文字？
					if( IsZenkaku(src[pos]) )
					{
						src[pos++] = ' '; src[pos++] = ' ';
						continue;
					}
					// 改行チェック
					switch (src[pos])
					{
					case 0x0D: src[pos++] = ' ';
						if(src[pos] != 0x0A )
							goto _JMP;	// 改行
					case 0x0A: src[pos++] = ' ';
						goto _JMP;	// 改行
					}
					// 半角文字を消す
					src[pos++] = ' ';
				}
_JMP:
				break;
			}
		default:
			pos ++;
			break;
		}
	}
_END:
	pos = 0;	// INIT
}

// スペースを全て詰める：但し、拡張関数に渡す用の文字列はチェックしない
//※コメントや改行を無くした状態で実行すること
void CSharpAt::CtSpaceCutSrcAll()
{
	pos = 0;
	while (pos < fsize)
	{
		// 拡張関数？
		char c = src[pos];
		if ((c >= 'A') && (c <= 'Z'))
		{
			// ;まで飛ばす
			if( CtSrcPass(';')==0 )
				break;
			c = src[pos];
		}
		// スペース？
		if( c==' ' )
		{
			int bk = pos;
			// スペースの終りまで進む
			if( CtNextSrc()==0 )
			{
				fsize -= pos - bk;
				break;
			}
			// スペースを詰める
			memcpy(&src[bk], &src[pos], fsize - pos);
			fsize -= pos-bk;	// サイズを減らす
			pos = bk;	// 戻す
			continue;
		}
		// NEXT
		pos ++;
	}
	src[pos] = 0;
	pos = 0;	// INIT
}

//---------------------------------------
// 関係ない文字をパス
//RET: 0=終了
int CSharpAt::CtNextSrc()
{
	while (pos < fsize)
	{
		switch(src[pos])
		{
		case ' ': pos++; break;	// Space
		case ';': pos++; break;	// ;
		default:
			return 1;	// NEXT
		}
	}
	return 0;	// 終了
}

// 文字列部分をパス
//RET: 0=終了
int CSharpAt::CtSrcStrPass()
{
	while (1)
	{
		if (pos >= fsize) return 0;	// 終了
		// 全角文字？
		if (IsZenkaku(src[pos]))
		{
			pos += 2;
			continue;
		}
		// 文字列終了？
		if (src[pos] == '\"')
		{
			pos++;
			break;
		}
		pos++;
	}
	return 1;
}

// 指定の文字まで次までパスする
//RET: 0=終了
int CSharpAt::CtSrcPass(char c)
{
	while (pos < fsize)
	{
		// 文字列内ならパス
		if (src[pos] == '\"')
		{
			pos++;
			if (CtSrcStrPass() == 0)
				return 0;	// 終了
		}
		// 全角？
		if (IsZenkaku(src[pos]))
		{
			pos += 2;
			continue;
		}
		if(src[pos++]==c)
			return 1;	// NEXT
	}
	return 0;	// 終了
}

// 指定1の文字まで次までパスする。但し、指定2の文字があった場合は、指定1の検出数を増やす
//※例えば、[・・[・・]・・]で、対応する]まで進みたい時用
//RET: 0=終了
int CSharpAt::CtSrcPassEx(char c1,char c2)
{
	int cnt=0;
	while (pos < fsize)
	{
		// 文字列内ならパス
		if (src[pos] == '\"')
		{
			pos++;
			if (CtSrcStrPass() == 0) return 0;	// 終了
		}
		char wk = src[pos++];
		if( wk == c1 )
		{
			if( cnt<=0 )
				return 1;	// NEXT
			cnt --;
		}else
		{
			if( wk == c2 )
				cnt ++;
			else
				// 全角？
				if(IsZenkaku(wk))
					pos += 2;
		}
	}
	return 0;	// 終了
}

// 文字列内に、変数用の数字（0～999）があれば取得し、無ければ0を設定
//RET: 進んだ数
int CSharpAt::GetMemNum(char *s,int &n)
{
	int ret=0;
	n = 0;
	if ((*s >= '0') && (*s <= '9'))
	{
		// 文字列から値を取得
		ret = GetNumStr(s,n);
		// 変数番号の範囲外？
		if ((n<0) || (n>999))
		{
			MES_PUT("Error: Mem number [ %d ]", n);
			n = 0;	// 0にして続ける
		}
	}
	return ret;
}

// 変数かどうか調べ、変数なら変数番号とその中身を取得
//IN&OUT : 変数番号取得用, 変数内の値を取得用
//RET: 0以外なら取得成功
int CSharpAt::GetMem(int &pn,int &n)
{
	pn = n = 0;	// INIT
	char c = src[pos];
	// 変数（a～z）以外？
	if( ! ((c>='a') && (c<='z')) )
		return 0;	// 変数以外だった
	pos ++;
	// 変数配列？
	char c2 = src[pos];
	int num = 0;
	if( c2=='(' )
	{
		pos ++;
		c2 = src[pos];
		if (!((c2 >= 'a') && (c2 <= 'z')))
		{
			ERR_PUT(pos - 1);
			return 0;
		}
		pos ++;
		int wk = GetMemNum(&src[pos],num);
		pos += wk;
		num = mem[(int)(c2 - 'a')*SAT_MEM1_MAX + num];
	}else
	{
		//----------------------------------
		// 数字？
		if ((c2 >= '0') && (c2 <= '9'))
		{
			// 次の数値を取得（数値でない場合は、0を返す）
			GetNum(num);
			// 変数番号の範囲外？
			if( (num<0) || (num>999) )
			{
				MES_PUT("Error: Mem number [ %d ]", num);
				ERR_PUT(pos - 1);
				return 0;
			}
		}
	}
	// 値を設定
	pn = (int)(c-'a')*SAT_MEM1_MAX + num;
	n = mem[pn];
	return 1;	// 変数だった
}

// 次の数値を取得（数値でない場合は、0を返す）
int CSharpAt::GetNum(int &n)
{
	int ret = GetNumStr(&src[pos], n);
	pos += ret;
	return ret;
}
// 文字列から、数値を取得（数値がない場合は、0を返す）
//RET: 進んだ数 
int CSharpAt::GetNumStr(char *s, int &n)
{
	int i=0;
	n = 0;
	// sscanfを使わずに高速取得
	char c = s[i];
	// マイナス以外？
	if (c != '-')
	{
		// 数字以外？
		if (!((c >= '0') && (c <= '9')))
			return 0;
	}
	// 数字を取得
	char buf[16] = { c,0 };
	for (i = 1; i<11; i++)
	{
		c = s[i];
		if (!((c >= '0') && (c <= '9')))
			break;
		buf[i] = c;
	}
	buf[i] = 0;
	// 数値へ変換
	n = atoi(buf);
	return i;
}

// ポインタなら位置とその中身の値を取得（ポインタでなければ0を返す）
//OUT: pn=ポインタ位置を取得、n=中身の値を取得
int CSharpAt::GetPtNum(int &pn,int &n)
{
	pn = at;
	n = 0;
	if( src[pos] != '@' )
		return 0;	// ポインタ以外
	// (がある？
	pos++;
	if( src[pos]=='(' )
	{
		pos++;
		// 数値を取得
		int n2;
		// 次の数値を取得（数値でない場合は、0を返す）
		if (GetNum(n2)==0)
		{
			// 変数かどうか調べ、変数なら変数番号とその中身を取得
			int pn2;
			if (GetMem(pn2, n2)==0)
			{
				ERR_PUT(pos - 2);
				return 0;
			}
		}
		// @ポインタの位置が不正？
		pn = n2 + at;
		if (IsAtOver(pn))
		{
			ERR_PUT(pos - 2);
			return 0;
		}
	}
	// @ポインタの中身を取得
	n = mem[pn];
	return 1;
}

// 変数か数字かポインタなら、その値をnに設定。それら以外なら0を返す
//RET: 0以外なら取得成功
int CSharpAt::GetNumEx(int &n)
{
	int pn;
	// ポインタならその中身の値を取得（ポインタでなければ0を返す）
	if( GetPtNum(pn,n) )
		return 1;
	// 次の数値を取得（数値でない場合は、0を返す）
	if( GetNum(n) )
		return 1;
	// 変数かどうか調べ、変数なら変数番号とその中身を取得
	if( GetMem(pn, n) )
		return 1;
	return 0;
}

// 指定の文字列から、GetNumEx を行う
//RET: 取得した文字数（0=エラー）
int CSharpAt::GetNumExStr(char *str, int &n)
{
	int len = strlen(str);
	if( len==0 ) return 0;
	// クラスの値を保持
	DWORD	bk_pos = pos;
	DWORD	bk_fsize = fsize;
	char	*bk_src = src;
	// 新しい値を設定
	pos = 0;
	fsize = len;
	src = str;
	// 実行
	int ret = GetNumEx(n);
	if( ret )
		ret = pos;
	// クラスの値を戻す
	pos = bk_pos;
	fsize = bk_fsize;
	src = bk_src;
	return ret;
}

// カンマ区切りの数値（変数やポインタもOK）を指定数取得する
//IN : 取得する文字列, 取得したint型を入れる配列, 取得数, 取得完了位置を取得（0なら取得しない）
//RET: 取得した数（0=失敗）
int CSharpAt::GetCommaNum(char *str, int *p_n, int num, char **s_end)
{
	int cnt=0;	// 取得数
	int len = strlen(str);
	if (len == 0) return 0;
	// クラスの値を保持
	DWORD	bk_pos = pos;
	DWORD	bk_fsize = fsize;
	char	*bk_src = src;
	// 新しい値を設定
	pos = 0;
	fsize = len;
	src = str;

	// 取得処理
	for(int i=0; i<num; i++)
	{
		if( GetNumEx(p_n[cnt])==0 )	// 失敗？
			break;
		cnt++;
		if( src[pos]!=',' )
			break;
		pos ++;
	}
	if( s_end )
		*s_end = &src[pos];
	// クラスの値を戻す
	pos = bk_pos;
	fsize = bk_fsize;
	src = bk_src;

	return cnt;
}

// 文字列を指定文字まで取得（SAT_STR_MAXまで取得可能）
//RET: 取得した文字数
int CSharpAt::GetStr(char *str, char chk_c)
{
	int n = GetStrEx(str, &src[pos], chk_c);
	pos += n;
	return n;
}

// 文字列を指定文字まで取得（SAT_STR_MAXまで取得可能）
//IN : 格納バッファ、取り出す側、チェックする文字
//RET: 取得した文字数
int CSharpAt::GetStrEx(char *buf, char *moto, char chk_c)
{
	int n = 0;
	bool is_str_pass = false; // 文字列内パス用
	while (moto[n])
	{
		if (n >= (SAT_STR_MAX - 1))
			break;
		char c = moto[n];
		// 全角？
		if (IsZenkaku(c))
		{
			*buf++ = moto[n++];
			*buf++ = moto[n++];
			continue;
		}
		if (chk_c == ';')
		{
			if (c == '\"')
			{
				is_str_pass = !is_str_pass;
				*buf++ = c;
				n++;
				continue;
			}
		}

		// 終了？
		if (!is_str_pass && c == chk_c)
		{
			n++;
			break;
		}
		// 半角文字
		*buf++ = c;
		n++;
	}
	*buf = 0;	// NULL
	return n;
}

// （汎用）指定文字か終わりまでの文字数を取得
size_t CSharpAt::Util_GetStrLen(const char* str, char chk_c)
{
	size_t n = 0;
	while (str[n])
	{
		char c = str[n];
		// 全角？
		if (IsZenkaku(c))
		{
			n += 2;
			continue;
		}
		// 終了？
		if (c == chk_c)
		{
			break;
		}
		// 半角文字
		n++;
	}
	return n;
}
// （汎用）指定文字列か終わりまでの文字数を取得
size_t CSharpAt::Util_GetStrLen(const char* str, const char* chk)
{
	size_t n = 0;
	int chk_len = strlen(chk);
	while (str[n])
	{
		// 終了？
		if (memcmp(str+n, chk, chk_len) == 0)
			break;
		// 全角？
		if (IsZenkaku(str[n]))
		{
			n += 2;
			continue;
		}
		// 半角文字
		n++;
	}
	return n;
}

// 拡張機能なら、拡張関数を実行する（0以外なら拡張機能を実行した）
//OUT: retに拡張関数からの戻り値が入る
int CSharpAt::PrEx(int &ret)
{
	ret = 0;
	char c = src[pos];
	if (! ((c >= 'A') && (c <= 'Z')) )// && (src[pos + 1] == '(')) )
		return 0;	// 拡張無し
	pos ++;
	// 文字列を指定文字まで取得
	GetStr(str_buf, ';');
	// 拡張関数を実行し、戻り値を変数に設定
	ret = p_ExProc[(BYTE)c - (BYTE)'A'](this, str_buf);
	return 1;	// 拡張関数を実行した
}

//---------------------------------------
// #@のメインループ内の処理：AI対戦等で、1ターン毎実行する時にも使える
//RET: 0なら終了
int CSharpAt::PrSharpAtMain()
{
	//-------------------------------------------------
	// コメントや改行等をパス
	if (CtNextSrc() == 0) return 0;
	//-------------------------------------------------
	// 拡張機能？
	int ret;
	if (PrEx(ret))
		return 1;
	//-------------------------------------------------
	int pn, n;	// 変数番号, 変数の中身
	char c = src[pos];
	// 関数系？
	if (c == '#')
	{
		pos++;
		// 関数番号を取得
		if (GetNumEx(n) == 0)
		{
			ERR_PUT(pos - 1);
			return 0;
		}
		// 関数の範囲外？
		if ((n<0) || (n >= SAT_PROC_MAX))
		{
			ERR_PUT(pos - 1);
			return 0;
		}
		// 関数の宣言？
		c = src[pos];
		if (c == '{')
		{
			pos++;
			// 関数位置保持
			proc[n] = pos;
			// 関数の終わりまでパス
			CtSrcPass('}');
		}
		else
		{	// 関数を実行
			// 現在の位置を保持
			CtPush(pos);
			// 関数へジャンプ
			pos = proc[n];
		}
		return 1;
	}
	// 関数から戻る？
	if (c == '}')
	{
		pos = CtPop();
		return 1;
	}
	// 関数からreturnする？
	if (c == '.')
	{
		pos++;
		// 進みながら']'があればpop、'['があればpushしていく。'}'で呼び出し前まで戻る
		while (pos < fsize)
		{
			if (IsZenkaku(src[pos]))	// 全角？
			{
				pos += 2;
				continue;
			}
			if (src[pos] == '}')
			{
				pos = CtPop();
				break;
			}
			if (src[pos] == ']')
				CtPop();	// popのみで値は捨てる
			else if (src[pos] == '[')
				CtPush(pos);
			pos++;
		}
		return 1;
	}
	//-------------------------------------------------
	// 分岐戻り位置保持
	if (c == '[')
	{
		pos++;
		CtPush(pos);
		return 1;
	}
	// 分岐の終わり？
	if (c == ']')
	{
		CtPop();	// popを捨てる
		pos++;
		// 次の文字が'['ならelseなので、対応する']'までパスする
		if (src[pos] == '[')
		{
			pos++;
			if (CtSrcPassEx(']', '[') == 0)	// [[]]の時にも正常に動作させる
				return 0;	// END
		}
		return 1;
	}
	//-------------------------------------------------
	// 左のオペランドを取得し、それに応じて処理を行う
	//-------------------------------------------------
	// @ポインタ？
	if (c == '@')
	{
		pos++;
		// ポインタの位置を変える？
		switch (src[pos])
		{
		case ':':	// ポインタに変数のアドレスを設定
			pos++;
			if (GetMem(pn, n))	// 変数？
			{
				at = pn;		// @をpn番号の変数にする 
			}
			else
			{
				ERR_PUT(pos - 1);
				return 0;
			}
			break;
		case '>':	// ポインタを右に移動
			pos++;
			if (GetNumEx(n))	// 値を取得できた？
			{
				at += n;
			}
			else
			{
				ERR_PUT(pos - 1);
				return 0;
			}
			break;
		case '<':	// ポインタを左に移動
			pos++;
			if (GetNumEx(n))	// 値を取得できた？
			{
				at -= n;
			}
			else
			{
				ERR_PUT(pos - 1);
				return 0;
			}
			break;
		case '(':	// ポインタの位置を変えながら・・・
			pos--;	// 戻す
					// ポインタの位置と中身を取得
			if (GetPtNum(pn, n) == 0)
				return 0;	// END
			goto _JMP_RT;
		default:
			// 演算子や分岐を調べる
			pn = at;
			n = mem[pn];
			goto _JMP_RT;
		}
		// @ポインタの位置が不正？
		if (IsAtOver(at))
		{
			ERR_PUT(pos - 1);
			return 0;
		}
		return 1;
	}
	// 変数かどうか調べ、変数なら変数番号とその中身を取得
	if (GetMem(pn, n))
	{
	_JMP_RT:
		// 変数から値を取得した
		// 次が演算子？
		c = src[pos];
		switch (c)
		{
			//--------------------------------------------------------------
			// 演算子
		case '=': case '+': case '-': case '*': case '/': case '%': case '<': case '>':
		{
			int n2 = 0;
			pos++;
			// 変数か数字かポインタなら、その値を取得
			if (GetNumEx(n2) == 0)
			{
				if (PrEx(n2) == 0)
				{
					ERR_PUT(pos - 1);
					return 0;
				}
			}
			// 演算子判定
			switch (c)
			{
			case '=':	// 代入
				mem[pn] = n2;	break;
			case '+':	// 足し算
				mem[pn] += n2;	break;
			case '-':	// 引き算
				mem[pn] -= n2;	break;
			case '*':	// 掛け算
				mem[pn] *= n2;	break;
			case '/':	// 割り算
				if (n2 == 0) break;
				mem[pn] /= n2;	break;
			case '%':	// 余り
				if (n2 == 0) break;
				mem[pn] = mem[pn] % n2;	break;
			case '<':	// 比較
				mem[pn] = (mem[pn] < n2); break;
			case '>':	// 比較
				mem[pn] = (mem[pn] > n2); break;
			}
			break;
		}
		//--------------------------------------------------------------
		// 先頭分岐（if文）
		case '[':
			// 値が0以外なら、そのまま進み、0なら']'まで進む
			if (n)
			{
				pos++;
				// この位置をスタック
				CtPush(pos);
			}
			else
			{
				// 指定の文字の次までパスする
				pos++;
				if (CtSrcPassEx(']', '[') == 0)	// [[]]の時にも正常に動作させる
					return 0;	// END
			}
			break;
			// 後尾分岐（for文）
		case ']':
			// 値が0なら、そのまま進み、0以外なら'['の次まで戻る
			if (n == 0)
			{
				CtPop();	// popを捨てる
				pos++;
				// 次の文字が'['ならelseなので、対応する']'までパスする
				if (src[pos] == '[')
				{
					pos++;
					if (CtSrcPassEx(']', '[') == 0)	// [[]]の時にも正常に動作させる
						return 0;	// END
				}
			}
			else
			{
				pos = CtPop();	// 戻る位置を設定
				st_pos++;	// もう一度スタックするので
			}
			break;
		}
	}
	else
	{
		ERR_PUT(pos - 1);
		return 0;
	}
	return 1;
}

//---------------------------------------
// #@のメインループ処理
//RET: -1=ERR
int CSharpAt::GoSharpAt()
{
	int cnt=0;
	// ファイルの終わりまでループ
	while( (pos<fsize) && (pos>=0) && (err==0) )
	{
		// #@メインループを１回分実行
		if( PrSharpAtMain()==0 )
			break;
		// 偶にSYSTEMへ返す
		cnt ++;
		if( cnt>100 )
		{
			cnt = 0;
			ERET( SysTask() );
		}
	}
	// 呼び出し側も終了させる？
	if( err==-1 ) return -1;
	return 0;
}


//=======================================
// DATA
//=======================================
CSharpAt *sat;			// #@制御クラス

#ifdef _DXLIB
// メッセージボックス
int MesBox(int no,char* s,char* title)
{
	// マウス表示
//	SetMouseDispFlag(true);
	MessageBox(GetMainWindowHandle(),s,title,MB_OK);
//	SetMouseDispFlag(false);
	return 0;
}
//=======================================
// WinMainから呼ばれる関数：#@を開始する
//=======================================
int StartSharpAt(char *p_fName)
{
	ClearDrawScreen();
	//---------------------------------------
	// 実行するファイル名を表示
	if( p_fName[0]==0 )
		p_fName = CSharpAt::sat_def_fname;
#ifndef _DXLIB
	MesBox(0, p_fName,CSharpAt::box_title);//"Eexecute a #@ program");
#endif
	//---------------------------------------
	// クラスを生成
	sat = new CSharpAt;
	// #@のファイルを読み込む
	ERET( sat->LoadSrcFile(p_fName) );
	//---------------------------------------
	// #@のメインループ処理
	ERET( sat->GoSharpAt() );

	//---------------------------------------
	// 全て解放
	delete sat;
	return 0;
}
#else
//=======================================
// ゲーム側から呼ばれる関数：#@を開始する
//=======================================
int GM::StartSharpAt(int)
{
#if 1 //ndef _DEBUG
//	int sz=sizeof(CSharpAt);	// クラスのサイズを調べる時用
	// 最初だけ説明を入れる
	static char flg=0;
	if( flg==0 )
//	if( GC::sv.pd.ev_flg.is_sharpat_help==0 )
	{
		flg = 1;
	//	GC::sv.pd.ev_flg.is_sharpat_help = 1;
		SE::Play(SE_STORY2);
		ERET( Eff::FadeOut(vram) );
		SE::Play(SE_EYE);
		ERET(PrMesWindDef(vram,str_sharpat_help));
	}
	SE::Play(SE_STORY2);
	ERET(Eff::FadeOut(vram));
#else
	ViewCls();
	ERET( PaintMain(1) );
#endif
	//---------------------------------------
	// 実行するファイルを表示
	char *p_fName = cmdLine;
//	if( (p_fName[0]==0) || (cmdLine==0) )
		p_fName = SetPath(CSharpAt::sat_def_fname);
//	MesBox(0, p_fName,"#@ Language");//CSharpAt::box_title);//"Eexecute a #@ program");
	{
		// ソース用のバッファ
		CBuf bf;
		ERET( bf.Create(SAT_SRC_SIZE_MAX) );
		char *str = (char*)bf.buf;
		// ソースを読み込む
		CStdFile sf;
		if( sf.Open(p_fName,LOAD)==-1 )
		{
_JMP:
			sprintf(util_buf,"FILE ERROR:\n%s",p_fName);
			MesBox(util_buf);
			return 0;
		}
		DWORD len = sf.GetSize();
		if( len>=SAT_SRC_SIZE_MAX )
			goto _JMP;
		sf.Read(str,len);
		sf.Close();
		str[len] = 0;
		// ダイアログを開く
		SE::Play(SE_OPEN);
		GM::ld.c_type = -1;
		GM::ld.sa_src_max = SAT_SRC_SIZE_MAX-20;
		GM::ld.p_str = p_fName;
		int NormalDlgOpen(void *_hInst,void *_hwnd,char *str);
		int ret = NormalDlgOpen(0,hwnd,str);
		if( ret==-2 )	// CANCEL?
			return 0;
		if(str[0]==0)	// 全て消してある？
		{
			//スペースだけいれておく
			strcpy(str," ");
		}
		// 保存
		if(sf.Open(p_fName,FNEW)==-1)
			goto _JMP;
		sf.Write(str,strlen(str));
		sf.Close();
		ERET( SysTask() );
	}

	//---------------------------------------
	// クラスを生成
	sat = new CSharpAt;
	// #@のファイルを読み込む
	ERET( sat->LoadSrcFile(p_fName) );
	//---------------------------------------
	// フレームレート設定
	WORD bk_tb_wait = tb_wait;
	tb_wait = (1000*100)/60;	//1フレームのWAIT: 50=20fps
	// #@のメインループ処理
	InitKeyFlag();
	ERET( sat->GoSharpAt() );
	tb_wait = bk_tb_wait;	// 戻す

	//---------------------------------------
	// 全て解放
	delete sat;
	return 0;
}
#endif

//=======================================
// 専用関数
//=======================================
// 未対応の文字列を表示用
void CSharpAt::NON_CODE_PUT(int _pos)
{
	if (_pos<0) _pos = 0;
	char buf[32];
	memcpy(buf, &src[_pos], 16);
	buf[16] = 0;
	MES_PUT("Non code (pos=%d): %s", _pos, buf);
	err = 1;	// 1=通常エラー
}
// エラー表示用：エラーの箇所から数文字分のプログラムを表示
//IN : エラー位置
void CSharpAt::ERR_PUT(int _pos)
{
	if( _pos<0 ) _pos = 0;
	char buf[32];
	memcpy(buf, &src[_pos], 16);
	buf[16] = 0;
	MES_PUT("Error(pos=%d): %s", _pos, buf);
	err = 1;	// 1=通常エラー
}
// メッセージ表示用：主にデバッグやエラー用
void CSharpAt::MES_PUT(char *mes, ...)
{
	char str[1024];
	va_list list;
	va_start(list, mes);
	vsprintf(str, mes, list);
	va_end(list);
	MesBox(0,str,box_title);
	InitKeyFlag();
}
// 1バイト目が全角文字なら1を返す
int CSharpAt::IsZenkaku(char c)
{
	BYTE b = (BYTE)c;
	if (b < 33)
	{
		if ((b == 10) || (b == 13) || (b == 9) || (b == 32))	// 改行やTab？（テキストファイル用）
			return 0;	// 半角
		return 1;		// 全角
	}
	if (b <= 126) return 0;		// ALF
	if (b < 161) return 1;		// 全角
	if (b <= 223) return 0;		// ALF
	return 1;	// 全角
}
