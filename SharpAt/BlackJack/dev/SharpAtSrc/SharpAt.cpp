// SharpAt.cpp
// #@�C���^�v���^�p�̊�b�v���O����
#include	"SharpAt.h"

//=======================================
// �Q�[���p
//=======================================
extern char *str_sharpat_help;	// �J�n���̐����p

//=======================================
// DATA
//=======================================
// �R�}���h���C���������ꍇ�͂��̃t�@�C�������s����
#ifdef _DEBUG
static char _sat_def_fname[] = "SharpAt_deb.txt";	// �f�t�H���g�̃t�@�C����
#else
static char _sat_def_fname[] = "SharpAt.txt";	// �f�t�H���g�̃t�@�C����
#endif
static char _box_title[] = "#@ Language";	// MES_PUT�̃^�C�g���o�[�̕�����i��{�j

//=======================================
// CLASS DATA
//=======================================
char CSharpAt::str_buf[SAT_STR_MAX + 16];	// ������擾�p�̃o�b�t�@�F��{���Ŏg���p
char CSharpAt::str_buf2[SAT_STR_MAX + 16];	// ������擾�p�̃o�b�t�@2�F��Ɋg�����Ŏg���p
char *CSharpAt::sat_def_fname = _sat_def_fname;	// �f�t�H���g�̃t�@�C������ݒ肵�Ă���
char *CSharpAt::box_title = _box_title;		// MES_PUT���̃^�C�g���o�[�̕������ݒ肵�Ă���

//=======================================
// #@����N���X�F�{��
//=======================================
// �R���X�g���N�^
CSharpAt::CSharpAt()
{
	src = _src; Free();
	// �����g���֐���ݒ�
	for(int i=0; i<26; i++)
	{
		p_ExProc[i] = ExPr_D;
	}
	SetExPr('C', ExPr_Calc);	// C�F������v�Z�p
	SetExPr('D', ExPr_DMes);	// D�F�f�o�b�O�p�̊g���֐�
	SetExPr('G', ExPr_Grp);		// G�F�摜�`��
	SetExPr('K', ExPr_Key);		// K�F�L�[����擾
	SetExPr('S', ExPr_Sound);	// S�F�T�E���h�p
	SetExPr('F', ExPr_File);	// F�F�t�@�C���p
	SetExPr('T', ExPr_Text);	// T�F�e�L�X�g�p
	// ���̑�
	sat_def_fname = _sat_def_fname;
	box_title = _box_title;
}
// �S�ď�����
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
// �f�B���N�g�����P�߂��i'\\'�̌��NULL�ɂ���j
//RET:�߂�����̕�����̒���
int GetBackDir(char *dir)
{
	int i;
	for(i=strlen(dir)-1; i>=1; i--)
	{
		// ��O�͑S�p�̌��H
		if(_mbsbtype((BYTE*)dir,i) == 2)
			i --;
		// �Z�p���[�^�H
		if((dir[i-1]=='\\') || (dir[i-1]=='/'))
			break;
	}
	dir[i] = 0;
	return i;
}
#endif

