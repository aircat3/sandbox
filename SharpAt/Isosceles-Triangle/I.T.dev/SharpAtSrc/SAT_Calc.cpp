// SAT_Calc.cpp
// #@�p�g���֐��FC�F������v�Z�p
#include "SharpAt.h"
#include <time.h>

/*

���g�p�၄
a=CR;	// int�^�̗�����Ԃ�
a=CC-10/a+b*((5+c)%8);	// �ʏ�̌v�Z�������s���A���ʂ�Ԃ�
a=CC(a>=10)&&((b+5)==0));	// �_�����Z���\
a=CHx,y,x2,y2,s1,s2:	// HitTest�p�Fx,y�̒l���A��`�ix2,y2,s1,s2�j���ɂ����1��Ԃ��A�����łȂ����0��Ԃ��Bs1,s2��x��y�̋�`�T�C�Y
h=CLx,y,x2,y2,n;	// LINE�ʒu���v�Z���鏈���Fx,y����x2,y2�܂ł̐���ŁAn�s�N�Z���i�񂾍��W���A�ϐ�x��y�ɐݒ肷��B�߂�l�́A�i�ޕ�����8�����ɕ��������ۂ̕����l
��cos�Asin�Asqrt���̌v�Z�́A�K�v�ɉ����Ēǉ����Ă���
��C�̌�ɕϐ��␔����������A�ʏ퉉�Z���s���F�Ⴆ�΁ACa=b*50+3; ��

*/

// �v�Z�p
#ifdef _DXLIB
const float PI = 3.14159265358979323846f;
const float PI_45 = 0.78539816339744827900f;
const float PI_90 = 1.57079632679489655800f;
const float PI_180 = PI;
const float PI_270 = PI+PI_90;
const float PI_360 = 6.28318530717958623200f;
#endif

// ���ʗp
enum
{
	CC_NUM,		// ���l

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

