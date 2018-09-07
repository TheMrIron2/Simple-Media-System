/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# (c) 2005-2009 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#include "SMS.h"

static const unsigned char s_IDCT[] __attribute__(   (  aligned( 16 ), section( ".rodata" )  )   ) = {
	0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x01, 0x69, 0x22, 0xde, 0x36, 0x02, 0x00, 0x21, 0x01, 
	0x16, 0x00, 0x00, 0x00, 0x74, 0x2f, 0xe5, 0xa3, 0xe0, 0x07, 0xdf, 0x03, 0x64, 0x5d, 0x00, 0x3e, 
	0x80, 0x2c, 0xdd, 0xf9, 0x13, 0x65, 0xe8, 0x51, 0x03, 0x43, 0xc4, 0x17, 0x4a, 0x0b, 0x69, 0x9c, 
	0xc9, 0xb6, 0xd9, 0x0a, 0x39, 0x68, 0x3f, 0x16, 0x8e, 0xba, 0x93, 0xc0, 0xfd, 0x4c, 0x6c, 0x90, 
	0xd8, 0xfc, 0x04, 0x36, 0xd7, 0xfd, 0x5c, 0x92, 0x94, 0xfe, 0x1f, 0x8a, 0xd8, 0x62, 0x78, 0xd9, 
	0xb9, 0xbb, 0x67, 0x8b, 0xf2, 0xd0, 0xdd, 0x54, 0xb0, 0x26, 0x0c, 0xc4, 0x46, 0x7d, 0x86, 0x12, 
	0x81, 0x82, 0x9e, 0x41, 0x7b, 0x19, 0x94, 0x27, 0x60, 0x00, 0x9d, 0x16, 0x30, 0x5b, 0xec, 0x84, 
	0x31, 0x76, 0x87, 0x68, 0x89, 0xa7, 0x6f, 0xac, 0xdc, 0xca, 0x80, 0x46, 0x3c, 0xbb, 0x30, 0x4a, 
	0x42, 0x5c, 0x27, 0xf0, 0x9a, 0x93, 0x84, 0x3f, 0xfd, 0xa4, 0x3b, 0x36, 0xae, 0x32, 0xb6, 0xf8, 
	0xbd, 0x2b, 0x4f, 0xd1, 0xee, 0x7b, 0xca, 0xf3, 0xc7, 0x97, 0xed, 0xcf, 0x38, 0x77, 0x65, 0x81, 
	0x1e, 0x98, 0x2c, 0x68, 0x8b, 0x61, 0x75, 0x5b, 0x9d, 0x7f, 0x7e, 0x9b, 0x8c, 0x4e, 0x67, 0x90, 
	0xbe, 0xed, 0xb3, 0x15, 0x25, 0x15, 0xc6, 0x2f, 0xec, 0xb2, 0x7b, 0xc4, 0xc9, 0x1b, 0xde, 0x46, 
	0x58, 0x3c, 0xbc, 0xc8, 0x04, 0xe8, 0x50, 0x0a, 0x12, 0x2f, 0x2e, 0x94, 0xd8, 0x53, 0x2c, 0x30, 
	0x5b, 0x18, 0x8d, 0x93, 0x1c, 0x3a, 0xaf, 0x5b, 0x94, 0x7c, 0xf8, 0x28, 0x1d, 0x36, 0xd1, 0x27, 
	0xb1, 0x8a, 0x33, 0x75, 0xd8, 0x64, 0x59, 0xcd, 0x56, 0x3f, 0xa9, 0xbf, 0x4a, 0x91, 0x6b, 0x15, 
	0xb2, 0x5b, 0x63, 0x41, 0x12, 0x06, 0x0d, 0x1c, 0xea, 0xe5, 0x5f, 0x83, 0x58, 0x4b, 0x3b, 0xf6, 
	0x03, 0xb2, 0x41, 0x74, 0x6b, 0x43, 0xf1, 0xa2, 0x9f, 0x27, 0x24, 0x87, 0x5c, 0x56, 0x49, 0xf9, 
	0x7f, 0xf4, 0x56, 0x19, 0x96, 0xec, 0x2c, 0x29, 0xe2, 0xc8, 0x39, 0x15, 0x11, 0x16, 0x88, 0x16, 
	0x43, 0x23, 0xba, 0xdd, 0x56, 0x81, 0x58, 0x8d, 0xa3, 0xc9, 0xed, 0xfd, 0xb5, 0x4e, 0xbf, 0xaf, 
	0x76, 0xb7, 0x18, 0x63, 0x22, 0x01, 0xd5, 0x92, 0xd0, 0x07, 0xad, 0x9b, 0xbe, 0xe5, 0xd4, 0xd0, 
	0x76, 0xe6, 0x5b, 0x90, 0xf2, 0x38, 0x4f, 0x9b, 0xca, 0xed, 0xed, 0x07, 0x9b, 0x80, 0xfe, 0x2f, 
	0xd1, 0x5e, 0x6b, 0xde, 0xc5, 0x9b, 0x4c, 0xfe, 0x2b, 0x78, 0xaa, 0x71, 0x8a, 0xd7, 0x5c, 0x96, 
	0xc8, 0x77, 0x7c, 0xd2, 0x1f, 0x55, 0xd8, 0x9a, 0x22, 0xdf, 0xa7, 0x8e, 0x95, 0x91, 0x9e, 0xf5, 
	0x23, 0xee, 0x98, 0x86, 0xc4, 0x05, 0x1c, 0x5f, 0xf6, 0x0b, 0x2a, 0xad, 0x47, 0x24, 0x52, 0x9c, 
	0x08, 0xed, 0x63, 0x03, 0xcf, 0x66, 0x12, 0x7e, 0xdd, 0x77, 0x6c, 0xe7, 0x38, 0xf3, 0x81, 0xf0, 
	0xf3, 0xfa, 0xd5, 0xd9, 0xb9, 0x41, 0xf8, 0xa3, 0x28, 0xf3, 0xdd, 0x24, 0x9f, 0x61, 0x87, 0xad, 
	0x65, 0x3a, 0x4f, 0x25, 0xa9, 0xb8, 0xac, 0xec, 0xf6, 0xde, 0xb6, 0x51, 0xc2, 0x2e, 0xb3, 0x65, 
	0x40, 0x03, 0x9b, 0x27, 0xaa, 0xa6, 0x5e, 0x25, 0x39, 0x24, 0xd2, 0xf1, 0xae, 0x5e, 0xe7, 0xe9, 
	0x07, 0xeb, 0x70, 0x49, 0x5f, 0x49, 0x10, 0x52, 0x93, 0xc4, 0x1c, 0xae, 0x58, 0xdc, 0xf2, 0xd5, 
	0x72, 0xa4, 0x9f, 0x27, 0xaf, 0xe1, 0xf7, 0x33, 0xbf, 0x63, 0x4f, 0x95, 0x6e, 0xbc, 0x0b, 0xc4, 
	0x58, 0x12, 0xb9, 0xb9, 0x2e, 0xa3, 0x7c, 0xc6, 0x58, 0x06, 0x14, 0x20, 0x5d, 0x31, 0x7d, 0xf4, 
	0xfe, 0x2f, 0xbc, 0x35, 0x8f, 0x1a, 0xfb, 0xc8, 0x90, 0xf5, 0x4b, 0xbd, 0x04, 0x54, 0xbc, 0x33, 
	0x52, 0xf6, 0xba, 0x27, 0x4a, 0xfb, 0xa0, 0x51, 0x69, 0x7c, 0x28, 0xe2, 0x97, 0x5a, 0x3e, 0x14, 
	0x4f, 0x9e, 0x21, 0x5f, 0x43, 0xd1, 0x4b, 0x35, 0xee, 0x36, 0x32, 0x46, 0x14, 0x8c, 0xc6, 0xef, 
	0xe5, 0x48, 0x12, 0x07, 0x98, 0x9a, 0xa4, 0xb3, 0x21, 0x50, 0xbb, 0xac, 0xa8, 0xba, 0xd5, 0x50, 
	0x38, 0xa4, 0x81, 0xa3, 0xc5, 0x9e, 0x74, 0xf0, 0xd7, 0xe3, 0xcc, 0xec, 0xcc, 0xa5, 0x40, 0x67, 
	0x2a, 0xfd, 0x94, 0x8a, 0xd5, 0x28, 0x18, 0xf3, 0x85, 0x20, 0xff, 0xb4, 0x7f, 0x33, 0xa7, 0x5b, 
	0xac, 0x20, 0x5d, 0xe5, 0x95, 0xed, 0xf4, 0x52, 0x5c, 0x0c, 0xc2, 0xd8, 0x69, 0xb5, 0x4c, 0x17, 
	0x49, 0x04, 0x34, 0x2f, 0x36, 0x3d, 0x31, 0x83, 0x36, 0xa3, 0xaf, 0xe6, 0x8e, 0x20, 0x20, 0x5b, 
	0x0c, 0xe7, 0x1a, 0xc2, 0xea, 0x63, 0x15, 0x76, 0x06, 0x5f, 0xdc, 0xf3, 0x32, 0x32, 0x27, 0xdb, 
	0x92, 0x83, 0xc4, 0x07, 0x84, 0xdf, 0x40, 0xef, 0xfa, 0xeb, 0xf7, 0xd9, 0xb5, 0xb2, 0x8c, 0xed, 
	0x71, 0x14, 0xeb, 0x5a, 0x14, 0x93, 0x0f, 0xdd, 0x6c, 0xbe, 0xa8, 0x1c, 0x98, 0x75, 0xd2, 0xa1, 
	0x96, 0xba, 0x29, 0xd8, 0xd5, 0xac, 0x26, 0x14, 0x15, 0x26, 0xe1, 0xbd, 0x41, 0xc3, 0x0f, 0xab, 
	0xa6, 0x5d, 0x3e, 0x7d, 0x21, 0x56, 0xa6, 0xc0, 0x4c, 0xd8, 0x34, 0x2f, 0xa1, 0xc5, 0xe7, 0xc8, 
	0xad, 0x35, 0xf6, 0x38, 0x33, 0x16, 0x9d, 0x43, 0x61, 0xe8, 0x7a, 0xe2, 0x35, 0xd4, 0xf0, 0xdc, 
	0x4d, 0xb8, 0xed, 0x2a, 0x91, 0xf9, 0x40, 0xeb, 0xbf, 0x51, 0x4c, 0xef, 0x49, 0x54, 0xdc, 0x4f, 
	0x9b, 0x36, 0xae, 0x30, 0xdf, 0x61, 0x9c, 0x06, 0x03, 0xac, 0x87, 0xec, 0xda, 0x4d, 0xd0, 0x59, 
	0x38, 0x66, 0xda, 0x99, 0xf1, 0x34, 0xfb, 0xec, 0x72, 0x70, 0xa8, 0x49, 0x4d, 0x14, 0xe3, 0xde, 
	0x53, 0x62, 0x71, 0x13, 0x48, 0xb2, 0xf0, 0x6b, 0xd5, 0x57, 0xe2, 0x16, 0xda, 0x37, 0xcd, 0x06, 
	0x8d, 0x05, 0x52, 0x91, 0x7b, 0xcd, 0xf6, 0xad, 0x5b, 0x59, 0x86, 0xa7, 0x7e, 0xaf, 0x59, 0xae, 
	0xf1, 0x6c, 0xef, 0x23, 0xa2, 0x9d, 0x6f, 0xdb, 0xba, 0x53, 0x3f, 0x83, 0x63, 0x39, 0x56, 0x7c, 
	0x4d, 0x28, 0x8f, 0x06, 0x3d, 0xf8, 0x20, 0x51, 0x7f, 0x1e, 0x80, 0xc7, 0x03, 0x6e, 0x0f, 0xe2, 
	0x18, 0xf8, 0xeb, 0x36, 0xdf, 0x19, 0x4e, 0xe5, 0x32, 0xfc, 0x5e, 0x69, 0x11, 0x59, 0xc5, 0x17, 
	0xb1, 0xf8, 0x7a, 0xb0, 0x49, 0xc5, 0x2e, 0x44, 0xf0, 0x27, 0xb4, 0xa2, 0x18, 0xb4, 0xe6, 0xf5, 
	0x2a, 0x19, 0x9a, 0x4b, 0x83, 0xc3, 0x23, 0x8a, 0x27, 0x1e, 0xc1, 0x4b, 0x2c, 0x9a, 0x31, 0x07, 
	0x4d, 0x38, 0xec, 0xc9, 0xc7, 0x3a, 0x7f, 0xa3, 0x93, 0x21, 0xde, 0x9e, 0xe3, 0x23, 0x54, 0x92, 
	0x71, 0x44, 0x5d, 0x00, 0xfc, 0x88, 0xe0, 0xef, 0x00, 0x01, 0xfc, 0x06, 0xe0, 0x0f, 0x00, 0x00, 
	0x1a, 0xfe, 0x66, 0xf3, 0x3e, 0x30, 0x0d, 0x8b, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x59, 0x5a, 
};

