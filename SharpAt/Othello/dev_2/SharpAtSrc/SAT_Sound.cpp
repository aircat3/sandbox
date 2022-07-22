// SAT_Sound.cpp
// #@�p�g���֐��FS�F�T�E���h�p
#include	"SharpAt.h"

/*

���g�p�၄
���N�����ɁAS0.ogg�`S99.ogg�̉��f�[�^��ǂݍ��ށB�i�t�@�C����������΃p�X���Ă����j
S0;		// S0.ogg��1��Đ�
SS0;	// S0.ogg�̍Đ����~
SL0;	// S0.ogg�����[�v�Đ�
SM0;	// �i�ȗp�jM0.ogg�����[�v�Đ��B�ȗp�̃t�@�C���͎��s���ꂽ���ɓǂݍ���
SMS;	// �Ȃ̍Đ����~

*/

//=======================================
// �g���֐� S�F�T�E���h�p
//=======================================
int CSharpAt::ExPr_Sound(CSharpAt *ps,char *s)
{
	// ���l�Ȃ���ʉ�
	int n;
	if (ps->GetNumExStr(s, n) )
	{
		// ���ʉ����Đ�
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
	// ���̑��𒲂ׂ�
	char c = *s ++;
	if (ps->GetNumExStr(s, n)==0)
	{
		ps->ERR_PUT(ps->pos - 4);
		ps->err = 1;	// 1=�ʏ�G���[
		return 0;
	}
	switch (c)
	{
	case 'M':	// BGM�n
#ifdef _DXLIB
		if(n<=-1)	// ��~�H
			StopMusic();
		else
		{
			char buf[256];
			sprintf(buf,"dt/M%d.mp3",n);
			PlayMusic(buf,DX_PLAYTYPE_LOOP);
		}
#else
		if(n<=-1)	// ��~�H
			BGM::Stop();
		else
			BGM::Play(n);
#endif
		break;
	case 'S':	// �T�E���h��~
#ifdef _DXLIB
		if((n>=0) && (n<SAT_SE_MAX))
			StopSoundMem(se[n]);
#else
		SE::Stop(n);
#endif
		break;
	case 'L':	// ���ʉ������[�v�Đ�
#ifdef _DXLIB
		if((n>=0) && (n<SAT_SE_MAX))
			PlaySoundMem(se[n],DX_PLAYTYPE_LOOP);
#else
		SE::Play(n,1);
#endif
		break;
	default:
		// ������̂܂܏o��
		CSharpAt::MES_PUT(s);
		break;
	}
	return 0;
}