// �\�[�X�t�@�C����ǂݍ���
//RET: -1=ERR
int CSharpAt::LoadSrcFile(char *fName)
{
	Free();	// ������
#ifdef _DXLIB
	int sf = FileRead_open(fName);
	if( sf==0 )
	{
		MES_PUT("Error: Not file [ %s ]",fName);
		return -1;
	}
	// �T�C�Y�I�[�o�[�H
	LONGLONG size = FileRead_size(fName);
	if(size > SAT_SRC_SIZE_MAX)
	{
		MES_PUT("Error: File size (%d / %d)",size,SAT_SRC_SIZE_MAX);
		return -1;
	}
	// �ǂݍ���
	ERET( FileRead_read(src, (int)size, sf) );
	fsize = (int)size;
	// �R�����g����s�ATab���X�y�[�X�Ŗ��߂�F�\�[�X�ǂݍ��ݎ��Ɏ��s����֐�
	SetCommentSpcAll();
	// �X�y�[�X��S�ċl�߂�F�A���A�g���֐��ɓn���p�̕�����̓`�F�b�N���Ȃ�
	CtSpaceCutSrcAll();
	// �t�H���_���ƃt�@�C�����𒊏o
	char dir[2000],fn[256];
	strcpy(dir,fName);
	int n = GetBackDir(dir);
	strcpy(fn,fName+n);
	// �����ۑ�
	sprintf(dir+strlen(dir),"tmp_%s.txt",fn);
	{
		// �t�@�C�����쐬����������
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
	// �t�@�C�����J��
	CStdFile sf;
	if( sf.Open(fName, LOAD)==-1 )
	{
		MES_PUT("Error: Not file [ %s ]", fName);
		return -1;
	}
	// �T�C�Y�I�[�o�[�H
	int size = sf.GetSize();
	if (size > SAT_SRC_SIZE_MAX)
	{
		MES_PUT("Error: File size (%d / %d)", size, SAT_SRC_SIZE_MAX);
		return -1;
	}
	// �ǂݍ���
	ERET( sf.Read(src,size) );
	fsize = size;
	// �R�����g����s�ATab���X�y�[�X�Ŗ��߂�F�\�[�X�ǂݍ��ݎ��Ɏ��s����֐�
	SetCommentSpcAll();
	// �X�y�[�X��S�ċl�߂�F�A���A�g���֐��ɓn���p�̕�����̓`�F�b�N���Ȃ�
	CtSpaceCutSrcAll();
	// �t�H���_���ƃt�@�C�����𒊏o
	char dir[2000], fn[256];
	strcpy(dir,fName);
	int n = GetBackDir(dir);
	strcpy(fn, fName+n);
	// �����ۑ�
	sprintf(dir+strlen(dir),"tmp_%s.txt",fn);
	ERET( sf.Open(dir,FNEW) );
	sf.Write(src,fsize);
#endif
	return 0;
}

// �w��̒l���X�^�b�N����
void CSharpAt::CtPush(int n)
{
	// ����ȏ�̓X�^�b�N�ł��Ȃ��H
	if( st_pos >= SAT_STACK_MAX )
	{
		MES_PUT("Error: Stack over");
		ERR_PUT(pos-1);
		return;
	}
	stack[st_pos++] = n;
}

// �X�^�b�N������o��
int CSharpAt::CtPop()
{
	if(st_pos<=0)
	{
		MES_PUT("Error: Stack zero");
		ERR_PUT(pos - 1);
		return 0;
	}
	st_pos --;
	return stack[st_pos];	// �|�b�v��̒l�̓N���A���Ȃ��悤��
}

// �R�����g����s�ATab���X�y�[�X�Ŗ��߂�F�\�[�X�ǂݍ��ݎ��Ɏ��s����֐�
void CSharpAt::SetCommentSpcAll()
{
	pos = 0;
	while (pos < fsize)
	{
		switch (src[pos])
		{
		case ' ': pos++; break;	// Space
		case 0x9: src[pos++] = ' '; break;	// TAB
		case 0x0D: src[pos++] = ' '; 	// ���s
		case 0x0A: src[pos++] = ' ';  break;	// ���s
		case '/':
			if (src[pos + 1] == '/')	// �ȍ~�R�����g�s�H
			{
				src[pos++] = ' '; src[pos++] = ' ';
				// ���s�܂Ńp�X����
				while (1)
				{
					if (pos >= fsize) goto _END;	// �I��
					// �S�p�����H
					if( IsZenkaku(src[pos]) )
					{
						src[pos++] = ' '; src[pos++] = ' ';
						continue;
					}
					// ���s�`�F�b�N
					switch (src[pos])
					{
					case 0x0D: src[pos++] = ' ';
						if(src[pos] != 0x0A )
							goto _JMP;	// ���s
					case 0x0A: src[pos++] = ' ';
						goto _JMP;	// ���s
					}
					// ���p����������
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

// �X�y�[�X��S�ċl�߂�F�A���A�g���֐��ɓn���p�̕�����̓`�F�b�N���Ȃ�
//���R�����g����s�𖳂�������ԂŎ��s���邱��
void CSharpAt::CtSpaceCutSrcAll()
{
	pos = 0;
	while (pos < fsize)
	{
		// �g���֐��H
		char c = src[pos];
		if ((c >= 'A') && (c <= 'Z'))
		{
			// ;�܂Ŕ�΂�
			if( CtSrcPass(';')==0 )
				break;
			c = src[pos];
		}
		// �X�y�[�X�H
		if( c==' ' )
		{
			int bk = pos;
			// �X�y�[�X�̏I��܂Ői��
			if( CtNextSrc()==0 )
			{
				fsize -= pos - bk;
				break;
			}
			// �X�y�[�X���l�߂�
			memcpy(&src[bk], &src[pos], fsize - pos);
			fsize -= pos-bk;	// �T�C�Y�����炷
			pos = bk;	// �߂�
			continue;
		}
		// NEXT
		pos ++;
	}
	src[pos] = 0;
	pos = 0;	// INIT
}

//---------------------------------------
// �֌W�Ȃ��������p�X
//RET: 0=�I��
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
	return 0;	// �I��
}

// �����񕔕����p�X
//RET: 0=�I��
int CSharpAt::CtSrcStrPass()
{
	while (1)
	{
		if (pos >= fsize) return 0;	// �I��
		// �S�p�����H
		if (IsZenkaku(src[pos]))
		{
			pos += 2;
			continue;
		}
		// ������I���H
		if (src[pos] == '\"')
		{
			pos++;
			break;
		}
		pos++;
	}
	return 1;
}

// �w��̕����܂Ŏ��܂Ńp�X����
//RET: 0=�I��
int CSharpAt::CtSrcPass(char c)
{
	while (pos < fsize)
	{
		// ��������Ȃ�p�X
		if (src[pos] == '\"')
		{
			pos++;
			if (CtSrcStrPass() == 0)
				return 0;	// �I��
		}
		// �S�p�H
		if (IsZenkaku(src[pos]))
		{
			pos += 2;
			continue;
		}
		if(src[pos++]==c)
			return 1;	// NEXT
	}
	return 0;	// �I��
}

// �w��1�̕����܂Ŏ��܂Ńp�X����B�A���A�w��2�̕������������ꍇ�́A�w��1�̌��o���𑝂₷
//���Ⴆ�΁A[�E�E[�E�E]�E�E]�ŁA�Ή�����]�܂Ői�݂������p
//RET: 0=�I��
int CSharpAt::CtSrcPassEx(char c1,char c2)
{
	int cnt=0;
	while (pos < fsize)
	{
		// ��������Ȃ�p�X
		if (src[pos] == '\"')
		{
			pos++;
			if (CtSrcStrPass() == 0) return 0;	// �I��
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
				// �S�p�H
				if(IsZenkaku(wk))
					pos += 2;
		}
	}
	return 0;	// �I��
}

// ��������ɁA�ϐ��p�̐����i0�`999�j������Ύ擾���A�������0��ݒ�
//RET: �i�񂾐�
int CSharpAt::GetMemNum(char *s,int &n)
{
	int ret=0;
	n = 0;
	if ((*s >= '0') && (*s <= '9'))
	{
		// �����񂩂�l���擾
		ret = GetNumStr(s,n);
		// �ϐ��ԍ��͈̔͊O�H
		if ((n<0) || (n>999))
		{
			MES_PUT("Error: Mem number [ %d ]", n);
			n = 0;	// 0�ɂ��đ�����
		}
	}
	return ret;
}

// �ϐ����ǂ������ׁA�ϐ��Ȃ�ϐ��ԍ��Ƃ��̒��g���擾
//IN&OUT : �ϐ��ԍ��擾�p, �ϐ����̒l���擾�p
//RET: 0�ȊO�Ȃ�擾����
int CSharpAt::GetMem(int &pn,int &n)
{
	pn = n = 0;	// INIT
	char c = src[pos];
	// �ϐ��ia�`z�j�ȊO�H
	if( ! ((c>='a') && (c<='z')) )
		return 0;	// �ϐ��ȊO������
	pos ++;
	// �ϐ��z��H
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
		// �����H
		if ((c2 >= '0') && (c2 <= '9'))
		{
			// ���̐��l���擾�i���l�łȂ��ꍇ�́A0��Ԃ��j
			GetNum(num);
			// �ϐ��ԍ��͈̔͊O�H
			if( (num<0) || (num>999) )
			{
				MES_PUT("Error: Mem number [ %d ]", num);
				ERR_PUT(pos - 1);
				return 0;
			}
		}
	}
	// �l��ݒ�
	pn = (int)(c-'a')*SAT_MEM1_MAX + num;
	n = mem[pn];
	return 1;	// �ϐ�������
}

// ���̐��l���擾�i���l�łȂ��ꍇ�́A0��Ԃ��j
int CSharpAt::GetNum(int &n)
{
	int ret = GetNumStr(&src[pos], n);
	pos += ret;
	return ret;
}
// �����񂩂�A���l���擾�i���l���Ȃ��ꍇ�́A0��Ԃ��j
//RET: �i�񂾐� 
int CSharpAt::GetNumStr(char *s, int &n)
{
	int i=0;
	n = 0;
	// sscanf���g�킸�ɍ����擾
	char c = s[i];
	// �}�C�i�X�ȊO�H
	if (c != '-')
	{
		// �����ȊO�H
		if (!((c >= '0') && (c <= '9')))
			return 0;
	}
	// �������擾
	char buf[16] = { c,0 };
	for (i = 1; i<11; i++)
	{
		c = s[i];
		if (!((c >= '0') && (c <= '9')))
			break;
		buf[i] = c;
	}
	buf[i] = 0;
	// ���l�֕ϊ�
	n = atoi(buf);
	return i;
}

// �|�C���^�Ȃ�ʒu�Ƃ��̒��g�̒l���擾�i�|�C���^�łȂ����0��Ԃ��j
//OUT: pn=�|�C���^�ʒu���擾�An=���g�̒l���擾
int CSharpAt::GetPtNum(int &pn,int &n)
{
	pn = at;
	n = 0;
	if( src[pos] != '@' )
		return 0;	// �|�C���^�ȊO
	// (������H
	pos++;
	if( src[pos]=='(' )
	{
		pos++;
		// ���l���擾
		int n2;
		// ���̐��l���擾�i���l�łȂ��ꍇ�́A0��Ԃ��j
		if (GetNum(n2)==0)
		{
			// �ϐ����ǂ������ׁA�ϐ��Ȃ�ϐ��ԍ��Ƃ��̒��g���擾
			int pn2;
			if (GetMem(pn2, n2)==0)
			{
				ERR_PUT(pos - 2);
				return 0;
			}
		}
		// @�|�C���^�̈ʒu���s���H
		pn = n2 + at;
		if (IsAtOver(pn))
		{
			ERR_PUT(pos - 2);
			return 0;
		}
	}
	// @�|�C���^�̒��g���擾
	n = mem[pn];
	return 1;
}

// �ϐ����������|�C���^�Ȃ�A���̒l��n�ɐݒ�B�����ȊO�Ȃ�0��Ԃ�
//RET: 0�ȊO�Ȃ�擾����
int CSharpAt::GetNumEx(int &n)
{
	int pn;
	// �|�C���^�Ȃ炻�̒��g�̒l���擾�i�|�C���^�łȂ����0��Ԃ��j
	if( GetPtNum(pn,n) )
		return 1;
	// ���̐��l���擾�i���l�łȂ��ꍇ�́A0��Ԃ��j
	if( GetNum(n) )
		return 1;
	// �ϐ����ǂ������ׁA�ϐ��Ȃ�ϐ��ԍ��Ƃ��̒��g���擾
	if( GetMem(pn, n) )
		return 1;
	return 0;
}

// �w��̕����񂩂�AGetNumEx ���s��
//RET: �擾�����������i0=�G���[�j
int CSharpAt::GetNumExStr(char *str, int &n)
{
	int len = strlen(str);
	if( len==0 ) return 0;
	// �N���X�̒l��ێ�
	DWORD	bk_pos = pos;
	DWORD	bk_fsize = fsize;
	char	*bk_src = src;
	// �V�����l��ݒ�
	pos = 0;
	fsize = len;
	src = str;
	// ���s
	int ret = GetNumEx(n);
	if( ret )
		ret = pos;
	// �N���X�̒l��߂�
	pos = bk_pos;
	fsize = bk_fsize;
	src = bk_src;
	return ret;
}

// �J���}��؂�̐��l�i�ϐ���|�C���^��OK�j���w�萔�擾����
//IN : �擾���镶����, �擾����int�^������z��, �擾��, �擾�����ʒu���擾�i0�Ȃ�擾���Ȃ��j
//RET: �擾�������i0=���s�j
int CSharpAt::GetCommaNum(char *str, int *p_n, int num, char **s_end)
{
	int cnt=0;	// �擾��
	int len = strlen(str);
	if (len == 0) return 0;
	// �N���X�̒l��ێ�
	DWORD	bk_pos = pos;
	DWORD	bk_fsize = fsize;
	char	*bk_src = src;
	// �V�����l��ݒ�
	pos = 0;
	fsize = len;
	src = str;

	// �擾����
	for(int i=0; i<num; i++)
	{
		if( GetNumEx(p_n[cnt])==0 )	// ���s�H
			break;
		cnt++;
		if( src[pos]!=',' )
			break;
		pos ++;
	}
	if( s_end )
		*s_end = &src[pos];
	// �N���X�̒l��߂�
	pos = bk_pos;
	fsize = bk_fsize;
	src = bk_src;

	return cnt;
}

// ��������w�蕶���܂Ŏ擾�iSAT_STR_MAX�܂Ŏ擾�\�j
//RET: �擾����������
int CSharpAt::GetStr(char *str, char chk_c)
{
	int n = GetStrEx(str, &src[pos], chk_c);
	pos += n;
	return n;
}

// ��������w�蕶���܂Ŏ擾�iSAT_STR_MAX�܂Ŏ擾�\�j
//IN : �i�[�o�b�t�@�A���o�����A�`�F�b�N���镶��
//RET: �擾����������
int CSharpAt::GetStrEx(char *buf, char *moto, char chk_c)
{
	int n = 0;
	bool is_str_pass = false; // ��������p�X�p
	while (moto[n])
	{
		if (n >= (SAT_STR_MAX - 1))
			break;
		char c = moto[n];
		// �S�p�H
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

		// �I���H
		if (!is_str_pass && c == chk_c)
		{
			n++;
			break;
		}
		// ���p����
		*buf++ = c;
		n++;
	}
	*buf = 0;	// NULL
	return n;
}

// �i�ėp�j�w�蕶�����I���܂ł̕��������擾
size_t CSharpAt::Util_GetStrLen(const char* str, char chk_c)
{
	size_t n = 0;
	while (str[n])
	{
		char c = str[n];
		// �S�p�H
		if (IsZenkaku(c))
		{
			n += 2;
			continue;
		}
		// �I���H
		if (c == chk_c)
		{
			break;
		}
		// ���p����
		n++;
	}
	return n;
}
// �i�ėp�j�w�蕶���񂩏I���܂ł̕��������擾
size_t CSharpAt::Util_GetStrLen(const char* str, const char* chk)
{
	size_t n = 0;
	int chk_len = strlen(chk);
	while (str[n])
	{
		// �I���H
		if (memcmp(str+n, chk, chk_len) == 0)
			break;
		// �S�p�H
		if (IsZenkaku(str[n]))
		{
			n += 2;
			continue;
		}
		// ���p����
		n++;
	}
	return n;
}

// �g���@�\�Ȃ�A�g���֐������s����i0�ȊO�Ȃ�g���@�\�����s�����j
//OUT: ret�Ɋg���֐�����̖߂�l������
int CSharpAt::PrEx(int &ret)
{
	ret = 0;
	char c = src[pos];
	if (! ((c >= 'A') && (c <= 'Z')) )// && (src[pos + 1] == '(')) )
		return 0;	// �g������
	pos ++;
	// ��������w�蕶���܂Ŏ擾
	GetStr(str_buf, ';');
	// �g���֐������s���A�߂�l��ϐ��ɐݒ�
	ret = p_ExProc[(BYTE)c - (BYTE)'A'](this, str_buf);
	return 1;	// �g���֐������s����
}

//---------------------------------------
// #@�̃��C�����[�v���̏����FAI�ΐ퓙�ŁA1�^�[�������s���鎞�ɂ��g����
//RET: 0�Ȃ�I��
int CSharpAt::PrSharpAtMain()
{
	//-------------------------------------------------
	// �R�����g����s�����p�X
	if (CtNextSrc() == 0) return 0;
	//-------------------------------------------------
	// �g���@�\�H
	int ret;
	if (PrEx(ret))
		return 1;
	//-------------------------------------------------
	int pn, n;	// �ϐ��ԍ�, �ϐ��̒��g
	char c = src[pos];
	// �֐��n�H
	if (c == '#')
	{
		pos++;
		// �֐��ԍ����擾
		if (GetNumEx(n) == 0)
		{
			ERR_PUT(pos - 1);
			return 0;
		}
		// �֐��͈̔͊O�H
		if ((n<0) || (n >= SAT_PROC_MAX))
		{
			ERR_PUT(pos - 1);
			return 0;
		}
		// �֐��̐錾�H
		c = src[pos];
		if (c == '{')
		{
			pos++;
			// �֐��ʒu�ێ�
			proc[n] = pos;
			// �֐��̏I���܂Ńp�X
			CtSrcPass('}');
		}
		else
		{	// �֐������s
			// ���݂̈ʒu��ێ�
			CtPush(pos);
			// �֐��փW�����v
			pos = proc[n];
		}
		return 1;
	}
	// �֐�����߂�H
	if (c == '}')
	{
		pos = CtPop();
		return 1;
	}
	// �֐�����return����H
	if (c == '.')
	{
		pos++;
		// �i�݂Ȃ���']'�������pop�A'['�������push���Ă����B'}'�ŌĂяo���O�܂Ŗ߂�
		while (pos < fsize)
		{
			if (IsZenkaku(src[pos]))	// �S�p�H
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
				CtPop();	// pop�݂̂Œl�͎̂Ă�
			else if (src[pos] == '[')
				CtPush(pos);
			pos++;
		}
		return 1;
	}
	//-------------------------------------------------
	// ����߂�ʒu�ێ�
	if (c == '[')
	{
		pos++;
		CtPush(pos);
		return 1;
	}
	// ����̏I���H
	if (c == ']')
	{
		CtPop();	// pop���̂Ă�
		pos++;
		// ���̕�����'['�Ȃ�else�Ȃ̂ŁA�Ή�����']'�܂Ńp�X����
		if (src[pos] == '[')
		{
			pos++;
			if (CtSrcPassEx(']', '[') == 0)	// [[]]�̎��ɂ�����ɓ��삳����
				return 0;	// END
		}
		return 1;
	}
	//-------------------------------------------------
	// ���̃I�y�����h���擾���A����ɉ����ď������s��
	//-------------------------------------------------
	// @�|�C���^�H
	if (c == '@')
	{
		pos++;
		// �|�C���^�̈ʒu��ς���H
		switch (src[pos])
		{
		case ':':	// �|�C���^�ɕϐ��̃A�h���X��ݒ�
			pos++;
			if (GetMem(pn, n))	// �ϐ��H
			{
				at = pn;		// @��pn�ԍ��̕ϐ��ɂ��� 
			}
			else
			{
				ERR_PUT(pos - 1);
				return 0;
			}
			break;
		case '>':	// �|�C���^���E�Ɉړ�
			pos++;
			if (GetNumEx(n))	// �l���擾�ł����H
			{
				at += n;
			}
			else
			{
				ERR_PUT(pos - 1);
				return 0;
			}
			break;
		case '<':	// �|�C���^�����Ɉړ�
			pos++;
			if (GetNumEx(n))	// �l���擾�ł����H
			{
				at -= n;
			}
			else
			{
				ERR_PUT(pos - 1);
				return 0;
			}
			break;
		case '(':	// �|�C���^�̈ʒu��ς��Ȃ���E�E�E
			pos--;	// �߂�
					// �|�C���^�̈ʒu�ƒ��g���擾
			if (GetPtNum(pn, n) == 0)
				return 0;	// END
			goto _JMP_RT;
		default:
			// ���Z�q�╪��𒲂ׂ�
			pn = at;
			n = mem[pn];
			goto _JMP_RT;
		}
		// @�|�C���^�̈ʒu���s���H
		if (IsAtOver(at))
		{
			ERR_PUT(pos - 1);
			return 0;
		}
		return 1;
	}
	// �ϐ����ǂ������ׁA�ϐ��Ȃ�ϐ��ԍ��Ƃ��̒��g���擾
	if (GetMem(pn, n))
	{
	_JMP_RT:
		// �ϐ�����l���擾����
		// �������Z�q�H
		c = src[pos];
		switch (c)
		{
			//--------------------------------------------------------------
			// ���Z�q
		case '=': case '+': case '-': case '*': case '/': case '%': case '<': case '>':
		{
			int n2 = 0;
			pos++;
			// �ϐ����������|�C���^�Ȃ�A���̒l���擾
			if (GetNumEx(n2) == 0)
			{
				if (PrEx(n2) == 0)
				{
					ERR_PUT(pos - 1);
					return 0;
				}
			}
			// ���Z�q����
			switch (c)
			{
			case '=':	// ���
				mem[pn] = n2;	break;
			case '+':	// �����Z
				mem[pn] += n2;	break;
			case '-':	// �����Z
				mem[pn] -= n2;	break;
			case '*':	// �|���Z
				mem[pn] *= n2;	break;
			case '/':	// ����Z
				if (n2 == 0) break;
				mem[pn] /= n2;	break;
			case '%':	// �]��
				if (n2 == 0) break;
				mem[pn] = mem[pn] % n2;	break;
			case '<':	// ��r
				mem[pn] = (mem[pn] < n2); break;
			case '>':	// ��r
				mem[pn] = (mem[pn] > n2); break;
			}
			break;
		}
		//--------------------------------------------------------------
		// �擪����iif���j
		case '[':
			// �l��0�ȊO�Ȃ�A���̂܂ܐi�݁A0�Ȃ�']'�܂Ői��
			if (n)
			{
				pos++;
				// ���̈ʒu���X�^�b�N
				CtPush(pos);
			}
			else
			{
				// �w��̕����̎��܂Ńp�X����
				pos++;
				if (CtSrcPassEx(']', '[') == 0)	// [[]]�̎��ɂ�����ɓ��삳����
					return 0;	// END
			}
			break;
			// �������ifor���j
		case ']':
			// �l��0�Ȃ�A���̂܂ܐi�݁A0�ȊO�Ȃ�'['�̎��܂Ŗ߂�
			if (n == 0)
			{
				CtPop();	// pop���̂Ă�
				pos++;
				// ���̕�����'['�Ȃ�else�Ȃ̂ŁA�Ή�����']'�܂Ńp�X����
				if (src[pos] == '[')
				{
					pos++;
					if (CtSrcPassEx(']', '[') == 0)	// [[]]�̎��ɂ�����ɓ��삳����
						return 0;	// END
				}
			}
			else
			{
				pos = CtPop();	// �߂�ʒu��ݒ�
				st_pos++;	// ������x�X�^�b�N����̂�
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
// #@�̃��C�����[�v����
//RET: -1=ERR
int CSharpAt::GoSharpAt()
{
	int cnt=0;
	// �t�@�C���̏I���܂Ń��[�v
	while( (pos<fsize) && (pos>=0) && (err==0) )
	{
		// #@���C�����[�v���P�񕪎��s
		if( PrSharpAtMain()==0 )
			break;
		// ���SYSTEM�֕Ԃ�
		cnt ++;
		if( cnt>100 )
		{
			cnt = 0;
			ERET( SysTask() );
		}
	}
	// �Ăяo�������I��������H
	if( err==-1 ) return -1;
	return 0;
}


//=======================================
// DATA
//=======================================
CSharpAt *sat;			// #@����N���X

#ifdef _DXLIB
// ���b�Z�[�W�{�b�N�X
int MesBox(int no,char* s,char* title)
{
	// �}�E�X�\��
//	SetMouseDispFlag(true);
	MessageBox(GetMainWindowHandle(),s,title,MB_OK);
//	SetMouseDispFlag(false);
	return 0;
}
//=======================================
// WinMain����Ă΂��֐��F#@���J�n����
//=======================================
int StartSharpAt(char *p_fName)
{
	ClearDrawScreen();
	//---------------------------------------
	// ���s����t�@�C������\��
	if( p_fName[0]==0 )
		p_fName = CSharpAt::sat_def_fname;
#ifndef _DXLIB
	MesBox(0, p_fName,CSharpAt::box_title);//"Eexecute a #@ program");
#endif
	//---------------------------------------
	// �N���X�𐶐�
	sat = new CSharpAt;
	// #@�̃t�@�C����ǂݍ���
	ERET( sat->LoadSrcFile(p_fName) );
	//---------------------------------------
	// #@�̃��C�����[�v����
	ERET( sat->GoSharpAt() );

	//---------------------------------------
	// �S�ĉ��
	delete sat;
	return 0;
}
#else
//=======================================
// �Q�[��������Ă΂��֐��F#@���J�n����
//=======================================
int GM::StartSharpAt(int)
{
#if 1 //ndef _DEBUG
//	int sz=sizeof(CSharpAt);	// �N���X�̃T�C�Y�𒲂ׂ鎞�p
	// �ŏ���������������
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
	// ���s����t�@�C����\��
	char *p_fName = cmdLine;
//	if( (p_fName[0]==0) || (cmdLine==0) )
		p_fName = SetPath(CSharpAt::sat_def_fname);
//	MesBox(0, p_fName,"#@ Language");//CSharpAt::box_title);//"Eexecute a #@ program");
	{
		// �\�[�X�p�̃o�b�t�@
		CBuf bf;
		ERET( bf.Create(SAT_SRC_SIZE_MAX) );
		char *str = (char*)bf.buf;
		// �\�[�X��ǂݍ���
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
		// �_�C�A���O���J��
		SE::Play(SE_OPEN);
		GM::ld.c_type = -1;
		GM::ld.sa_src_max = SAT_SRC_SIZE_MAX-20;
		GM::ld.p_str = p_fName;
		int NormalDlgOpen(void *_hInst,void *_hwnd,char *str);
		int ret = NormalDlgOpen(0,hwnd,str);
		if( ret==-2 )	// CANCEL?
			return 0;
		if(str[0]==0)	// �S�ď����Ă���H
		{
			//�X�y�[�X��������Ă���
			strcpy(str," ");
		}
		// �ۑ�
		if(sf.Open(p_fName,FNEW)==-1)
			goto _JMP;
		sf.Write(str,strlen(str));
		sf.Close();
		ERET( SysTask() );
	}

	//---------------------------------------
	// �N���X�𐶐�
	sat = new CSharpAt;
	// #@�̃t�@�C����ǂݍ���
	ERET( sat->LoadSrcFile(p_fName) );
	//---------------------------------------
	// �t���[�����[�g�ݒ�
	WORD bk_tb_wait = tb_wait;
	tb_wait = (1000*100)/60;	//1�t���[����WAIT: 50=20fps
	// #@�̃��C�����[�v����
	InitKeyFlag();
	ERET( sat->GoSharpAt() );
	tb_wait = bk_tb_wait;	// �߂�

	//---------------------------------------
	// �S�ĉ��
	delete sat;
	return 0;
}
#endif

//=======================================
// ��p�֐�
//=======================================
// ���Ή��̕������\���p
void CSharpAt::NON_CODE_PUT(int _pos)
{
	if (_pos<0) _pos = 0;
	char buf[32];
	memcpy(buf, &src[_pos], 16);
	buf[16] = 0;
	MES_PUT("Non code (pos=%d): %s", _pos, buf);
	err = 1;	// 1=�ʏ�G���[
}
// �G���[�\���p�F�G���[�̉ӏ����琔�������̃v���O������\��
//IN : �G���[�ʒu
void CSharpAt::ERR_PUT(int _pos)
{
	if( _pos<0 ) _pos = 0;
	char buf[32];
	memcpy(buf, &src[_pos], 16);
	buf[16] = 0;
	MES_PUT("Error(pos=%d): %s", _pos, buf);
	err = 1;	// 1=�ʏ�G���[
}
// ���b�Z�[�W�\���p�F��Ƀf�o�b�O��G���[�p
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
// 1�o�C�g�ڂ��S�p�����Ȃ�1��Ԃ�
int CSharpAt::IsZenkaku(char c)
{
	BYTE b = (BYTE)c;
	if (b < 33)
	{
		if ((b == 10) || (b == 13) || (b == 9) || (b == 32))	// ���s��Tab�H�i�e�L�X�g�t�@�C���p�j
			return 0;	// ���p
		return 1;		// �S�p
	}
	if (b <= 126) return 0;		// ALF
	if (b < 161) return 1;		// �S�p
	if (b <= 223) return 0;		// ALF
	return 1;	// �S�p
}
