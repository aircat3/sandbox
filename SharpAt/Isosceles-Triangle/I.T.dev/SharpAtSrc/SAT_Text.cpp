// SAT_Text.cpp
// #@�p�g���֐��FT�F�e�L�X�g�p
#include "SharpAt.h"
#include <vector>
#include <string>

/*

�E/dt/t??.dat ����̃e�L�X�g�f�[�^�ǂݍ��݂��\�B
�E�t�@�C���ԍ��� 0�`99 �܂ŁB
�E1�x�Ɏg����t�@�C����1�܂ŁB
�E�ʏ�̃������ł͂Ȃ��A��p��string�֕ێ��B

���g�p�၄
�@TL0;�@�c�@t0�ԃt�@�C�����J���F0�`99�܂�
�@TG0,x,y;�@�c�@0�s�ڂ�x,y�̈ʒu�ɕ\��
  TB0;�@�c�@��ƃo�b�t�@��string��p�̈��0�s�ڂ��R�s�[
  TBSa;�@�c�@��ƃo�b�t�@���̍ŏ��� $ �ʒu��string��p�̈��a�s�ڂ�}��
  TBIa;�@�c�@��ƃo�b�t�@���̍ŏ��� $ �ʒu�ɕϐ�a�̒l��}��
  TBGx,y;�@�c�@��ƃo�b�t�@�̕�������Ax,y�̈ʒu�ɕ\��
 */

// ��pstring
static std::vector<std::string> stringList;	// �ǂݍ��񂾑S������
static std::string stringBuf; // ��ƃo�b�t�@�p

// �t�@�C�����當�����ǂݍ���
static int LoadText(int n)
{
	// �t�@�C�����J��
	char fname[320];
	sprintf(fname, "%sdt/t%d.dat", szExecDir, n);
	FILE* fp = fopen(fname, "rb");
	if (fp == nullptr)
		return -1;
	fpos_t fsize = 0;
	long fsizeb = fseek(fp, 0, SEEK_END);
	fgetpos(fp, &fsize);
	fseek(fp, fsizeb, SEEK_SET);
	// �S�ǂݍ���
	fsize++;
	std::string buf((size_t)fsize, 0);
	char* str_all = (char*)buf.c_str();
	fread((void*)str_all, 1, (size_t)fsize, fp);
	// �������ĕێ�
	stringList.clear();
	{
		size_t n = 0, max = (size_t)fsize;
		while (n < max)
		{
			size_t len = CSharpAt::Util_GetStrLen(str_all + n, '\n');
			if (len == 0) break;
			str_all[n + len] = 0;
			if (str_all[n + len-1] == '\r')
				str_all[n + len-1] = 0;
			std::string str(len,0);
			str = str_all + n;
			stringList.push_back(str);
			n += len+1;
		}
	}
	// �I��
	fclose(fp);
	return 0;	// OK
}

// List���̕�������擾
static const char* GetString(int n)
{
	if (stringList.size() > (size_t)n)
		return stringList[n].c_str();
	return " ";
}

//=======================================
// �g���֐� T�F�e�L�X�g�p
//=======================================
int CSharpAt::ExPr_Text(CSharpAt *ps,char *s)
{
	char* get = 0;
	int p[8];	// �A�������l���擾�p
	int n;
	char c = *s ++;
	if (ps->GetNumExStr(s, n)==0)
	{
		// �o�b�t�@������H
		if (c == 'B')
		{
			c = *s++;
			// �\���H
			if (c == 'G')
			{
				if (ps->GetCommaNum(s, p, 2, &get) != 2)	// �J���}��؂�̐��l���擾
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				DrawString(p[0], p[1], stringBuf.c_str());
				return 0;
			}
			if (ps->GetNumExStr(s, n)) // OK?
			{
				switch (c)
				{
				case 'S': // ������
					{
						auto pos = Util_GetStrLen(stringBuf.c_str(), '$');
						if (pos >= stringBuf.length())
							return 0;
						stringBuf.erase(pos, 1);
						stringBuf.insert(pos, GetString(n));
					}
					return 0;
				case 'I': // ����
					{
						auto pos = Util_GetStrLen(stringBuf.c_str(), '$');
						if (pos >= stringBuf.length())
							return 0;
						stringBuf.erase(pos, 1);
						stringBuf.insert(pos, std::to_string(n));
					}
					return 0;
				}
			}
		}
		ps->ERR_PUT(ps->pos - 4);
		return 0;
	}
	switch (c)
	{
	case 'L':	// �ǂݍ���
		{
			// 0-99�܂�
			if (n >= 0 && n <= 99)
			{
				if (LoadText(n) == -1)
				{
					MES_PUT("Error: Text Open = %d", n);
					ps->err = 1;
				}
			}
			else
			{
				MES_PUT("Error: Text File No Over = %d", n);
				ps->err = 1;
			}
		}
		return 0;
	case 'G':	// �\��
		{
			// ���W�擾
			s+=2;
			if (ps->GetCommaNum(s, p, 2, &get) != 2)	// �J���}��؂�̐��l���擾
			{
				ps->ERR_PUT(ps->pos - 4);	// ERR
				return 0;
			}
			// �ʒu���ړ�		
			//s = get;
			// �쐬�����t�H���g�ŕ`��
			DrawString(p[0], p[1], GetString(n));
		}
		return 0;
	case 'B':	// �o�b�t�@�փx�[�X�������ݒ�
		stringBuf = GetString(n);
		return 0;
	default:
		// ������̂܂܏o��
		CSharpAt::MES_PUT(s);
		break;
	}
	return 0;
}
