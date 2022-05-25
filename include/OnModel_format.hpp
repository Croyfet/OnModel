#pragma once

namespace cc
{
	namespace ONM
	{

		// Cretmes OnModel Object Format Binaly

		//   file extension : oofb
		//       endianness : BIG
		//     magic number : 'CRTMSFMT' (8 byte)
		// sub magic number : 'ONMOB   ' (8 byte)
		// 
		// TAG
		//     0-7		char[8]		tag name
		//     8-15		uint64_t	tagsize(exclude tagname and tagsize)
		//     16-		-			content
		// 
		//     type (must first)
		//         tag name : "type"
		//          tagsize : (option)
		//          content : [int64_t]
		//                       Default OnModel : 1
		//
		//     Compression
		//         tag name : "compress"
		//          tagsize : (option)
		//          content : [bool](int64_t)
		//
		//     Hash
		//         tag name : "hash"
		//          tagsize : (option)
		//          content : [int64_t]hash type
		//                    [variable length]hash data (only main)
		// 
		//     Text
		//         tag name : "txt    "
		//          tagsize : content length
		//          content : free length string (If you needs buffer, fill null and write extra size to tagsize)
		// 
		//     Magnetic field
		//         tag name : "mf      "
		//          tagsize : (option) sizeof(double)*3
		//          content : [double]x
		//                  : [double]y
		//                  : [double]z
		// 
		//     Temperature
		//         tag name : "tempra  "
		//          tagsize : (option) sizeof(double)
		//          content : [double]
		//
		//     ExchangeInteraction
		//         tag name : "jei     "
		//          tagsize : (option) sizeof(double)
		//          content : [double]







		// Cretmes OnModel Object Format Text

		//   file extension : oofb
		//       endianness : BIG
		//     magic number : 'CRTMSFMT' (8 byte)
		// sub magic number : 'ONMOT   ' (8 byte)
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 
		// 




		//const char SUBMAGICNUMBER_ONM_BINARY[8] = { 'O','N', 'M', 'O', 'B', ' ', ' ', ' ' };
		//const char SUBMAGICNUMBER_ONM_TEXT[8] = { 'O','N', 'M', 'O', 'T', ' ', ' ', ' ' };


	}
}

