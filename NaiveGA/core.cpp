#include "core.h"

#include <iostream>
#include <string>
#include <windows.h>

#include <stdlib.h>
#include <time.h>

using namespace std;

const char Core::charFace[8] = {'|', '=', '|', '=', '/', '\\', '/', '\\'};
const int Core::dirToCoord[8][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
const string Core::geneOpName[7] = {"0 And : ", "1 Or  : ", "2 Sub : ", "3 LSHT: ", "4 RSHT: ", "5 NUL : ", "6 XOR : "};

Core::Core()
{
    mapW = 40;
    mapH = 20;
    simTime = 0;
    srand(time(NULL));
    scrn.Initialize(mapW + 40, mapH + 3, 1, 2);
    ready = false;
    poolSize = 0;
    geneSize = 0;
    movDirBlock = 0;
}

Core::~Core()
{
    // Free memory here
    return;
}

void Core::vanillaSim()
{
    if (!ready){vanillaSimInit(50, 5);}
    gotoxy(1, 1);
    cout << "AI: laser avoidance";
    movDirBlock = 0;
    int tested[poolSize];
    int fitness[3];
    int exchg, curTested;
    int best, second, last;
    for (int i = 0; i < poolSize; i++) tested[i] = i;
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            fitness[i] = 0;
            exchg = rand() % poolSize;
            if (exchg != i)
            {
                tested[i] ^= tested[exchg];
                tested[exchg] ^= tested[i];
                tested[i] ^= tested[exchg];
            }
        }
        for (curTested = 0; curTested < 3; curTested++)
        {
            fieldCleanUp();
            bool alive;
            for (simTime = 0, alive = true; alive; simTime++)
            {
                updateLaser();
                vanillaLaserSpawn(100);
                if (updateBot(tested[curTested])) alive = false;
                movDirBlock = (!movDirBlock);
            }
            fitness[curTested] = simTime;
            for (simTime = 0, alive = true; alive; simTime++)
            {
                updateLaser();
                vanillaLaserSpawn(100);
                if (updateBot(tested[curTested])) alive = false;
                movDirBlock = (!movDirBlock);
            }
            fitness[curTested] = (fitness[curTested] + simTime) / 2;
            if (fitness[curTested] >= 50)
            {
                fieldCleanUp();
                scrn.ClrBuffer();
                scrn.UpdateScreen();
                bool alive = true;
                for (simTime = 0; alive; simTime++)
                {
                    updateLaser();
                    vanillaLaserSpawn(100);
                    if (updateBot(tested[curTested])) alive = false;
                    scrn.ClrBuffer();
                    updateScrn(tested[curTested]);
                    scrn.UpdateScreen();
                    Sleep(100);
                    movDirBlock = (!movDirBlock);
                }
                Sleep(500);
            }
        }
        if (fitness[1] > fitness[0])
        {
            best = 1;
            second = 0;
        }else{
            best = 0;
            second = 1;
        }
        if (fitness[2] > fitness[second])
        {
            last = second;
            if (fitness[2] > fitness[best])
            {
                second = best;
                best = 2;
            }else{
                second = 2;
            }
        }else{
            last = 2;
        }
        crossOver(tested[best], tested[second], tested[last]);
        mutation(best, 1);
        mutation(second, 1);
        mutation(last, 2);
    }
    ready = false;
    return;
}

void Core::fieldCleanUp()
{
    for (int i = 0; i < mapH; i++)
    {
        for (int j = 0; j < mapW; j++)
        {
            objMap[i][j] = 0;
        }
    }
    bot.x = mapW / 2;
    bot.y = mapH / 2;
    return;
}

void Core::vanillaSimInit(int pool_size, int gene_size)
{
    ready = true;
    simTime = 0;
    poolSize = pool_size;
    geneSize = gene_size;
    objMap = new int *[mapH];
    gene = new int *[poolSize];
    geneOp = new int *[poolSize];
    for (int i = 0; i < mapH; i++)
    {
        objMap[i] = new int[mapW];
        for (int j = 0; j < mapW; j++)
        {
            objMap[i][j] = 0;
        }
    }
    for (int i = 0; i < poolSize; i++)
    {
        gene[i] = new int[gene_size];
        geneOp[i] = new int[gene_size];
        randGene(i);
    }
    movDirBlock = 0;
    bot.opResult = new int[geneSize + 1];
    return;
}

