#include "File.h"

#include "define.h"

#include <string.h>


File::File()
{
}


File::~File()
{
}

/*�Q�l:http://www.charatsoft.com/develop/otogema/ */

bool File::GetData(const char* fileName, DATA *data) {
	int FileHandle = FileRead_open(fileName);

	char s[256] = "";
	char d[256] = "";
	strcpy_s(s, 256, fileName);
	GetDirec(d,s);

	InitData(data);

	char buf[1024];

	while (1) {
		FileRead_gets(buf, 1024, FileHandle);
		if (/*buf[0] == NULL &&*/ FileRead_eof(FileHandle))        // �t�@�C���̏I�[�Ȃ猟���I���
			break;

		// �R�}���h�ȊO�Ȃ��΂�
		if (buf[0] != '#')
			continue;

		// �f�[�^����͂��āA�������ɏ�������
		int com = GetCommand(buf);
		char str[1024];
		ZeroMemory(str, 1024);
		if (!GetCommandString(buf, str)) {      // ������̎擾�����s�Ȃ�
			FileRead_close(FileHandle);
			return FALSE;
		}
		
		//char s[256];
		int level = 0;
		char name[256] = "";

		switch (com)
		{
			//case 0: // PLAYER
			//	data. = atoi(str);
			//	break;
		case 0: // GENRE
			strcpy_s(data->mGenre,246, str);
			break;
		case 1: // TITLE
			strcpy_s(data->mTitle,256, str);
			break;
		case 2: // ARTIST
			strcpy_s(data->mArtist,256, str);
			break;
		case 3: // BPM
			data->fBpm = (float)atof(str);
			break;
		case 4: // DEMO
			sprintf_s(s, 256, "%s%s", d, str);	//�f�B���N�g��/�t�@�C����
			strcpy_s(data->mDemo, 256, s);
			break;
		case 5: // MUSIC
			sprintf_s(s, 256, "%s%s", d, str);
			strcpy_s(data->mMusic, 256, s);
			break;
		case 6: // JAKET
			sprintf_s(s, 256, "%s%s", d, str);
			strcpy_s(data->mJaket, 256, s);
			break;
		case 7: // LEVEL
			sscanf_s(str, "%d,%s", &level, name, 256);
			strcpy_s(data->mLevel[level], 256, name);
			break;
		case 8: // SCORE
			sscanf_s(str, "%d,%s", &level, name, 256);
			sprintf_s(s, 256, "%s%s", d, name);
			strcpy_s(data->mScore[level], 256, s);
			break;
		}
	}

	//���R�[�h
	sprintf_s(s, 256, "%s%s", d, "record.dat");
	strcpy_s(data->mRecord, 256, s);

	FileRead_close(FileHandle);
	return TRUE;
}

bool File::GetNotes(const char* fileName, vector<NOTE> *notes) {
	int FileHandle = FileRead_open(fileName);

	char buf[1024];

	while (1) {
		FileRead_gets(buf, 1024, FileHandle);
		if (/*buf[0] == NULL &&*/ FileRead_eof(FileHandle))        // �t�@�C���̏I�[�Ȃ猟���I���
			break;
		if (buf == "\n")	//��s�͔�΂�
			continue;

		int time=0, side=0, type=0, pos=0;
		NOTE *note = new NOTE();
		sscanf_s(buf, "%d,%d,%d,%d", &time, &side, &type, &pos);
		note->mTime = time, note->mSide = side, note->mType = type, note->mPosX = pos;
		notes->push_back(*note);
		delete note;
	}

	FileRead_close(FileHandle);
	return TRUE;
}


// �R�}���h�̔ԍ���Ԃ�
int File::GetCommand(char *s)
{
	const char *command[DATA_N] = {
		//"PLAYER\0",
		"GENRE\0",
		"TITLE\0",
		"ARTIST\0",
		"BPM\0",
		"DEMO\0",
		"MUSIC\0",
		"JAKET\0",
		"LEVEL\0",
		"SCORE\0"
	};

	s++;  //'#'�̎���

	// �������[�`��
	int i;
	for (i = 0; i<DATA_N; i++) {
		if (_strnicmp(s, command[i], strlen(command[i])) == 0)
			return i;       // �R�}���h�Ȃ炻�̔ԍ���Ԃ�
	}

	// �I�u�W�F�z�u�Ȃ� -1
	return -1;
}

// �R�}���h�̃p�����[�^��������擾�i'\n'�͍폜 ':'����؂�Ƃ��ď����j
// src = ���o������������
// dst = ���o������̃o�b�t�@
bool File::GetCommandString(char *src, char *dst)
{
	int i, j;
	i = j = 0;
	// �܂��\�[�X�f�[�^����f�[�^�����܂ł̃|�C���^���Z�o
	while (1)
	{
		if (src[i] == ' ' || src[i] == 0x09 || src[i] == ':') {
			i++;
			break;
		}
		if (src[i] == '\n' || src[i] == NULL) {
			return FALSE;
		}
		i++;
	}

	// �I�[�܂ł��R�s�[
	while (1)
	{
		if (src[i] == '\n' || src[i] == NULL)
			break;
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = NULL;
	return TRUE;
}

void File::GetDirec(char* direc,char* fileName) {
	char result[256] = "";
	char *p = strstr(fileName, "data.txt");
	int i;
	for (i = 0; fileName[i] != *p; i++) {
		result[i] = fileName[i];
	}
	result[i] = '\0';
	
	strcpy_s(direc, 256, result);
}

void File::InitData(DATA *data) {
	strcpy_s(data->mGenre, 256, "");
	strcpy_s(data->mTitle, 256, "");
	strcpy_s(data->mArtist, 256, "");
	data->fBpm = 120;
	strcpy_s(data->mDemo, 256, "");
	strcpy_s(data->mMusic, 256, "");
	strcpy_s(data->mJaket, 256, "");
	strcpy_s(data->mLevel[0], 256, "���x��1");
	strcpy_s(data->mLevel[1], 256, "���x��2");
	strcpy_s(data->mLevel[2], 256, "���x��3");
	strcpy_s(data->mLevel[3], 256, "���x��4");
	strcpy_s(data->mScore[0], 256, "");
	strcpy_s(data->mScore[1], 256, "");
	strcpy_s(data->mScore[2], 256, "");
	strcpy_s(data->mScore[3], 256, "");
}