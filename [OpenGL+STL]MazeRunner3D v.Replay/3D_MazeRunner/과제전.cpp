#include "과제전.h"
#include "stdafx.h"

void saveFPS(float x, float z, float deltaAngle, float angle) {
	ofstream out("Replay.txt", ios::app);
	out << (float)x;
	out << " ";
	out << (float)z;
	out << " ";
	out << (float)deltaAngle;
	out << " ";
	out << (float)angle;
	out << "\n";
}

void replayLoad() {
	ifstream in("Replay.txt");

	Point2D point;
	Angle angle;

	cont.clear();
	while (in >> point.first >> point.second >> angle.first >> angle.second) {
		cont.push_back(make_pair(point, angle));
	} // 컨테이너에 텍스트로 저장해놓은 좌표값과 회전정도를 불러오기.
}

void UI_1() {
	play_bool = true;
	replay_bool = false;
	printf("[ Gmae Start! ]\n\n");
	Replay();
	ofstream out("Replay.txt");
}

void UI_2() {
	play_bool = false;
	replay_bool = true;
	printf("Replay\n");
	replayLoad();
	Replay();
	it = cont.begin();
}

void UI() {
	system("cls");
	int input;
	while (true) {
		printf("□■□■□■□■□■□■□■\n");
		printf("■                        ■\n");
		printf("□                        □\n");
		printf("■      1. GamePlay       ■\n");
		printf("□      2. Replay         □\n");
		printf("■                        ■\n");
		printf("□                        □\n");
		printf("■□■□■□■□■□■□■□\n");
		printf("■                        ■\n");
		printf("□ Gameplay : ↑ ↓ → ← □\n");
		printf("■ Replay :   E 되감기    ■\n");
		printf("□            W 일시정지  □\n");
		printf("■                        ■\n");
		printf("□                        □\n");
		printf("■ [일시정지] S 한칸 뒤   ■\n");
		printf("□            D 한칸 앞   □\n");
		printf("■                        ■\n");
		printf("■□■□■□■□■□■□■□\n");
		
		cin >> input;

		if (input == 1) {
			startinit();
			UI_1();
			break;
		}
		if (input == 2) {
			startinit();
			UI_2();
			break;
		}
	}
}

void startinit()
{
	cheat = 3;
	speedup = 0;
	speed_cnt = 0;
	passblock = 0;
	mini_item = 8;
	cutoff = 10;
	density = 0.3;
	mainscore = 0;
	hp_max = 0;
	hint = false;
	x = 0;
	z = 5;
	angle = 0;
	deltaAngle = 0;
	Light_rot = 0;
	light_1 = true;
	replay_bool = false;
	play_bool = false;
}

///////////////////////////////////////////////

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	// 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp)<1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp)<(unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth *
			(*info)->bmiHeader.biBitCount + 7) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp)<(unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}

void Fog()
{
	glEnable(GL_FOG);

	glFogf(GL_FOG_MODE, GL_EXP);

	glFogfv(GL_FOG_COLOR, fog_color); // fog_color는 안개의 색을 의미한다. 

	glFogf(GL_FOG_START, start_fog); // start는 world coordinate상에서 안개 시작 위치를 의미한다.  

	glFogf(GL_FOG_END, end_fog); // end는 world coordinate상에서 안개 끝 위치를 의미한다. 

	glFogf(GL_FOG_DENSITY, density); // fog mode가 GL_EXP, GL_EXP2일 경우 밀도의 설정이 가능 
}

void draw_hp()
{
	for (int i = 0; i < hp_max; i++)
	{
		glPushMatrix();
		{
			glColor3f(1, 1, 1);
			glTranslatef(-45 + i * 22, -57, 0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glBegin(GL_POLYGON);
			{
				glTexCoord2f(1, 1);
				glVertex2f(-8, 8);
				glTexCoord2f(0, 1);
				glVertex2f(-8, -8);
				glTexCoord2f(0, 0);
				glVertex2f(8, -8);
				glTexCoord2f(1, 0);
				glVertex2f(8, 8);
			}
			glEnd();
		}
		glPopMatrix();
	}
}

int Loadfile()
{
	FILE *fp;
	switch (mapcollect)
	{
	case 0:
		fp = fopen(MAP_1, "rt");
		break;
	case 1:
		fp = fopen(MAP_2, "rt");
		break;
	case 2:
		fp = fopen(MAP_3, "rt");
		break;
	case 3:
		fp = fopen(MAP_4, "rt");
		break;
	case 4:
		fp = fopen(MAP_5, "rt");
		break;
	case 5:
		fp = fopen(INIT, "rt");
		break;
	}


	if (fp == NULL)
	{
		printf("\n실패\n");
		return 1;
	}

	printf("\n완료\n");

	int cha;

	while (feof(fp) == 0)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				fscanf(fp, "%d", &cha);
				makeboard[i][j] = cha;
			}
		}
	}
	fclose(fp);

	return 1;
}

void renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void letter_Play()
{
	glColor3f(1, 1, 1);
	char *string = "ITEM	:";    glRasterPos2f(-80, -80);  // 문자 출력할 위치 설정  
	int len = (int)strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);

	glColor3f(1, 1, 1);
	char *string_1 = "HP	:";    glRasterPos2f(-80, -60);  // 문자 출력할 위치 설정  
	int len_1 = (int)strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_1[i]);

	glColor3f(1, 1, 1);
	char *string_2 = "SCORE	:";    glRasterPos2f(-80, -40);  // 문자 출력할 위치 설정  
	int len_2 = (int)strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_2[i]);
}

