/*
 * bmp_loader.h
 *
 *  Created on: 2013-01-24
 *      Author: jurgens
 */

#ifndef BMP_LOADER_H_
#define BMP_LOADER_H_

#include <brush.h>

#include <boost/iostreams/device/mapped_file.hpp>
namespace bios = boost::iostreams;

class BmpBrush : public Brush
{
    bios::mapped_file m_FileHandle;
public:
    BmpBrush();

    ~BmpBrush();

    bool Load( const char* filename );

};


#endif /* BMP_LOADER_H_ */
