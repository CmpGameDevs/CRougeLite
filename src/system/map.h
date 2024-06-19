/**********************************************
* 
*    ███╗   ███╗ █████╗ ██████╗ 
*    ████╗ ████║██╔══██╗██╔══██╗
*    ██╔████╔██║███████║██████╔╝
*    ██║╚██╔╝██║██╔══██║██╔═══╝ 
*    ██║ ╚═╝ ██║██║  ██║██║     
*    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     
**********************************************/

#include "../CRougeLite.h"

/**
 * initTilesMapper - initilizes the TilesMapper mapper with NULL,
 * and set path with mapper path.
*/
void initTilesMapper();

/**
 * loadTilesMapper - loades the tiles mapper file at the set path into memory.
 *
 * return: 0 if no errors, 1 if an error occured.
*/
int loadTilesMapper();