void Core::vanillaLaserSpawn(int randSize)
{
    for (int i = 0; i < mapW; i++)
        if (!(rand() % randSize))
            objMap[0][i] |= 4;
    for (int i = 0; i < mapW; i++)
        if (!(rand() % randSize))
            objMap[mapH - 1][i] |= 1;
    for (int i = 0; i < mapH; i++)
        if (!(rand() % randSize))
            objMap[i][0] |= 2;
    for (int i = 0; i < mapH; i++)
        if (!(rand() % randSize))
            objMap[i][mapW - 1] |= 8;
    return;
}

void Core::updateLaser()
{
    for (int i = 0; i < mapH; i++)
    {
        for (int j = 0; j < mapW; j++)
        {
            if (objMap[i][j] & 1) // up
            {
                if (i > 0)
                {
                    objMap[i + dirToCoord[0][1]][j + dirToCoord[0][0]] |= 1;
                }
                objMap[i][j] &= (FULLINT ^ 1);
            }
            if (objMap[i][j] & 8) // left
            {
                if (j > 0)
                {
                    objMap[i + dirToCoord[3][1]][j + dirToCoord[3][0]] |= 8;
                }
                objMap[i][j] &= (FULLINT ^ 8);
            }
        }
    }
    for (int i = mapH - 1; i >= 0; i--)
    {
        for (int j = mapW - 1; j >= 0; j--)
        {
            if (objMap[i][j] & 2) // right
            {
                if (j < mapW - 1)
                {
                    objMap[i + dirToCoord[1][1]][j + dirToCoord[1][0]] |= 2;
                }
                objMap[i][j] &= (FULLINT ^ 2);
            }
            if (objMap[i][j] & 4) // down
            {
                if (i < mapH - 1)
                {
                    objMap[i + dirToCoord[2][1]][j + dirToCoord[2][0]] |= 4;
                }
                objMap[i][j] &= (FULLINT ^ 4);
            }
        }
    }
    return;
}

bool Core::updateBot(int geneID)
{
    if (geneID >= poolSize){return 1;}
    bool hit = false;
    int removeSelf = (FULLINT ^ 512);
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            objMap[bot.y + i][bot.x + j] &= removeSelf;
    updateBotSensor();
    updateBotAI(geneID);
    //if (movDirBlock)
    {
        if ((bot.opResult[geneSize] & 1) ^ (bot.opResult[geneSize] & 4))
        {
            if (bot.opResult[geneSize] & 1)
            {
                if (bot.y > 3) bot.y--;
            }else if(bot.y < mapH - 4){
                bot.y++;
            }
        }
    }/*else*/{
        if ((bot.opResult[geneSize] & 2) ^ (bot.opResult[geneSize] & 8))
        {
            if (bot.opResult[geneSize] & 8)
            {
                if (bot.x > 3) bot.x--;
            }else if(bot.x < mapW - 4){
                bot.x++;
            }
        }
    }
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (objMap[bot.y + i][bot.x + j] > 0)
            {
                hit = true;
                objMap[bot.y + i][bot.x + j] = 256;
            }else{
                objMap[bot.y + i][bot.x + j] |= 512;
            }
        }
    }
    return hit;
}

void Core::updateBotSensor()
{
    bot.sensor = 0;
    bool detected = false;
    int x, y;
    for (int d = 0; d < 8; d++)
    {
        x = bot.x + dirToCoord[d][0];
        y = bot.y + dirToCoord[d][1];
        detected = false;
        if (d < 4)
        {
            for (int i = -2; i < 3; i++)
            {
                for (int j = -2; j < 3; j++)
                {
                    detected |= (objMap[y + i][x + j] & laserFilter);
                }
            }
        }else{
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    detected |= (objMap[y + i][x + j] & laserFilter);
                }
            }
        }
        bot.sensor |= (detected << d);
    }
    return;
}

void Core::updateBotAI(int geneID)
{
    bot.opResult[0] = bot.sensor; // Separate them to make the code more readable, but they are supposed to refer to the same value when they are used
    for (int p = 0; p < geneSize; p++)
    {
        bot.opResult[p + 1] = processGeneSeg(bot.opResult[p], geneID, p);
    }
    return;
}

