#ifndef use_TINY_IRR_XML
#define use_TINY_IRR_XML

#include <tinyXML.h>
#include <irrlicht.h>

struct TiIrrDocument : public TiXmlDocument{
  //! loads a tinyXML document from the Irrlicht file system (also from zip archives),
  bool LoadFile(irr::io::IFileSystem* FileSystem, const char* filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING){
    bool loadOkay = false;
    irr::io::IReadFile* file = FileSystem->createAndOpenFile(filename); // open file
    if(file){
      void* mem = malloc(file->getSize());                    // preserve memory
      file->read(mem, file->getSize());                       // load file into memory
      file->drop();                                           // close (drop) file
      loadOkay = this->Parse((const char*)mem, 0, encoding);  // parse memory
      if(mem) free(mem);                                      // free memory
    }
    return loadOkay;
  }
};

#endif // use_TINY_IRR_XML
