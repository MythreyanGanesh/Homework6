#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Reading"

using namespace std;


class BinaryFileHeader{

public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

//records in the file have a fixed length buffer that will hold a c_style string
const int maxRecordStringLength = 25;
class BinaryFileRecord{

public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

int main()
{

  WINDOW        *window;
  CDKSCREEN     *cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  const char            *rowTitles[MATRIX_HEIGHT+1] = {"R0", "R1", "R2", "R3", "R4", "R5"};
  const char            *columnTitles[MATRIX_WIDTH+1] = {"C0", "C1", "C2", "C3"};
  int           boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int           boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
   */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
                          MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
			  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */
  setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
  drawCDKMatrix(myMatrix, true);    /* required  */
  
  BinaryFileHeader *myHeader = new BinaryFileHeader();
  ifstream binHeader("/scratch/perkins/cs3377.bin", ios::in | ios::binary);
  binHeader.read((char *) myHeader, sizeof(BinaryFileHeader));
  string output;
  stringstream ss;
  ss << "Magic: 0x" << hex << myHeader->magicNumber;
  output = ss.str();
  setCDKMatrixCell(myMatrix, 1, 1, output.c_str());
  ss.str("");
  ss.clear();
  ss << "Version: " << myHeader->versionNumber;
  output = ss.str();
  setCDKMatrixCell(myMatrix, 1, 2, output.c_str());
  ss.str("");
  ss.clear();
  ss << "NumRecords: " << myHeader->numRecords;
  output = ss.str();
  setCDKMatrixCell(myMatrix, 1 ,3, output.c_str());
  
  drawCDKMatrix(myMatrix, true);
  int y;

  cin >> y;
  //clean up screen
  endCDK();
  return 0;
}
