// SAT_Key.cpp
// #@�p�g���֐��FK�F�L�[����擾�p
#include	"SharpAt.h"

/*

���g�p�၄
a=KA;		// A�`Z�A0�`9���̃L�[����p�A�w��̃L�[��������Ă����1�A�����łȂ����0��Ԃ�
a=KUP;		// UP DW LF RT �ŏ㉺���E�L�[������\
a=KEn;		// En=Enter, Es=Esc, Ta=Tab, Ct=Ctrl, Sp=Space, Sh=Shift�L�[������\
a=KML;		// ML=�}�E�X�̍��N���b�N, MR=�}�E�X�̉E�N���b�N
a=KMX;		// MX=�}�E�X��X���W, MY=�}�E�X��Y���W
�i��L�ȊO�́A�K�v�ɉ����Ēǉ����Ă����j

�����Ӂ�
�����݂́A���Flip���ɃL�[���씻�肪����

*/

//=======================================
// ��`�n
//=======================================

//=======================================
// �g���֐� K�F�L�[����擾
//=======================================
#ifdef _DXLIB
int CSharpAt::ExPr_Key(CSharpAt *ps,char *s)
{
	// �}�E�X�n�H
	if(s[0] == 'M')
	{
		int x,y;
		switch(s[1])
		{
		case 'L': return ((GetMouseInput() & MOUSE_INPUT_LEFT)!=0);
		case 'R': return ((GetMouseInput() & MOUSE_INPUT_RIGHT)!=0);
		case 'X': GetMousePoint(&x,&y); return x;
		case 'Y': GetMousePoint(&x,&y); return y;
		}
		return 0;	// ERR
	}
	// �㉺���E�L�[
	if((s[0] == 'U') && (s[1] == 'P'))
		return ((CheckHitKey(KEY_INPUT_UP))!=0);
	if((s[0] == 'D') && (s[1] == 'W'))
		return ((CheckHitKey(KEY_INPUT_DOWN))!=0);
	if((s[0] == 'R') && (s[1] == 'T'))
		return ((CheckHitKey(KEY_INPUT_RIGHT))!=0);
	if((s[0] == 'L') && (s[1] == 'F'))
		return ((CheckHitKey(KEY_INPUT_LEFT))!=0);
	// �X�y�[�X��
	if((s[0] == 'S') && (s[1] == 'p'))
		return ((CheckHitKey(KEY_INPUT_SPACE))!=0);
	if((s[0] == 'E') && (s[1] == 'n'))
		return ((CheckHitKey(KEY_INPUT_RETURN))!=0);
	if((s[0] == 'E') && (s[1] == 's'))
		return ((CheckHitKey(KEY_INPUT_ESCAPE))!=0);
	if((s[0] == 'T') && (s[1] == 'a'))
		return ((CheckHitKey(KEY_INPUT_TAB))!=0);
	if((s[0] == 'C') && (s[1] == 't'))
		return ((CheckHitKey(KEY_INPUT_LCONTROL)||CheckHitKey(KEY_INPUT_RCONTROL))!=0);
	if((s[0] == 'S') && (s[1] == 'h'))
		return ((CheckHitKey(KEY_INPUT_LSHIFT)||CheckHitKey(KEY_INPUT_RSHIFT))!=0);
	return 0;
}
#else
int CSharpAt::ExPr_Key(CSharpAt *ps,char *s)
{
	// �}�E�X�n�H
	if(s[0] == 'M')
	{
		switch(s[1])
		{
		case 'L': return ((c_key&MUS_L)!=0);
		case 'R': return ((c_key&MUS_R)!=0);
		case 'X': return musPX;
		case 'Y': return musPY;
		}
		return 0;	// ERR
	}
	// �㉺���E�L�[
	if((s[0] == 'U') && (s[1] == 'P'))
		return ((t_key&KEY_UP)!=0);
	if((s[0] == 'D') && (s[1] == 'W'))
		return ((t_key&KEY_DW)!=0);
	if((s[0] == 'R') && (s[1] == 'T'))
		return ((t_key&KEY_RT) != 0);
	if((s[0] == 'L') && (s[1] == 'F'))
		return ((t_key&KEY_LF) != 0);
	// �X�y�[�X��
	if((s[0] == 'S') && (s[1] == 'p'))
		return ((c_key&KEY_SPC) != 0);
	if((s[0] == 'E') && (s[1] == 'n'))
		return ((c_key&KEY_RET) != 0);
	if((s[0] == 'E') && (s[1] == 's'))
		return ((c_key&KEY_ESC) != 0);
	if((s[0] == 'T') && (s[1] == 'a'))
		return ((c_key&KEY_TAB) != 0);
	if((s[0] == 'C') && (s[1] == 't'))
		return ((c_key&KEY_CTL) != 0);
	if((s[0] == 'S') && (s[1] == 'h'))
		return ((c_key&KEY_SFT) != 0);
	return 0;
}
#endif
