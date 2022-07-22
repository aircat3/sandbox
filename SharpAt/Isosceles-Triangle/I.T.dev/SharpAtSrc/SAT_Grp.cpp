// SAT_Grp.cpp
// #@�p�g���֐��FG�F�摜�`��p
#include	"SharpAt.h"

/*

���g�p�၄�i���l�̂Ƃ���́A�S�ĕϐ���|�C���^���g����j
���N�����ɁAG0.png�`G999.png�������I�ɓǂݍ��ށB�i�摜��������΃p�X���Ă����j
GS640,480;	// ��ʃT�C�Y��ݒ�F�f�t�H���g��640x480

G0,100,200;	// G0.png��100,200�̈ʒu�ɕ`��Fx,y���W�͏ȗ���
GP;			// r,g,b�̐F�ŁAx,y�̈ʒu�ɓ_��`��i�A���t�@�l�Ή��j
GB;			// r,g,b�̐F�ŁAx,y�̈ʒu����Ax1,y1�̃T�C�Y�̎l�p��`��i�A���t�@�l�Ή��j
GM4;		// �`�掞�̍��W�ɁA�w��̐��l���|����BGM4�Ȃ�4�{�Ȃ̂ŁAG0,10,10;�̏ꍇ�Ax=40,y=40�ɕ`�悷�邱�ƂɂȂ�
GF;			// Flip���ߓ��̎��ۂɉ�ʂɕ\�����鏈���i��L�͑S�ė��o�b�t�@�ɕ`�悵�Ă���ׁAFlip���Ȃ��Ɖ�ʂɕ\������Ȃ��j
GA40;		// �ȍ~�̃A���t�@�l��ݒ�F40�Ȃ�40���A0�Ȃ�\������Ȃ�

GC;			// ��ʂ����œh��Ԃ��iGA�ɂ��A���t�@�l���L���j
GCr,g,b;	// ��ʂ��w���RGB�l�œh��Ԃ��iGA�ɂ��A���t�@�l���L���j

GW50;		// Flip�O�ɁA�E�G�C�g������悤�ɂ���F�l��ms�i1000��1�b�j���E�G�C�g�����̏ꍇ�A��60fps�ɂȂ��Ă���
��Shift�L�[�������Ă���ԁA�E�G�C�g��������ԂɂȂ�

GT10,20"�E�E�E";	// ""���̕������10,20�̈ʒu�ɕ`��i�����T�C�Y�̓f�t�H���g�j
GT10,20,a;			// a0�̒l��10,20�̈ʒu�ɕ`��i�����T�C�Y�̓f�t�H���g�j

*/

//=======================================
// ��`�n
//=======================================
#define SAT_GRP_MAX	1000	// �ǂݍ��߂閇���i0�`SAT_GRP_MAX-1.png�j
// �摜�^�C�v
enum
{
	GRPDT_NM=0,		// �ʏ�`��
	GRPDT_V,		// �㉺���]
	GRPDT_H,		// ���E���]
};

//=======================================
// Grp�p�N���X
//=======================================
class CGrp
{
public:
	int	bai;	// ���W�{��
	int gmx, gmy;	// ��ʃT�C�Y
	int wait;	// �~���b�P��
	char dtype;	// �`��^�C�v�FGRPDT_???
	float alpha;	// �A���t�@���F0-1.0f�F0�Ȃ�`�悵�Ȃ�

	CGrp();
	~CGrp(){;}	// �K�v�Ȃ�f�X�g���N�^�ŉ���������s��
};
// �R���X�g���N�^�ŉ摜��ǂݍ���
CGrp::CGrp()
{
	// DATA������
	bai = 1;
	dtype = GRPDT_NM;
	alpha = 100;
	gmx = GMX;
	gmy = GMY;
	wait = 0;
	// G0.png�`G999.png��ǂݍ��ށ@���Q�[���ł͓ǂݍ��ݍς݂Ȃ̂ŉ������Ȃ�
	;
}
static CGrp cgrp;	// Grp�p�摜�N���X

#ifdef _DXLIB
// �摜��S�ēǂݍ���
void LoadGrpAll()
{
	char buf[256];
	for(int i=0; i<SAT_GRP_MAX; i++)
	{
		sprintf(buf,"dt/G%d.png",i);
		int sf = FileRead_open(buf);	// �t�@�C�������邩���ׂ�
		if(sf==0)
		{
			grp[i] = -1;
			continue;
		}
		FileRead_close(sf);
		grp[i] = LoadGraph(buf);
	}
}
#endif

// �i�ėp�j������\��
void CSharpAt::DrawString(int x,int y,const char* str)
{
	if (!str) return;
	int sx = x * cgrp.bai;
	int sy = y * cgrp.bai;
	int incy = 18 * cgrp.bai;
	auto str_en = str + strlen(str);
	while (str < str_en)
	{
		// ���s�R�[�h�ŕ�����
		auto next = Util_GetStrLen(str, "\\n");
		// �쐬�����t�H���g�ŕ`��
		DrawNStringToHandle(sx, sy, str, next, GetColor(255, 255, 255), fontH);
		if (str[next] == 0)
			break;
		str += next+2;
		sy += incy;
	}
}

