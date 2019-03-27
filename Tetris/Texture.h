//Texture.h - texture wrapper class

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

extern SDL_Renderer* gRenderer;
const int DEFAULT = -1; //used to detect optional parameters

class Texture {
	friend class Playfield;
	friend class Player;
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path);
	bool loadFromText(std::string text, TTF_Font* font, SDL_Color textColor);
	void free();
	void render(int x, int y);
	void render(int x, int y, int w, int h);
private:
	SDL_Texture* texture; //actual texture
	int width, height;
};