static const unsigned char s_CSC1[] __attribute__(   (  aligned( 16 ), section( ".rodata" )  )   ) = {
	0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x01, 0x69, 0x22, 0xde, 0x36, 0x02, 0x00, 0x21, 0x01, 
	0x16, 0x00, 0x00, 0x00, 0x74, 0x2f, 0xe5, 0xa3, 0xe0, 0x04, 0xcf, 0x02, 0x5e, 0x5d, 0x00, 0x26, 
	0x00, 0x2c, 0xe1, 0x00, 0xca, 0x7a, 0x4c, 0x6c, 0x03, 0x8e, 0xcf, 0xf8, 0xa6, 0x81, 0x65, 0xb7, 
	0x0b, 0xba, 0x51, 0x2c, 0x45, 0x87, 0xc2, 0x80, 0x8e, 0x2b, 0x9d, 0x92, 0x42, 0x0f, 0xbb, 0x67, 
	0x26, 0x32, 0x2a, 0xc8, 0xe0, 0xaf, 0xfc, 0xb4, 0x01, 0xa7, 0x86, 0x1d, 0x89, 0xda, 0xb0, 0xe4, 
	0x2a, 0xde, 0x12, 0xdd, 0xe3, 0x5c, 0x3b, 0xd4, 0xa2, 0xca, 0x18, 0xc8, 0xa0, 0x64, 0xfe, 0x49, 
	0xbb, 0xfb, 0xd5, 0xba, 0xa8, 0xdb, 0xb1, 0x1f, 0xc2, 0x2d, 0x8e, 0x14, 0x21, 0x42, 0x5d, 0x7e, 
	0x00, 0xf9, 0x00, 0x31, 0xe1, 0x0e, 0xd5, 0xd1, 0xab, 0x57, 0x3e, 0x7c, 0x26, 0xe6, 0x5b, 0x60, 
	0xaa, 0xab, 0x54, 0xd2, 0x4d, 0x6e, 0x73, 0x50, 0x00, 0x40, 0x16, 0x94, 0xa9, 0x24, 0x06, 0x70, 
	0xd2, 0x3b, 0x85, 0xcb, 0x65, 0x05, 0xc0, 0xf9, 0x95, 0x76, 0xfb, 0x10, 0x99, 0x78, 0x6b, 0x0b, 
	0xdf, 0x45, 0x90, 0xf5, 0x0a, 0x7f, 0x95, 0x44, 0x89, 0x32, 0x8c, 0x2d, 0x2e, 0xf2, 0x06, 0xf6, 
	0x1c, 0xcd, 0xe3, 0x1e, 0x77, 0x6b, 0x54, 0xbd, 0x7e, 0xff, 0x80, 0xa5, 0xc5, 0x5b, 0x9e, 0x54, 
	0x16, 0x76, 0x92, 0xb9, 0x25, 0x32, 0x67, 0x22, 0x94, 0x88, 0x8a, 0x78, 0x04, 0x74, 0xf3, 0x0b, 
	0x0a, 0x9a, 0x07, 0xe5, 0x26, 0xd5, 0x8e, 0x91, 0xa1, 0xae, 0x8a, 0x47, 0x09, 0x68, 0x5d, 0x7a, 
	0xc6, 0xb8, 0x62, 0x50, 0x4a, 0x58, 0x3b, 0xef, 0x98, 0x87, 0xcb, 0x5f, 0xb4, 0x80, 0x22, 0xf2, 
	0xd4, 0x5c, 0x92, 0xbe, 0xea, 0xf9, 0xfe, 0xe1, 0x6c, 0xed, 0x39, 0x35, 0x8e, 0x3b, 0xe0, 0xc6, 
	0xc2, 0x2f, 0x99, 0x6a, 0xa6, 0x10, 0x84, 0x13, 0x62, 0x19, 0x65, 0xf6, 0x1e, 0x53, 0xf7, 0x04, 
	0x15, 0xdc, 0x02, 0x71, 0xe0, 0x65, 0xec, 0xd0, 0x60, 0x10, 0xcd, 0x8c, 0xa2, 0xb8, 0xd8, 0x9f, 
	0xb9, 0xa9, 0x12, 0x1e, 0x86, 0xbc, 0x76, 0x7f, 0xc6, 0xf8, 0x0e, 0x51, 0xe3, 0x57, 0xad, 0xb9, 
	0x49, 0xba, 0x01, 0x8a, 0x4c, 0x78, 0x8a, 0x8a, 0xf1, 0x7b, 0xf7, 0x25, 0xd8, 0x33, 0xfd, 0x55, 
	0x39, 0x03, 0x0a, 0xf3, 0x0b, 0x8b, 0xa1, 0x37, 0x5b, 0x9a, 0x27, 0xe2, 0x15, 0x43, 0x4f, 0xfe, 
	0x7f, 0xa6, 0xfc, 0xb2, 0x10, 0xbd, 0x4a, 0x7c, 0x04, 0x7f, 0xd9, 0xbc, 0x51, 0x07, 0x04, 0xfa, 
	0x39, 0x2e, 0x70, 0xe0, 0x6c, 0xf5, 0x59, 0x96, 0xf3, 0xdd, 0xa5, 0x37, 0x93, 0xc0, 0x0a, 0x9d, 
	0x47, 0x86, 0xbc, 0x30, 0xd2, 0xaa, 0xee, 0xbd, 0x9b, 0x63, 0x5e, 0x0b, 0x4e, 0x89, 0xc1, 0x10, 
	0x1e, 0x1c, 0x74, 0x56, 0x2a, 0x35, 0xa1, 0x38, 0x8e, 0x9e, 0x83, 0xad, 0xfc, 0x68, 0x95, 0xdd, 
	0x97, 0xd7, 0x82, 0x0c, 0xc4, 0x6f, 0x82, 0x46, 0x4b, 0x5d, 0xdb, 0x72, 0xb0, 0xbb, 0x43, 0x06, 
	0x8c, 0x79, 0x8e, 0xbf, 0xc0, 0x12, 0x5b, 0x09, 0x2e, 0xdb, 0x79, 0xa4, 0xdd, 0xac, 0x0e, 0x81, 
	0x6a, 0x5c, 0x8c, 0x15, 0x22, 0x5d, 0x7d, 0xfa, 0xe3, 0x09, 0xd6, 0xc9, 0x06, 0xed, 0x62, 0x32, 
	0x6d, 0x9b, 0x82, 0x50, 0x0c, 0x03, 0x00, 0x2e, 0x1f, 0x55, 0x4f, 0xda, 0x1d, 0x58, 0x2e, 0x46, 
	0x72, 0xb7, 0x3c, 0xa7, 0x57, 0x01, 0x25, 0x23, 0xd0, 0xbc, 0x07, 0x01, 0x5d, 0xfd, 0xd3, 0x69, 
	0xd5, 0x11, 0xa0, 0xce, 0x89, 0xdb, 0x89, 0xeb, 0x7c, 0xb8, 0xa2, 0x89, 0x43, 0xe5, 0xe4, 0xec, 
	0xa6, 0x32, 0x30, 0xef, 0xed, 0xfb, 0xf2, 0xc8, 0x8b, 0x97, 0x81, 0x1e, 0x83, 0xa3, 0x2c, 0x77, 
	0x16, 0x3a, 0x5d, 0xd4, 0x4e, 0xbb, 0x5b, 0x44, 0x65, 0x16, 0x99, 0xed, 0x14, 0x7b, 0xa6, 0x3e, 
	0xfb, 0x65, 0xf1, 0xc1, 0x36, 0xe1, 0x2d, 0x55, 0x50, 0xb1, 0xeb, 0xf4, 0x8a, 0x1f, 0xfe, 0xcf, 
	0x7a, 0xe1, 0xf0, 0xab, 0xc4, 0x9b, 0x8c, 0xcf, 0x2e, 0xb2, 0x2d, 0x6f, 0xd4, 0xfa, 0x55, 0x8b, 
	0x4c, 0x8d, 0xd8, 0x73, 0x83, 0xe0, 0xee, 0x0f, 0xa7, 0xc3, 0xc5, 0x48, 0xc9, 0x3b, 0x48, 0x99, 
	0x1b, 0x43, 0x2c, 0x46, 0x84, 0x0e, 0x0d, 0x8d, 0x78, 0x48, 0x17, 0x8c, 0xb5, 0xc4, 0xb0, 0xe6, 
	0x32, 0x66, 0xda, 0x01, 0xaa, 0x70, 0xb7, 0xa2, 0x0c, 0xf7, 0x68, 0x77, 0xea, 0x8c, 0x15, 0x04, 
	0xb5, 0xa4, 0x53, 0x67, 0x3e, 0x7c, 0x89, 0x65, 0x42, 0xd5, 0x04, 0xc5, 0x21, 0xb5, 0xcf, 0xa1, 
	0xe8, 0xfa, 0xa5, 0x3a, 0xc4, 0x66, 0x1d, 0xe7, 0x1e, 0x34, 0xb8, 0x93, 0xc4, 0x00, 0x00, 0x00, 
	0x66, 0x3e, 0xe0, 0x62, 0x00, 0x01, 0xf6, 0x04, 0xd0, 0x09, 0x00, 0x00, 0x0f, 0x0a, 0x9b, 0xdc, 
	0x3e, 0x30, 0x0d, 0x8b, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x59, 0x5a,
};

