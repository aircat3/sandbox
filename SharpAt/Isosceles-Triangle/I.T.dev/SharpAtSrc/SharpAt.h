#pragma once
/*=============================================================

 SharpAt.h
 #@�iSharpAt�j�C���^�v���^�p�̋��ʃw�b�_

 Version : 1.05

 ���o�[�W����������
 1.01 : �h��class�Ŋg���֐���ǉ����Ղ��悤�ɁAp_ExProc�����N���X���ɓ���ASetExPr() �ŐV�����g���֐���ݒ�ł���悤�ɕύX
 1.02 : GP;��GB;��ǉ��B�܂��AGn,x,y; �� Gn; �Əȗ��\�ɕύX
 1.03 : �`�掞�Acgrp.bai���g���Ă��Ȃ��������̂��C���Acos��sin�v�Z��ǉ�
 1.04 : �ŐV��Ԃł̃����N�G���[�C���BGT�̃e�L�X�g�\�����C��
 1.05 : GT��[];�Ȃǂ̕\���ɑΉ�
 1.06 : F�ł̃t�@�C������ɑΉ�
 1.07 : F�ł̃t�@�C���ʒu��ύX�AT�n�̃e�L�X�g�֐��Q��ǉ�

//=============================================================*/

#ifndef GIWLIB
#define _DXLIB	// �L���ɂ���ƁADX���C�u�����ŕ`�悷��
#endif

#define WIN32_LEAN_AND_MEAN
#include	<windows.h>
#include	<stdarg.h>		// ... ���g���ׂ̃w�b�_

#ifdef _DXLIB
//--------------------------------------
#pragma warning( disable: 4996 )      // sprintf�����g����悤�ɂ���
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<time.h>
#include	"DxLib.h"	// DX���C�u����

// �G���[�`�F�b�N�p
#define ERET(p) {if(p==-1) return -1;}

// ��Βl
inline int _PM(int n)
{
	if(n<0) n *= -1;
	return n;
}
// dt��͈͓��̒l�ɂ��A�߂�l�ŕԂ�
inline int MinMax(int dt,int min,int max)
{
	if(dt<min)	return min;
	if(dt>max)	return max;
	return dt;
}
// DXLib�ł͉������Ȃ�
inline void InitKeyFlag(){;}
// SYSTEM�ɕԂ�����
inline int SysTask(){if(ProcessMessage()==0) return 0; return -1;}

#else
//--------------------------------------
#include	"main.h"

#endif


//=======================================
// �萔
//=======================================
class CSharpAt;
typedef int(*SHARP_AT_PROC)(CSharpAt*,char*);	// �g���֐����Ăяo���֐��̃t�H�[�}�b�g

#define SAT_DEF_ALF_MAX 26		// a�`z
#define SAT_MEM1_MAX 1000		// 1�������̍ő吔
#define SAT_DEF_MEM_MAX (SAT_DEF_ALF_MAX*SAT_MEM1_MAX)	// ��{�������̃T�C�Y
#define SAT_SRC_SIZE_MAX (1024*1024)	// �\�[�X�t�@�C���̍ő�T�C�Y
#define SAT_STACK_MAX (1024*1024/4)		// �X�^�b�N�T�C�Y
#define SAT_PROC_MAX 1000		// �֐��̈ʒu�ێ��p
#define SAT_STR_MAX 1024		// �֐����̕�����擾�p�T�C�Y
#define SAT_GRP_MAX 1000	// �ő�摜��
#define SAT_SE_MAX 1000		// �ő���ʉ���

// @�|�C���^�̕s���͈́H
inline bool IsAtOver(int at) {
	return ((at < 0) || (at >= SAT_DEF_MEM_MAX));
}

