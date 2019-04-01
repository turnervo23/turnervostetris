#include "UI.h"

UI::UI(Playfield* p) {
	playfield = p;
}

void UI::render() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	timeTexture.render(playfield->x - 150, playfield->y + 100);
}

void UI::setTime(int t) {
	time = t;

	//Convert to min/sec/ms
	int min = time / 60000;
	int sec = (time - min * 60000) / 1000;
	int ms = (time - min * 60000 - sec * 1000);

	//Format
	std::string minStr = std::to_string(min);
	while (minStr.size() < 2) {
		minStr = "0" + minStr;
	}
	std::string secStr = std::to_string(sec);
	while (secStr.size() < 2) {
		secStr = "0" + secStr;
	}
	std::string msStr = std::to_string(ms);
	while (msStr.size() < 3) {
		msStr = "0" + msStr;
	}

	timeStr = "Time: " + minStr + ":" + secStr + "." + msStr;
	if (min > 99) {
		timeStr = "Time: 99:59.999";
	}

	timeTexture.loadFromText(timeStr, font, textColor);
}

void UI::setFont(TTF_Font* f, SDL_Color c) {
	font = f;
	textColor = c;
}

void UI::update() {
}