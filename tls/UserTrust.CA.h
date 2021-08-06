static unsigned int USERTRUST_CA_LEN = 1506;
static unsigned char USERTRUST_CA_DER[] = {
	0x30, 0x82, 0x05, 0xde, 0x30, 0x82, 0x03, 0xc6, 0xa0, 0x03, 0x02, 0x01,
	0x02, 0x02, 0x10, 0x01, 0xfd, 0x6d, 0x30, 0xfc, 0xa3, 0xca, 0x51, 0xa8,
	0x1b, 0xbc, 0x64, 0x0e, 0x35, 0x03, 0x2d, 0x30, 0x0d, 0x06, 0x09, 0x2a,
	0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0c, 0x05, 0x00, 0x30, 0x81,
	0x88, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
	0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13,
	0x0a, 0x4e, 0x65, 0x77, 0x20, 0x4a, 0x65, 0x72, 0x73, 0x65, 0x79, 0x31,
	0x14, 0x30, 0x12, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x0b, 0x4a, 0x65,
	0x72, 0x73, 0x65, 0x79, 0x20, 0x43, 0x69, 0x74, 0x79, 0x31, 0x1e, 0x30,
	0x1c, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x15, 0x54, 0x68, 0x65, 0x20,
	0x55, 0x53, 0x45, 0x52, 0x54, 0x52, 0x55, 0x53, 0x54, 0x20, 0x4e, 0x65,
	0x74, 0x77, 0x6f, 0x72, 0x6b, 0x31, 0x2e, 0x30, 0x2c, 0x06, 0x03, 0x55,
	0x04, 0x03, 0x13, 0x25, 0x55, 0x53, 0x45, 0x52, 0x54, 0x72, 0x75, 0x73,
	0x74, 0x20, 0x52, 0x53, 0x41, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66,
	0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74, 0x68,
	0x6f, 0x72, 0x69, 0x74, 0x79, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x30, 0x30,
	0x32, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5a, 0x17, 0x0d,
	0x33, 0x38, 0x30, 0x31, 0x31, 0x38, 0x32, 0x33, 0x35, 0x39, 0x35, 0x39,
	0x5a, 0x30, 0x81, 0x88, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
	0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55,
	0x04, 0x08, 0x13, 0x0a, 0x4e, 0x65, 0x77, 0x20, 0x4a, 0x65, 0x72, 0x73,
	0x65, 0x79, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13,
	0x0b, 0x4a, 0x65, 0x72, 0x73, 0x65, 0x79, 0x20, 0x43, 0x69, 0x74, 0x79,
	0x31, 0x1e, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x15, 0x54,
	0x68, 0x65, 0x20, 0x55, 0x53, 0x45, 0x52, 0x54, 0x52, 0x55, 0x53, 0x54,
	0x20, 0x4e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x31, 0x2e, 0x30, 0x2c,
	0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x25, 0x55, 0x53, 0x45, 0x52, 0x54,
	0x72, 0x75, 0x73, 0x74, 0x20, 0x52, 0x53, 0x41, 0x20, 0x43, 0x65, 0x72,
	0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41,
	0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x30, 0x82, 0x02, 0x22,
	0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
	0x01, 0x05, 0x00, 0x03, 0x82, 0x02, 0x0f, 0x00, 0x30, 0x82, 0x02, 0x0a,
	0x02, 0x82, 0x02, 0x01, 0x00, 0x80, 0x12, 0x65, 0x17, 0x36, 0x0e, 0xc3,
	0xdb, 0x08, 0xb3, 0xd0, 0xac, 0x57, 0x0d, 0x76, 0xed, 0xcd, 0x27, 0xd3,
	0x4c, 0xad, 0x50, 0x83, 0x61, 0xe2, 0xaa, 0x20, 0x4d, 0x09, 0x2d, 0x64,
	0x09, 0xdc, 0xce, 0x89, 0x9f, 0xcc, 0x3d, 0xa9, 0xec, 0xf6, 0xcf, 0xc1,
	0xdc, 0xf1, 0xd3, 0xb1, 0xd6, 0x7b, 0x37, 0x28, 0x11, 0x2b, 0x47, 0xda,
	0x39, 0xc6, 0xbc, 0x3a, 0x19, 0xb4, 0x5f, 0xa6, 0xbd, 0x7d, 0x9d, 0xa3,
	0x63, 0x42, 0xb6, 0x76, 0xf2, 0xa9, 0x3b, 0x2b, 0x91, 0xf8, 0xe2, 0x6f,
	0xd0, 0xec, 0x16, 0x20, 0x90, 0x09, 0x3e, 0xe2, 0xe8, 0x74, 0xc9, 0x18,
	0xb4, 0x91, 0xd4, 0x62, 0x64, 0xdb, 0x7f, 0xa3, 0x06, 0xf1, 0x88, 0x18,
	0x6a, 0x90, 0x22, 0x3c, 0xbc, 0xfe, 0x13, 0xf0, 0x87, 0x14, 0x7b, 0xf6,
	0xe4, 0x1f, 0x8e, 0xd4, 0xe4, 0x51, 0xc6, 0x11, 0x67, 0x46, 0x08, 0x51,
	0xcb, 0x86, 0x14, 0x54, 0x3f, 0xbc, 0x33, 0xfe, 0x7e, 0x6c, 0x9c, 0xff,
	0x16, 0x9d, 0x18, 0xbd, 0x51, 0x8e, 0x35, 0xa6, 0xa7, 0x66, 0xc8, 0x72,
	0x67, 0xdb, 0x21, 0x66, 0xb1, 0xd4, 0x9b, 0x78, 0x03, 0xc0, 0x50, 0x3a,
	0xe8, 0xcc, 0xf0, 0xdc, 0xbc, 0x9e, 0x4c, 0xfe, 0xaf, 0x05, 0x96, 0x35,
	0x1f, 0x57, 0x5a, 0xb7, 0xff, 0xce, 0xf9, 0x3d, 0xb7, 0x2c, 0xb6, 0xf6,
	0x54, 0xdd, 0xc8, 0xe7, 0x12, 0x3a, 0x4d, 0xae, 0x4c, 0x8a, 0xb7, 0x5c,
	0x9a, 0xb4, 0xb7, 0x20, 0x3d, 0xca, 0x7f, 0x22, 0x34, 0xae, 0x7e, 0x3b,
	0x68, 0x66, 0x01, 0x44, 0xe7, 0x01, 0x4e, 0x46, 0x53, 0x9b, 0x33, 0x60,
	0xf7, 0x94, 0xbe, 0x53, 0x37, 0x90, 0x73, 0x43, 0xf3, 0x32, 0xc3, 0x53,
	0xef, 0xdb, 0xaa, 0xfe, 0x74, 0x4e, 0x69, 0xc7, 0x6b, 0x8c, 0x60, 0x93,
	0xde, 0xc4, 0xc7, 0x0c, 0xdf, 0xe1, 0x32, 0xae, 0xcc, 0x93, 0x3b, 0x51,
	0x78, 0x95, 0x67, 0x8b, 0xee, 0x3d, 0x56, 0xfe, 0x0c, 0xd0, 0x69, 0x0f,
	0x1b, 0x0f, 0xf3, 0x25, 0x26, 0x6b, 0x33, 0x6d, 0xf7, 0x6e, 0x47, 0xfa,
	0x73, 0x43, 0xe5, 0x7e, 0x0e, 0xa5, 0x66, 0xb1, 0x29, 0x7c, 0x32, 0x84,
	0x63, 0x55, 0x89, 0xc4, 0x0d, 0xc1, 0x93, 0x54, 0x30, 0x19, 0x13, 0xac,
	0xd3, 0x7d, 0x37, 0xa7, 0xeb, 0x5d, 0x3a, 0x6c, 0x35, 0x5c, 0xdb, 0x41,
	0xd7, 0x12, 0xda, 0xa9, 0x49, 0x0b, 0xdf, 0xd8, 0x80, 0x8a, 0x09, 0x93,
	0x62, 0x8e, 0xb5, 0x66, 0xcf, 0x25, 0x88, 0xcd, 0x84, 0xb8, 0xb1, 0x3f,
	0xa4, 0x39, 0x0f, 0xd9, 0x02, 0x9e, 0xeb, 0x12, 0x4c, 0x95, 0x7c, 0xf3,
	0x6b, 0x05, 0xa9, 0x5e, 0x16, 0x83, 0xcc, 0xb8, 0x67, 0xe2, 0xe8, 0x13,
	0x9d, 0xcc, 0x5b, 0x82, 0xd3, 0x4c, 0xb3, 0xed, 0x5b, 0xff, 0xde, 0xe5,
	0x73, 0xac, 0x23, 0x3b, 0x2d, 0x00, 0xbf, 0x35, 0x55, 0x74, 0x09, 0x49,
	0xd8, 0x49, 0x58, 0x1a, 0x7f, 0x92, 0x36, 0xe6, 0x51, 0x92, 0x0e, 0xf3,
	0x26, 0x7d, 0x1c, 0x4d, 0x17, 0xbc, 0xc9, 0xec, 0x43, 0x26, 0xd0, 0xbf,
	0x41, 0x5f, 0x40, 0xa9, 0x44, 0x44, 0xf4, 0x99, 0xe7, 0x57, 0x87, 0x9e,
	0x50, 0x1f, 0x57, 0x54, 0xa8, 0x3e, 0xfd, 0x74, 0x63, 0x2f, 0xb1, 0x50,
	0x65, 0x09, 0xe6, 0x58, 0x42, 0x2e, 0x43, 0x1a, 0x4c, 0xb4, 0xf0, 0x25,
	0x47, 0x59, 0xfa, 0x04, 0x1e, 0x93, 0xd4, 0x26, 0x46, 0x4a, 0x50, 0x81,
	0xb2, 0xde, 0xbe, 0x78, 0xb7, 0xfc, 0x67, 0x15, 0xe1, 0xc9, 0x57, 0x84,
	0x1e, 0x0f, 0x63, 0xd6, 0xe9, 0x62, 0xba, 0xd6, 0x5f, 0x55, 0x2e, 0xea,
	0x5c, 0xc6, 0x28, 0x08, 0x04, 0x25, 0x39, 0xb8, 0x0e, 0x2b, 0xa9, 0xf2,
	0x4c, 0x97, 0x1c, 0x07, 0x3f, 0x0d, 0x52, 0xf5, 0xed, 0xef, 0x2f, 0x82,
	0x0f, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x42, 0x30, 0x40, 0x30, 0x1d,
	0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x53, 0x79, 0xbf,
	0x5a, 0xaa, 0x2b, 0x4a, 0xcf, 0x54, 0x80, 0xe1, 0xd8, 0x9b, 0xc0, 0x9d,
	0xf2, 0xb2, 0x03, 0x66, 0xcb, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f,
	0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x01, 0x06, 0x30, 0x0f, 0x06,
	0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01,
	0x01, 0xff, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
	0x01, 0x01, 0x0c, 0x05, 0x00, 0x03, 0x82, 0x02, 0x01, 0x00, 0x5c, 0xd4,
	0x7c, 0x0d, 0xcf, 0xf7, 0x01, 0x7d, 0x41, 0x99, 0x65, 0x0c, 0x73, 0xc5,
	0x52, 0x9f, 0xcb, 0xf8, 0xcf, 0x99, 0x06, 0x7f, 0x1b, 0xda, 0x43, 0x15,
	0x9f, 0x9e, 0x02, 0x55, 0x57, 0x96, 0x14, 0xf1, 0x52, 0x3c, 0x27, 0x87,
	0x94, 0x28, 0xed, 0x1f, 0x3a, 0x01, 0x37, 0xa2, 0x76, 0xfc, 0x53, 0x50,
	0xc0, 0x84, 0x9b, 0xc6, 0x6b, 0x4e, 0xba, 0x8c, 0x21, 0x4f, 0xa2, 0x8e,
	0x55, 0x62, 0x91, 0xf3, 0x69, 0x15, 0xd8, 0xbc, 0x88, 0xe3, 0xc4, 0xaa,
	0x0b, 0xfd, 0xef, 0xa8, 0xe9, 0x4b, 0x55, 0x2a, 0x06, 0x20, 0x6d, 0x55,
	0x78, 0x29, 0x19, 0xee, 0x5f, 0x30, 0x5c, 0x4b, 0x24, 0x11, 0x55, 0xff,
	0x24, 0x9a, 0x6e, 0x5e, 0x2a, 0x2b, 0xee, 0x0b, 0x4d, 0x9f, 0x7f, 0xf7,
	0x01, 0x38, 0x94, 0x14, 0x95, 0x43, 0x07, 0x09, 0xfb, 0x60, 0xa9, 0xee,
	0x1c, 0xab, 0x12, 0x8c, 0xa0, 0x9a, 0x5e, 0xa7, 0x98, 0x6a, 0x59, 0x6d,
	0x8b, 0x3f, 0x08, 0xfb, 0xc8, 0xd1, 0x45, 0xaf, 0x18, 0x15, 0x64, 0x90,
	0x12, 0x0f, 0x73, 0x28, 0x2e, 0xc5, 0xe2, 0x24, 0x4e, 0xfc, 0x58, 0xec,
	0xf0, 0xf4, 0x45, 0xfe, 0x22, 0xb3, 0xeb, 0x2f, 0x8e, 0xd2, 0xd9, 0x45,
	0x61, 0x05, 0xc1, 0x97, 0x6f, 0xa8, 0x76, 0x72, 0x8f, 0x8b, 0x8c, 0x36,
	0xaf, 0xbf, 0x0d, 0x05, 0xce, 0x71, 0x8d, 0xe6, 0xa6, 0x6f, 0x1f, 0x6c,
	0xa6, 0x71, 0x62, 0xc5, 0xd8, 0xd0, 0x83, 0x72, 0x0c, 0xf1, 0x67, 0x11,
	0x89, 0x0c, 0x9c, 0x13, 0x4c, 0x72, 0x34, 0xdf, 0xbc, 0xd5, 0x71, 0xdf,
	0xaa, 0x71, 0xdd, 0xe1, 0xb9, 0x6c, 0x8c, 0x3c, 0x12, 0x5d, 0x65, 0xda,
	0xbd, 0x57, 0x12, 0xb6, 0x43, 0x6b, 0xff, 0xe5, 0xde, 0x4d, 0x66, 0x11,
	0x51, 0xcf, 0x99, 0xae, 0xec, 0x17, 0xb6, 0xe8, 0x71, 0x91, 0x8c, 0xde,
	0x49, 0xfe, 0xdd, 0x35, 0x71, 0xa2, 0x15, 0x27, 0x94, 0x1c, 0xcf, 0x61,
	0xe3, 0x26, 0xbb, 0x6f, 0xa3, 0x67, 0x25, 0x21, 0x5d, 0xe6, 0xdd, 0x1d,
	0x0b, 0x2e, 0x68, 0x1b, 0x3b, 0x82, 0xaf, 0xec, 0x83, 0x67, 0x85, 0xd4,
	0x98, 0x51, 0x74, 0xb1, 0xb9, 0x99, 0x80, 0x89, 0xff, 0x7f, 0x78, 0x19,
	0x5c, 0x79, 0x4a, 0x60, 0x2e, 0x92, 0x40, 0xae, 0x4c, 0x37, 0x2a, 0x2c,
	0xc9, 0xc7, 0x62, 0xc8, 0x0e, 0x5d, 0xf7, 0x36, 0x5b, 0xca, 0xe0, 0x25,
	0x25, 0x01, 0xb4, 0xdd, 0x1a, 0x07, 0x9c, 0x77, 0x00, 0x3f, 0xd0, 0xdc,
	0xd5, 0xec, 0x3d, 0xd4, 0xfa, 0xbb, 0x3f, 0xcc, 0x85, 0xd6, 0x6f, 0x7f,
	0xa9, 0x2d, 0xdf, 0xb9, 0x02, 0xf7, 0xf5, 0x97, 0x9a, 0xb5, 0x35, 0xda,
	0xc3, 0x67, 0xb0, 0x87, 0x4a, 0xa9, 0x28, 0x9e, 0x23, 0x8e, 0xff, 0x5c,
	0x27, 0x6b, 0xe1, 0xb0, 0x4f, 0xf3, 0x07, 0xee, 0x00, 0x2e, 0xd4, 0x59,
	0x87, 0xcb, 0x52, 0x41, 0x95, 0xea, 0xf4, 0x47, 0xd7, 0xee, 0x64, 0x41,
	0x55, 0x7c, 0x8d, 0x59, 0x02, 0x95, 0xdd, 0x62, 0x9d, 0xc2, 0xb9, 0xee,
	0x5a, 0x28, 0x74, 0x84, 0xa5, 0x9b, 0xb7, 0x90, 0xc7, 0x0c, 0x07, 0xdf,
	0xf5, 0x89, 0x36, 0x74, 0x32, 0xd6, 0x28, 0xc1, 0xb0, 0xb0, 0x0b, 0xe0,
	0x9c, 0x4c, 0xc3, 0x1c, 0xd6, 0xfc, 0xe3, 0x69, 0xb5, 0x47, 0x46, 0x81,
	0x2f, 0xa2, 0x82, 0xab, 0xd3, 0x63, 0x44, 0x70, 0xc4, 0x8d, 0xff, 0x2d,
	0x33, 0xba, 0xad, 0x8f, 0x7b, 0xb5, 0x70, 0x88, 0xae, 0x3e, 0x19, 0xcf,
	0x40, 0x28, 0xd8, 0xfc, 0xc8, 0x90, 0xbb, 0x5d, 0x99, 0x22, 0xf5, 0x52,
	0xe6, 0x58, 0xc5, 0x1f, 0x88, 0x31, 0x43, 0xee, 0x88, 0x1d, 0xd7, 0xc6,
	0x8e, 0x3c, 0x43, 0x6a, 0x1d, 0xa7, 0x18, 0xde, 0x7d, 0x3d, 0x16, 0xf1,
	0x62, 0xf9, 0xca, 0x90, 0xa8, 0xfd
};