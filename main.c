#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <SDL/SDL_mixer.h>


void initialiser_audio(Mix_Music **music, const char *emplacement)
{
    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) == -1)
    {
        printf("Erreur Mix_OpenAudio: %s\n", Mix_GetError());
        return;
    }

    // Chargement de la musique
    *music = Mix_LoadMUS(emplacement);
    if (*music == NULL)
    {
        printf("Erreur chargement musique: %s\n", Mix_GetError());
        return;
    }

    // Lecture de la musique
    if (Mix_PlayMusic(*music, -1) == -1)
    {
        printf("Erreur lecture musique: %s\n", Mix_GetError());
        return;
    }

    // Réglage du volume
    Mix_VolumeMusic(100);
}

// Function to check if a point is inside a rectangle
int IsPointInRect(SDL_Rect *rect, int x, int y) {
    return (x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h);
}

void DrawTextBox(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, const char *text, SDL_Rect *textPos, SDL_Color boxColor) {
    // Render the text surface
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        printf("Text rendering error: %s\n", TTF_GetError());
        return;
    }

    // Create a rectangle for the background box
    SDL_Rect boxRect = {textPos->x - 10, textPos->y - 10, textSurface->w + 20, textSurface->h + 20};

    // Draw the background box
    SDL_FillRect(screen, &boxRect, SDL_MapRGB(screen->format, boxColor.r, boxColor.g, boxColor.b));

    // Draw the text on top of the box
    SDL_BlitSurface(textSurface, NULL, screen, textPos);

    // Free the text surface
    SDL_FreeSurface(textSurface);
}
Mix_Music *music;

int main(int argc, char *argv[]) {
    
    // Initialize SDL and SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("TTF Initialization Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Set video mode (window size: 1200x675)
    SDL_Surface *screen = SDL_SetVideoMode(731, 781, 32, SDL_SWSURFACE);
    if (!screen) {
        printf("Video Mode Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Load images
    SDL_Surface *background = IMG_Load("best.jpeg");
    SDL_Surface *yes_button = IMG_Load("yes_button.png");
    SDL_Surface *no_button = IMG_Load("no_button.png");

    if (!background || !yes_button || !no_button) {
        printf("Image Load Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("ARCADECLASSIC.TTF", 32);
    if (!font) {
        printf("Font Load Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Color textColor = {0, 100, 0};
    SDL_Color boxColor = {255, 255, 200};  // Light yellow color for the box

    // Positioning for initial screen
    SDL_Rect background_pos = {0, 0};
    SDL_Rect text_pos = {screen->w / 2 - 150, 200};  // Center the text horizontally (with some padding)
    SDL_Rect yes_pos = {screen->w / 2 - yes_button->w - 10, screen->h / 2 - 150};  // Center horizontally and a bit lower vertically
    SDL_Rect no_pos = {screen->w / 2 + 10, screen->h / 2 - 150};  // Center horizontally and a bit lower vertically

    // Draw the initial screen
    SDL_BlitSurface(background, NULL, screen, &background_pos);

    // Draw the message with the box around it
    DrawTextBox(screen, font, textColor, "Do you want to save the game?", &text_pos, boxColor);
    SDL_BlitSurface(yes_button, NULL, screen, &yes_pos);
    SDL_BlitSurface(no_button, NULL, screen, &no_pos);
    SDL_Flip(screen);

    int running = 1;
    SDL_Event event;
    int showNewButtons = 0;
    initialiser_audio(&music, "music.mp3");

    // Event loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Check if the 'Yes' button was clicked
                if (!showNewButtons && event.button.button == SDL_BUTTON_LEFT) {
                    if (IsPointInRect(&yes_pos, event.button.x, event.button.y)) {
                        showNewButtons = 1;
                    }
                }
            }
        }

        // Update screen when necessary
        // New positioning for the buttons after "Yes" is clicked
		SDL_Rect charger_pos = {screen->w / 2 - 150, screen->h / 2 - 100};  // Center "Charger Partie"
	SDL_Rect nouvelle_pos = {screen->w / 2 - 150, screen->h / 2};  // Center "Nouvelle Partie"

	// Update the screen when necessary
	if (showNewButtons) {
	    // Clear the screen (or just redraw relevant parts)
	    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

	    // Draw the background again
	    SDL_BlitSurface(background, NULL, screen, &background_pos);

	    // Draw new buttons "Charger Partie" and "Nouvelle Partie" with boxes
	    DrawTextBox(screen, font, textColor, "Charger Partie", &charger_pos, boxColor);
	    DrawTextBox(screen, font, textColor, "Nouvelle Partie", &nouvelle_pos, boxColor);
	}


        SDL_Flip(screen);
    }

    // Cleanup
    SDL_FreeSurface(background);
    SDL_FreeSurface(yes_button);
    SDL_FreeSurface(no_button);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