void letter_Menu()
{
	glColor3f(1, 1, 1);
	char *string_10 = "START";    glRasterPos2f(-68, -70);  // 문자 출력할 위치 설정  
	int len_10 = (int)strlen(string_10);
	for (int i = 0; i < len_10; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_10[i]);

	glColor3f(1, 1, 1);
	char *string_11 = "SCORE";    glRasterPos2f(-15, -70);  // 문자 출력할 위치 설정  
	int len_11 = (int)strlen(string_11);
	for (int i = 0; i < len_11; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_11[i]);

	glColor3f(1, 1, 1);
	char *string_12 = "QUIT";    glRasterPos2f(43, -70);  // 문자 출력할 위치 설정  
	int len_12 = (int)strlen(string_12);
	for (int i = 0; i < len_12; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_12[i]);
}

void letter_item()
{
	if (mini_item == 0)
	{
		glColor3f(1, 1, 1);
		char *string_20 = "SPEED UP";    glRasterPos2f(x, y+3);  // 문자 출력할 위치 설정  
		int len_20 = (int)strlen(string_20);
		for (int i = 0; i < len_20; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_20[i]);
	}

	if (mini_item == 1)
	{
		char *string_21 = "HP! UP";    glRasterPos2f(x, y + 3);  // 문자 출력할 위치 설정  
		int len_21 = (int)strlen(string_21);
		for (int i = 0; i < len_21; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_21[i]);
	}

	if (mini_item == 2)
	{
		char *string_22 = "BREAK BLOCK![2]";    glRasterPos2f(x, y + 3);  // 문자 출력할 위치 설정  
		int len_22 = (int)strlen(string_22);
		for (int i = 0; i < len_22; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_22[i]);
	}

	if (mini_item == 3)
	{
		char *string_23 = "WIDE VIEW";    glRasterPos2f(x, y + 3);  // 문자 출력할 위치 설정  
		int len_23 = (int)strlen(string_23);
		for (int i = 0; i < len_23; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_23[i]);
	}

	if (mini_item == 4)
	{
		char *string_24 = "LIGHT UP";    glRasterPos2f(x, y + 3);  // 문자 출력할 위치 설정  
		int len_24 = (int)strlen(string_24);
		for (int i = 0; i < len_24; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_24[i]);
	}

	if (mini_item == 5)
	{
		char *string_25 = "LIGHT UP";    glRasterPos2f(x, y + 3);  // 문자 출력할 위치 설정  
		int len_25 = (int)strlen(string_25);
		for (int i = 0; i < len_25; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_25[i]);
	}
}

void letter_time()
{
	char p[20];
	char s[20];
	sprintf(p, "Break : [ %d ]", passblock);
	sprintf(s, "Speed : [ %d ]", speed_cnt);
	sprintf(t, "%d", mainscore);

	char *c;
	glRasterPos3f(-50, -40, 0);
	for (c = t; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	} // 점수표시

	char *cp;
	glRasterPos3f(0, -40, 0);
	for (cp = p; *cp != '\0'; cp++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *cp);
	} // 블록 깨기

	char *cs;
	glRasterPos3f(0, -80, 0);
	for (cs = s; *cs != '\0'; cs++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *cs);
	} // 속도
}

void letter_score()
{
	glColor3f(1, 1, 1);
	char *string_50 = "V Easy";    glRasterPos2f(-15, -44);  // 문자 출력할 위치 설정  
	int len_50 = (int)strlen(string_50);
	for (int i = 0; i < len_50; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_50[i]);

	glColor3f(1, 1, 1);
	char *string_51 = "Easy";    glRasterPos2f(-15, -18);  // 문자 출력할 위치 설정  
	int len_51 = (int)strlen(string_51);
	for (int i = 0; i < len_51; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_51[i]);

	glColor3f(1, 1, 1);
	char *string_52 = "Normal";    glRasterPos2f(-15, 8);  // 문자 출력할 위치 설정  
	int len_52 = (int)strlen(string_52);
	for (int i = 0; i < len_52; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_52[i]);

	glColor3f(1, 1, 1);
	char *string_53 = "Hard";    glRasterPos2f(-15, 34);  // 문자 출력할 위치 설정  
	int len_53 = (int)strlen(string_53);
	for (int i = 0; i < len_53; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_53[i]);

	glColor3f(1, 1, 1);
	char *string_54 = "V Hard";    glRasterPos2f(-15, 60);  // 문자 출력할 위치 설정  
	int len_54 = (int)strlen(string_54);
	for (int i = 0; i < len_54; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string_54[i]);
}

///////////////////////////////////////////////////////////////////////

