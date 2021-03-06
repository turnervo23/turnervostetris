#include "Texture.h"

extern SDL_Renderer* gRenderer;

Texture::Texture() {
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture() {
	free();
}

//Loads texture from file, returns false if error
bool Texture::loadFromFile(std::string path){
	//deallocate old texture
	free();
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == NULL) {
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	//Set standard alpha blending
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

	//Get image dimensions
	width = loadedSurface->w;
	height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	texture = newTexture;

	return (texture != NULL);
}

bool Texture::loadFromText(std::string text, TTF_Font* font, SDL_Color color) {
	free();
	SDL_Texture* newTexture = NULL;
	
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, 1000);
	if (textSurface == NULL) {
		printf("Unable to create text surface! TTF Error: %s\n", TTF_GetError());
		return false;
	}
	newTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	if (newTexture == NULL) {
		printf("Unable to create text texture! SDL Error: %s\n", SDL_GetError());
	}
	width = textSurface->w;
	height = textSurface->h;
	SDL_FreeSurface(textSurface);
	texture = newTexture;
	return (texture != NULL);
}

void Texture::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

//Render with default width/height
void Texture::render(int x, int y) {
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);
}

//Render with specified width/height
void Texture::render(int x, int y, int w, int h) {
	SDL_Rect renderQuad = { x, y, w, h };
	SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);
}

//Set alpha of texture
void Texture::setAlpha(int alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);
}

//Sets color of texture. Used to take turn 1 block texture into 7
void Texture::setColor(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(texture, r, g, b);
}