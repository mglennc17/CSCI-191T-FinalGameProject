#ifndef _SCORE_H
#define _SCORE_H

#include <_textDisplay.h>


class _score
{
    public:
        _score();
        virtual ~_score();

        _textDisplay *myText = new _textDisplay();
        void drawScore();
        void resetScore();
        void updateScore(float);

        float playerScore;
        char strScore[5];

    protected:

    private:
};

#endif // _SCORE_H