void computeDir(float deltaAngle) {
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void collision(point a, int i, int j)
{
	if (a.max_x > x && a.min_x < x && a.max_z<z - 3 && a.min_z > z - 3)
	{
		if (passblock > 0)
		{
			PlaySound(TEXT(SOUND_FILE_NAME), NULL, SND_ASYNC | SND_SYNC); // 메뉴바 선택음...!
			passblock--;
			makeboard[i][j] = 4;
		}
		else
		{
			PlaySound(TEXT(SOUND_FILE_NAME_CRASH), NULL, SND_ASYNC | SND_ALIAS);
			if (hp_max == 1)
			{
				startinit();
				PlaySound(TEXT(SOUND_FILE_NAME_FAIL), NULL, SND_ASYNC | SND_ALIAS);
				UI();
				hp_max++;
			}
			//cout << "crash" << endl;

			x = 0;
			z = 3;
			hp_max -= 1;
		}
	}
}

void collision_sideline(point a, int i, int j)
{
	if (a.max_x > x && a.min_x < x && a.max_z<z - 3 && a.min_z > z - 3)
	{
		PlaySound(TEXT(SOUND_FILE_NAME_CRASH), NULL, SND_ASYNC | SND_ALIAS);
		if (hp_max == 1)
		{
			startinit();
			PlaySound(TEXT(SOUND_FILE_NAME_FAIL), NULL, SND_ASYNC | SND_ALIAS);
			UI();
			hp_max++;
		}
		//cout << "crash" << endl;
		
		x = 0;
		z = 3;
		hp_max -= 1;
	}
}

void collision_item(point a)
{
	if (a.max_x > x && a.min_x < x && a.max_z<z - 3 && a.min_z > z - 3)
	{
		mainscore += 40;
		bool_item = true;
		//cout << "get item" << endl;
		PlaySound(TEXT(SOUND_FILE_NAME), NULL, SND_ASYNC | SND_ALIAS);

		srand((unsigned)time(NULL));

		mini_item = rand() % 5;
		switch (mini_item)
		{
		case 0:
			speedup += 0.1; // 속도증가
			speed_cnt++;
			//cout << "속도증가" << endl;
			break;
		case 1:
			hp_max++; // 체력 증가
			//cout << "체력증가" << endl;
			break;
		case 2:
			passblock += 2;
			//pass = true;
			//cout << "블럭깨기" << endl;
			break;
		case 3:
			cheat += 1;
			//cout << "시야 증가" << endl;
			break;
		case 4:
			cutoff += 10;
			//cout << "밝기증가" << endl;
			break;
		}
	}
}

void collision_line(point a)
{
	if (a.max_x > x && a.min_x < x && a.max_z<z - 3 && a.min_z > z - 3)
	{
		mainscore += 20;
		bool_line = true;
	}
}

void collision_endline(point a)
{
	char str[100];
	if (a.max_x > x && a.min_x < x && a.max_z<z - 3 && a.min_z > z - 3)
	{
		PlaySound(TEXT(SOUND_FILE_NAME_FINISH), NULL, SND_ASYNC | SND_ALIAS);

		printf("Replay가 저장되었습니다. \n");
		hp_max = 0;
		startinit();

		UI();
	}
}

void collision_ghost(point a)
{
	if (a.max_x > x && a.min_x < x && a.max_z<z - 3 && a.min_z > z - 3)
	{
		PlaySound(TEXT(SOUND_FILE_NAME_CRASH), NULL, SND_ASYNC | SND_ALIAS);
		if (hp_max == 1)
		{
			startinit();
			light_1 = true;
			PlaySound(TEXT(SOUND_FILE_NAME_FAIL), NULL, SND_ASYNC | SND_ALIAS);

			UI();
			hp_max++;
		}
		//cout << "crash ghost" <<endl;

		printf("%3f \n", x);
		printf("%3f \n\n", z);


		printf("%3f \n", a.max_x);
		printf("%3f \n", a.min_x);
		printf("%3f \n", a.max_z);
		printf("%3f \n", a.min_z);
		x = 0;
		z = 3;
		hp_max -= 1;
		//light_1 = true;
	}
}

bool warning(point a)
{
	//glBegin(GL_LINE_LOOP);
	//{
	//	glVertex3f(a.max_x, 1, a.max_z);
	//	glVertex3f(a.min_x, 1, a.max_z);
	//	glVertex3f(a.min_x, 1, a.min_z);
	//	glVertex3f(a.max_x, 1, a.min_z);
	//}
	//glEnd();

	if (a.max_x < x - 2.5 || a.min_x > x + 2.5)
		return false;
	if (a.min_z < z - 5.5 || a.max_z > z - 0.5)
		return false;

	return true;
}

bool collision_ramain(point a)
{
	if (a.max_x < x - 3 || a.min_x > x + 3)
		return false;
	if (a.min_z < z - 3 || a.max_z > z + 3)
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////

void effectonoff()
{
	if (shadingon == true)
	{
		glShadeModel(GL_SMOOTH);
	}
	else
	{
		glShadeModel(GL_FLAT);
	}
	if (depthon == true)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (cullingon == true)
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glFrontFace(GL_CCW);
		glDisable(GL_CULL_FACE);
	}
}

void init_board()
{
	x_Rotate = 0.0;
	y_Rotate = 0.0;
	z_Rotate = 0.0;
	XR_P = 0;
	YR_P = 0;
	ZR_P = 0;
	bool greedon = true;
	// 축 이동

	look_x = 0;
	look_y = 0;
	look_z = 0;
} // 초기화 함수

void LargeView()
{
	look_x = 0.00001;
	look_y = 13;
	look_z = 9;
	center_x = 0;
	center_y = -130;
	center_z = -200;
	up_x = 0;
	up_y = 1;
	up_z = 0;
}

void TopView()
{
	look_x = 0.00001;
	look_y = 8;
	look_z = 0.00001;
	center_x = 0;
	center_y = 1;
	center_z = 0;
	up_x = 0;
	up_y = -1;
	up_z = 0;
}

void CameraView()
{
	look_x = 0;
	look_y = 0;
	look_z = -3;
	center_x = 0;
	center_y = 0;
	center_z = -100;
	up_x = 0;
	up_y = 1;
	up_z = 0;
}

void Light_1()
{
	GLfloat AmbientLight[] = { ambientL, ambientL, ambientL, ambientL };//주변 조명
	GLfloat DiffuseLight[] = { diffuseL, diffuseL, diffuseL, diffuseL };//산란 반사 조명
	GLfloat SpecularLight[] = { specularL, specularL, specularL, specularL };//거울반사 조명
	GLfloat lightPos[] = { -28, 0, 0, 1 };
	GLfloat specref[] = { 1,1,1,1 };
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	if (light_1 == true)
	{
		glPushMatrix();
		{
			glColor3f(4, 4, 0);
			glTranslated(-75, 0, 0);
			glTranslated(0, -1, 0);
			glTranslated(0, 0, 0);
			glutSolidSphere(10, 50, 50);
		}
		glPopMatrix();

		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}
}

void Lantern()
{
	GLfloat Direction[] = { 0, 0, 1 };//주변 조명
	GLfloat lightPos[] = { 0, 1, 0, 1 };
	GLfloat specref[] = { 1,1,1,1 };

	GLfloat AmbientLight[] = { ambientL, ambientL, ambientL, ambientL };//주변 조명
	GLfloat DiffuseLight[] = { diffuseL, diffuseL, diffuseL, diffuseL };//산란 반사 조명
	GLfloat SpecularLight[] = { specularL, specularL, specularL, specularL };//거울반사 조명

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff - warning_sign);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.9);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);

	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	glEnable(GL_COLOR_MATERIAL); // 오오... 개체의 기본색을 입히는거같아..!
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	if (light_1 == false)
	{
		glEnable(GL_LIGHT1);
	}
	else
	{
		glDisable(GL_LIGHT1);
	}
}