static const unsigned char s_CSC2h[] __attribute__(   (  aligned( 16 ), section( ".rodata" )  )   ) = {
	0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x01, 0x69, 0x22, 0xde, 0x36, 0x02, 0x00, 0x21, 0x01, 
	0x16, 0x00, 0x00, 0x00, 0x74, 0x2f, 0xe5, 0xa3, 0xe0, 0x08, 0x4f, 0x04, 0x10, 0x5d, 0x00, 0x42, 
	0x00, 0x2c, 0xe0, 0x00, 0xd1, 0x02, 0x5f, 0x68, 0x03, 0x8f, 0xae, 0xcc, 0x87, 0x95, 0xea, 0xba, 
	0x40, 0x47, 0x29, 0x93, 0x10, 0xa0, 0x5c, 0x52, 0x93, 0x58, 0x3a, 0x67, 0x8f, 0x0f, 0x02, 0xc5, 
	0x92, 0xe5, 0xa2, 0x35, 0xf1, 0xab, 0x48, 0x1a, 0x2a, 0x16, 0x6d, 0xa0, 0x7c, 0x4e, 0x6c, 0xd8, 
	0xff, 0x49, 0xce, 0xc1, 0x2f, 0x43, 0x58, 0x6e, 0xf1, 0xc0, 0x18, 0xcd, 0x61, 0x90, 0x69, 0x13, 
	0xdd, 0x09, 0x4d, 0x75, 0x9f, 0xb5, 0x28, 0xcb, 0x7f, 0xad, 0x22, 0x9f, 0x7c, 0x63, 0x52, 0xf9, 
	0xeb, 0x3a, 0x7b, 0xbd, 0x15, 0xc2, 0xfd, 0x78, 0x99, 0x38, 0x35, 0xe0, 0xbf, 0xec, 0xb1, 0x79, 
	0xc7, 0xfc, 0x66, 0xce, 0xf9, 0xdf, 0x82, 0x3d, 0x41, 0xeb, 0x54, 0x89, 0xf4, 0xf5, 0xda, 0xf7, 
	0xb3, 0x01, 0x19, 0x12, 0x43, 0xa9, 0x28, 0x86, 0x03, 0xea, 0x5f, 0x4e, 0x96, 0x87, 0xa3, 0x2c, 
	0xc2, 0x20, 0x77, 0xb4, 0xe9, 0x49, 0xcb, 0x2b, 0x76, 0xb3, 0x5b, 0xd2, 0x08, 0x93, 0x4e, 0x3a, 
	0x52, 0x53, 0x56, 0xfd, 0x03, 0x2a, 0x23, 0x99, 0x39, 0x13, 0x93, 0x08, 0x4d, 0x2d, 0x90, 0x88, 
	0x10, 0xc3, 0x88, 0x41, 0x6b, 0xd7, 0x9c, 0xd7, 0xe5, 0x60, 0xd3, 0xf5, 0x25, 0x47, 0xef, 0x58, 
	0x83, 0xaa, 0xb8, 0x07, 0xcf, 0x47, 0xc5, 0x0a, 0x12, 0x73, 0x01, 0xa7, 0x28, 0xf2, 0x89, 0xa8, 
	0x72, 0x29, 0xdd, 0xbb, 0x92, 0x63, 0x21, 0x20, 0x54, 0xb2, 0xe7, 0x20, 0x05, 0xdd, 0x25, 0x83, 
	0xe7, 0x9a, 0x97, 0xa2, 0xeb, 0x4e, 0x40, 0x49, 0x25, 0x4c, 0xbd, 0x9e, 0x45, 0x1c, 0xe2, 0x71, 
	0x77, 0x46, 0xf7, 0x71, 0x93, 0x41, 0x31, 0x44, 0xa5, 0x33, 0x2f, 0x09, 0x5b, 0x54, 0x35, 0xed, 
	0xd2, 0xa0, 0xd8, 0xd1, 0x3a, 0x18, 0x88, 0x47, 0x60, 0xa4, 0xe1, 0xa7, 0x6d, 0x0a, 0xb3, 0x95, 
	0xc3, 0xc6, 0x83, 0xe9, 0xe0, 0xbf, 0xd2, 0x97, 0x98, 0x08, 0xcb, 0x51, 0x93, 0x60, 0x40, 0xcb, 
	0x6d, 0xbd, 0x40, 0xdc, 0x3c, 0x09, 0xa6, 0x78, 0x7e, 0x39, 0xc4, 0x19, 0xcd, 0x4e, 0x04, 0x39, 
	0x7a, 0xb1, 0x09, 0xb3, 0xe5, 0x88, 0x93, 0xa6, 0x76, 0xd9, 0xcb, 0x4f, 0x8a, 0x2b, 0xb2, 0x8f, 
	0x63, 0x21, 0xc2, 0x30, 0x7d, 0xfb, 0xab, 0xa1, 0x0e, 0xd9, 0x6e, 0x50, 0xc0, 0xda, 0xdc, 0xd7, 
	0x88, 0x54, 0xec, 0xfd, 0x66, 0x2b, 0x1e, 0x12, 0x25, 0xdf, 0x34, 0xf9, 0xa6, 0x1e, 0x2b, 0xb3, 
	0xe0, 0xd3, 0x6f, 0x2e, 0xcd, 0x1c, 0x2a, 0xdc, 0xeb, 0xc0, 0x05, 0xea, 0x74, 0xbe, 0x77, 0x33, 
	0xb3, 0x51, 0x53, 0xf4, 0x22, 0x16, 0x6e, 0xfe, 0x88, 0x64, 0x11, 0xf0, 0xd9, 0x32, 0x2b, 0x67, 
	0xbd, 0xd8, 0x28, 0xef, 0xc7, 0x66, 0xf6, 0x42, 0x09, 0xc3, 0x85, 0x74, 0x49, 0x7b, 0x7f, 0x73, 
	0xd1, 0x40, 0x5c, 0xa4, 0x4f, 0x3a, 0xbc, 0x90, 0x8f, 0xa5, 0x65, 0x95, 0xa3, 0xff, 0x14, 0x71, 
	0x4d, 0x82, 0xd9, 0x23, 0x75, 0xc0, 0x5f, 0x24, 0xae, 0x76, 0x4d, 0xc0, 0x02, 0xd4, 0x15, 0x82, 
	0xfc, 0x23, 0x73, 0x49, 0x8d, 0xa3, 0xdd, 0x42, 0xe3, 0xf0, 0xe4, 0x06, 0x41, 0x82, 0x4b, 0x32, 
	0xae, 0x69, 0xd2, 0x52, 0xd2, 0x77, 0xeb, 0x70, 0x0b, 0x37, 0xc4, 0xe8, 0xdd, 0x23, 0xbc, 0x40, 
	0x25, 0x8f, 0xde, 0x67, 0xf3, 0x6c, 0x01, 0x0d, 0x2d, 0x13, 0xf8, 0x00, 0xbb, 0x06, 0xa7, 0x00, 
	0x51, 0x68, 0x19, 0xc5, 0xaa, 0x7d, 0xa5, 0xde, 0x83, 0x4f, 0x32, 0xd1, 0xf0, 0xdf, 0x48, 0xf7, 
	0x29, 0x14, 0x27, 0x3f, 0xcf, 0x43, 0x34, 0xaf, 0x37, 0xc3, 0x0f, 0x2d, 0x64, 0x30, 0xb5, 0x26, 
	0xed, 0x14, 0x18, 0x8a, 0xfa, 0x99, 0xf0, 0x69, 0x5a, 0x3f, 0xe0, 0xec, 0x7a, 0xc2, 0xe0, 0xbd, 
	0xf7, 0xea, 0xdb, 0x51, 0x28, 0xe5, 0xfd, 0x6f, 0xa6, 0xfc, 0x77, 0xa7, 0xa0, 0xeb, 0xe8, 0x25, 
	0xc9, 0x3b, 0xbf, 0xff, 0xe5, 0x8d, 0xa2, 0x17, 0x4d, 0xa6, 0xa5, 0x5e, 0x5d, 0x90, 0x03, 0xa3, 
	0x84, 0x48, 0xfa, 0xc2, 0xf7, 0x77, 0x86, 0x25, 0xeb, 0xb1, 0x6e, 0x37, 0xa9, 0x67, 0xc3, 0xbd, 
	0x30, 0xd4, 0x3c, 0xea, 0x39, 0x38, 0x6d, 0xfd, 0x45, 0x78, 0xb0, 0x56, 0xbb, 0xd5, 0xf2, 0xca, 
	0xfb, 0x23, 0x2b, 0xaa, 0xf1, 0x29, 0xb5, 0x60, 0x09, 0xdc, 0x07, 0x2c, 0x96, 0x8b, 0xdc, 0xdf, 
	0xd8, 0x84, 0x2f, 0xfd, 0x1a, 0x2c, 0x59, 0xfc, 0x54, 0x5c, 0x08, 0x6c, 0xa7, 0x60, 0x51, 0xf3, 
	0x2c, 0x61, 0x03, 0x0a, 0x82, 0x62, 0xb8, 0x56, 0xbd, 0x91, 0xea, 0x4e, 0x82, 0x0c, 0x11, 0xd6, 
	0xc1, 0x0a, 0xb8, 0xae, 0x1b, 0x50, 0x33, 0x9c, 0x24, 0x28, 0xfd, 0xe7, 0x7e, 0x36, 0xb2, 0xf5, 
	0x9d, 0xe3, 0x63, 0xe0, 0x76, 0x60, 0x8b, 0x33, 0x31, 0xdd, 0xb1, 0xef, 0x69, 0xef, 0xab, 0xca, 
	0x37, 0x54, 0x22, 0x3a, 0x0b, 0x66, 0x47, 0xef, 0xa7, 0xb1, 0x5b, 0x46, 0x44, 0xa8, 0x90, 0x41, 
	0xe7, 0xe1, 0x9e, 0xbf, 0xf7, 0xe8, 0x10, 0x37, 0x5c, 0x03, 0xbc, 0x96, 0xa0, 0x90, 0xe3, 0x5c, 
	0x7f, 0x4a, 0xbe, 0x82, 0x73, 0x1e, 0x49, 0xad, 0x23, 0x65, 0xf0, 0x2a, 0x81, 0x48, 0x8c, 0xb8, 
	0x96, 0x59, 0x08, 0x5f, 0xe0, 0xd1, 0x9a, 0xdd, 0xba, 0x53, 0xd7, 0x04, 0x70, 0xfc, 0xc1, 0x6f, 
	0x7e, 0x4d, 0x20, 0xa5, 0x28, 0x41, 0xc3, 0xdb, 0x31, 0xd7, 0x44, 0x85, 0x67, 0x49, 0xaf, 0x6a, 
	0xea, 0xd7, 0xb7, 0xda, 0x82, 0x32, 0x1d, 0x92, 0x09, 0x70, 0xa6, 0x1e, 0xf7, 0x1a, 0xc4, 0xfc, 
	0x58, 0x70, 0x8e, 0x04, 0x1e, 0xb4, 0x26, 0xce, 0x4c, 0xf6, 0xfa, 0x14, 0x8c, 0x97, 0x41, 0x0f, 
	0x6f, 0x89, 0x63, 0x7b, 0xc1, 0x5e, 0xb0, 0x3a, 0x85, 0xd0, 0x6c, 0x82, 0x5b, 0xc5, 0x4e, 0x44, 
	0x62, 0x0a, 0xa6, 0x07, 0xcf, 0xb5, 0x6d, 0x42, 0x8a, 0xcd, 0x2c, 0xa9, 0x36, 0x7d, 0x72, 0xfa, 
	0xe6, 0xb7, 0x52, 0x76, 0x79, 0xd1, 0x23, 0xa7, 0x5e, 0x37, 0xab, 0xfe, 0x94, 0xa9, 0x06, 0x13, 
	0xeb, 0x4e, 0xae, 0xdd, 0x31, 0x6a, 0xd9, 0x6b, 0x76, 0xbd, 0xb9, 0x98, 0xa4, 0x8c, 0xea, 0x26, 
	0xcb, 0xe9, 0x4f, 0x5a, 0x87, 0x87, 0x0a, 0x8b, 0x02, 0xf2, 0x9b, 0xa7, 0x2f, 0xc3, 0xdc, 0x10, 
	0x43, 0x23, 0xd8, 0xce, 0xec, 0x73, 0x20, 0x77, 0x75, 0x71, 0x20, 0x84, 0xaa, 0x8b, 0x02, 0xf2, 
	0xed, 0x9e, 0xbb, 0x5c, 0x6b, 0xe7, 0x07, 0x21, 0x22, 0x67, 0x10, 0xc4, 0x3f, 0x05, 0x91, 0x07, 
	0xaa, 0x4b, 0x82, 0xd9, 0x14, 0x29, 0xda, 0xc7, 0xf0, 0xf8, 0x0a, 0x2d, 0x48, 0x70, 0x41, 0x07, 
	0xa5, 0x6d, 0x1f, 0x22, 0x7c, 0xe3, 0xdd, 0x69, 0x8b, 0xc7, 0x02, 0x2c, 0xe6, 0x92, 0xd1, 0x5b, 
	0x20, 0xdd, 0xf1, 0x4c, 0xcd, 0x43, 0x2c, 0xcb, 0x69, 0x1b, 0x7b, 0xe8, 0x70, 0x97, 0xb0, 0xb3, 
	0x08, 0x89, 0xa0, 0x2d, 0xcb, 0x43, 0xaa, 0x98, 0x98, 0x6c, 0x33, 0xb6, 0x0a, 0x4c, 0x7b, 0xfe, 
	0x90, 0xea, 0x40, 0x4f, 0x28, 0x72, 0x28, 0x22, 0x8e, 0x3e, 0x94, 0xfb, 0x07, 0x14, 0xaf, 0x2a, 
	0x27, 0x69, 0xd6, 0x77, 0xa5, 0xcf, 0x34, 0x02, 0x91, 0x74, 0x61, 0xb4, 0x0c, 0xfe, 0xb8, 0x7c, 
	0x53, 0xc5, 0xf0, 0xa2, 0xb3, 0x5a, 0xb8, 0x99, 0x55, 0x90, 0x1c, 0x31, 0xb2, 0xf2, 0x5d, 0x80, 
	0xd7, 0x4e, 0x0e, 0xac, 0x6a, 0xc8, 0xb4, 0xf8, 0xbe, 0xb7, 0xe2, 0xd6, 0x61, 0x75, 0x08, 0xae, 
	0x74, 0x46, 0xb4, 0x7d, 0x36, 0x5d, 0x32, 0xe8, 0xfc, 0xa9, 0x65, 0x91, 0x4e, 0xc1, 0xe1, 0x9b, 
	0x09, 0x89, 0xc8, 0xe3, 0x8d, 0x90, 0xfe, 0xef, 0x33, 0x5f, 0xd1, 0xb1, 0x08, 0x37, 0xa1, 0x00, 
	0xcc, 0x35, 0x68, 0xde, 0x00, 0x01, 0xa8, 0x08, 0xd0, 0x10, 0x00, 0x00, 0x76, 0x16, 0x18, 0x3c, 
	0x3e, 0x30, 0x0d, 0x8b, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x59, 0x5a, 
};

