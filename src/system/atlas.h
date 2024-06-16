/***************************************************************
 *
 *
 *      █████╗ ████████╗██╗      █████╗ ███████╗
 *     ██╔══██╗╚══██╔══╝██║     ██╔══██╗██╔════╝
 *     ███████║   ██║   ██║     ███████║███████╗
 *     ██╔══██║   ██║   ██║     ██╔══██║╚════██║
 *     ██║  ██║   ██║   ███████╗██║  ██║███████║
 *     ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝
 *
 *
 *     Handles loading of atlas and getting the texture.
 *     Authored by Amir Kedis
 *
 *
 **************************************************************/
#ifndef ATLAS_H
#define ATLAS_H

#include "../CRougeLite.h"

void initAtlas();
void loadAtlasData();
AtlasImage getAtlasImage(char *filename);

#endif // ATLAS_H

