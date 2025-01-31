
This is a simple Snake game, developed with Raylib, a library designed to make creating C++ video games easily.

This Snake Game have next rules:
. You must eat as many blueberries as possible.
. Every time you eat a blueberry, your score will increase.
. If the snake crashes into its own body or into the edges of the screen, you'll lose the game.
. if you lose your score will restart.
Controls:
. Arrow Up(move the snake up).
. Arrow Down(move the snake down).
. Arrow Left(move the snake left).
. Arrow Right(move the snake right).
Steps to set up and run the game on your system:
1. Clone the repository:
   git clone https://github.com/your-username/your-repo.git
   
3. Install Raylib: Follow the official Raylib documentation to install the library for your os.
   
5. complie the game:
   g++ -o snake_game snake_game.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

7. Run it:
   ./snake_game
