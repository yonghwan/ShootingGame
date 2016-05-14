#include "Controller.h"
#include "TFHandler.h"
#include "SDL/SDL.h"
#include <cstring>

class HiScoreController: public Controller {

	public: 
	HiScoreController(int score);
	int step(int input,SDL_Surface* screen);
	~HiScoreController();
	
	protected:
	int score;
	SDL_Surface* tiles;
	SDL_Surface* text;
	SDL_Surface* select;
	TFHandler* typeface;
	std::string** scoreNames;
	int* scoreValues;
	bool isHiScore;
	int playerRank;
	int mode;
	int time;
	
	int rect_minx;
	int rect_maxx;
	int rect_miny;
	int rect_maxy;
	int selectX;
	int selectY;
	int cooldown;
	SDL_Rect clipRect;
	SDL_Rect posRect;
	
	
	
	static const int num_hiscores;
	static const int HSTable_minx;
	static const int HSTable_maxx;
	static const int HSTable_miny;
	static const int HSTable_maxy;
	
	static const int HSTable_namexpos;
	static const int HSTable_scorexpos;
	static const int HSTable_listypos;
	
	static const int entryX;
	static const int entryY;
	static const int maxNameLen;
	
	static const char* alph;
	static const char* numer;
};
