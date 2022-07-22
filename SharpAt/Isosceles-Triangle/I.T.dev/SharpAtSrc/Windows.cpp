// Windows.cpp

#include	"SharpAt.h"

// DATA
int fontH=0;			// �t�H���g�p
int GMX = 640;	// ���݂̉�ʃT�C�YX
int GMY = 480;	// ���݂̉�ʃT�C�YY
int grp[SAT_GRP_MAX];	// �摜�p
int se[SAT_SE_MAX];		// �T�E���h�p

char szExecDir[260];	// �t�@�C���p�X�p
int szExecDirLen = 0;

// #@�̎��s�֐�
int StartSharpAt(char *p_fName);
// �摜��S�ēǂݍ���
void LoadGrpAll();

// �t�H���g������쐬
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

// �t�@�C�������݂���H
bool IsFile(const char* fname)
{
	char fn[320];
	sprintf(fn, "%s%s", szExecDir, fname);
	auto fp = fopen(fn, "r");
	if (fp == nullptr) return false;
	fclose(fp);
	return true;
}

// �v���O�����J�n
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	SetUseDirectInputFlag(FALSE);	// �f�o�b�O���[�h�ŏd���Ȃ�̂��������p
	SetOutApplicationLogValidFlag(FALSE);	// LOG�t�@�C�����o�͂��Ȃ�
	// TITLE�ύX
	char title[] = "#@(SharpAt) Language";
	SetMainWindowText(title);
	// �p�X�擾
	{
		int len = GetModuleFileName(0, szExecDir, sizeof(szExecDir));
		szExecDirLen = GetBackDir(szExecDir);
	}
	//-----------------------------------------------------------------------
	// �f�o�b�O�t�@�C��������Ȃ�A�t���X�N���[���m�F���p�X
	if(!IsFile("debug.ini"))
	{
		// �t���X�N���[���H
		char *cmd = lpCmdLine;
		if( cmd[0]==0 )
			cmd = CSharpAt::sat_def_fname;
		sprintf(CSharpAt::str_buf, "Program file : %s\n\nFull screen mode?",cmd);
		if( MessageBox(GetMainWindowHandle(),CSharpAt::str_buf,title,MB_YESNO)!=IDYES)
		{
			ChangeWindowMode(TRUE);			// TRUE�ɂ���ƃE�C���h�E���[�h�ŋN��
		}
	}else
		ChangeWindowMode(TRUE);			// TRUE�ɂ���ƃE�C���h�E���[�h�ŋN��
	//-----------------------------------------------------------------------
	if(DxLib_Init() == -1)			// DX���C�u�������J�n���鏈���i�Q�[���p�̉�ʂ������j
		return -1;
	// �}�E�X�\��OFF
//	SetMouseDispFlag(false);

	// �t�H���g�쐬
	CreFontDef();

	// �摜��S�ēǂݍ���
	LoadGrpAll();
	// ���ʉ���ǂݍ���
	char buf[260];
	for(int i=0; i<SAT_SE_MAX; i++)
	{
		sprintf(buf,"%sdt/S%d.ogg", szExecDir, i);
		int sf = FileRead_open(buf);	// �t�@�C�������邩���ׂ�
		if(sf==0)
		{
			se[i] = -1;
			continue;
		}
		FileRead_close(sf);
		se[i] = LoadSoundMem(buf);
	}

	// �`�����w��
	SetDrawScreen(DX_SCREEN_BACK);

	// #@�̎��s�֐�
	StartSharpAt(lpCmdLine);

	// �쐬�����t�H���g�f�[�^���폜����
	FreeFontDef();

	DxLib_End();		// DX���C�u�������I������i�Q�[����ʂ����j
	return 0;			// �v���O�������I������
}
