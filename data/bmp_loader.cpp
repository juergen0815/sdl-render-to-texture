/*
 * bmp_loader.cpp
 *
 *  Created on: 2013-01-24
 *      Author: jurgens
 */

#include "bmp_loader.h"

#include <err.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace bfs  = boost::filesystem;

BmpBrush::BmpBrush()
{
    m_Width = 0;
    m_Height = 0;
    m_BytesPerPixel = 0; /* 3:RGB, 4:RGBA */
    m_Pixels = nullptr;
}

BmpBrush::~BmpBrush()
{

}

bool BmpBrush::Load( const char* filename )
{
#pragma pack( push, 2 )
    struct BmpHeader
    {
        // BitmapHeader:
        unsigned short  magic;          // format magic, always 'BM'
        unsigned int    filesize;       // total file size, inc. headers
        unsigned int    reserved;       // for future use
        unsigned int    data_offset;    // image data offset in the file

        // BitmapInfoHeader:
        unsigned int    bih_size;       // 2nd part's size
        unsigned int    width;          // bitmap's dimensions
        unsigned int    height;
        unsigned short  planes;         // num of planes
        unsigned short  bpp;            // bits per pixel
        unsigned int    compression;    // compression method
        unsigned int    size_of_bmp;    // size of the bitmap
        unsigned int    h_res;
        unsigned int    v_res;          // image resolution in dpi
        unsigned int    num_clrs;       // number of colors used
        unsigned int    num_signif_clrs;// number of significant colors
    };
#pragma pack( pop )

    if (!m_FileHandle.is_open() ) {
        this->m_Pixels = nullptr;
        m_FileHandle.open( filename, std::ios_base::binary | std::ios_base::in );
        if ( m_FileHandle.is_open() && m_FileHandle.size() >= sizeof(BmpHeader) ) {
            const BmpHeader* bmp = (const BmpHeader*)m_FileHandle.const_data();
            if ( bmp->magic == 0x4D42 ) // == (unsigned short)'MB'
            {
                this->m_Width  = bmp->width;
                this->m_Height = bmp->height;
                if ( bmp->planes == 1 &&
                     (bmp->bpp == 24 || bmp->bpp == 32) &&
                     bmp->compression == 0 )
                {
                    this->m_BytesPerPixel = (bmp->bpp >> 3);
                    this->m_Pixels = ((const char*)bmp) + bmp->data_offset;
                    return true;
                } else {
//                    ASSERT( true, "Unsupported format. Cannot load BMP (%s)!", filename );
                }
            } else {
//                ASSERT( true, "File '%s' is not a BMP format!", filename );
            }
        }
//        ASSERT( true, "File open error! Invalid file or file size! (%s)", filename );
    }
    return this->m_Pixels != nullptr;
}

