/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef QtSlicer_h
#define QtSlicer_h

#include "QtImageViewer_Export.h"

//Qt include
#include <QDialog>

//itk include
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "ui_QtSlicerGUI.h"

class QtImageViewer_EXPORT QtSlicer : public QDialog, public Ui::GuiDialogBase
{
  Q_OBJECT;
  Q_ENUMS(displayDetailsState);
public:
  typedef QDialog Superclass;

  QtSlicer( QWidget* parent = 0,
            Qt::WindowFlags fl = Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
  virtual ~QtSlicer();

  enum DisplayStates{
    OFF = 0x00,
    ON_SLICEVIEW = 0x01,
    ON_TEXTBOX = 0x02,
    OFF_COLLAPSE = 0x04,
    ON_COLLAPSE = 0x08
  };

  typedef itk::Image<double,3>                ImageType;
  typedef unsigned char                       OverlayPixelType;
  typedef itk::Image<OverlayPixelType,3>      OverlayType;

  bool loadOverlayImage(QString overlayImagePath = QString());
  bool loadInputImage(QString filePathTLoad = QString());

public slots:
  void setDisplayState(int details);
  void hideHelp();
  void showHelp(bool checked = true);
  void updateSliceMaximum();
  void setInputImage(ImageType * newImData);
  void setOverlayImage(OverlayType * newImData);

protected:
  QDialog* HelpDialog;
  bool IsRedirectingEvent;

  /// Reimplemented to propagate key events to QtGlSliceView.
  virtual void keyPressEvent(QKeyEvent* event);
  virtual bool eventFilter(QObject* obj, QEvent* event);

  /// Resize the entire dialog based on the current size and to ensure it fits
  /// the contents.
  /// \sa QWidget::resize(), QWidget::adjustSize()
  void updateSize();
};

#endif
