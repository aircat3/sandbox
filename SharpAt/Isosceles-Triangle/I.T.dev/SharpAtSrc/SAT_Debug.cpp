// SAT_Debug.cpp
// #@�p�g���֐��FD�F�f�o�b�O�p�̃��b�Z�[�W�\��
#include	"SharpAt.h"

/*

���g�p�၄
Da;	// a0�̒l�����b�Z�[�W�{�b�N�X���ŕ\��
D@;	// @�|�C���^���̒l��\��
D����������;	// ����������̂܂ܕ\��

��#@�ւ̓o�^���@��
SharpAt.cpp �� p_ExProc �ɁA�擪�̊֐��iExPr_?�j��ǉ�����

*/

//=======================================
// �g���֐� D�F�f�o�b�O�p�̃��b�Z�[�W�o��
//=======================================
int CSharpAt::ExPr_DMes(CSharpAt *ps, char *s)
{
	// �ϐ����Ȃ�A���̒l��\��
	int n;
	if (ps->GetNumExStr(s, n))
	{
		CSharpAt::MES_PUT("%d", n);
	}
	else
	{
		// ������̂܂܏o��
		CSharpAt::MES_PUT(s);
	}
	return 0;
}

// ���Ή��R�[�h�ł��邱�Ƃ�\��
int CSharpAt::ExPr_D(CSharpAt *ps,char *s)
{
	ps->NON_CODE_PUT(ps->pos-3);
	return 0;
}