	CC_END,		// �f�[�^�I��
};
// �D�揇�ʃe�[�u���F�傫������D��
static char cct[]=
{
	0,	//CC_NUM,	// ���l

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

// �v�Z���̕����ۑ��p�f�[�^
typedef struct
{
	int type;	// �f�[�^�̎�ށFCC_???
	int pt;		// �l
} CC_DATA;
// �v�Z�p�f�[�^
#define CCMAX 256
static CC_DATA dt[CCMAX+1];	// CC_TBL�\���̂ŕێ�

// dt���P�l�߂�
//IN : dt�̈ʒu�A�l�߂鐔
static void DelDt(int n,int n2)
{
	for(int i=n; i<CCMAX; i++)
	{
		dt[i].type = dt[i+n2].type;
		dt[i].pt = dt[i+n2].pt;
		if( dt[i].type==CC_END ) break;
	}
}

// �i����q�jdt[]��()����D�悵�Čv�Z���Ă���
//IN : dt�̈ʒu
static int CalcDt(int n)
{
	// �D�揇�ʂ̍������ɏ�������B'('������΂��̒��ɓ���
	int yuu = cct[CC_KL];	// ���݌v�Z����D�揇��
	while(1)
	{
		for(int i=n; i<CCMAX; i++)
		{
_JMP_ST:
			if( dt[i].type==CC_END )	// �I��
			{
				// �v�Z�����H
				if( (i-1)==n )
					return dt[n].pt;
				break;
			}
			if(dt[i].type==CC_NUM )		// ���l�̓p�X
				continue;
			if( cct[dt[i].type]<yuu )	// ���s���Ȃ��H
				continue;
			if( dt[i].type==CC_KL )	// (�H
			{
				DelDt(i,1);	// dt���l�߂�
				// )�܂Ōv�Z
				CalcDt(i);
				continue;
			}
			if(dt[i].type==CC_KR)	// )�H
			{
				// �����v�Z���������H
				if( (i-1)==n )
				{
					DelDt(i,1);	// dt���l�߂�
					return dt[n].pt;
				}
				break;
			}
			// �v�Z���s��
			switch( dt[i].type )
			{
			case CC_KAKE:	// *
				dt[i-1].pt *= dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_WARU:	// /
				if(dt[i+1].pt==0 )
					dt[i-1].pt = 0;	// 0�ɂ��Ă���
				else
					dt[i-1].pt /= dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_AMARI:	// %
				dt[i-1].pt = dt[i-1].pt % dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_TASU:	// +
				dt[i-1].pt += dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_HIKU:	// -
				dt[i-1].pt -= dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_RB:		// <
				dt[i-1].pt = dt[i-1].pt < dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_LB:		// >
				dt[i-1].pt = dt[i-1].pt > dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_RBEQ:	// <=
				dt[i-1].pt = dt[i-1].pt <= dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_LBEQ:	// >=
				dt[i-1].pt = dt[i-1].pt >= dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_EQ:		// ==
				dt[i-1].pt = dt[i-1].pt == dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_NEQ:	// !=
				dt[i-1].pt = dt[i-1].pt != dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_AND:	// &&
				dt[i-1].pt = dt[i-1].pt && dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			case CC_OR:		// ||
				dt[i-1].pt = dt[i-1].pt || dt[i+1].pt;
				DelDt(i,2);	// dt���l�߂�
				goto _JMP_ST;
			}
		}
		yuu --;	// �D��x��������
		if( yuu<=0 ) break;
	}
	return dt[n].pt;
}

// �����񂩂�A�ʏ�̌v�Z���s��
int CalcStr(CSharpAt *psat, char *s)
{
	int n,wk;
	memset(dt,0,sizeof(dt));	// INIT
	//-----------------------------------------------
	// �����񂩂�A�����≉�Z�q�𕪂��Ď擾����
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
			// �ŏ��Ȃ�A������
			if(i==0) goto _JMP_NUM;
			dt[i].type = CC_TASU; s++; break;
		case '-':
			// �ŏ��Ȃ�A������
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
		default:	// ���l��ϐ�
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
	// END�ݒ�
	dt[i].type = CC_END; i++;
	//-----------------------------------------------
	// ()����D�悵�ď������Ă����F����q��
	return CalcDt(0);	// �v�Z���ʂ�Ԃ�
}

#ifdef _DXLIB
// ����(x,y,x2,y2)�ɂ���w��(len)�̈ʒu(get_x,get_y)�����߂�
void GetLinePos(int x,int y,int x2,int y2,int len,int &get_x,int &get_y)
{
	int wx2 = x2-x;
	int wy2 = y2-y;
	if(_PM(wx2) > _PM(wy2))	//�w���傫��
	{
		if(wx2 < 0)	len *= -1;	//�����C��
		get_x = len;
		if(wx2)
			get_y = (wy2*len)/wx2;
		else
			get_y = 0;
	}
	else					//�x���傫��
	{
		if(wy2 < 0)	len *= -1;	//�����C��
		get_y = len;
		if(wy2)
			get_x = (wx2*len)/wy2;
		else
			get_x = 0;
	}
	get_x += x;
	get_y += y;
}

// ���W�A�������߂�
float GetRadian(float x,float y,float x1,float y1)
{
	//--0���̕������獶���Ń��W�A���p�����߂�
	float tei= x1-x;		//���
	float naname= y1-y;		//�΂�
	int ch=0;
	//--�w���O�x�̂Ƃ�
	if(tei==0)
	{
		if(naname<0)
			return PI;
		else
			return 0;
	}
	//--�x���O�x�̂Ƃ�
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
		naname = naname * -1;	//�}�C�i�X�␳
		ch=1;
	}
	naname=(float)hypot(tei,naname);	//�������f�d�s
	naname=tei/naname;
	naname=acosf(naname);
	//	kakudo=(naname*180)/PI;
	if(ch==1)	//�x���}�C�i�X�̏ꍇ�͔��]
				//	kakudo = 360-kakudo;
		naname = PI*2-naname;
	naname -= PI/2;	//�␳
	if(naname<0)
		naname += PI*2;
	return naname;
}
#endif

// ���W�A������W�������擾
//RET: 0-8����
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
// �g���֐� C�F������v�Z�p
//=======================================
int CSharpAt::ExPr_Calc(CSharpAt *ps,char *s)
{
	int p[8];	// �A�������l���擾�p
	switch( *s )
	{
	case 'R':	// �����H
		{
			static char flg=0;
			if( flg==0 )
			{
				flg = 1;
				srand((unsigned)time(0));	// �ŏ�����������������
			}
			return rand();
		}
		break;
	case 'C':	// �ʏ�̌v�Z
		{
			s++;
			return CalcStr(ps,s);
		}
		break;
	case 'H':	// HitTest�F��`�������ׂ�
		{
			s++;
			if (ps->GetCommaNum(s, p, 6) != 6)	// �J���}��؂�̐��l���擾
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
	case 'L':	// Line�ʒu�v�Z
		{
			s++;
			if(ps->GetCommaNum(s,p,5) != 5)	// �J���}��؂�̐��l���擾
			{
				ps->ERR_PUT(ps->pos - 4);
				return 0;
			}
			int x=p[0],y=p[1],x2=p[2],y2=p[3];
			GetLinePos(x,y,x2,y2,p[4],ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX],ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX]);
			return _GetAngle2Hou(GetRadian((float)x,(float)y,(float)x2,(float)y2));	// �p�x�����߁A���������߂�
		}
		break;
	default:
		// ���Ή��̖���
		ps->NON_CODE_PUT(ps->pos - 4);
		break;
	}
	return 0;
}