//=======================================
// �g���֐� G�F�摜�`��
//=======================================
int CSharpAt::ExPr_Grp(CSharpAt *ps,char *s)	// *s�ɂ́A�ŏ���G�ƍŌ��;��������Ԃ̕����񂪓����Ă���
{
	char *get=0;
	int n, x,y,x1,y1,r,g,b;
	int p[8];	// �A�������l���擾�p
	switch( *s )
	{
	case 'P':	// r,g,b�̐F�ŁAx,y�̈ʒu�ɓ_��`��i�A���t�@�l�Ή��j
		x = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX];
		y = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX];
		r = ps->mem[(int)('r' - 'a')*SAT_MEM1_MAX];
		g = ps->mem[(int)('g' - 'a')*SAT_MEM1_MAX];
		b = ps->mem[(int)('b' - 'a')*SAT_MEM1_MAX];
#ifdef _DXLIB
		DrawPixel(x,y,GetColor(r,g,b));
#else
		if( HitTest_fast((DWORD)x,(DWORD)y,vram) )
		{
			n = (x * cgrp.bai)+(y * cgrp.bai)*GMX;
			WORD *mp = &((WORD*)vram.map)[n];
			if(cgrp.alpha >= 1.0f)
				*mp = RGB555(r,g,b);
			else
				*mp = SpCol(*mp,(WORD)r,(WORD)g,(WORD)b,cgrp.alpha);
		}
#endif
		return 0;
	case 'B':	// r,g,b�̐F�ŁAx,y�̈ʒu����Ax1,y1�̃T�C�Y�̎l�p��`��i�A���t�@�l�Ή��j
		x = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX];
		y = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX];
		x1 = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX +1];
		y1 = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX +1];
		r = ps->mem[(int)('r' - 'a')*SAT_MEM1_MAX];
		g = ps->mem[(int)('g' - 'a')*SAT_MEM1_MAX];
		b = ps->mem[(int)('b' - 'a')*SAT_MEM1_MAX];
#ifdef _DXLIB
		DrawBox(x,y,x+x1-1,y+y1-1,GetColor(r,g,b),TRUE);
#else
		if(cgrp.alpha >= 1.0f)
			GRP_DrawBox16(vram,vram,x * cgrp.bai,y * cgrp.bai,x1,y1,RGB555(r,g,b));
		else
			GRP_DrawBox16SpCol(vram,vram,x * cgrp.bai,y * cgrp.bai,x1,y1,RGB555(r,g,b),cgrp.alpha);
#endif
		return 0;
	case 'F':	// Flip�i��ʂ��X�V�j
#ifdef _DXLIB
		if((cgrp.wait>0) && ((CheckHitKey(KEY_INPUT_LSHIFT))==0))
			Sleep(cgrp.wait);
		// ��ʂ��t���b�v
		ScreenFlip();
#else
		if((cgrp.wait>0) && ((wm.c_key&KEY_SFT)==0))
			Sleep(cgrp.wait);
		ERET( SysTask() );
		// ������擾
		GetGameKey();
		// �E�G�C�g�F�V�t�g�L�[�ō���������
		ERET(Eff::EffWait(wm.c_key));
		if( PaintMain(1)==-1 )
		{
			ps->err = -1;	// -1=�����I��������
			return -1;
		}
		// F8�L�[�ŉ�ʂ�BMP�ŕۑ�
		if (wm.f_key == KEY_F8)
		{
			wm.cGdi.SaveBmp16(SetPath("scr_shot.bmp", 1));	//�X�N���[���V���b�g
			InitKeyFlag();
		}
#endif
		break;
	case 'S':	// ��ʃT�C�Y�ύX
		s++;
#ifdef _DXLIB
		if(ps->GetCommaNum(s,p,2)!=2)	// �J���}��؂�̐��l���擾
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		// ��ʃT�C�Y�ύX
		if( SetGraphMode(p[0],p[1],32)==DX_CHANGESCREEN_OK )	// �����H
		{
			cgrp.gmx = GMX = p[0];
			cgrp.gmy = GMY = p[1];
			SetDrawScreen(DX_SCREEN_BACK);
			// �摜��S�ēǂݍ���
			LoadGrpAll();
			// �t�H���g�쐬
			void CreFontDef();
			CreFontDef();
		}
