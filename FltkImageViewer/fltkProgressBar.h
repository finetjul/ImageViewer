/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkProgressBar.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/


#ifndef __fltk_ProgressBar_h
#define __fltk_ProgressBar_h

#include "itkCommand.h"
#include "fltkCommandEvents.h"
#include <FL/Fl_Slider.H>


namespace fltk {

/**
 *  GUI object that derives from a FLTK Slider and
 *  implements a progress barr.
 *
 *  It contains an Update Command object compatible
 *  with ITK Observer Pattern
 */
class ProgressBar : public Fl_Slider
{
public:

  /**
   * Command Class invoked for button redraw
   */
  typedef itk::MemberCommand< ProgressBar >  RedrawCommandType;


  /**
   * Constructor
   */
  ProgressBar(int x, int y, int w, int h, char * label=0):
    Fl_Slider( x, y, w, h, label ) {
      m_RedrawCommand = RedrawCommandType::New();
      m_RedrawCommand->SetCallbackFunction( this, &ProgressBar::ProcessEvent );
      m_RedrawCommand->SetCallbackFunction( this, &ProgressBar::ConstProcessEvent );
    }


  
  /**
   * Get Command
   */
  RedrawCommandType::Pointer GetRedrawCommand( void ) const
  {
    return m_RedrawCommand.GetPointer();
  }

  
  /**
   * Manage a Progress event
   */
  void ProcessEvent(itk::Object * caller, const itk::EventObject & event )
  {
    if( typeid( itk::ProgressEvent )   ==  typeid( event ) )
      {
      itk::ProcessObject::Pointer  process = 
                 dynamic_cast< itk::ProcessObject *>( caller );
      this->value( process->GetProgress() );
      this->redraw();
      }
  }

  void ConstProcessEvent(const itk::Object * caller, const itk::EventObject & event )
  {
    if( typeid( itk::ProgressEvent )   ==  typeid( event ) ) 
      {
      itk::ProcessObject::ConstPointer  process = 
                 dynamic_cast< const itk::ProcessObject *>( caller );
      this->value( process->GetProgress() );
      this->redraw();
      }
  }
 

  /**
   * Manage a Progress event
   */
  void Observe( itk::Object *caller )
  {
    caller->AddObserver(  itk::ProgressEvent(), m_RedrawCommand.GetPointer() );
  }

private:

  RedrawCommandType::Pointer m_RedrawCommand;
  
};


} // end namespace fltk


#endif
