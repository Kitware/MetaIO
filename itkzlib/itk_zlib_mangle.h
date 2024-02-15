#ifndef itk_zlib_mangle_h
#define itk_zlib_mangle_h

/*

This header file mangles all symbols exported from the zlib library.
It is included in all files while building the zlib library.  Due to
namespace pollution, no zlib headers should be included in .h files in
ITK.

The following command was used to obtain the symbol list:

nm -g libitkzlib.a | grep -e ' [TDRSB] ' | cut -d' ' -f3 | grep -v -e '^_init$' -e '^_fini$' -e '^_edata$' -e '^__bss_start$'

This is the way to recreate the whole list by piping in the above:

LC_COLLATE=POSIX sort -u | sed -e 's/\(.*\)/#define \1 itk_zlib_\1/'

*/

#define _dist_code itk_zlib__dist_code
#define _length_code itk_zlib__length_code
#define _tr_align itk_zlib__tr_align
#define _tr_flush_bits itk_zlib__tr_flush_bits
#define _tr_flush_block itk_zlib__tr_flush_block
#define _tr_init itk_zlib__tr_init
#define _tr_stored_block itk_zlib__tr_stored_block
#define _tr_tally itk_zlib__tr_tally
#define adler32 itk_zlib_adler32
#define adler32_combine itk_zlib_adler32_combine
#define adler32_combine64 itk_zlib_adler32_combine64
#define adler32_z itk_zlib_adler32_z
#define compress itk_zlib_compress
#define compress2 itk_zlib_compress2
#define compressBound itk_zlib_compressBound
#define crc32 itk_zlib_crc32
#define crc32_combine itk_zlib_crc32_combine
#define crc32_combine64 itk_zlib_crc32_combine64
#define crc32_combine_gen itk_zlib_crc32_combine_gen
#define crc32_combine_gen64 itk_zlib_crc32_combine_gen64
#define crc32_combine_op itk_zlib_crc32_combine_op
#define crc32_z itk_zlib_crc32_z
#define deflate itk_zlib_deflate
#define deflateBound itk_zlib_deflateBound
#define deflateCopy itk_zlib_deflateCopy
#define deflateEnd itk_zlib_deflateEnd
#define deflateGetDictionary itk_zlib_deflateGetDictionary
#define deflateInit2_ itk_zlib_deflateInit2_
#define deflateInit_ itk_zlib_deflateInit_
#define deflateParams itk_zlib_deflateParams
#define deflatePending itk_zlib_deflatePending
#define deflatePrime itk_zlib_deflatePrime
#define deflateReset itk_zlib_deflateReset
#define deflateResetKeep itk_zlib_deflateResetKeep
#define deflateSetDictionary itk_zlib_deflateSetDictionary
#define deflateSetHeader itk_zlib_deflateSetHeader
#define deflateTune itk_zlib_deflateTune
#define deflate_copyright itk_zlib_deflate_copyright
#define get_crc_table itk_zlib_get_crc_table
#define gzclose itk_zlib_gzclose
#define inflate itk_zlib_inflate
#define inflateBack itk_zlib_inflateBack
#define inflateBackEnd itk_zlib_inflateBackEnd
#define inflateBackInit_ itk_zlib_inflateBackInit_
#define inflateCodesUsed itk_zlib_inflateCodesUsed
#define inflateCopy itk_zlib_inflateCopy
#define inflateEnd itk_zlib_inflateEnd
#define inflateGetDictionary itk_zlib_inflateGetDictionary
#define inflateGetHeader itk_zlib_inflateGetHeader
#define inflateInit2_ itk_zlib_inflateInit2_
#define inflateInit_ itk_zlib_inflateInit_
#define inflateMark itk_zlib_inflateMark
#define inflatePrime itk_zlib_inflatePrime
#define inflateReset itk_zlib_inflateReset
#define inflateReset2 itk_zlib_inflateReset2
#define inflateResetKeep itk_zlib_inflateResetKeep
#define inflateSetDictionary itk_zlib_inflateSetDictionary
#define inflateSync itk_zlib_inflateSync
#define inflateSyncPoint itk_zlib_inflateSyncPoint
#define inflateUndermine itk_zlib_inflateUndermine
#define inflateValidate itk_zlib_inflateValidate
#define inflate_copyright itk_zlib_inflate_copyright
#define inflate_fast itk_zlib_inflate_fast
#define inflate_table itk_zlib_inflate_table
#define uncompress itk_zlib_uncompress
#define uncompress2 itk_zlib_uncompress2
#define zError itk_zlib_zError
#define z_errmsg itk_zlib_z_errmsg
#define zcalloc itk_zlib_zcalloc
#define zcfree itk_zlib_zcfree
#define zlibCompileFlags itk_zlib_zlibCompileFlags
#define zlibVersion itk_zlib_zlibVersion

#endif
