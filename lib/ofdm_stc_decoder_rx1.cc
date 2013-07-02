#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_sync_block.h>
#include <gr_io_signature.h>
#include "ofdm_stc_decoder_rx1.h"

#include <iostream>

#define DEBUG 0


ofdm_stc_decoder_rx1 ::
ofdm_stc_decoder_rx1( int vlen )
  : gr_sync_block(
      "stc_decoder_rx1",
      gr_make_io_signature6(
        6, 6,
        sizeof( gr_complex ) * vlen,            // ofdm_blcoks
        sizeof( gr_complex ) * vlen,            // ofdm_blcoks2
        sizeof( gr_complex ) * vlen,            // h0
        sizeof( gr_complex ) * vlen,            // h1
        sizeof( gr_complex ) * vlen,            // h2
        sizeof( gr_complex ) * vlen ),          // h3
      gr_make_io_signature(
        1, 1,
        sizeof( gr_complex ) * vlen ) )

  , d_vlen( vlen )

{
  assert( ( vlen % 2 ) == 0 ); // alignment 16 byte
}

int
ofdm_stc_decoder_rx1 ::work (
        int                         noutput_items,
        gr_vector_const_void_star & input_items,
        gr_vector_void_star       & output_items )
{
  gr_complex const * ofdm_blocks = static_cast< gr_complex const * >( input_items[0] );
  gr_complex const * ofdm_blocks2 = static_cast< gr_complex const * >( input_items[1] );
  gr_complex const * h0 = static_cast< gr_complex const * >( input_items[2] );
  gr_complex const * h1 = static_cast< gr_complex const * >( input_items[3] );
  gr_complex const * h2 = static_cast< gr_complex const * >( input_items[4] );
  gr_complex const * h3 = static_cast< gr_complex const * >( input_items[5] );
  gr_complex * out = static_cast< gr_complex * >( output_items[0] );

  for( int j = 0; j < d_vlen; ++j )
  {
        if( j % 2 == 0 )
        {
            out[j] = 0 ;
        } else {
            out[j] = std::conj( h1[j] ) * ofdm_blocks[j-1] - h0[j] * std::conj( ofdm_blocks[j] ) +std::conj( h3[j] ) * ofdm_blocks2[j-1] - h2[j] * std::conj( ofdm_blocks2[j] );
        }
  }

  return noutput_items;

} // general_work

ofdm_stc_decoder_rx1_sptr
ofdm_make_stc_decoder_rx1( int vlen )
{
  return ofdm_stc_decoder_rx1::create( vlen );
}

ofdm_stc_decoder_rx1_sptr
ofdm_stc_decoder_rx1 ::
create( int vlen )
{
  try
  {
    ofdm_stc_decoder_rx1_sptr tmp(
      new ofdm_stc_decoder_rx1( vlen ) );

    return tmp;
  }
  catch ( ... )
  {
    std::cerr << "[ERROR] Caught exception at creation of "
              << "ofdm_stc_decoder_rx1" << std::endl;
    throw;
  } // catch ( ... )
}