void item()
{
	glPushMatrix();
	{
		glEnable(GL_BLEND);
		glPushMatrix();
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			//glColor3f(0.7, 0.2, 0.2);
			glTranslatef(0, 2, 0);
			glColor4f(0, 0.8, 0, 0.8);
			glRotatef(obj_rot, 0, 1, 0);
			glutSolidCube(0.3);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor4f(0.7, 0, 0, 0.5);
			glTranslatef(0, 2, 0);
			glRotatef(obj_rot, 0, 1, 0);
			glRotatef(90, -1, 0, -1);
			glutSolidCube(0.6);
		}
		glPopMatrix();

		glDisable(GL_BLEND);
	}
	glPopMatrix();
}

void remain()
{
	glPushMatrix();
	{
		glEnable(GL_BLEND);
		glPushMatrix();
		{
			//glScaled(1.5,1,1.5);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			//glColor3f(0.7, 0.2, 0.2);
			glTranslatef(0, 0.6, 0);
			glColor4f(0, 0.5, 0.5, 0.8);
			glRotatef(obj_rot, 0, 1, 0);
			glutSolidSphere(0.1, 15, 15);
			//glutSolidCube(0.1);
		}
		glPopMatrix();

		glPushMatrix();
		{
			//glScaled(1.5, 1, 1.5);
			glColor4f(0.0, 0.0, 0.8, 0.5);
			glTranslatef(0, 0.6, 0);
			glRotatef(obj_rot + 5, 0, 1, 0);
			glRotatef(90, -1, 0, -1);
			//glutSolidSphere(0.15, 15, 15);
			glutSolidCube(0.25);
		}
		glPopMatrix();

		glDisable(GL_BLEND);
	}
	glPopMatrix();
}

void Person()
{
	glPushMatrix();
	{
		glTranslatef(0, 0.5, 0);
		glColor3f(1, 8, 1);
		glScalef(0.4, 0.75, 0.4);

		glBegin(GL_QUADS);
		{
			//윗면
			glColor3f(0.0f, 0.0f, 1.0f);// Blue
			glVertex3f(1, 1, 1);
			glColor3f(0.0f, 1.0f, 0.0f);// Green
			glVertex3f(1, 1, -1);
			glColor3f(1.0f, 0.0f, 0.0f);// Red
			glVertex3f(-1, 1, -1);
			glColor3f(1.0f, 1.0f, 0.0f);// Yellow
			glVertex3f(-1.0, 1.0, 1.0f);

			//뒷면
			glColor3f(0.0f, 1.0f, 0.0f);// Green
			glVertex3f(1, 1, -1);
			glColor3f(0.0f, 0.0f, 0.0f);// Black
			glVertex3f(1, -1, -1);
			glColor3f(1.0f, 1.0f, 1.0f);// White
			glVertex3f(-1, -1, -1);
			glColor3f(1.0f, 0.0f, 0.0f);// Red
			glVertex3f(-1, 1, -1);

			//아랫면
			glColor3f(1.0f, 0.0, 1.0f);// Magenta
			glVertex3f(-1, -1, 1);
			glColor3f(1.0f, 1.0f, 1.0f);// White
			glVertex3f(-1, -1, -1);
			glColor3f(0.0f, 0.0f, 0.0f);// Black
			glVertex3f(1, -1, -1);
			glColor3f(0.0f, 1.0f, 1.0f);// Cyan
			glVertex3f(1, -1, 1);

			//왼면
			glColor3f(1.0f, 0.0f, 0.0f);// Red
			glVertex3f(-1, 1, -1);
			glColor3f(1.0f, 1.0f, 1.0f);// White
			glVertex3f(-1, -1, -1);
			glColor3f(1.0f, 0.0, 1.0f);// Magenta
			glVertex3f(-1, -1, 1);
			glColor3f(1.0f, 1.0f, 0.0f);// Yellow
			glVertex3f(-1.0, 1.0, 1.0f);

			//오른면

			glColor3f(0.0f, 0.0f, 1.0f);// Blue
			glVertex3f(1, 1, 1);
			glColor3f(0.0f, 1.0f, 1.0f);// Cyan
			glVertex3f(1, -1, 1);
			glColor3f(0.0f, 0.0f, 0.0f);// Black
			glVertex3f(1, -1, -1);
			glColor3f(0.0f, 1.0f, 0.0f);// Green
			glVertex3f(1, 1, -1);
		}
		glEnd();

		glLineWidth(2);
		glColor3f(0.5, 0.5, 0.5);
		//glBegin(GL_LINE_LOOP);
		//{
		//	glVertex3f(static_person.max_x + 15, 2, static_person.max_z + 15);
		//	glVertex3f(static_person.min_x + 15, 2, static_person.max_z + 15);
		//	glVertex3f(static_person.min_x + 15, 2, static_person.min_z + 15);
		//	glVertex3f(static_person.max_x + 15, 2, static_person.min_z + 15);
		//}
		//glEnd();
		glLineWidth(1);

	}
	glPopMatrix();
}

