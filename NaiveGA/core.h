#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#define INPUTSIZE 8
#define USEFULGENESIZE 8

#include <string>

#include "consoleGL.h"

using namespace std;

struct TestBot
{
public:
    int x;
    int y;
    int sensor;
    int *opResult;
};

class Core
{
public:
    Core();
    ~Core();
    void vanillaSim();
private:
    ConsoleScreen scrn;
    int **objMap;
    int mapW;
    int mapH;
    int simTime;
    int **gene;
    int **geneOp;
    int poolSize;
    int geneSize;
    int movDirBlock; // 0 Horizontal only 1 Vertical only
    TestBot bot;
    static const int laserFilter = 255;
    static const int laserBit = 8;
    static const int FULLINT = (~0);
    static const char charFace[8];
    static const int dirToCoord[8][2];
    static const string geneOpName[7];
    bool ready;
    void fieldCleanUp();
    void vanillaSimInit(int pool_size, int gene_size); // all new call should be done here
    void vanillaLaserSpawn(int randSize); // probability is 1 / randSize
    void updateLaser(); // 0 nothing 1 hit
    bool updateBot(int geneID); // 0 safe 1 hit
    void updateBotSensor();
    void updateBotAI(int geneID);
    void updateScrn(int geneID);
    void randGene(int geneID);
    void crossOver(int goodGeneID, int goodGeneID2, int badGeneID);
    void mutation(int geneID, int randPercentage);
    int processGeneSeg(int input, int geneID, int genePos);
};

#endif // CORE_H_INCLUDED
