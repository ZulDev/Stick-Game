#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>

int live = 100;
int live_2 = 1000;
int power_player1 = 1;

/// Variable Player 1
float PX_1,PY_1; /// Coordinate of PLayer 1
int Player1_live = live;
bool Player1_isAlive = true;
bool Player1_isWin = false;
///float BulletX_1,BulletY_1; /// Coordinate Bullet for player 1

/// Variable Player 2
float PX_2,PY_2; /// Coordinate PLayer 2
int Player2_live = live_2;
bool Player2_isAlive = true;
bool Player2_isWin = false;
///float BulletX_2,BulletY_2; /// Coordinate Bullet for player 2

/// System Logic and other

bool isHome = true;
bool can_jump_player1 = false; /// For player 1
bool can_jump_player2 = false; /// For player 2
bool inPlat = false; /// New
bool doOnce = false; /// Random plat time.
float getTime = float(); /// Hitung waktu
bool only_one = true;
int getTimeINT = 0; /// get Time but int
int HandPlatX = 240;
int HandPlatY = 322;
///int HandPLatXARR[2] = {HandPlatX,-HandPlatX};
int MAX_PLAT = 20; /// PLat Size

float gravity = 0.2, dy= 0,dy_2 = 0;///ground = 370 - 95; /// Handle Jump

/// Width and height the window | Game
const int width = 800;
const int height = 370;

/// Old -> struct platform{int x,y;};
struct platform {
    int x[20], y[20];
};

/// Function
void Keyboard_Player1(bool &,bool &,float &, sf::Sound &);
void Keyboard_Player2(bool &,bool &,float &, sf::Sound &);
void Logic_Plat(sf::Sprite &, sf::Sprite &, platform &);
void Logic_Bullet(sf::Sprite &, sf::Sprite &,bool &,
                  bool &, float &, float &, float &,
                  float &, sf::Sound &, bool &, bool &);
void UpdateMovementPlayer1();
void UpdateMovementPlayer2();
void UpdatePlayer1_BulletPos(sf::Sprite &,float &,float &, bool &);
void UpdatePlayer2_BulletPos(sf::Sprite &,float &,float &, bool &);
void GeneratePosPlat(platform &);
void printStatistic_Player1(platform &);
void printStatistic_Player2(platform &);
void draw_plat(sf::Sprite &, platform &, sf::RenderWindow &);
void printPosMouse(sf::Vector2i &,sf::RenderWindow &);