void ghost()
{
	for (int i = 0; i < 4; i++)
	{
		glEnable(GL_BLEND);
		glPushMatrix();
		{
			glTranslatef((10 * i) - 16, 0, -16 + Big_x);
			glRotatef(0, 0, 1, 0);
			glBlendFunc(GL_ONE, GL_ONE);
			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(0.0f, 0.85f, 0.0f);
			glutSolidSphere(0.9f, 20, 20);

			glTranslatef(0.0f, 1.15f, 0.0f);
			glutSolidSphere(0.50f, 20, 20);

			glPushMatrix();
			glColor3f(0.0f, 0.0f, 0.0f);
			glTranslatef(0.07f, 0.14f, 0.45f);
			glutSolidSphere(0.13f, 10, 10);
			glTranslatef(-0.2f, 0.0f, 0.0f);
			glutSolidSphere(0.13f, 10, 10);
			glPopMatrix();

			glColor3f(1.0f, 0.5f, 0.5f);
			glutSolidCone(0.2f, 1.0f, 10, 2);
		}
		glPopMatrix();
		glDisable(GL_BLEND);

		static_ghost[i].min_x = -0.85 + (10 * i) - 16;
		static_ghost[i].min_z = 0.85 + Big_x - 16;
		static_ghost[i].max_x = 0.85 + (10 * i) - 16;
		static_ghost[i].max_z = -0.85 + Big_x - 16;


		//glBegin(GL_LINE_LOOP);
		//{
		//	glVertex3f(static_ghost[i].max_x, 1, static_ghost[i].max_z);
		//	glVertex3f(static_ghost[i].min_x, 1, static_ghost[i].max_z);
		//	glVertex3f(static_ghost[i].min_x, 1, static_ghost[i].min_z);
		//	glVertex3f(static_ghost[i].max_x, 1, static_ghost[i].min_z);
		//}
		//glEnd();

		glLineWidth(1); // 블럭 위치-
	}
	///////////////////////////////////////////////////////// 가로
	for (int i = 5; i < 8; i++)
	{
		glEnable(GL_BLEND);
		glPushMatrix();
		{
			glTranslatef(Big_x - 17, 0, (13 * i) - 78);
			glRotatef(90, 0, 1, 0);
			glBlendFunc(GL_ONE, GL_ONE);
			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(0.0f, 0.85f, 0.0f);
			glutSolidSphere(0.9f, 20, 20);

			glTranslatef(0.0f, 1.15f, 0.0f);
			glutSolidSphere(0.50f, 20, 20);

			glPushMatrix();
			glColor3f(0.0f, 0.0f, 0.0f);
			glTranslatef(0.07f, 0.14f, 0.45f);
			glutSolidSphere(0.13f, 10, 10);
			glTranslatef(-0.2f, 0.0f, 0.0f);
			glutSolidSphere(0.13f, 10, 10);
			glPopMatrix();

			glColor3f(1.0f, 0.5f, 0.5f);
			glutSolidCone(0.2f, 1.0f, 10, 2);
		}
		glPopMatrix();
		glDisable(GL_BLEND);

		static_ghost[i].min_x = 0.85 + Big_x - 17;
		static_ghost[i].min_z = -0.85 + (13 * i) - 78;
		static_ghost[i].max_x = -0.85 + Big_x - 17;
		static_ghost[i].max_z = 0.85 + (13 * i) - 78;


		//glBegin(GL_LINE_LOOP);
		//{
		//	glVertex3f(static_ghost[i].max_x, 1, static_ghost[i].max_z);
		//	glVertex3f(static_ghost[i].min_x, 1, static_ghost[i].max_z);
		//	glVertex3f(static_ghost[i].min_x, 1, static_ghost[i].min_z);
		//	glVertex3f(static_ghost[i].max_x, 1, static_ghost[i].min_z);
		//}
		//glEnd();
		glLineWidth(1); // 블럭 위치-
	}
}

