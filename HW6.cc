
/*
 * Filename       HW6.cc
 * Date           4/25/2020
 * Author         Mythreyan Ganesh
 * Email          mrg180010@utdallas.edu
 * Course         CS 3377.501
 * Version        1.0
 * Copyright      2020, All Right Reserved
 *
 * Description
 *
 *               This is the main file that has the BinaryFileHeader and BinaryFileRecord class defined and 
 *               reads in the binary file and displays it to the screen using a CDK Matrix
 *
 *
 *
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

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

  const char            *rowTitles[MATRIX_HEIGHT+1] = {"x", "a", "b", "c", "d", "e"};
  const char            *columnTitles[MATRIX_WIDTH+1] = {"y", "a", "b", "c"};
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

  
     /* required  */
  
  BinaryFileHeader *myHeader = new BinaryFileHeader();
  ifstream binHeader("/scratch/perkins/cs3377.bin", ios::in | ios::binary);
  if(!binHeader){
    printf("Error opening a binary file");
    _exit(1);
  }

  binHeader.read((char *) myHeader, sizeof(BinaryFileHeader));
  string output;
  stringstream ss;
  ss << "Magic: 0x" << hex << myHeader->magicNumber;
  output = ss.str();
  setCDKMatrixCell(myMatrix, 1, 1, output.c_str());
  ss.str("");
  ss.clear();
  ss << "Version: " << dec <<  myHeader->versionNumber;
  output = ss.str();
  setCDKMatrixCell(myMatrix, 1, 2, output.c_str());
  ss.str("");
  ss.clear();
  ss << "NumRecords: " << myHeader->numRecords;
  output = ss.str();
  setCDKMatrixCell(myMatrix, 1 ,3, output.c_str());
  ss.str("");
  ss.clear();
  long int offset = sizeof(BinaryFileHeader);
  BinaryFileRecord *myRecord = new BinaryFileRecord();
  ifstream binInfile("/scratch/perkins/cs3377.bin", ios::in | ios::binary);
  if(!binInfile){
    printf("Error opening a binary file");
    _exit(1);
  }
  binInfile.seekg(offset);
  for(int i = 2; i <= MATRIX_HEIGHT; i++){
    if(!binInfile.eof()){
      binInfile.read((char *) myRecord, sizeof(BinaryFileRecord));
      ss << "strlen: " << strlen(myRecord->stringBuffer);
      output = ss.str();
      setCDKMatrixCell(myMatrix, i, 1, output.c_str());
      setCDKMatrixCell(myMatrix, i, 2, myRecord->stringBuffer);
      ss.str("");
      ss.clear();
    }
  }
 
  drawCDKMatrix(myMatrix, true);
  int y;

  cin >> y;
  //clean up screen
  endCDK();
  return 0;
}
