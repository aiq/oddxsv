#include "oddxsv/oXsvTable.h"

#include "clingo/io/cScanner.h"
#include "clingo/lang/func.h"

/*******************************************************************************

*******************************************************************************/

#define finished_( Slice, Ptr )                                                \
(                                                                              \
   Ptr > end_c_( Slice ) and                                                   \
   not points_into_c_( Slice, Ptr )                                            \
)

cByte const* next_xsv_entry( cBytes data,
                             cByte sep,
                             cByte const* itr,
                             cBytes dest[static 1] )
{
   if ( is_empty_c_( data ) ) return NULL;

   if ( itr == NULL )
   {
      itr = begin_c_( data );
   }

   if ( finished_( data, itr ) ) return NULL;

   cByte const* beg = itr;

   cBytes cnt = atween_c_( itr, end_c_( data ) );
   if ( is_empty_c_( cnt ) )
   {
      *dest = (cBytes)atween_c_( beg, beg );
      return beg+1;
   }

   cScanner* sca = &make_scanner_c_( cnt.s, cnt.v );
   if ( on_byte_c( sca, sep ) )
   {
      *dest = (cBytes)atween_c_( beg, beg );
      return beg+1;
   }
   if ( not move_to_byte_c( sca, sep ) )
   {
      *dest = cnt;
      itr = end_c_( cnt );
      return itr+1;
   }

   cByte const* end = sca->mem;
   *dest = (cBytes)atween_c_( beg, end );
   return end+1;
}

/*******************************************************************************
********************************************************************* Functions
********************************************************************************

*******************************************************************************/

bool init_xsv_table_o( oXsvTable table[static 1],
                       cBytes data,
                       cErrorStack es[static 1] )
{
   table->cols = 0;
   table->rows = 0;
   cByte const* rowItr;
   cBytes row;
   for ( rowItr = next_xsv_entry( data, oXSV_ROW_SEP_, NULL, &row );
         rowItr != NULL;
         rowItr = next_xsv_entry( data, oXSV_ROW_SEP_, rowItr, &row ) )
   {
      int64_t cols = 0;
      cByte const* cellItr;
      cBytes cell;
      for ( cellItr = next_xsv_entry( row, oXSV_CELL_SEP_, NULL, &cell );
            cellItr != NULL;
            cellItr = next_xsv_entry( row, oXSV_CELL_SEP_, cellItr, &cell ) )
      {
         cols++;
      }
      if ( table->cols != 0 and cols != table->cols )
      {
         return false; // ERROR rows mismatch
      }
      table->cols = cols;
      table->rows++;
   }
   return true;
};

/*******************************************************************************

*******************************************************************************/

cChars xsv_row_cell_o( oXsvRow row, int64_t col )
{
   must_be_in_range_c_( 0, col, row.cols-1 );
   int64_t count = 0;
   cChars cell;
   iterate_xsv_cells_o_( itr, &cell, &row )
   {
      ++count;
      if ( count == col )
         return cell;
   }
   return (cChars)invalid_c_();
}

cChars xsv_table_cell_o( oXsvTable const table[static 1],
                         int64_t row,
                         int64_t col )
{
   oXsvRow xsvRow = xsv_table_row_o( table, row );
   return xsv_row_cell_o( xsvRow, col );
}

oXsvRow xsv_table_row_o( oXsvTable const table[static 1],
                         int64_t row )
{
   must_be_in_range_c_( 0, row, table->rows-1 );
   int64_t count = 0;
   oXsvRow csvRow;
   iterate_xsv_rows_o_( itr, &csvRow, table )
   {
      ++count;
      if ( count == row )
         return csvRow;
   }
   return (oXsvRow){0};
}

/*******************************************************************************

*******************************************************************************/

cByte const* next_xsv_row_o( oXsvTable const table[static 1],
                             cByte const* itr,
                             oXsvRow row[static 1] )
{
   cBytes byteCell;
   itr = next_xsv_entry( table->data, oXSV_ROW_SEP_, itr, &byteCell );
   row->data = byteCell;
   row->cols = table->cols;
   return itr;
}

cByte const* next_xsv_cell_o( oXsvRow const row[static 1],
                              cByte const* itr,
                              cChars cell[static 1] )
{
   cBytes byteCell;
   itr = next_xsv_entry( row->data, oXSV_CELL_SEP_, itr, &byteCell );
   *cell = (cChars){ .s=byteCell.s, .v=(char const*)byteCell.v };
   return itr;
}