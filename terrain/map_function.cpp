//
// Created by aleksander on 01.02.2022.
//

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <vector>

#define RESOLUTION 1201
#define SQ(x) ((x) * (x))


std::vector<std::string> getTargets(std::string path) {
    std::vector<std::string> targets;

    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_name[0] != '.') {
                std::string mname = ent->d_name;
                targets.push_back(path + "/" + mname);
            }
            printf ("%s\n", ent->d_name);
        }

        closedir (dir);
    } else {
        std::cout << "isn't a valid directory" << std::endl << "terminating" << std::endl;
        exit(0);
    }

    std::cout << "located " << targets.size() << " maps..." << std::endl;

    return targets;
}

void readfile(std::string filename, unsigned short int *mapdump) {
    std::ifstream mapdata(filename, std::ios::out | std::ios::binary);

    if (!mapdata) {
        std::cout << "Failed to open file " << filename << std::endl << "terminating...";
        exit(1);
    }

    unsigned char *tempData = (unsigned char *)malloc(2 * SQ(RESOLUTION) * sizeof(unsigned char));
    mapdata.read((char *)tempData, 2 * SQ(RESOLUTION));
    
    for(int i = 0 ; i < RESOLUTION; i++)
        for(int j = 0 ; j < RESOLUTION; j++) {
            int xcoord = i;
            int ycoord = (RESOLUTION + (j - i - 1)) % RESOLUTION;
            int index = RESOLUTION * xcoord + ycoord;
            int findex = RESOLUTION * i + j;
            if ((tempData[2 * findex + 0] << 8 | tempData[2 * findex + 1]) > ((1 << 14) - 1))
                mapdump[i] = 0;
            else
                mapdump[index] = tempData[2 * findex + 0] << 8 | tempData[2 * findex + 1];
        }

    free(tempData);
}

unsigned short int * readfile(std::string filename) {
    unsigned short int *originalmap = (unsigned short int *)malloc(SQ(RESOLUTION) * sizeof(unsigned short int));
    readfile(filename, originalmap);
    return originalmap;
}