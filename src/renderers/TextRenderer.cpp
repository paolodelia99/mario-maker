//
// Created by paolo on 12/02/21.
//
#include "TextRenderer.h"

TextRenderer::TextRenderer() {
    textFont = LoadFontEx(FONT_FILE_NAME, 16, 0, 250);
}

TextRenderer::~TextRenderer() {
    UnloadFont(textFont);
}

void TextRenderer::render(ECS::World *world) {
    for (auto ent : world->each<TextComponent>()) {
        auto textComponent = ent->get<TextComponent>();

        std::string s = std::to_string(textComponent->getValue());
        std::string additionalString = "";

        switch (textComponent->type) {
            case Text::Type::SCORE_COUNTER:
                for (int i = 0; i < 10 - s.length(); i++)
                    additionalString += "0";
                s = additionalString + s;
                break;
            case Text::Type::COIN_COUNTER:
                s = "x " + s;
                break;
            case Text::Type::TIMER:
                break;
            default:
                break;
        }

        const char *pchar = s.c_str();

        DrawTextEx(textFont, pchar, textComponent->position, textFont.baseSize, 1, WHITE);
    }
}
