#include "_score.h"

_score::_score()
{
    //ctor
    myText->textInit("images/fonts/numEtc.png",9,3);
    playerScore = 0;
}

_score::~_score()
{
    //dtor
}

void _score::resetScore()
{
    playerScore = 0;
    strcpy(strScore,"00000\0");
}

void _score::drawScore()
{
    //itoa(playerScore,strScore,10);
    myText->drawText("ABCD",0.2);
}

void _score::updateScore(float speed)
{
    playerScore += speed;
    if (playerScore == 0) return;
    int cpy = playerScore;
    for (int i = 0; i < 5; i++) {
        strScore[i] = (cpy % 10) + '0';
        cpy /= 10;
    }
    char temp = strScore[0];
    strScore[0] = strScore[4];
    strScore[4] = temp;
    temp = strScore[1];
    strScore[1] = strScore[3];
    strScore[3] = temp;
}

