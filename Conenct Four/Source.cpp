#include <SFML/Graphics.hpp>

// ' ' - empty; 'r' - red; 'y' - yellow; 'w' - one of the connected 4
char board[6][7];

int column, row;

int score[2] = {0, 0};
char turn = 'r';

int scoreboardSize = 65;
int HEIGHT = 600 + scoreboardSize;
int WIDTH = 700;
sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Connect Four");

void resetBoard()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++) board[i][j] = ' ';
    }
}

void drawScoreboard()
{
    // separate from the rest of the field
    sf::RectangleShape rect;

    rect.setPosition(0, scoreboardSize - 3);
    rect.setSize(sf::Vector2f(WIDTH, 3));
    rect.setFillColor(sf::Color::Black);
    window.draw(rect);

    // icons
    sf::CircleShape circ;
    circ.setRadius(25);
    circ.setPosition(200, 5);
    circ.setFillColor(sf::Color::Red);
    window.draw(circ);

    circ.setPosition(450, 5);
    circ.setFillColor(sf::Color::Yellow);
    window.draw(circ);

    // score
    sf::Text text;

    std::string s1 = std::to_string(score[0]);
    std::string s2 = std::to_string(score[1]);

    text.setString(s1 + " : " + s2);

    sf::Font font;
    font.loadFromFile("Fonts/SF_Arch_Rival.ttf");
    text.setFont(font);
    text.setPosition(325, 10);

    window.draw(text);
}

void drawBoard()
{
    window.clear(sf::Color(39, 56, 73));

    drawScoreboard();

    // highlights column with mouse on it
    sf::RectangleShape rect;

    rect.setPosition(column * 100, scoreboardSize);
    rect.setSize(sf::Vector2f(100, HEIGHT - scoreboardSize));
    rect.setFillColor(sf::Color(45, 60, 80));

    window.draw(rect);

    // circles
    sf::CircleShape circ;
    sf::Vector2f circlePosition(5, 5 + scoreboardSize);
    circ.setRadius(45);

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            circ.setOutlineThickness(0);
            int transparency = 255;
            if (i == row && j == column) transparency = 175;

            if (board[i][j] == ' ')         circ.setFillColor(sf::Color(15, 25, 30)); // empty
            else if (board[i][j] == 'r')    circ.setFillColor(sf::Color(235, 56, 70, transparency)); // red
            else if (board[i][j] == 'y')    circ.setFillColor(sf::Color(245, 211, 75, transparency)); // yellow
      
            else if (board[i][j] == 'w' && turn == 'r')
            {
                circ.setFillColor(sf::Color(245, 211, 75));
                circ.setOutlineThickness(4);
                circ.setOutlineColor(sf::Color(235, 56+20, 70+20));
            }
            else if (board[i][j] == 'w' && turn == 'y')
            {
                circ.setFillColor(sf::Color(235, 56, 70));
                circ.setOutlineThickness(3);
                circ.setOutlineColor(sf::Color(255, 213, 128));
            }

            circ.setPosition(circlePosition);
            window.draw(circ);

            circlePosition.x += 100;
        }
        circlePosition.x = 5;
        circlePosition.y += 100;
    }
    window.display();
}

bool win() {
    // row win
    int counter = 0;
    for (int j = 0; j < 7; j++)
    {
        if (board[row][j] == turn)
        {
            board[row][j] = 'w';
            counter++;
        }
        else
        {
            counter = 0;
            for (int i = 0; i <= j; i++)
                if (board[row][i] == 'w') board[row][i] = turn;
        }
        if (counter == 4) return true;
    }
    for (int j = 0; j < 7; j++)
        if (board[row][j] == 'w') board[row][j] = turn;

    // column win
    counter = 0;
    for (int i = 0; i < 6; i++)
    {
        if (board[i][column] == turn)
        {
            board[i][column] = 'w';
            counter++;
        }
        else
        {
            counter = 0;
            for (int j = 0; j <= i; j++)
                if (board[j][column] == 'w') board[j][column] = turn;
        }
        if (counter == 4) return true;
    }
    for (int i = 0; i < 6; i++)
        if (board[i][column] == 'w') board[i][column] = turn;

    // 1st diagonal win
    counter = 1;
    board[row][column] = 'w';
    for (int i = 1; (row - i >= 0) && (column - i >= 0); i++)
    {
        if (board[row - i][column - i] == turn)
        {
            board[row - i][column - i] = 'w';
            counter++;
        } 
        else break;
    }

    for (int i = 1; (row + i < 6) && (column + i < 7); i++)
    {
        if (board[row + i][column + i] == turn)
        {
            board[row + i][column + i] = 'w';
            counter++;
        } 
        else break;
    }
    if (counter >= 4) return true;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
            if (board[i][j] == 'w') board[i][j] = turn;
    }

    // 2nd diagonal win
    counter = 1;
    board[row][column] = 'w';
    for (int i = 1; (row - i >= 0) && (column + i < 7); i++)
    {
        if (board[row - i][column + i] == turn) 
        {
            counter++;
            board[row - i][column + i] == 'w';
        }
        else break;
    }

    for (int i = 1; (row + i < 6) && (column - i >= 0); i++)
    {
        if (board[row + i][column - i] == turn)
        {
            counter++;
            board[row + i][column - i] = 'w';
        } 
        else break;
    }
    if (counter >= 4) return true;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
            if (board[i][j] == 'w') board[i][j] = turn;
    }
    // else
    return false;
}

bool draw() {

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++)
            if (board[i][j] == ' ') return false;
    }

    return true;
}

int main() {
    window.setFramerateLimit(30);

    bool gameOver = false;

    resetBoard();

    sf::Clock clock;
    sf::Time moveDelay = sf::seconds(0.25);
    sf::Time newGamedelay = sf::seconds(2.5);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) window.close();
        }


        if (!gameOver) {

            bool moved = false;

            sf::Time elapsed = clock.getElapsedTime();

            // mouse clicked
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && elapsed >= moveDelay && row >= 0) {

                moved = true;
                clock.restart();

                if (win()) {
                    gameOver = true;

                    if (turn == 'r') score[0]++;
                    else score[1]++;
                }
                if (draw())
                    gameOver = true;

                else if (turn == 'r') turn = 'y';
                else if (turn == 'y') turn = 'r';
            }

            if (!moved && row >= 0)   board[row][column] = ' ';


            int mouseX = sf::Mouse::getPosition(window).x;
            column = mouseX / 100;

            row = 5;
            while (board[row][column] != ' ')
                row--;

            for (int i = 0; i < 7; i++)
                if (i == column && row >= 0 && !gameOver)    board[row][i] = turn;

            drawBoard();

            continue;
        }

        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed >= newGamedelay) {
            resetBoard();
            gameOver = false;
        }

    }

    return 0;
}