//=======================================
// #@����N���X�F��`
//=======================================
class CSharpAt
{
public:
	// DATA
	int		pos;					// ���݂̃\�[�X�̈ʒu
	int		at;						// @�|�C���^�̌��ݒn
	int		fsize;					// �ǂݍ��񂾃\�[�X�̃t�@�C���T�C�Y
	int		st_pos;					// �X�^�b�N�ʒu
	char	err;					// 0�ȊO�Ȃ�G���[�����������F1�Ȃ�v���O�����̂ݏI���B-1�Ȃ�Ăяo�����̃v���O�������I��������K�v������
	char	*src;					// �\�[�X����p
	char	_src[SAT_SRC_SIZE_MAX + 16];	// �����Ƀ\�[�X��ǂݍ��ށF1MB����ɐ�������i�����]���Ɋm�ہj
	int		mem[SAT_DEF_MEM_MAX + 16];	// �ʏ탁����[a�`z(26) * 0�`999(1000)]�i�����]���Ɋm�ہj
	int		stack[SAT_STACK_MAX];	// �֐���J��Ԃ��p�̃X�^�b�N[SAT_STACK_MAX]
	int		proc[SAT_PROC_MAX];		// �֐��̈ʒu��ێ�[SAT_PROC_MAX]�Fproc[0]�ɂ́A#0�֐��̊J�n�ʒu�A{�̎��̈ʒu������
	SHARP_AT_PROC p_ExProc[26];		// �g���֐��p
	static char *sat_def_fname;		// �f�t�H���g�̃t�@�C������ݒ肵�Ă���
	static char	*box_title;			// MES_PUT���̃^�C�g���o�[�̕������ݒ肵�Ă���

	// �R���X�g���N�^
	CSharpAt();
	// �f�X�g���N�^
	~CSharpAt() { Free(); }

	// �S�ď�����
	void Free();
	// �\�[�X�t�@�C����ǂݍ���
	//RET: -1=ERR
	int LoadSrcFile(char *fName);
	// �w��̒l���X�^�b�N����
	void CtPush(int n);
	// �X�^�b�N������o��
	int CtPop();
	// �R�����g����s�ATab���X�y�[�X�Ŗ��߂�F�\�[�X�ǂݍ��ݎ��Ɏ��s����֐�
	void SetCommentSpcAll();
	// �X�y�[�X��S�ċl�߂�F�A���A�g���֐��ɓn���p�̕�����̓`�F�b�N���Ȃ�
	//���R�����g����s�𖳂�������ԂŎ��s���邱��
	void CtSpaceCutSrcAll();
	// �֌W�Ȃ��������p�X
	//RET: 0=�I��
	int CtNextSrc();
	// �����񕔕����p�X
	//RET: 0=�I��
	int CtSrcStrPass();
	// �w��̕����܂Ŏ��܂Ńp�X����
	//RET: 0=�I��
	int CtSrcPass(char c);
	// �w��1�̕����܂Ŏ��܂Ńp�X����B�A���A�w��2�̕������������ꍇ�́A�w��1�̌��o���𑝂₷
	//���Ⴆ�΁A[�E�E[�E�E]�E�E]�ŁA�Ή�����]�܂Ői�݂������p
	//RET: 0=�I��
	int CtSrcPassEx(char c1=']', char c2='[');
	// �ϐ����ǂ������ׁA�ϐ��Ȃ�ϐ��ԍ��Ƃ��̒��g���擾
	//IN&OUT : �ϐ��ԍ��擾�p, �ϐ����̒l���擾�p
	//RET: 0�ȊO�Ȃ�擾����
	int GetMem(int &pn, int &n);
	// ���̐��l���擾�i���l�łȂ��ꍇ�́A0��Ԃ��j
	int GetNum(int &n);
	// �|�C���^�Ȃ炻�̈ʒu�ƒ��g�̒l���擾�i�|�C���^�łȂ����0��Ԃ��j
	//OUT: pn=�|�C���^�ʒu���擾�An=���g�̒l���擾
	int GetPtNum(int &pn,int &n);
	// �ϐ����������|�C���^�Ȃ�A���̒l��n�ɐݒ�
	//RET: 0�ȊO�Ȃ�擾����
	int GetNumEx(int &n);
	// �w��̕����񂩂�AGetNumEx ���s��
	//RET: �擾�����������i0=�G���[�j
	int GetNumExStr(char *str, int &n);
	// �J���}��؂�̐��l�i�ϐ���|�C���^��OK�j���w�萔�擾����
	//IN : �擾���镶����, �擾����int�^������z��, �擾��, �擾�����ʒu���擾�i0�Ȃ�擾���Ȃ��j
	//RET: �擾�������i0=���s�j
	int GetCommaNum(char *str, int *p_n, int num, char **s_end=0);
	// ��������w�蕶���܂Ŏ擾�iSAT_STR_MAX�܂Ŏ擾�\�j
	//RET: �擾����������
	int GetStr(char *str, char c = ';');
	// �i�ėp�j�w�蕶�����I���܂ł̕��������擾
	static size_t Util_GetStrLen(const char* str, char chk_c);
	// �i�ėp�j�w�蕶���񂩏I���܂ł̕��������擾
	static size_t Util_GetStrLen(const char* str, const char* chk);

