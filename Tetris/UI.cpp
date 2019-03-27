#include "UI.h"

UI::UI() {

}

void UI::render() {
	timeTexture.render(200, 200);
}

void UI::setTime(int t) {
	time = t;
	timeTexture.loadFromText(std::to_string(t), font, textColor);
}

void UI::setFont(TTF_Font* f) {
	font = f;
}

void UI::setFontColor(SDL_Color c) {
	textColor = c;
}

void UI::update() {
	render();
}