int main()
{
    srand(time(0));

    /// Generate a random value for position player 1.
    int randomValue = rand() % 9;
    /// Generate a random number for picking position x of plat
    int PickPosPlatX = rand() % 2;

    /// Load Sound Effect
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer dieBuffer;
    shootBuffer.loadFromFile("audios/shootAudio.wav");
    dieBuffer.loadFromFile("audios/dieSound.wav");

    sf::Sound shootSound(shootBuffer);
    sf::Sound dieSound(dieBuffer);

    /// Load music_home / audio
    sf::Music music_home;
    if(!music_home.openFromFile("audios/music1.wav")) std::cerr << "Can't Load The music Audio!\n";

    /// Load Texture From images
    sf::Texture tPlayer1,tHomeView,tPlat,tPlayer2,tBG;

    tPlayer1.loadFromFile("images/Player.png");  /// player
    tPlayer2.loadFromFile("images/Enemy.png"); /// Musuh
    tHomeView.loadFromFile("images/homeView.png"); /// home
    tPlat.loadFromFile("images/plat.png");         /// Platform
    tBG.loadFromFile("images/bg2.png");

    sf::Sprite sPlayer1(tPlayer1),sHomeView(tHomeView),sPlat(tPlat),sPlayer2(tPlayer2),sBG(tBG);

    /// WIN DRAW FOR PLAYER 1 | PLAYER 2
    sf::Texture tWINP1,tWINP2,tDRAW;
    tWINP1.loadFromFile("images/player1win.png");
    tWINP2.loadFromFile("images/player2win.png");
    tDRAW.loadFromFile("images/drawTitle.png");

    sf::Sprite sWINP1(tWINP1),sWINP2(tWINP2),sDRAW(tDRAW);

    /// Buat objek si PLat | Coordinate | deklarasi
    platform plat;

    /// Call Func to generate the position of platform
    GeneratePosPlat(plat);

    /// initialize with random And initialiase player 1 and 2

    PX_1 = plat.x[1] + 12;
    PY_1 = plat.y[1] - 47;

    PX_2 = plat.x[14] + 12;
    PY_2 = plat.y[14] - 47;

    ///sPlayer1.setPosition(PX_1,PY_1); /// Random Coordinate plat.
    ///sPlayer2.setPosition(PX_2,PY_2); /// Last PLat coordinat


    /// New Later hapus maybe | testing
    /// Bullet For Player 1
    sf::CircleShape Bullet1(2.5f);
    Bullet1.setFillColor(sf::Color::Black);
    Bullet1.setOrigin(0,2.5f); /// Kiri Tngh ujung.
    /// Bullet For Player 2
    sf::CircleShape Bullet2(200.0f);
    Bullet2.setFillColor(sf::Color::Black);
    Bullet2.setOrigin(0,200.0f); /// Kiri Tngh ujung.
    /// ///////////////////////////////////////////////

    /// New
    float BulletX_1,BulletY_1; /// Coordinate Bullet for player 1
    float BulletX_2,BulletY_2; /// Coordinate Bullet for player 2
    bool Player1_isShot = false;
    bool Player2_isShot = false;
    bool Player1_canShot = true;
    bool Player2_canShot = true;
    /// initialize value for player 1.
    BulletX_1 = PX_1 + 37;
    BulletY_1 = PY_1 + 18.67;

    /// initialize value for player 2.
    BulletX_2 = PX_2 - 4;
    BulletY_2 = PY_2 + 18;

    /// ///////////////////

    /// The Engine.
    sf::RenderWindow window(sf::VideoMode(width,height), "Mini Stick Game");
    window.setFramerateLimit(60);

    sf::Clock clock;

    while(window.isOpen())
    {
        sf::Time times = clock.restart();
        getTime += times.asSeconds();
        getTimeINT = getTime;

        sf::Event event;


        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonReleased:
                    /*if(isHome == true)
                    /{
                        music_home.setLoop(true);
                        music_home.play();
                    }*/
                    //else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && (isHome == false)) music_home.stop();
                    break;
                case sf::Event::KeyPressed:
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
                    break;
                case sf::Event::KeyReleased:
                    /// If Player Release The Key for Shoot Then set to false.
                    if(BulletX_1 >= 800) {
                        Player1_isShot = false;
                        Player1_canShot = true;
                        only_one = true;
                    }
                    if(BulletX_2 <= 0) {
                        Player2_isShot = false;
                        Player2_canShot = true;
                    }

                    break;
                default:
                    break;
            }
        }
        if (isHome) {
            //music_home.setLoop(true);
            //music_home.play();
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            /// If user Click button PLayed.
            if((mousePos.x >= 292 && mousePos.x <= 495) &&
               (mousePos.y >= 125 && mousePos.y <= 185) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) isHome = false;

            /// If user Click button Exit
            if((mousePos.x >= 285 && mousePos.x <= 505) &&
               (mousePos.y >= 205 && mousePos.y <= 260) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {window.close(); break;}

            printPosMouse(mousePos,window); /// Print the position Mouse now.

            window.clear(sf::Color::White);
            window.draw(sHomeView);
            window.display();
        }
        else
        {
            if(!Player1_isWin && !Player2_isWin)
            {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                printPosMouse(mousePos,window);

                srand(time(0));
                music_home.stop();

                Keyboard_Player1(Player1_canShot,Player1_isShot,BulletX_1,shootSound); /// Handle For Player 1 if keyboard is pressed

                Keyboard_Player2(Player2_canShot,Player2_isShot,BulletX_2,shootSound); /// Handle For Player 2 if keyboard is pressed

                Logic_Plat(sPlayer1,sPlayer2,plat);

                printStatistic_Player1(plat); /// Print some info statistic about player 1;
                printStatistic_Player2(plat); /// Print some info statistic about player 2;

                window.clear(sf::Color::White);
                window.draw(sBG);

                draw_plat(sPlat,plat,window);

                sPlayer1.setPosition(PX_1,PY_1);
                sPlayer2.setPosition(PX_2,PY_2);
                Bullet1.setPosition(BulletX_1,BulletY_1);
                Bullet2.setPosition(BulletX_2,BulletY_2);
                Logic_Bullet(sPlayer1,sPlayer2,Player1_isShot,Player2_isShot,
                             BulletX_1,BulletY_1,BulletX_2,BulletY_2,dieSound,Player1_canShot,Player2_canShot); /// Call Bullet Logic
                if(Player1_isAlive) window.draw(sPlayer1);
                if(Player2_isAlive) window.draw(sPlayer2);
                if(Player1_isShot) window.draw(Bullet1);
                if(Player2_isShot) window.draw(Bullet2);
                UpdateMovementPlayer1(); /// Movement Player 1
                UpdateMovementPlayer2(); /// Movement Player 2
                UpdatePlayer1_BulletPos(sPlayer1,BulletX_1,BulletY_1,Player1_isShot);
                UpdatePlayer2_BulletPos(sPlayer2,BulletX_2,BulletY_2,Player2_isShot);
                window.display();
            }
            else {
                std::cout << "Darah Player 1 : " << Player1_live << std::endl;
                std::cout << "Darah Player 2 : " << Player2_live << std::endl;
                if(Player1_isWin) {
                    window.clear(sf::Color::White);
                    window.draw(sWINP1);
                    window.display();
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        power_player1 = 1;
                        Player1_isWin = false;
                        Player2_isAlive = true;
                        Player1_live = live;
                        Player2_live = live_2;
                        sPlayer1.setPosition(PX_1 = 0,PY_1 = 0);
                        sPlayer2.setPosition(PX_2 = width - 36,PY_2 = 0);
                    }

                }
                else if(Player2_isWin) {
                    window.clear(sf::Color::White);
                    window.draw(sWINP2);
                    window.display();
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        power_player1 = 1;
                        Player2_isWin = false;
                        Player1_isAlive = true;
                        Player1_live = live;
                        Player2_live = live_2;
                        sPlayer1.setPosition(PX_1 = 0,PY_1 = 0);
                        sPlayer2.setPosition(PX_2 = width - 36,PY_2 = 0);
                    }
                }
                /// Draw
                else {
                    window.clear(sf::Color::White);
                    window.draw(sDRAW);
                    window.display();
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        power_player1 = 1;
                        Player2_isWin = false;
                        Player1_isAlive = true;
                        Player1_live = live;
                        Player2_live = live_2;
                        sPlayer1.setPosition(PX_1 = 0,PY_1 = 0);
                        sPlayer2.setPosition(PX_2 = width - 36,PY_2 = 0);
                    }
                }

                ///window.close();
            }
        }
    }
    std::cout << "Tekan Apa saja untuk keluar!\n";
    std::cin.get();
    return 0;
}
/// Prototipe function
void Keyboard_Player1(bool &Player1_canShot,bool &Player1_isShot, float &BulletX_1,sf::Sound &shootSound) {
    /// Movement for Player 1
        float randAD[2] = {1.5,-1.5}; int getRand = rand() % 2;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) PX_1 -= randAD[getRand]; /// + + = +, - + = -; logic
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) PX_1 -= 1.5f;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) PX_1 += 1.5f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (can_jump_player1)) {
                dy = 2.5f; /// this
                dy -= 10.0f;
                can_jump_player1 = false;
            }

        /// Shoot
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if(Player1_canShot/* && only_one*/) {
                std::cout << "Player 1 Shoot!\n";
                shootSound.play();
                Player1_isShot = true;
                Player1_canShot = false;
                //only_one = false;
            }
        }

        /// Developer Mode
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) power_player1 += 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) power_player1 -= 1;
}