	// �g���@�\�Ȃ�A�g���֐������s����i0�ȊO�Ȃ�g���@�\�����s�����j
	//OUT: ret�Ɋg���֐�����̖߂�l������
	int PrEx(int &ret);

	// �i�ėp�j������\��
	static void DrawString(int x, int y, const char* str);

	// @�|�C���^�𓥂܂����������ʒu���擾
	int* GetAt() {
		if (IsAtOver(at)) return mem;
		return &(mem[at]);
	}

	// ���Ή��̕������\���p
	void NON_CODE_PUT(int _pos);
	// �G���[�\���p�F�G���[�̉ӏ����琔�������̃v���O������\��
	//IN : �G���[�ʒu
	void ERR_PUT(int _pos);

	//=======================================
	// #@�̃��C�����[�v���̏����FAI�ΐ퓙�ŁA1�^�[�������s���鎞�ɂ��g����
	//RET: 0�Ȃ�I��
	int PrSharpAtMain();
	// #@�̃��C�����[�v����
	//RET: -1=ERR
	int GoSharpAt();

	//=======================================
	// �g���֐��ݒ�p
	//=======================================
	// �g���֐���ݒ肷��
	//IN : 'S'���̎��ʎq, �ݒ肷��g���֐�
	void SetExPr(char c,SHARP_AT_PROC p){p_ExProc[c-'A'] = p;}

	//=======================================
	// ��{�̊g���֐��Q
	//=======================================
	static int ExPr_D(CSharpAt *ps,char *s);	// ���Ή��R�[�h�ł��邱�Ƃ�\��
	static int ExPr_Calc(CSharpAt *ps,char *s);	// C�F������v�Z�p
	static int ExPr_DMes(CSharpAt *ps,char *s);	// D�F�f�o�b�O�\���p
	static int ExPr_Grp(CSharpAt *ps,char *s);	// G�F�摜�`��
	static int ExPr_Key(CSharpAt *ps,char *s);	// K�F�L�[����擾
	static int ExPr_Sound(CSharpAt *ps,char *s);// S�F�T�E���h�p
	static int ExPr_File(CSharpAt* ps, char* s);// F�F�t�@�C���p
	static int ExPr_Text(CSharpAt* ps, char* s);// T�F�e�L�X�g�p

	//=======================================
	// static�֐���DATA
	//=======================================
	// DATA
	static char str_buf[SAT_STR_MAX + 16];	// ������擾�p�̃o�b�t�@
	static char str_buf2[SAT_STR_MAX + 16];	// ������擾�p�̃o�b�t�@2
	// ���b�Z�[�W�\���p�F��Ƀf�o�b�O��G���[�p
	static void MES_PUT(char *mes, ...);
	// 1�o�C�g�ڂ��S�p�����Ȃ�1��Ԃ�
	static int IsZenkaku(char c);
	// ��������w�蕶���܂Ŏ擾�iSAT_STR_MAX�܂Ŏ擾�\�j
	//IN : �i�[�o�b�t�@�A���o�����A�`�F�b�N���镶��
	//RET: �擾����������
	static int GetStrEx(char *buf, char *moto, char chk_c);
	// ��������ɁA�ϐ��p�̐����i0�`999�j������Ύ擾���A�������0��ݒ�
	//RET: �i�񂾐�
	static int GetMemNum(char *s,int &n);
	// �����񂩂�A���l���擾�i���l���Ȃ��ꍇ�́A0��Ԃ��j
	//RET: �i�񂾐� 
	static int GetNumStr(char *s,int &n);


};	// class SharpAt


#ifdef _DXLIB
// �O���ϐ�
extern int fontH;				// �t�H���g�p
extern int GMX;// = 640;	// ���݂̉�ʃT�C�YX
extern int GMY;// = 480;	// ���݂̉�ʃT�C�YY
extern int grp[SAT_GRP_MAX];	// �摜�p
extern int se[SAT_SE_MAX];		// �T�E���h�p
#endif

extern char szExecDir[];	// �t�@�C���p�X�p
extern int szExecDirLen;

// �f�B���N�g�����P�߂��i'\\'�̌��NULL�ɂ���j
//RET:�߂�����̕�����̒���
int GetBackDir(char* dir);