#endif
		break;
	case 'M':	// �`����W�̔{���ݒ�
		s++;
		if(ps->GetNumExStr(s, n)==0 )	// ���l��ϐ�����l���擾
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		cgrp.bai = n;
		break;
	case 'D':	// �`��^�C�v��ݒ�
		s++;
		if (ps->GetNumExStr(s, n) == 0)	// ���l��ϐ�����l���擾
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		cgrp.dtype = n;
		break;
	case 'A':	// �A���t�@�l��ݒ�
		s++;
		if (ps->GetNumExStr(s, n) == 0)	// ���l��ϐ�����l���擾
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		n = MinMax(n,0,100);
#ifdef _DXLIB
		if( n>=100 )
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		else
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,(n*255)/100);
#endif
		cgrp.alpha = (float)n / 100.0f;
		break;
	case 'W':	// �E�G�C�g�ݒ�
		s++;
		if (ps->GetNumExStr(s, n) == 0)	// ���l��ϐ�����l���擾
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		cgrp.wait = n;
		break;
	case 'C':	// ��ʂ����œh��Ԃ��iGA�ɂ��A���t�@�l���L���j
		{
			s++;
#ifdef _DXLIB
			DWORD cl = GetColor(0,0,0);
			if(*s!=0)
			{
				if(ps->GetCommaNum(s,p,3)!=3)	// �J���}��؂�̐��l���擾
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				cl = GetColor(p[0],p[1],p[2]);
			}
			DrawBox(0,0,cgrp.gmx,cgrp.gmy,cl,TRUE);
#else
			// �F�l������H
			WORD cl = 0;
			if( *s!=0 )
			{
				if(ps->GetCommaNum(s, p, 3)!=3 )	// �J���}��؂�̐��l���擾
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				cl = RGB555(p[0], p[1], p[2]);
			}
			// �A���t�@�����H
			if( cgrp.alpha>=1.0f )
				GRP_DrawBox16(vram,vram,0,0,cgrp.gmx,cgrp.gmy, cl);
			else
				GRP_DrawBox16SpCol(vram,0,0,cgrp.gmx,cgrp.gmy, cl, cgrp.alpha);
#endif
		}
		break;
	case 'T':	// �e�L�X�g
		s++;
		if (ps->GetCommaNum(s, p, 2, &get) != 2)	// �J���}��؂�̐��l���擾
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		// �ʒu���ړ�		
		s = get;
		{
#ifdef _DXLIB
			// ������H
			if(*s=='\"')
			{
				s++;
				// ������`��
				CSharpAt::GetStrEx(CSharpAt::str_buf2,s,'\"');
				// �쐬�����t�H���g�ŕ`��
				DrawString(p[0], p[1], CSharpAt::str_buf2);
			}
			else
			{
				// ���l��`��
				if(ps->GetNumExStr(s,n)==0)
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				itoa(n,CSharpAt::str_buf2,10);
				DrawString(p[0], p[1], CSharpAt::str_buf2);
			}
#else
			// ������H
			if( *s=='\"' )
			{
				s++;
				// ������`��
				CSharpAt::GetStrEx(CSharpAt::str_buf2,s,'\"');
				GM::DrKanji(vram,p[0] * cgrp.bai,p[1] * cgrp.bai, CSharpAt::str_buf2);
			}else
			{
				// ���l��`��
				if(ps->GetNumExStr(s,n)==0 )
				{
					ps->ERR_PUT(ps->pos - 4);	// ERR
					return 0;
				}
				DrawNum(vram,p[0] * cgrp.bai,p[1] * cgrp.bai,n);//,0,0,0,0, (float)grp.alpha / 100.0f);
			}
#endif
		}
		break;
	default:	// �摜�\��
		n = ps->GetCommaNum(s,p,3);
		if( n==1 )	// ���W���ȗ������H
		{
			p[1] = ps->mem[(int)('x' - 'a')*SAT_MEM1_MAX];
			p[2] = ps->mem[(int)('y' - 'a')*SAT_MEM1_MAX];
		}
		else if (n != 3)	// �J���}��؂�̐��l���擾
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		// �͈͊O�H
		if( (p[0]<0) || (p[0]>=SAT_GRP_MAX) )
		{
			ps->ERR_PUT(ps->pos - 4);	// ERR
			return 0;
		}
		{
#ifdef _DXLIB
			// �摜�ԍ����Q�[���p�ɕϊ�
			int gno = p[0];
			if(grp[gno]<=-1)	// ����ȏ�͊G������
				break;
			// �`��
			DrawGraph(p[1]*cgrp.bai,p[2]*cgrp.bai,grp[gno],TRUE);
#else
			// �摜�ԍ����Q�[���p�ɕϊ�
			int gno = p[0];
			if( gno>=Grp::sat.MaxHed() )	// ����ȏ�͊G������
				break;
			// �A���t�@�����H
			if (cgrp.alpha >= 1.0f)
				Grp::sat.HDraw(vram,vram,p[1]*cgrp.bai,p[2] * cgrp.bai, gno);
			else
				Grp::sat.HDrawSpCol(vram, vram, p[1] * cgrp.bai, p[2] * cgrp.bai, gno, cgrp.alpha);
#endif
		}
		break;
	}

	// $$ ������̂܂܏o��
//	CSharpAt::MES_PUT(s);
	return 0;
}