void Keyboard_Player2(bool &Player2_canShot,bool &Player2_isShot, float &BulletX_2,sf::Sound &shootSound) {
    /// Movement For Player 2
        float randAD_2[2] = {1.5,-1.5}; int getRand_2 = rand() % 2;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) PX_2 -= randAD_2[getRand_2]; /// + + = +, - + = -; logic
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) PX_2 -= 1.5f;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) PX_2 += 1.5f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (can_jump_player2)) {
                dy_2 = 2.5f; /// this
                dy_2 -= 10.0f;
                can_jump_player2 = false;
            }

        /// Shoot
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            if(Player2_canShot) {
                std::cout << "Player 2 Shoot!\n";
                shootSound.play();
                Player2_isShot = true;
                Player2_canShot = false;
            }
        }
}

void Logic_Plat(sf::Sprite &sPlayer1,sf::Sprite &sPlayer2,platform &plat) {
    /// LOGIC PLAT New
        for(unsigned short int i = 0; i < MAX_PLAT; i++) {
            if((PX_1 + 22 > plat.x[i])  && (PX_1 < plat.x[i] + 66) && (PY_1 + 42.5 > plat.y[i])  && (PY_1 + 42.5 < plat.y[i] + 10) && (dy > 0))
            {
                dy = 0.0;
                PY_1 = sPlayer1.getPosition().y;
                can_jump_player1 = true;
            }

            if((PX_2 + 30 > plat.x[i])  && (PX_2 < plat.x[i] + 50.5) && (PY_2 + 42.5 > plat.y[i])  && (PY_2 + 42.5 < plat.y[i] + 10) && (dy_2 > 0))
            {
                dy_2 = 0.0;
                PY_2 = sPlayer2.getPosition().y;
                can_jump_player2 = true;
            }
        }
}

