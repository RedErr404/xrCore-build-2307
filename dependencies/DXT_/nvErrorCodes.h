#pragma once

typedef enum NV_ERROR_CODE
{
    NV_OK                   =   0,
    NV_FAIL                 =  -1, // generic
    NV_CANT_OPEN_INPUT_FILE    =  -2,
    NV_CANT_OPEN_OUTPUT_FILE   =  -3,
    NV_BAD_OPTION              =  -4,
    NV_BAD_OUTPUT_DIRECTORY    =  -5,
    NV_CANT_DECOMPRESS_IMAGE   =  -6,
    NV_BAD_LIST_FILE           =  -7,
    NV_CANT_OPEN_LIST_FILE     =  -8,
    NV_BAD_LIST_FILE_CONTENTS  =  -9,
    NV_IMAGE_NOT_SQUARE        = -10,
    NV_IMAGES_NOT_SAME_SIZE    = -11,    
    NV_UNKNOWN_CONVERSION      = -12,       // for normal map generation
    NV_CANT_OPEN_PROFILE       = -13,
    NV_IMAGE_NOT_MULT4         = -14,
    NV_OUTPUT_FILE_IS_READ_ONLY= -15,
    NV_INPUT_POINTER_ZERO      = -16,
    NV_DEPTH_IS_NOT_3_OR_4     = -17,
    NV_IMAGE_NOT_POWER_2       = -18,
    NV_CANT_MAKE_MATRIX        = -19,
    NV_CANT_NORMALIZE          = -20,
    NV_CANT_INVERT_MATRIX      = -21,
    NV_CANT_MAKE_VECTOR        = -22,
    NV_CANT_INITIALIZE_CONVOLVER = -23,
    NV_EMPTY_IMAGE             = -24,
    NV_BAD_FLAGS               = -25,
    NV_GBM_IO_ERR              = -26,
    NV_TGA_IO_ERR              = -27,
    NV_TGA_RLE_ERR             = -28,
    NV_PNG_IO_ERR              = -29,
    NV_CANT_CREATE_INDEX_BUFFER= -30,
    NV_CANT_CREATE_CUBE_MAP    = -31,
    NV_BAD_FORMAT              = -32,
    NV_CANT_CREATE_TEXTURE     = -33,
    NV_UNKNOWN_FORMAT          = -34,
    NV_CANT_CREATE_VERTEX_BUFFER = -35,
    NV_CANT_CREATE_PIXEL_SHADER= -36,
    NV_CANT_CREATE_VERTEX_DECL = -37,
    NV_NEED_4_PLANES_FOR_RGBE  = -38,
    NV_PSD_IO_ERR              = -39,
    NV_NO_FILENAME             = -40,

    NV_CANT_DECOMPOSE_MATRIX   = -41,
    NV_HDR_IO_ERR              = -42,
    NV_RGBA_IO_ERR             = -43,
    NV_READ_FAILED             = -44,
    NV_WRITE_FAILED            = -45,
    NV_BAD_ARG                 = -46,
    NV_CANT_LOCK               = -47,
    NV_FAILED_UNLOCK           = -48,
    NV_NOT_RING_LOOP           = -49,

    NV_CHART_FOLDED            = -50, 
    NV_CHART_CANNOT_INVERT     = -51,
    NV_CHART_BAD_PERIMETER     = -52,
    NV_CHART_NO_DATA           = -53,
    NV_CHART_CANT_OPTIMIZE     = -54,
    NV_CHART_NO_POINTS_FOUND   = -55,
    NV_CHART_ZERO_SIZE         = -56,
    NV_CHART_NO_UNPINNED       = -57,
    NV_CHART_OVERLAP           = -58,
    NV_CHART_NO_FACES          = -59,
    NV_CHART_MAX_DISTORTION_EXCEEDED = -60,
    NV_CHART_AVE_DISTORTION_EXCEEDED = -61,

    NV_CANT_CREATE_PRECONDITIONER = -70,
    NV_NO_CONVERGENCE           = -71,
    NV_ZERO_DIAGONAL            = -72,
    NV_ZERO_TRIANGLE_AREA       = -73,
    NV_LINE_ON_PLANE            = -74,
    NV_LINE_AND_PLANE_PARALLEL  = -75,
    NV_CANT_COMPILE_EFFECT_FILE = -76,
                                               // open
    NV_CANT_UNLOCK_INDEX_BUFFER = -78,
    NV_CANT_CLONE_MESH          = -79,
    NV_CANT_FIND_FILE           = -80,
    NV_INVALID_FILENAME         = -81,
    NV_CANT_SET_MATRIX          = -82,
    NV_NEED_PIXEL_SHADER2       = -83,
    NV_CANT_UNLOCK_TEXTURE      = -84,
    NV_OUT_OF_MEMORY            = -85,
    NV_TIFF_IO_ERR             =  -86,


    NV_FILE_BAD_INDEX           = -90,
    NV_FILE_NOT_ENOUGH_INDICES  = -91,
    NV_FILE_FORMAT_NOT_SUPPORTED= -92,
    NV_CANT_CREATE_EFFECT       = -93,
    NV_NAN                      = -94,

    NV_SURFACE_IS_CUBE_MAP         = -95,
    NV_SURFACE_IS_VOLUME_MAP       = -96,
    NV_UNSUPPORTED_FORMAT        = -97,
    NV_MEMORY_ALLOCATION_FAILED        = -98,
    NV_CUBE_MAP_NEEDS_SIX_FACES  = -99,
    NV_IMAGE_EXCEEDS_INTERNAL_SIZE  = -100,
    NV_CUBE_MAP_WIDTH_IS_NOT_SIX_TIMES_HEIGHT  = -101,
    NV_VOLUME_MAP_IS_NOT_POWER2  = -102,




};

 const char *getErrorString(NV_ERROR_CODE hr);