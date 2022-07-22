// SAT_File.cpp
// #@�p�g���֐��FF�F�t�@�C���p
#include	"SharpAt.h"

/*

�E/dt/file??.dat �ւ̃t�@�C�����삪�\�B
�E�t�@�C���ԍ��� 0�`99 �܂ŁB
�E1�x�ɊJ����t�@�C����1�܂ŁB
���������͈͂𒴂��Ȃ��悤�ɂ��Ă���ׁA�s�����쓙�͂ł��Ȃ��悤�ɂ��Ă���܂��B

���g�p�၄
�@F0;�@�c�@0�ԃt�@�C�����J���F0�`99�܂�
�@FC;�@�c�@�J���Ă���t�@�C�������
�@FW50;�@�c�@@�|�C���^����50���̃��������J���Ă�t�@�C���ɏ�������
�@FR50;�@�c�@�J���Ă�t�@�C������50���̒l���A@�|�C���^�ɓǂݍ���
�@FS-10;�@�c�@�t�@�C���ʒu��10���߂�
�@FS10;�@�c�@�t�@�C���ʒu��10���i�߂�

 ��1����4�o�C�g

 */

// �t�@�C���p
static FILE* fID = nullptr;

// �t�@�C�����J��
static FILE* FileOpen(int n)
{
	if (fID)
	{
		fclose(fID);
		fID = nullptr;
	}
	char fname[320];
	sprintf(fname, "%sdt/file%d.dat", szExecDir, n);
	fID = fopen(fname, "wb+");
	return fID;
}

//=======================================
// �g���֐� F�F�t�@�C���p
//=======================================
int CSharpAt::ExPr_File(CSharpAt *ps,char *s)
{
	// ���l�Ȃ�t�@�C�����J��
	int n;
	if (ps->GetNumExStr(s, n))
	{
		// 0-99�܂�
		if (n >= 0 && n <= 99)
		{
			if (FileOpen(n) == nullptr)
			{
				MES_PUT("Error: File Open = %d", n);
				ps->err = 1;
			}
		}
		else
		{
			MES_PUT("Error: File No Over = %d", n);
			ps->err = 1;
		}
		return 0;
	}
	// ���̑��𒲂ׂ�
	char c = *s ++;
	if (ps->GetNumExStr(s, n)==0)
	{
		if (c == 'C')	// Close
		{
			if (fID)
				fclose(fID);
			fID = nullptr;
			return 0;
		}
		ps->ERR_PUT(ps->pos - 4);
		return 0;
	}
	switch (c)
	{
	case 'W':	// ��������
		{
			// �͈͒����H
			if (IsAtOver(ps->at + n))
			{
				MES_PUT("Error: File Write Over = %d", n);
				ps->err = 1;
				return 0;
			}
			int* p = ps->GetAt();
			fwrite(p, sizeof(int), n, fID);
		}
		break;
	case 'R':	// �ǂݍ���
		{
			// �͈͒����H
			if (IsAtOver(ps->at + n))
			{
				MES_PUT("Error: File Read Over = %d", n);
				ps->err = 1;
				return 0;
			}
			int* p = ps->GetAt();
			fread(p, sizeof(int), n, fID);
		}
		break;
	case 'S':	// �V�[�N
		fseek(fID, n * sizeof(int), SEEK_CUR);
		break;
	default:
		// ������̂܂܏o��
		CSharpAt::MES_PUT(s);
		break;
	}
	return 0;
}