static const unsigned char s_CSC2v[] __attribute__(   (  aligned( 16 ), section( ".rodata" )  )   ) = {
	0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x01, 0x69, 0x22, 0xde, 0x36, 0x02, 0x00, 0x21, 0x01, 
	0x16, 0x00, 0x00, 0x00, 0x74, 0x2f, 0xe5, 0xa3, 0xe0, 0x06, 0x5f, 0x03, 0x48, 0x5d, 0x00, 0x32, 
	0x80, 0x2c, 0xe0, 0x09, 0x0b, 0x11, 0x2c, 0x27, 0x04, 0xb1, 0x4c, 0xd4, 0xfc, 0xe3, 0xd7, 0x6a, 
	0x13, 0xd6, 0xe9, 0xe1, 0x01, 0xba, 0x64, 0xf8, 0x0a, 0x89, 0x10, 0x9b, 0x18, 0x2d, 0xb6, 0x63, 
	0xb4, 0xea, 0x25, 0x1c, 0x3f, 0x03, 0x7d, 0x92, 0x84, 0x16, 0xb5, 0x77, 0x93, 0xa3, 0xe0, 0xb3, 
	0xe2, 0xd1, 0xaf, 0x3c, 0x6b, 0x6e, 0xf7, 0xd6, 0xb0, 0x7a, 0x90, 0xbb, 0xc4, 0x42, 0x1d, 0xf8, 
	0x02, 0xd4, 0x36, 0xb4, 0xaa, 0x98, 0x9f, 0x19, 0x46, 0x9d, 0xfe, 0xc7, 0xce, 0x51, 0x7a, 0xc6, 
	0x33, 0x09, 0x19, 0xce, 0x7f, 0xf8, 0x5b, 0xec, 0x5c, 0xb8, 0x87, 0xa3, 0xd5, 0xf4, 0x4b, 0x5b, 
	0x01, 0x99, 0xb2, 0xa7, 0xb6, 0x33, 0x48, 0x22, 0xd6, 0x16, 0xc8, 0x80, 0xfc, 0x95, 0x35, 0x21, 
	0x1e, 0x27, 0x5e, 0xb7, 0xc9, 0x6c, 0x80, 0x63, 0x94, 0x75, 0x6d, 0xee, 0x2d, 0xb2, 0xbe, 0xc5, 
	0x96, 0x89, 0x1b, 0xaa, 0x72, 0x26, 0xe4, 0x0c, 0x1e, 0x38, 0x28, 0x79, 0x66, 0xec, 0x5e, 0xc1, 
	0x3b, 0x81, 0xd7, 0xd5, 0xff, 0xf9, 0xa2, 0xd5, 0x66, 0x1e, 0x4f, 0xc9, 0x14, 0x2d, 0xfd, 0x79, 
	0xa1, 0xf9, 0x88, 0x79, 0x19, 0x06, 0x0e, 0xd0, 0x26, 0x87, 0x9b, 0xe5, 0x9f, 0xf0, 0x21, 0x55, 
	0xd9, 0xd9, 0xd5, 0x8e, 0xef, 0x93, 0xc3, 0xc4, 0xb7, 0xda, 0xc0, 0xd1, 0x9e, 0x8e, 0xa8, 0x49, 
	0x8a, 0x0d, 0xd5, 0xde, 0x5a, 0xdf, 0xa0, 0x0d, 0x42, 0x02, 0x01, 0xcf, 0x78, 0xcc, 0xec, 0x1e, 
	0x9a, 0x9d, 0x9f, 0x15, 0x08, 0xb7, 0x1d, 0xc0, 0x8d, 0x26, 0x94, 0xca, 0xd6, 0x21, 0xe6, 0xe9, 
	0x08, 0xa7, 0x25, 0x60, 0x65, 0xc6, 0x31, 0x0a, 0xf6, 0x89, 0x97, 0x8e, 0xdd, 0xfc, 0xcc, 0x9d, 
	0x13, 0x6b, 0x40, 0x99, 0xa5, 0x78, 0x7e, 0xce, 0x99, 0x24, 0x31, 0xf5, 0x56, 0x0f, 0x62, 0x36, 
	0x58, 0x65, 0x63, 0x45, 0x4e, 0xc9, 0xd2, 0xb2, 0xcd, 0x55, 0x78, 0x38, 0xae, 0x95, 0x94, 0x6e, 
	0x22, 0x7b, 0x44, 0x6e, 0xbf, 0x94, 0x68, 0xe6, 0xbb, 0xec, 0x58, 0x35, 0x7a, 0xd0, 0xb4, 0x52, 
	0xca, 0x77, 0xf2, 0x76, 0x03, 0xd6, 0x70, 0x18, 0xe1, 0xcf, 0x52, 0xd8, 0x4e, 0x49, 0xdb, 0x4c, 
	0x31, 0xc7, 0x26, 0x0e, 0x47, 0xb8, 0x1c, 0xef, 0xa1, 0xe5, 0xc5, 0xb1, 0x0c, 0xe9, 0x43, 0xf9, 
	0x11, 0xb0, 0x10, 0x03, 0x6e, 0x9d, 0x6a, 0x72, 0x42, 0x7e, 0xaa, 0xc9, 0x01, 0x69, 0x73, 0xf5, 
	0x9c, 0xee, 0x58, 0xd0, 0xcb, 0xd0, 0xdd, 0x4a, 0x99, 0x8f, 0xbe, 0x35, 0x1c, 0x45, 0x96, 0x4f, 
	0xdc, 0x23, 0x68, 0xe4, 0x07, 0xb2, 0x3c, 0xa3, 0xd9, 0xf2, 0x9d, 0x24, 0xe0, 0x5b, 0x57, 0x81, 
	0xae, 0x4c, 0xe2, 0x31, 0x14, 0x23, 0x99, 0x9f, 0xe8, 0x10, 0xfe, 0x62, 0x76, 0x70, 0x7a, 0x25, 
	0x19, 0x4f, 0x1f, 0x17, 0x15, 0xcb, 0xc7, 0x4f, 0xee, 0xd9, 0xce, 0xe3, 0x17, 0x66, 0xa6, 0x19, 
	0xdf, 0xfa, 0xda, 0x9c, 0x89, 0xf1, 0xdc, 0xeb, 0xee, 0x16, 0xa6, 0xd2, 0x34, 0xf7, 0x1e, 0xcb, 
	0x7b, 0xd8, 0xf0, 0x3c, 0x14, 0x70, 0x33, 0xee, 0x95, 0x2a, 0xb1, 0x86, 0x1b, 0x3b, 0x0c, 0x68, 
	0x40, 0xc7, 0x63, 0x6e, 0x11, 0x3a, 0xae, 0xd8, 0x60, 0xe4, 0xe0, 0xa4, 0x2a, 0x15, 0xfa, 0xcd, 
	0xaa, 0xcc, 0x17, 0x8c, 0xbf, 0x0c, 0x00, 0x5e, 0xcc, 0x15, 0x49, 0x82, 0x8c, 0x1c, 0x12, 0x5b, 
	0x1a, 0x1c, 0xcd, 0xce, 0xad, 0xdf, 0x07, 0x78, 0xe8, 0x1f, 0x85, 0x4b, 0x8c, 0xef, 0x28, 0xe8, 
	0x2e, 0x18, 0x3f, 0xd0, 0xd0, 0x90, 0x01, 0x42, 0x9a, 0x22, 0x8d, 0xce, 0xe4, 0x5e, 0x9b, 0x27, 
	0x2a, 0x10, 0xa9, 0x18, 0xa9, 0xb1, 0xee, 0x19, 0x99, 0xfb, 0x6d, 0x1e, 0x4b, 0x88, 0x99, 0xc3, 
	0xc7, 0xdb, 0xf4, 0x41, 0x5b, 0xa5, 0x56, 0x05, 0xaa, 0x7c, 0xc4, 0x62, 0xdd, 0xd7, 0x8d, 0x4a, 
	0xc9, 0xdf, 0xe1, 0x7e, 0xbb, 0xde, 0xa2, 0xf0, 0x20, 0x80, 0xf9, 0xf7, 0xb7, 0x6b, 0xe9, 0x15, 
	0x18, 0x0e, 0x36, 0x34, 0x00, 0x1e, 0xb1, 0x94, 0x8f, 0xbb, 0x94, 0x79, 0xbb, 0x9a, 0xde, 0x8b, 
	0xaa, 0xdb, 0xfb, 0x8e, 0x52, 0x83, 0x94, 0x30, 0x2a, 0x43, 0x8d, 0x04, 0xce, 0x7e, 0x17, 0xde, 
	0xad, 0x31, 0x4d, 0x01, 0x71, 0x45, 0x24, 0xfd, 0x6a, 0x1b, 0x96, 0x9a, 0xae, 0x78, 0x78, 0x43, 
	0x73, 0xfa, 0xf3, 0x02, 0x07, 0x38, 0xf1, 0xe9, 0xa7, 0x85, 0xb8, 0x74, 0x31, 0xcd, 0xc7, 0x84, 
	0xf8, 0xee, 0x2e, 0xe4, 0x29, 0x88, 0xaf, 0xd9, 0x9a, 0x2a, 0x6b, 0x30, 0x0f, 0x5f, 0x07, 0x93, 
	0x90, 0xfb, 0xc0, 0x0b, 0x68, 0x68, 0x6d, 0x46, 0x27, 0xc9, 0x78, 0x82, 0x39, 0x94, 0x08, 0x40, 
	0x05, 0x4c, 0x94, 0x6b, 0xee, 0x3c, 0x77, 0x92, 0xae, 0x8e, 0xd9, 0xe0, 0xeb, 0x5c, 0xf9, 0xe0, 
	0x88, 0xfe, 0x15, 0x5b, 0x6c, 0xc2, 0x17, 0xd0, 0x41, 0x43, 0xab, 0xb1, 0x56, 0x57, 0x17, 0x8f, 
	0x25, 0x51, 0xf3, 0x74, 0xa6, 0x3c, 0x33, 0x3c, 0xf5, 0xb5, 0x17, 0xf1, 0x42, 0x6a, 0x81, 0x90, 
	0xba, 0xe4, 0x33, 0x57, 0xea, 0xac, 0xb2, 0x83, 0xa3, 0x78, 0x12, 0x74, 0xed, 0x13, 0xf2, 0x95, 
	0x31, 0x9d, 0xbf, 0xe9, 0x2c, 0x15, 0xa5, 0x38, 0x57, 0x04, 0xca, 0x7f, 0x05, 0x05, 0x37, 0xca, 
	0x01, 0xf2, 0xc8, 0xd0, 0xf1, 0x12, 0xb7, 0xae, 0x5c, 0xb0, 0xa6, 0x49, 0x09, 0x87, 0xd2, 0x3a, 
	0xed, 0x8f, 0x30, 0x3d, 0xf8, 0x6f, 0xf1, 0x17, 0xab, 0x5c, 0xeb, 0xe9, 0xe2, 0x0b, 0xa3, 0xf5, 
	0xb8, 0xf9, 0xfb, 0x78, 0xa9, 0x72, 0x19, 0x5f, 0x38, 0x49, 0x00, 0x3a, 0x5e, 0x41, 0xf1, 0x39, 
	0x90, 0xee, 0x0b, 0x5f, 0x47, 0x1e, 0x6f, 0x9d, 0xe7, 0x28, 0xe7, 0x6a, 0x9c, 0x14, 0x30, 0x04, 
	0xb4, 0x55, 0x19, 0x29, 0x21, 0x66, 0xbf, 0x81, 0xe4, 0x90, 0xfd, 0x03, 0xe8, 0x25, 0xea, 0x4d, 
	0x4b, 0x1c, 0x0c, 0xb2, 0x3e, 0x80, 0xf1, 0x6b, 0x60, 0x7c, 0x23, 0x87, 0xd1, 0x9f, 0x0f, 0x15, 
	0x2b, 0x1d, 0xc8, 0x2b, 0xfc, 0xa6, 0xb3, 0xad, 0x03, 0x97, 0xcc, 0x53, 0x6e, 0xc7, 0x58, 0x46, 
	0x76, 0x8a, 0x78, 0x5d, 0x7a, 0x3c, 0x00, 0x00, 0x9e, 0xf0, 0x8c, 0xd7, 0x00, 0x01, 0xe0, 0x06, 
	0xe0, 0x0c, 0x00, 0x00, 0xa3, 0x82, 0x34, 0x85, 0x3e, 0x30, 0x0d, 0x8b, 0x02, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x59, 0x5a, 
};