void ghost_1()
{
	for (int i = 0; i < 5; i++)
	{
		glEnable(GL_BLEND);
		glPushMatrix();
		{
			glTranslatef((7 * i) - 16, 0, -small_z + 16);
			glBlendFunc(GL_ONE, GL_ONE);
			glRotatef(180, 0, 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(0.0f, 0.75f, 0.0f);
			glutSolidSphere(0.40f, 20, 20);

			glTranslatef(0.0f, 0.55f, 0.0f);
			glutSolidSphere(0.20f, 20, 20);

			glPushMatrix();
			glColor3f(0.0f, 0.0f, 0.0f);
			glTranslatef(0.05f, 0.10f, 0.18f);
			glutSolidSphere(0.05f, 10, 10);
			glTranslatef(-0.1f, 0.0f, 0.0f);
			glutSolidSphere(0.05f, 10, 10);
			glPopMatrix();

			glColor3f(1.0f, 0.5f, 0.5f);
			glutSolidCone(0.08f, 0.45f, 10, 2);
		}
		glPopMatrix();
		glDisable(GL_BLEND);

		static_ghost_1[i].min_x = -0.5 + (7 * i) - 16;
		static_ghost_1[i].min_z = 0.5 + -small_z + 16;
		static_ghost_1[i].max_x = 0.5 + (7 * i) - 16;
		static_ghost_1[i].max_z = -0.5 + -small_z + 16;

		//glBegin(GL_LINE_LOOP);
		//{
		//	glVertex3f(static_ghost_1[i].max_x, 1, static_ghost_1[i].max_z);
		//	glVertex3f(static_ghost_1[i].min_x, 1, static_ghost_1[i].max_z);
		//	glVertex3f(static_ghost_1[i].min_x, 1, static_ghost_1[i].min_z);
		//	glVertex3f(static_ghost_1[i].max_x, 1, static_ghost_1[i].min_z);
		//}
		//glEnd();
		glLineWidth(1); // 블럭 위치
	}

}


void drawGreed(void)
{
	glColor3f(0.4, 0.4, 0.4);
	glLineWidth(0.5);
	glBegin(GL_LINES);
	for (int i = 0; i < 100; i++) {
		glVertex3f(-50, 0, i - 50);
		glVertex3f(50, 0, i - 50);
	}
	for (int i = 0; i < 100; i++) {
		glVertex3f(i - 50, 0, -50);
		glVertex3f(i - 50, 0, 50);
	}
	glEnd();
}

void color_change()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			random_device seed_gen;
			mt19937 engine(seed_gen());
			uniform_real_distribution<> dist(distribution - 0.1, distribution);

			colorbuffer[i][j] = dist(engine);
		}
	}
}

void draw_block()
{
	glPushMatrix();
	{
		glColor3f(colorbuffer[0][0], colorbuffer[0][1], colorbuffer[0][2]);
		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glColor3f(colorbuffer[1][0], colorbuffer[1][1], colorbuffer[1][2]);
		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glColor3f(colorbuffer[2][0], colorbuffer[2][1], colorbuffer[2][2]);
		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glColor3f(colorbuffer[3][0], colorbuffer[3][1], colorbuffer[3][2]);
		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void Line_block()
{
	glColor3f(0.7, 0.7, 0.7);

	glPushMatrix();
	{
		glNormal3f(0, 1, 0);
		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void board_maker()
{
	random_device seed_gen;
	mt19937 engine(seed_gen());
	uniform_real_distribution<> dist(0,1);

	glEnable(GL_BLEND);
	glPushMatrix();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glColor3f(0.7, 0.2, 0.2);
		glTranslatef(0, 2, 2);
		glColor4f(dist(engine), dist(engine), dist(engine), dist(engine));
		glRotatef(obj_rot*2, 0, 1, 0);
		glutSolidCube(0.5);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor4f(dist(engine), dist(engine), dist(engine), dist(engine));
		glTranslatef(0, 2, 2);
		glRotatef(obj_rot*2, 0, 1, 0);
		glRotatef(90, -1, 0, -1);
		glutSolidCube(1.0);
	}
	glPopMatrix();

	glDisable(GL_BLEND);

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (makeboard[i][j] == 1)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					draw_block();
				}
				glPopMatrix(); // 블럭 표시

				glColor3f(1, 0, 0);
				glLineWidth(1);
				//glBegin(GL_LINE_LOOP);
				//{
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].min_z);
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].min_z);
				//}
				//glEnd();
				glLineWidth(1); // 블럭 위치
			}
			else if (makeboard[i][j] == 2)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					//item();
				}
				glPopMatrix(); // 아이템 표시

				glColor3f(0, 1, 0);
				glLineWidth(1);
				//glBegin(GL_LINE_LOOP);
				//{
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].min_z);
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].min_z);
				//}
				//glEnd();
				glLineWidth(1); // 아이템 위치
			}
			else if (makeboard[i][j] == 3)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					glEnable(GL_BLEND);
					glPushMatrix();
					{
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						//glColor3f(0.7, 0.2, 0.2);
						glTranslatef(0, 2, 0);
						glColor4f(dist(engine), dist(engine), dist(engine), dist(engine));
						glRotatef(obj_rot, 0, 1, 0);
						glutSolidCube(0.5);
					}
					glPopMatrix();

					glPushMatrix();
					{
						glColor4f(dist(engine), dist(engine), dist(engine), dist(engine));
						glTranslatef(0, 2.0, 0);
						glRotatef(obj_rot, 0, 1, 0);
						glRotatef(90, -1, 0, -1);
						glutSolidCube(1.0);
					}
					glPopMatrix();

					glDisable(GL_BLEND);
				}
				glPopMatrix();

				glColor3f(0, 0, 1);
				glLineWidth(1);
				//glBegin(GL_LINE_LOOP);
				//{
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].min_z);
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].min_z);
				//}
				//glEnd();
				glLineWidth(1); // 도착지 위치
			}
			else if (makeboard[i][j] == 4)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					remain();
				}
				glPopMatrix();

				glColor3f(0, 1, 0);
				//glLineWidth(1);
				//glBegin(GL_LINE_LOOP);
				//{
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].min_z);
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].min_z);
				//}
				//glEnd();
				//glLineWidth(1); // 아이템 위치
			}
			if (makeboard[i][j] == 5)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					Line_block();
				}
				glPopMatrix(); // 블럭 표시

				glColor3f(1, 0, 0);
				glLineWidth(1);
				//glBegin(GL_LINE_LOOP);
				//{
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].max_z);
				//	glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].min_z);
				//	glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].min_z);
				//}
				//glEnd();
				glLineWidth(1); // 블럭 위치
			}
		}
	}
}

