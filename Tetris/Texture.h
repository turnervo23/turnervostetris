//Texture.h - texture wrapper class
#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

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
	void setAlpha(int alpha);
	void setColor(Uint8 r, Uint8 g, Uint8 b);
private:
	SDL_Texture* texture; //actual texture
	int width, height;
};

#endif