static const unsigned char s_CSC4[] __attribute__(   (  aligned( 16 ), section( ".rodata" )  )   ) = {
	0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x01, 0x69, 0x22, 0xde, 0x36, 0x02, 0x00, 0x21, 0x01, 
	0x16, 0x00, 0x00, 0x00, 0x74, 0x2f, 0xe5, 0xa3, 0xe0, 0x07, 0x4f, 0x03, 0x97, 0x5d, 0x00, 0x3a, 
	0x00, 0x2c, 0xdd, 0xf9, 0x1c, 0x02, 0xc1, 0x63, 0x04, 0xfd, 0xb0, 0xde, 0x47, 0x39, 0x4e, 0x39, 
	0x05, 0xeb, 0xc0, 0xfa, 0x85, 0x11, 0xc2, 0x33, 0x57, 0x6c, 0xe0, 0xdf, 0x1c, 0x69, 0x1c, 0xe8, 
	0xe4, 0x04, 0x2f, 0xc5, 0x6c, 0x88, 0x81, 0x11, 0xdc, 0x97, 0xaa, 0x08, 0x6c, 0x9e, 0x75, 0xc5, 
	0x28, 0x1c, 0x0a, 0x24, 0x2d, 0x23, 0xfc, 0xc9, 0xb2, 0x42, 0x18, 0x73, 0xf7, 0x7f, 0x36, 0x4a, 
	0x43, 0x8b, 0xbe, 0x95, 0x01, 0x7d, 0xa1, 0x15, 0x85, 0xbc, 0x6f, 0x43, 0xa7, 0x73, 0x08, 0x3c, 
	0x77, 0x1c, 0xd9, 0xf8, 0x2e, 0x3f, 0x48, 0xa3, 0xd2, 0x0b, 0x62, 0x4d, 0xc6, 0x6f, 0x50, 0x1b, 
	0xe8, 0x45, 0x43, 0x20, 0xd3, 0x70, 0x81, 0x72, 0x28, 0xb8, 0xa4, 0xf5, 0x04, 0x3f, 0x89, 0x0a, 
	0xa0, 0x7b, 0x72, 0xf5, 0x24, 0x34, 0x1f, 0x12, 0x30, 0x20, 0x53, 0xe8, 0x45, 0x5a, 0x54, 0x46, 
	0x60, 0x2e, 0x25, 0x97, 0xdf, 0x9a, 0x16, 0x5c, 0x04, 0xc6, 0x67, 0x80, 0x97, 0xaa, 0xfe, 0xb7, 
	0xe7, 0xbe, 0x8c, 0xac, 0xa6, 0x6e, 0x73, 0xd9, 0xb9, 0xa4, 0xde, 0x20, 0xa5, 0x63, 0x52, 0xc2, 
	0x83, 0x63, 0x21, 0x30, 0xdf, 0x7a, 0x7d, 0xb2, 0x01, 0x4b, 0x77, 0x49, 0x0f, 0xe0, 0xbb, 0x40, 
	0x95, 0xed, 0x47, 0x86, 0x78, 0xa8, 0xb4, 0x61, 0x52, 0x11, 0x93, 0x2a, 0x31, 0x28, 0xbf, 0x92, 
	0x12, 0xfa, 0xae, 0x8e, 0x81, 0xf2, 0x66, 0x62, 0x22, 0x54, 0xe6, 0x98, 0xfe, 0x15, 0x97, 0x22, 
	0xed, 0x3a, 0xbe, 0xb0, 0x93, 0xfc, 0x67, 0xcf, 0xd6, 0xf1, 0xbf, 0x87, 0x64, 0x5e, 0xed, 0x4b, 
	0x0e, 0x05, 0x27, 0x29, 0x23, 0xce, 0x96, 0x43, 0x12, 0x86, 0xad, 0x01, 0xff, 0x1a, 0x87, 0xea, 
	0x24, 0xc1, 0xf4, 0x73, 0x9f, 0xbe, 0x02, 0x18, 0x47, 0xd0, 0xa7, 0x8c, 0x8e, 0xf4, 0x51, 0xdf, 
	0xfa, 0xb7, 0x73, 0x1d, 0xb8, 0x12, 0xfc, 0xd3, 0x26, 0xa1, 0xd4, 0xe8, 0x11, 0x19, 0xb0, 0x55, 
	0xb9, 0x3b, 0x2f, 0x26, 0xf1, 0xb6, 0x8a, 0x15, 0x6c, 0xb4, 0xe8, 0x6a, 0x26, 0xe0, 0x02, 0xeb, 
	0x23, 0x23, 0x63, 0xdc, 0x7a, 0xde, 0x60, 0x5c, 0x6b, 0x16, 0xe9, 0xdd, 0x12, 0x02, 0xf3, 0x7c, 
	0xf8, 0xcc, 0x72, 0x77, 0xfc, 0xf9, 0x3a, 0x40, 0x96, 0x55, 0x87, 0xbd, 0xb6, 0xe9, 0xec, 0x31, 
	0x84, 0xb2, 0x13, 0x44, 0xd5, 0xdf, 0x39, 0xef, 0x5b, 0xdf, 0xfa, 0x19, 0x0b, 0x11, 0x0a, 0x9e, 
	0x46, 0x75, 0xeb, 0x75, 0x91, 0x70, 0x87, 0x8b, 0xe0, 0x40, 0xb5, 0x4e, 0x3e, 0xc2, 0x33, 0x64, 
	0xe6, 0xc4, 0x35, 0x01, 0xee, 0xa7, 0x87, 0x32, 0xe0, 0x81, 0x5b, 0xe5, 0x0d, 0xb6, 0xad, 0x60, 
	0xe5, 0x29, 0x04, 0x21, 0x23, 0x21, 0xc3, 0xf7, 0x33, 0xf4, 0xc2, 0x31, 0xf9, 0x9a, 0xee, 0xe2, 
	0x90, 0x34, 0x9b, 0xa1, 0xed, 0xb1, 0x2b, 0x1a, 0x6a, 0x7e, 0xe4, 0xd3, 0x1c, 0xe8, 0x78, 0x91, 
	0x30, 0x7e, 0xd6, 0x1a, 0xdd, 0x4c, 0x44, 0xea, 0xbf, 0xe8, 0xd4, 0xeb, 0x52, 0x9e, 0x18, 0x38, 
	0x67, 0xb4, 0x45, 0xa2, 0xe6, 0x01, 0x5e, 0x6b, 0x4d, 0x9d, 0x4e, 0x8e, 0xdd, 0x3e, 0x1b, 0x4d, 
	0x91, 0xb0, 0x41, 0xd2, 0x67, 0x98, 0x7c, 0x13, 0x17, 0x45, 0xac, 0x87, 0x5f, 0x21, 0x7d, 0x5b, 
	0x17, 0xa9, 0xc4, 0x4a, 0x0f, 0x7d, 0x30, 0x78, 0xb9, 0x39, 0x67, 0x96, 0xc2, 0xf3, 0x78, 0xb2, 
	0xc9, 0xe5, 0x0f, 0x6c, 0xbc, 0x3e, 0x43, 0x3d, 0xb0, 0xb7, 0x70, 0x92, 0xe5, 0x13, 0x70, 0xd9, 
	0x96, 0x58, 0xa1, 0x6b, 0x9d, 0x97, 0x88, 0x31, 0xaf, 0x22, 0xc3, 0xfb, 0x9d, 0xa9, 0x9c, 0xc5, 
	0x58, 0x62, 0xf9, 0x50, 0xb6, 0xf5, 0xf9, 0xfc, 0xa0, 0xf8, 0x0e, 0x2d, 0x67, 0xc3, 0xb3, 0xd0, 
	0x48, 0xe8, 0x73, 0x87, 0xca, 0x6f, 0xb3, 0x44, 0xed, 0x45, 0xfd, 0xa7, 0x9f, 0xc0, 0x28, 0x88, 
	0x21, 0x79, 0x78, 0xa3, 0x63, 0x75, 0xf7, 0xea, 0x1d, 0x1e, 0xe8, 0x03, 0x5b, 0x37, 0xca, 0xd6, 
	0x77, 0xe8, 0xe7, 0x73, 0x25, 0xf0, 0xc7, 0xde, 0x44, 0x27, 0x36, 0xd7, 0x36, 0x9a, 0x6f, 0x7d, 
	0xbe, 0x82, 0x01, 0x21, 0x46, 0x14, 0xfa, 0x73, 0x4a, 0xa3, 0x7a, 0xa8, 0x65, 0xd3, 0x0a, 0x0f, 
	0x04, 0xaf, 0xb7, 0xac, 0x20, 0x0c, 0x53, 0x47, 0x46, 0x8b, 0x2a, 0x1a, 0xd6, 0xed, 0x90, 0x81, 
	0x5d, 0xb9, 0xaf, 0xf2, 0x7b, 0x19, 0xbb, 0x7f, 0xdb, 0xd9, 0xac, 0xac, 0xa8, 0x44, 0x44, 0xf3, 
	0xc3, 0x7a, 0xbf, 0xf1, 0xd8, 0x68, 0xd7, 0xe8, 0x1a, 0xec, 0xfe, 0xd8, 0x63, 0x9d, 0x4f, 0xe9, 
	0x8e, 0x81, 0x53, 0x0f, 0x63, 0x24, 0x6b, 0x3c, 0x94, 0x59, 0x67, 0xdf, 0xbe, 0x0d, 0x29, 0x06, 
	0xe8, 0x2c, 0xb8, 0xee, 0x6a, 0x08, 0x07, 0xbc, 0xf3, 0x3d, 0x3c, 0x44, 0x83, 0xd9, 0x9d, 0xf5, 
	0xc3, 0xab, 0xd7, 0xbc, 0x42, 0x0c, 0x94, 0x3c, 0x4f, 0x68, 0xcd, 0x72, 0x4f, 0x40, 0xd6, 0x8c, 
	0x99, 0x6a, 0xd2, 0xa3, 0xf4, 0x2b, 0x71, 0xcd, 0x5b, 0xde, 0xb2, 0x3e, 0xdb, 0x97, 0xe6, 0x91, 
	0x2c, 0x81, 0x76, 0xa8, 0xf5, 0x18, 0xa0, 0xff, 0xdd, 0x9d, 0x3d, 0xe7, 0x66, 0xe3, 0x1b, 0xc7, 
	0xa1, 0x66, 0xcf, 0xbd, 0xfc, 0x3c, 0xa8, 0xea, 0xc7, 0x43, 0xb9, 0xdb, 0xa3, 0xf8, 0x1e, 0x9a, 
	0xfe, 0xdb, 0x9d, 0xad, 0x75, 0xdd, 0xe4, 0xd0, 0xa4, 0x91, 0xf2, 0x04, 0x03, 0x26, 0x1b, 0x6c, 
	0xe8, 0x41, 0x1f, 0xf6, 0xb0, 0xf1, 0xff, 0xa5, 0xb3, 0x35, 0x8f, 0xf7, 0x0b, 0xc6, 0xe5, 0x95, 
	0xde, 0x9d, 0xbe, 0x47, 0x19, 0x5d, 0x6d, 0x9d, 0x32, 0x05, 0x9e, 0x06, 0xf6, 0xcb, 0xf8, 0xe2, 
	0x15, 0x08, 0x2d, 0x85, 0x2b, 0xd0, 0xa0, 0x69, 0x69, 0xb5, 0x70, 0x5a, 0xa9, 0xff, 0x91, 0xd7, 
	0x76, 0xd4, 0xf1, 0xf2, 0xfc, 0xc6, 0x31, 0x49, 0xa9, 0x91, 0xf4, 0x27, 0x5a, 0xb9, 0xd1, 0xa1, 
	0xf5, 0x59, 0x6c, 0xec, 0xa5, 0x3b, 0x9e, 0x51, 0x8b, 0x5a, 0x39, 0xc7, 0x3f, 0xe3, 0x62, 0x6f, 
	0x65, 0x5c, 0x16, 0x0c, 0x6a, 0x32, 0x02, 0x66, 0x2c, 0xd5, 0x5e, 0x17, 0xd9, 0xe5, 0xe8, 0x0c, 
	0x33, 0x9c, 0x28, 0x61, 0x36, 0x6e, 0x67, 0xfc, 0xa9, 0x74, 0x45, 0x11, 0xb5, 0xe0, 0xea, 0xaa, 
	0x79, 0xa5, 0xbe, 0x6c, 0xc8, 0xda, 0x94, 0x5e, 0xb9, 0xd6, 0x94, 0x96, 0xde, 0xd0, 0x90, 0xcc, 
	0x78, 0xd9, 0x06, 0x4c, 0xba, 0x56, 0xc8, 0x45, 0xb6, 0xc7, 0x5e, 0x39, 0xb0, 0xfc, 0x4e, 0x17, 
	0x30, 0x0a, 0x6a, 0x6f, 0x9e, 0xc0, 0xa4, 0x4c, 0x64, 0xb8, 0x03, 0x35, 0xd7, 0x58, 0xd0, 0xeb, 
	0x8b, 0x2d, 0xfe, 0x33, 0xd9, 0x0e, 0x57, 0xd4, 0x6b, 0x08, 0xfd, 0x6e, 0x7c, 0x4f, 0xa6, 0x61, 
	0x0b, 0x06, 0x31, 0xa8, 0xfd, 0x0a, 0x00, 0x00, 0x44, 0x1c, 0x36, 0x9e, 0x00, 0x01, 0xaf, 0x07, 
	0xd0, 0x0e, 0x00, 0x00, 0x65, 0x3f, 0xf5, 0xb5, 0x3e, 0x30, 0x0d, 0x8b, 0x02, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x59, 0x5a, 
};

const SMS_LZMA2Data g_JPEGData[ 5 ] __attribute__(   (  section( ".rodata" )  )   ) = {
 { 2016, sizeof ( s_IDCT  ), s_IDCT  },
 { 1232, sizeof ( s_CSC1  ), s_CSC1  },
 { 2128, sizeof ( s_CSC2h ), s_CSC2h },
 { 1632, sizeof ( s_CSC2v ), s_CSC2v },
 { 1872, sizeof ( s_CSC4  ), s_CSC4  }
};