void Core::updateScrn(int geneID)
{
    int tmp;
    string output;
    for (int i = 0; i < mapH; i++)
    {
        output = string(mapW, '.');
        for (int j = 0; j < mapW; j++)
        {
            if (objMap[i][j] >> laserBit) // Check if solid exist
            {
                if (objMap[i][j] & laserFilter || objMap[i][j] & 256) // Check if laser exist or bot damaged
                {
                    output[j] = '*';
                }else{
                    output[j] = '#';
                }
            }else{
                if (objMap[i][j] & laserFilter) // Check if laser exist
                {
                    tmp = objMap[i][j] & laserFilter;
                    for (int k = 0; tmp > 0; k++)
                    {
                        if (tmp & 1)
                        {
                            if (output[j] == '.')
                            {
                                output[j] = charFace[k];
                            }else{
                                output[j] = '+';
                            }
                        }
                        tmp >>= 1;
                    }
                }
            }
        }
        scrn.PushStr(output, 1, i + 1);
    }
    if (geneID >= poolSize) return;
    // Begin Measurements
    output = string(INPUTSIZE, '_');
    int sensorV = bot.sensor;
    for (int i = INPUTSIZE - 1; i >= 0; i--)
    {
        output[i] = (sensorV & 1)? '1': '0';
        sensorV >>= 1;
    }
    output = "Sensor: " + output;
    scrn.PushStr(output, mapW + 1, 1);

    string outValue;
    for (int p = 0; p < geneSize; p++)
    {
        outValue = string(USEFULGENESIZE, '_');
        int opV = gene[geneID][p];
        for (int i = USEFULGENESIZE - 1; i >= 0; i--)
        {
            outValue[i] = (opV & 1)? '1': '0';
            opV >>= 1;
        }
        output = geneOpName[geneOp[geneID][p]] + outValue + " -> ";
        outValue = string(USEFULGENESIZE, '_');
        opV = bot.opResult[p + 1];
        for (int i = USEFULGENESIZE - 1; i >= 0; i--)
        {
            outValue[i] = (opV & 1)? '1': '0';
            opV >>= 1;
        }
        output += outValue;
        scrn.PushStr(output, mapW + 1, p + 2);
    }

    outValue = string(32, '_');
    int opV = simTime;
    for (int i = 31; i >= 0; i--)
    {
        outValue[i] = (opV % 10) + '0';
        opV /= 10;
    }
    output = "Round time: " + outValue;
    scrn.PushStr(output, 1, mapH + 1);

    outValue = string(32, '_');
    opV = geneID;
    for (int i = 31; i >= 0; i--)
    {
        outValue[i] = (opV % 10) + '0';
        opV /= 10;
    }
    output = "ID: " + outValue;
    scrn.PushStr(output, 1, mapH + 2);
    // End Measurement
    return;
}

void Core::randGene(int geneID)
{
    for (int i = 0; i < geneSize; i++)
    {
        gene[geneID][i] = rand();
        geneOp[geneID][i] = rand() % 7;
    }
    return;
}

void Core::crossOver(int goodGeneID, int goodGeneID2, int badGeneID)
{
    if (rand() % 2)
    {
        goodGeneID ^= goodGeneID2;
        goodGeneID2 ^= goodGeneID;
        goodGeneID ^= goodGeneID2;
    }
    int critPos = rand() % (geneSize - 1);
    int i;
    for (i = 0; i <= critPos; i++)
    {
        gene[badGeneID][i] = gene[goodGeneID][i];
        geneOp[badGeneID][i] = geneOp[goodGeneID][i];
    }
    for (; i < geneSize; i++)
    {
        gene[badGeneID][i] = gene[goodGeneID2][i];
        geneOp[badGeneID][i] = geneOp[goodGeneID2][i];
    }
    return;
}

void Core::mutation(int geneID, int randPercentage)
{
    if (rand() % 100 < randPercentage)
    {
        int mutPos = rand() % geneSize;
        if (rand())
        {
            gene[geneID][mutPos] = rand();
            geneOp[geneID][mutPos] = rand() % 7;
        }else{
            gene[geneID][mutPos] ^= (1 << (rand() % USEFULGENESIZE));
        }
    }
    return;
}

int Core::processGeneSeg(int input, int geneID, int genePos)
{
    int segment = gene[geneID][genePos];
    switch (geneOp[geneID][genePos])
    {
    case 0:
        return (input & segment);
    case 1:
        return (input | segment);
    case 2:
        return (input & (~segment));
    case 3:
        return (input << 1);
    case 4:
        return (input >> 1);
    case 5:
        break;
    case 6:
        return (input ^ segment);
    }
    return input;
}
