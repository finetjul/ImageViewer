/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindow.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <fstream>

#if    defined(_WIN32)
#include <itkWindows.h>
#endif

#include <fltkGlWindow.h>
#include <FL/fl_ask.H>
#include <FL/fl_file_chooser.H>


namespace fltk {


//--------------------------------------------------
//
//    Creator
//
//--------------------------------------------------
GlWindow::GlWindow(int x,int y,int w,int h, const char * label)
  :Fl_Gl_Window(x,y,w,h,label) 
{
  m_RedrawCommand = RedrawCommandType::New();
  m_RedrawCommand->SetWidget( this );
  m_Notifier   = itk::Object::New();
}


//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
GlWindow::~GlWindow() 
{

}



//--------------------------------------------------
//
//    Grab Image and Store in a PPM file 
//    This is the user interface part.
//
//--------------------------------------------------
void GlWindow::SaveImage(void) 
{

  const char * filename = 
        fl_file_chooser("Please select a file name","*.ppm","");

  if( !filename ) return;

  SaveImage( filename );

}



//------------------------------------------
//
//    Return Redraw Command
//
//------------------------------------------
GlWindow::RedrawCommandType *
GlWindow
::GetRedrawCommand(  void )
{
  
  return m_RedrawCommand.GetPointer();

}

 
//------------------------------------------
//
//    Return Notifier Object
//    this is used by observers interested
//    in events generated by GlWindow
//
//------------------------------------------
itk::Object *
GlWindow::GetNotifier(  void )
{
  return m_Notifier.GetPointer();
}


  


//--------------------------------------------------
//
//    Grab Image and Store in a PPM file
//
//--------------------------------------------------
void GlWindow::SaveImage(const char * filename) 
{

  if( !filename ) 
  {
    return;
  }

  if( strlen(filename) == 0 ) 
  {
    return;
  }

  const int wnx = w();
  const int wny = h();

  

  std::ofstream  of;
  of.open(filename);
  if( of.fail() ) 
  {
    fl_alert("Error opening file %s",filename);
    return;
  }

  unsigned char *image = new unsigned char[ wnx * wny * 8 ];

  if( !image ) 
  {
    fl_alert("Problem Allocating memory for image grabbing buffer");
    of.close();
    return;
  }


  of << "P6" << std::endl;      // Magic number for PPM files
  of << wnx << " " << wny << std::endl;

  of << 255 << std::endl;

  make_current();
  draw();

  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,wnx,wny,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid *)image);

  for(int y=wny-1; y>=0; y--)  
  {
    unsigned char *p = image + 4*y*wnx;
    for(int x=0; x<wnx; x++) 
    {
      const unsigned char red   = *p++;
      const unsigned char green = *p++;
      const unsigned char blue  = *p++;
                                   p++; // alpha channel
      of << red << green << blue;
    }
  }


  of.close();

  delete [] image;
  
  
}



} // end namespace fltk