void Logic_Bullet(sf::Sprite &sPlayer1, sf::Sprite &sPlayer2, bool &Player1_isShot,
                  bool &Player2_isShot, float &BulletX_1, float &BulletY_1, float &BulletX_2,
                float &BulletY_2, sf::Sound &dieSound, bool &Player1_canShot, bool &Player2_canShot) {
    /// Cek Apakah Si bullet Mengenai si player di antara kepala sampai kaki.

    /// Bullet Player 1 mengenai si player 2
    if((BulletX_1 >= sPlayer2.getPosition().x + 14 && BulletX_1 <= sPlayer2.getPosition().x + 35) &&
       (BulletY_1 >= sPlayer2.getPosition().y && BulletY_1 <= sPlayer2.getPosition().y + 46) && (Player1_isShot))
            {
                std::cout << "Tembakan si player 1 Mengenai Player 2!\n";
                dieSound.play();
                Player2_live -= power_player1;
                std::cout << "Darah Player 2 : " << Player2_live << "\n";
                BulletX_1 = 800;
                Player1_isShot = false;
                Player1_canShot = true;
                if(Player2_live <= 0) {
                    Player2_isAlive = false;
                    Player1_isWin = true;
                }
            }

    /// Bullet Player 2 mengenai si player 1
    if((BulletX_2 >= sPlayer1.getPosition().x && BulletX_2 <= sPlayer1.getPosition().x + 20) &&
       (BulletY_2 >= sPlayer1.getPosition().y && BulletY_2 <= sPlayer1.getPosition().y + 46) && (Player2_isShot))
            {
                std::cout << "Tembakan si player 2 Mengenai Player 1!\n";
                dieSound.play();
                Player1_live--;
                std::cout << "Darah Player 1 : " << Player1_live << "\n";
                BulletX_2 = 0;
                Player2_isShot = false;
                Player2_canShot = true;
                if(Player1_live <= 0) {
                    Player1_isAlive = false;
                    Player2_isWin = true;
                }
            }

}

void UpdateMovementPlayer1() {
    /// For Player system movement.
    if(PX_1 <= 0) PX_1 = 0;
    else if(PX_1 >= 726) PX_1 = 726;

    if(PY_1 <= 0.0f) {PY_1 = 0.0f; dy = 0;}


    dy += gravity;

    PY_1 += dy;

    ///if(inPlat);

    if(dy >= 5.0f) dy = 5.0f; /// Jika dy sudah mencapai 5 maka di hentikan.
    /// High Jump -> if(dy >= 2.0f) dy = -10.0f;

    /// //if(PY_1 >= 275.5) {PY_1 = 275.5; can_jump_player1 = true;}
    if(PY_1 >= 323) {PY_1 = 323; can_jump_player1 = true;}

}

void UpdateMovementPlayer2() {
    /// For Player system movement.
    if(PX_2 <= 39.5) PX_2 = 39.5;
    else if(PX_2 >= 764) PX_2 = 764;

    if(PY_2 <= 0.0f) {PY_2 = 0.0f; dy_2 = 0;}

    /// Handel later si dy and gravity si player 2
    dy_2 += gravity;

    PY_2 += dy_2;

    ///if(inPlat);

    if(dy_2 >= 5.0f) dy_2 = 5.0f; /// Jika dy sudah mencapai 5 maka di hentikan.

    if(PY_2 >= 323) {PY_2 = 323; can_jump_player2 = true;}
}

void UpdatePlayer1_BulletPos(sf::Sprite &sPLayer1, float &BulletX_1, float &BulletY_1, bool &Player1_isShot) {
    if(Player1_isShot) {
        BulletX_1 += 10;
    } else {
        BulletX_1 = sPLayer1.getPosition().x + 37;
        BulletY_1 = sPLayer1.getPosition().y + 18.67;
    }
}

