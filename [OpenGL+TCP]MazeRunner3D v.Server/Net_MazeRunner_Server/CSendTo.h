#pragma once
#include "CForServer.h"

class CSendTo
{
private:
	ForPingPong S_Server_Data;
public:
	bool SendPlayerInfo(SOCKET& sock);
	bool SendMapInfo(SOCKET& sock);
	bool SendGhostInfo(SOCKET& sock);
	bool SendOtherInfo(SOCKET& sock);

	void get_maze(int i, int j, int get_map)
	{
		S_Server_Data.MazeArray[i][j].iStatus = get_map;
	}// 이거 왜만든거??

	Maze Get_Maze(int X, int Y)
	{
		return S_Server_Data.MazeArray[X][Y];
	}
	
	
	void set_forpingpong(ForPingPong*);
	ForPingPong* get_forpingpong(void);
	
	void Set_Player(CPlayer*, int);
	void Set_Maze(CMaze*, int x, int y);
	void Set_Ghost(CGhost*, int);
	void Set_Connect(bool, int);

	bool Get_Connect(int);

	Player Get_Player(int PlayerN)
	{
		return S_Server_Data.PlayerArray[PlayerN];
	}
public:
	CSendTo();
	~CSendTo();
};

