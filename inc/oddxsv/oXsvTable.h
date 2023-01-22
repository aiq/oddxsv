#ifndef ODDXSV_OXSVTABLE_H
#define ODDXSV_OXSVTABLE_H

#include "clingo/lang/error.h"
#include "clingo/type/cBytes.h"
#include "clingo/type/cChars.h"
#include "oddxsv/apidecl.h"

/*******************************************************************************
********************************************************* Types and Definitions
********************************************************************************
 
*******************************************************************************/

#define oXSV_CELL_SEP_ 0x1F
#define oXSV_ROW_SEP_ 0x1F

struct oXsvRow
{
   cBytes data;
   int64_t cols;
};
typedef struct oXsvRow oXsvRow;

struct oXsvTable
{
   cBytes data;
   int64_t rows;
   int64_t cols;
   int64_t maxRowSize;
};
typedef struct oXsvTable oXsvTable;

/*******************************************************************************
********************************************************************* Functions
********************************************************************************

*******************************************************************************/

ODDXSV_API bool init_xsv_table_o( oXsvTable table[static 1],
                                  cBytes data,
                                  cErrorStack es[static 1] );

/*******************************************************************************

*******************************************************************************/

ODDXSV_API cChars xsv_row_cell_o( oXsvRow row, int64_t col );

ODDXSV_API cChars xsv_table_cell_o( oXsvTable const table[static 1],
                                    int64_t row,
                                    int64_t col );

ODDXSV_API oXsvRow xsv_table_row_o( oXsvTable const table[static 1],
                                    int64_t row );

/*******************************************************************************

*******************************************************************************/

#define iterate_xsv_rows_o_( Itr, RowPtr, TablePtr )                           \
   for ( cByte const* Itr = next_xsv_row_o( TablePtr, NULL, RowPtr );          \
         Itr != NULL;                                                          \
         Itr = next_xsv_row_o( TablePtr, Itr, RowPtr ) )
ODDXSV_API cByte const* next_xsv_row_o( oXsvTable const table[static 1],
                                        cByte const* itr,
                                        oXsvRow row[static 1] );

#define iterate_xsv_cells_o_( Itr, CellPtr, RowPtr )                           \
   for ( cByte const* Itr = next_xsv_cell_o( RowPtr, NULL, CellPtr );          \
         Itr != NULL;                                                          \
         Itr = next_xsv_cell_o( RowPtr, Itr, CellPtr ) )
ODDXSV_API cByte const* next_xsv_cell_o( oXsvRow const row[static 1],
                                         cByte const* itr,
                                         cChars cell[static 1] );

/*******************************************************************************

*******************************************************************************/

ODDXSV_API bool store_as_xsv_row_o( cCharsSlice cells,
                                    cRecorder rec[static 1] );

#endif