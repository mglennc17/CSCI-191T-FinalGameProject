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
    strcpy(strScore,"00000");
}

void _score::drawScore()
{
    //itoa(playerScore,strScore,10);
    myText->drawText("ABCD",0.2);
}

void _score::updateScore(float speed)
{
    playerScore += speed;
    itoa((int)playerScore,strScore,10);
}