void UpdatePlayer2_BulletPos(sf::Sprite &sPlayer2, float &BulletX_2, float &BulletY_2, bool &Player2_isShot) {
    if(Player2_isShot) {
        BulletX_2 -= 10;
    } else {
        BulletX_2 = sPlayer2.getPosition().x - 6;
        BulletY_2 = sPlayer2.getPosition().y + 18;
    }
}

void GeneratePosPlat(platform &obj) {
    /// Generated Random kordinat untuk si plat.
    for(unsigned short int i = 0; i < MAX_PLAT; i++)
    {
        obj.x[i] = rand() % 764;
        obj.y[i] = rand() % 370;
    }

}

void printStatistic_Player1(platform &obj) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        std::cout << "Position PX_1   : " << PX_1 << std::endl;
        std::cout << "Position PY_1   : " << PY_1 << std::endl;
        std::cout << "Value of dy     : " << dy << std::endl;
        std::cout << "can_jump_player1: " << can_jump_player1 << std::endl;
        std::cout << "Nilai PlatX indek ke [" << 1 << "] = " << obj.x[1] << std::endl;
        std::cout << "Nilai PlatY indek ke [" << 1 << "] = " << obj.y[1] << std::endl;
        //std::cout << "HandPlatY      : " << HandPlatY << std::endl;
        ///std::cout << "RandValue       : " << randomValue << std::endl;
        ///std::cout << "Nilai PlatX indek ke [" << randomValue << "] = " << plat.x[randomValue] << std::endl;
        ///std::cout << "Nilai PlatY indek ke [" << randomValue << "] = " << plat.y[randomValue] << std::endl;
        }
}

void printStatistic_Player2(platform &obj) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
        std::cout << "Position PX_2   : " << PX_2 << std::endl;
        std::cout << "Position PY_2   : " << PY_2 << std::endl;
        std::cout << "Value of dy_2   : " << dy_2 << std::endl;
        std::cout << "can_jump_player2: " << can_jump_player2 << std::endl;
        std::cout << "Nilai PlatX indek ke [" << 14 << "] = " << obj.x[14] << std::endl;
        std::cout << "Nilai PlatY indek ke [" << 14 << "] = " << obj.y[14] << std::endl;
        //std::cout << "HandPlatY      : " << HandPlatY << std::endl;
        ///std::cout << "RandValue       : " << randomValue << std::endl;
        ///std::cout << "Nilai PlatX indek ke [" << randomValue << "] = " << plat.x[randomValue] << std::endl;
        ///std::cout << "Nilai PlatY indek ke [" << randomValue << "] = " << plat.y[randomValue] << std::endl;
        }
}

void draw_plat(sf::Sprite &sPlat, platform &plat, sf::RenderWindow &window) {
    /// New Draw -> The Platform
        for(unsigned short int i = 0; i < MAX_PLAT; i++) {
            ///if(plat.y[i] <= 65) plat.y[i] = 40 + rand() % height ;  ///plat.y[i] = 300;
            ///if(plat.y[i] <= 65) {plat.y[i] = 322; plat.x[i] = HandPlatX; HandPlatX += 40;}
            if(getTimeINT % 5 == 0) {
                getTime = 1;
                plat.x[i] = rand() % width - 66;
                plat.y[i] = 40 + rand() % height;
                ///if(plat.y[i] <= 65) plat.y[i] = 40 + rand() % height;
            }

            if(i == 0) {
                sPlat.setPosition(plat.x[i] = 215,plat.y[i] = 322);
            }
            /// Last Plat
            if(i == MAX_PLAT - 1) {
                /// sPlayer2.setPosition(plat.x[i] + 12,plat.y[i] - 47); /// Spawn PLayer 2
                HandPlatY = 322;
            }
            sPlat.setPosition(plat.x[i],plat.y[i]);
            window.draw(sPlat);

            /// Sebelum Last PLat
            if(i < MAX_PLAT - 2) {
                ///srand(time(0));
                /// ///HandPlatX = 215 - HandPLatXARR[PickPosPlatX];
                HandPlatY -= 35;
                plat.y[i + 1] = HandPlatY;
                ///plat.x[i + 1] = HandPlatX;
            }
        }
}

void printPosMouse(sf::Vector2i &mousePos,sf::RenderWindow &obj) {
    mousePos = sf::Mouse::getPosition(obj);

    /// Print cordinate player
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {std::cout << "Position x : " << mousePos.x << std::endl;
    std::cout << "Position y : " << mousePos.y << std::endl;
    std::cout << "GetTime : " << getTime << std::endl;}
}