void Plane()
{
	glPushMatrix();
	{
		glColor3d(0.7, 0.7, 0.7);
		glScaled(50, 1, 50);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void Reshape(int w1, int h1)
{
	PlaySound(TEXT(SOUND_FILE_NAME_BGM_1), NULL, SND_ASYNC | SND_SYNC);

	if (h1 == 0)
		h1 = 1;

	w = w1;
	h = h1;

	glutSetWindow(subWindow1);
	glutPositionWindow(border, border);
	glutReshapeWindow(w - 2 * border, h / 2 - border * 3 / 2);
	setProjection(w - 2 * border, h / 2 - border * 3 / 2);

	glutSetWindow(subWindow2);
	glutPositionWindow(border, (h + border) / 2);
	glutReshapeWindow(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
	setProjection(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);

	glutSetWindow(subWindow3);
	glutPositionWindow((w + border) / 2, (h + border) / 2);
	glutReshapeWindow(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
	setProjection(w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);

	color_change();
}

void setProjection(int w, int h)
{
	float ratio;

	ratio = 1.0f*w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h); // 윈도우 뷰포트

	gluPerspective(45, ratio, 0.1, 1000); // 1. 클리핑 공간 설정 원근투영
										  //glTranslatef(0.0, 1.5, -3.0); // 시야 확보

										  //glOrtho(0.0, w, 0.0, h, -1, 1); // 2. 클리핑 공간 설정 : 직각 투영

	glMatrixMode(GL_MODELVIEW);
} // Reshape == 화면 호출 때만 부르는거기에 한 번 만해도 족한 것들

void restorePerspectiveProjection()
{

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection()
{

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	gluOrtho2D(0, w, h, 0);

	glMatrixMode(GL_MODELVIEW);
}

// 메인메뉴 표시

void renderScene()
{
	glutSetWindow(mainWindow);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

// 모든 화면에 그려지는 랜더링씬
void renderScene2()
{
	if (deltaAngle)
	{
		computeDir(deltaAngle);
	}

	//effectonoff();

	gluLookAt(look_x, look_y, look_z, center_x, center_y, center_z, up_x, up_y, up_z);

	//drawxyz(); // 축그려주기

	glPushMatrix();
	{
		glRotatef(Light_rot, 0, 0, -1);
		Light_1();
	}
	glPopMatrix();

	Plane();
}

// 서브윈도우1
void renderScenesw1() {

	glutSetWindow(subWindow1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	if (hp_max != 0)
	{
		gluLookAt(x, y, z,
			x + lx, y + ly, z + lz,
			0.0f, 1.0f, 0.0f);
	}
	else
	{
		gluLookAt(x, y, z+100,
			x + lx, y + ly, z + lz,
			0.0f, 1.0f, 0.0f);
	}

	renderScene2();
	board_maker();

	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(x, y - 1.5, z - 3);
		glRotatef(180 - (angle + deltaAngle)*180.0 / 3.14, 0.0, 1.0, 0.0);
		Person();

		if (greedon == true)
		{
			glPushMatrix();
			{
				glTranslated(0, 0.3, 0);
				drawGreed();
			}
			glPopMatrix();
		}
	}
	glPopMatrix();


	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (makeboard[i][j] == 0) // 그냥 길...
			{
			}
			if (pass == false)
			{
				if (makeboard[i][j] == 1) // 벽 부딪치면 게임 out6
				{
					collision(static_block[i][j], i, j);
				}
			}
			if (makeboard[i][j] == 2) // 아이템 먹기
			{
				collision_item(static_block[i][j]);
				if (bool_item == true)
				{
					makeboard[i][j] = 0;
					bool_item = false;
				}
			}
			if (makeboard[i][j] == 3) // 게임 끝
			{
				collision_endline(static_block[i][j]);
				if (bool_item == true)
				{
					makeboard[i][j] = 0;
					bool_item = false;
				}

			}
			if (makeboard[i][j] == 5) // 그냥 길...
			{
				collision_sideline(static_block[i][j], i, j);
			}
		}
	} // 벽 부딪치는거 체크
	if (hp_max == 0)
	{
		light_1 = true;
		play_bool = false;
	}

	// 프레임 표시
	frame++;

	glColor3f(0.0, 1.0, 0.0);
	timing = glutGet(GLUT_ELAPSED_TIME);
	if (timing - timebase > 1000) {
		sprintf(s, "3D_MazeRunner - FPS:%4.2f",
			frame*1000.0 / (timing - timebase));
		timebase = timing;
		frame = 0;
	}
	glColor3f(1.0, 1.0, 1.0);

	setOrthographicProjection();

	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(5, 30, 0, GLUT_BITMAP_HELVETICA_12, s);
	glPopMatrix();

	restorePerspectiveProjection();

	glutSwapBuffers();
}

// 서브윈도우 2
void renderScenesw2() {

	glutSetWindow(subWindow2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

			gluLookAt(x, y + 3 + cheat, z,
				x, y - 1, z,
				lx, 0, lz);
	

	// 레드 콘 카메라
	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(x, y, z);
		glRotatef(180 - (angle + deltaAngle)*180.0 / 3.14, 0.0, 1.0, 0.0);
		glutSolidCone(0.2, 0.8f, 4, 4);
	}
	glPopMatrix();

	renderScene2();
	board_maker();

	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(x, y - 1.5, z - 3);
		glRotatef(180 - (angle + deltaAngle)*180.0 / 3.14, 0.0, 1.0, 0.0);
		Person();
		Lantern();

		if (greedon == true)
		{
			glPushMatrix();
			{
				glTranslated(0, 0.3, 0);
				drawGreed();
			}
			glPopMatrix();
		}
	}
	glPopMatrix();

	glutSwapBuffers();
}

// 서브윈도우 3
void renderScenesw3()
{
	glutSetWindow(subWindow3);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(
		0, 0, 200,
		0, 1, 0,
		0, 1, 0);

	// 레드 콘
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(x, y, z);
	glRotatef(180 - (angle + deltaAngle)*180.0 / 3.14, 0.0, 1.0, 0.0);
	glutSolidCone(0.2, 0.8f, 4, 4);
	glPopMatrix();

	renderScene2();

		letter_Play();
		letter_item();
		letter_time();

	initTextures();
	draw_hp();

	glEnable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

// 좌표계산
void computePos(float deltaMove)
{

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

// 전체 랜더링
void DrawScene()
{
	// 키보드 이동 체크
	if (deltaMove)
	{
		computePos(deltaMove);
		glutSetWindow(mainWindow);
		glutPostRedisplay();
	}

	renderScenesw1();
	renderScenesw2();
	renderScenesw3();
}



// -----------------------------------
//             TIME
// -----------------------------------

void TimerFunction(int value)
{
	if (replay_bool == false) // 실제 게임
	{
		if (play_bool == true)
		{
			saveFPS(x, z, deltaAngle, angle);
		}


		if (mainscore % 5 == 0) {
			color_change();
			if (mainscore > 0 && mainscore < 1000) {
				distribution = 0.5;
			}
			if (mainscore > 1000 && mainscore < 2000) {
				distribution = 0.65;
			}
			if (mainscore > 2000 && mainscore < 3000) {
				distribution = 0.8;
			}
			if (mainscore > 3000 && mainscore < 4000) {
				distribution = 0.95;
			}
		}

		glutPostRedisplay();   // 화면 재 출력 
		glutTimerFunc(timef, TimerFunction, 1); // 타이머함수 재 설정
	}
	else { // replay시 실행될 곳.
		x = (*it).first.first;
		z = (*it).first.second;
		deltaAngle = (*it).second.first;
		angle = (*it).second.second;
		if (pause == false) {
			if (state == true) {
				it++;
			}
			else {
				if (it != cont.begin()) {
					it--;
				}
			}
		}

		if (it == cont.end()) {
			startinit();

			printf("Replay 종료\n");

			UI();
		}


		if (mainscore % 5 == 0) {
			color_change();
			if (mainscore > 0 && mainscore < 1000) {
				distribution = 0.5;
			}
			if (mainscore > 1000 && mainscore < 2000) {
				distribution = 0.65;
			}
			if (mainscore > 2000 && mainscore < 3000) {
				distribution = 0.8;
			}
			if (mainscore > 3000 && mainscore < 4000) {
				distribution = 0.95;
			}
		}
		glutPostRedisplay();   // 화면 재 출력 
		glutTimerFunc(timef, TimerFunction, 1); // 타이머함수 재 설정
	}
} // 시간


  // -----------------------------------
  //             MAIN and INIT
  // -----------------------------------

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// 키보드 마우스 콜백
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutSpecialUpFunc(releaseKey);
	//glutMouseFunc(Mouse);
	//glutMotionFunc(MouseMove);
}

int main(int argc, char **argv) {

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			static_block[i][j].max_x += i*1.1;
			static_block[i][j].max_z += j*1.1;
			static_block[i][j].min_x += i*1.1;
			static_block[i][j].min_z += j*1.1;
		}
	} // 블럭 초기화

	UI();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	mainWindow = glutCreateWindow("컴그 최종프로젝트");
	glutTimerFunc(timef, TimerFunction, 1);

	// 윈도우
	glutDisplayFunc(renderScene);
	glutReshapeFunc(Reshape);
	glutIdleFunc(DrawScene);
	init();

	// 서브윈도우
	subWindow1 = glutCreateSubWindow(mainWindow, border, border, w - 2 * border, h / 2 - border * 3 / 2);
	glutDisplayFunc(renderScenesw1);
	init();

	subWindow2 = glutCreateSubWindow(mainWindow, border, (h + border) / 2, w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
	glutDisplayFunc(renderScenesw2);
	init();

	subWindow3 = glutCreateSubWindow(mainWindow, (w + border) / 2, (h + border) / 2, w / 2 - border * 3 / 2, h / 2 - border * 3 / 2);
	glutDisplayFunc(renderScenesw3);
	init();

	glutMainLoop();

	return 1;
}

void initTextures()
{
	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	pBytes = LoadDIBitmap("HP.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 81, 82, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
}

void Replay()
{
	FILE *fp;

		PlaySound(TEXT(SOUND_FILE_NAME_ON), NULL, SND_ASYNC | SND_SYNC); // 메뉴바 선택음...!
		fp = fopen(MAP_3, "rt");
		map = 3;
		//cout << map << endl;
	
	if (fp == NULL)
	{
		printf("\n실패\n");
	}

	printf("\n완료\n");

	int cha;

	while (feof(fp) == 0)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				fscanf(fp, "%d", &cha);
				makeboard[i][j] = cha;
			}
		}
	}
	fclose(fp);

	hp_max += 1;
}